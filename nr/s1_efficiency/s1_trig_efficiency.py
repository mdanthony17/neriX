#!/usr/bin/python

import ROOT as root
import sys
from rootpy import stl
from rootpy.plotting import Hist2D, Hist, Legend, Canvas
from rootpy.io import File
from rootpy.tree import Tree, TreeModel, TreeChain

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

defaultTree = fDefault.T1
defaultTree.AddFriend(fDefault.T2)
defaultTree.AddFriend(fDefault.T3)

ledTree = fLed.T0

defaultTree.create_buffer()
ledTree.create_buffer()

nBinsS1 = 60
lbS1 = 0
ubS1 = 3

c1 = Canvas()

hS1Efficiency = Hist(nBinsS1, lbS1, ubS1, name='hS1Default', title='Default Processing', drawstyle='hist')
hS1Led = Hist(nBinsS1, lbS1, ubS1, name='hS1Led', title='LED Processing', drawstyle='hist')

for i in xrange(0, ledTree.GetEntriesFast()):
	if i % 10000 == 0:
		print 'Processing event ' + str(i) + ' / ' + str(ledTree.GetEntriesFast())
	
	#if i == 1000:
	#	break

	defaultTree.GetEntry(i)
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

	#
	#print len(ledSingleBefore), len(ledSingleAfter)

	ledSinglePeak = list(ledTree.SinglePeak)
	if len(ledSinglePeak) > 0:
		ledSinglePeak = ledSinglePeak[16]

	ledPeak = (ledSinglePeak + ledSingleBefore[0] + ledSingleBefore[1] + ledSingleBefore[2] + ledSingleBefore[3] + ledSingleBefore[4] + ledSingleAfter[0] + ledSingleAfter[1] + ledSingleAfter[2] + ledSingleAfter[3])/1.48e6
	




	s1Time = list(defaultTree.S1sPeak)
	if len(s1Time) == 0:
		s1Time = list(defaultTree.S2sPeak)
		#if ledPeak > 3 and ledPeak < 4:
		#	print i
		if len(s1Time) == 0:
			#if ledPeak > 1 and ledPeak < 2:
			#	print i
			continue

	#print ledTime, s1Time

	for time in s1Time:
		if (ledTime - time)**2 < acceptableTimeDifferenceSquared:
			#ledHeight = list(ledTree.SingleHeight)[16]
			hS1Efficiency.Fill(ledPeak)
			
			break
		else:
			
			#print 'Difference: ' + str(((ledTime - s1Time)**2)**0.5)
			pass


ledParameter = '(SingleBefore[16][1]+SingleBefore[16][0]+SinglePeak[16]+SingleAfter[16][0]+SingleAfter[16][1]) / 1.48e6'
#ledParameter = 'SingleHeight[16]'
ledCut = 'SingleSample[16] > 157 && SingleSample[16] < 188'
#ledCut = 'SingleSample[16] > 157 && SingleSample[16] < 188 && SingleAfter[16][0]/SinglePeak[16] < 1 && SingleBefore[16][0]/SinglePeak[16] < 0.35'
fLed.T0.Draw(ledParameter, hist=hS1Led, selection=ledCut)

hS1Efficiency.SetColor('b')
hS1Led.SetColor('r')

hS1Led.Draw()
hS1Efficiency.Draw('same')

c1.Update()

c2 = Canvas()

hS1EfficiencyClone = hS1Efficiency.Clone()
hS1EfficiencyClone.Divide(hS1Led)
hS1EfficiencyClone.Draw()

c2.Update()

raw_input('Please press enter to continue...')




