#!/usr/bin/python
import sys, array, os
sys.path.insert(0, '..')

import ROOT as root
from rootpy.plotting import Hist, Hist2D, Canvas, Legend
import neriX_simulation_config
import numpy as np
import corner
import cPickle as pickle

if len(sys.argv) != 6:
	print 'Use is python plot_free_parameters.py <degree> <cathode> <anode> <analysis type> <num walkers>'
	sys.exit()



#nameOfResultsDirectory = neriX_simulation_config.nameOfResultsDirectory
nameOfResultsDirectory = '../fake_data/results'
pathToSamplerDictionary = nameOfResultsDirectory

degreeSetting = int(sys.argv[1])
cathodeSetting = float(sys.argv[2])
anodeSetting = float(sys.argv[3])
sMeasurement = sys.argv[4]
numWalkers = int(sys.argv[5])


# change to switch between real and fake data
realDate = True

if realDate:
	nameOfResultsDirectory = neriX_simulation_config.nameOfResultsDirectory
	lPlots = ['plots', 'corner_plots', 'data']
else:
	nameOfResultsDirectory = '../fake_data/results'
	lPlots = ['plots', 'corner_plots', 'fake_data']




sPathToFile = './%s/%ddeg_%.3fkV_%.1fkV/%s/sampler_dictionary.p' % (nameOfResultsDirectory, degreeSetting, cathodeSetting, anodeSetting, sMeasurement)

if os.path.exists(sPathToFile):
	dSampler = pickle.load(open(sPathToFile, 'r'))
	aSampler = dSampler[numWalkers][-1]['_chain'] # look at last sampler only (can change)
	print 'Successfully loaded sampler!'
else:
	print sPathToFile
	print 'Could not find file!'



# need to figure this out
numDim = 18

lLabelsForCorner = ('photon_yield', 'charge_yield', 'res_s1', 'res_s2', 'n_g1', 'n_res_spe', 'n_par0_tac_eff', 'n_par1_tac_eff', 'n_par0_pf_eff', 'n_par1_pf_eff', 'n_g2', 'n_gas_gain_mean', 'n_gas_gain_width', 'n_par0_trig_eff', 'n_par1_trig_eff', 'n_par0_e_to_i', 'n_par1_e_to_i', 'n_par2_e_to_i')

samples = aSampler[:, -5:, :].reshape((-1, numDim))
fig = corner.corner(samples, labels=lLabelsForCorner)
#fig = corner.corner(samples)

# path for save
sPathForSave = './'
for directory in lPlots:
	sPathForSave += directory + '/'

fig.savefig('%s/corner_plot_%ddeg_%.3fkV_%.1fkV.png' % (sPathForSave, degreeSetting, cathodeSetting, anodeSetting))



#raw_input('Enter to continue...')
