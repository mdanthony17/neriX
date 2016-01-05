#!/usr/bin/python

import ROOT as root
import sys
import neriX_analysis
from rootpy.plotting import Hist2D, Hist, Legend

#--------------- Start Parameters to Change ----------------

eDepNumBins = 50
eDepMin = -2.
eDepMax = 25.

s1NumBins = 40
s1Min = 0.
s1Max = 40

s2NumBins = 20
s2Min = 0.
s2Max = 150#2.2e5*5

xyNumBins = 100
xyMin = -1.4
xyMax = 1.4

rNumBins = 100
rMin = 0.
rMax = 1.4

dtNumBins = 100/2
dtMin = -25.
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



if(len(sys.argv) != 3):
	print 'Usage is python coin_analysis <ROOT filename> <ROOT filename>'
	sys.exit(1)

file1 = sys.argv[1]
file2 = sys.argv[2]
run1 = neriX_analysis.neriX_analysis(file1)
#run2 = neriX_analysis.neriX_analysis(file2)



#choose options
option1 = 'COLZ'
optionSame = 'SAME'


#choose cuts for run 1
run1.add_dt_cut(2., 13.)
#run1.add_temp_neutron_cut(45)

run1.set_event_list()

sRun1 = ' - run_11'
sRun2 = ' - run_15'


#create histograms for run1
"""
ha1 = root.TH2F('ha1', '661.657 - GeEnergy vs S1', s1NumBins, s1Min, s1Max, eDepNumBins, eDepMin, eDepMax)
run1.get_T1().Draw('eDep:S1sTot[0]>>ha1', run1.get_cuts(), '')

ha1.GetXaxis().SetTitle('S1sTot[0] [PE]')
ha1.GetYaxis().SetTitle('661.657 - GeEnergy [keV]')
ha1.SetStats(0)
"""
ha2 = Hist2D(s1NumBins, s1Min, s1Max, s2NumBins, s2Min, s2Max, name='ha2', title='S1 vs S2')
run1.Draw('S1sTotBottom[0]:S2sTotBottom[0]', hist=ha2)
ha2.GetXaxis().SetTitle('S1sTotBottom[0] [PE]')
ha2.GetYaxis().SetTitle('S2sTotBottom[0] [PE]')
ha2.SetStats(0)

ha3 = Hist2D(xyNumBins, xyMin, xyMax, xyNumBins, xyMin, xyMax, name='ha3', title='XY Dist')
run1.Draw('X:Y', hist=ha3)
ha3.GetXaxis().SetTitle('X [mm]')
ha3.GetYaxis().SetTitle('Y [mm]')
ha3.SetStats(0)

