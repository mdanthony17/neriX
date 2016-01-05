#!/usr/bin/python

import ROOT as root
import sys
import neriX_analysis
from rootpy.plotting import Hist2D, Hist, Legend, Canvas

#--------------- Start Parameters to Change ----------------

s2NumBins = 100
s2Min = 0.
s2Max = 5000

#--------------- End Parameters to Change ----------------



if(len(sys.argv) != 1):
	print 'Usage is python coin_analysis'
	sys.exit(1)


hS2 = Hist(s2NumBins, s2Min, s2Max, name='hS2', title='S2 Spectrum', drawstyle='hist')
hS2Efficiency = Hist(s2NumBins, s2Min, s2Max, name='hS2Efficiency', title='S2 Spectrum with cut', drawstyle='hist')

lFiles = ['nerix_150914_1236', 'nerix_150914_1549']#, 'nerix_150914_1618', 'nerix_150914_1719']


for numFile, file in enumerate(lFiles):

	current_analysis = neriX_analysis.neriX_analysis(file)

	print 'File ' + str(numFile+1) + ' / ' + str(len(lFiles))

	peakLowThreshold = 11e3
	peakTrigDistanceSquared = 15**2

	#choose cuts for run 1
	current_analysis.add_dt_cut()
	#current_analysis.add_cut('(S2sPeak[] > 11000)')

	current_analysis.set_event_list()




	smallestTimeBetweenTriggers = 100000000

	numEvents = current_analysis.get_T1().GetEntriesFast()
	#numEvents = 10000
	for i in xrange(numEvents):
		if i % 10000 == 0:
			print 'Processing event ' + str(i) + ' / ' + str(numEvents)

		current_analysis.get_T1().GetEntry(i)
		current_analysis.get_T2().GetEntry(i)

		lS2sTotBottom = list(current_analysis.get_T2().S2sTotBottom)
		lS2sPeak = list(current_analysis.get_T1().S2sPeak)
		lS2sLowWidth = list(current_analysis.get_T1().S2sLowWidth)
		lS2sCoin = list(current_analysis.get_T2().S2sCoin)
		lTrigLeftEdge = list(current_analysis.get_T1().TrigLeftEdge)

		if len(lS2sTotBottom) < 1:
			continue

		if len(lTrigLeftEdge) < 1:
			#print lS2sTotBottom, lS2sPeak
			continue

		if len(lTrigLeftEdge) > 2:
			# looks like a small issue in xerawdp so drop last trig
			lTrigLeftEdge = lTrigLeftEdge[:-1]

		peakLowThreshold = lTrigLeftEdge[0] + 10480 # first peak plus holdoff (from smallest time between
		widthThreshold = lS2sLowWidth[0]*5
		filledStandard = -1

		if len(lTrigLeftEdge) > 1:
			#if lTrigLeftEdge[1]-lTrigLeftEdge[0] < smallestTimeBetweenTriggers and lTrigLeftEdge[1]-lTrigLeftEdge[0] > 2000:
			#	smallestTimeBetweenTriggers = lTrigLeftEdge[1]-lTrigLeftEdge[0]
		
			lS2sPeakOrder = list(lS2sPeak)
			lDistances = list(lS2sPeak)
			lIndices = range(len(lS2sPeak))
			for i in xrange(len(lDistances)):
				lDistances[i] = (lDistances[i] - lTrigLeftEdge[1])**2

			# sort peaks by distance to trigger
			lDistances, lIndices, lS2sPeakOrder = zip(*sorted(zip(lDistances, lIndices, lS2sPeakOrder)))

			for peakNum, peakTime in enumerate(lS2sPeak):
				if peakTime < peakLowThreshold or lS2sPeak[peakNum] > lS2sPeakOrder[0]:
					# check that trigger could occur (100 us after first and before second)
					continue
				
				if lS2sTotBottom[peakNum] < 1:
					# check that bottom pmt saw something
					continue
				
				if lS2sCoin[peakNum] < 3:
					continue
				
				#if lS2sLowWidth[peakNum] > widthThreshold:
					# check that bottom pmt saw something
				#	continue
					
				#if lS2sTotBottom[peakNum] > 1000 and lS2sTotBottom[peakNum] < 2000:
				#	print i, peakTime, lS2sPeakOrder[0], lS2sTotBottom[peakNum]
				#	print peakNum, lIndices[0]
				
				hS2.Fill(lS2sTotBottom[peakNum])
				if peakNum == lIndices[0]:
					filledStandard = 1
				
			if filledStandard > 0:
				# filledStandard makes sure that the peak that created trigger passed
				# basic quality cuts
				#if lS2sTotBottom[lIndices[0]] > 1000 and lS2sTotBottom[lIndices[0]] < 2000:
				#	print lS2sPeakOrder[0], lS2sTotBottom[peakNum]
				hS2Efficiency.Fill(lS2sTotBottom[lIndices[0]])


		else:
			for peakNum, peakTime in enumerate(lS2sPeak):
				if peakTime < peakLowThreshold:
					# check that trigger could occur (100 us after first)
					continue
				
				if lS2sTotBottom[peakNum] < 50:
					# check that bottom pmt saw something
					continue
					
				if lS2sCoin[peakNum] < 3:
					continue
				
				#print i, peakTime, lS2sTotBottom[peakNum]
				
				#if lS2sLowWidth[peakNum] > widthThreshold:
					# check that bottom pmt saw something
					#print i, peakTime, lS2sLowWidth[peakNum], widthThreshold
				#	continue
				
				#print peakTime
				hS2.Fill(lS2sTotBottom[peakNum])
				# no left trig edge


