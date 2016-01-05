#!/usr/bin/python

import ROOT as root
import sys, array
import neriX_analysis
from rootpy.plotting import Hist, Hist2D, Canvas, Legend

if(len(sys.argv) != 3):
	print 'Usage is simple_plot <ROOT filename> <tdc channel>'
	sys.exit(1)

filename = sys.argv[1]
tdcChan = sys.argv[2]

run = neriX_analysis.neriX_analysis(filename)

#choose options
option1 = 'COLZ'

dChannels = {'0':'18',
			 '1':'10',
			 '2':'13',
			 '3':'9',
			 '4':'14',
			 '5':'8'
			 }


#choose cuts
#run.add_cut('SingleSample['+str(i)+'] > 157 && SingleSample['+str(i)+'] < 188')
#run.add_dt_cut(2., 13.)
#run.add_radius_cut(0., 25.)
run.add_cut('TimeTDC[' + tdcChan + '] > 0')
run.add_cut('S1s[][' + dChannels[tdcChan] + '] < 0.5')

run.set_event_list()


#--------------- Start Parameters to Change ----------------

#constants for histogram
#parameter 1 will be on x-axis
parameterToExamine = 'TimeTDC[' + tdcChan + ']'
lowerBound = 0
upperBound = 210
nBins = 100

#choose options
option1 = ''

#--------------- End Parameters to Change ----------------

#run.get_tree().Scan('EventId:TrigArea:TrigLeftEdge-S1sPeak[0]:GeHeight[0]', run.get_cuts())


c1 = root.TCanvas('c1','c1',200,10,1000,700)
h1 = Hist(nBins, lowerBound, upperBound, name='h1', title='h1', drawstyle='hist')

run.Draw(parameterToExamine, hist=h1, selection='')
c1.SetGridx(1)
#c1.SetLogy()

h1.SetTitle('')
h1.GetYaxis().SetTitle('Counts')
h1.GetXaxis().SetTitle(parameterToExamine)
h1.SetLineWidth(2)

h1.SetStats(0)
h1.Draw(option1)
c1.Update()

fGaus = root.TF1('fGaus', 'gaus', lowerBound, upperBound)
#fGaus.SetParameters(ampGuess, meanGuess, sigmaGuess)

h1.Fit('fGaus', '')
fGaus.Draw('same')

c1.Update()



raw_input('Enter to continue...')
