import ROOT as root
from rootpy.plotting import Canvas, Hist2D, Hist
import neriX_analysis
import neriX_datasets
import sys, os
import numpy as np
from scipy import stats

root.TVirtualFitter.SetMaxIterations(10000)

def make_string_for_1d_gaussian(electron_number, bAfterMainS2):
	# 0 = mean
	# 1 = width
	# 2 = first peak amplitude
	# 3-(1+num_electrons) = relative peak amplitudes


	string_func = '1./(2*3.14*%d*[1]**2.)**0.5*exp(-0.5/%.1f*((x - %.1f*[0])/[1])**2)' % (electron_number, electron_number, electron_number)
	#if bAfterMainS2:
	#	string_func += ' + [%d] + [%d]*exp(-x/[%d])' % (electron_number+4, electron_number+5, electron_number+6)
	
	if electron_number == 1:
		string_func = '[2]*' + string_func
	else:
		string_func = ('[%d]*[2]*' % (electron_number+1)) + string_func

	return '( %s )' % string_func


def make_string_for_2d_gaussian(electron_number, tot_mean):
	# 0 = bot mean percentage
	# 1 = bot width
	# 2 = top width
	# 3 = correlation
	# 4 = first peak ampl
	# 5-(3+num_electrons) = relative peak ampl

	string_func = '1./(2*3.14*[1]*[2]*%.1f*(1-[3]**2)**0.5)*exp( -0.5/(1-[3]**2)*( (x-%.1f*%.2f*[0])**2/[1]**2/%.1f + (y-%.1f*%.2f*(1-[0]))**2/[2]**2/%.1f - 2*[3]*(x-%.1f*%.2f*[0])*(y-%.1f*%.2f*(1-[0]))/[1]/[2]/%.1f ) )' % (electron_number, electron_number, tot_mean, electron_number, electron_number, tot_mean, electron_number, electron_number, tot_mean, electron_number, tot_mean, electron_number)
	if electron_number == 1:
		string_func = '[4]*' + string_func
	else:
		string_func = ('[%d]*[4]*' % (electron_number+3)) + string_func

	return '( %s )' % string_func




def compare_methods_s2_tot(filename, l_num_electrons_to_check, tot_threshold_low, tot_threshold_high):
	dum1, dum2, hS2Tot_afterS2 = fit_s2_tot_1d(filename, l_num_electrons_to_check, tot_threshold_low, tot_threshold_high, look_after_main=True, show_fits=False)
	dum1, dum2, hS2Tot_afterS1 = fit_s2_tot_1d(filename, l_num_electrons_to_check, tot_threshold_low, tot_threshold_high, look_after_main=False, show_fits=False)

	hS2Tot_afterS1.SetColor(root.kBlue)
	hS2Tot_afterS1.SetStats(0)
	hS2Tot_afterS2.SetColor(root.kRed)
	hS2Tot_afterS2.SetStats(0)

	c1 = Canvas()
	
	hS2Tot_afterS2.SetTitle('Gas Gain Method Comparison')
	hS2Tot_afterS2.GetXaxis().SetTitle('ctS2sTotBottom [PE]')
	hS2Tot_afterS2.GetYaxis().SetTitle('Counts')
	hS2Tot_afterS2.Draw()
	hS2Tot_afterS1.Draw('same')
	
	start_integral_value = 60
	integral_after_s2 = hS2Tot_afterS2.Integral(hS2Tot_afterS2.FindBin(start_integral_value), hS2Tot_afterS2.FindBin(tot_threshold_high)) / hS2Tot_afterS2.Integral()
	integral_after_s1 = hS2Tot_afterS1.Integral(hS2Tot_afterS1.FindBin(start_integral_value), hS2Tot_afterS1.FindBin(tot_threshold_high)) / hS2Tot_afterS1.Integral()
	
	print '\n\nPercentage of events after %.1f PE for post S2: %.5f' % (start_integral_value, integral_after_s2)
	print 'Percentage of events after %.1f PE for post S1: %.5f\n\n' % (start_integral_value, integral_after_s1)
	
	sAfterS2 = '%% post S2 events after %.1f PE: %.5f'  % (start_integral_value,  integral_after_s2)
	pt1 = root.TPaveText(.5,.75,.9,.8,'blNDC')
	text1 = pt1.AddText(sAfterS2)
	pt1.SetTextColor(root.kRed)
	pt1.SetFillStyle(0)
	pt1.SetBorderSize(0)
	pt1.Draw('same')

	sAfterS1 = '%% post S1 events after %.1f PE: %.5f'  % (start_integral_value,  integral_after_s1)
	pt2 = root.TPaveText(.5,.7,.9,.75,'blNDC')
	text2 = pt2.AddText(sAfterS1)
	pt2.SetTextColor(root.kBlue)
	pt2.SetFillStyle(0)
	pt2.SetBorderSize(0)
	pt2.Draw('same')
	
	c1.Update()

	raw_input('press enter')
	del c1




