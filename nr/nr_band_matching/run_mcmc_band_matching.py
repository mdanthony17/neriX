#!/usr/bin/python
import sys, array, os
#sys.path.insert(0, '..')

import ROOT as root
import neriX_analysis, fit_nr_band
from rootpy.plotting import Hist, Hist2D, Canvas, Legend
import nr_band_config, string
import numpy as np
import cPickle as pickle

if len(sys.argv) != 7:
	print 'Use is python perform_full_matching.py <filename> <anode setting> <cathode setting> <num walkers> <num steps> <num threads>'
	sys.exit()


filename = sys.argv[1]
anode_setting = float(sys.argv[2])
cathode_setting = float(sys.argv[3])
num_walkers = int(sys.argv[4])
num_steps = int(sys.argv[5])
num_threads = int(sys.argv[6])


current_nr_band_matching = fit_nr_band.nr_band_fitter(filename, anode_setting, cathode_setting)

# intrinsic_res_s1, intrinsic_res_s2, g1_rv, spe_res_rv, g2_rv, gas_gain_rv
# gas_gain_width_rv, s1_eff_par0, s1_eff_par1, s2_eff_par0, s2_eff_par1
# exciton_to_ion_par0_rv, exciton_to_ion_par1_rv, exciton_to_ion_par2_rv

current_nr_band_matching.fit_nr_band_nest(num_steps=num_steps, num_walkers=num_walkers, num_threads=num_threads)





