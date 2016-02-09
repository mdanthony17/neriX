#!/usr/bin/python

import ROOT as root
import sys, array, math
import neriX_analysis
from rootpy.plotting import Hist, Hist2D, Canvas, Legend
from rootpy.io import root_open, File
import numpy as np
from tqdm import tqdm

if(len(sys.argv) != 2):
	print 'Usage is s1_tac_correction.py <ROOT filename>'
	sys.exit(1)
	
filename = sys.argv[1]


current_analysis = neriX_analysis.neriX_analysis(filename)

lowTimeDiff = 0
highTimeDiff = 6

lowForGaussian = -5
highForGaussian = 10


current_analysis.add_xs1asym_cut()
current_analysis.set_event_list()


#--------------- Start Parameters to Change ----------------


#s1Branch = 'S1sTotBottom[S1Order[0]]'
s1Branch = 'cpS1sTotBottom[S1Order[0]]'
lowerBoundS1 = -0.5
upperBoundS1 = 39.5
nBinsS1 = 20


#--------------- End Parameters to Change ----------------

#tof_analysis.get_T1().Scan('EventId:TrigLeftEdge-S1sPeak[0]:S1sTotBottom[0]', 'S1sTotBottom[0] > 20')


c1 = root.TCanvas('c1','c1',200,10,1000,700)
c1.SetGridx()
c1.SetGridy()



# -------------------------------------
#  Find events to check waveforms
# -------------------------------------

"""
#numEvents = current_analysis.get_T1().GetEntriesFast()
numEvents = 10000
for i in tqdm(xrange(numEvents)):

	current_analysis.get_T1().GetEntry(i)
	current_analysis.get_T2().GetEntry(i)

	lS1sTotBottom = list(current_analysis.get_T2().S1sTotBottom)
	lS1sPeak = list(current_analysis.get_T1().S1sPeak)
	lS1Order = list(current_analysis.get_T1().S1Order)
	lS2sPeak = list(current_analysis.get_T1().S2sPeak)

	lS1sLeftEdge = list(current_analysis.get_T1().S1sLeftEdge)
	lS1sRightEdge = list(current_analysis.get_T1().S1sRightEdge)
	
	lS2sLeftEdge = list(current_analysis.get_T1().S2sLeftEdge)
	lS2sRightEdge = list(current_analysis.get_T1().S2sRightEdge)

	lS1sNoiseMedian = list(current_analysis.get_T2().S1sNoiseMedian)
	lS1sNoiseTrapezoid = list(current_analysis.get_T2().S1sNoiseTrapezoid)

	lTrigLeftEdge = list(current_analysis.get_T1().TrigLeftEdge)
	lXY = list(current_analysis.get_T2().S2sPosFann)

	if len(lS1sTotBottom) < 1:
		continue

	if len(lS2sPeak) < 1:
		continue

	if len(lTrigLeftEdge) < 1:
		continue
	
	if lS1sTotBottom[lS1Order[0]] < 0.5 or lS1sTotBottom[lS1Order[0]] > 15:
		continue

	if len(lS1sPeak) > 1 and lS1sPeak[lS1Order[1]] - lS1sPeak[lS1Order[0]] < 10:
		continue

	trigFound = -1
	for trigNumber, trigTime in enumerate(lTrigLeftEdge):
		if (trigTime - lS1sPeak[lS1Order[0]]) > lowTimeDiff and (trigTime - lS1sPeak[lS1Order[0]]) < highTimeDiff:
			trigFound = trigNumber
			break

	if trigFound == -1:
		print '\nNo trigger event: %d, %d, %d\n' % (i, lS1sPeak[lS1Order[0]], lTrigLeftEdge[trigFound])
	else:
		print '\nTrigger event: %d, %d, %d\n' % (i, lS1sPeak[lS1Order[0]], lTrigLeftEdge[trigFound])
"""




hNoCut = Hist(nBinsS1, lowerBoundS1, upperBoundS1, name='s1_spec_no_trig_cut', title='s1_spec_no_trig_cut', drawstyle='hist')

hWithCut = Hist(nBinsS1, lowerBoundS1, upperBoundS1, name='s1_spec_with_trig_cut', title='s1_spec_with_trig_cut', drawstyle='hist')

hGaus = Hist(highForGaussian - lowForGaussian, lowForGaussian, highForGaussian, name='hGaus')
hGaus.SetMarkerSize(0)
hGaus.SetStats(0)
hGaus.GetXaxis().SetTitle('TrigLeftEdge[] - S1sPeak[S1Order[0]]')
hGaus.GetYaxis().SetTitle('Counts')
hGaus.SetTitle('Time between Trigger and First S1 in Waveform - %s' % current_analysis.get_filename_no_ext())


sBothCut = '%s > 0.5' % s1Branch
sBothCut += ' && Alt$(S1sPeak[S1Order[1]] - S1sPeak[S1Order[0]], 100000) > %f' % highTimeDiff

