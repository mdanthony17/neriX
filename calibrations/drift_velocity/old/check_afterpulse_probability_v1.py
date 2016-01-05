#!/usr/bin/python

import ROOT as root
import sys, array, math
import neriX_analysis
from rootpy.plotting import Hist, Hist2D, Canvas, Legend

if(len(sys.argv) != 2):
	print 'Usage is s1_tac_correction.py <ROOT filename>'
	sys.exit(1)
	
filename = sys.argv[1]


ap_analysis = neriX_analysis.neriX_analysis(filename)


#choose cuts
ap_analysis.add_dt_cut(8., 13.)
ap_analysis.add_radius_cut(0., 0.7)
#ap_analysis.add_cut('(S2sPeak[]-S2sPeak[0])/100. > 1. && (S2sPeak[]-S1sPeak[0])/100. < 3.')
ap_analysis.add_cut('S2sPeak[] < (S1sPeak[0] + 250)')
#ap_analysis.add_cut('S2sPeak[] < 2000')
ap_analysis.add_cut('S2sTotTop[] > 1000')
#ap_analysis.add_cut('S2sPeak[] != S2sPeak[0]')
numEventsAp = ap_analysis.get_num_events_before_cuts()

ap_analysis.set_event_list()

lxe_analysis = neriX_analysis.neriX_analysis(filename)

lxe_analysis.add_dt_cut(8., 13.)
lxe_analysis.add_radius_cut(0., 0.7)
lxe_analysis.add_cut('S2sPeak[] == S2sPeak[0]')
numEventsLxe = lxe_analysis.get_num_events_before_cuts()

lxe_analysis.set_event_list()

# scale h1 (tof) with this value
#numEventsTof, numEventsLxe = tof_analysis.get_num_events(), lxe_analysis.get_num_events()

#\ap_analysis.get_T1().Scan('EventId:S2sPeak[]-S1sPeak[0]:S1sTotBottom[0]:S2sTotTop[]:S2sTotBottom[]', ap_analysis.get_cuts())

#--------------- Start Parameters to Change ----------------


parameterToExamineS2 = 'S1sTotBottom[0]'
lowerBoundS2 = 0
upperBoundS2 = 5000
parameterToExamineSecondaryS2 = 'S2sTotTop[]'
upperBoundSecondaryS2 = 10000
nBinsS2 = 100


#--------------- End Parameters to Change ----------------

#run.get_tree().Scan('EventId:TrigArea:TrigLeftEdge-S1sPeak[0]:GeHeight[0]', run.get_cuts())


c1 = root.TCanvas('c1','c1',200,10,1000,700)
c1.SetGridx()
c1.SetGridy()


h1 = Hist(nBinsS2, lowerBoundS2, upperBoundS2, name='h1', title='h1', drawstyle='hist')
ap_analysis.Draw(parameterToExamineS2, hist=h1, selection='')

h2 = Hist(nBinsS2, lowerBoundS2, upperBoundS2, name='h2', title='h2', drawstyle='hist')
lxe_analysis.Draw(parameterToExamineS2, hist=h2, selection='')

hEff = root.TEfficiency(h1, h2)
#hEff = h1.Clone()#root.TEfficiency(h1, h2)
#hEff.Divide(h2)


hEff.SetTitle('Probability of afterpulse from gate')
#hEff.GetXaxis().SetTitle(parameterToExamineS2 + ' [PE]')
#hEff.GetYaxis().SetTitle('Counts')

#hEff.SetStats(0)
hEff.Draw()

h3 = Hist2D(nBinsS2, lowerBoundS2, upperBoundS2, nBinsS2, lowerBoundS2, upperBoundSecondaryS2, name='h3', title='h3')
ap_analysis.Draw(parameterToExamineS2 + ':' + parameterToExamineSecondaryS2, hist=h3, selection='')
#ap_analysis.get_T1().Scan('EventId:S2sTotBottom[1]')

c2 = Canvas()
h3.Draw('colz')
c2.Update()


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
