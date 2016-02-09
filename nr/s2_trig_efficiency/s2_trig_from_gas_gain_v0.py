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

lowTimeDiff = 50
highTimeDiff = 100

numBinsForGaussian = 50
lowForGaussian = -300
highForGaussian = 300

dtLow = 250
dtHigh = 2300

#choose cuts
#current_analysis.add_z_cut()
#current_analysis.add_radius_cut(0., 0.85)


#--------------- Start Parameters to Change ----------------


#parameterToExamineS1 = 'S1sTotBottom[0]'
lowerBoundS2 = 0
upperBoundS2 = 2000
nBinsS1 = 20


#--------------- End Parameters to Change ----------------

#tof_analysis.get_T1().Scan('EventId:TrigLeftEdge-S1sPeak[0]:S1sTotBottom[0]', 'S1sTotBottom[0] > 20')


c1 = root.TCanvas('c1','c1',200,10,1000,700)
c1.SetGridx()
c1.SetGridy()


hNoCut = Hist(nBinsS1, lowerBoundS2, upperBoundS2, name='s2_spec_no_trig_cut', title='s1_spec_no_trig_cut', drawstyle='hist')

hWithCut = Hist(nBinsS1, lowerBoundS2, upperBoundS2, name='s2_spec_with_trig_cut', title='s1_spec_with_trig_cut', drawstyle='hist')

hGaus = Hist(numBinsForGaussian, lowForGaussian, highForGaussian, name='hGaus')

totalEventsWithTrigger = 0
TotalEventsWithoutTrigger = 0

