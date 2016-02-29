#!/usr/bin/python

import ROOT as root
import sys
import neriX_analysis
from rootpy.plotting import Hist2D, Hist, Legend
from rootpy.io import root_open, File

#--------------- Start Parameters to Change ----------------

s1NumBins = 20
s1Min = -0.5
s1Max = 39.5

s2NumBins = 10
s2Min = 0.
s2Max = 4000

xyNumBins = 100
xyMin = -1.4
xyMax = 1.4

rNumBins = 100
rMin = 0.
rMax = 1.4

dtNumBins = 100/2
dtMin = -20.
dtMax = 0.

s2WidthNumBins = 200
s2WidthMin = 40.
s2WidthMax = 160.

tofNumBins = 100
tofMin = -80
tofMax = 120

lhNumBins = 50
lhMin = 0
lhMax = 2.5

liqsciS1TimeDiffBins = 200
liqsciS1TimeDiffMin = -100
liqsciS1TimeDiffMax = 100


#--------------- End Parameters to Change ----------------



# normal trigger list

lNormalTrigger = ['nerix_160218_1158',
				  'nerix_160218_1525',
				  'nerix_160219_1529',
				  'nerix_160219_1715',
				  'nerix_160221_1049',
				  'nerix_160224_1542',
				  'nerix_160226_1014'
				 ]



# "special" trigger list

lSpecialTrigger = ['nerix_160223_1039',
				   'nerix_160223_1043',
				   'nerix_160223_1246',
				   'nerix_160223_1430',
				   'nerix_160224_1039'
				  ]



if(len(sys.argv) != 5):
	print 'Usage is python compare_trigger_setups.py <run> <anode> <cathode> <degree>'
	sys.exit(1)

run = int(sys.argv[1])
anodeSetting = float(sys.argv[2])
cathodeSetting = float(sys.argv[3])
degreeSetting = int(sys.argv[4])



# -----------------------------
# Set branches
# -----------------------------



s1Branch = 'cpS1sTotBottom[0] - S1sNoiseTrapezoid[0][16]'
#s1Branch = 'cpS1sTotBottom[0]'
s2Branch = 'cpS2sTotBottom[0]'

#create histograms for current_analysis_normal

print '\nUsing S1 Branch: %s \n\n' % s1Branch
print '\nUsing S2 Branch: %s \n\n' % s2Branch

colzOption = 'COLZ'
sForHistograms = ' - %d deg, %.3f kV' % (degreeSetting, cathodeSetting)


# -----------------------------
# -----------------------------
# -----------------------------
# Load normal trigger and run
# analysis
# -----------------------------
# -----------------------------
# -----------------------------

current_analysis_normal = neriX_analysis.neriX_analysis(lNormalTrigger, degreeSetting, cathodeSetting, anodeSetting)




#choose cuts for run 1
current_analysis_normal.add_cut('S1sTotBottom[0] > 0')
#current_analysis_normal.add_cut('S2sTotBottom[0] > 0')
current_analysis_normal.add_z_cut()
current_analysis_normal.add_radius_cut(0, 0.85)
current_analysis_normal.add_single_scatter_cut()
current_analysis_normal.add_xs1asym_cut()
current_analysis_normal.add_xs2asym_cut()
current_analysis_normal.add_temp_neutron_cut(degreeSetting)
#current_analysis_normal.add_temp_gamma_cut(degreeSetting)
#current_analysis_normal.add_cut('TMath::Log10(S2sTotBottom[0]/S1sTotBottom[0]) < 3 && S1sTotBottom[0] < 35')

current_analysis_normal.set_event_list()





hLiqSciS1TimeDiff_a = Hist(liqsciS1TimeDiffBins, liqsciS1TimeDiffMin, liqsciS1TimeDiffMax, name='hLiqSciS1TimeDiff_a', title='Time Between Liqsci and S1 Spectrum'+sForHistograms, drawstyle='hist')
current_analysis_normal.Draw('LiqSciPeak[] - S1sPeak[0]', hist=hLiqSciS1TimeDiff_a, selection=current_analysis_normal.get_cuts())
hLiqSciS1TimeDiff_a.GetXaxis().SetTitle('Time Between Liqsci and S1 Spectrum [samples]')
hLiqSciS1TimeDiff_a.GetYaxis().SetTitle('Rate [1/s]')
hLiqSciS1TimeDiff_a.SetStats(0)



current_analysis_normal.add_s1_liqsci_peak_cut()

