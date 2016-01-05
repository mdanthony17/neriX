import ROOT as root
from ROOT import gROOT
import sys, os, root_numpy, threading, random, emcee, corner
from rootpy.io import File
from rootpy.tree import Tree, TreeModel, TreeChain
from rootpy.plotting import Hist, Hist2D, Canvas, Graph, func
import numpy as np
from math import exp, factorial, erf
from iminuit import Minuit
from scipy import optimize, misc
import copy_reg, types, pickle, click, time
# for debugging
import matplotlib.pyplot as plt
import matplotlib.mlab as mlab



def produce_binomial_probabilities_range(probabiltyOfSuccuess, numberOfTrials, lowBound, highBound, stepSize = 1):
	# make sure that bounds and stepsize are integers
	assert isinstance(numberOfTrials, int)
	assert isinstance(lowBound, int)
	assert isinstance(highBound, int)
	assert isinstance(stepSize, int)
	assert (highBound - lowBound) % stepSize == 0
	
	if probabiltyOfSuccuess < 0 or probabiltyOfSuccuess > 1:
		#print 'Probability takes on value less than zero or greater than one - return zeros.'
		return np.zeros((highBound - lowBound) / stepSize)
	
	if stepSize != 1:
		print 'WARNING: This function should be used with a step size of one unless one handles rebinning properly (currently does not).'
	
	aBinomial = np.zeros((highBound - lowBound) / stepSize)

	for i, numberOfSucceses in enumerate(xrange(lowBound, highBound, stepSize)):
		if numberOfSucceses < 0 or numberOfSucceses > numberOfTrials:
			aBinomial[i] = 0.
		else:
			nChooseK = misc.comb(numberOfTrials, numberOfSucceses)
			probabilityTerm = probabiltyOfSuccuess**numberOfSucceses * (1-probabiltyOfSuccuess)**(numberOfTrials-numberOfSucceses)
			aBinomial[i] = nChooseK*probabilityTerm

	return aBinomial


def binomial_prob(numberOfSucceses, numberOfTrials, probabiltyOfSuccuess):
	if numberOfSucceses < 0 or numberOfSucceses > numberOfTrials:
		return 0
	
	nChooseK = misc.comb(numberOfTrials, numberOfSucceses)
	probabilityTerm = probabiltyOfSuccuess**numberOfSucceses * (1-probabiltyOfSuccuess)**(numberOfTrials-numberOfSucceses)
	return nChooseK*probabilityTerm




maxNumPhotons = 2
maxNumElectrons = 2
maxNumExcitons = 2
maxNumIons = 2

probQuenching = 0.1


# rows are excitons
# columns are ions
aCountsInitial = np.asarray([[0, 0], [5., 10.]])

aUnfolding = np.zeros(shape=(maxNumPhotons, maxNumElectrons, maxNumExcitons, maxNumIons))

aExcitonCenters = np.asarray([i for i in xrange(maxNumExcitons)])
aIonCenters = np.asarray([i for i in xrange(maxNumIons)])

print aExcitonCenters

# works!!

for i in xrange(maxNumPhotons):
	for j in xrange(maxNumElectrons):
		currentBinomial = lambda numExcitons, numIons: binomial_prob(i, numExcitons, 1-probQuenching)*binomial_prob(j, numIons, 1.)
		uFuncBinomial = np.frompyfunc(currentBinomial, 2, 1)
		#print i, j
		#print uFuncBinomial.outer(aExcitonCenters, aIonCenters)
		aUnfolding[i, j, :, :] = uFuncBinomial.outer(aExcitonCenters, aIonCenters)

print aUnfolding
print np.tensordot(aUnfolding, aCountsInitial, axes=([2, 3], [0, 1]))
"""

def findMatrixForGivenNumberOfPhotonsAndElectrons(i, j):
	currentBinomial = lambda numExcitons, numIons: binomial_prob(i, numExcitons, 1-probQuenching)*binomial_prob(j, numIons, 1.)
	uFuncBinomial = np.frompyfunc(currentBinomial, 2, 1)
	print i, j
	print uFuncBinomial.outer(aExcitonCenters, aIonCenters)
	aUnfolding[i, j, :, :] = uFuncBinomial.outer(aExcitonCenters, aIonCenters)
"""






