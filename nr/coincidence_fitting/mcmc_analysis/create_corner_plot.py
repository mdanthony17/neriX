#!/usr/bin/python
import sys, array, os
sys.path.insert(0, '..')

import ROOT as root
from rootpy.plotting import Hist, Hist2D, Canvas, Legend
import neriX_simulation_config
import numpy as np
import corner
import cPickle as pickle

if len(sys.argv) != 6 and len(sys.argv) != 9 and len(sys.argv) != 7 and len(sys.argv) != 10:
	print 'Use is python plot_free_parameters.py <degree> <cathode> <anode> <analysis type> <num walkers> [<use fake data>: t/f] [relative accidental rate] [num fake events] [name_notes]'
	sys.exit()

print '\n\nNEED TO ADJUST CODE FOR FAKE DATA ACCIDENTAL RATE AND NUM EVENTS!!!'


checkFakeData = False
useNameNote = False
if len(sys.argv) == 10:
	checkFakeData = True
	useNameNote = True
elif len(sys.argv) == 9:
	checkFakeData = True
elif len(sys.argv) == 7:
	useNameNote = True


degreeSetting = int(sys.argv[1])
cathodeSetting = float(sys.argv[2])
anodeSetting = float(sys.argv[3])
sMeasurement = sys.argv[4]
numWalkers = int(sys.argv[5])


# change to switch between real and fake data
if checkFakeData:
	if sys.argv[6] == 't':
		useFakeData = True
		relativeAccidentalRate = float(sys.argv[7])
		numFakeEvents = int(sys.argv[8])
	else:
		useFakeData = False
		relativeAccidentalRate = False
		numFakeEvents = -1

if useNameNote:
	if checkFakeData:
		name_notes = sys.argv[9]
	else:
		name_notes = sys.argv[6]

sForNameInFake = ''

if not useFakeData:
	nameOfResultsDirectory = neriX_simulation_config.nameOfResultsDirectory
	lPlots = ['plots', 'corner_plots', 'data']
	useFakeValueInPlots = False
else:
	nameOfResultsDirectory = 'fake_data/results'
	lPlots = ['plots', 'corner_plots', 'fake_data']
	useFakeValueInPlots = True
	sForNameInFake = '_fake'


dir_specifier_name = '%ddeg_%.3fkV_%.1fkV' % (degreeSetting, cathodeSetting, anodeSetting)
if useFakeData:
	dir_specifier_name += '_%.2f_%d_events' % (relativeAccidentalRate, numFakeEvents)
if useNameNote:
	dir_specifier_name += '_' + name_notes

sPathToFile = './%s/%s/%s/sampler_dictionary.p' % (nameOfResultsDirectory, dir_specifier_name, sMeasurement)

if os.path.exists(sPathToFile):
	dSampler = pickle.load(open(sPathToFile, 'r'))
	aSampler = dSampler[numWalkers][-1]['_chain'] # look at last sampler only (can change)
	print 'Successfully loaded sampler!'
else:
	print sPathToFile
	print 'Could not find file!'



# need to figure this out
numDim = 17

lLabelsForCorner = ('photon_yield', 'charge_yield', 'res_s1', 'res_s2', 'n_g1', 'n_res_spe', 'n_par0_tac_eff', 'n_par0_pf_eff', 'n_par1_pf_eff', 'n_g2', 'n_gas_gain_mean', 'n_gas_gain_width', 'n_par0_trig_eff', 'n_par1_trig_eff', 'n_par0_e_to_i', 'n_par1_e_to_i', 'n_par2_e_to_i')

samples = aSampler[:, -5:, :].reshape((-1, numDim))
fig = corner.corner(samples, labels=lLabelsForCorner)
#fig = corner.corner(samples)

# path for save
sPathForSave = './'
for directory in lPlots:
	sPathForSave += directory + '/'

fig.savefig('%scorner_plot%s_%s.png' % (sPathForSave, sForNameInFake, dir_specifier_name))



#raw_input('Enter to continue...')
