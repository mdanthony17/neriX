#!/usr/bin/python

import ROOT as root
import sys
import neriX_analysis
from rootpy.plotting import Hist2D, Hist, Legend, Canvas
from rootpy.io import root_open, File
import root_numpy
import os, re

import numpy as np
import cPickle as pickle


s1Min = 0
s1Max = 40

s2Min = 0.
s2Max = 4000


if(len(sys.argv) != 5):
	print 'Usage is python coincidence_quick_check.py <run> <anode> <cathode> <degree>'
	sys.exit(1)

run = int(sys.argv[1])
anodeSetting = float(sys.argv[2])
cathodeSetting = float(sys.argv[3])
degreeSetting = int(sys.argv[4])

s_main_save_directory = './results/%.3fkV_%ddeg/' % (cathodeSetting, degreeSetting)
if not os.path.exists(s_main_save_directory):
    os.makedirs(s_main_save_directory)

l_files_for_settings = neriX_analysis.pull_all_files_given_parameters(run, anodeSetting, cathodeSetting, degreeSetting)


current_analysis = neriX_analysis.neriX_analysis(neriX_analysis.pull_all_files_given_parameters(run, anodeSetting, cathodeSetting, degreeSetting), degreeSetting, cathodeSetting, anodeSetting)
sForHistograms = ' - %d deg, %.3f kV' % (degreeSetting, cathodeSetting)

colzOption = 'COLZ'


current_analysis.add_radius_cut(0, 0.85)
current_analysis.add_single_scatter_cut()
#current_analysis.add_xs1asym_cut() # applied later if using S1 branch
current_analysis.add_xs2asym_cut()


current_analysis.add_temp_neutron_cut(degreeSetting, b_use_not=False)



s1Branch = 'cpS1sTotBottom[]'
s2Branch = 'cpS2sTotBottom[0]'

current_analysis.add_cut('%s > 0' % s1Branch)
current_analysis.add_cut('%s > 0' % s2Branch)
current_analysis.add_z_cut(any_peak=True)
current_analysis.add_xs1asym_cut(any_peak=True)



#current_analysis.set_event_list()
num_threads = 7
current_analysis.multithread_set_event_list(num_threads)

lb_tof = -10
ub_tof = 25

l_liqsci = [0, 1, 2, 3]
l_color = ['black', 'blue', 'red', 'green']
d_liqsci = {}

c1 = Canvas()

for num_liqsci in l_liqsci:
    d_liqsci[num_liqsci] = Hist(ub_tof-lb_tof, lb_tof, ub_tof, name='h_tof_%d' % (num_liqsci), title='h_tof', color=l_color[num_liqsci])

    current_analysis.Draw('(LiqSciPeak[%d] - S1sPeak[])' % (num_liqsci), hist=d_liqsci[num_liqsci])

    d_liqsci[num_liqsci].Draw('same')

c1.Update()

raw_input('Press enter to continue...')


