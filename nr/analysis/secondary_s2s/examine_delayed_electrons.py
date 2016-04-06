#!/usr/bin/python

import ROOT as root
import sys
import neriX_analysis
from rootpy.plotting import Hist

if(len(sys.argv) < 2):
	print 'Usage is simple_plot <ROOT filename>'
	sys.exit(1)

filename = sys.argv[1]
current_analysis = neriX_analysis.neriX_analysis(filename)
filename = current_analysis.get_filename()[:-5]


#choose cuts
current_analysis.add_radius_cut(0., 0.3)
current_analysis.add_single_scatter_cut()
current_analysis.add_cut('(S2sPeak[]-S1sPeak[0])/100. > 2 && (S2sPeak[]-S1sPeak[0])/100. < 12')
current_analysis.add_cut('S2sPeak[] != S2sPeak[0]')

current_analysis.set_event_list()


#--------------- Start Parameters to Change ----------------

#constants for histogram
#parameter 1 will be on x-axis
# range for 12/30 PMT5 out (1420019400, 1420020000)
# range for 12/30 PMT3 out (1420002200, 1420002310)
dt_s2 = '(S2sPeak[]-S2sPeak[0])/100.' # need to correct in Z
lowerBound_dt_s2 = 0
upperBound_dt_s2 = 20
nBins_dt_s2 = 20

#parameter 2 will be on y-axis
dx = 'S2sPosCoG[][0]-S2sPosCoG[0][0]'
title_dx = 'dx_{CoG}'
lowerBound_dx = -0.6
upperBound_dx = 0.6
nBins_dx = 40

dy = 'S2sPosCoG[][1]-S2sPosCoG[0][1]'
title_dy = 'dy_{CoG}'
lowerBound_dy = -0.6
upperBound_dy = 0.6
nBins_dy = 40

dr = '( (S2sPosCoG[][0]-S2sPosCoG[0][0])^2. + (S2sPosCoG[][1]-S2sPosCoG[0][1])^2. )^0.5'
title_dr = 'dr_{CoG}'
lowerBound_dr = 0
upperBound_dr = 0.6
nBins_dr = 20

x = 'S2sPosCoG[][0]'
title_x = 'x_{CoG}'
lowerBound_x = -0.6
upperBound_x = 0.6
nBins_x = 40

y = 'S2sPosCoG[][1]'
title_y = 'y_{CoG}'
lowerBound_y = -0.6
upperBound_y = 0.6
nBins_y = 40

#choose options
option1 = 'COLZ'

#--------------- End Parameters to Change ----------------

#current_analysis.get_T1().Scan('EventId:(S2sPeak[]-S2sPeak[0])/100.:S2sPosCoG[][0]-S2sPosCoG[0][0]', current_analysis.get_cuts())

c1 = root.TCanvas('c1','c1',200,200,1000,1000)
hdx = root.TH2F('hdx', 'hdx', nBins_dt_s2, lowerBound_dt_s2, upperBound_dt_s2, nBins_dx, lowerBound_dx, upperBound_dx)
hdx.SetTitle(title_dx + ' - ' + filename)
hdx.GetXaxis().SetTitle(dt_s2)
hdx.GetYaxis().SetTitle(title_dx)

current_analysis.Draw(dt_s2 + ':' + dx + '', hist=hdx, selection=current_analysis.get_cuts())
hdx.Draw('colz')

hdx.SetStats(0)
hdx.Draw(option1)
#c1.SetLogz()
c1.Update()


c2 = root.TCanvas('c2','c2',200,200,1000,1000)
hdy = root.TH2F('hdy', 'hdy', nBins_dt_s2, lowerBound_dt_s2, upperBound_dt_s2, nBins_dy, lowerBound_dy, upperBound_dy)
hdy.SetTitle(title_dy + ' - ' + filename)
hdy.GetXaxis().SetTitle(dt_s2)
hdy.GetYaxis().SetTitle(title_dy)

current_analysis.Draw(dt_s2 + ':' + dy + '', hist=hdy, selection=current_analysis.get_cuts())
hdy.Draw('colz')

hdy.SetStats(0)
hdy.Draw(option1)
#c2.SetLogz()
c2.Update()



c3 = root.TCanvas('c3','c3',200,200,1000,1000)
hdr = root.TH2F('hdr', 'hdr', nBins_dt_s2, lowerBound_dt_s2, upperBound_dt_s2, nBins_dr, lowerBound_dr, upperBound_dr)
hdr.SetTitle(title_dr + ' - ' + filename)
hdr.GetXaxis().SetTitle(dt_s2)
hdr.GetYaxis().SetTitle(title_dr)

current_analysis.Draw(dt_s2 + ':' + dr + '', hist=hdr, selection=current_analysis.get_cuts())
hdr.Draw('colz')

hdr.SetStats(0)
hdr.Draw(option1)
#c3.SetLogz()
c3.Update()



c4 = root.TCanvas('c4','c4',200,200,1000,1000)
hxy = root.TH2F('hxy', 'hxy', nBins_dx, lowerBound_dx, upperBound_dx, nBins_dy, lowerBound_dy, upperBound_dy)
hxy.SetTitle('XY of S2s After Main - ' + filename)
hxy.GetXaxis().SetTitle(title_x)
hxy.GetYaxis().SetTitle(title_y)

current_analysis.Draw(x + ':' + y + '', hist=hxy, selection=current_analysis.get_cuts())
hxy.Draw('colz')

hxy.SetStats(0)
hxy.Draw(option1)
c4.SetLogz()
c4.Update()

"""
c5 = root.TCanvas('c5','c5',200,200,1000,1000)
hr2 = Hist(nBins_dr, lowerBound_dr**2, upperBound_dr**2, name='hr2', title='hr2')
hr2.SetTitle('R^2 of S2s After Main - ' + filename)
hr2.GetXaxis().SetTitle('R_{CoG}^{2}')
hr2.GetYaxis().SetTitle('Counts')

current_analysis.Draw('(%s^2. + %s^2.)^0.5' % (x, y), hist=hr2, selection=current_analysis.get_cuts())
hr2.Draw()
hr2.SetStats(0)
c5.Update()
"""


raw_input('Enter to continue...')

neriX_analysis.save_plot(['results', 'run_%d' % current_analysis.get_run(), current_analysis.get_filename_no_ext()], c1, 'dx_vs_s2_dt_%s' % current_analysis.get_filename_no_ext())
neriX_analysis.save_plot(['results', 'run_%d' % current_analysis.get_run(), current_analysis.get_filename_no_ext()], c2, 'dy_vs_s2_dt_%s' % current_analysis.get_filename_no_ext())
neriX_analysis.save_plot(['results', 'run_%d' % current_analysis.get_run(), current_analysis.get_filename_no_ext()], c3, 'dr_vs_s2_dt_%s' % current_analysis.get_filename_no_ext())
neriX_analysis.save_plot(['results', 'run_%d' % current_analysis.get_run(), current_analysis.get_filename_no_ext()], c4, 'xy_secondary_s2s_%s' % current_analysis.get_filename_no_ext())


