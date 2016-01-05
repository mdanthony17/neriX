import ROOT as root
import neriX_datasets
from neriX_analysis import neriX_analysis
import sys, random, string, pickle, time
from array import array
from rootpy.io import File
from rootpy.tree import Tree, TreeModel
from rootpy.plotting import Hist, Hist2D, Canvas, Legend
from rootpy.plotting.utils import draw
import numpy as np
import matplotlib.pyplot as plt


# generator for floats
def drange(start, stop, step):
	r = start
	while r < stop:
		yield r
		r += step



def convert_name_to_unix_time(fileName):
	if fileName[-5:] == '.root':
		fileName = fileName[:-5]
	
	# check that we know position of dates in name
	if fileName[0:5] == 'nerix':
		sTimeTaken = '20' + fileName[6:17] + '00'
		return int(time.mktime(time.strptime(sTimeTaken, '%Y%m%d_%H%M%S')))
	elif fileName[0:8] == 'ct_nerix':
		sTimeTaken = '20' + fileName[9:20] + '00'
		return int(time.mktime(time.strptime(sTimeTaken, '%Y%m%d_%H%M%S')))
	else:
		print 'Only know how to handle nerix and ct_nerix files currently'
		print 'Please add to convert_name_to_unix_time function in order to handle the new case'
		sys.exit()



def ces_bkg_res(currentAnalysis, g1, g2, numBins=80, lowerBound=0, upperBound=400, drawFit=False):


	
	if drawFit:
		cCES = Canvas(width=900, height=700, name='cCES')
		cCES.SetGridx()
		cCES.SetGridy()

	hCES = Hist(numBins, lowerBound, upperBound, name='hCES', title='hCES', drawstyle='hist')
	currentAnalysis.Draw('0.0137*(cpS1sTotBottom[0]/%f + cpS2sTotBottom[0]/%f)' % (g1, g2), hist=hCES)



	# use tspectrum to find peaks
	spec = root.TSpectrum(2, 0.7)
	spec.Search(hCES)
	bX = spec.GetPositionX()
	bX.SetSize(2)
	aX = np.array(bX, 'f')
	if aX[0] > aX[1]:
		aX[0], aX[1] = aX[1], aX[0]
	#print aX

	if aX[0] < 100:
		return 1e5
	
	lowerLimitFit = aX[0] * 0.85
	upperLimitFit = aX[1] * 1.15
	
	fDoubleGaussian = root.TF1('fDoubleGaussian', '[0]*exp(-0.5*pow((x-[1])/[2], 2)) + [3]*exp(-0.5*pow((x-[4])/[5], 2))', lowerLimitFit, upperLimitFit)
	fDoubleGaussian.SetParameters(500, aX[0], 0.1*aX[0], 500, aX[1], aX[1]*0.1)
	fitResult = hCES.Fit('fDoubleGaussian', 'QLR')

	if drawFit:
		hCES.Draw()
		fDoubleGaussian.Draw('same')
		cCES.Update()
		raw_input('Press enter to continue...')
	
	res164 = fDoubleGaussian.GetParameter(2)/fDoubleGaussian.GetParameter(1)
	res236 = fDoubleGaussian.GetParameter(5)/fDoubleGaussian.GetParameter(1)
	
	fitStatus = fitResult.CovMatrixStatus()
	#print fitStatus
	if fitStatus != 0:
		# return large number if fit fails
		print 'Fit failed!'
		return 1e5
	else:
		# want to minimize sum of squares of resolution of each peak
		return res164**2 + res236**2, fDoubleGaussian




def ces_bkg_res_minimizer(g1Low, g1High, g1Step, g2Low, g2High, g2Step, currentAnalysis, numBins=80, lowerBound=0, upperBound=400, drawFit=False):
	#assert (g1High - g1Low) % g1Step == 0.
	#assert (g2High - g2Low) % g2Step == 0.
	
	lG1 = [g1 for g1 in drange(g1Low, g1High, g1Step)]
	lG2 = [g2 for g2 in drange(g2Low, g2High, g2Step)]
	lRes = [[0. for j in xrange(len(lG1))] for i in xrange(len(lG2))]
	
	lowestResPars = (-1, -1)
	lowestRes = 1e3
	fLowest = None

	for i, g1 in enumerate(lG1):
		for j, g2 in enumerate(lG2):
			lRes[j][i], fRes = ces_bkg_res(currentAnalysis, g1, g2, numBins, lowerBound, upperBound, drawFit)
			if lRes[j][i] < lowestRes:
				lowestRes = lRes[j][i]
				lowestResPars = (g1, g2)
				fLowest = fRes

	print 'Lowest Res: %f ' % lowestRes
	print 'g1: %f' % lowestResPars[0]
	print 'g2: %f' % lowestResPars[1]
			
	#cs = plt.contour(lG1, lG2, lRes)
	#plt.clabel(cs, fontsize=9, inline=1)
	#plt.show()

	print fLowest.GetParameter(1), fLowest.GetParameter(2)
	print fLowest.GetParameter(4), fLowest.GetParameter(5)

	# sometimes fitter reverses intended order
	if fLowest.GetParameter(1) > fLowest.GetParameter(4):
		mean164, mean236 = fLowest.GetParameter(4), fLowest.GetParameter(1)
		width164, width236 = fLowest.GetParameter(5), fLowest.GetParameter(2)
	else:
		mean164, mean236 = fLowest.GetParameter(1), fLowest.GetParameter(4)
		width164, width236 = fLowest.GetParameter(2), fLowest.GetParameter(5)

	lbCES164 = mean164 - 0.5*width164
	ubCES164 = mean164 + 0.25*width164

	lbCES236 = mean236 - 0.25*width236
	ubCES236 = mean236 + 0.5*width236


	sCES164Cut = '(0.0137*(cpS1sTotBottom[0]/%f + cpS2sTotBottom[0]/%f) > %f && 0.0137*(cpS1sTotBottom[0]/%f + cpS2sTotBottom[0]/%f) < %f)' % (g1, g2, lbCES164, g1, g2, ubCES164)
	sCES236Cut = '(0.0137*(cpS1sTotBottom[0]/%f + cpS2sTotBottom[0]/%f) > %f && 0.0137*(cpS1sTotBottom[0]/%f + cpS2sTotBottom[0]/%f) < %f)' % (g1, g2, lbCES236, g1, g2, ubCES236)

	return g1, g2, sCES164Cut, sCES236Cut



