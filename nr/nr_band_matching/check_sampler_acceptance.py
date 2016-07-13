#!/usr/bin/python
import sys, array, os
#sys.path.insert(0, '..')

import ROOT as root
from rootpy.plotting import Hist, Hist2D, Canvas, Legend
import nr_band_config, neriX_analysis
import numpy as np
import corner
import cPickle as pickle
import time

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

sPathToFile = '%s/%s/%s/sampler_acceptance_fraction.p' % (nameOfResultsDirectory, dir_specifier_name, filename)

if os.path.exists(sPathToFile):
	a_acceptance_fraction = pickle.load(open(sPathToFile, 'r'))
	print 'Successfully loaded acceptance fraction array!'
else:
	print sPathToFile
	print 'Could not find file!'
	sys.exit()



c_acceptance = Canvas()
h_acceptance = Hist(100, 0, 1, name='h_acceptance', title='Acceptance Fraction of Most Recent Sampler')
h_acceptance.fill_array(a_acceptance_fraction)
h_acceptance.Scale(1./h_acceptance.Integral())
h_acceptance.SetStats(0)
h_acceptance.Draw()
c_acceptance.Update()

raw_input('Press enter to continue...')


neriX_analysis.save_plot(l_plots, c_acceptance, 'nr_band_acceptance_fraction_%s' % (filename))





