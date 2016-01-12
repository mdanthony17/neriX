#!/usr/bin/python

import ROOT as root
import sys, array, os, pickle
from rootpy.plotting import Hist, Hist2D, Canvas, Legend
import neriX_simulation_config
import numpy as np

if len(sys.argv) != 6:
	print 'Use is python plot_free_parameters.py <degree> <cathode> <anode> <analysis type> <num walkers>'
	sys.exit()


nameOfResultsDirectory = neriX_simulation_config.nameOfResultsDirectory
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

numBinsPY = 40
lowerBoundPY = 7
upperBoundPY = 14

#--------------- End Parameters to Change ----------------


c1 = Canvas()
hPY = Hist(numBinsPY, lowerBoundPY, upperBoundPY, name='hPY', drawstyle='hist')
hPY.fill_array(aSampler[:,:,0].flatten())
hPY.Draw()

aQuantiles = hPY.quantiles([.5-.341, .5, .5+.341])
print aQuantiles


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