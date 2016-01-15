#!/usr/bin/python

import ROOT as root
import sys, array, os, pickle
from rootpy.plotting import Hist, Hist2D, Canvas, Legend
import neriX_simulation_config
import numpy as np

if len(sys.argv) != 6:
	print 'Use is python plot_free_parameters.py <degree> <cathode> <anode> <analysis type> <num walkers>'
	sys.exit()


useFakeValueInPlots = True


#nameOfResultsDirectory = neriX_simulation_config.nameOfResultsDirectory
nameOfResultsDirectory = 'fake_data/results'
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


dParametersToDraw = {'lindhard_factor':{'index':0,
									    'binning':[100, 0.1, 0.35],
										'true_value_for_fake':0.18},
					 'recombination_probability':{'index':1,
												  'binning':[100, 0.15, 0.4],
												  'true_value_for_fake':0.24},
					 'res_s1':{'index':2,
							   'binning':[100, 0.85, 1.15],
							   'true_value_for_fake':1.0},
					 'res_s2':{'index':3,
							   'binning':[100, 0.85, 1.15],
							   'true_value_for_fake':1.0}
					}



#--------------- End Parameters to Change ----------------


dPlots = {}

for parameter in dParametersToDraw:
	# initialize canvas and histogram
	dPlots[parameter] = {}
	dPlots[parameter]['canvas'] = Canvas()
	dPlots[parameter]['hist'] = Hist(dParametersToDraw[parameter]['binning'][0], dParametersToDraw[parameter]['binning'][1], dParametersToDraw[parameter]['binning'][2], name='h_%s' % parameter, title=parameter, drawstyle='hist')
	
	# fill histogram and draw it
	dPlots[parameter]['hist'].fill_array(aSampler[:,:,dParametersToDraw[parameter]['index']].flatten())
	dPlots[parameter]['hist'].Draw()

	# find quantiles
	aQuantiles = dPlots[parameter]['hist'].quantiles([.5-.341, .5, .5+.341])
	dPlots[parameter]['lines'] = [0 for i in xrange(len(aQuantiles))]
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


	dPlots[parameter]['canvas'].Update()


"""
c1 = Canvas()
hLindhard = Hist(numBinsLindhard, lowerBoundLindhard, upperBoundLindhard, name='hLindhard', drawstyle='hist')
hLindhard.fill_array(aSampler[:,:,0].flatten())
hLindhard.Draw()

aQuantiles = hLindhard.quantiles([.5-.341, .5, .5+.341])
print aQuantiles
"""

raw_input('Enter to continue...')

"""

#--------------- Start Parameters to Change ----------------

#constants for histogram
#parameter 1 will be on x-axis
parameterToExamine = '(S1sTotBottom[0]/.10 + S2sTotBottom[0]/24.)*.0137'
lowerBound = 0
upperBound = 775
nBins = 100

#choose options
option1 = ''

#--------------- End Parameters to Change ----------------

#run.get_T1().Scan('EventId:TacHeight:TrigLeftEdge-S1sPeak[0]:GeHeight[0]', run.get_cuts())


c1 = root.TCanvas('c1','c1',200,10,1000,700)
h1 = Hist(nBins, lowerBound, upperBound, name='h1', title='h1', drawstyle='hist')

run.Draw(parameterToExamine, hist=h1, selection='')
#run.get_tree().Scan('EventId:TimeTDC[4]:S2s[][15]:S2sPeak[0]')



c1.SetGridx(1)
#c1.SetLogy()

h1.SetTitle('')
h1.GetYaxis().SetTitle('Counts')
h1.GetXaxis().SetTitle(parameterToExamine)
h1.SetLineWidth(2)

h1.SetStats(0)
h1.Draw(option1)
c1.Update()


"""