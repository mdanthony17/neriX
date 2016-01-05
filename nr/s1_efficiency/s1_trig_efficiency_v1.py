#!/usr/bin/python

import ROOT as root
import sys
from rootpy.plotting import Hist2D, Hist, Legend, Canvas
from rootpy.io import File
from rootpy.tree import Tree, TreeModel, TreeChain


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

#defaultParameter = 'S2sTotBottom[0]' # no S1 if can't find S2!
#ledParameter = '(SingleBefore[16][1]+SingleBefore[16][0]+SinglePeak[16]+SingleAfter[16][0]+SingleAfter[16][1]) / 1.48e6'
defaultParameter = 'S1sHeight[0]'
ledParameter = 'SingleHeight[16]'

nBinsS1 = 100
lbS1 = 0
ubS1 = 0.04

c1 = Canvas()

hS1Default = Hist(nBinsS1, lbS1, ubS1, name='hS1Default', title='Default Processing', drawstyle='hist')
hS1Led = Hist(nBinsS1, lbS1, ubS1, name='hS1Led', title='LED Processing', drawstyle='hist')


#fDefault.T1.Scan('EventId', defaultParameter + ' > .02')
#fDefault.T1.Scan('EventId:S1sPeak:'+defaultParameter, defaultParameter + ' > .01 && ' + defaultParameter + ' < .03 && S1sPeak > 150')
#fLed.T0.Scan('EventId:SingleSample[0]:'+ledParameter, ledParameter + ' > .01 && ' + ledParameter + ' < .03 && SingleSample[0] > 150')

fDefault.T1.Draw(defaultParameter, hist=hS1Default)
fLed.T0.Draw(ledParameter, hist=hS1Led)

hS1Default.Sumw2()
hS1Led.Sumw2()

hS1Default.SetColor('b')
hS1Led.SetColor('r')

hS1Led.Draw()
hS1Default.Draw('same')

c1.Update()


c2 = Canvas()

hS1DefaultClone = hS1Default.Clone()
hS1DefaultClone.Divide(hS1Led)
hS1DefaultClone.Draw()

c2.Update()


raw_input('Please press enter to continue...')




