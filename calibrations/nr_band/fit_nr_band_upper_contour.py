#!/usr/bin/python

import ROOT as root
import sys
import neriX_analysis
from rootpy.plotting import Hist2D, Hist, Legend, Canvas
import math, root_numpy
import numpy as np





#--------------- Start Parameters to Change ----------------


s1NumBins = 24
s1Min = 0
s1Max = 24
s1FitMin = 2

s2NumBins = 40
s2Min = 0
s2Max = 2400


s1Branch = 'cpS1sTotBottom[0]'
s2Branch = 'cpS2sTotBottom[0]'


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
currentAnalysis.add_single_scatter_cut()
currentAnalysis.add_xs1asym_cut()
currentAnalysis.add_xs2asym_cut()

#currentAnalysis.add_cut('%s < 4. && %s > 1000 && %s < 2400' % (s1Branch, s2Branch))
#currentAnalysis.get_T1().Scan('EventId:%s:%s:S1sPeak[0]:S2sPeak[0]' % (s1Branch, s2Branch), currentAnalysis.get_cuts())

currentAnalysis.set_event_list()



# check spectrum

c1 = Canvas(name='c1', title='c1', width=1200, height=500)
c1.Divide(2)




c1.cd(1)

#create histograms for currentAnalysis

hS1S2 = Hist2D(s1NumBins, s1Min, s1Max, s2NumBins, s2Min, s2Max, name='hS1S2', title='S1 vs S2 - ' + file1)
currentAnalysis.Draw('%s:%s' % (s1Branch, s2Branch), hist=hS1S2, selection=currentAnalysis.get_cuts())
hS1S2.GetXaxis().SetTitle('%s [PE]' % (s1Branch))
hS1S2.GetYaxis().SetTitle('%s [PE]' % (s2Branch))
hS1S2.SetStats(0)
hS1S2.Draw('colz')

percent_from_median = 45.
tProfile = neriX_analysis.profile_y_median(hS1S2, percent_from_median=percent_from_median)
gProfileS1 = root.TGraphAsymmErrors(*tProfile)
#gProfileS1.SetFillStyle(3005)
gProfileS1.Draw('same p')



c1.GetPad(1).SetLogz()
c1.GetPad(1).SetGridx()
c1.GetPad(1).SetGridy()
c1.Update()



c1.cd(2)

num_points = tProfile[0]
x_values = tProfile[1]
y_upper_bound = tProfile[6]
for i, y_value in enumerate(tProfile[2]):
	y_upper_bound[i] += y_value

g_upper_bounds = root.TGraph(num_points, x_values, y_upper_bound)
g_upper_bounds.SetMarkerStyle(8)
g_upper_bounds.SetTitle('Upper Bounds @ %.2f%% - %s' % (50 + percent_from_median, file1))
g_upper_bounds.GetXaxis().SetRangeUser(s1Min, s1Max)
g_upper_bounds.GetYaxis().SetRangeUser(s2Min, s2Max)
g_upper_bounds.Draw('ap')


c1.GetPad(2).SetLogz()
c1.GetPad(2).SetGridx()
c1.GetPad(2).SetGridy()

c1.Update()


# 2d fit does not work well

f1 = root.TF1('f1', 'pol2', s1FitMin, s1Max)

#hProfile.Fit('f1', 'MELR')
fitResult = g_upper_bounds.Fit('f1', 'MELRS')
f1.SetLineStyle(1)
f1.SetLineWidth(2)
f1.SetLineColor(root.kRed)

c1.cd(1)
f1.Draw('same')
c1.cd(2)
f1.Draw('same')


fDraw = root.TF1('f1', 'pol2', s1Min, s1Max)
fDraw.SetParameters(f1.GetParameter(0), f1.GetParameter(1), f1.GetParameter(2))
fDraw.SetLineStyle(2)
fDraw.SetLineWidth(2)
fDraw.SetLineColor(root.kRed)

c1.cd(1)
fDraw.Draw('same')
c1.cd(2)
fDraw.Draw('same')


c1.Update()

raw_input('\nEnter to continue...\n')


print '\n\nBelow is the fit needed for the NR Band\n'
print '((%%s > %.1f) || (%%s < (%.3e + %.3e*%%s + %.3e*pow(%%s, 2.))))' % (s1Max, f1.GetParameter(0), f1.GetParameter(1), f1.GetParameter(2))
print '\n\n'



#c1.SaveAs('~/Desktop/electron_lifetime.png')

neriX_analysis.save_plot(['results', 'run_%d' % currentAnalysis.get_run(), currentAnalysis.get_filename()[:-5]], c1, 'nr_band_fit_%s' % currentAnalysis.get_filename()[:-5])

