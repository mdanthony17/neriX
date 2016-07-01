#!/usr/bin/python
import sys, array, os
#sys.path.insert(0, '..')

import ROOT as root
from rootpy.plotting import Hist, Hist2D, Canvas, Legend
import nr_band_config
import numpy as np
import corner
import cPickle as pickle

if len(sys.argv) != 5:
	print 'Use is python perform_full_matching.py <filename> <anode setting> <cathode setting> <num walkers>'
	sys.exit()


filename = sys.argv[1]
anode_setting = float(sys.argv[2])
cathode_setting = float(sys.argv[3])
num_walkers = int(sys.argv[4])



nameOfResultsDirectory = nr_band_config.results_directory_name
l_plots = ['plots', filename]

dir_specifier_name = '%.3fkV_%.1fkV' % (cathode_setting, anode_setting)

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
numDim = 14

lLabelsForCorner = ('intrinsic_res_s1', 'intrinsic_res_s2', 'g1_rv', 'spe_res_rv', 'g2_rv', 'gas_gain_rv', 'gas_gain_width_rv', 's1_eff_par0', 's1_eff_par1', 's2_eff_par0', 's2_eff_par1', 'exciton_to_ion_par0_rv', 'exciton_to_ion_par1_rv', 'exciton_to_ion_par2_rv')

samples = aSampler[:, -50:, :].reshape((-1, numDim))
fig = corner.corner(samples, labels=lLabelsForCorner)


# path for save
sPathForSave = './'
for directory in l_plots:
	sPathForSave += directory + '/'

if not os.path.exists(sPathForSave):
	os.makedirs(sPathForSave)

fig.savefig('%snr_band_corner_%s.png' % (sPathForSave, filename))