ha4 = Hist2D(rNumBins, rMin**2, rMax**2, dtNumBins, dtMin, dtMax, name='ha4', title='r^2 vs dt' + sRun1)
run1.Draw('R*R:-dt', hist=ha4)
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
run1.get_T1().Draw('eDep>>ha6', run1.get_cuts(), '')
ha6.GetXaxis().SetTitle('661.657 - GeEnergy [keV]')
ha6.GetYaxis().SetTitle('Rate')
ha6.Scale(1./float(run1.get_livetime()))
ha6.SetLineColor(root.kBlue)
ha6.SetStats(0)
"""
ha7 = Hist(s1NumBins, s1Min, s1Max, name='ha7', title='s1', drawstyle='hist')
run1.get_T1().Draw('S1sTotBottom[0]', hist=ha7)
ha7.GetXaxis().SetTitle('S1sTot[0] [PE]')
ha7.GetYaxis().SetTitle('Rate')
ha7.Scale(1./ha7.Integral())
ha7.SetLineColor(root.kBlue)
ha7.SetStats(0)

ha8 = Hist(s2NumBins, s2Min, s2Max, name='ha8', title='s2', drawstyle='hist')
run1.get_T1().Draw('S2sTotBottom[0]', hist=ha8)
ha8.GetXaxis().SetTitle('S2sTot[0] [PE]')
ha8.GetYaxis().SetTitle('Rate')
ha8.Scale(1./ha8.Integral())
ha8.SetLineColor(root.kBlue)
ha8.SetStats(0)
"""
ha11 = root.TH2F('ha11', 'GeHeight vs Time', 100, run1.get_timestamp(0), run1.get_timestamp(run1.get_T1().GetEntries() - 1), 50, 1.3, 2.3)
run1.get_T1().Draw('GeHeight[0]:TimeSec>>ha11', run1.get_cuts(), '')
ha11.GetXaxis().SetTitle('TimeSec [s]')
ha11.GetYaxis().SetTitle('GeHeight[0] [V]')
ha11.SetStats(0)
"""
ha12 = Hist(dtNumBins, dtMin, dtMax, name='ha12', title='dt', drawstyle='hist')
ha12.SetColor(root.kRed)
run1.Draw('-dt', hist=ha12)
ha12.GetXaxis().SetTitle('dt [us]')
ha12.GetYaxis().SetTitle('Normalized Counts')
ha12.Scale(1./ha12.Integral())
ha12.SetStats(0)


#must delete run1 to begin run2!
del run1


#setup run2
run2 = neriX_analysis.neriX_analysis(file2)

#choose cuts for run 2
run2.add_dt_cut(2., 13.)
#run2.add_temp_neutron_cut(45)

run2.set_event_list()

#run2.get_T1().Scan('EventId:S2sTotBottom[0]:S2sSaturation[0]', run2.get_cuts())

#create histograms for run2
"""
hb1 = root.TH2F('hb1', '661.657 - GeEnergy vs S1', s1NumBins, s1Min, s1Max, eDepNumBins, eDepMin, eDepMax)
run2.get_T1().Draw('eDep:S1sTot[0]>>hb1', run2.get_cuts(), '')
hb1.GetXaxis().SetTitle('S1sTot[0] [PE]')
hb1.GetYaxis().SetTitle('661.657 - GeEnergy [keV]')
hb1.SetStats(0)
"""
hb2 = Hist2D(s1NumBins, s1Min, s1Max, s2NumBins, s2Min, s2Max, name='hb2', title='S1 vs S2')
run2.Draw('S1sTotBottom[0]:S2sTotBottom[0]', hist=hb2)
hb2.GetXaxis().SetTitle('S1sTotBottom[0] [PE]')
hb2.GetYaxis().SetTitle('S2sTotBottom[0] [PE]')
hb2.SetStats(0)

hb3 = Hist2D(xyNumBins, xyMin, xyMax, xyNumBins, xyMin, xyMax, name='hb3', title='XY Dist')
run2.Draw('X:Y', hist=hb3)
hb3.GetXaxis().SetTitle('X [mm]')
hb3.GetYaxis().SetTitle('Y [mm]')
hb3.SetStats(0)

hb4 = Hist2D(rNumBins, rMin**2, rMax**2, dtNumBins, dtMin, dtMax, name='hb4', title='r^2 vs dt'+sRun2)
run2.Draw('R*R:-dt', hist=hb4)
hb4.GetXaxis().SetTitle('R^2 [mm^2]')
hb4.GetYaxis().SetTitle('dt [#mus]')
hb4.SetStats(0)

hb5 = Hist2D(s2WidthNumBins, s2WidthMin, s2WidthMax, dtNumBins, dtMin, dtMax, name='hb5', title='S2sLowWidth vs dt')
run2.Draw('S2sLowWidth[0]:-dt', hist=hb5)
hb5.GetXaxis().SetTitle('S2sLowWidth[0] [10 ns]')
hb5.GetYaxis().SetTitle('dt [#mus]')
hb5.SetStats(0)
"""
hb6 = root.TH1F('hb6', '661.657 - GeEnergy', eDepNumBins, eDepMin, eDepMax)
run2.get_T1().Draw('eDep>>hb6', run2.get_cuts(), '')
hb6.GetXaxis().SetTitle('661.657 - GeEnergy [keV]')
hb6.GetYaxis().SetTitle('Rate')
hb6.Scale(1./float(run2.get_livetime()))
hb6.SetLineColor(root.kRed)
hb6.SetStats(0)
"""
hb7 = Hist(s1NumBins, s1Min, s1Max, name='hb7', title='s1', drawstyle='hist')
run2.get_T1().Draw('S1sTotBottom[0]', hist=hb7)
hb7.GetXaxis().SetTitle('S1sTot[0] [PE]')
hb7.GetYaxis().SetTitle('Rate')
hb7.Scale(1./hb7.Integral())
hb7.SetLineColor(root.kRed)
hb7.SetStats(0)

hb8 = Hist(s2NumBins, s2Min, s2Max, name='hb8', title='s2', drawstyle='hist')
run2.get_T1().Draw('S2sTotBottom[0]', hist=hb8)
hb8.GetXaxis().SetTitle('S2sTot[0] [PE]')
hb8.GetYaxis().SetTitle('Rate')
hb8.Scale(1./hb8.Integral())
hb8.SetLineColor(root.kRed)
hb8.SetStats(0)
"""
hb11 = root.TH2F('hb11', 'GeHeight vs Time', 100, run2.get_timestamp(0), run2.get_timestamp(run2.get_T1().GetEntries() - 1), 50, 1.3, 2.3)
run2.get_T1().Draw('GeHeight[0]:TimeSec>>ha11', run2.get_cuts(), '')
hb11.GetXaxis().SetTitle('TimeSec [s]')
hb11.GetYaxis().SetTitle('GeHeight[0] [V]')
hb11.SetStats(0)
"""
hb12 = Hist(dtNumBins, dtMin, dtMax, name='hb12', title='dt', drawstyle='hist')
hb12.SetColor(root.kBlue)
run2.Draw('-dt', hist=hb12)
hb12.Scale(1./hb12.Integral())
hb12.GetXaxis().SetTitle('dt [us]')
hb12.SetStats(0)

del run2

"""
#create run1 for 1-4 keV slice (new cut)
run1s = neriX_analysis.neriX_analysis(file1)

