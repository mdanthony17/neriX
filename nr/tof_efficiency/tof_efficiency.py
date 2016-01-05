#!/usr/bin/python

import ROOT as root
import sys, array, math
import neriX_analysis
from rootpy.plotting import Hist, Hist2D, Canvas, Legend
from rootpy.io import root_open, File
import numpy as np

if(len(sys.argv) != 2):
	print 'Usage is s1_tac_correction.py <ROOT filename>'
	sys.exit(1)
	
filename = sys.argv[1]


current_analysis = neriX_analysis.neriX_analysis(filename)

lowTimeDiff = 0
highTimeDiff = 6

lowForGaussian = -5
highForGaussian = 10

dtLow = 200
dtHigh = 1300

#choose cuts
current_analysis.add_dt_cut(2., 13.)
current_analysis.add_radius_cut(0., 0.7)


#--------------- Start Parameters to Change ----------------


#parameterToExamineS1 = 'S1sTotBottom[0]'
lowerBoundS1 = -0.5
upperBoundS1 = 39.5
nBinsS1 = 40


#--------------- End Parameters to Change ----------------

#tof_analysis.get_T1().Scan('EventId:TrigLeftEdge-S1sPeak[0]:S1sTotBottom[0]', 'S1sTotBottom[0] > 20')


c1 = root.TCanvas('c1','c1',200,10,1000,700)
c1.SetGridx()
c1.SetGridy()


hNoCut = Hist(nBinsS1, lowerBoundS1, upperBoundS1, name='s1_spec_no_trig_cut', title='s1_spec_no_trig_cut', drawstyle='hist')

hWithCut = Hist(nBinsS1, lowerBoundS1, upperBoundS1, name='s1_spec_with_trig_cut', title='s1_spec_with_trig_cut', drawstyle='hist')

hGaus = Hist(highForGaussian - lowForGaussian, lowForGaussian, highForGaussian, name='hGaus')



numEvents = current_analysis.get_T1().GetEntriesFast()
#numEvents = 10000
for i in xrange(numEvents):
	if i % 10000 == 0:
		print 'Processing event ' + str(i) + ' / ' + str(numEvents)

	current_analysis.get_T1().GetEntry(i)
	current_analysis.get_T2().GetEntry(i)

	lS1sTotBottom = list(current_analysis.get_T2().S1sTotBottom)
	lS1sPeak = list(current_analysis.get_T1().S1sPeak)
	lS2sPeak = list(current_analysis.get_T1().S2sPeak)

	lS1sLeftEdge = list(current_analysis.get_T1().S1sLeftEdge)
	lS1sRightEdge = list(current_analysis.get_T1().S1sRightEdge)

	lS1sNoiseMedian = list(current_analysis.get_T2().S1sNoiseMedian)
	lS1sNoiseTrapezoid = list(current_analysis.get_T2().S1sNoiseTrapezoid)

	lTrigLeftEdge = list(current_analysis.get_T1().TrigLeftEdge)
	lXY = list(current_analysis.get_T2().S2sPosFann)

	lBaseline = list(current_analysis.get_T1().BaselinesAvg)


	if len(lS1sTotBottom) < 1:
		continue

	if len(lS2sPeak) < 1:
		continue

	if len(lTrigLeftEdge) < 1:
		continue

	#if len(lXY) < 1 or (lXY[0][0]**2 + lXY[0][1]**2)**0.5 > 0.7:
	#	continue

	# check that this peak passes dt cut
	#if (lS2sPeak[0]-lS1sPeak[0]) < dtLow or (lS2sPeak[0]-lS1sPeak[0]) > dtHigh:
	#	continue

	for peakNumber, peakTime in enumerate(lS1sPeak):
		hNoCut.Fill(lS1sTotBottom[peakNumber] - lS1sNoiseTrapezoid[0][16])
		#if peakNumber > 0:
		#	continue
	
		for trigNumber, trigTime in enumerate(lTrigLeftEdge):
			#print trigTime, peakTime
			if (trigTime - peakTime) > lowTimeDiff and (trigTime - peakTime) < highTimeDiff:
				#if lS1sTotBottom[peakNumber] < 10:
				#	print i, trigTime, peakTime, lS1sTotBottom[peakNumber] - lS1sNoiseTrapezoid[0][16], peakNumber, lS1sLeftEdge[peakNumber], lS1sRightEdge[peakNumber]
				hWithCut.Fill(lS1sTotBottom[peakNumber] - lS1sNoiseTrapezoid[0][16])
				break

		for trigNumber, trigTime in enumerate(lTrigLeftEdge):
			#print trigTime, peakTime
			if (trigTime - peakTime) > lowForGaussian and (trigTime - peakTime) < highForGaussian:
				hGaus.Fill(trigTime - peakTime)





hEff = root.TEfficiency(hWithCut, hNoCut)
hEff.SetTitle('S1 Efficiency for TAC; S1sTotBottom[0] [PE]; Percentage Causing Discriminator Pulse for TAC');
#hEff.GetXaxis().SetTitle('S1 [PE]')
#hEff.GetYaxis().SetTitle('Percentage Causing Discriminator Pulse for TAC')
#hEff.Draw()

gEff = hEff.CreateGraph()
gEff.GetXaxis().SetRangeUser(lowerBoundS1, upperBoundS1)
gEff.Draw('AP')
c1.Update()




c2 = Canvas()
hGaus.Draw()
c2.Update()

hEffForFitting = hWithCut.Clone()
hEffForFitting.Divide(hNoCut)



fSigmoid = root.TF1('fSigmoid', '-1 + (2. / (1 + exp([1]*([0]-x))))', lowerBoundS1, upperBoundS1)
fSigmoid.SetParameters(0., 0.1, 1)
#fSigmoid.FixParameter(0, 0)
fSigmoid.SetParLimits(0, -15, 15)
fSigmoid.SetParLimits(1, 0.01, 50)
#fSigmoid.SetParLimits(2, 0.1, 100)
#fSigmoid.SetParLimits(3, 0.01, 0.5)
#fSigmoid.FixParameter(0, 1050)
#fSigmoid.FixParameter(1, 0.03)
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

sFitInfo1 = '#epsilon_{trig} = -1 + #frac{2}{1 + e^{#alpha (#beta-x)}}'
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

thresholdX = fSigmoid.GetX(0.5, lowerBoundS1, upperBoundS1)
lThreshold = root.TLine()
lThreshold.SetLineColor(root.kOrange + 1)
lThreshold.SetLineStyle(7)
lThreshold.DrawLine(thresholdX, gEff.GetYaxis().GetXmin(), thresholdX, gEff.GetYaxis().GetXmax())


c1.Update()



raw_input('Enter to continue...')

response = raw_input('Would you like to save the histograms used for TEfficiency for later analysis?  Press "y" to proceed, otherwise press anything: ')

if response == 'y':
	outputFile = root_open('../leff_minimizer/efficiency_files/s1_disc_efficiency.root', 'recreate')
	
	hWithCut.Write()
	hNoCut.Write()
	fSigmoid.Write()

	#sCuts = root.TObjString('cuts_used')
	#sCuts.Write(current_analysis.get_cuts(), root.TObject.kOverwrite)

