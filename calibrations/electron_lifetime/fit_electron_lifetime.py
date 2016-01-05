#!/usr/bin/python

import ROOT as root
import sys
import neriX_analysis
from rootpy.plotting import Hist2D, Hist, Legend
import math

#--------------- Start Parameters to Change ----------------


s2NumBins = 20
s2Min = 20e3
s2Max = 70e3

dtNumBins = 20
dtMin = 3.
dtMax = 11.


#--------------- End Parameters to Change ----------------



if(len(sys.argv) != 2):
	print 'Usage is python coin_analysis <ROOT filename>'
	sys.exit(1)

file1 = sys.argv[1]
run1 = neriX_analysis.neriX_analysis(file1)


#choose options
option1 = 'COLZ'
optionSame = 'SAME'


#choose cuts for run 1
run1.add_dt_cut(dtMin, dtMax)
run1.add_radius_cut(0, 0.5)

# cut on 40 keV peak
run1.add_cut('S1sTotBottom[0] > 80 && S1sTotBottom[0] < 380')
run1.add_cut('S2sTotTop[0] > 20e3 && S2sTotTop[0] < 70e3')

# cut on 164 keV peak
#run1.add_cut('S1sTotBottom[0] > 400 && S1sTotBottom[0] < 900')
#run1.add_cut('S2sTotBottom[0] > 200e3 && S2sTotBottom[0] < 400e3')

#run1.get_T1().Scan('EventId:S2sTot[0]:S2sSaturation[0]', run1.get_cuts())

run1.set_event_list()

#create histograms for run1

ha2 = Hist2D(dtNumBins, dtMin, dtMax, s2NumBins, s2Min, s2Max, name='ha2', title='dt vs S2 - ' + file1)
run1.Draw('dt:S2sTotBottom[0]', hist=ha2)
ha2.GetXaxis().SetTitle('dt [\mu s]')
ha2.GetYaxis().SetTitle('S2sTotBottom[0] [PE]')
ha2.SetStats(0)


hProfile = ha2.ProfileX('hProfile')

hProfile.GetXaxis().SetTitle('dt [\mu s]')
hProfile.GetYaxis().SetTitle('S2sTotBottom[0] [PE]')
hProfile.GetYaxis().SetRangeUser(s2Min, s2Max)
hProfile.SetMarkerSize(0)
hProfile.SetStats(0)



c1 = root.TCanvas('c1','',1100,480)
c1.Divide(2)
c1.cd(1)
ha2.Draw(option1)
c1.SetLogz()
c1.SetGridx()
c1.SetGridy()
c1.cd(2)
hProfile.Draw()
c1.SetGridx()
c1.SetGridy()


f1 = root.TF1('f1', 'pol1', dtMin, dtMax)

hProfile.Fit('f1', 'f')
f1.SetLineStyle(1)
f1.SetLineWidth(2)
f1.SetLineColor(root.kRed)

c1.cd(1)
f1.Draw('same')
c1.cd(2)
f1.Draw('same')


#sFitInfo1 = 'variation = (%.2e +/- %.2e) [(Change in S2 / Mean)/us]' % (math.fabs(f1.GetParameter(1)/ha2.GetMean(2)), f1.GetParError(1)/ha2.GetMean(2))
electronLifetime = (1/math.exp(1)-1)*f1.GetParameter(0)/f1.GetParameter(1)
sFitInfo1 = 'e^{-} lifetime = (%.2f +/- %.2f) us' % ( electronLifetime, math.fabs(electronLifetime * ((f1.GetParError(0)/(f1.GetParameter(0)*(1/math.exp(1)-1)))**2 + (f1.GetParError(1)/f1.GetParameter(1))**2)**0.5 ) )
pt1 = root.TPaveText(.1,.65,.9,.9,'blNDC')
text1 = pt1.AddText(sFitInfo1)
pt1.SetTextColor(root.kBlue)
pt1.SetFillStyle(0)
pt1.SetBorderSize(0)
pt1.Draw('same')


c1.Update()

raw_input('Enter to continue...')

c1.SaveAs('~/Desktop/electron_lifetime.png')
