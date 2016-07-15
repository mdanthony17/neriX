#!/usr/bin/python

import ROOT as root
import sys
import neriX_analysis
from rootpy.plotting import Hist2D, Hist, Legend, Canvas
import math, root_numpy
import numpy as np





#--------------- Start Parameters to Change ----------------


s1NumBins = 40
s1Min = 0
s1Max = 30
s1FitMin = 4
s1FitMax = 15

s2NumBins = 40
s2Min = 0
s2Max = 3000

asym_num_bins = 40
asym_low = -1
asym_high = 1

log_num_bins = 200
log_low = 1
log_high = 4.5


s1Branch = 'cpS1sTotBottom[0]'
s2Branch = 'cpS2sTotBottom[0]'
s1_asym_branch = 's1asym' #'(%s - S1sTotTop[0])/(%s + S1sTotTop[0])' % (s1Branch, s1Branch)
s2_asym_branch = 's2asym' #'(%s - S2sTotTop[0])/(%s + S2sTotTop[0])' % (s2Branch, s2Branch)


#--------------- End Parameters to Change ----------------



if(len(sys.argv) != 2):
	print 'Usage is python coin_analysis <ROOT filename>'
	sys.exit(1)

file1 = sys.argv[1]
currentAnalysis = neriX_analysis.neriX_analysis(file1)


#choose options
option1 = 'COLZ'
optionSame = 'SAME'


#choose cuts for run 1
currentAnalysis.add_z_cut()
currentAnalysis.add_radius_cut(0, 0.85)
currentAnalysis.add_cut('%s < %f && %s < %f' % (s1Branch, s1Max, s2Branch, s2Max))
#currentAnalysis.add_cut('log10(%s/%s) < 3.5' % (s2Branch, s1Branch)) # no NR band events above this cut so safe

# cuts to test
currentAnalysis.add_single_scatter_cut()
currentAnalysis.add_xs1asym_cut()
currentAnalysis.add_xs2asym_cut()


currentAnalysis.set_event_list()


# create functions for cuts

f_s1_asym = root.TF1('f_s1_asym', '-1.4*exp(-x/10.)+0.4', s1Min, s1Max)
f_s2_asym = root.TF1('f_s2_asym', '-0.9*exp(-x/400.)-0.05', s2Min, s2Max)
f_log_s1_s2 = root.TF1('f_log_s1_s2', '[0]*exp(-x/[1])+[2]', s1FitMin, s1Max)


# check spectrum

c1 = Canvas(name='c1', title='c1', width=600, height=500)

#create histograms for currentAnalysis

h_s1_asym = Hist2D(s1NumBins, s1Min, s1Max, asym_num_bins, asym_low, asym_high, name='h_s1_asym', title='S1 vs S1 Asymmetry - ' + file1)
currentAnalysis.Draw('%s:%s' % (s1Branch, s1_asym_branch), hist=h_s1_asym, selection=currentAnalysis.get_cuts())
h_s1_asym.GetXaxis().SetTitle('%s [PE]' % (s1Branch))
h_s1_asym.GetYaxis().SetTitle('%s' % (s1_asym_branch))
h_s1_asym.SetStats(0)
h_s1_asym.Draw('colz')

f_s1_asym.Draw('same')

c1.SetLogz()
c1.SetGridx()
c1.SetGridy()
c1.Update()


c2 = Canvas(name='c2', title='c2', width=600, height=500)

#create histograms for currentAnalysis

h_s2_asym = Hist2D(s2NumBins, s2Min, s2Max, asym_num_bins, asym_low, asym_high, name='h_s2_asym', title='S2 vs S2 Asymmetry - ' + file1)
currentAnalysis.Draw('%s:%s' % (s2Branch, s2_asym_branch), hist=h_s2_asym, selection=currentAnalysis.get_cuts())
h_s2_asym.GetXaxis().SetTitle('%s [PE]' % (s1Branch))
h_s2_asym.GetYaxis().SetTitle('%s' % (s1_asym_branch))
h_s2_asym.SetStats(0)
h_s2_asym.Draw('colz')