hTOF_a = Hist(tofNumBins, tofMin, tofMax, name='hTOF_a', title='TOF Spectrum'+sForHistograms, drawstyle='hist')
current_analysis_normal.Draw('TimeOfFlight', hist=hTOF_a, selection=current_analysis_normal.get_cuts())
hTOF_a.GetXaxis().SetTitle('TOF [ns]')
hTOF_a.GetYaxis().SetTitle('Rate [1/s]')
hTOF_a.SetStats(0)

current_analysis_normal.add_temp_tof_cut(degreeSetting)




#current_analysis_normal.get_lT1()[0].Scan('EventId:S1sPeak[0]:%s:S2sPeak[0]:S2sTotBottom[0]:TimeOfFlight' % sS1Branch, '%s > 6' % sS1Branch)

hS1S2_a = Hist2D(s1NumBins, s1Min, s1Max, s2NumBins, s2Min, s2Max, name='hS1S2_a', title='TAC in Trigger: S1 vs S2'+sForHistograms)
current_analysis_normal.Draw('%s:%s' % (s1Branch, s2Branch), hist=hS1S2_a, selection=current_analysis_normal.get_cuts())
hS1S2_a.GetXaxis().SetTitle('%s [PE]' % s1Branch)
hS1S2_a.GetYaxis().SetTitle('%s [PE]' % s2Branch)
hS1S2_a.SetStats(0)

hUniformity_a = Hist2D(rNumBins, rMin**2, rMax**2, dtNumBins, dtMin, dtMax, name='hUniformity_a', title='Uniformity'+sForHistograms)
current_analysis_normal.Draw('R*R:-dt', hist=hUniformity_a, selection=current_analysis_normal.get_cuts())
hUniformity_a.GetXaxis().SetTitle('R^2 [mm^2]')
hUniformity_a.GetYaxis().SetTitle('dt [#mus]')
hUniformity_a.SetStats(0)

hS1_a = Hist(s1NumBins, s1Min, s1Max, name='hS1_a', title='S1 Spectrum'+sForHistograms)
hS1_a.SetMarkerSize(0)
current_analysis_normal.Draw(s1Branch, hist=hS1_a, selection=current_analysis_normal.get_cuts())
hS1_a.GetXaxis().SetTitle('%s [PE]' % s1Branch)
hS1_a.GetYaxis().SetTitle('Rate [1/s]')
#hS1_a.Scale(1./float(current_analysis_normal.get_livetime()))
hS1_a.SetStats(0)

hS2_a = Hist(s2NumBins, s2Min, s2Max, name='hS2_a', title='S2 Spectrum'+sForHistograms)
hS2_a.SetMarkerSize(0)
current_analysis_normal.Draw(s2Branch, hist=hS2_a, selection=current_analysis_normal.get_cuts())
hS2_a.GetXaxis().SetTitle('%s [PE]' % s2Branch)
hS2_a.GetYaxis().SetTitle('Rate [1/s]')
#hS2_a.Scale(1./float(current_analysis_normal.get_livetime()))
hS2_a.SetStats(0)

neriX_analysis.success_message('Sumamry for normal trigger:')

print '\n\nTotal number of events: %s' % hS2_a.Integral()
print 'Total time: %.0f \n\n' % current_analysis_normal.get_livetime()
print 'Rate: %.2f events per hour \n\n' % (float(hS2_a.Integral()) / current_analysis_normal.get_livetime() * 3600.)

livetime_normal = current_analysis_normal.get_livetime()

del current_analysis_normal


# -----------------------------
# -----------------------------
# -----------------------------
# Load special trigger and run
# analysis
# -----------------------------
# -----------------------------
# -----------------------------


current_analysis_special = neriX_analysis.neriX_analysis(lSpecialTrigger, degreeSetting, cathodeSetting, anodeSetting)




#choose cuts for run 1
current_analysis_special.add_cut('S1sTotBottom[0] > 0')
#current_analysis_special.add_cut('S2sTotBottom[0] > 0')
current_analysis_special.add_z_cut()
current_analysis_special.add_radius_cut(0, 0.85)
current_analysis_special.add_single_scatter_cut()
current_analysis_special.add_xs1asym_cut()
current_analysis_special.add_xs2asym_cut()
current_analysis_special.add_temp_neutron_cut(degreeSetting)
#current_analysis_special.add_temp_gamma_cut(degreeSetting)
#current_analysis_special.add_cut('TMath::Log10(S2sTotBottom[0]/S1sTotBottom[0]) < 3 && S1sTotBottom[0] < 35')

current_analysis_special.set_event_list()