numEvents = current_analysis.get_T1().GetEntriesFast()
#numEvents = 10000
for i in tqdm(xrange(numEvents)):
	#if i % 10000 == 0:
	#	print 'Processing event %d / %d' % (i, numEvents)

	current_analysis.get_T1().GetEntry(i)
	current_analysis.get_T2().GetEntry(i)

	lS1sTotBottom = list(current_analysis.get_T2().S1sTotBottom)
	lS2sTotBottom = list(current_analysis.get_T2().S2sTotBottom)
	#lS2s = list(current_analysis.get_T2().S2s)

	lS1sPeak = list(current_analysis.get_T1().S1sPeak)
	lS2sPeak = list(current_analysis.get_T1().S2sPeak)

	lS2Order = list(current_analysis.get_T1().S2Order)
	
	lS2sLeftEdge = list(current_analysis.get_T1().S2sLeftEdge)
	lS2sRightEdge = list(current_analysis.get_T1().S2sRightEdge)

	lS2sWidth = list(current_analysis.get_T1().S2sLowWidth)

	lS1sNoiseMedian = list(current_analysis.get_T2().S1sNoiseMedian)
	lS1sNoiseTrapezoid = list(current_analysis.get_T2().S1sNoiseTrapezoid)

	lTrigLeftEdge = list(current_analysis.get_T1().TrigLeftEdge)
	lTrigLength = list(current_analysis.get_T1().TrigLength)
	lXY = list(current_analysis.get_T2().S2sPosFann)

	lBaseline = list(current_analysis.get_T1().BaselinesAvg)


	if len(lS1sTotBottom) < 1:
		continue

	if len(lS2sPeak) < 2:
		continue

	if len(lTrigLeftEdge) < 1:
		continue


	#if len(lXY) < 1 or (lXY[0][0]**2 + lXY[0][1]**2)**0.5 > 0.7:
	#	continue

	# check that this peak passes dt cut
	#if not ((lS2sPeak[0]-lS1sPeak[0]) > dtLow and (lS2sPeak[0]-lS1sPeak[0]) < dtHigh):
	#if not ((lS2sPeak[0]-lS1sPeak[0]) > dtLow):
	#	continue

	# check for double scatters
	#if 0.2*lS2sTotBottom[0] < lS2sTotBottom[1]:
	#	continue

	for peakNumber, peakTime in enumerate(lS2sLeftEdge):
		if not peakNumber == lS2Order[0]:
			continue
	
		# check that the signal is between S1s[0] and S2s[0]
		#if not (((peakTime > lS1sPeak[0] + 20) and  (peakTime < lS1sPeak[0] + 250))):
		#if not (((peakTime > lS1sPeak[0] + 20) and  (peakTime < lS2sLeftEdge[0]))):
		#if not (((peakTime > lS1sPeak[0] + 100) and  (peakTime < lS1sPeak[0] + 165)) or ((peakTime > lS1sPeak[0] + 1530) and  (peakTime < lS1sPeak[0] + 1590))):
		if not (((peakTime > lS1sPeak[0] + 100) and  (peakTime < lS1sPeak[0] + 165))):
		#if not (((peakTime > lS1sPeak[0] + 1530) and  (peakTime < lS1sPeak[0] + 1570))):
		#if not (((peakTime > (lS1sPeak[0] + 20)) and  (peakTime < (lS1sPeak[0] + 200))) or peakTime > (lS2sPeak[0] + 3000)):
			continue

		# only look at peaks in our range of interest
		if lS2sTotBottom[lS2Order[0]] > upperBoundS2:
			continue

		if not (lS2sRightEdge[lS2Order[0]]-lS2sLeftEdge[lS2Order[0]])<400:
			continue

		hNoCut.Fill(lS2sTotBottom[peakNumber])
		TotalEventsWithoutTrigger += 1
		trigFound = -1
	
		for trigNumber, trigTime in enumerate(lTrigLeftEdge):
			# check if false trigger on S1
			if trigTime - lS1sPeak[0] < 60:
					continue
		
		
			#if (trigTime - peakTime) > -30 and (trigTime - peakTime) < 30:
			#	print 'Event: %d' % i
			#	print trigTime, peakTime, lS1sPeak[0], lS2sTotBottom[peakNumber]
			if ((trigTime - peakTime) > lowTimeDiff and (trigTime - peakTime) < highTimeDiff) or (trigTime > peakTime and trigTime < (peakTime + lS2sWidth[peakNumber])):
				hWithCut.Fill(lS2sTotBottom[peakNumber])
				trigFound = trigTime
				#print i, trigTime, lS2sLeftEdge[peakNumber], -(lS2sLeftEdge[peakNumber] - trigTime)
				totalEventsWithTrigger += 1
				break

		"""
		if lS2sTotBottom[peakNumber] > 1500 and trigFound == -1:
			print 'Event: %d' % i
			print lTrigLeftEdge
			print lTrigLength
			print peakTime, lS2sTotBottom[peakNumber], lS2sWidth[peakNumber]
			print lS1sPeak[0], lS2sPeak[0], lS2sPeak[0] - lS1sPeak[0]
			print 'check width!'
		"""

		for trigNumber, trigTime in enumerate(lTrigLeftEdge):
			if (trigTime - peakTime) > lowForGaussian and (trigTime - peakTime) < highForGaussian:
				if trigTime - lS1sPeak[0] < 60:
					continue
				#hGaus.Fill(trigTime - lS1sPeak[0])
				hGaus.Fill(trigTime - lS2sLeftEdge[peakNumber])



print '\n\nNumber of events with trigger: %d' % totalEventsWithTrigger
print 'Number of events without trigger: %d\n\n' % TotalEventsWithoutTrigger

#hNoCut.Draw()
#hWithCut.SetLineColor(root.kRed)
#hWithCut.Draw('same')

hEff = root.TEfficiency(hWithCut, hNoCut)
hEff.SetTitle('S2 Trigger Efficiency from Gas Gain Calibration; S2 Size [PE]; Percentage Causing Trigger');

gEff = hEff.CreateGraph()
gEff.GetXaxis().SetRangeUser(lowerBoundS2, upperBoundS2)
gEff.Draw('AP')
c1.Update()




c2 = Canvas()
hGaus.Draw()
c2.Update()

