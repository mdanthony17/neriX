#!/usr/bin/python

import ROOT as root
import sys
import neriX_analysis

if(len(sys.argv) < 2):
	print 'Usage is simple_plot <ROOT filename>'
	sys.exit(1)

file = sys.argv[1]
run = neriX_analysis.neriX_analysis(file)


#choose cuts
run.add_z_cut()
run.add_radius_cut(0., 0.85)
run.add_single_scatter_cut()
#run.add_eDep_cut(0., 20.)
#run.add_s1_trig_cut()
#run.add_cut('(S1sPeak[] > 3350 && S1sPeak[] < 3450)')
#run.add_cut('S2sTotBottom[0] > 2000 && S2sTotBottom[0] < 2200')
#run.add_temp_neutron_cut(45)

# box cut
#run.add_cut('(S1sTotBottom[0] > -1 && S1sTotBottom[0] < 20)')
#run.add_cut('(S2sTotBottom[0] > 22e3 && S2sTotBottom[0] < 42e3)')

run.set_event_list()


#--------------- Start Parameters to Change ----------------

#constants for histogram
#parameter 1 will be on x-axis
# range for 12/30 PMT5 out (1420019400, 1420020000)
# range for 12/30 PMT3 out (1420002200, 1420002310)
parameter1ToExamine = 'cpS1sTotBottom[0]'
lowerBound1 = 0
upperBound1 = 1500
nBins1 = 50

#parameter 2 will be on y-axis
parameter2ToExamine = 'cpS2sTotBottom[0]'
lowerBound2 = 0
upperBound2 = 300e3
nBins2 = 50

#choose options
option1 = 'COLZ'

#--------------- End Parameters to Change ----------------

#run.get_T1().Scan('EventId:S1sPeak[0]:S2sPeak[0]', run.get_cuts())

c1 = root.TCanvas('c1','c1',200,200,1000,1000)
h1 = root.TH2F('h1', 'h1', nBins1, lowerBound1, upperBound1, nBins2, lowerBound2, upperBound2)
#h1.SetTitle('Old NN')
h1.GetXaxis().SetTitle(parameter1ToExamine)
h1.GetYaxis().SetTitle(parameter2ToExamine)

run.Draw(parameter1ToExamine + ':' + parameter2ToExamine + '', hist=h1, selection=run.get_cuts())
h1.Draw('colz')

"""
spec = root.TSpectrum2(2)
spec.Search(h1)
"""

h1.SetStats(0)
c1.SetLogz()
h1.Draw(option1)
c1.Update()

raw_input("Enter to continue...")
