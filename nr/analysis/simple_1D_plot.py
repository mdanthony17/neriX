#!/usr/bin/python

import ROOT as root
import sys, array
import neriX_analysis
from rootpy.plotting import Hist, Hist2D, Canvas, Legend

if(len(sys.argv) < 2):
	print 'Usage is simple_plot <ROOT filename>'
	sys.exit(1)


#file = sys.argv[1]

#filename = '/Volumes/Seagate Backup Plus Drive/nerix_data/run_10/root_files/' + sys.argv[1]
#filename = '/Volumes/Seagate Backup Plus Drive/nerix_data/run_11_merged/' + sys.argv[1]

filename = sys.argv[1]

run = neriX_analysis.neriX_analysis(filename)

#choose options
option1 = 'COLZ'


#choose cuts
run.add_z_cut()
run.add_radius_cut(0., 0.7)
run.add_single_scatter_cut()
run.add_cut('(s1asym > 0)')
#run.add_eDep_cut(0., 20.)
#run.add_s1_trig_cut()
#run.add_cut('(S1sPeak[] > 3380 && S1sPeak[] < 3430)')
#run.add_cut('(S2sPeak[0]-3400) > 200 && (S2sPeak[0]-3400) < 1300')
#run.add_temp_neutron_cut(45)
#run.add_cut('S2sTotBottom[0] > 2000 && S2sTotBottom[0] < 2200')

run.set_event_list()


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

# test with TSpectrum
"""
spec = root.TSpectrum(3)
spec.Search(h1, 2, 'nobackground new', 0.01)
"""

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
tempArray = array.array('d')
tempArray = spec.GetPositionX()
print tempArray[0], tempArray[1], tempArray[2]
"""

raw_input('Enter to continue...')
