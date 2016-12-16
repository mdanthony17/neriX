#!/usr/bin/python

import ROOT as root
import sys
import neriX_analysis
from rootpy.plotting import Hist2D, Hist, Legend, Canvas

import numpy as np

#--------------- Start Parameters to Change ----------------


psdNumBins = 100
psdMin = 0
psdMax = 0.7

lhNumBins = 100
lhMin = 0
lhMax = 2


#--------------- End Parameters to Change ----------------



if(len(sys.argv) != 5):
    print 'Usage is python coincidence_quick_check.py <run> <anode> <cathode> <degree>'
    sys.exit(1)

run = int(sys.argv[1])
anode_setting = float(sys.argv[2])
cathode_setting = float(sys.argv[3])
degree_setting = int(sys.argv[4])

l_filenames = neriX_analysis.pull_all_files_given_parameters(run, anode_setting, cathode_setting, degree_setting)
current_analysis = neriX_analysis.neriX_analysis(l_filenames, degree_setting, cathode_setting, anode_setting)

colzOption = 'COLZ'

upper_limit_er_fit = 0.2
lower_limit_er_fit = 0.

guess_nr_fit_mean = 0.35

d_psd = {}
if degree_setting == 2300 or degree_setting == 3000:
    d_psd[0] = {'h_min':0.10, 'h_max':0.70}
    d_psd[1] = {'h_min':0.20, 'h_max':1.00}
    d_psd[2] = {'h_min':0.30, 'h_max':1.45}
    d_psd[3] = {'h_min':0.50, 'h_max':1.35}
elif degree_setting == 3500 or degree_setting == 4500:
    d_psd[0] = {'h_min':0.10, 'h_max':0.70}
    d_psd[1] = {'h_min':0.20, 'h_max':1.00}
    d_psd[2] = {'h_min':0.30, 'h_max':1.45}
    d_psd[3] = {'h_min':0.50, 'h_max':1.35}
    upper_limit_er_fit = 0.11
elif degree_setting == 5300 or degree_setting == 6200:
    d_psd[0] = {'h_min':0.10, 'h_max':0.70}
    d_psd[1] = {'h_min':0.20, 'h_max':1.00}
    d_psd[2] = {'h_min':0.30, 'h_max':1.45}
    d_psd[3] = {'h_min':0.50, 'h_max':1.35}
    upper_limit_er_fit = 0.14
    lower_limit_er_fit = 0.06
    guess_nr_fit_mean = 0.25


l_column_names = ['ls_height', 'ls_psd_par']
l_columns = [np.empty(0) for i in xrange(len(l_column_names))]

current_analysis.add_single_scatter_cut()
current_analysis.add_radius_cut(0, 0.85)
current_analysis.add_xs2asym_cut()
current_analysis.add_z_cut(any_peak=True)
current_analysis.add_s1_liqsci_peak_cut()
current_analysis.add_xs1asym_cut(any_peak=True)

num_threads = 7
current_analysis.multithread_set_event_list(num_threads)
#[0]*exp(-0.5*(x-[1])^2/[2]^2) + [3]*exp(-0.5*(x-[4])^2/[5]^2)

numEJs = 4


