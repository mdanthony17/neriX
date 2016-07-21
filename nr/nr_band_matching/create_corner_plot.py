#!/usr/bin/python
import sys, array, os
#sys.path.insert(0, '..')

import ROOT as root
from rootpy.plotting import Hist, Hist2D, Canvas, Legend
import nr_band_config
import numpy as np
import corner
import cPickle as pickle
import time

if len(sys.argv) != 5 and len(sys.argv) != 6:
	print 'Use is python perform_full_matching.py <filename> <anode setting> <cathode setting> <num walkers> [<deviation_from_nest(efficiency fit only!!!)>]'
	sys.exit()

if len(sys.argv) == 6:
	fit_efficiency = True
	deviation_from_nest = float(sys.argv[5])
else:
	fit_efficiency = False


filename = sys.argv[1]
anode_setting = float(sys.argv[2])
cathode_setting = float(sys.argv[3])
num_walkers = int(sys.argv[4])



nameOfResultsDirectory = nr_band_config.results_directory_name
l_plots = ['plots', filename]

dir_specifier_name = '%.3fkV_%.1fkV' % (cathode_setting, anode_setting)


if not fit_efficiency:
	nameOfResultsDirectory += '/yields_fit'
else:
	nameOfResultsDirectory += '/efficiency_fit/%.2f_deviation_from_nest' % (deviation_from_nest)


sPathToFile = '%s/%s/%s/sampler_dictionary.p' % (nameOfResultsDirectory, dir_specifier_name, filename)

if os.path.exists(sPathToFile):
	dSampler = pickle.load(open(sPathToFile, 'r'))
	aSampler = dSampler[num_walkers][-1]['_chain'] # look at last sampler only (can change)
	print 'Successfully loaded sampler!'
else:
	print sPathToFile
	print 'Could not find file!'
	sys.exit()



# need to figure this out
if not fit_efficiency:
	numDim = 30
else:
	numDim = 3

lLabelsForCorner = ['py_0', 'py_1', 'py_2', 'py_3', 'py_4', 'py_5', 'py_6', 'py_7', 'qy_0', 'qy_1', 'qy_2', 'qy_3', 'qy_4', 'qy_5', 'qy_6', 'qy_7', 'intrinsic_res_s1', 'intrinsic_res_s2', 'g1_value', 'spe_res_rv', 'g2_value', 'gas_gain_rv', 'gas_gain_width_rv', 's1_eff_par0', 's1_eff_par1', 's2_eff_par0', 's2_eff_par1', 'exciton_to_ion_par0_rv', 'exciton_to_ion_par1_rv', 'exciton_to_ion_par2_rv']
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
if not fit_efficiency:
	plot_name = 'yields_fit_%s' % (plot_name)
else:
	plot_name = 'efficiency_fit_%.2f_deviation_%s' % (deviation_from_nest, plot_name)

fig.savefig('%s%s.png' % (sPathForSave, plot_name))


