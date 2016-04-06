#!/usr/bin/python

import ROOT as root
import sys, array, math
import neriX_analysis
from rootpy.plotting import Hist, Hist2D, Canvas, Legend
from rootpy.io import root_open, File
import numpy as np
from tqdm import tqdm

if(len(sys.argv) != 2):
	print 'Usage is s1_tac_correction.py <ROOT filename>'
	sys.exit(1)
	
filename = sys.argv[1]


current_analysis = neriX_analysis.neriX_analysis(filename)


lowForGaussian = -50
highForGaussian = 40


current_analysis.add_xs1asym_cut()
current_analysis.set_event_list()


#--------------- Start Parameters to Change ----------------

gainUsed = 5.69e4 # 1.48e6

s1Branch = 'S1sTotBottom[S1Order[0]]*%f' % gainUsed
#s1Branch = 'cpS1sTotBottom[S1Order[0]]'
lowerBoundS1 = 0
upperBoundS1 = 40*gainUsed
nBinsS1 = 40


#--------------- End Parameters to Change ----------------

#tof_analysis.get_T1().Scan('EventId:TrigLeftEdge-S1sPeak[0]:S1sTotBottom[0]', 'S1sTotBottom[0] > 20')


c1 = root.TCanvas('c1','c1',200,10,1000,700)
c1.SetGridx()
c1.SetGridy()

hGaus = Hist(highForGaussian - lowForGaussian, lowForGaussian, highForGaussian, name='hGaus')

hRightEdge = Hist(highForGaussian - lowForGaussian, lowForGaussian, highForGaussian, name='hRightEdge')

hTimeClosestAndLength = Hist2D(highForGaussian - lowForGaussian, lowForGaussian, highForGaussian, highForGaussian - lowForGaussian + 10, lowForGaussian, highForGaussian + 10, name='hTimeClosestAndLength')

hNoTrig = Hist(nBinsS1, lowerBoundS1, upperBoundS1, name='hNoTrig')
hTrig = Hist(nBinsS1, lowerBoundS1, upperBoundS1, name='hTrig')

hNumTrigs = Hist(60, 0, 60, name='hNumTrigs')

hSizeAndTime = Hist2D(nBinsS1, lowerBoundS1, upperBoundS1, highForGaussian - lowForGaussian, lowForGaussian, highForGaussian, name='hSizeAndTime')

# -------------------------------------
#  Find events to check waveforms
# -------------------------------------

