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
    print 'Use is python run_cascade_mcmc.py <filename, mpe1> <filename, mpe2> <num walkers> <num_steps> <num_mc_events>'
    sys.exit()

print '\n\nBy default look for all energies - change source if anything else is needed.\n'



filename_mpe1 = sys.argv[1]
filename_mpe2 = sys.argv[2]
num_walkers = int(sys.argv[3])
num_steps = int(sys.argv[4])
num_mc_events = int(sys.argv[5])

d_fit_filenames = {}
d_fit_filenames['mpe1'] = filename_mpe1
d_fit_filenames['mpe2'] = filename_mpe2

fitter = fit_pmt_gain.fit_pmt_gain(d_fit_filenames, num_mc_events=num_mc_events, b_making_comparison_plots=True)

fitter.suppress_likelihood()

#a_bounds = [(0.75, 1), (24, 26), (0.12, 0.13), (1e3, 1e5), (5e4, 5e5), (1e4, 1e6), (0, 1), (0.6, 2), (1.5, 3.0)]
#fitter.differential_evolution_minimizer(a_bounds, maxiter=50, tol=0.01, popsize=10, polish=False)

fitter.run_cascade_model_mcmc(num_walkers=num_walkers, num_steps=num_steps, threads=1)

