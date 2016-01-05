#!/usr/bin/python

import ROOT as root
import sys, pickle
from rootpy import stl
from rootpy.plotting import Hist2D, Hist, Legend, Canvas
from rootpy.io import File
from rootpy.tree import Tree, TreeModel, TreeChain
import numpy as np

# MUST INCLUDE TYPES TO AVOID SEG FAULT
stl.vector(stl.vector('float'))
stl.vector(stl.vector('int'))

acceptableTimeDifferenceSquared = 15**2

defaultDataPath = './data_default/'
ledDataPath = './data_led/'

if(len(sys.argv) != 2):
	print 'Usage is python s1_trig_efficiency.py <ROOT filename>'
	sys.exit(1)

filename = sys.argv[1]

if filename[-5:] != '.root':
	filename += '.root'

fDefault = File(defaultDataPath + filename)
fLed = File(ledDataPath + filename)

ledTree = fLed.T0

ledTree.create_buffer()


lenSingleBefore = 5
lenSingleAfter = 4

lAllSPE = []


for i in xrange(0, ledTree.GetEntriesFast()):
	if i % 10000 == 0:
		print 'Processing event ' + str(i) + ' / ' + str(ledTree.GetEntriesFast())
	
	#if i == 1000:
	#	break

	ledTree.GetEntry(i)

	
	ledTime = list(ledTree.SingleSample)
	if len(ledTime) > 0:
		ledTime = ledTime[16]
	else:
		continue

	# make sure time is between good window
	if ledTime < 157 or ledTime > 188:
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

	ledPeak = (ledSinglePeak + ledSingleBefore[0] + ledSingleBefore[1] + ledSingleBefore[2] + ledSingleBefore[3] + ledSingleBefore[4] + ledSingleAfter[0] + ledSingleAfter[1] + ledSingleAfter[2] + ledSingleAfter[3])/1.48e6

	if ledPeak < 0.5 or ledPeak > 1.2:
		continue
	print 'Current Event: ' + str(i)

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

	lAllSPE.append(lCurrentSPE)

pickle.dump(lAllSPE, open('sample_spe.p', 'wb'))


