#!/usr/bin/python

import ROOT as root
import sys, array, math
import neriX_analysis
from rootpy.plotting import Hist, Hist2D, Canvas, Legend
from rootpy.io import root_open, File
import numpy as np
from tqdm import tqdm
import root_numpy

if(len(sys.argv) != 2):
	print 'Usage is s2_trig_from_gas_gain.py <ROOT filename>'
	sys.exit(1)
	
filename = sys.argv[1]


current_analysis = neriX_analysis.neriX_analysis(filename)


#--------------- Start Parameters to Change ----------------

l_s2_settings = [20, 0, 2000]

l_s2_time_diff = [100, 0.5, 20] # us
s2_time_diff_branch = '(S2sLeftEdge[] - S1sPeak[0])/100.'

l_s2_width_settings = [50, 0, 400]
s2_width_branch = 'S2sLowWidth[]'

l_s2_trig_time_diff = [100, 0, 300]
l_s2_trig_time_diff_branch = 'S2sRightEdge[0] - TrigLeftEdge[]'


#--------------- End Parameters to Change ----------------

#tof_analysis.get_T1().Scan('EventId:TrigLeftEdge-S1sPeak[0]:S1sTotBottom[0]', 'S1sTotBottom[0] > 20')


c_s2_time_diff = Canvas()
c_s2_time_diff.SetGridx()
c_s2_time_diff.SetGridy()

#current_analysis.add_cut('(S2sTot[1]/S2sTot[0])<0.02')
#current_analysis.add_cut('S2sPeak[] != S2sPeak[0]')

h_s2_time_diff = Hist(*l_s2_time_diff, name='h_s2_time_diff', title='S2 Time Differences')
current_analysis.Draw(s2_time_diff_branch, hist=h_s2_time_diff, selection=current_analysis.get_cuts())
h_s2_time_diff.GetXaxis().SetTitle('Time between main S2 and subsequent S2 [us]')
h_s2_time_diff.GetYaxis().SetTitle('Counts')
h_s2_time_diff.SetStats(0)
h_s2_time_diff.SetMarkerSize(0)
h_s2_time_diff.Draw()

c_s2_time_diff.Update()


lb_s2_time_diff_cut = 15#float(raw_input('\nPlease enter the lower bound of the cathode cut: '))
ub_s2_time_diff_cut = 17#float(raw_input('\nPlease enter the upper bound of the cathode cut: '))

s_s2_time_diff_cut = '(%s > %f && %s < %f)' % (s2_time_diff_branch, lb_s2_time_diff_cut, s2_time_diff_branch, ub_s2_time_diff_cut)

current_analysis.add_cut(s_s2_time_diff_cut)
current_analysis.set_event_list()


c_s2_width = Canvas()

h_s2_width = Hist(*l_s2_width_settings, name='h_s2_width', title='S2 Width')
current_analysis.Draw(s2_width_branch, hist=h_s2_width, selection=current_analysis.get_cuts())
h_s2_width.GetXaxis().SetTitle('S2 Width [samples]')
h_s2_width.GetYaxis().SetTitle('Counts')
h_s2_width.SetStats(0)
h_s2_width.SetMarkerSize(0)

h_s2_width.Draw()


c_s2_tot_bottom = Canvas()

h_s2_tot_bottom = Hist(*l_s2_settings, name='h_s2_tot_bottom', title='S2 Spectrum')
current_analysis.Draw('S2sTotBottom[0]', hist=h_s2_tot_bottom, selection=current_analysis.get_cuts())
h_s2_tot_bottom.GetXaxis().SetTitle('S2 [PE]')
h_s2_tot_bottom.GetYaxis().SetTitle('Counts')
h_s2_tot_bottom.SetStats(0)
h_s2_tot_bottom.SetMarkerSize(0)

h_s2_tot_bottom.Draw()


c_s2_tot_bottom.Update()








#current_analysis.get_T1().Scan('EventId:S2sTotBottom[0]', 'S2sTotBottom[0] < 200')


h_s2_spec_no_trig_cut = Hist(*l_s2_settings, name='h_s2_spec_no_trig_cut')
h_s2_spec_no_trig_cut.SetStats(0)
h_s2_spec_no_trig_cut.SetColor(root.kBlue)
h_s2_spec_no_trig_cut.SetMarkerSize(0)

h_s2_spec_with_trig_cut = Hist(*l_s2_settings, name='h_s2_spec_with_trig_cut')
h_s2_spec_with_trig_cut.SetStats(0)
h_s2_spec_with_trig_cut.SetColor(root.kRed)
h_s2_spec_with_trig_cut.SetMarkerSize(0)

h_s2_trig_time_diff = Hist(*l_s2_trig_time_diff, name='h_s2_trig_time_diff', title='Time difference between S2 and Trigger')
h_s2_trig_time_diff.SetStats(0)
h_s2_trig_time_diff.SetColor(root.kBlue)
h_s2_trig_time_diff.SetMarkerSize(0)


