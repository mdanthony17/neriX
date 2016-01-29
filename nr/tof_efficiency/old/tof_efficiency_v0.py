#!/usr/bin/python

import ROOT as root
import sys, array, math
import neriX_analysis
from rootpy.plotting import Hist, Hist2D, Canvas, Legend

"""
if(len(sys.argv) != 2):
	print 'Usage is s1_tac_correction.py <ROOT filename>'
	sys.exit(1)
	
filename = sys.argv[1]
"""

lTofFiles = ['nerix_151014_1734.root', 'nerix_151015_1607.root', 'nerix_151015_1713.root', 'nerix_151016_0731', 'nerix_151016_0941']
LXeFile = 'nerix_151016_1003.root'

tof_analysis = neriX_analysis.neriX_analysis(lTofFiles, -5, 1.054, 4.5)



#choose cuts
tof_analysis.add_dt_cut(2., 13.)
tof_analysis.add_radius_cut(0., 0.7)
numEventsTof = tof_analysis.get_num_events_before_cuts()

tof_analysis.set_event_list()

lxe_analysis = neriX_analysis.neriX_analysis(LXeFile)

lxe_analysis.add_dt_cut(2., 13.)
lxe_analysis.add_radius_cut(0., 0.7)
numEventsLxe = lxe_analysis.get_num_events_before_cuts()

lxe_analysis.set_event_list()

# scale h1 (tof) with this value
#numEventsTof, numEventsLxe = tof_analysis.get_num_events(), lxe_analysis.get_num_events()

scaleFactor = (numEventsTof/numEventsLxe)


#--------------- Start Parameters to Change ----------------


parameterToExamineS1 = 'S1sTotBottom[0]'
lowerBoundS1 = 0
upperBoundS1 = 100
nBinsS1 = 100


#--------------- End Parameters to Change ----------------

#run.get_tree().Scan('EventId:TrigArea:TrigLeftEdge-S1sPeak[0]:GeHeight[0]', run.get_cuts())


c1 = root.TCanvas('c1','c1',200,10,1000,700)
c1.SetGridx()
c1.SetGridy()


h1 = Hist(nBinsS1, lowerBoundS1, upperBoundS1, name='h1', title='h1', drawstyle='hist')
tof_analysis.Draw(parameterToExamineS1, hist=h1, selection='')

h2 = Hist(nBinsS1, lowerBoundS1, upperBoundS1, name='h2', title='h2', drawstyle='hist')
lxe_analysis.Draw(parameterToExamineS1, hist=h2, selection='')
h2.Scale(scaleFactor)

#hEff = root.TEfficiency(h1, h2)
hEff = h1.Clone()#root.TEfficiency(h1, h2)
hEff.Divide(h2)

"""
hEff.SetTitle('')
hEff.GetXaxis().SetTitle('S1 [PE]')
hEff.GetYaxis().SetTitle('Time of Flight [ns]')
"""

# make log-bins on x-axis

#hEff.SetStats(0)
hEff.Draw()


"""
fCorrection = root.TF1('fCorrection', 'pol6', lowerBoundS1, upperBoundS1)
hProfile.Fit(fCorrection, 'NE')
fCorrection.Draw('same')
"""

"""
lZero = root.TLine()
lZero.SetLineColor(root.kMagenta)
#lZero.SetLineStyle(7)
lZero.SetLineWidth(3)
#print gVoltageVsGain.GetYaxis().GetXmin(),
lZero.DrawLine(lowerBoundS1, 0, upperBoundS1, 0)
lZero.Draw('same')
"""

c1.Update()


raw_input('Enter to continue...')
