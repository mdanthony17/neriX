#!/usr/bin/python

import ROOT as root
import sys
import neriX_analysis
from rootpy.plotting import Hist2D, Hist, Legend, Canvas

#--------------- Start Parameters to Change ----------------

s2NumBins = 20
s2Min = 0.
s2Max = 20000

#--------------- End Parameters to Change ----------------



if(len(sys.argv) != 2):
	print 'Usage is python coin_analysis <ROOT filename>'
	sys.exit(1)

filename = sys.argv[1]
current_analysis = neriX_analysis.neriX_analysis(filename)



#choose cuts for run 1
current_analysis.add_dt_cut()
current_analysis.add_cut('NaiPeak > 1100 && NaiPeak < 1300')
#current_analysis.add_cut('(TrigLeftEdge[0] - S2sPeak[0]) > -30 && (TrigLeftEdge[0] - S2sPeak[0]) < 50')
current_analysis.add_cut('(NaiLeftEdge[0] - S1sPeak[0]) > 55 && (NaiLeftEdge[0] - S1sPeak[0]) < 80')

sAdditionalCut = '(TrigLeftEdge[0] - S2sPeak[0]) > -30 && (TrigLeftEdge[0] - S2sPeak[0]) < 50'
#sAdditionalCut = 'TrigLeftEdge[0] > -1'

current_analysis.set_event_list()

c1 = Canvas()

hS2 = Hist(s2NumBins, s2Min, s2Max, name='hS2', title='S2 Spectrum', drawstyle='hist')
current_analysis.Draw('S2sTotBottom[0]', hist=hS2)
hS2.Sumw2()

hS2.Draw()
c1.Update()

c2 = Canvas()

hS2Efficiency = Hist(s2NumBins, s2Min, s2Max, name='hS2Efficiency', title='S2 Efficiency - ' + filename)
current_analysis.Draw('S2sTotBottom[0]', hist=hS2Efficiency, selection=sAdditionalCut)
hS2Efficiency.Sumw2()
hS2Efficiency.GetXaxis().SetTitle('S2sTotBottom[0] [pe]')
hS2Efficiency.GetYaxis().SetTitle('Percentage of Events with Trigger')
hS2Efficiency.SetStats(0)

hS2Efficiency.Divide(hS2)
#hS2Efficiency.Scale(1./0.9) # scale for dead time on S2 holdoff

hS2Efficiency.Draw('e')
hS2Efficiency.SetMarkerSize(1)


fSigmoid = root.TF1('fSigmoid', '[0] / (1 + exp([2]*([1]-x)))', s2Min, s2Max)
#fSigmoid.SetParameters(9, 0.5, 740, 2e-4)
fSigmoid.SetParLimits(0, .9, 1.1)
fSigmoid.SetParLimits(1, 3000, 5000)
fSigmoid.SetParLimits(2, 1e-3, 1e-2)
#fSigmoid.FixParameter(0, 9.55)
#fSigmoid.FixParameter(1, 4000)
#fSigmoid.FixParameter(2, .1)
fSigmoid.SetLineStyle(9)
fSigmoid.SetLineWidth(1)

hS2Efficiency.Fit('fSigmoid', 'LL', '', s2Min, s2Max)
sigmoidMax = float(fSigmoid.GetParameter(0))
fMaxLine = root.TF1('fMaxLine', 'pol0', s2Min, s2Max)
fMaxLine.SetParameter(0, sigmoidMax)
fMaxLine.SetLineColor(root.kBlue)
fMaxLine.SetLineStyle(7)
fMaxLine.SetLineWidth(1)

fSigmoid.Draw('same')
fMaxLine.Draw('same')


c2.Update()

raw_input('Please press enter to continue...')