f_s2_asym.Draw('same')

c2.SetLogz()
c2.SetGridx()
c2.SetGridy()
c2.Update()






c4 = Canvas(name='c4', title='c4', width=600, height=500)

#create histograms for currentAnalysis

h_log_s1_s2 = Hist2D(s1NumBins, s1Min, s1Max, log_num_bins, log_low, log_high, name='h_log_s1_s2', title='Log10(S2/S1) vs S1 - ' + file1)
currentAnalysis.Draw('%s:log10(%s/%s)' % (s1Branch, s2Branch, s1Branch), hist=h_log_s1_s2, selection=currentAnalysis.get_cuts())
h_log_s1_s2.GetXaxis().SetTitle('%s [PE]' % (s1Branch))
h_log_s1_s2.GetYaxis().SetTitle('Log(%s/%s)' % (s2Branch, s1Branch))
h_log_s1_s2.SetStats(0)
h_log_s1_s2.Draw('colz')

percent_from_median = 45.
tProfile = neriX_analysis.profile_y_median(h_log_s1_s2, percent_from_median=percent_from_median)
gProfileS1 = root.TGraphAsymmErrors(*tProfile)
gProfileS1.SetFillStyle(3005)
gProfileS1.Draw('same p')

c4.SetLogz()
c4.SetGridx()
c4.SetGridy()
c4.Update()


num_points = tProfile[0]
x_values = tProfile[1]
y_upper_bound = tProfile[6]
for i, y_value in enumerate(tProfile[2]):
	y_upper_bound[i] += y_value

g_upper_bounds = root.TGraph(num_points, x_values, y_upper_bound)
g_upper_bounds.SetMarkerStyle(8)
g_upper_bounds.SetTitle('Upper Bounds @ %.2f%% - %s' % (50 + percent_from_median, file1))
g_upper_bounds.GetXaxis().SetRangeUser(s1Min, s1Max)
g_upper_bounds.GetYaxis().SetRangeUser(log_low, log_high)
g_upper_bounds.Draw('p same')

f_log_s1_s2.SetParameters(2, 3, 2)
g_upper_bounds.Fit('f_log_s1_s2', 'LLR')

f_log_s1_s2_draw = root.TF1('f_log_s1_s2_draw', '[0]*exp(-x/[1])+[2]', s1Min, s1Max)
f_log_s1_s2_draw.SetParameters(f_log_s1_s2.GetParameter(0), f_log_s1_s2.GetParameter(1), f_log_s1_s2.GetParameter(2))
f_log_s1_s2_draw.SetLineStyle(2)
f_log_s1_s2_draw.SetLineWidth(2)
f_log_s1_s2_draw.SetLineColor(root.kRed)

f_log_s1_s2_draw.Draw('same')


c4.Update()


# turn upper contour into cut and
# function for drawing

s_log_cut = 'log10(%s/%s) < (%f*exp(-%s/%f) + %f)' % (s2Branch, s1Branch, f_log_s1_s2.GetParameter(0), s1Branch, f_log_s1_s2.GetParameter(1), f_log_s1_s2.GetParameter(2))
currentAnalysis.add_cut(s_log_cut)




c3 = Canvas(name='c3', title='c3', width=600, height=500)

#create histograms for currentAnalysis

h_s1_s2 = Hist2D(s1NumBins, s1Min, s1Max, s2NumBins, s2Min, s2Max, name='h_s1_s2', title='S1 vs S2 - ' + file1)
currentAnalysis.Draw('%s:%s' % (s1Branch, s2Branch), hist=h_s1_s2, selection=currentAnalysis.get_cuts())
h_s1_s2.GetXaxis().SetTitle('%s [PE]' % (s1Branch))
h_s1_s2.GetYaxis().SetTitle('%s [PE]' % (s2Branch))
h_s1_s2.SetStats(0)
h_s1_s2.Draw('colz')

c3.SetLogz()
c3.SetGridx()
c3.SetGridy()
c3.Update()






raw_input('\nEnter to continue...\n')