hLiqSciS1TimeDiff_b = Hist(liqsciS1TimeDiffBins, liqsciS1TimeDiffMin, liqsciS1TimeDiffMax, name='hLiqSciS1TimeDiff_b', title='Time Between Liqsci and S1 Spectrum'+sForHistograms, drawstyle='hist')
current_analysis_special.Draw('LiqSciPeak[] - S1sPeak[0]', hist=hLiqSciS1TimeDiff_b, selection=current_analysis_special.get_cuts())
hLiqSciS1TimeDiff_b.GetXaxis().SetTitle('Time Between Liqsci and S1 Spectrum [samples]')
hLiqSciS1TimeDiff_b.GetYaxis().SetTitle('Rate [1/s]')
hLiqSciS1TimeDiff_b.SetStats(0)



current_analysis_special.add_s1_liqsci_peak_cut()

hTOF_b = Hist(tofNumBins, tofMin, tofMax, name='hTOF_b', title='TOF Spectrum'+sForHistograms, drawstyle='hist')
current_analysis_special.Draw('TimeOfFlight', hist=hTOF_b, selection=current_analysis_special.get_cuts())
hTOF_b.GetXaxis().SetTitle('TOF [ns]')
hTOF_b.GetYaxis().SetTitle('Rate [1/s]')
hTOF_b.SetStats(0)

# keep below to check with same efficiency applied
#current_analysis_special.add_temp_tof_cut(degreeSetting)




#current_analysis_special.get_lT1()[0].Scan('EventId:S1sPeak[0]:%s:S2sPeak[0]:S2sTotBottom[0]:TimeOfFlight' % sS1Branch, '%s > 6' % sS1Branch)

hS1S2_b = Hist2D(s1NumBins, s1Min, s1Max, s2NumBins, s2Min, s2Max, name='hS1S2_b', title='No TAC in Trigger: S1 vs S2'+sForHistograms)
current_analysis_special.Draw('%s:%s' % (s1Branch, s2Branch), hist=hS1S2_b, selection=current_analysis_special.get_cuts())
hS1S2_b.GetXaxis().SetTitle('%s [PE]' % s1Branch)
hS1S2_b.GetYaxis().SetTitle('%s [PE]' % s2Branch)
hS1S2_b.SetStats(0)

hUniformity_b = Hist2D(rNumBins, rMin**2, rMax**2, dtNumBins, dtMin, dtMax, name='hUniformity_b', title='Uniformity'+sForHistograms)
current_analysis_special.Draw('R*R:-dt', hist=hUniformity_b, selection=current_analysis_special.get_cuts())
hUniformity_b.GetXaxis().SetTitle('R^2 [mm^2]')
hUniformity_b.GetYaxis().SetTitle('dt [#mus]')
hUniformity_b.SetStats(0)

hS1_b = Hist(s1NumBins, s1Min, s1Max, name='hS1_b', title='S1 Spectrum'+sForHistograms)
hS1_b.SetMarkerSize(0)
current_analysis_special.Draw(s1Branch, hist=hS1_b, selection=current_analysis_special.get_cuts())
hS1_b.GetXaxis().SetTitle('%s [PE]' % s1Branch)
hS1_b.GetYaxis().SetTitle('Rate [1/s]')
#hS1_b.Scale(1./float(current_analysis_special.get_livetime()))
hS1_b.SetStats(0)

hS2_b = Hist(s2NumBins, s2Min, s2Max, name='hS2_b', title='S2 Spectrum'+sForHistograms)
hS2_b.SetMarkerSize(0)
current_analysis_special.Draw(s2Branch, hist=hS2_b, selection=current_analysis_special.get_cuts())
hS2_b.GetXaxis().SetTitle('%s [PE]' % s2Branch)
hS2_b.GetYaxis().SetTitle('Rate [1/s]')
#hS2_b.Scale(1./float(current_analysis_special.get_livetime()))
hS2_b.SetStats(0)

neriX_analysis.success_message('Sumamry for normal trigger:')

livetime_special = current_analysis_special.get_livetime()

print '\n\nTotal number of events: %s' % hS2_b.Integral()
print 'Total time: %.0f \n\n' % current_analysis_special.get_livetime()
print 'Rate: %.2f events per hour \n\n' % (float(hS2_b.Integral()) / livetime_special * 3600.)







#draw histograms onto canvases

c1 = root.TCanvas('c1','',1280,480)
c1.Divide(2)
c1.cd(1)
hS1S2_a.Scale(1./livetime_normal)
hS1S2_a.Draw(colzOption)
c1.cd(2)
hS1S2_b.Scale(1./livetime_special)
hS1S2_b.Draw(colzOption)
c1.Update()

