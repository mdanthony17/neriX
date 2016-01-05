#!/usr/bin/python

import ROOT as root
import sys
import neriX_analysis
from rootpy.plotting import Hist2D, Hist, Legend, Canvas

#--------------- Start Parameters to Change ----------------

s1NumBins = 40
s1Min = 0.
s1Max = 40

s2NumBins = 20
s2Min = 0.
s2Max = 4000

tofNumBins = 100
tofMin = -100
tofMax = 100

peakNumBins = 10
peakMin = -0.5
peakMax = 9.5

#--------------- End Parameters to Change ----------------



if(len(sys.argv) != 2):
	print 'Usage is python high_rate_analysis.py <ROOT filename>'
	sys.exit(1)

filename = sys.argv[1]
current_analysis = neriX_analysis.neriX_analysis(filename)

"""
# long window
s1PeakPositionLow = 3350
s1PeakPositionHigh = 3450
"""
"""
# short window
s1PeakPositionLow = 380
s1PeakPositionHigh = 430
"""

peakDistanceMin = 0
peakDistanceMax = 25

dtLow = 200
dtHigh = 1300


#choose cuts for run 1

hS1 = Hist(s1NumBins, s1Min, s1Max, name='hS1', title='S1 Spectrum', drawstyle='hist')
hS2 = Hist(s2NumBins, s2Min, s2Max, name='hS2', title='S2 Spectrum', drawstyle='hist')
hTOF = Hist(tofNumBins, tofMin, tofMax, name='hTOF', title='TOF Spectrum', drawstyle='hist')
hS1S2 = Hist2D(s1NumBins, s1Min, s1Max, s2NumBins, s2Min, s2Max, name='hS1S2', title='S1 vs S2 Spectrum', drawstyle='hist')
hPeak = Hist(peakNumBins, peakMin, peakMax, name='hPeak', title='S1 Peak Used', drawstyle='hist')

numEvents = current_analysis.get_T1().GetEntriesFast()
#numEvents = 10000
for i in xrange(numEvents):
	if i % 5000 == 0:
		print 'Processing event ' + str(i) + ' / ' + str(numEvents)

	current_analysis.get_T1().GetEntry(i)
	current_analysis.get_T2().GetEntry(i)

	lS1sTotBottom = list(current_analysis.get_T2().S1sTotBottom)
	lS1sPeak = list(current_analysis.get_T1().S1sPeak)
	lS2sTotBottom = list(current_analysis.get_T2().S2sTotBottom)
	lS2sPeak = list(current_analysis.get_T1().S2sPeak)

	lLSRaw = list(current_analysis.get_T1().LiqSciRaw)
	lLSPeak = list(current_analysis.get_T1().LiqSciPeak)
	lLSTailRaw = list(current_analysis.get_T1().LiqSciTailRaw)
	lLSHeight = list(current_analysis.get_T1().LiqSciHeight)

	lPosRec = list(current_analysis.get_T2().S2sPosFann)

	#print lLSPeak

	tof = list(current_analysis.get_T2().TimeOfFlight)
	if len(tof) > 0:
		tof = tof[0]
	else:
		continue

	if len(lS1sTotBottom) < 1:
		continue

	if len(lS2sTotBottom) < 1:
		continue

	psd = -1
	lsHeight = -1
	for j in xrange(2): # only look at EJ0, 1
		if lLSHeight[j] > 0:
			psd = lLSTailRaw[j] / lLSRaw[j]
			lsHeight = lLSHeight[j]


	if not ( (psd > 0.22 and lsHeight > .2 and lsHeight < 1.0) or (psd > 0.15 and lsHeight > .2 and lsHeight < 1.0) ):
		continue

	# get peak number that fits time window, otherwise cut out event
	s1PeakNum = -1
	s1PeakTime = -1
	for peakNum, peakTime in enumerate(lS1sPeak):
		#if peakTime > s1PeakPositionLow and peakTime < s1PeakPositionHigh:
		for j in xrange(2):
			if lLSPeak[j] > 0 and (lLSPeak[j]-peakTime) > peakDistanceMin and (lLSPeak[j]-peakTime) < peakDistanceMax:
				s1PeakNum = peakNum
				s1PeakTime = peakTime
				#print lLSPeak[j], s1PeakTime, s1PeakNum
				break
	if s1PeakNum == -1:
		continue


	if tof < 30. or tof > 45.:
		continue

	for peakNum, peakTime in enumerate(lS2sPeak):
		
		# only look at largest peak
		peakNum = 0
		peakTime = lS2sPeak[peakNum]
		
		if (peakTime-lS1sPeak[s1PeakNum]) < dtLow or (peakTime-lS1sPeak[s1PeakNum]) > dtHigh:
			# check that this peak passes dt cut
			continue
		
		if lS2sTotBottom[peakNum] < 1:
			# check that bottom pmt saw something
			continue
		
		#if lPosRec[peakNum][0]**2 + lPosRec[peakNum][1]**2 > 1.0**2:
		#	continue

		#if lS2sTotBottom[peakNum] > 2000 and lS2sTotBottom[peakNum] < 3000:
		#	print i, s1PeakTime, lS1sTotBottom[s1PeakNum], peakNum, lS2sTotBottom[peakNum], tof
		
		#if lS1sTotBottom[s1PeakNum] > 2 and lS1sTotBottom[s1PeakNum] < 12:
		#	print i, s1PeakTime, lS1sTotBottom[s1PeakNum], peakNum, lS2sTotBottom[peakNum], tof
		
		hS1.Fill(lS1sTotBottom[s1PeakNum])
		hS2.Fill(lS2sTotBottom[peakNum])
		hTOF.Fill(tof)
		hS1S2.Fill(lS1sTotBottom[s1PeakNum], lS2sTotBottom[peakNum])
		hPeak.Fill(s1PeakNum)
		break # avoid multiple fills for S2
		

	



#print 'Smallest Time Between Triggers: ' + str(smallestTimeBetweenTriggers) + ' samples'

c1 = Canvas()
hS1.Draw()
c1.Update()

c2 = Canvas()
hS2.Draw()
c2.Update()

c3 = Canvas()
hTOF.Draw()
c3.Update()

c4 = Canvas()
hS1S2.Draw('colz')
c4.Update()

c5 = Canvas()
hPeak.Draw()
c5.Update()


raw_input('Please press enter to continue...')




