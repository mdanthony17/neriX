#!/usr/bin/python

import ROOT as root
import sys, pickle
from rootpy import stl
from rootpy.plotting import Hist2D, Hist, Legend, Canvas
from rootpy.io import File
from rootpy.tree import Tree, TreeModel, TreeChain
import numpy as np
from sklearn import preprocessing
from math import exp
import random, operator

def frange(x, y, jump=1.):
	while x < y:
		yield x
		x += jump


def spe_waveform_generator(tauRise, tauFall, sumMean, sumSigma, timeWindow=200, numBins=10000, numRandomIterations = 100):
	# all times in ns
	assert timeWindow % 10 == 0 and numBins % 10 == 0
	aPoints = np.zeros(numBins)
	sizeOfBin = float(timeWindow)/float(numBins)
	midTime = float(timeWindow)/2.
	for i in xrange(numBins):
		currentTime = i*sizeOfBin
		if currentTime < midTime:
			aPoints[i] = exp( (currentTime-midTime) / tauRise)
		else:
			aPoints[i] = exp( (-currentTime+midTime) / tauFall)

	# use to make integrals equal
	randSum = random.normalvariate(sumMean, sumSigma)
	scaleFactor = np.sum(aPoints) / randSum
	#aPoints = aPoints / scaleFactor

	# ten bins in pickled waveform
	tenNsNumBins = int(10.*numBins/timeWindow)

	# generate random number in the ten ns window
	random.seed()
	lTotalWaveform = [0 for i in xrange(10)]

	for i in xrange(numRandomIterations):
		lWaveform = []
		startingBin = int(random.uniform(0, tenNsNumBins))
		while startingBin < numBins:
			lWaveform.append(sum(aPoints[startingBin:startingBin+tenNsNumBins]))
			startingBin += tenNsNumBins

		# get index of max value
		index, value = max(enumerate(lWaveform), key=operator.itemgetter(1))

		# place max index in sixth spot (like real spe waveforms)
		# keep 5 before and 4 after max
		#lWaveform = preprocessing.normalize(lWaveform[index-5:index+5], norm='l2')
		lTotalWaveform = [(x + y) for x,y in zip(lTotalWaveform, lWaveform[index-5:index+5])]

	lTotalWaveform = [x/(numRandomIterations*scaleFactor) for x in lTotalWaveform]


	# take average over many random starting positions
	#lTotalWaveform = preprocessing.normalize(lTotalWaveform, norm='l2')
	#print lTotalWaveform
	return lTotalWaveform



# MUST INCLUDE TYPES TO AVOID SEG FAULT
stl.vector(stl.vector('float'))
stl.vector(stl.vector('int'))

filename = './sample_spe.p'

lAllSPE = pickle.load(open(filename, 'rb'))
#lNormalizedSPE = preprocessing.normalize(lAllSPE, norm='l2')

lRise = [0.5, 5.0, 0.1]
lFall = [2.0, 25.0, 0.1]

numTestSamples = len(lAllSPE)
currentChi2 = 1e40
currentRise = -1
currentFall = -1


aSums = np.zeros(numTestSamples)

# normalize each sample waveform
for i in xrange(numTestSamples):
	aSums[i] = sum(lAllSPE[i])

sumMean = np.mean(aSums)
sumStd = np.std(aSums)

#print sumMean, sumStd

lCurrentWaveform = spe_waveform_generator(2, 2, sumMean, sumStd)

for rise in frange(lRise[0], lRise[1], lRise[2]):
	print 'Current Rise Time: ' + str(rise) + ' ns'
	for fall in frange(lFall[0], lFall[1], lFall[2]):
		# loop through rise and fall values
		lCurrentWaveform = spe_waveform_generator(rise, fall, sumMean, sumStd)
		totChi2 = 0.
		#print lCurrentWaveform
		#print lAllSPE[1]
		for i in xrange(numTestSamples):
			#print [(x-y)**2 for x,y in zip(lCurrentWaveform, lAllSPE[i])]
			for j in xrange(10):
				totChi2 += (lCurrentWaveform[j] - lAllSPE[i][j])**2
				#print totChi2
		totChi2 = totChi2 / (2.*numTestSamples)
		#print totChi2
		if totChi2 < currentChi2:
			currentChi2 = totChi2
			currentRise = rise
			currentFall = fall
			print currentRise, currentFall, currentChi2

print 'Lowest Chi2: ' + str(currentChi2)
print 'Optimum Rise: ' + str(currentRise)
print 'Optimum Fall: ' + str(currentFall)





