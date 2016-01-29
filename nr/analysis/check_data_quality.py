#!/usr/bin/python

import ROOT as root
import sys
import neriX_analysis
from rootpy.plotting import Hist2D, Hist, Legend

#--------------- Start Parameters to Change ----------------

eDepNumBins = 50
eDepMin = -2.
eDepMax = 25.

s1NumBins = 20
s1Min = 0.
s1Max = 40

s2NumBins = 20
s2Min = 0.
s2Max = 4000

xyNumBins = 100
xyMin = -30
xyMax = 30

rNumBins = 100
rMin = 0.
rMax = 22.

dtNumBins = 100/2
dtMin = -20.
dtMax = 0.

s2WidthNumBins = 200
s2WidthMin = 40.
s2WidthMax = 160.

#for S1 trigger analysis
s1TopNumBins = 30
s1TopMin = 0
s1TopMax = 15

s1BotNumBins = 100
s1BotMin = 0
s1BotMax = 180


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
run1.add_z_cut()
run1.add_radius_cut(0, 0.85)
run1.add_xs1asym_cut()
run1.add_xs2asym_cut()
run1.add_single_scatter_cut()
#run1.add_cut('TMath::Log10(S2sTotBottom[0]/S1sTotBottom[0]) < 3 && S1sTotBottom[0] < 35')

#print 'Livetime: ' + str(run1.get_livetime())

run1.set_event_list()


s1Branch = 'cpS1sTotBottom[0]'
s2Branch = 'cpS2sTotBottom[0]'


#create histograms for run1
"""
ha1 = root.TH2F('ha1', '661.657 - GeEnergy vs S1', s1NumBins, s1Min, s1Max, eDepNumBins, eDepMin, eDepMax)
run1.get_tree().Draw('eDep:S1sTot[0]>>ha1', run1.get_cuts(), '')

ha1.GetXaxis().SetTitle('S1sTot[0] [PE]')
ha1.GetYaxis().SetTitle('661.657 - GeEnergy [keV]')
ha1.SetStats(0)
"""
ha2 = Hist2D(s1NumBins, s1Min, s1Max, s2NumBins, s2Min, s2Max, name='ha2', title='S1 vs S2 - ' + file1)
run1.Draw('%s:%s' % (s1Branch, s2Branch), hist=ha2)
ha2.GetXaxis().SetTitle('%s [PE]' % s1Branch)
ha2.GetYaxis().SetTitle('%s [PE]' % s2Branch)
ha2.SetStats(0)

ha3 = Hist2D(xyNumBins, xyMin, xyMax, xyNumBins, xyMin, xyMax, name='ha3', title='XY Dist - ' + file1)
run1.Draw('X*20.9:Y*20.9', hist=ha3)
ha3.GetXaxis().SetTitle('X [mm]')
ha3.GetYaxis().SetTitle('Y [mm]')
ha3.SetStats(0)

ha4 = Hist2D(rNumBins, rMin**2, rMax**2, dtNumBins, dtMin, dtMax, name='ha4', title='neriX Uniformity - ' + file1)
run1.Draw('R*R*20.9*20.9:-dt', hist=ha4)
ha4.GetXaxis().SetTitle('R^2 [mm^2]')
ha4.GetYaxis().SetTitle('dt [#mus]')
ha4.SetStats(0)

