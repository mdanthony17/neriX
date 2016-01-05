#!/usr/bin/python

import ROOT as root
import sys, array
import neriX_analysis

if(len(sys.argv) < 2):
	print 'Usage is simple_plot <ROOT filename>'
	sys.exit(1)


file = sys.argv[1]
run = neriX_analysis.neriX_analysis('../Analysis/data/run_13/' + file)

#choose options
option1 = 'COLZ'


#choose cuts
run.add_dt_cut(2., 13.)
#run.add_radius_cut(0., 25.)
run.add_cut('S1sTotBottom[0] > 20')
run.add_cut('Sum$(S1s[][8]) < 2.')

print 'Number of entries before cuts: ' + str(run.get_tree().GetEntries())


#--------------- Start Parameters to Change ----------------

#constants for histogram
#parameter 1 will be on x-axis
parameterToExamine = '-1*(TimeTDC[4]-168)'
lowerBound = 0
upperBound = 160
nBins = 160*3

#choose options
option1 = ''

#--------------- End Parameters to Change ----------------

#run.get_tree().Scan('EventId:TrigArea:TrigLeftEdge-S1sPeak[0]:GeHeight[0]', run.get_cuts())


c1 = root.TCanvas('c1','c1',200,10,1000,700)
h1 = root.TH1F('h1', 'h1', nBins, lowerBound, upperBound)

run.get_tree().Draw(parameterToExamine, hist=h1, selection=run.get_cuts())

f1 = root.TF1('f1', '[0] * ( [1]*exp(-x*[2]) + (1-[1])*exp(-x*[3]) ) + [4]', lowerBound, upperBound)

f1.SetParameters(100, 0.6, 1./2.5, 1./27., 50)
f1.SetParLimits(0, 1, 1e4)
f1.SetParLimits(1, 0, 1)
f1.SetParLimits(2, 1./4., 1.)
f1.FixParameter(2, 1./2.2)
#f1.SetParLimits(3, 1./29., 1./10.)
f1.FixParameter(3, 1./27.)
f1.SetParLimits(4, 0, 1e3)

h1.Fit('f1', 'M')

c1.Update()


raw_input("Enter to continue...")
