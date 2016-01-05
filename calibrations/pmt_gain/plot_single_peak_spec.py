#!/usr/bin/python

import ROOT as root
import sys, array
#import neriX_analysis
from rootpy.io import File
from rootpy.tree import Tree, TreeModel, TreeChain
from rootpy.plotting import Hist, Hist2D, Canvas, Legend
import pickle, neriX_led_analysis




if(len(sys.argv) != 5):
	print 'Usage is simple_plot <high LED filename> <low LED filename> <filename from pickle file (SPE)> <channel number>'
	sys.exit(1)


dSPEFits = pickle.load(open('completed_spe_fits.p', 'r'))

filenameHigh = '../../data/run_15/' + sys.argv[1]
filenameLow = '../../data/run_15/' + sys.argv[2]

if filenameHigh[-5:] != '.root':
	filenameHigh += '.root'
if filenameLow[-5:] != '.root':
	filenameLow += '.root'

channel = int(sys.argv[4]) - 1
speGainForChannel = dSPEFits[15][neriX_led_analysis.convert_name_to_unix_time(sys.argv[3])][channel+1]['spe_mean']

#choose cuts
xTime = 'SingleSample['+str(channel)+'] > 157 && SingleSample['+str(channel)+'] < 188'

fLedHigh = File(filenameHigh)
fLedLow = File(filenameLow)

T0_high = fLedHigh.T0
T0_low = fLedLow.T0


#--------------- Start Parameters to Change ----------------

#constants for histogram
#parameter 1 will be on x-axis
parameterToExamine ='SingleBefore[%d][1]+SingleBefore[%d][0]+SinglePeak[%d]+SingleAfter[%d][0]+SingleAfter[%d][1]' % (5*(channel,))
lowerBound = 0
upperBoundHigh = 5*150e6
#upperBound = 3*150e6
upperBoundLow = 8*20e6
nBins = 400

#choose options
option1 = ''

#--------------- End Parameters to Change ----------------

#run.get_tree().Scan('EventId:TrigArea:TrigLeftEdge-S1sPeak[0]:GeHeight[0]', run.get_cuts())


c1 = root.TCanvas('c1','c1',200,10,2000,700)
c1.Divide(2)

hHigh = Hist(nBins, lowerBound, upperBoundHigh, name='hHigh', title='hHigh', drawstyle='hist')
T0_high.Draw(parameterToExamine, hist=hHigh, selection=xTime)

c1.cd(1)
c1.SetGridx(1)
hHigh.SetTitle('High LED Voltage')
hHigh.GetYaxis().SetTitle('Counts')
hHigh.GetXaxis().SetTitle(parameterToExamine)
hHigh.SetLineWidth(2)

fHigh = root.TF1('fHigh','gaus', lowerBound, upperBoundHigh)
hHigh.Fit('fHigh', 'LE')

hHigh.SetStats(0)
hHigh.Draw(option1)
fHigh.Draw('same')
c1.Update()



hLow = Hist(nBins, lowerBound, upperBoundLow, name='hLow', title='hLow', drawstyle='hist')
T0_low.Draw(parameterToExamine, hist=hLow, selection=xTime)

c1.cd(2)
c1.SetGridx(1)
hLow.SetTitle('Low LED Voltage')
hLow.GetYaxis().SetTitle('Counts')
hLow.GetXaxis().SetTitle(parameterToExamine)
hLow.SetLineWidth(2)

fLow = root.TF1('fLow','gaus', lowerBound, upperBoundLow)
hLow.Fit('fLow', 'LE')

hLow.SetStats(0)
hLow.Draw(option1)
fLow.Draw('same')
c1.Update()


raw_input('If fits okay press enter and copy and paste line below to wiki...')



print '\n\n$ \\frac{%.2E}{\\frac{%.2E}{%.2E}} = %.2E $\n\n' % (fLow.GetParameter(1), fHigh.GetParameter(1), speGainForChannel, fLow.GetParameter(1)*speGainForChannel/fHigh.GetParameter(1))

