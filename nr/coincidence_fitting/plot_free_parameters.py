#!/usr/bin/python

import ROOT as root
import sys, array, os
from rootpy.plotting import Hist, Hist2D, Canvas, Legend
import neriX_simulation_config
import numpy as np
import cPickle as pickle

if len(sys.argv) != 6:
	print 'Use is python plot_free_parameters.py <degree> <cathode> <anode> <analysis type> <num walkers>'
	sys.exit()


useFakeValueInPlots = True


nameOfResultsDirectory = neriX_simulation_config.nameOfResultsDirectory
#nameOfResultsDirectory = 'fake_data/results'
pathToSamplerDictionary = nameOfResultsDirectory

degreeSetting = int(sys.argv[1])
cathodeSetting = float(sys.argv[2])
anodeSetting = float(sys.argv[3])
sMeasurement = sys.argv[4]
numWalkers = int(sys.argv[5])

sPathToFile = './%s/%ddeg_%.3fkV_%.1fkV/%s/sampler_dictionary.p' % (nameOfResultsDirectory, degreeSetting, cathodeSetting, anodeSetting, sMeasurement)

if os.path.exists(sPathToFile):
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
									 'binning':[100, 2, 13],
									 'true_value_for_fake':7.6},
					 'charge_yield':{'index':1,
									 'form':'Q_{y}',
									 'unit': '#frac{electrons}{keV}',
									 'binning':[100, 3, 10],
									 'true_value_for_fake':5.3},
					 'res_s1':{'index':2,
							   'unit': '',
							   'form':'R_{S1}',
							   'binning':[100, 0., 2],
							   'true_value_for_fake':1.0},
					 'res_s2':{'index':3,
							   'form':'R_{S2}',
							   'unit': '',
							   'binning':[100, 0., 2],
							   'true_value_for_fake':1.0}
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




raw_input('Enter to continue...')
