#!/usr/bin/python
import sys, array, os
sys.path.insert(0, '..')

import ROOT as root
from rootpy.plotting import Hist, Hist2D, Canvas, Legend
import numpy as np
import corner, time
import cPickle as pickle
import fit_pmt_gain
import subprocess
import led_datasets


l_files = led_datasets.l_files


for files in l_files:

    print '\n\n\n\n\n\nWorking on %s and %s\n' % (files[0], files[1])

    num_walkers = '128'
    num_steps = '150'
    num_mc_events = str(int(1e5))
    num_steps_to_include = '20'

    subprocess.call(['python', 'run_cascade_mcmc.py', files[0], files[1], num_walkers, num_steps, num_mc_events])
    subprocess.call(['python', 'create_cascade_plots.py', files[0], files[1], num_walkers, num_steps_to_include, num_mc_events])