numEvents = current_analysis.get_T1().GetEntriesFast()
numEvents = 20000
for i in tqdm(xrange(numEvents)):

	current_analysis.get_T1().GetEntry(i)
	current_analysis.get_T2().GetEntry(i)

	lS1sTotBottom = list(current_analysis.get_T2().S1sTotBottom)
	lS1sTotTop = list(current_analysis.get_T2().S1sTotTop)
	lS1sPeak = list(current_analysis.get_T1().S1sPeak)
	lS1Order = list(current_analysis.get_T1().S1Order)
	lS2Order = list(current_analysis.get_T1().S2Order)
	lS2sPeak = list(current_analysis.get_T1().S2sPeak)

	lS1sLeftEdge = list(current_analysis.get_T1().S1sLeftEdge)
	lS1sRightEdge = list(current_analysis.get_T1().S1sRightEdge)
	
	lS2sLeftEdge = list(current_analysis.get_T1().S2sLeftEdge)
	lS2sRightEdge = list(current_analysis.get_T1().S2sRightEdge)

	lS1sNoiseMedian = list(current_analysis.get_T2().S1sNoiseMedian)
	lS1sNoiseTrapezoid = list(current_analysis.get_T2().S1sNoiseTrapezoid)

	lTrigLeftEdge = list(current_analysis.get_T1().TrigLeftEdge)
	lTrigLength = list(current_analysis.get_T1().TrigLength)
	lXY = list(current_analysis.get_T2().S2sPosFann)

	if len(lS1sTotBottom) < 1:
		continue

	if len(lS2sPeak) < 1:
		continue

	if len(lTrigLeftEdge) < 1:
		continue
	
	#if len(lTrigLeftEdge) > 15:
	#	continue

	# cut on noisy waveforms
	if lS1sTotBottom[lS1Order[0]] <= 0:
		continue
	
	
	#if lS1sTotBottom[lS1Order[0]] < 0.5 or lS1sTotBottom[lS1Order[0]] > 15:
	#	continue
	
	#if lS1sTotBottom[lS1Order[0]] < 50 or lS1sTotBottom[lS1Order[0]] > 200:
	#	continue
	
	# make sure largest S1 is the correct one
	if lS1Order[0] != 0:
		continue

	# make sure no close S1s
	if len(lS1sPeak) > 1 and lS1sPeak[lS1Order[1]] - lS1sPeak[lS1Order[0]] < 300:
		continue

	# make sure no close S2s
	if lS2sLeftEdge[lS2Order[0]] - lS1sPeak[lS1Order[0]] < 100:
		continue

	# remove noisy S1s (asymmetry)
	if (lS1sTotBottom[lS1Order[0]] - lS1sTotTop[lS1Order[0]])/(lS1sTotBottom[lS1Order[0]] + lS1sTotTop[lS1Order[0]]) < 0:
		continue




	# find closest trigger
	trigFound = -1
	closestTrigger = 10000000
	closestRightEdge = 10000000
	for trigNumber, trigTime in enumerate(lTrigLeftEdge):
		timeDiff = trigTime - lS1sPeak[lS1Order[0]]
		if abs(timeDiff) < abs(closestTrigger):
			closestTrigger = timeDiff
			closestRightEdge = timeDiff + lTrigLength[trigNumber]
			trigFound = trigNumber

	# search for noise nearby (BIASED!)
	"""
	noiseFound = False
	lowerBoundNoise = lTrigLeftEdge[trigFound] - 200
	upperBoundNoise = lTrigLeftEdge[trigFound] + 200
	for trigNumber, trigTime in enumerate(lTrigLeftEdge):
		#print lowerBoundNoise, upperBoundNoise, trigTime
		if trigTime == lTrigLeftEdge[trigFound]:
			continue
		if trigTime > lowerBoundNoise and trigTime < upperBoundNoise:
			#print 'noise found'
			noiseFound = True
			break
	if noiseFound:
		continue
	"""
	

	#if closestTrigger > -20 and closestTrigger < -1 and closestTrigger + lTrigLength[trigFound] > 9 and closestTrigger + lTrigLength[trigFound] < 45 and lS1sTotBottom[lS1Order[0]] < 40:
	#	print i, closestTrigger, lTrigLeftEdge[trigFound], lS1sPeak[lS1Order[0]]

	#if closestTrigger + lTrigLength[trigFound] > 28 and closestTrigger + lTrigLength[trigFound] < 40:
	#	print i, closestTrigger, lTrigLeftEdge[trigFound], lTrigLeftEdge[trigFound]+lTrigLength[trigFound], lS1sPeak[lS1Order[0]]


	hNumTrigs.Fill(len(lTrigLeftEdge))
	hGaus.Fill(closestTrigger)
	hRightEdge.Fill(closestTrigger + lTrigLength[trigFound])
	hTimeClosestAndLength.Fill(closestTrigger, closestTrigger + lTrigLength[trigFound])
	hSizeAndTime.Fill(lS1sTotBottom[lS1Order[0]]*gainUsed, closestTrigger)

	"""
	if (closestTrigger > 0 and closestTrigger < 5) and lS1sTotBottom[lS1Order[0]] < 100:
		print i, closestTrigger, lTrigLeftEdge[trigFound], lTrigLeftEdge[trigFound]+lTrigLength[trigFound], lS1sPeak[lS1Order[0]], lS1sTotBottom[lS1Order[0]]
	"""

	# use register code to correct S1 values
	#if (closestTrigger > 0 and closestTrigger < 5) and (closestRightEdge > 8 and closestRightEdge < 12):
	#if (closestTrigger > 0 and closestTrigger < 5):
	if closestTrigger < 7 and closestRightEdge > 6:
		passCut = True
		hTrig.Fill(lS1sTotBottom[lS1Order[0]]*gainUsed)
		#if lS1sTotBottom[lS1Order[0]] < 40:
		#	print lS1sTotBottom[lS1Order[0]]
		#print lS1sTotBottom[lS1Order[0]]
	else:
		passCut = False
		hNoTrig.Fill(lS1sTotBottom[lS1Order[0]]*gainUsed)


	#if lS1sTotBottom[lS1Order[0]] < 5:
	#	print i, passCut, closestTrigger, lTrigLeftEdge[trigFound], lTrigLeftEdge[trigFound]+lTrigLength[trigFound], lS1sPeak[lS1Order[0]], lS1sTotBottom[lS1Order[0]]