# fit s2total spectrum to determine parameters
# that can be fixed during 2d fit
# parameters we want: # electrons, fractional heights, mean_tot
def fit_s2_tot_1d(filename, l_num_electrons_to_check, tot_threshold_low, tot_threshold_high, look_after_main, show_fits=False):
	
	current_analysis = neriX_analysis.neriX_analysis(filename)

	aColors = [4, 2, 8, 7, 5, 9]
	aColors += aColors
	aColors += aColors
	aColors += aColors
	aColors += aColors
	aColors += aColors

	if look_after_main:
		tot_branch = 'ctS2sTotBottom[] + S2sTotTop[]'
		asym_branch = '(ctS2sTotBottom[] - S2sTotTop[])/(ctS2sTotBottom[] + S2sTotTop[])'
		width_branch = 'S2sRightEdge[] - S2sLeftEdge[]'
		height_branch = 'S2sHeight[]/(%s)' % tot_branch
		peak_branch = 'S2sPeak[]'
		left_edge_branch = 'S2sLeftEdge[]'
		right_edge_branch = 'S2sRightEdge[]'
	else:
		tot_branch = 'ctS2sTotBottom[S2Order[0]] + S2sTotTop[S2Order[0]]'
		asym_branch = '(ctS2sTotBottom[S2Order[0]] - S2sTotTop[S2Order[0]])/(ctS2sTotBottom[S2Order[0]] + S2sTotTop[S2Order[0]])'
		width_branch = 'S2sRightEdge[S2Order[0]] - S2sLeftEdge[S2Order[0]]'
		height_branch = 'S2sHeight[S2Order[0]]/(%s)' % tot_branch
		peak_branch = 'S2sPeak[S2Order[0]]'
		left_edge_branch = 'S2sLeftEdge[S2Order[0]]'
		right_edge_branch = 'S2sRightEdge[S2Order[0]]'

	s2TotBinning = [40, 0, tot_threshold_high]
	# iSampleWindow = [115,140] # run 15
	#iSampleWindow = [100, 200]  # after KNF
	iSampleWindow = [150,300] # run 10
	iS2Wdith = 300
	iSamplesBetweenS2 = 200
	iNumPMTsCoincidence = 0
	#iNumS2s = 2
	
	guessS2TotMean = 25
	guessS2TotWidth = 5
	limitsS2TotMean = [10, 45]
	limitsS2TotWidth = [5, 45]

	guessFDTotCenter = 15
	guessFDTotShape = 3
	limitsFDTotCenter = [0, 20]
	limitsFDTotShape = [1e-3, 5]

	asym_width = 1.5


	# --------------------------------------------
	# --------------------------------------------
    # Add cuts for analysis
	# --------------------------------------------
	# --------------------------------------------


	current_analysis.add_cut('%s > %f' % (tot_branch, tot_threshold_low))
	current_analysis.add_cut('%s < %f' % (tot_branch, s2TotBinning[2]))
	current_analysis.add_cut('log10((S1Tot + S2Tot)/(AreaTot - S1Tot - S2Tot)) > 0.')
	#current_analysis.add_cut('NbS2Peaks <= %d' % (iNumS2s))
	#current_analysis.add_radius_cut(0, 0.5)

	if look_after_main:
		#current_analysis.add_cut('S2sPeak[] - S2sPeak[0] > 1000')
		current_analysis.add_cut('S2sPeak[] - S1sPeak[0] > 1550 && S2sPeak[] - S1sPeak[0] < 1650')
		current_analysis.add_cut('S2sRightEdge[] - S2sLeftEdge[] < %i' % (iS2Wdith))
		current_analysis.add_cut('S2sCoin[] >= %d' % (iNumPMTsCoincidence))
		#current_analysis.add_cut('NbS2Peaks < 4')
		current_analysis.add_cut('Alt$(S2sLeftEdge[S2Order[$Iteration+1]],100000) - S2sRightEdge[S2Order[]] > %i' % (iSamplesBetweenS2))
		current_analysis.add_cut('S2sLeftEdge[S2Order[]] - Alt$(S2sRightEdge[S2Order[$Iteration-1]],-100000)> %i' % (iSamplesBetweenS2))
	else:
		current_analysis.add_cut('S2sPeak[S2Order[0]] > (S1sPeak[0] + %i) && S2sPeak[S2Order[0]] < (S1sPeak[0] + %i)' % (iSampleWindow[0], iSampleWindow[1]))
		current_analysis.add_cut('Alt$(S2sLeftEdge[S2Order[1]],100000) - S2sRightEdge[S2Order[0]] > %i' % (iSamplesBetweenS2))
		current_analysis.add_cut('S2Order[0] != 0')
		current_analysis.add_cut('S2sRightEdge[S2Order[0]] - S2sLeftEdge[S2Order[0]] < %i' % (iS2Wdith))
		current_analysis.add_cut('S2sCoin[S2Order[0]] >= %d' % (iNumPMTsCoincidence))


	current_analysis.set_event_list()

	
	#current_analysis.get_T1().Scan('EventId:%s:%s:%s:%s' % (tot_branch, peak_branch, left_edge_branch, right_edge_branch), current_analysis.get_cuts())


	# --------------------------------------------
	# --------------------------------------------
    # Fill Histogram
	# --------------------------------------------
	# --------------------------------------------


	hS2Tot = Hist(*s2TotBinning, name='hS2Tot', title='Total Gas Gain - V_{c} %.3f kV' % current_analysis.get_cathode_setting())
	hS2Tot.SetMarkerSize(0)
	current_analysis.Draw(tot_branch, hist=hS2Tot, selection=current_analysis.get_cuts())


	"""
	c2 = Canvas()
	hS2Asym = Hist(50, -1, 1, name='hS2Asym', title='hS2Asym')
	current_analysis.Draw(asym_branch, hist=hS2Asym, selection=current_analysis.get_cuts())
	hS2Asym.Draw()

	fAsymGaus = root.TF1('fAsymGaus', 'gaus', -1, 1)
	fAsymGaus.SetParameters(100, 0., 0.3)
	hS2Asym.Fit('fAsymGaus', '')
	
	c2.Update()
	raw_input('press enter')

	current_analysis.add_cut('(%s > %f) && (%s < %f)' % (asym_branch, fAsymGaus.GetParameter(1) - asym_width*fAsymGaus.GetParameter(2), asym_branch, fAsymGaus.GetParameter(1) + asym_width*fAsymGaus.GetParameter(2)))

	#s_asym_ternary = '( (x-y)/(x+y) > %f && (x-y)/(x+y) < %f ) ? 1. : 0.' % (fAsymGaus.GetParameter(1) - asym_width*fAsymGaus.GetParameter(2), fAsymGaus.GetParameter(1) + asym_width*fAsymGaus.GetParameter(2))
	"""



	# --------------------------------------------
	# --------------------------------------------
    # Make threshold strings for fit
	# --------------------------------------------
	# --------------------------------------------

	sTotLowThreshold = '(x > %f ? 1. : 0.)' % (tot_threshold_low)
	sTotHighThreshold = '(x < %f ? 1. : 0.)' % (s2TotBinning[2])


	lFits = [0. for i in xrange(len(l_num_electrons_to_check))]
	lLnlikelihoods = [0. for i in xrange(len(l_num_electrons_to_check))]

	for i, tot_number_electrons in enumerate(l_num_electrons_to_check):
		lGaussianFunctions = [make_string_for_1d_gaussian(electron_number, bAfterMainS2) for electron_number in xrange(1, tot_number_electrons+1)]
		sGaussianFunctions = ' + '.join(lGaussianFunctions)
		
		if bAfterMainS2:
			pass
			#sGaussianFunctions += ' + [%d]' % (electron_number+4)
			#sGaussianFunctions += ' + [%d] + [%d]*exp(-x/[%d])' % (electron_number+4, electron_number+5, electron_number+6)
		
		#print sGaussianFunctions
		
		sFDTot = '1./(1+exp(([%d] - x)/([%d])))' % (tot_number_electrons+2, tot_number_electrons+3)

		sFitFunction = '(%s)*(%s)*(%s)*(%s)' % (sTotLowThreshold, sTotHighThreshold, sGaussianFunctions, sFDTot)
		#sFitFunction = '(%s)*(%s)*(%s)' % (sTotLowThreshold, sTotHighThreshold, sGaussianFunctions)
		
		#print sFitFunction

		lFits[i] = root.TF1('fit_%d_electrons' % tot_number_electrons, sFitFunction, *(s2TotBinning[1:]))

		lFits[i].SetParameter(0, guessS2TotMean)
		lFits[i].SetParameter(1, guessS2TotWidth)
		
		lFits[i].SetParLimits(0, *limitsS2TotMean)
		lFits[i].SetParLimits(1, *limitsS2TotWidth)
		lFits[i].SetParLimits(2, 0, 1e7)

		# set parameter limits on amplitudes
		if tot_number_electrons > 1:
			for electron_number in xrange(3, 2+tot_number_electrons):
				lFits[i].SetParLimits(electron_number, 0, 1)

		lFits[i].SetParameter(2+tot_number_electrons, guessFDTotCenter)
		lFits[i].SetParameter(3+tot_number_electrons, guessFDTotShape)

		lFits[i].SetParLimits(2+tot_number_electrons, *limitsFDTotCenter)
		lFits[i].SetParLimits(3+tot_number_electrons, *limitsFDTotShape)

		if bAfterMainS2:
			lFits[i].SetParameter(6+tot_number_electrons, 25)

			lFits[i].SetParLimits(5+tot_number_electrons, 1, 1e7)
			lFits[i].SetParLimits(6+tot_number_electrons, 1, 50)

		current_fit_result = hS2Tot.Fit('fit_%d_electrons' % (tot_number_electrons), 'SLN')

		fitter = root.TVirtualFitter.Fitter(hS2Tot)
		amin = np.asarray([0], dtype=np.float64)
		dum1 = np.asarray([0], dtype=np.float64)
		dum2 = np.asarray([0], dtype=np.float64)
		dum3 = np.asarray([0], dtype=np.int32)
		dum4 = np.asarray([0], dtype=np.int32)

		fitter.GetStats(amin, dum1, dum2, dum3, dum4)
		lLnlikelihoods[i] = -amin/2.


		if show_fits:

			c1 = Canvas()
			hS2Tot.Draw()
			lFits[i].Draw('same')
			
			s_gaussian = '[0]*exp(-0.5/%.1f*((x - %.1f*[1])/[2])**2)'
			l_functions = []

			for num_electrons in xrange(1, tot_number_electrons+1):
				l_functions.append(root.TF1('peak_%d' % num_electrons, '[0]*exp(-0.5*((x - [1])/[2])**2)', *(s2TotBinning[1:])))
				
				if num_electrons == 1:
					ampl = lFits[i].GetParameter(2)
					mean = lFits[i].GetParameter(0)
					width = lFits[i].GetParameter(1)
					if width > 0:
						ampl /= (2*3.14*width**2.)**0.5
					l_functions[-1].SetParameters(ampl, mean, width)
				else:
					ampl = lFits[i].GetParameter(2)*lFits[i].GetParameter(1+num_electrons)
					mean = lFits[i].GetParameter(0) * num_electrons
					width = lFits[i].GetParameter(1)*num_electrons**0.5
					if width > 0:
						ampl /= (2*3.14*width**2.)**0.5
					l_functions[-1].SetParameters(ampl, mean, width)

				l_functions[-1].SetLineColor(aColors[num_electrons-1])
				l_functions[-1].Draw('same')
			
			
			c1.Update()

			raw_input('press enter...')

			del c1
		

	if len(l_num_electrons_to_check) > 1:
		for i in xrange(len(l_num_electrons_to_check) - 1):
			testStat = -2 * (lLnlikelihoods[i] - lLnlikelihoods[i + 1])
			pValue = 1 - stats.chi2.cdf(testStat, 1)
			indexToUse = i
			if pValue > 0.05:
				break
	else:
		indexToUse = 0

	most_significant_num_electrons = l_num_electrons_to_check[i]
	most_significant_ln_likelihood = lLnlikelihoods[i]
	most_significant_mean = lFits[indexToUse].GetParameter(0)
	most_significant_width = lFits[indexToUse].GetParameter(1)
	l_most_significant_ampl_ratios = [lFits[indexToUse].GetParameter(i) for i in xrange(3, 2+most_significant_num_electrons)] # len = num_electrons - 1

	if show_fits:
		c1 = Canvas()
		hS2Tot.Draw()
		lFits[indexToUse].Draw('same')
		
		s_gaussian = '[0]*exp(-0.5/%.1f*((x - %.1f*[1])/[2])**2)'
		l_functions = []

		for num_electrons in xrange(1, most_significant_num_electrons+1):
			l_functions.append(root.TF1('peak_%d' % num_electrons, '[0]*exp(-0.5*((x - [1])/[2])**2)', *(s2TotBinning[1:])))
			
			if num_electrons == 1:
				ampl = lFits[indexToUse].GetParameter(2)
				mean = lFits[indexToUse].GetParameter(0)
				width = lFits[indexToUse].GetParameter(1)
				if width > 0:
					ampl /= (2*3.14*width**2.)**0.5
				l_functions[-1].SetParameters(ampl, mean, width)
			else:
				ampl = lFits[indexToUse].GetParameter(2)*lFits[indexToUse].GetParameter(1+num_electrons)
				mean = lFits[indexToUse].GetParameter(0) * num_electrons
				width = lFits[indexToUse].GetParameter(1)*num_electrons**0.5
				if width > 0:
					ampl /= (2*3.14*width**2.)**0.5
				l_functions[-1].SetParameters(ampl, mean, width)

			l_functions[-1].SetLineColor(aColors[num_electrons-1])
			l_functions[-1].Draw('same')
		
		
		c1.Update()

		"""
		c2 = Canvas()
		hS2Width = Hist(100, 0, 400, name='hS2Width', title='Total Gas Gain - V_{c} %.3f kV' % current_analysis.get_cathode_setting())
		hS2Width.SetMarkerSize(0)
		current_analysis.Draw(width_branch, hist=hS2Width, selection=current_analysis.get_cuts())
		hS2Width.Draw()
		c2.Update()

		c3 = Canvas()
		hS2Height = Hist(100, 0, 0.003, name='hS2Height', title='Total Gas Gain - V_{c} %.3f kV' % current_analysis.get_cathode_setting())
		hS2Height.SetMarkerSize(0)
		current_analysis.Draw(height_branch, hist=hS2Height, selection=current_analysis.get_cuts())
		hS2Height.Draw()
		c3.Update()
		"""

		#current_analysis.get_T1()[0].Scan('EventId:%s:%s:%s:%s' % (tot_branch, peak_branch, left_edge_branch, right_edge_branch), current_analysis.get_cuts())

		print '\n\nResults:'
		print 'S2Tot Mean: %.2f' % most_significant_mean
		print 'S2Tot Width: %.2f' % most_significant_width
		print 'Num Electrons in range: %d\n\n' % most_significant_num_electrons

		raw_input('press enter...')

		del c1
		del current_analysis



	return most_significant_mean, l_most_significant_ampl_ratios, hS2Tot