#rechoose cuts for 1-4 keV slice
run1s.add_dt_cut(2., 13.)
run1s.add_radius_cut(0., 20.)
run1s.add_eDep_cut(1, 4)
run1s.add_coincidence_cut()

ha9 = root.TH1F('ha9', 's1', 40, 0, 40)
run1s.get_T1().Draw('S1sTot[0]>>ha9', run1s.get_cuts(), '')
ha9.GetXaxis().SetTitle('S1sTot[0] [PE]')
ha9.GetYaxis().SetTitle('Rate')
#ha9.Scale(1./float(run1s.get_livetime()))
ha9.SetLineColor(root.kBlue)
ha9.SetStats(1)

ha10 = root.TH1F('ha10', 's2', 40, 0, 8000)
run1s.get_T1().Draw('S2sTot[0]>>ha10', run1s.get_cuts(), '')
ha10.GetXaxis().SetTitle('S2sTot[0] [PE]')
ha10.GetYaxis().SetTitle('Rate')
#ha10.Scale(1./float(run1s.get_livetime()))
ha10.SetLineColor(root.kBlue)
ha10.SetStats(1)

del run1s



#create run2 for 1-4 keV slice (new cut)
run2s = neriX_analysis.neriX_analysis(file2)

#rechoose cuts for 1-4 keV slice
run2s.add_dt_cut(2., 13.)
run2s.add_radius_cut(0., 20.)
run2s.add_eDep_cut(1, 4)
run2s.add_coincidence_cut()

hb9 = root.TH1F('hb9', 's1', 40, 0, 40)
run2s.get_T1().Draw('S1sTot[0]>>hb9', run2s.get_cuts(), '')
hb9.GetXaxis().SetTitle('S1sTot[0] [PE]')
hb9.GetYaxis().SetTitle('Rate')
#hb9.Scale(1./float(run2s.get_livetime()))
hb9.SetLineColor(root.kRed)
hb9.SetStats(0)

hb10 = root.TH1F('hb10', 's2', 40, 0, 8000)
run2s.get_T1().Draw('S2sTot[0]>>hb10', run2s.get_cuts(), '')
hb10.GetXaxis().SetTitle('S2sTot[0] [PE]')
hb10.GetYaxis().SetTitle('Rate')
#hb10.Scale(1./float(run2s.get_livetime()))
hb10.SetLineColor(root.kRed)
hb10.SetStats(0)

del run2s


"""


#draw histograms onto canvases
"""
c1 = root.TCanvas('c1','661.657 - GeEnergy vs S1',1280,480)
c1.Divide(2,1)
c1.cd(1)
ha1.Draw(option1)
c1.cd(2)
hb1.Draw(option1)
c1.Update()
"""
c2 = root.TCanvas('c2','',1280,480)
c2.Divide(2,1)
c2.cd(1)
ha2.Draw(option1)
c2.cd(2)
hb2.Draw(option1)
c2.Update()

c3 = root.TCanvas('c3','XY Dist',1280,480)
c3.Divide(2,1)
c3.cd(1)
ha3.Draw(option1)
c3.cd(2)
hb3.Draw(option1)
c3.Update()

c4 = root.TCanvas('c4','',1280,480)
c4.Divide(2,1)
c4.cd(1)
ha4.Draw(option1)
c4.cd(2)
hb4.Draw(option1)
c4.Update()

c5 = root.TCanvas('c5','',1280,480)
c5.Divide(2,1)
c5.cd(1)
ha5.Draw(option1)
c5.cd(2)
hb5.Draw(option1)
c5.Update()
"""
c6 = root.TCanvas('c6','661.657 - GeEnergy',640,480)
c6.cd()
ha6.Draw()
hb6.Draw(optionSame)
c6.Update()
"""
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
"""
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

c12 = root.TCanvas('c11','',1280,480)
ha12.Draw()
hb12.Draw('same')

legend = Legend(2)
legend.AddEntry(ha12, label='run_11')
legend.AddEntry(hb12, label='run_14')
legend.Draw('same')

c12.Update()



raw_input("Enter to continue...")
