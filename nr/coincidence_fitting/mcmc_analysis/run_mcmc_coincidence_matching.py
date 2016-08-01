#!/usr/bin/python
import sys, array, os
sys.path.insert(0, '..')

import ROOT as root
import neriX_analysis, fit_nr
from rootpy.plotting import Hist, Hist2D, Canvas, Legend
import neriX_simulation_config, string
import numpy as np
import cPickle as pickle

if len(sys.argv) != 4:
	print 'Use is python perform_full_matching.py <cathode setting> <num walkers> <num steps>'
	sys.exit()

print '\n\nBy default look for all energies - change source if anything else is needed.\n'

cathode_setting = float(sys.argv[1])
num_walkers = int(sys.argv[2])
num_steps = int(sys.argv[3])

d_coincidence_data = {}
d_coincidence_data['degree_settings'] = [2300, 3000, 3500, 4500, 5300]
d_coincidence_data['cathode_settings'] = [0.345]


current_matching = fit_nr.fit_nr(d_coincidence_data, num_mc_events=100000)

# intrinsic_res_s1, intrinsic_res_s2, g1_rv, spe_res_rv, g2_rv, gas_gain_rv
# gas_gain_width_rv, s1_eff_par0, s1_eff_par1, s2_eff_par0, s2_eff_par1
# exciton_to_ion_par0_rv, exciton_to_ion_par1_rv, exciton_to_ion_par2_rv

current_matching.run_mcmc(num_steps=num_steps, num_walkers=num_walkers, num_threads=1, fractional_deviation_start_pos=0.1, proposal_scale=2.)





