#!/usr/bin/python

import ROOT as root
import sys, array
import neriX_analysis
from rootpy.io import File
from rootpy.tree import Tree, TreeModel, TreeChain
from rootpy.plotting import Hist, Hist2D, Canvas, Legend

if(len(sys.argv) != 3):
	print 'Usage is simple_plot <ROOT filename> <channel number>'
	sys.exit(1)



filename = '../../data/run_15/' + sys.argv[1]

if filename[-5:] != '.root':
	filename += '.root'

channel = int(sys.argv[2]) - 1

#choose cuts
xTime = 'SingleSample['+str(channel)+'] > 157 && SingleSample['+str(channel)+'] < 188'

fLed = File(filename)

T0 = fLed.T0


#--------------- Start Parameters to Change ----------------

#constants for histogram
#parameter 1 will be on x-axis
parameterToExamine ='SingleBefore[%d][1]+SingleBefore[%d][0]+SinglePeak[%d]+SingleAfter[%d][0]+SingleAfter[%d][1]' % (5*(channel,))
lowerBound = 0
#upperBound = 6*150e6
#upperBound = 3*150e6
upperBound = 4*20e6
nBins = 400

#choose options
option1 = ''

#--------------- End Parameters to Change ----------------

#run.get_tree().Scan('EventId:TrigArea:TrigLeftEdge-S1sPeak[0]:GeHeight[0]', run.get_cuts())


c1 = root.TCanvas('c1','c1',200,10,1000,700)
h1 = Hist(nBins, lowerBound, upperBound, name='h1', title='h1', drawstyle='hist')

T0.Draw(parameterToExamine, hist=h1, selection=xTime)


c1.SetGridx(1)
#c1.SetLogy()

h1.SetTitle('')
h1.GetYaxis().SetTitle('Counts')
h1.GetXaxis().SetTitle(parameterToExamine)
h1.SetLineWidth(2)

#1600: fit gives 8.58410e+07
#1605: fit gives 6.36948e+06

f1 = root.TF1('f1','gaus', lowerBound, upperBound)
h1.Fit('f1', 'LE')


#h1.SetStats(0)
h1.Draw(option1)
f1.Draw('same')
c1.Update()


raw_input('Enter to continue...')