num_events = current_analysis.get_num_events_before_cuts()
#num_events = 20000
for i in tqdm(xrange(num_events)):

	current_analysis.get_T1().GetEntry(i)
	current_analysis.get_T2().GetEntry(i)

	l_s1_peak = list(current_analysis.get_T1().S1sPeak)
	l_s2_peak = list(current_analysis.get_T1().S2sPeak)
	l_s2_coin = list(current_analysis.get_T2().S2sCoin)
	l_s2_right_edge = list(current_analysis.get_T1().S2sRightEdge)
	l_s2_order = list(current_analysis.get_T1().S2Order)
	l_s2_tot_bottom = list(current_analysis.get_T2().S2sTotBottom)
	l_s2_width = list(current_analysis.get_T1().S2sWidth)
	l_trig_left_edge = list(current_analysis.get_T1().TrigLeftEdge)

	if len(l_s2_peak) < 2:
		continue

	num_peaks = len(l_s2_peak)

	for order_num, peak_num in enumerate(l_s2_order):
		if peak_num == 0:
			continue
		peak_location = l_s2_peak[peak_num]


		# below is for arbitrary S2
		"""
		# make sure that trigger is in S2 range of interest
		if l_s2_tot_bottom[peak_num] < 200 or l_s2_tot_bottom[peak_num] > 2000:
			continue
			
		# do width cut to make sure it is in typical range of (600, 1400) ns
		if l_s2_width[peak_num] < 70 or l_s2_width[peak_num] > 140:
			continue

		# check for double scatters that could throw off distance
		# from largest S2 cut
		if l_s2_tot_bottom[1] > 0.1*l_s2_tot_bottom[0]:
			continue
			
		#if l_s2_coin[1] < 3:
		#	continue
	
		#if (peak_location-l_s2_peak[0])/100. < lb_s2_time_diff_cut or (peak_location-l_s2_peak[0])/100. > ub_s2_time_diff_cut:
		if (peak_location-l_s2_peak[0]) < 500 or peak_location > 16000:
		#if (peak_location-l_s2_peak[0]) < 1500 or (peak_location-l_s2_peak[0]) > 1700:
			continue

		# check if previous peak was within 1 us
		#print peak_location, l_s2_peak[l_s2_order[order_num-1]]
		if (peak_location - l_s2_peak[l_s2_order[order_num-1]]) < 200:
			continue

		# check that pulse is either the last one or that the next
		# peak is more than 1 us away
		#print order_num, num_peaks
		if order_num != (num_peaks-1) and (l_s2_peak[l_s2_order[order_num+1]] - peak_location) < 200:
			#print 'failed next peak'
			#print l_s2_peak[l_s2_order[order_num+1]], peak_location
			continue


		# short cut to check if there is a trigger late in the waveform
		#if len(l_trig_left_edge) == 1:
		#	continue
		"""
		
		# below is for FIRST S2 (gate photoionization)
		
		if order_num != 0:
			continue
		
		if len(l_s1_peak) == 0:
			continue
		
		peak_location = l_s2_peak[peak_num]

		# make sure that trigger is in S2 range of interest
		if l_s2_tot_bottom[peak_num] < 200 or l_s2_tot_bottom[peak_num] > 2000:
			continue
			
		# do width cut to make sure it is in typical range of (600, 1400) ns
		if l_s2_width[peak_num] < 30 or l_s2_width[peak_num] > 90:
			continue


		if l_s2_coin[peak_num] < 5:
			continue


		# check for double scatters that could throw off distance
		# from largest S2 cut
		if l_s2_tot_bottom[1] > 0.05*l_s2_tot_bottom[0]:
			continue

		#print peak_location, l_s1_peak[0], peak_location-l_s1_peak[0]
		if (peak_location-l_s1_peak[0]) < 50 or (peak_location-l_s1_peak[0]) > 200:
			continue

		# check that pulse is either the last one or that the next
		# peak is more than 1 us away
		#print l_s2_peak
		if (l_s2_peak[l_s2_order[order_num+1]] - peak_location) < 400:
			continue



		h_s2_spec_no_trig_cut.Fill(l_s2_tot_bottom[peak_num])
		

		trigger_seen = False

		for trigger in l_trig_left_edge:
			if trigger_seen:
				break
		
			h_s2_trig_time_diff.Fill(l_s2_right_edge[peak_num] - trigger)
			
			#print len(l_trig_left_edge)
			#print trigger
			#print peak_location
			if (trigger - peak_location) > -50 and (trigger - peak_location) < 50:
			#if (l_s2_right_edge[peak_num] - trigger) > 100 and (l_s2_right_edge[peak_num] - trigger) < 300:
				h_s2_spec_with_trig_cut.Fill(l_s2_tot_bottom[peak_num])
				trigger_seen = True


		if trigger_seen:
			print '\nTrigger Seen :)!'
			print 'Event: %d, S2: %f, Width: %d, Location: %d, Trigger Location: %d' % (i, l_s2_tot_bottom[peak_num], l_s2_width[peak_num], peak_location, trigger)
		else:
			print '\nNo Trigger Seen :('
			print 'Event: %d, S2: %f, Width: %d, Location: %d' % (i, l_s2_tot_bottom[peak_num], l_s2_width[peak_num], peak_location)






c_cut_comparison = Canvas()

h_s2_spec_no_trig_cut.Draw()
h_s2_spec_with_trig_cut.Draw('same')

c_cut_comparison.Update()

c_s2_trig_time_diff = Canvas()

h_s2_trig_time_diff.Draw()

c_s2_trig_time_diff.Update()

raw_input('Press enter to continue')




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




"""
"""

response = raw_input('Would you like to save the histograms used for TEfficiency for later analysis?  Press "y" to proceed, otherwise press anything: ')

if response == 'y':
	outputFile = root_open('../leff_minimizer/efficiency_files/s2_trig_efficiency.root', 'recreate')
	
	hWithCut.Write()
	hNoCut.Write()
	fSigmoid.Write()

	#sCuts = root.TObjString('cuts_used')
	#sCuts.Write(current_analysis.get_cuts(), root.TObject.kOverwrite)
"""