for i in xrange(numEJs):
    d_psd[i]['canvas'] = Canvas(1280,480)
    d_psd[i]['canvas'].Divide(2, 1)


    d_psd[i]['canvas'].cd(1)

    d_psd[i]['h_2d'] = Hist2D(lhNumBins, lhMin, lhMax, psdNumBins, psdMin, psdMax, name='hEJ' + str(i), title='PSD Plot for EJ%d' % (i))
    current_analysis.Draw('LiqSciHeight[%d]:LiqSciTailRaw[%d]/LiqSciRaw[%d]' % (i, i, i), hist=d_psd[i]['h_2d'], selection='(LiqSciHeight[%d] > %f && LiqSciHeight[%d] < %f)' % (i, d_psd[i]['h_min'], i, d_psd[i]['h_max']))
    
    d_psd[i]['h_2d'].GetXaxis().SetTitle('LiqSciHeight [V]')
    d_psd[i]['h_2d'].GetYaxis().SetTitle('PSD Parameter')
    d_psd[i]['h_2d'].SetMarkerSize(0)
    d_psd[i]['h_2d'].SetStats(0)
    d_psd[i]['h_2d'].Draw()
    d_psd[i]['canvas'].Update()
    
    
    d_psd[i]['canvas'].cd(2)
    
    d_psd[i]['h_psd'] = Hist(psdNumBins, psdMin, psdMax, name='h_psd' + str(i), title='PSD Cut for EJ%d' % (i))
    current_analysis.Draw('LiqSciTailRaw[%d]/LiqSciRaw[%d]' % (i, i), hist=d_psd[i]['h_psd'], selection=('(LiqSciHeight[%d] > %f && LiqSciHeight[%d] < %f)' % (i, d_psd[i]['h_min'], i, d_psd[i]['h_max'])))
    
    d_psd[i]['h_psd'].GetXaxis().SetTitle('PSD Parameter')
    d_psd[i]['h_psd'].GetYaxis().SetTitle('Counts')
    d_psd[i]['h_psd'].SetMarkerSize(0)
    d_psd[i]['h_psd'].SetStats(0)

    d_psd[i]['h_psd'].Draw()
    
    
    
    
    # fit ER
    d_psd[i]['f_trial_er'] = root.TF1('f_trial_er', 'gaus', 0, upper_limit_er_fit)
    d_psd[i]['f_trial_er'].SetParameters(100, 0.09, 0.05)
    d_psd[i]['h_psd'].Fit('f_trial_er', 'MELRQN')
    
    trial_mean = d_psd[i]['f_trial_er'].GetParameter(1)
    trial_width = d_psd[i]['f_trial_er'].GetParameter(2)
    
    d_psd[i]['f_psd_er'] = root.TF1('f_psd_er', 'gaus', trial_mean-1.*trial_width, trial_mean+1.*trial_width)
    d_psd[i]['f_psd_er'].SetParameters(100, trial_mean, trial_width)
    d_psd[i]['h_psd'].Fit('f_psd_er', 'MELRQ0')
    d_psd[i]['f_psd_er'].Draw('same')
    
    # fit NR
    d_psd[i]['f_trial_nr'] = root.TF1('f_trial_nr', 'gaus', 0.2, 0.6)
    d_psd[i]['f_trial_nr'].SetParameters(100, guess_nr_fit_mean, 0.08)
    d_psd[i]['h_psd'].Fit('f_trial_nr', 'MELRQN')
    
    trial_mean = d_psd[i]['f_trial_nr'].GetParameter(1)
    trial_width = d_psd[i]['f_trial_nr'].GetParameter(2)
    
    d_psd[i]['f_psd_nr'] = root.TF1('f_psd_nr', 'gaus', trial_mean-1.*trial_width, trial_mean+1.*trial_width)
    d_psd[i]['f_psd_nr'].SetParameters(100, trial_mean, trial_width)
    d_psd[i]['h_psd'].Fit('f_psd_nr', 'MELRQ0')
    d_psd[i]['f_psd_nr'].Draw('same')
    
    
    d_psd[i]['canvas'].Update()

    # make 3 sigma cut from mean of ERs in PSD
    # use max of 3 sigma from mean of ER or 2 sigma from NR
    psd_cut_line = max(d_psd[i]['f_psd_er'].GetParameter(1) + 3*d_psd[i]['f_psd_er'].GetParameter(2), d_psd[i]['f_psd_nr'].GetParameter(1) - 2*d_psd[i]['f_psd_nr'].GetParameter(2))
    print 'EJ%d: %f' % (i, psd_cut_line)

    # get info for line
    
    d_psd[i]['canvas'].cd(1)
    
    d_psd[i]['line_left'] = root.TLine(d_psd[i]['h_min'], psdMax, d_psd[i]['h_min'], psd_cut_line)
    d_psd[i]['line_left'].SetLineColor(6)
    d_psd[i]['line_left'].SetLineStyle(2)
    d_psd[i]['line_left'].Draw('same')
    d_psd[i]['line_bottom'] = root.TLine(d_psd[i]['h_min'], psd_cut_line, d_psd[i]['h_max'], psd_cut_line)
    d_psd[i]['line_bottom'].SetLineColor(6)
    d_psd[i]['line_bottom'].SetLineStyle(2)
    d_psd[i]['line_bottom'].Draw('same')
    d_psd[i]['line_right'] = root.TLine(d_psd[i]['h_max'], psd_cut_line, d_psd[i]['h_max'], psdMax)
    d_psd[i]['line_right'].SetLineColor(6)
    d_psd[i]['line_right'].SetLineStyle(2)
    d_psd[i]['line_right'].Draw('same')
    
    d_psd[i]['tpt_fit'] = root.TPaveText(0.6, 0.75, 0.85, 0.85,'blNDC')
    d_psd[i]['tpt_fit'].AddText('%.2f V < H_{%d} < %.2f V' % (d_psd[i]['h_min'], i, d_psd[i]['h_max']))
    d_psd[i]['tpt_fit'].AddText('PSD_{%d} > %.3f' % (i, psd_cut_line))
    
    d_psd[i]['tpt_fit'].SetFillStyle(0)
    d_psd[i]['tpt_fit'].SetBorderSize(0)
    d_psd[i]['tpt_fit'].Draw('same')
    

    d_psd[i]['canvas'].Update()

    neriX_analysis.save_plot(['results', '%.3fkV_%ddeg' % (cathode_setting, degree_setting)], d_psd[i]['canvas'], 'psd_plots_EJ%d_%.3fkV_%ddeg' % (i, cathode_setting, degree_setting), batch_mode=True)




#raw_input("Enter to continue...")
