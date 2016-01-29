#!/usr/bin/python

import ROOT as root
import sys
import neriX_analysis
from rootpy.plotting import Hist2D, Hist, Legend, Canvas

#--------------- Start Parameters to Change ----------------

s2NumBins = 100
s2Min = 0.
s2Max = 10e4

s1NumBins = 100
s1Min = 0.
s1Max = 10e3

naiNumBins = 100
naiMin = 1100
naiMax = 1250

dtS1NaiNumBins = 100
dtS1NaiMin = -100
dtS1NaiMax = 100

trigNumbBins = 100
trigMin = -200
trigMax = 200

#--------------- End Parameters to Change ----------------



if(len(sys.argv) != 2):
	print 'Usage is python coin_analysis <ROOT filename>'
	sys.exit(1)

filename = sys.argv[1]
current_analysis = neriX_analysis.neriX_analysis(filename)



#choose cuts for run 1
current_analysis.add_dt_cut()
#current_analysis.add_cut('NaiPeak > 1100 && NaiPeak < 1300')
#current_analysis.add_cut('(TrigLeftEdge[0] - S2sPeak[0]) > -30 && (TrigLeftEdge[0] - S2sPeak[0]) < 50')
#current_analysis.add_cut('(NaiLeftEdge[0] - S1sPeak[0]) > 55 && (NaiLeftEdge[0] - S1sPeak[0]) < 80')
current_analysis.set_event_list()

#current_analysis.get_T1().Scan('EventId:TrigLeftEdge:S2sPeak[0]')

c1 = Canvas()

hNaI = Hist(naiNumBins, naiMin, naiMax, name='hNai', title='NaI Distribution', drawstyle='hist')
current_analysis.Draw('NaiPeak', hist=hNaI)
hNaI.Draw()

c1.Update()

c2 = Canvas()

hS2 = Hist(s2NumBins, s2Min, s2Max, name='hS2', title='S2 Distribution', drawstyle='hist')
current_analysis.Draw('S2sTotBottom[0]', hist=hS2)
hS2.Draw()

c2.Update()


c3 = Canvas()

hDtS1NaI = Hist(dtS1NaiNumBins, dtS1NaiMin, dtS1NaiMax, name='hDtS1NaI', title='Time between NaI and S1', drawstyle='hist')
current_analysis.Draw('NaiLeftEdge[0] - S1sPeak[0]', hist=hDtS1NaI)
hDtS1NaI.Draw()

c3.Update()

c4 = Canvas()

hTrig = Hist(trigNumbBins, trigMin, trigMax, name='hTrig', title='Time between S2 trigger and S2', drawstyle='hist')
current_analysis.Draw('TrigLeftEdge[] - S2sPeak[0]', hist=hTrig)
# almost all events have only 1 event in TrigLeftEdge
hTrig.Draw()

c4.Update()

c5 = Canvas()

hS1S2 = Hist2D(s1NumBins, s1Min, s1Max, s2NumBins, s2Min, s2Max, name='hS1S2', title='S1 vs. S2', drawstyle='hist')
current_analysis.Draw('S1sTotBottom[0]:S2sTotBottom[0]', hist=hS1S2)
# almost all events have only 1 event in TrigLeftEdge
hS1S2.Draw('colz')

c5.Update()


raw_input('Please press enter to continue...')




