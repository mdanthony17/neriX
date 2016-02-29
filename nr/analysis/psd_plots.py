#!/usr/bin/python

import ROOT as root
import sys
import neriX_analysis
from rootpy.plotting import Hist2D, Hist, Legend, Canvas

#--------------- Start Parameters to Change ----------------


psdNumBins = 50
psdMin = 0
psdMax = 0.5

lhNumBins = 50
lhMin = 0
lhMax = 2


#--------------- End Parameters to Change ----------------



if(len(sys.argv) != 2):
	print 'Usage is python coincidence_quick_check.py <filename>'
	sys.exit(1)

filename = sys.argv[1]

current_analysis = neriX_analysis.neriX_analysis(filename)

colzOption = 'COLZ'



current_analysis.set_event_list()


numEJs = 4
lCanvas = [0 for i in xrange(numEJs)]
lPSDHists = [0 for i in xrange(numEJs)]
for i in xrange(numEJs):
	lCanvas[i] = Canvas(1280/2,480)

	lPSDHists[i] = Hist2D(lhNumBins, lhMin, lhMax, psdNumBins, psdMin, psdMax, name='hEJ' + str(i), title='PSD Plot for EJ%d' % (i))
	current_analysis.Draw('LiqSciHeight[%d]:LiqSciTailRaw[%d]/LiqSciRaw[%d]' % (i, i, i), hist=lPSDHists[i])
	lPSDHists[i].GetXaxis().SetTitle('LiqSciHeight [V]')
	lPSDHists[i].GetYaxis().SetTitle('PSD Parameter')
	lPSDHists[i].SetMarkerSize(0)
	lPSDHists[i].SetStats(0)

	lPSDHists[i].Draw()
	lCanvas[i].Update()



raw_input("Enter to continue...")
