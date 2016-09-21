#!/usr/bin/python
import sys, array, os
sys.path.insert(0, '..')

import ROOT as root
from rootpy.plotting import Hist, Hist2D, Canvas, Legend
import numpy as np
import corner, time
import cPickle as pickle
import fit_pmt_gain

if len(sys.argv) != 6:
    print 'Use is python run_cascade_mcmc.py <filename, mpe1> <filename, mpe2> <num walkers> <num_steps_to_include_in_plots> <num_mc_events>'
    sys.exit()

print '\n\nBy default look for all energies - change source if anything else is needed.\n'



filename_mpe1 = sys.argv[1]
filename_mpe2 = sys.argv[2]
num_walkers = int(sys.argv[3])
num_steps_to_include = int(sys.argv[4])
num_mc_events = int(sys.argv[5])

d_fit_filenames = {}
d_fit_filenames['mpe1'] = filename_mpe1
d_fit_filenames['mpe2'] = filename_mpe2

fitter = fit_pmt_gain.fit_pmt_gain(d_fit_filenames, num_mc_events=num_mc_events, b_making_comparison_plots=True)

fitter.draw_cascade_model_with_error_bands(num_walkers=num_walkers, num_steps_to_include=num_steps_to_include)
fitter.draw_cascade_model_corner_plot(num_walkers=num_walkers, num_steps_to_include=num_steps_to_include)
fitter.draw_fit_with_peaks(num_walkers=num_walkers, num_steps_to_include=num_steps_to_include)


