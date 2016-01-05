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
import matplotlib.pyplot as plt

def frange(x, y, jump=1.):
	while x < y:
		yield x
		x += jump


def random_double_exponential_normalized(shortDecay, longDecay, singletOverTriplet):
	# use 0.1 ns resolution over 50 ns window
	# deltaT is in ns, not 0.1 ns
	numBins = int(50. / 0.1)
	aFunc = np.zeros(numBins)
	for i in xrange(numBins):
		aFunc[i] = singletOverTriplet*exp(-i/shortDecay) + exp(-i/longDecay)

	aFunc /= np.sum(aFunc) # normalize function
	aTimes = [0.1*i for i in xrange(numBins)]
	return np.random.choice(aTimes, p=aFunc) # times are the actual times in ns and aFunc are the weights (probabilities of each time)



def spe_waveform_generator(shortDecay, longDecay, tauRise, tauFall, sumMean, sumSigma, noiseMean, noiseSigma, addNoise=False, timeWindow=200, numBins=10000, numRandomIterations = 100):
	# all times in ns
	assert timeWindow % 10 == 0 and numBins % 10 == 0
	numBinsInWaveform = int(timeWindow/10)
	aPoints = np.zeros(numBins)
	sizeOfBin = float(timeWindow)/float(numBins)
	#midTime = float(timeWindow)/2.
	quarterTime = float(timeWindow)/4.
	for i in xrange(numBins):
		currentTime = i*sizeOfBin
		if currentTime < quarterTime:
			aPoints[i] = exp( (currentTime-quarterTime) / tauRise)
		else:
			aPoints[i] = exp( (-currentTime+quarterTime) / tauFall)

	# use to make integrals equal
	# sum mean and sigma taken from real spe signals
	randSum = random.normalvariate(sumMean, sumSigma)
	scaleFactor = np.sum(aPoints) / randSum
	#aPoints = aPoints / scaleFactor

	# ten bins in pickled waveform
	tenNsNumBins = int(10*numBins/timeWindow)

	# generate random number in the ten ns window
	random.seed()
	lTotalWaveform = [0 for i in xrange(numBinsInWaveform)]

	for i in xrange(numRandomIterations):
		lWaveform = []
		startingBin = int(random.uniform(0, tenNsNumBins))
		peakBin = startingBin
		while startingBin < numBins:
			lWaveform.append(sum(aPoints[startingBin:startingBin+tenNsNumBins]))
			startingBin += tenNsNumBins

		# get index of max value
		index, value = max(enumerate(lWaveform), key=operator.itemgetter(1))

		# place max index in sixth spot (like real spe waveforms)
		# keep 5 before and 4 after max
		#lWaveform = preprocessing.normalize(lWaveform[index-5:index+5], norm='l2')
		lTotalWaveform = [(x + y) for x,y in zip(lTotalWaveform, lWaveform)]

	lTotalWaveform = [x/(numRandomIterations*scaleFactor) for x in lTotalWaveform]

	peakTime = quarterTime + peakBin*timeWindow/numBins
	
	if addNoise:
		for i in xrange(len(lTotalWaveform)):
			lTotalWaveform[i] += random.gauss(noiseMean, noiseSigma)

	# take average over many random starting positions
	#lTotalWaveform = preprocessing.normalize(lTotalWaveform, norm='l2')
	#print lTotalWaveform
	#print peakTime
	return lTotalWaveform, peakTime



def mpe_waveform_generator(numPE, shortDecay, longDecay, tauRise, tauFall, sumMean, sumSigma, noiseMean, noiseSigma, addNoise=False, timeWindow=200, numBins=10000, numRandomIterations = 100):
	assert timeWindow % 10 == 0 and numBins % 10 == 0
	numBinsInWaveform = int(timeWindow/10)
	lTotalWaveform = [0 for i in xrange(numBinsInWaveform)]
	firstPeTime = 0
	
	for i in xrange(numPE):
		sSpeResult = spe_waveform_generator(shortDecay, longDecay, tauRise, tauFall, sumMean, sumSigma, noiseMean, noiseSigma, addNoise, timeWindow, numBins, numRandomIterations)
		currentWaveform = sSpeResult[0]
		peakTime = sSpeResult[1]
		if i == 0:
			firstPeTime = peakTime

		lTotalWaveform = [(x + y) for x,y in zip(lTotalWaveform, currentWaveform)]


	#print lTotalWaveform
	return lTotalWaveform


def correct_waveform(lWaveform, correctionTerm):
	lWaveform = [x/correctionTerm for x in lWaveform]


	return lWaveform




# MUST INCLUDE TYPES TO AVOID SEG FAULT
stl.vector(stl.vector('float'))
stl.vector(stl.vector('int'))

filename = './sample_spe.p'

lAllSPE = pickle.load(open(filename, 'rb'))
#lNormalizedSPE = preprocessing.normalize(lAllSPE, norm='l2')

RISE_TIME = 2.7 # ns
FALL_TIME = 8.2 # ns
NOISE_MEAN = 1091.90862539 # channels
NOISE_SIGMA = 54240.6315954 # channels
SHORT_DECAY = 3.1 # ns
LONG_DECAY = 24 # ns
SINGLET_OVER_TRIPLET = 0.17 # direct excitation only (not recombination)

numWaveformsToSim = 100

numTestSamples = len(lAllSPE)
aSums = np.zeros(numTestSamples)

# normalize each sample waveform
for i in xrange(numTestSamples):
	aSums[i] = sum(lAllSPE[i])

sumMean = np.mean(aSums)
sumStd = np.std(aSums)

#print sumMean, sumStd
#for i in xrange(100):
#	print double_exponential_normalized(SHORT_DECAY, LONG_DECAY, SINGLET_OVER_TRIPLET)

#print 1./(1e8*50.*10.*1.6e-19)
electronTomVConversion = 1./(1e8*50.*10.*1.6e-19) # [electrons / mV]
electronConversion = electronTomVConversion * 0.0001373291 # 14 bits over 2.25V dynamic range

# lCurrentWaveform is the output (in electrons) of the waveform
# This is not the number of channels on the ADC which is the final step
lCurrentWaveform = mpe_waveform_generator(1, SHORT_DECAY, LONG_DECAY, RISE_TIME, FALL_TIME, sumMean, sumStd, NOISE_MEAN, NOISE_SIGMA, True)
#lCurrentWaveform = correct_waveform(lCurrentWaveform, electronConversion)

plt.plot(range(20), lCurrentWaveform, drawstyle='steps')
plt.show()




