#!/usr/bin/python

import ROOT as root
import sys
import neriX_analysis
from rootpy.plotting import Hist2D, Hist, Legend
from rootpy.io import root_open, File
import root_numpy

#--------------- Start Parameters to Change ----------------

s1NumBins = 40
s1Min = -0.5
s1Max = 39.5

logNumBins = 20
logMin = 1
logMax = 3.5

s2NumBins = 40
s2Min = 0.
s2Max = 4000

xyNumBins = 100
xyMin = -1.4
xyMax = 1.4

rNumBins = 100
rMin = 0.
rMax = 1.4

dtNumBins = 100/2
dtMin = -20.
dtMax = 0.

s2WidthNumBins = 200
s2WidthMin = 40.
s2WidthMax = 160.

tofNumBins = 100
tofMin = -80
tofMax = 120

psdNumBins = 50
psdMin = 0
psdMax = 1

lhNumBins = 50
lhMin = 0
lhMax = 2.5

liqsciS1TimeDiffBins = 200
liqsciS1TimeDiffMin = -100
liqsciS1TimeDiffMax = 100


#--------------- End Parameters to Change ----------------



if(len(sys.argv) != 5):
	print 'Usage is python coincidence_quick_check.py <run> <anode> <cathode> <degree>'
	sys.exit(1)

run = int(sys.argv[1])
anodeSetting = float(sys.argv[2])
cathodeSetting = float(sys.argv[3])
degreeSetting = int(sys.argv[4])

l_files_for_settings = neriX_analysis.pull_all_files_given_parameters(run, anodeSetting, cathodeSetting, degreeSetting)


current_analysis = neriX_analysis.neriX_analysis(neriX_analysis.pull_all_files_given_parameters(run, anodeSetting, cathodeSetting, degreeSetting), degreeSetting, cathodeSetting, anodeSetting)
sForHistograms = ' - %d deg, %.3f kV' % (degreeSetting, cathodeSetting)

colzOption = 'COLZ'


current_analysis.add_radius_cut(0, 0.85)
current_analysis.add_single_scatter_cut()
#current_analysis.add_xs1asym_cut() # applied later if using S1 branch
current_analysis.add_xs2asym_cut()
current_analysis.add_temp_neutron_cut(degreeSetting)



s1Branch = 'cpS1sTotBottom[]'
s2Branch = 'cpS2sTotBottom[0]'

current_analysis.add_cut('%s > 0' % s1Branch)
current_analysis.add_cut('%s > 0' % s2Branch)
current_analysis.add_cut('%s < %f' % (s1Branch, s1Max))
current_analysis.add_cut('%s < %f' % (s2Branch, s2Max))

current_analysis.add_z_cut(any_peak=True)
current_analysis.add_s1_liqsci_peak_cut()
current_analysis.add_xs1asym_cut(any_peak=True)



#current_analysis.set_event_list()
num_threads = 7
current_analysis.multithread_set_event_list(num_threads)



#create histograms for current_analysis

print '\nUsing S1 Branch: %s \n\n' % s1Branch
print '\nUsing S2 Branch: %s \n\n' % s2Branch


num_files_used = len(current_analysis.get_lT1())

for i in xrange(num_files_used):
    current_analysis.get_lT1()[i].GetPlayer().SetScanRedirect(True)
    current_analysis.get_lT1()[i].GetPlayer().SetScanRedirect(True)

    current_analysis.get_lT1()[i].Scan('EventId:cpS1sTotBottom[]:cpS2sTotBottom[0]', current_analysis.get_cuts())

    """
    num_entries = current_analysis.get_lT1()[i].GetEventList().GetN()
    for event_num in xrange(num_entries):
        print event_num
        tree_event_number = current_analysis.get_lT1()[i].GetEventList().GetEntry(event_num)
        print current_analysis.get_lT1()[i].GetEventList().GetIndex(tree_event_number)
        current_analysis.get_lT1()[i].GetEventList().Print()
    """





