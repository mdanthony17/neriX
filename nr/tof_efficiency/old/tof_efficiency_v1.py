#!/usr/bin/python

import ROOT as root
import sys, array, math
import neriX_analysis
from rootpy.plotting import Hist, Hist2D, Canvas, Legend
from rootpy.io import root_open, File

if(len(sys.argv) != 2):
	print 'Usage is s1_tac_correction.py <ROOT filename>'
	sys.exit(1)
	
filename = sys.argv[1]


tof_analysis = neriX_analysis.neriX_analysis(filename)


#choose cuts
tof_analysis.add_dt_cut(2., 13.)
tof_analysis.add_radius_cut(0., 0.7)

# see if S1 caused a discriminator pulse that will be sent
# to the TAC
discPulseCut = 'TrigLeftEdge-S1sPeak[0]>-1 && TrigLeftEdge-S1sPeak[0]<3'

#--------------- Start Parameters to Change ----------------


parameterToExamineS1 = 'S1sTotBottom[0]'
lowerBoundS1 = -0.5
upperBoundS1 = 99.5
nBinsS1 = 100


#--------------- End Parameters to Change ----------------

#tof_analysis.get_T1().Scan('EventId:TrigLeftEdge-S1sPeak[0]:S1sTotBottom[0]', 'S1sTotBottom[0] > 20')


c1 = root.TCanvas('c1','c1',200,10,1000,700)
c1.SetGridx()
c1.SetGridy()


h1 = Hist(nBinsS1, lowerBoundS1, upperBoundS1, name='s1_spec_no_trig_cut', title='s1_spec_no_trig_cut', drawstyle='hist')
tof_analysis.Draw(parameterToExamineS1, hist=h1, selection='')

h2 = Hist(nBinsS1, lowerBoundS1, upperBoundS1, name='s1_spec_with_trig_cut', title='s1_spec_with_trig_cut', drawstyle='hist')
tof_analysis.Draw(parameterToExamineS1, hist=h2, selection=discPulseCut)

hEff = root.TEfficiency(h2, h1)
hEff.SetTitle('S1 Efficiency for TAC; S1sTotBottom[0] [PE]; Percentage Causing Discriminator Pulse for TAC');
#hEff.GetXaxis().SetTitle('S1 [PE]')
#hEff.GetYaxis().SetTitle('Percentage Causing Discriminator Pulse for TAC')
hEff.Draw()
c1.Update()

paintedEfficiency = hEff.GetPaintedGraph()
paintedEfficiency.GetXaxis().SetRangeUser(lowerBoundS1, upperBoundS1)

"""

fSigmoid = root.TF1('fSigmoid', '-1 + (2. / (1 + exp([1]*([0]-x)))^[2])', lowerBoundS1, upperBoundS1)
fSigmoid.SetParameters(0., 0.1, 1)
fSigmoid.FixParameter(0, 0)
#fSigmoid.SetParLimits(0, -50, 15)
fSigmoid.SetParLimits(1, 0.01, 0.5)
fSigmoid.SetParLimits(2, 0.1, 100)
#fSigmoid.SetParLimits(3, 0.01, 0.5)
#fSigmoid.FixParameter(0, 1050)
#fSigmoid.FixParameter(1, 0.03)
#fSigmoid.FixParameter(3, .1)
fSigmoid.SetLineStyle(9)
fSigmoid.SetLineWidth(1)

hEff.Fit(fSigmoid)
fSigmoid.Draw('same')
"""

c1.Update()


raw_input('Enter to continue...')

response = raw_input('Would you like to save the histograms used for TEfficiency for later analysis?  Press "y" to proceed, otherwise press anything: ')

if response == 'y':
	outputFile = root_open('../leff_minimizer/efficiency_files/s1_disc_efficiency.root', 'recreate')
	
	h1.Write()
	h2.Write()

	sCuts = root.TObjString('cuts_used')
	sCuts.Write(tof_analysis.get_cuts(), root.TObject.kOverwrite)