#print 'Smallest Time Between Triggers: ' + str(smallestTimeBetweenTriggers) + ' samples'

c1 = Canvas()
c1.SetLogy()

hS2Efficiency.GetXaxis().SetTitle('S2sTotBottom [pe]')
hS2Efficiency.GetYaxis().SetTitle('Percentage of Events with Trigger')
hS2Efficiency.SetStats(0)

hS2.SetColor('b')
hS2.Draw()
#hS2Efficiency.SetColor('r')
hS2Efficiency.Draw('same')


c1.Update()

c2 = Canvas()

hS2EfficiencyClone = hS2Efficiency.Clone()

hS2EfficiencyClone.Divide(hS2)
#hS2Efficiency.Scale(1./0.9) # scale for dead time on S2 holdoff

hS2EfficiencyClone.Draw('e')
hS2EfficiencyClone.SetMarkerSize(1)


fSigmoid = root.TF1('fSigmoid', '[0] / (1 + exp([2]*([1]-x)))', s2Min, s2Max)
#fSigmoid.SetParameters(9, 0.5, 740, 2e-4)
fSigmoid.SetParLimits(0, .9, 1.1)
fSigmoid.SetParLimits(1, 500, 2000)
fSigmoid.SetParLimits(2, 1e-3, 1e-2)
#fSigmoid.FixParameter(0, 1.0)
#fSigmoid.FixParameter(1, 4000)
#fSigmoid.FixParameter(2, .1)
fSigmoid.SetLineStyle(9)
fSigmoid.SetLineWidth(1)

hS2EfficiencyClone.Fit('fSigmoid', 'LL', '', s2Min, s2Max)
sigmoidMax = float(fSigmoid.GetParameter(0))
fMaxLine = root.TF1('fMaxLine', 'pol0', s2Min, s2Max)
fMaxLine.SetParameter(0, sigmoidMax)
fMaxLine.SetLineColor(root.kBlue)
fMaxLine.SetLineStyle(7)
fMaxLine.SetLineWidth(1)

fSigmoid.Draw('same')
fMaxLine.Draw('same')


c2.Update()

raw_input('Please press enter to continue...')




