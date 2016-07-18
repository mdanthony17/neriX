#!/usr/bin/python

import ROOT as root
import sys, array, math
import neriX_analysis
from rootpy.plotting import Hist, Hist2D, Canvas, Legend
from rootpy.io import root_open, File
import numpy as np
from tqdm import tqdm


l_filenames = ['nerix_160715_1527']
#l_filenames = ['nerix_160715_1527', 'nerix_160716_1245', 'nerix_160717_1438']


current_analysis = neriX_analysis.neriX_analysis(l_filenames, -50, 1.054, 4.5)

lowTimeDiff = 50
highTimeDiff = 100



#--------------- Start Parameters to Change ----------------

na_height_branch = 'NaiHeight[0]'
l_na_height_settings = [50, 0.105, 0.125]
na_height_guess = 0.115

mod_dt_branch = '(S2sPeak[0] - NaiPeak[0])'
dt_lb = 200.
dt_ub = 1800.

mod_dt_offset_branch = '(NaiPeak[0] - S1sPeak[0])'
l_mod_dt_offset_settings = [15, 180, 195]

neriX_analysis.warning_message('Hard-coded gain correction')
s2_branch = 'S2sTotBottom[0]*1.48e6/9e5'
l_s2_settings = [50, 0, 1000]

trig_branch = 'TrigLeftEdge-S2sLeftEdge[0]'
l_trig_settings = [50, 0, 150]
trig_lb = 40
trig_ub = 100

trial_width = 0.5
cut_width = 2.0


#--------------- End Parameters to Change ----------------




c1 = Canvas()
c1.SetGridx()
c1.SetGridy()


h_na22_height = Hist(*l_na_height_settings, name='h_na22_height', title='Na22 Spectrum')
h_na22_height.SetMarkerSize(0)

current_analysis.Draw(na_height_branch, hist=h_na22_height)


f_na22_height_trial = root.TF1('f_na22_height_trial', 'gaus', l_na_height_settings[1], l_na_height_settings[2])
h_na22_height.Fit('f_na22_height_trial', 'LSQRN')
left_edge_from_trial = f_na22_height_trial.GetParameter(1) - trial_width*f_na22_height_trial.GetParameter(2)
right_edge_from_trial = f_na22_height_trial.GetParameter(1) + trial_width*f_na22_height_trial.GetParameter(2)

f_na22_height = root.TF1('f_na22_height', 'gaus', left_edge_from_trial, right_edge_from_trial)
h_na22_height.Fit('f_na22_height', 'LSR')

h_na22_height.Draw()
c1.Update()


# modified dt cut

c2 = Canvas()
c2.SetGridx()
c2.SetGridy()


h_mod_dt_offset = Hist(*l_mod_dt_offset_settings, name='h_mod_dt_offset', title='Modified dt Offset (NaI Peak - S1 Peak)')
h_mod_dt_offset.SetMarkerSize(0)

current_analysis.Draw(mod_dt_offset_branch, hist=h_mod_dt_offset)

f_mod_dt_offset = root.TF1('f_mod_dt_offset', 'gaus', l_mod_dt_offset_settings[1], l_mod_dt_offset_settings[2])
h_mod_dt_offset.Fit('f_mod_dt_offset', 'LSQR')

mean_offset = f_mod_dt_offset.GetParameter(1)

h_mod_dt_offset.Draw()
c2.Update()






s_na22_height_cut = '%s > %f && %s < %f' % (na_height_branch, f_na22_height.GetParameter(1) - cut_width*f_na22_height.GetParameter(2), na_height_branch, f_na22_height.GetParameter(1) + cut_width*f_na22_height.GetParameter(2))
s_mod_dt_cut = '%s > %f && %s < %f' % (mod_dt_branch, dt_lb-mean_offset, mod_dt_branch, dt_ub-mean_offset)

current_analysis.add_cut(s_na22_height_cut)
#current_analysis.add_cut(s_mod_dt_cut)
current_analysis.add_z_cut()
current_analysis.add_cut('%s > 0' % (s2_branch))
current_analysis.add_cut('%s < %f' % (s2_branch, l_s2_settings[2]))

current_analysis.multithread_set_event_list(7)


current_analysis.get_T1().Scan('EventId:S1sPeak[0]:S2sPeak[0]:NaiPeak[0]:%s:TrigLeftEdge' % s2_branch, '%s < 300' % s2_branch)


c4 = Canvas()
c4.SetGridx()
c4.SetGridy()


h_trig_time = Hist(*l_trig_settings, name='h_trig_time')
h_trig_time.SetMarkerSize(0)

current_analysis.Draw(trig_branch, hist=h_trig_time)

#f_mod_dt_offset = root.TF1('f_mod_dt_offset', 'gaus', l_mod_dt_offset_settings[1], l_mod_dt_offset_settings[2])
#h_mod_dt_offset.Fit('f_mod_dt_offset', 'LSQR')

h_trig_time.Draw()
c4.Update()




h_no_cut = Hist(*l_s2_settings, name='h_no_cut')
current_analysis.Draw(s2_branch, hist=h_no_cut, selection=current_analysis.get_cuts())

current_analysis.add_cut('%s > %f && %s < %f' % (trig_branch, trig_lb, trig_branch, trig_ub))
h_with_cut = Hist(*l_s2_settings, name='h_with_cut')
current_analysis.Draw(s2_branch, hist=h_with_cut, selection=current_analysis.get_cuts())

c3 = Canvas()
c3.SetGridx()
c3.SetGridy()

h_eff = root.TEfficiency(h_with_cut, h_no_cut)
h_eff.SetTitle('S2 Trigger Efficiency from Gas Gain Calibration; S2 Size [PE]; Percentage Causing Trigger');

g_eff = h_eff.CreateGraph()
g_eff.GetXaxis().SetRangeUser(l_s2_settings[1], l_s2_settings[2])
g_eff.Draw('AP')


f_eff = root.TF1('f_eff', 'exp(-[0]*exp(-x*[1]))', l_s2_settings[1], l_s2_settings[2])
f_eff.SetParameters(10., 1./100.)
f_eff.SetParLimits(0, 0, 300)
f_eff.SetParLimits(1, 0, 1)
frp_eff = g_eff.Fit('f_eff', 'SNRLL')
f_eff.Draw('same')



c3.Update()


raw_input('Press enter to continue...')

"""

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
"""
