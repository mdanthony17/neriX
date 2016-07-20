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
	
sPathToFile = '%s/%s/%s/sampler_acceptance_fraction.p' % (nameOfResultsDirectory, dir_specifier_name, filename)
sPathToFile_autocorrelation = '%s/%s/%s/sampler_acor.p' % (nameOfResultsDirectory, dir_specifier_name, filename)

if os.path.exists(sPathToFile):
	a_acceptance_fraction = pickle.load(open(sPathToFile, 'r'))
	print 'Successfully loaded acceptance fraction array!'
	a_acor = pickle.load(open(sPathToFile_autocorrelation, 'r'))
else:
	print sPathToFile
	print 'Could not find file!'
	sys.exit()

print a_acor

c_acceptance = Canvas()
h_acceptance = Hist(100, 0, 1, name='h_acceptance', title='Acceptance Fraction of Most Recent Sampler')
h_acceptance.fill_array(a_acceptance_fraction)
h_acceptance.Scale(1./h_acceptance.Integral())
#h_acceptance.SetStats(0)
h_acceptance.Draw()
c_acceptance.Update()

raw_input('Press enter to continue...')

plot_name = 'nr_band_acceptance_fraction_%s' % (filename)
if not fit_efficiency:
	plot_name = 'yields_fit_%s' % (plot_name)
else:
	plot_name = 'efficiency_fit_%.2f_deviation_%s' % (deviation_from_nest, plot_name)

neriX_analysis.save_plot(l_plots, c_acceptance, plot_name)