c3 = Canvas()
hNoCut.SetMarkerSize(0)
hNoCut.Draw()
hWithCut.SetMarkerSize(0)
hWithCut.SetLineColor(root.kRed)
hWithCut.Draw('same')
c3.Update()




hEffForFitting = hWithCut.Clone()
hEffForFitting.Divide(hNoCut)



#fSigmoid = root.TF1('fSigmoid', '-1 + (2. / (1 + exp([1]*([0]-x))))', lowerBoundS2, upperBoundS2)
fSigmoid = root.TF1('fSigmoid', '(1. / (1 + exp([1]*([0]-x))))', lowerBoundS2, upperBoundS2)
fSigmoid.SetParameters(0., 800, 0.006)
#fSigmoid.FixParameter(0, 0)
fSigmoid.SetParLimits(0, 0, 2500)
fSigmoid.SetParLimits(1, 0.0005, 0.05)
#fSigmoid.SetParLimits(2, 0.1, 100)
#fSigmoid.SetParLimits(3, 0.01, 0.5)
#fSigmoid.FixParameter(0, 800)
#fSigmoid.FixParameter(1, 0.006)
#fSigmoid.FixParameter(3, .1)
fSigmoid.SetLineStyle(9)
fSigmoid.SetLineWidth(1)
fSigmoid.SetLineColor(root.kBlue)

c1.cd()
#hEffForFitting.Fit(fSigmoid, 'NL')
gEff.Fit(fSigmoid, 'NL')

fSigmoid.Draw('same')

gConfInterval = root.TGraphAsymmErrors(*neriX_analysis.create_graph_with_confidence_interval_for_fit(gEff, root.TVirtualFitter.GetFitter()))
gConfInterval.SetLineColor(root.kBlue)
gConfInterval.SetFillColor(root.kBlue)
gConfInterval.SetFillStyle(3005)
gConfInterval.Draw('3 same')

sFitInfo1 = '#epsilon_{trig} = #frac{1}{1 + e^{#alpha (#beta-x)}}'
pt1 = root.TPaveText(.5,.35,.8,.45,'blNDC')
text1 = pt1.AddText(sFitInfo1)
pt1.SetTextColor(root.kBlack)
pt1.SetFillStyle(0)
pt1.SetBorderSize(0)
pt1.Draw('same')

sFitInfo2 = '#alpha = %.4f #pm %.4f' % (fSigmoid.GetParameter(1), fSigmoid.GetParError(1))
pt2 = root.TPaveText(.5,.25,.8,.35,'blNDC')
text2 = pt2.AddText(sFitInfo2)
pt2.SetTextColor(root.kBlack)
pt2.SetFillStyle(0)
pt2.SetBorderSize(0)
pt2.Draw('same')

sFitInfo3 = '#beta = %.4f #pm %.4f' % (fSigmoid.GetParameter(0), fSigmoid.GetParError(0))
pt3 = root.TPaveText(.5,.15,.8,.25,'blNDC')
text3 = pt3.AddText(sFitInfo3)
pt3.SetTextColor(root.kBlack)
pt3.SetFillStyle(0)
pt3.SetBorderSize(0)
pt3.Draw('same')

thresholdX = fSigmoid.GetX(0.5, lowerBoundS2, upperBoundS2)
lThreshold = root.TLine()
lThreshold.SetLineColor(root.kOrange + 1)
lThreshold.SetLineStyle(7)
lThreshold.DrawLine(thresholdX, gEff.GetYaxis().GetXmin(), thresholdX, gEff.GetYaxis().GetXmax())


c1.Update()



raw_input('Enter to continue...')

response = raw_input('Would you like to save the histograms used for TEfficiency for later analysis?  Press "y" to proceed, otherwise press anything: ')

if response == 'y':
	outputFile = root_open('../leff_minimizer/efficiency_files/s2_trig_efficiency.root', 'recreate')
	
	hWithCut.Write()
	hNoCut.Write()
	fSigmoid.Write()

	#sCuts = root.TObjString('cuts_used')
	#sCuts.Write(current_analysis.get_cuts(), root.TObject.kOverwrite)

