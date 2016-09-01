#!/usr/bin/python
import sys, array, os
sys.path.insert(0, '..')

import ROOT as root
from rootpy.plotting import Hist, Hist2D, Canvas, Legend
import neriX_simulation_config
import numpy as np
import corner
import cPickle as pickle
import time, emcee

if len(sys.argv) != 4:
    print 'Use is python perform_full_matching.py <cathode setting> <num walkers> <fit type>'
    sys.exit()

print '\n\nBy default look for all energies - change source if anything else is needed.\n'



cathode_setting = float(sys.argv[1])
num_walkers = int(sys.argv[2])
fit_type = sys.argv[3]

if fit_type == 's':
    directory_descriptor = 'single_energy'
elif fit_type == 'm':
    directory_descriptor = 'multiple_energies'
elif fit_type == 'ml':
    directory_descriptor = 'multiple_energies_lindhard_model'

l_degree_settings_in_use = [2300, 3000, 3500, 4500, 5300]
s_degree_settings = ''
for degree_setting in l_degree_settings_in_use:
    s_degree_settings += '%s,' % (degree_setting)
s_degree_settings = s_degree_settings[:-1]

nameOfResultsDirectory = neriX_simulation_config.results_directory_name
l_plots = ['plots']

dir_specifier_name = '%.3f_kV_%s_deg' % (cathode_setting, s_degree_settings)

nameOfResultsDirectory += '/%s' % (directory_descriptor)

sPathToFile = './%s/%s/sampler_dictionary.p' % (nameOfResultsDirectory, dir_specifier_name)

if os.path.exists(sPathToFile):
    dSampler = pickle.load(open(sPathToFile, 'r'))
    l_chains = []
    for sampler in dSampler[num_walkers]:
        l_chains.append(sampler['_chain'])
    a_full_chain = np.concatenate(l_chains, axis=1)
    #print a_full_chain.shape

    print 'Successfully loaded sampler!'
else:
    print sPathToFile
    print 'Could not find file!'
    sys.exit()

print emcee.autocorr.integrated_time(np.mean(a_full_chain, axis=0), axis=0,
                                        low=10, high=None, step=1, c=2,
                                        fast=False)




