#!/usr/bin/python
import sys, array, os
sys.path.insert(0, '..')

import ROOT as root
import neriX_analysis
from rootpy.plotting import Hist, Hist2D, Canvas, Legend
import neriX_simulation_config
import numpy as np
import cPickle as pickle

if len(sys.argv) != 6 and len(sys.argv) != 9:
	print 'Use is python plot_free_parameters.py <degree> <cathode> <anode> <analysis type> <num walkers> [use fake data: t/f] [relative accidental rate] [num fake events]'
	sys.exit()



degreeSetting = int(sys.argv[1])
cathodeSetting = float(sys.argv[2])
anodeSetting = float(sys.argv[3])
sMeasurement = sys.argv[4]
numWalkers = int(sys.argv[5])

# change to switch between real and fake data

if len(sys.argv) == 9:
	if sys.argv[6] == 't':
		useFakeData = True
		relativeAccidentalRate = float(sys.argv[7])
		num_fake_events = int(sys.argv[8])
	else:
		useFakeData = False
		relativeAccidentalRate = False
		num_fake_events = -1
else:
	useFakeData = False
	relativeAccidentalRate = False
	num_fake_events = -1


if not useFakeData:
	nameOfResultsDirectory = neriX_simulation_config.nameOfResultsDirectory
	lPlots = ['plots', 'free_parameters', 'data', '%ddeg_%.3fkV_%.1fkV' % (degreeSetting, cathodeSetting, anodeSetting)]
	useFakeValueInPlots = False
	sPathToFile = './%s/%ddeg_%.3fkV_%.1fkV/%s/sampler_dictionary.p' % (nameOfResultsDirectory, degreeSetting, cathodeSetting, anodeSetting, sMeasurement)
else:
	nameOfResultsDirectory = neriX_simulation_config.pathToThisModule + '/mcmc_analysis/fake_data/results'
	lPlots = ['..', 'plots', 'free_parameters', 'fake_data', '%ddeg_%.3fkV_%.1fkV_%.2f_%d_events' % (degreeSetting, cathodeSetting, anodeSetting, relativeAccidentalRate, num_fake_events)]
	useFakeValueInPlots = True
	sPathToFile = '%s/%ddeg_%.3fkV_%.1fkV_%.2f_%d_events/%s/sampler_dictionary.p' % (nameOfResultsDirectory, degreeSetting, cathodeSetting, anodeSetting, relativeAccidentalRate, num_fake_events, sMeasurement)


pathToSamplerDictionary = nameOfResultsDirectory



if os.path.exists(sPathToFile):
	print 'Beginning to load sampler.'
	dSampler = pickle.load(open(sPathToFile, 'r'))
	aSampler = dSampler[numWalkers][-1]['_chain'] # look at last sampler only (can change)
	print 'Successfully loaded sampler!'
else:
	print sPathToFile
	print 'Could not find file!'



#--------------- Start Parameters to Change ----------------


dParametersToDraw = {'photon_yield':{'index':0,
									 'form':'P_{y}',
									 'unit': '#frac{photons}{keV}',
									 'binning':[100, 1, 7],
									 'true_value_for_fake':4.32},
					 'charge_yield':{'index':1,
									 'form':'Q_{y}',
									 'unit': '#frac{electrons}{keV}',
									 'binning':[100, 3, 10],
									 'true_value_for_fake':6.78},
					 'res_s1':{'index':2,
							   'unit': '',
							   'form':'R_{S1}',
							   'binning':[100, 0., 2],
							   'true_value_for_fake':0.3},
					 'res_s2':{'index':3,
							   'form':'R_{S2}',
							   'unit': '',
							   'binning':[100, 0., 2],
							   'true_value_for_fake':0.05}
					}



#--------------- End Parameters to Change ----------------


dPlots = {}

for parameter in dParametersToDraw:
	# initialize canvas and histogram
	dPlots[parameter] = {}
	dPlots[parameter]['canvas'] = Canvas()
	dPlots[parameter]['hist'] = Hist(dParametersToDraw[parameter]['binning'][0], dParametersToDraw[parameter]['binning'][1], dParametersToDraw[parameter]['binning'][2], name='h_%s' % parameter, title=parameter)
	dPlots[parameter]['hist'].SetMarkerSize(0)
	dPlots[parameter]['hist'].SetStats(0)
	
	# fill histogram and draw it
	dPlots[parameter]['hist'].fill_array(aSampler[:,-5:,dParametersToDraw[parameter]['index']].flatten())
	dPlots[parameter]['hist'].Draw()

	# find quantiles
	aQuantiles = dPlots[parameter]['hist'].quantiles([.5-.341, .5, .5+.341])
	dPlots[parameter]['lines'] = [0 for i in xrange(len(aQuantiles))]
	
	# create lines for plots
	for i, line in enumerate(dPlots[parameter]['lines']):
		dPlots[parameter]['lines'][i] = root.TLine(aQuantiles[i], dPlots[parameter]['hist'].GetYaxis().GetXmin(), aQuantiles[i], dPlots[parameter]['hist'].GetMaximum())
		dPlots[parameter]['lines'][i].SetLineColor(root.kBlue)
		dPlots[parameter]['lines'][i].SetLineStyle(7)
		dPlots[parameter]['lines'][i].Draw('same')
	
	if useFakeValueInPlots:
		dPlots[parameter]['lines'].append(root.TLine(dParametersToDraw[parameter]['true_value_for_fake'], dPlots[parameter]['hist'].GetYaxis().GetXmin(), dParametersToDraw[parameter]['true_value_for_fake'], dPlots[parameter]['hist'].GetMaximum()))
		dPlots[parameter]['lines'][-1].SetLineColor(root.kRed)
		dPlots[parameter]['lines'][-1].SetLineStyle(7)
		dPlots[parameter]['lines'][-1].Draw('same')


	# create text boxes for plots
	dPlots[parameter]['textbox'] = root.TPaveText(.6,.55,.88,.6,'blNDC')
	dPlots[parameter]['textbox'].AddText('%s = %.2f^{-%.2f}_{+%.2f} %s' % (dParametersToDraw[parameter]['form'], aQuantiles[1], aQuantiles[1] - aQuantiles[0], aQuantiles[2] - aQuantiles[1], dParametersToDraw[parameter]['unit']))
	dPlots[parameter]['textbox'].SetFillStyle(0)
	dPlots[parameter]['textbox'].SetBorderSize(0)
	dPlots[parameter]['textbox'].Draw('same')


	dPlots[parameter]['canvas'].Update()

for parameter in dPlots:
	nameForFile = parameter
	if useFakeData:
		nameForFile += '_fake'
		fullFilename = '%s_%ddeg_%.3fkV_%.1fkV_%.2f_%d_events' % (nameForFile, degreeSetting, cathodeSetting, anodeSetting, relativeAccidentalRate, num_fake_events)
	else:
		'%s_%ddeg_%.3fkV_%.1fkV' % (nameForFile, degreeSetting, cathodeSetting, anodeSetting)
	neriX_analysis.save_plot(lPlots, dPlots[parameter]['canvas'], fullFilename, batch_mode=True)

