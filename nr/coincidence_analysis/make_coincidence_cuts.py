#!/usr/bin/python

import ROOT as root
import sys
import neriX_analysis
from rootpy.plotting import Hist2D, Hist, Legend
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
s_scan_save_directory = './results/%.3fkV_%ddeg/reduced/' % (cathodeSetting, degreeSetting)
if not os.path.exists(s_scan_save_directory):
    os.makedirs(s_scan_save_directory)

l_files_for_settings = neriX_analysis.pull_all_files_given_parameters(run, anodeSetting, cathodeSetting, degreeSetting)


current_analysis = neriX_analysis.neriX_analysis(neriX_analysis.pull_all_files_given_parameters(run, anodeSetting, cathodeSetting, degreeSetting), degreeSetting, cathodeSetting, anodeSetting)
sForHistograms = ' - %d deg, %.3f kV' % (degreeSetting, cathodeSetting)

colzOption = 'COLZ'


current_analysis.add_radius_cut(0, 0.85)
current_analysis.add_single_scatter_cut()
#current_analysis.add_xs1asym_cut() # applied later if using S1 branch
current_analysis.add_xs2asym_cut()
current_analysis.add_temp_neutron_cut(degreeSetting)



s1Branch = 'cpS1sTotBottom[]'
s2Branch = 'cpS2sTotBottom[0]'

current_analysis.add_cut('%s > 0' % s1Branch)
current_analysis.add_cut('%s > 0' % s2Branch)
current_analysis.add_cut('%s < %f' % (s1Branch, s1Max))
current_analysis.add_cut('%s < %f' % (s2Branch, s2Max))

current_analysis.add_z_cut(any_peak=True)
current_analysis.add_s1_liqsci_peak_cut()
current_analysis.add_xs1asym_cut(any_peak=True)



#current_analysis.set_event_list()
num_threads = 7
current_analysis.multithread_set_event_list(num_threads)



#create histograms for current_analysis

print '\nUsing S1 Branch: %s \n\n' % s1Branch
print '\nUsing S2 Branch: %s \n\n' % s2Branch


num_files_used = len(current_analysis.get_lT1())
tot_num_events = 0

for i in xrange(num_files_used):
    #print current_analysis.get_filename_no_ext(i)
    tot_num_events += current_analysis.get_lT1()[i].GetEventList().GetN()

    current_analysis.get_lT1()[i].GetPlayer().SetScanRedirect(True)
    current_analysis.get_lT1()[i].GetPlayer().SetScanFileName('%s/%s.txt' % (s_scan_save_directory, current_analysis.get_filename_no_ext(i)))

    current_analysis.get_lT1()[i].Scan('EventId:cpS1sTotBottom[]:cpS2sTotBottom[0]', current_analysis.get_cuts())


l_files = os.listdir(s_scan_save_directory)

d_s1_s2 = {}
d_s1_s2['s1'] = []
d_s1_s2['s2'] = []
#event_counter = 0

s_file_tracker = ''

for current_file in l_files:
    if current_file[-3:] != 'txt':
        continue
    
    s_file_tracker += '%s\n' % (current_file[:-4])

    s_current_file = open('%s/%s' % (s_scan_save_directory, current_file)).read()
    #s_current_file += '\n'

    # get s1 and s2 strings from scan output
    l_s1_s2_strings = re.findall('\d+\.\d* \* \d+\.\d*', s_current_file)

    # break up each string in list
    for s_current in l_s1_s2_strings:
        l_s1_s2 = map(float, s_current.split(' * '))
        #print l_s1_s2
        #print event_counter, tot_num_events
        d_s1_s2['s1'].append(l_s1_s2[0])
        d_s1_s2['s2'].append(l_s1_s2[1])
        #event_counter += 1

s_file_tracker += '\n\n\n%s\n' % (current_analysis.get_cuts())

with open('%sfiles_used.txt' % (s_main_save_directory), 'w') as f_files_used:
    f_files_used.write(s_file_tracker)

d_s1_s2['s1'] = np.asarray(d_s1_s2['s1'])
d_s1_s2['s2'] = np.asarray(d_s1_s2['s2'])

print d_s1_s2['s1']
print d_s1_s2['s2']

pickle.dump(d_s1_s2, open('%ss1_s2.pkl' % (s_main_save_directory), 'w'))

