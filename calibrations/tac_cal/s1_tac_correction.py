#!/usr/bin/python

import ROOT as root
import sys, array, math
import neriX_analysis
from rootpy.plotting import Hist, Hist2D, Canvas, Legend

"""
if(len(sys.argv) != 2):
	print 'Usage is s1_tac_correction.py <ROOT filename>'
	sys.exit(1)
	
filename = sys.argv[1]
"""

lTofFiles = ['nerix_151014_1734.root', 'nerix_151015_1607.root', 'nerix_151015_1713.root', 'nerix_151016_0731', 'nerix_151016_0941']


run = neriX_analysis.neriX_analysis(lTofFiles, -5, 1.054, 4.5)

#choose options
option1 = 'COLZ'



#choose cuts
run.add_dt_cut(2., 13.)
run.add_radius_cut(0., 0.7)
run.add_cut('TacHeight > 0')
#run.add_cut('(LiqSciPeak[3] - S1sPeak[]) < 10 && (LiqSciPeak[3] - S1sPeak[]) > -2')

run.set_event_list()


#--------------- Start Parameters to Change ----------------

#constants for histogram
#parameter 1 will be on x-axis
parameterToExamineTAC = 'TimeOfFlight'#'TacHeight'
lowerBoundTAC = -20#0.24
upperBoundTAC = 15#0.35
nBinsTAC = 40


parameterToExamineS1 = 'S1sTotBottom[0]'
lowerBoundS1 = 1
upperBoundS1 = 2000
nBinsS1 = 40

"""
parameterToExamineS1 = 'S1sHeight[0][16]'
lowerBoundS1 = .001
upperBoundS1 = 10
nBinsS1 = 50
"""

logLowerBoundS1 = math.log(lowerBoundS1, 10)
logUpperBoundS1 = math.log(upperBoundS1, 10)

#choose options
option1 = 'COLZ'

#--------------- End Parameters to Change ----------------

#run.get_tree().Scan('EventId:TrigArea:TrigLeftEdge-S1sPeak[0]:GeHeight[0]', run.get_cuts())


c1 = root.TCanvas('c1','c1',200,10,1000,700)
h1 = Hist2D(nBinsS1, lowerBoundS1, upperBoundS1, nBinsTAC, lowerBoundTAC, upperBoundTAC, name='h1', title='h1', drawstyle='hist')

c1.SetGridx(1)
c1.SetGridy()

h1.SetTitle('')
h1.GetXaxis().SetTitle('S1 [PE]')
h1.GetYaxis().SetTitle('Time of Flight [ns]')

# make log-bins on x-axis

aBins = array.array('f', [0 for i in xrange(nBinsS1+1)])
logBinWidth = (logUpperBoundS1 - logLowerBoundS1)/nBinsS1
for i in xrange(nBinsS1+1):
	aBins[i] = 10.**(logLowerBoundS1 + i*logBinWidth)
h1.GetXaxis().Set(nBinsS1, aBins)
c1.SetLogx()
c1.SetLogz()


run.Draw(parameterToExamineS1 + ':' + parameterToExamineTAC, hist=h1, selection='')
h1.SetStats(0)
h1.Draw(option1)

hProfile = h1.ProfileX('hProfile')
hProfile.SetMarkerSize(0.5)
hProfile.SetStats(0)
hProfile.Draw('same')

#h1.GetPalette().Draw('same')


"""
fCorrection = root.TF1('fCorrection', 'pol6', lowerBoundS1, upperBoundS1)
hProfile.Fit(fCorrection, 'NE')
fCorrection.Draw('same')
"""


lZero = root.TLine()
lZero.SetLineColor(root.kMagenta)
#lZero.SetLineStyle(7)
lZero.SetLineWidth(3)
#print gVoltageVsGain.GetYaxis().GetXmin(),
lZero.DrawLine(lowerBoundS1, 0, upperBoundS1, 0)
lZero.Draw('same')


c1.Update()


raw_input('Enter to continue...')