def fit_s2_2d(filename, most_significant_mean, l_most_significant_ampl_ratios, tot_threshold_low, tot_threshold_high, bot_threshold_low, top_threshold_low, look_after_main, show_fits=False):
	
	current_analysis = neriX_analysis.neriX_analysis(filename)

	aColors = [4, 2, 8, 7, 5, 9]

	if look_after_main:
		tot_branch = 'ctS2sTotBottom[] + S2sTotTop[]'
		bot_branch = 'ctS2sTotBottom[]'
		top_branch = 'S2sTotTop[]'
	else:
		tot_branch = 'ctS2sTotBottom[S2Order[0]] + S2sTotTop[S2Order[0]]'
		bot_branch = 'ctS2sTotBottom[S2Order[0]]'
		top_branch = 'S2sTotTop[S2Order[0]]'

	most_significant_num_electrons = len(l_most_significant_ampl_ratios) + 1

	s2BotBinning = [30, 0, 60]
	s2TopBinning = [30, 0, 60]

	# iSampleWindow = [115,140] # run 15
	#iSampleWindow = [100, 200]  # after KNF
	iSampleWindow = [180,220] # run 10
	iS2Wdith = 200
	iSamplesBetweenS2 = 200
	
	guessS2BotFrac = 0.6
	limitsS2BotFrac = [0.4, 0.8]
	guessS2BotWidth = 7
	limitsS2BotWidth = [5, 20]

	guessS2TopWidth = 7
	limitsS2TopWidth = [3, 20]
	
	guessCorrelation = -0.5
	limitsCorrelation = [-1, 1]
	
	guessFDTotCenter = 5
	guessFDTotShape = 3
	limitsFDTotCenter = [0, 20]
	limitsFDTotShape = [1e-3, 5]

	guessFDTopCenter = 5
	guessFDTopShape = 3
	limitsFDTopCenter = [-10, 10]
	limitsFDTopShape = [1e-3, 5]

	# --------------------------------------------
	# --------------------------------------------
    # Add cuts for analysis
	# --------------------------------------------
	# --------------------------------------------


	current_analysis.add_cut('%s > %f' % (tot_branch, tot_threshold_low))
	current_analysis.add_cut('%s < %f' % (tot_branch, tot_threshold_high))
	
	current_analysis.add_cut('%s > %f' % (bot_branch, bot_threshold_low))
	current_analysis.add_cut('%s > %f' % (top_branch, top_threshold_low))

	if look_after_main:
		current_analysis.add_cut('S2sPeak[] - S2sPeak[0] > 1500')
		current_analysis.add_cut('S2sRightEdge[] - S2sLeftEdge[] < %i' % (iS2Wdith))
	else:
		current_analysis.add_cut('S2sPeak[S2Order[0]] > (S1sPeak[0] + %i) && S2sPeak[S2Order[0]] < (S1sPeak[0] + %i)' % (iSampleWindow[0], iSampleWindow[1]))
		current_analysis.add_cut('Alt$(S2sLeftEdge[S2Order[1]],100000) - S2sRightEdge[S2Order[0]] > %i' % (iSamplesBetweenS2))
		current_analysis.add_cut('S2sRightEdge[S2Order[0]] - S2sLeftEdge[S2Order[0]] < %i' % (iS2Wdith))


	current_analysis.set_event_list()


	# --------------------------------------------
	# --------------------------------------------
    # Fill Histogram
	# --------------------------------------------
	# --------------------------------------------


	hS2BottomTop = Hist2D(*(s2BotBinning+s2TopBinning), name='hS2BottomTop', title='Gas Gain - V_{c} %.3f kV' % current_analysis.get_cathode_setting())
	hS2BottomTop.SetStats(0)
	current_analysis.Draw('%s:%s' % (bot_branch, top_branch), hist=hS2BottomTop, selection=current_analysis.get_cuts())



	sTotLowThreshold = '(x+y > %f ? 1. : 0.)' % (tot_threshold_low)
	sTotHighThreshold = '(x+y < %f ? 1. : 0.)' % (tot_threshold_high)
	sBotLowThreshold = '(x > %f ? 1. : 0.)' % (bot_threshold_low)
	sTopLowThreshold = '(y > %f ? 1. : 0.)' % (top_threshold_low)


	lGaussianFunctions = [make_string_for_2d_gaussian(electron_number, most_significant_mean) for electron_number in xrange(1, most_significant_num_electrons+1)]
	sGaussianFunctions = ' + '.join(lGaussianFunctions)

	sFDTot = '1./(1+exp(([%d] - x - y)/([%d])))' % (electron_number+4, electron_number+5)
	sFDTop = '1./(1+exp(([%d] - y)/([%d])))' % (electron_number+6, electron_number+7)

	sFitFunction = '(%s)*(%s)*(%s)*(%s)*(%s)*(%s)*(%s)' % (sTotLowThreshold, sTotHighThreshold, sBotLowThreshold, sTopLowThreshold, sGaussianFunctions, sFDTot, sFDTop)
	
	print '\n\n'
	print sFitFunction
	print '\n\n'

	fBotTop = root.TF2('fBotTop', sFitFunction, *(s2BotBinning[1:]+s2TopBinning[1:]))


	# 0 = bot mean percentage
	# 1 = bot width
	# 2 = top width
	# 3 = correlation
	# 4 = first peak ampl
	# 5-(3+num_electrons) = relative peak ampl

	fBotTop.SetParameter(0, guessS2BotFrac)
	fBotTop.SetParameter(1, guessS2BotWidth)
	fBotTop.SetParameter(2, guessS2TopWidth)
	fBotTop.SetParameter(3, guessCorrelation)

	fBotTop.SetParLimits(0, *limitsS2BotFrac)
	fBotTop.SetParLimits(1, *limitsS2BotWidth)
	fBotTop.SetParLimits(2, *limitsS2TopWidth)
	fBotTop.SetParLimits(3, *limitsCorrelation)
	fBotTop.SetParLimits(4, 1, 1e8)
	
	#fBotTop.SetParameter(electron_number+4, guessFDTotCenter)
	#fBotTop.SetParameter(electron_number+5, guessFDTotShape)
	fBotTop.FixParameter(electron_number+4, -100)
	fBotTop.FixParameter(electron_number+5, .1)

	fBotTop.SetParameter(electron_number+6, guessFDTopCenter)
	fBotTop.SetParameter(electron_number+7, guessFDTopShape)
	
	#fBotTop.SetParLimits(electron_number+4, *limitsFDTotCenter)
	#fBotTop.SetParLimits(electron_number+5, *limitsFDTotShape)
	fBotTop.SetParLimits(electron_number+6, *limitsFDTopCenter)
	fBotTop.SetParLimits(electron_number+7, *limitsFDTopShape)

	if most_significant_num_electrons > 1:
		for i in xrange(most_significant_num_electrons-1):
			fBotTop.FixParameter(5+i, l_most_significant_ampl_ratios[i])

	fBotTop.SetContour(5)
	fitResult = hS2BottomTop.Fit('fBotTop', 'SL')

	print 'here'

	c1 = Canvas()
	hS2BottomTop.Draw('colz')
	fBotTop.Draw('same')
	c1.Update()
	raw_input('press enter')