sTriggerCut = sBothCut + ' && (TrigLeftEdge[] - S1sPeak[S1Order[0]]) > %f && (TrigLeftEdge[] - S1sPeak[S1Order[0]]) < %f' % (lowTimeDiff, highTimeDiff)


current_analysis.Draw(s1Branch, hist=hNoCut, selection=sBothCut)
current_analysis.Draw(s1Branch, hist=hWithCut, selection=sTriggerCut)

current_analysis.Draw('TrigLeftEdge[] - S1sPeak[S1Order[0]]', hist=hGaus)



hEff = root.TEfficiency(hWithCut, hNoCut)
hEff.SetTitle('S1 Efficiency for TAC - %s; cpS1sTotBottom[S1Order[0]] [PE]; Percentage Causing Discriminator Pulse for TAC' % (current_analysis.get_filename_no_ext()));

gEff = hEff.CreateGraph()
gEff.GetXaxis().SetRangeUser(lowerBoundS1, upperBoundS1)
gEff.Draw('AP')
c1.Update()




c2 = Canvas()
c2.SetLogy()
hGaus.Draw()
c2.Update()

hEffForFitting = hWithCut.Clone()
hEffForFitting.Divide(hNoCut)


fEfficiency = root.TF1('fEfficiency', '1. - exp(-[0]*x)', lowerBoundS1, upperBoundS1)
fEfficiency.SetParLimits(0, 0.01, 50)


c1.cd()
#hEffForFitting.Fit(fEfficiency, 'NL')
gEff.Fit(fEfficiency, 'NL')

fEfficiency.Draw('same')

gConfInterval = root.TGraphAsymmErrors(*neriX_analysis.create_graph_with_confidence_interval_for_fit(gEff, root.TVirtualFitter.GetFitter()))
gConfInterval.SetLineColor(root.kBlue)
gConfInterval.SetFillColor(root.kBlue)
gConfInterval.SetFillStyle(3005)
gConfInterval.Draw('3 same')

sFitInfo1 = '#epsilon_{trig} = 1 - e^{-#alphax}'
pt1 = root.TPaveText(.5,.35,.8,.45,'blNDC')
text1 = pt1.AddText(sFitInfo1)
pt1.SetTextColor(root.kBlack)
pt1.SetFillStyle(0)
pt1.SetBorderSize(0)
pt1.Draw('same')

sFitInfo2 = '#alpha = %.4f #pm %.4f' % (fEfficiency.GetParameter(0), fEfficiency.GetParError(0))
pt2 = root.TPaveText(.5,.25,.8,.35,'blNDC')
text2 = pt2.AddText(sFitInfo2)
pt2.SetTextColor(root.kBlack)
pt2.SetFillStyle(0)
pt2.SetBorderSize(0)
pt2.Draw('same')


thresholdX = fEfficiency.GetX(0.5, lowerBoundS1, upperBoundS1)
lThreshold = root.TLine()
lThreshold.SetLineColor(root.kOrange + 1)
lThreshold.SetLineStyle(7)
lThreshold.DrawLine(thresholdX, gEff.GetYaxis().GetXmin(), thresholdX, gEff.GetYaxis().GetXmax())


c1.Update()



c2.cd()

lTimeDiffCutLow = root.TLine()
lTimeDiffCutLow.SetLineColor(root.kRed)
lTimeDiffCutLow.SetLineStyle(7)
lTimeDiffCutLow.DrawLine(lowTimeDiff, hGaus.GetMinimum(), lowTimeDiff, hGaus.GetMaximum())

lTimeDiffCutHigh = root.TLine()
lTimeDiffCutHigh.SetLineColor(root.kRed)
lTimeDiffCutHigh.SetLineStyle(7)
lTimeDiffCutHigh.DrawLine(highTimeDiff, hGaus.GetMinimum(), highTimeDiff, hGaus.GetMaximum())

c2.Update()


neriX_analysis.save_plot(['results', 'run_%d' % current_analysis.get_run(), current_analysis.get_filename_no_ext()], c1, 's1_disc_eff_%s' % current_analysis.get_filename_no_ext())
neriX_analysis.save_plot(['results', 'run_%d' % current_analysis.get_run(), current_analysis.get_filename_no_ext()], c2, 'time_diff_cut_%s' % current_analysis.get_filename_no_ext())
neriX_analysis.write_root_object(['results', 'run_%d' % current_analysis.get_run(), current_analysis.get_filename_no_ext()], fEfficiency, 'efficiency_fit_%s' % current_analysis.get_filename_no_ext())

raw_input('Enter to continue...')


"""

response = raw_input('Would you like to save the histograms used for TEfficiency for later analysis?  Press "y" to proceed, otherwise press anything: ')

if response == 'y':
	outputFile = root_open('../leff_minimizer/efficiency_files/s1_disc_efficiency.root', 'recreate')
	
	hWithCut.Write()
	hNoCut.Write()
	fEfficiency.Write()

	#sCuts = root.TObjString('cuts_used')
	#sCuts.Write(current_analysis.get_cuts(), root.TObject.kOverwrite)

"""