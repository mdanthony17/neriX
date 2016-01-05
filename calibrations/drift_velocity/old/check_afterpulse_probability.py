#!/usr/bin/python

import ROOT as root
import sys
import neriX_analysis
from rootpy.plotting import Hist2D, Hist, Legend, Canvas


if(len(sys.argv) != 2):
	print 'Usage is s1_tac_correction.py <ROOT filename>'
	sys.exit(1)
	
filename = sys.argv[1]


run = neriX_analysis.neriX_analysis(filename)





#--------------- Start Parameters to Change ----------------


lowerBoundS2 = 0
upperBoundS2 = 5000
upperBoundSecondaryS2 = 100
nBinsS2 = 100


#--------------- End Parameters to Change ----------------

#run.get_tree().Scan('EventId:TrigArea:TrigLeftEdge-S1sPeak[0]:GeHeight[0]', run.get_cuts())


c1 = root.TCanvas('c1','c1',200,10,1000,700)
c1.SetGridx()
c1.SetGridy()



h1 = Hist(nBinsS2, lowerBoundS2, upperBoundS2, name='h1', title='h1', drawstyle='hist')
h2 = Hist(nBinsS2, lowerBoundS2, upperBoundS2, name='h2', title='h2', drawstyle='hist')

h3 = Hist2D(nBinsS2, lowerBoundS2, upperBoundS2, nBinsS2, lowerBoundS2, upperBoundSecondaryS2, name='h3', title='h3')

h4 = Hist(150, 0, 150, name='h2', title='h2', drawstyle='hist')
c3 = Canvas()
c3.Update()

c4 = Canvas()

#choose cuts
#run.add_dt_cut(8., 13.)
#run.add_radius_cut(0., 0.7)
#run.add_cut('S2sPeak[] < (S1sPeak[0] + 250)')
#run.add_cut('S2sTotTop[] > 1000')

timeFromS1 = 2.5*100 # time in us * 100 (samples/us)

numEvents = run.get_T1().GetEntriesFast()
numEvents = 100000
for i in xrange(numEvents):
	if i % 10000 == 0:
		print 'Processing event ' + str(i) + ' / ' + str(numEvents)

	run.get_T1().GetEntry(i)
	run.get_T2().GetEntry(i)

	lS2sTotBottom = list(run.get_T2().S2sTotBottom)
	lS2sTotTop = list(run.get_T2().S2sTotTop)
	lS2sPeak = list(run.get_T1().S2sPeak)
	lS1sPeak = list(run.get_T1().S1sPeak)
	lS1sTotBottom = list(run.get_T2().S1sTotBottom)
	lS2sCoin = list(run.get_T2().S2sCoin)


	if len(lS2sTotBottom) < 1 or len(lS1sPeak) < 1 or len(lS2sPeak) < 1 or len(lS1sTotBottom) < 1:
		continue
	driftTime = (lS2sPeak[0]-lS1sPeak[0])/100.
	if driftTime < 2. or driftTime > 13:
		continue

	peakLowThreshold = lS1sPeak[0] + timeFromS1 # first peak plus holdoff (from smallest time between

	h2.Fill(lS1sTotBottom[0])

	for peakNum, peakTime in enumerate(lS2sPeak):
		if peakNum == 0:
			continue
		if peakTime < peakLowThreshold and peakTime > lS1sPeak[0] and lS2sCoin[peakNum] > 4:
			h1.Fill(lS1sTotBottom[0])
			h3.Fill(lS1sTotBottom[0], lS2sTotBottom[peakNum])
			h4.Fill(lS2sTotBottom[peakNum])
			break

hEff = root.TEfficiency(h1, h2)
#hEff = h1.Clone()
#hEff.Divide(h2)

c1.cd()
hEff.Draw()
c3.cd()
h3.Draw('colz')

c4.cd()
h4.Draw()


c1.Update()
c3.Update()
c4.Update()


raw_input('Please press enter to continue...')




