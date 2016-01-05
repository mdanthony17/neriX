import ROOT as root
import neriX_datasets
from neriX_analysis import neriX_analysis
import sys, random, string, pickle, time
from array import array
from rootpy.io import File
from rootpy.tree import Tree, TreeModel
from rootpy.plotting import Hist, Hist2D, Hist3D, Canvas, Legend
from rootpy.plotting.utils import draw
import gc, math

def propagate_div_uncertainty(a, b, a_err, b_err):
	return math.fabs(a/b)*( (a_err/a)**2 + (b_err/b)**2 )**0.5

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


def fit_photopeak(lFilesToLoad, radialCut, numProcessors=1):

	# ---------- ERROR CHECKING ----------

	currentAnalysis = neriX_analysis(lFilesToLoad, 4.5, 0.345, -1)
	
	currentAnalysis.add_z_cut()
	currentAnalysis.add_single_scatter_cut()
	currentAnalysis.add_radius_cut(0, radialCut)
	currentAnalysis.add_cut('(s1asym > 0)')


	# ---------- END ERROR CHECKING ----------




	# ---------- CONSTANTS FOR FITS AND HISTOGRAMS ----------

	numBinsCES = 50

	degree = currentAnalysis.get_degree_setting()
	if degree > -1:
		S1NB = 100
		S1LB = 0
		S1UB = 2500

		S2NB = 100
		S2LB = 0
		S2UB = 8.0e5

	elif degree == -2 or degree == -4:
		S1NB = 100
		S1LB = 0
		S1UB = 1400

		S2NB = 100
		S2LB = 0
		S2UB = 400e3
		
		#print 'NOTE: lowered for 3kV anode'
	
		secondParameter = 'S2sTotBottom[0]'
	
	
	elif degree == -1:
		S1NB = 25
		S1LB = 0
		S1UB = 3500

		S2NB = 25
		S2LB = 0
		S2UB = 1100e3
		
		upperBoundCES = 800.
		
		#print 'NOTE: lowered for 3kV anode'
	
		secondParameter = 'S2sTotBottom[0]'
	

	else:
		print 'Only setup to handle coincidence and minitron and Co57 calibrations (degree = -4)'
		sys.exit()

	dHistParameters = {'S1NB':S1NB,
					   'S1LB':S1LB,
					   'S1UB':S1UB,
					   'S2NB':S2NB,
					   'S2LB':S2LB,
					   'S2UB':S2UB}


	sHistParameters = ''
	for key in dHistParameters:
		sHistParameters += key + '\t'

	# ---------- END CONSTANTS FOR FITS AND HISTOGRAMS ----------



	# ---------- SET EVENT LIST ----------

	#currentAnalysis.set_event_list()
	currentAnalysis.multithread_set_event_list(numProcessors)

	# ---------- END SET EVENT LIST ----------



	# ---------- MAKE CES CUT ----------

	g1 = 0.14 #0.1
	g2 = 26.2 #24.
	sigmaCES = 1.

	cCES = Canvas(width=900, height=700, name='cCES')
	cCES.SetGridx()
	cCES.SetGridy()

	hCES = Hist(numBinsCES, 0, upperBoundCES, name='hCES', title='hCES', drawstyle='hist')
	currentAnalysis.Draw('0.0137*(S1sTotBottom[0]/%f + %s/%f)' % (g1, secondParameter, g2), hist=hCES)

	hCES.Draw()
	cCES.Update()

	lbCES = float(raw_input('\n\nPlease enter lower bound on combined energy scale: '))
	ubCES = float(raw_input('\n\nPlease enter upper bound on combined energy scale: '))


	fGausCES = root.TF1('fGausCES', 'gaus', lbCES, ubCES)
	#fGausCES = root.TF1('fGausCES', '[0]*exp(-[1]*x) + [2]*exp(-0.5*pow((x-[3])/[4], 2))', lbCES, ubCES)
	#fGausCES.SetParameters(500, 0.01, 500, 650, 50)
	hCES.Fit(fGausCES, 'LR')
	fGausCES.Draw('same')
	cCES.Update()

	#lbCES = fGausCES.GetParameter(1) - sigmaCES*fGausCES.GetParameter(2)
	#ubCES = fGausCES.GetParameter(1) + sigmaCES*fGausCES.GetParameter(2)
	raw_input('Fit okay?')
	

	sCES = '(0.0137*(S1sTotBottom[0]/%f + %s/%f) > %f && 0.0137*(S1sTotBottom[0]/%f + %s/%f) < %f)' % (g1, secondParameter, g2, lbCES, g1, secondParameter, g2, ubCES)
	currentAnalysis.add_cut(sCES)
	#currentAnalysis.set_event_list()

	del cCES



	# ---------- END MAKE CES CUT ----------



	# ---------- MAIN LOOP ----------

	
	c1 = Canvas(width=900, height=700, name='c1')
	c1.SetLogz()
	c1.SetGridx()
	c1.SetGridy()
	
	
	h1 = Hist2D(dHistParameters['S1NB'], dHistParameters['S1LB'], dHistParameters['S1UB'], dHistParameters['S2NB'], dHistParameters['S2LB'], dHistParameters['S2UB'], name='h1', title='S2 vs. S1')
	h1.GetXaxis().SetTitle('S1 [PE]')
	h1.GetYaxis().SetTitle('S2 [PE]')
	h1.SetStats(0)

	print '\n\nSecond Parameter being used: ' + secondParameter + '\n\n'
	currentAnalysis.multithread_draw(numProcessors, 'S1sTotBottom[0]:' + secondParameter, hist=h1, selection=currentAnalysis.get_cuts())

	h1.Draw('colz')

	c1.Update()
	
	
	print '\n\nSelect box in S1 and S2 for peak you would like to examine time dependence of.\n\n'

	s1lb = float(raw_input('\n\nPlease select s1lb: '))
	s1ub = float(raw_input('\n\nPlease select s1ub: '))
	s2lb = float(raw_input('\n\nPlease select s2lb: '))
	s2ub = float(raw_input('\n\nPlease select s2ub: '))
	
	
	
	
	
	c6 = Canvas(width=1100, height=480, name='c6')
	c6.Divide(2)

	c6.cd(1)
	hS1 = h1.ProjectionX('hS1')
	hS1.SetMarkerSize(0)
	hS1.GetXaxis().SetRangeUser(s1lb, s1ub)
	hS1.Draw()

	fS1 = root.TF1('fS1', 'gaus', s1lb, s1ub)
	hS1.Fit('fS1', 'MEL')
	fS1.Draw('same')
	meanS1OfPeak = fS1.GetParameter(1)
	meanErrS1OfPeak = fS1.GetParError(1)

	c6.cd(2)
	hS2 = h1.ProjectionY('hS2')
	hS2.SetMarkerSize(0)
	hS2.GetXaxis().SetRangeUser(s2lb, s2ub)
	hS2.Draw()

	fS2 = root.TF1('fS2', 'gaus', s2lb, s2ub)
	hS2.Fit('fS2', 'MEL')
	fS2.Draw('same')
	meanS2OfPeak = fS2.GetParameter(1)
	meanErrS2OfPeak = fS2.GetParError(1)

	c6.Update()
	
	
	
	
	# define constants
	

	rNumBins = 10
	rMin = 0
	rMax = radialCut**2

	zNumBins = 20
	zMin = -25
	zMax = 0

	s1NumBins = 50
	s2NumBins = 50
	
	rLowFit = 0.01
	rHighFit = 0.81
	
	zLowFit = -22
	zHighFit = -4

	
	
	
	
	
	# R vs Z vs S1/S2
	
	c2DCorrection = Canvas(width=1100, height=480, name='c6')
	c2DCorrection.Divide(2)

	c2DCorrection.cd(1)
	h2DCorrectionS1 = Hist3D(zNumBins, zMin, zMax, rNumBins, rMin, rMax, s1NumBins, s1lb, s1ub, name='h2DCorrectionS1')
	currentAnalysis.Draw('Z:R^2:S1sTotBottom[0]', hist=h2DCorrectionS1, selection=currentAnalysis.get_cuts())
	h2DCorrectionS1Projection = h2DCorrectionS1.Project3DProfile('xy')
	h2DCorrectionS1Projection.Scale(1./meanS1OfPeak)
	h2DCorrectionS1Projection.SetStats(0)
	h2DCorrectionS1Projection.SetName('pos_correction_S1')
	h2DCorrectionS1Projection.SetTitle('pos_correction_S1')
	h2DCorrectionS1Projection.GetXaxis().SetTitle('R^2')
	h2DCorrectionS1Projection.GetYaxis().SetTitle('Z [mm]')
	h2DCorrectionS1Projection.Draw('colz')
	
	
	c2DCorrection.cd(2)
	h2DCorrectionS2 = Hist3D(zNumBins, zMin, zMax, rNumBins, rMin, rMax, s1NumBins, s2lb, s2ub, name='h2DCorrectionS2')
	currentAnalysis.Draw('Z:R^2:S2sTotBottom[0]', hist=h2DCorrectionS2, selection=currentAnalysis.get_cuts())
	h2DCorrectionS2Projection = h2DCorrectionS2.Project3DProfile('xy')
	h2DCorrectionS2Projection.Scale(1./meanS2OfPeak)
	h2DCorrectionS2Projection.SetStats(0)
	h2DCorrectionS2Projection.SetName('pos_correction_S2')
	h2DCorrectionS2Projection.SetTitle('pos_correction_S2')
	h2DCorrectionS2Projection.GetXaxis().SetTitle('R^2')
	h2DCorrectionS2Projection.GetYaxis().SetTitle('Z [mm]')
	h2DCorrectionS2Projection.Draw('colz')
	
	fPosRec = File('../../../python_modules/src/pos_correction_run_15.root', 'recreate')
	#fPosRec = File('./src/pos_correction_run_15.root', 'recreate')
	h2DCorrectionS1Projection.Write()
	h2DCorrectionS2Projection.Write()
	fPosRec.Close()
	
	
	c2DCorrection.Update()
	
	
	cXY = Canvas()
	hXY = Hist2D(40, -1, 1, 40, -1, 1, name='hXY')
	currentAnalysis.Draw('X:Y', hist=hXY, selection=currentAnalysis.get_cuts())
	hXY.Draw('colz')
	cXY.Update()
	
	
	


	hRS1 = Hist2D(rNumBins, rMin, rMax, s1NumBins, s1lb, s1ub, name='hRS1')
	hRS1.GetXaxis().SetTitle('Radius^2 [normalized to max]')
	hRS1.GetYaxis().SetTitle('S1 of selected peak')
	hRS1.SetStats(0)

	currentAnalysis.multithread_draw(numProcessors, 'R^2:S1sTotBottom[0]', hist=hRS1, selection=currentAnalysis.get_cuts())
	
	hProfileRS1 = hRS1.ProfileX('hProfileRS1')
	hProfileRS1.GetXaxis().SetTitle('Radius^2 [normalized to max]')
	hProfileRS1.GetYaxis().SetTitle('S1 of selected peak')
	hProfileRS1.GetYaxis().SetRangeUser(s1lb, s1ub)
	hProfileRS1.SetStats(0)
	
	fRS1 = root.TF1('fRS1', 'pol1', rLowFit, rHighFit)
	hProfileRS1.Fit('fRS1', 'MERCFNQ')

	c2 = Canvas(width=1100, height=480, name='c2')
	c2.Divide(2)
	
	c2.cd(1)
	hRS1.Draw('colz')
	c2.SetGridx()
	c2.SetGridy()
	
	c2.cd(2)
	hProfileRS1.Draw()
	fRS1.Draw('same')

	sFitInfo1 = 'slope/mean = %.4f +/- %.4f' % (fRS1.GetParameter(1)/meanS1OfPeak, propagate_div_uncertainty(fRS1.GetParameter(1), meanS1OfPeak, fRS1.GetParError(1), meanErrS1OfPeak))
	pt1 = root.TPaveText(.2,.75,.6,.85,'blNDC')
	text1 = pt1.AddText(sFitInfo1)
	pt1.SetTextColor(root.kBlue)
	pt1.SetFillStyle(0)
	pt1.SetBorderSize(0)
	pt1.Draw('same')

	c2.SetGridx()
	c2.SetGridy()

	c2.cd()
	c2.Update()
	
	
	

	hRS2 = Hist2D(rNumBins, rMin, rMax, s2NumBins, s2lb, s2ub, name='hRS2')
	hRS2.GetXaxis().SetTitle('Radius^2 [normalized to max]')
	hRS2.GetYaxis().SetTitle('S2 of selected peak')
	hRS2.SetStats(0)

	currentAnalysis.multithread_draw(numProcessors, 'R^2:S2sTotBottom[0]', hist=hRS2, selection=currentAnalysis.get_cuts())

	hProfileRS2 = hRS2.ProfileX('hProfileRS2')
	hProfileRS2.GetXaxis().SetTitle('Radius^2 [normalized to max]')
	hProfileRS2.GetYaxis().SetTitle('S2 of selected peak')
	hProfileRS2.GetYaxis().SetRangeUser(s2lb, s2ub)
	hProfileRS2.SetStats(0)

	fRS2 = root.TF1('fRS2', 'pol1', rLowFit, rHighFit)
	hProfileRS2.Fit('fRS2', 'MERCFNQ')

	c3 = Canvas(width=1100, height=480, name='c3')
	c3.Divide(2)
	
	c3.cd(1)
	hRS2.Draw('colz')
	c3.SetGridx()
	c3.SetGridy()
	
	c3.cd(2)
	hProfileRS2.Draw()
	fRS2.Draw('same')

	sFitInfo2 = 'slope/mean = %.4f +/- %.4f' % (fRS2.GetParameter(1)/meanS2OfPeak, propagate_div_uncertainty(fRS2.GetParameter(1), meanS2OfPeak, fRS2.GetParError(1), meanErrS2OfPeak))
	pt2 = root.TPaveText(.2,.75,.6,.85,'blNDC')
	text2 = pt2.AddText(sFitInfo2)
	pt2.SetTextColor(root.kBlue)
	pt2.SetFillStyle(0)
	pt2.SetBorderSize(0)
	pt2.Draw('same')

	c3.SetGridx()
	c3.SetGridy()

	c3.cd()
	c3.Update()



	hZS1 = Hist2D(zNumBins, zMin, zMax, s1NumBins, s1lb, s1ub, name='hZS1')
	hZS1.GetXaxis().SetTitle('Z [mm]')
	hZS1.GetYaxis().SetTitle('S1 of selected peak')
	hZS1.SetStats(0)

	currentAnalysis.multithread_draw(numProcessors, 'Z:S1sTotBottom[0]', hist=hZS1, selection=currentAnalysis.get_cuts())

	hProfileZS1 = hZS1.ProfileX('hProfileZS1')
	hProfileZS1.GetXaxis().SetTitle('Z [mm]')
	hProfileZS1.GetYaxis().SetTitle('S1 of selected peak')
	hProfileZS1.GetYaxis().SetRangeUser(s1lb, s1ub)
	hProfileZS1.SetStats(0)

	fZS1 = root.TF1('fZS1', 'pol1', zLowFit, zHighFit)
	hProfileZS1.Fit('fZS1', 'MERCFNQ')

	c4 = Canvas(width=1100, height=480, name='c4')
	c4.Divide(2)
	
	c4.cd(1)
	hZS1.Draw('colz')
	c4.SetGridx()
	c4.SetGridy()
	
	c4.cd(2)
	hProfileZS1.Draw()
	fZS1.Draw('same')

	sFitInfo3 = 'slope/mean = %.4f +/- %.4f [1/mm]' % (fZS1.GetParameter(1)/meanS1OfPeak, propagate_div_uncertainty(fZS1.GetParameter(1), meanS1OfPeak, fZS1.GetParError(1), meanErrS1OfPeak))
	pt3 = root.TPaveText(.2,.75,.6,.85,'blNDC')
	text3 = pt3.AddText(sFitInfo3)
	pt3.SetTextColor(root.kBlue)
	pt3.SetFillStyle(0)
	pt3.SetBorderSize(0)
	pt3.Draw('same')

	c4.SetGridx()
	c4.SetGridy()

	c4.cd()
	c4.Update()
	
	
	
	

	hZS2 = Hist2D(zNumBins, zMin, zMax, s2NumBins, s2lb, s2ub, name='hZS2')
	hZS2.GetXaxis().SetTitle('Z [mm]')
	hZS2.GetYaxis().SetTitle('S2 of selected peak')
	hZS2.SetStats(0)

	currentAnalysis.multithread_draw(numProcessors, 'Z:S2sTotBottom[0]', hist=hZS2, selection=currentAnalysis.get_cuts())

	hProfileZS2 = hZS2.ProfileX('hProfileZS2')
	hProfileZS2.GetXaxis().SetTitle('Z [mm]')
	hProfileZS2.GetYaxis().SetTitle('S2 of selected peak')
	hProfileZS2.GetYaxis().SetRangeUser(s2lb, s2ub)
	hProfileZS2.SetStats(0)

	fZS2 = root.TF1('fZS2', 'pol1', zLowFit, zHighFit)
	hProfileZS2.Fit('fZS2', 'MERCFNQ')

	c5 = Canvas(width=1100, height=480, name='c5')
	c5.Divide(2)
	
	c5.cd(1)
	hZS2.Draw('colz')
	c5.SetGridx()
	c5.SetGridy()
	
	c5.cd(2)
	hProfileZS2.Draw()
	fZS2.Draw('same')

	sFitInfo4 = 'slope/mean = %.4f +/- %.4f [1/mm]' % (fZS2.GetParameter(1)/meanS2OfPeak, propagate_div_uncertainty(fZS2.GetParameter(1), meanS2OfPeak, fZS2.GetParError(1), meanErrS2OfPeak))
	pt4 = root.TPaveText(.2,.75,.6,.85,'blNDC')
	text4 = pt4.AddText(sFitInfo4)
	pt4.SetTextColor(root.kBlue)
	pt4.SetFillStyle(0)
	pt4.SetBorderSize(0)
	pt4.Draw('same')

	c5.SetGridx()
	c5.SetGridy()

	c5.cd()
	c5.Update()

	"""
	c2.cd(1)
	c2.Update()
	c2.cd(2)
	c2.Update()
	c3.cd(0)
	c3.Update()
	c4.cd(0)
	c4.Update()
	"""

	raw_input('Press enter to continue...')

	peak = 'cs137'
	
	c1.SaveAs('/Users/Matt/Desktop/s1_s2_%s_select.png' % peak)
	c2.Print('/Users/Matt/Desktop/s1_r2_%s_select.png' % peak)
	c3.SaveAs('/Users/Matt/Desktop/s2_r2_%s_select.png' % peak)
	c4.SaveAs('/Users/Matt/Desktop/s1_z_%s_select.png' % peak)
	c5.SaveAs('/Users/Matt/Desktop/s2_z_%s_select.png' % peak)
	#c6.SaveAs('/Users/Matt/Desktop/separate_peaks_164_select_151109.png')


if __name__ == '__main__':
	if len(sys.argv) != 5:
		print 'Use is python set_photopeak_fit_info.py <0deg ROOT file> <90deg ROOT file> <180deg ROOT file> <270deg ROOT file>'
		sys.exit()

	filesToLoad = [sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4]]

	#file0Deg = sys.argv[1]
	#file90Deg = sys.argv[1]
	#file0Deg = sys.argv[1]
	#file0Deg = sys.argv[1]
	radialCut = 0.85

	fit_photopeak(filesToLoad, radialCut)
