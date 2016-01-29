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



#--------------- Start Parameters to Change ----------------


#parameterToExamineS1 = 'S1sTotBottom[0]'
lowerBoundS2 = 0
upperBoundS2 = 4000
nBinsS1 = 20


#--------------- End Parameters to Change ----------------

#tof_analysis.get_T1().Scan('EventId:TrigLeftEdge-S1sPeak[0]:S1sTotBottom[0]', 'S1sTotBottom[0] > 20')


c1 = root.TCanvas('c1','c1',200,10,1000,700)
c1.SetGridx()
c1.SetGridy()


hNoCut = Hist(nBinsS1, lowerBoundS2, upperBoundS2, name='s2_spec_no_trig_cut', title='s1_spec_no_trig_cut', drawstyle='hist')

hWithCut = Hist(nBinsS1, lowerBoundS2, upperBoundS2, name='s2_spec_with_trig_cut', title='s1_spec_with_trig_cut', drawstyle='hist')


#choose cuts
#current_analysis.add_z_cut()
#current_analysis.add_radius_cut(0., 0.85)



current_analysis.add_cut('abs(S2sPeak[S2Order[0]]-S1sPeak[S1Order[0]])>400')
current_analysis.add_cut('(S2sPeak[S2Order[0]]-S1sPeak[0])>300')
current_analysis.add_cut('(Alt$(S2sPeak[S2Order[1]],1e5)-S2sPeak[S2Order[0]])>400')
current_analysis.add_cut('(S2sRightEdge[S2Order[0]]-S2sLeftEdge[S2Order[0]])<400')

current_analysis.set_event_list()

current_analysis.Draw('S2sTotBottom[S2Order[0]]', hist=hNoCut)
current_analysis.Draw('S2sTotBottom[S2Order[0]]', hist=hWithCut, selection='(-S2sPeak[S2Order[0]] + TrigLeftEdge) > 50 && (-S2sPeak[S2Order[0]] + TrigLeftEdge) < 100')


print '\n\nNumber of events with trigger: %d' % hWithCut.Integral()
print 'Number of events: %d\n\n' % hNoCut.Integral()

#hNoCut.Draw()
#hWithCut.SetLineColor(root.kRed)
#hWithCut.Draw('same')

hEff = root.TEfficiency(hWithCut, hNoCut)
hEff.SetTitle('S2 Trigger Efficiency from Gas Gain Calibration; S2 Size [PE]; Percentage Causing Trigger');

gEff = hEff.CreateGraph()
gEff.GetXaxis().SetRangeUser(lowerBoundS2, upperBoundS2)
gEff.Draw('AP')
c1.Update()




hEffForFitting = hWithCut.Clone()
hEffForFitting.Divide(hNoCut)



#fSigmoid = root.TF1('fSigmoid', '-1 + (2. / (1 + exp([1]*([0]-x))))', lowerBoundS2, upperBoundS2)
fSigmoid = root.TF1('fSigmoid', '(1. / (1 + exp([1]*([0]-x))))', lowerBoundS2, upperBoundS2)
fSigmoid.SetParameters(0., 800, 0.006)
#fSigmoid.FixParameter(0, 0)
fSigmoid.SetParLimits(0, 0, 2500)
fSigmoid.SetParLimits(1, 0.0005, 0.01)
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