c3 = root.TCanvas('c3','',1280,480)
c3.Divide(2)
c3.cd(1)
hUniformity_a.Draw(colzOption)
c3.cd(2)
hUniformity_b.Draw(colzOption)
c3.Update()

c5 = root.TCanvas('c5','',1280/2,480)
hS1_a.SetColor(root.kRed)
hS1_b.SetColor(root.kBlue)
hS1_a.Scale(1./livetime_normal)
hS1_b.Scale(1./livetime_special)
hS1_a.GetYaxis().SetRangeUser(0, max(hS1_a.GetMaximum(), hS1_b.GetMaximum())*1.2)
hS1_a.Draw()
hS1_b.Draw('same')
leg5 = Legend(2)
leg5.AddEntry(hS1_a, label='TAC in trigger')
leg5.AddEntry(hS1_b, label='TAC not in trigger')
leg5.Draw('same')
c5.Update()

c6 = root.TCanvas('c6','',1280/2,480)
hS2_a.Scale(1./livetime_normal)
hS2_b.Scale(1./livetime_special)
hS2_a.GetYaxis().SetRangeUser(0, max(hS2_a.GetMaximum(), hS2_b.GetMaximum())*1.2)
hS2_a.SetColor(root.kRed)
hS2_b.SetColor(root.kBlue)
hS2_a.Draw()
hS2_b.Draw('same')
leg6 = Legend(2)
leg6.AddEntry(hS2_a, label='TAC in trigger')
leg6.AddEntry(hS2_b, label='TAC not in trigger')
leg6.Draw('same')
c6.Update()

c7 = root.TCanvas('c7','',1280/2,480)
hTOF_a.Scale(1./livetime_normal)
hTOF_b.Scale(1./livetime_special)
hTOF_a.GetYaxis().SetRangeUser(0, max(hTOF_a.GetMaximum(), hTOF_b.GetMaximum())*1.2)
hTOF_a.SetColor(root.kRed)
hTOF_b.SetColor(root.kBlue)
hTOF_a.Draw()
hTOF_b.Draw('same')
leg7 = Legend(2)
leg7.AddEntry(hTOF_a, label='TAC in trigger')
leg7.AddEntry(hTOF_b, label='TAC not in trigger')
leg7.Draw('same')
c7.Update()

c9 = root.TCanvas('c9','',1280/2,480)
hLiqSciS1TimeDiff_a.Scale(1./livetime_normal)
hLiqSciS1TimeDiff_b.Scale(1./livetime_special)
hLiqSciS1TimeDiff_a.GetYaxis().SetRangeUser(0, max(hLiqSciS1TimeDiff_a.GetMaximum(), hLiqSciS1TimeDiff_b.GetMaximum())*1.2)
hLiqSciS1TimeDiff_a.SetColor(root.kRed)
hLiqSciS1TimeDiff_b.SetColor(root.kBlue)
hLiqSciS1TimeDiff_a.Draw()
hLiqSciS1TimeDiff_b.Draw('same')
leg9 = Legend(2)
leg9.AddEntry(hLiqSciS1TimeDiff_a, label='TAC in trigger')
leg9.AddEntry(hLiqSciS1TimeDiff_b, label='TAC not in trigger')
leg9.Draw('same')
c9.Update()


neriX_analysis.save_plot(['trigger_comparison', '%.3fkV_%ddeg' % (cathodeSetting, degreeSetting)], c1, 's1s2_%.3fkV_%ddeg' % (cathodeSetting, degreeSetting))
neriX_analysis.save_plot(['trigger_comparison', '%.3fkV_%ddeg' % (cathodeSetting, degreeSetting)], c5, 's1_%.3fkV_%ddeg' % (cathodeSetting, degreeSetting))
neriX_analysis.save_plot(['trigger_comparison', '%.3fkV_%ddeg' % (cathodeSetting, degreeSetting)], c6, 's2_%.3fkV_%ddeg' % (cathodeSetting, degreeSetting))
neriX_analysis.save_plot(['trigger_comparison', '%.3fkV_%ddeg' % (cathodeSetting, degreeSetting)], c7, 'tof_%.3fkV_%ddeg' % (cathodeSetting, degreeSetting))
neriX_analysis.save_plot(['trigger_comparison', '%.3fkV_%ddeg' % (cathodeSetting, degreeSetting)], c9, 'liqsci_s1_time_diff_%.3fkV_%ddeg' % (cathodeSetting, degreeSetting))


