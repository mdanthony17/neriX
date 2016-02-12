#!/usr/bin/python
import sys, array, os
sys.path.insert(0, '..')

import ROOT as root
import neriX_analysis, neriX_simulation_analysis
from rootpy.plotting import Hist, Hist2D, Canvas, Legend
import neriX_simulation_config, string
import numpy as np
import cPickle as pickle

if len(sys.argv) != 7 and len(sys.argv) != 8:
	print 'Use is python perform_full_matching.py <degree> <cathode> <anode> <num walkers> <num steps> <num threads> [<use fake data>: t/f]'
	sys.exit()



degreeSetting = int(sys.argv[1])
cathodeSetting = float(sys.argv[2])
anodeSetting = float(sys.argv[3])
numWalkers = int(sys.argv[4])
numSteps = int(sys.argv[5])
numThreads = int(sys.argv[6])

try:
	if string.lower(sys.argv[7]) == 't':
		usingFakeData = True
	else:
		usingFakeData = False
except:
	usingFakeData = False


full_matching_analysis = neriX_simulation_analysis.neriX_simulation_analysis(15, 4.5, 1.054, 45, use_fake_data=usingFakeData)

sParametersFullMatching = (('photon_yield', 10.), ('charge_yield', 8.), ('res_s1', 0.3), ('res_s2', 0.1), ('n_g1', 0), ('n_res_spe', 0), ('n_par0_tac_eff', 0), ('n_par0_pf_eff', 0), ('n_par1_pf_eff', 0), ('n_g2', 0), ('n_gas_gain_mean', 0), ('n_gas_gain_width', 0), ('n_par0_trig_eff', 0), ('n_par1_trig_eff', 0), ('n_par0_e_to_i', 0), ('n_par1_e_to_i', 0), ('n_par2_e_to_i', 0))

full_matching_analysis.run_mcmc('full_matching', sParametersFullMatching, numWalkers=numWalkers, numSteps=numSteps, numThreads=numThreads, gpu_compute=False)