hGaus.SetMarkerSize(0)
hGaus.SetStats(0)
hGaus.GetXaxis().SetTitle('TrigLeftEdge[] - S1sPeak[S1Order[0]]')
hGaus.GetYaxis().SetTitle('Counts')
hGaus.SetTitle('Time between Closest Trigger and First S1 in Waveform - %s' % current_analysis.get_filename_no_ext())


c1.SetLogy()
hGaus.Draw()
c1.Update()

c2 = Canvas()
hTimeClosestAndLength.SetStats(0)
hTimeClosestAndLength.Draw('colz')
c2.Update()

c3 = Canvas()
hRightEdge.SetMarkerSize(0)
hRightEdge.SetStats(0)
hRightEdge.Draw()
c3.Update()

c4 = Canvas()

hNoTrig.Add(hTrig)
hEff = root.TEfficiency(hTrig, hNoTrig)

gEff = hEff.CreateGraph()
gEff.GetXaxis().SetRangeUser(lowerBoundS1, upperBoundS1)
gEff.Draw('AP')

"""
fEfficiency = root.TF1('fEfficiency', '1. / (1 + exp([1]*([0]-x)))', lowerBoundS1, upperBoundS1)
fEfficiency.SetParameters(6., 0.5)
fEfficiency.SetParLimits(1, 0.02e-6, 0.8e-5)
#fEfficiency.FixParameter(1, .5)
"""

"""
fEfficiency = root.TF1('fEfficiency', '((1.) / (1 + exp([1]*([0]-x))) + [2])/(1+[2])', lowerBoundS1, upperBoundS1)
fEfficiency.SetParameters(4.5e5, 5e-6, 0.05)
fEfficiency.SetParLimits(0, 1e4, 1e6)
fEfficiency.SetParLimits(1, 0.02e-6, 0.8e-5)
fEfficiency.SetParLimits(2, 0, 1)
#fEfficiency.FixParameter(1, .5)
"""
"""
fEfficiency = root.TF1('fEfficiency', '( ([0]*x + [1]*x^2 + [2]*x^4 < 1 ? [0]*x + [1]*x^2 + [2]*x^4 : 1.) + [3])/(1+[3])', lowerBoundS1, upperBoundS1)
fEfficiency.SetParameters(-3.14e-6, -7e-13, -3.9e-25, 0.05)
#fEfficiency.SetParLimits(1, 0.02e-6, 0.8e-5)
fEfficiency.SetParLimits(3, 0, 1)
#fEfficiency.FixParameter(1, .5)
"""


#fEfficiency = root.TF1('fEfficiency', 'TMath::Max(((1.)*(1. - exp(-[0]*(x-[1]))) + [2])/(1.+[2]), (0. + [2])/(1.+[2]))', lowerBoundS1, upperBoundS1)
fEfficiency = root.TF1('fEfficiency', '(TMath::Max((1.)*(1. - exp(-[0]*(x-[1]))), 0) + [2])/(1+[2])', lowerBoundS1, upperBoundS1)
fEfficiency.SetParameters(4e-6, 3e4, 0)
fEfficiency.SetParLimits(0, 5e-7, 1e-5)
fEfficiency.SetParLimits(1, -1e6, 1e6)
fEfficiency.SetParLimits(2, 0, 1)
#fEfficiency.FixParameter(2, 0.)


fEfficiency.SetLineStyle(9)
fEfficiency.SetLineWidth(1)

#gEff.Fit(fEfficiency, 'MENLL')
#hEff.Fit(fEfficiency, 'NLL')

# use TBinomialEfficiencyFitter
fitter_binomial = root.TBinomialEfficiencyFitter(hTrig, hNoTrig)
fitter_binomial.Fit(fEfficiency, 'NLL')


fEfficiency.Draw('same')
c4.Update()

print fitter_binomial.GetFitter()

raw_input()

gConfInterval = root.TGraphAsymmErrors(*neriX_analysis.create_graph_with_confidence_interval_for_fit(gEff, fitter_binomial.GetFitter()))
gConfInterval.SetLineColor(root.kBlue)
gConfInterval.SetFillColor(root.kBlue)
gConfInterval.SetFillStyle(3005)
gConfInterval.Draw('3 same')



c4.Update()

c5 = Canvas()
hSizeAndTime.Draw('colz')
c5.Update()

c6 = Canvas()
hNumTrigs.Draw()
c6.Update()


raw_input('Press enter to continue...')


