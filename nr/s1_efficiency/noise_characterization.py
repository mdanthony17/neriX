#!/usr/bin/python

import ROOT as root
import sys
from rootpy import stl
from rootpy.plotting import Hist2D, Hist, Legend, Canvas
from rootpy.io import File
from rootpy.tree import Tree, TreeModel, TreeChain
import numpy as np
import matplotlib.pyplot as plt

# MUST INCLUDE TYPES TO AVOID SEG FAULT
stl.vector(stl.vector('float'))
stl.vector(stl.vector('int'))


ledDataPath = './data_led/'

if(len(sys.argv) != 2):
	print 'Usage is python s1_trig_efficiency.py <ROOT filename>'
	sys.exit(1)

filename = sys.argv[1]

if filename[-5:] != '.root':
	filename += '.root'

fLed = File(ledDataPath + filename)

ledTree = fLed.T0

ledTree.create_buffer()


lenSingleBefore = 5
lenSingleAfter = 4

lMeans = []
lStds = []


for i in xrange(0, ledTree.GetEntriesFast()):
	if i % 10000 == 0:
		print 'Processing event ' + str(i) + ' / ' + str(ledTree.GetEntriesFast())
	
	#if i == 10000:
	#	break

	ledTree.GetEntry(i)

	
	ledTime = list(ledTree.SingleSample)
	if len(ledTime) > 0:
		ledTime = ledTime[16]
	else:
		continue

	# make sure time is NOT in a good window for noise
	if ledTime > 150:
		continue


	ledSingleBefore = list(ledTree.SingleBefore)
	if len(ledSingleBefore) == 0:
		continue
	else:
		ledSingleBefore = list(ledSingleBefore[16])

	ledSingleAfter = list(ledTree.SingleAfter)
	if len(ledSingleAfter) == 0:
		continue
	else:
		ledSingleAfter = list(ledSingleAfter[16])

	ledSinglePeak = list(ledTree.SinglePeak)
	if len(ledSinglePeak) > 0:
		ledSinglePeak = ledSinglePeak[16]


	lCurrentSPE = [0 for i in xrange(lenSingleAfter+lenSingleBefore+1)]

	currentIndex = 0
	for i in xrange(lenSingleBefore-1, -1, -1):
		lCurrentSPE[currentIndex] = ledSingleBefore[i]
		currentIndex += 1

	lCurrentSPE[currentIndex] = ledSinglePeak
	currentIndex += 1

	for i in xrange(0, lenSingleAfter):
		lCurrentSPE[currentIndex] = ledSingleAfter[i]
		currentIndex += 1

	#print ledSingleAfter
	#print lCurrentSPE

	lMeans.append(np.mean(lCurrentSPE[:3]))
	lStds.append(np.std(lCurrentSPE[:3]))

# average of averages and average of standard deviations
meanNoise = np.mean(lMeans)
sigmaNoise = np.mean(lStds)


print 'Mean: ' + str(meanNoise)
print 'Sigma: ' + str(sigmaNoise)

#plt.hist(lAllSPE, bins=10, range=(-2*sigmaNoise, 2*sigmaNoise))
#plt.axis([-2*sigmaNoise, 2*sigmaNoise, 0, 1e6])
#plt.show()