def fit_s1_with_ces_cut(currentAnalysis, sCesCut, lowerS1=100, upperS1=1000, numBinsS1=25):
	cS1 = Canvas(width=900, height=700, name='cCES')
	cS1.SetGridx()
	cS1.SetGridy()
	
	print sCesCut

	hS1 = Hist(numBinsS1, lowerS1, upperS1, name='hS1', title='hS1', drawstyle='hist')
	currentAnalysis.Draw('cpS1sTotBottom[0]', hist=hS1, selection=sCesCut)

	fS1Trial = root.TF1('fS1Trial', 'gaus', lowerS1, upperS1)
	hS1.Fit('fS1Trial', 'LR')
	
	fS1 = root.TF1('fS1', 'gaus', fS1Trial.GetParameter(1) - 1.*fS1Trial.GetParameter(2), fS1Trial.GetParameter(1) + 1.*fS1Trial.GetParameter(2))
	hS1.Fit('fS1', 'LR')

	hS1.Draw()
	fS1.Draw('same')

	cS1.Update()

	raw_input('Press enter to continue...')

	return fS1




def fit_s2_with_ces_cut(currentAnalysis, sCesCut, lowerS2=80e3, upperS2=250e3, numBinsS2=25):
	cS2 = Canvas(width=900, height=700, name='cCES')
	cS2.SetGridx()
	cS2.SetGridy()
	
	print sCesCut

	hS2 = Hist(numBinsS2, lowerS2, upperS2, name='hS2', title='hS2', drawstyle='hist')
	currentAnalysis.Draw('cpS2sTotBottom[0]', hist=hS2, selection=sCesCut)

	fS2Trial = root.TF1('fS2Trial', 'gaus', lowerS2, upperS2)
	hS2.Fit('fS2Trial', 'LR')
	
	fS2 = root.TF1('fS2', 'gaus', fS2Trial.GetParameter(1) - 1.*fS2Trial.GetParameter(2), fS2Trial.GetParameter(1) + 1.*fS2Trial.GetParameter(2))
	hS2.Fit('fS2', 'LR')

	hS2.Draw()
	fS2.Draw('same')

	cS2.Update()

	raw_input('Press enter to continue...')

	return fS2







if __name__ == '__main__':
	if len(sys.argv) != 2:
		print 'Use is python ces_bkg_res_minimizer.py <name of file>'
		sys.exit()
	
	currentAnalysis = neriX_analysis(sys.argv[1])

	currentAnalysis.add_z_cut()
	currentAnalysis.add_single_scatter_cut()
	currentAnalysis.add_radius_cut(0, 0.5)
	currentAnalysis.add_cut('(s1asym > 0)')

	currentAnalysis.set_event_list()
	
	#ces_bkg_res(neriX_analysis(sys.argv[1]), 0.12, 24, drawFit=True)
	g1, g2, sCES164Cut, sCES236Cut = ces_bkg_res_minimizer(0.09, 0.14, 0.01, 22., 31., 0.1, currentAnalysis, drawFit=False)

	ces_bkg_res(currentAnalysis, g1, g2, drawFit=True)
	fS1 = fit_s1_with_ces_cut(currentAnalysis, sCES164Cut)
	fS2 = fit_s2_with_ces_cut(currentAnalysis, sCES164Cut)

	FILENAME = currentAnalysis.get_filename()
	startTime = convert_name_to_unix_time(FILENAME)

	dParametersToPrint = {'start_time':startTime, 'run':currentAnalysis.get_run(), 'anode':currentAnalysis.get_anode_setting(), 'cathode':currentAnalysis.get_cathode_setting(), 'degree':currentAnalysis.get_degree_setting(), 's1_mean':fS1.GetParameter(1), 's1_mean_err':fS1.GetParError(1), 's1_sigma':fS1.GetParameter(2), 's1_sigma_err':fS1.GetParError(2), 's2_mean':fS2.GetParameter(1), 's2_mean_err':fS2.GetParError(1), 's2_sigma':fS2.GetParameter(2), 's2_sigma_err':fS2.GetParError(2)}

	print '\n\nPlease copy below into fits_completed.py\n'
	print '#' + str(FILENAME)
	print 'dFitParameters[\'' + str(FILENAME) + '\'] =', dParametersToPrint
	print '\n\n'