lFiles = ['nerix_150106_1137']
#lFiles = ['nerix_140915_1158', 'nerix_140917_1010', 'nerix_140929_1115', 'nerix_140929_1212', 'nerix_140930_0925', 'nerix_140930_1029', 'nerix_140930_1127', 'nerix_140930_1626', 'nerix_141003_1232', 'nerix_141006_0951', 'nerix_141014_1024', 'nerix_141014_1414', 'nerix_141015_1155', 'nerix_141022_1127', 'nerix_141029_1110', 'nerix_141103_1101', 'nerix_141103_1553', 'nerix_141103_1627', 'nerix_141103_1655']


#lFiles = ['nerix_141203_1506', 'nerix_141208_1123', 'nerix_141210_1028','nerix_141217_1041', 'nerix_150106_1100', 'nerix_150106_1137', 'nerix_150106_1234', 'nerix_150106_1404', 'nerix_150114_1407', 'nerix_150128_1158', 'nerix_150128_1327', 'nerix_150128_1454', 'nerix_150128_1622']

# choose whether to look for late S2 or
# for single electrons from photoionization of
# the gate

bAfterMainS2 = False
#bAfterMainS2 = True

lNumElectrons = [2, 3, 4, 5]
#lNumElectrons = [7, 8, 9, 10, 11]