ha5 = Hist2D(s2WidthNumBins, s2WidthMin, s2WidthMax, dtNumBins, dtMin, dtMax, name='ha5', title='S2sLowWidth vs dt')
run1.Draw('S2sLowWidth[0]:-dt', hist=ha5)
ha5.SetTitle('S2sLowWidth[0] [10 ns]')
ha5.GetYaxis().SetTitle('dt [#mus]')
ha5.SetStats(0)
"""
ha6 = root.TH1F('ha6', '661.657 - GeEnergy', eDepNumBins, eDepMin, eDepMax)
run1.get_tree().Draw('eDep>>ha6', run1.get_cuts(), '')
ha6.GetXaxis().SetTitle('661.657 - GeEnergy [keV]')
ha6.GetYaxis().SetTitle('Rate')
ha6.Scale(1./float(run1.get_livetime()))
ha6.SetLineColor(root.kBlue)
ha6.SetStats(0)

ha7 = root.TH1F('ha7', 's1', s1NumBins, s1Min, s1Max)
run1.get_tree().Draw('S1sTot[0]>>ha7', run1.get_cuts(), '')
ha7.GetXaxis().SetTitle('S1sTot[0] [PE]')
ha7.GetYaxis().SetTitle('Rate')
ha7.Scale(1./float(run1.get_livetime()))
ha7.SetLineColor(root.kBlue)
ha7.SetStats(0)

ha8 = root.TH1F('ha8', 's2', s2NumBins, s2Min, s2Max)
run1.get_tree().Draw('S2sTot[0]>>ha8', run1.get_cuts(), '')
ha8.GetXaxis().SetTitle('S2sTot[0] [PE]')
ha8.GetYaxis().SetTitle('Rate')
ha8.Scale(1./float(run1.get_livetime()))
ha8.SetLineColor(root.kBlue)
ha8.SetStats(0)

ha11 = root.TH2F('ha11', 'GeHeight vs Time', 100, run1.get_timestamp(0), run1.get_timestamp(run1.get_tree().GetEntries() - 1), 50, 1.3, 2.3)
run1.get_tree().Draw('GeHeight[0]:TimeSec>>ha11', run1.get_cuts(), '')
ha11.GetXaxis().SetTitle('TimeSec [s]')
ha11.GetYaxis().SetTitle('GeHeight[0] [V]')
ha11.SetStats(0)
"""
ha12 = Hist(dtNumBins, dtMin, dtMax, name='ha12', title='dt - No washer beneath bricks, two for collimation', drawstyle='hist')
ha12.SetColor(root.kRed)
run1.Draw('-dt', hist=ha12)
ha12.GetXaxis().SetTitle('dt [us]')
ha12.GetYaxis().SetTitle('Normalized Counts')
ha12.Scale(1./ha12.Integral())
ha12.SetStats(0)


ha13 = Hist2D(s1NumBins, s1Min, s1Max, 20, 1, 3.5, name='ha13', title='neriX Discrimination - ' + file1)
run1.Draw('%s:TMath::Log10(%s/%s)' % (s1Branch, s2Branch, s1Branch), hist=ha13)
ha13.GetXaxis().SetTitle('%s [PE]' % s1Branch)
ha13.GetYaxis().SetTitle('Log10(S2/S1)')
ha13.SetStats(0)



#draw histograms onto canvases

c2 = root.TCanvas('c2','',1280/2,480)
ha2.Draw(option1)
c2.SetLogz()
c2.Update()

c3 = root.TCanvas('c3','XY Dist',1280/2,480)
ha3.Draw(option1)
c3.Update()

c4 = root.TCanvas('c4','',1280/2,480)
ha4.Draw(option1)
c4.Update()

c5 = root.TCanvas('c5','',1280/2,480)
ha5.Draw(option1)
c5.Update()
"""
c6 = root.TCanvas('c6','661.657 - GeEnergy',640,480)
c6.cd()
ha6.Draw()
hb6.Draw(optionSame)
c6.Update()

c7 = root.TCanvas('c7','S1',640,480)
c7.cd()
ha7.Draw()
hb7.Draw(optionSame)
c7.Update()

c8 = root.TCanvas('c8','S2',640,480)
c8.cd()
ha8.Draw()
hb8.Draw(optionSame)
c8.Update()

c9 = root.TCanvas('c9','S1 1-4 keV',640,480)
c9.cd()
ha9.Draw()
hb9.Draw(optionSame)
c9.Update()

c10 = root.TCanvas('c10','S2 1-4 keV',640,480)
c10.cd()
ha10.Draw()
hb10.Draw(optionSame)
c10.Update()

c11 = root.TCanvas('c11','GeHeight[0] vs. Time',1280,480)
c11.Divide(2,1)
c11.cd(1)
ha11.Draw(option1)
c11.cd(2)
hb11.Draw(option1)
c11.Update()
"""

c12 = root.TCanvas('c12','',1280/2,480)
ha12.Draw(option1)
c12.Update()

c13 = root.TCanvas('c13','Neutron Gamma Dist',1280/2,480)
ha13.Draw(option1)
c13.Update()





raw_input("Enter to continue...")
