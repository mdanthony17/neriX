#!/usr/bin/python
import sys, array, os
#sys.path.insert(0, '..')

import ROOT as root
from rootpy.plotting import Hist, Hist2D, Canvas, Legend
import nr_band_config
import numpy as np
import corner
import cPickle as pickle
import time, emcee

if len(sys.argv) != 5:
	print 'Use is python perform_full_matching.py <filename> <anode setting> <cathode setting> <num walkers> [<deviation_from_nest(efficiency fit only!!!)>]'
	sys.exit()



filename = sys.argv[1]
anode_setting = float(sys.argv[2])
cathode_setting = float(sys.argv[3])
num_walkers = int(sys.argv[4])



nameOfResultsDirectory = nr_band_config.results_directory_name
l_plots = ['plots', filename]

dir_specifier_name = '%.3fkV_%.1fkV' % (cathode_setting, anode_setting)


nameOfResultsDirectory += '/yields_fit'


sPathToFile = '%s/%s/%s/sampler_dictionary.p' % (nameOfResultsDirectory, dir_specifier_name, filename)

if os.path.exists(sPathToFile):
	dSampler = pickle.load(open(sPathToFile, 'r'))
	l_chains = []
	for sampler in dSampler[num_walkers]:
		l_chains.append(sampler['_chain'])
	a_full_chain = np.concatenate(l_chains, axis=1)
	#print a_full_chain.shape
	
	l_chains = dSampler[num_walkers][-1]['_chain'] # look at last sampler only (can change)
	print 'Successfully loaded sampler!'
else:
	print sPathToFile
	print 'Could not find file!'
	sys.exit()


print emcee.autocorr.integrated_time(np.mean(a_full_chain, axis=0), axis=0,
                                        low=10, high=None, step=1, c=2,
                                        fast=False)

"""
# need to figure this out
if not fit_efficiency:
	numDim = 36
else:
	numDim = 3

lLabelsForCorner = ['py_0', 'py_1', 'py_2', 'py_3', 'py_4', 'py_5', 'py_6', 'py_7', 'qy_0', 'qy_1', 'qy_2', 'qy_3', 'qy_4', 'qy_5', 'qy_6', 'qy_7', 'intrinsic_res_s1', 'intrinsic_res_s2', 'g1_value', 'spe_res_rv', 'g2_value', 'gas_gain_rv', 'gas_gain_width_rv', 'pf_eff_par0', 'pf_eff_par1', 's1_eff_par0', 's1_eff_par1', 's2_eff_par0', 's2_eff_par1', 'pf_stdev_par0', 'pf_stdev_par1', 'pf_stdev_par2', 'exciton_to_ion_par0_rv', 'exciton_to_ion_par1_rv', 'exciton_to_ion_par2_rv', 'scale_par']
if fit_efficiency:
	lLabelsForCorner = ['scale', 's2_eff_par0', 's2_eff_par1']

samples = aSampler[:, -5:, :].reshape((-1, numDim))

start_time = time.time()
print 'Starting corner plot...\n'
fig = corner.corner(samples, labels=lLabelsForCorner, quantiles=[0.16, 0.5, 0.84], show_titles=True, title_kwargs={"fontsize": 12})
print 'Corner plot took %.3f minutes.\n\n' % ((time.time()-start_time)/60.)

# path for save
sPathForSave = './'
for directory in l_plots:
	sPathForSave += directory + '/'

if not os.path.exists(sPathForSave):
	os.makedirs(sPathForSave)

plot_name = 'nr_band_corner_%s' % (filename)
plot_name = 'yields_fit_%s' % (plot_name)

fig.savefig('%s%s.png' % (sPathForSave, plot_name))
"""