tot_threshold_low = 0
tot_threshold_high = 80


#compare_methods_s2_tot(lFiles[0], lNumElectrons, 0, tot_threshold_high)

mean, lAmplRatios, hS2Tot = fit_s2_tot_1d(lFiles[0], lNumElectrons, tot_threshold_low, tot_threshold_high, bAfterMainS2, show_fits=True)
fit_s2_2d(lFiles[0], mean, lAmplRatios, tot_threshold_low, tot_threshold_high, 0, 0, bAfterMainS2, show_fits=True)



"""
for file in lFiles:
    lLnlikelihoods = [0 for i in xrange(len(lNumElectrons))]
    lStrings = [0 for i in xrange(len(lNumElectrons))]
    for i, num_electrons in enumerate(lNumElectrons):
        print '\n\n\nStarting %s with %d electrons.\n\n\n' % (file, lNumElectrons[i])
        lStrings[i], lLnlikelihoods[i], forTextFile = gas_gain_fitting(file, num_electrons)

    # find first step with p-value > 0.05 and take previous

    # ex:
    # 2->3: p < 0.05
    # 3->4: p > 0.05 - STOP!
    # use 3 electrons

    if len(lNumElectrons) > 1:
        for i in xrange(len(lNumElectrons) - 1):
            testStat = -2 * (lLnlikelihoods[i] - lLnlikelihoods[i + 1])
            pValue = 1 - stats.chi2.cdf(testStat, 1)
            indexToUse = i
            if pValue > 0.05:
                break
    else:
        indexToUse = 0

    # run one more time to save image of "correct"
    # number of electrons
    dummy1, dummy2, forTextFile = gas_gain_fitting(file, lNumElectrons[indexToUse])

    sForTextFile += forTextFile
    print sForTextFile

    sSingleFile = lStrings[indexToUse]
    sForFile += '# %s\n%s\n\n\n' % (file, sSingleFile)

print sForFile

print '\n\n\nCopy and paste into text file\n\n\n'

print sForTextFile
"""


