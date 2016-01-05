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
tofMin = -50
tofMax = 200

psdNumBins = 50
psdMin = 0
psdMax = 1

lhNumBins = 50
lhMin = 0
lhMax = 2.5


#--------------- End Parameters to Change ----------------



if(len(sys.argv) != 5):
	print 'Usage is python coincidence_quick_check.py <run> <anode> <cathode> <degree>'
	sys.exit(1)

run = int(sys.argv[1])
anodeSetting = float(sys.argv[2])
cathodeSetting = float(sys.argv[3])
degreeSetting = int(sys.argv[4])


current_analysis = neriX_analysis.neriX_analysis(neriX_analysis.pull_all_files_given_parameters(run, anodeSetting, cathodeSetting, degreeSetting), degreeSetting, cathodeSetting, anodeSetting)

sForHistograms = ' - %d deg, %.3f kV' % (degreeSetting, cathodeSetting)

colzOption = 'COLZ'

#choose cuts for run 1
current_analysis.add_cut('S1sTotBottom[0] > 0')
#current_analysis.add_cut('S2sTotBottom[0] > 0')
current_analysis.add_z_cut()
current_analysis.add_radius_cut(0, 0.85)
current_analysis.add_single_scatter_cut()
current_analysis.add_s1_liqsci_peak_cut()
current_analysis.add_xs1asym_cut()
current_analysis.add_xs2asym_cut()
current_analysis.add_temp_neutron_cut(degreeSetting)
current_analysis.add_temp_tof_cut(degreeSetting)
#current_analysis.add_cut('TimeOfFlight > -50 && TimeOfFlight < -10') # accidental spec

#current_analysis.add_cut('S1sPeak[0] > 395 && S1sPeak[0] < 425')
#current_analysis.add_cut('S1sPeak[0] > 3380 && S1sPeak[0] < 3430')

current_analysis.set_event_list()

s1Branch = 'cpS1sTotBottom[0]'
s2Branch = 'cpS2sTotBottom[0]'

#create histograms for current_analysis

#sS1Branch = 'S1sTotBottom[0]'# - S1sNoiseMedian[0][16]'
#sS1Branch = 's1_tot_bot_minus_noise_t'
print '\nUsing S1 Branch: %s \n\n' % s1Branch
print '\nUsing S2 Branch: %s \n\n' % s2Branch


print '\n\nTotal number of events: %s\n\n' % current_analysis.get_num_events()

#current_analysis.get_lT1()[0].Scan('EventId:S1sPeak[0]:%s:S2sPeak[0]:S2sTotBottom[0]:TimeOfFlight' % sS1Branch, '%s > 6' % sS1Branch)

hS1S2 = Hist2D(s1NumBins, s1Min, s1Max, s2NumBins, s2Min, s2Max, name='hS1S2', title='S1 vs S2'+sForHistograms)
current_analysis.Draw('%s:%s' % (s1Branch, s2Branch), hist=hS1S2)
hS1S2.GetXaxis().SetTitle('%s [PE]' % s1Branch)
hS1S2.GetYaxis().SetTitle('%s [PE]' % s2Branch)
hS1S2.SetStats(0)

hXY = Hist2D(xyNumBins, xyMin, xyMax, xyNumBins, xyMin, xyMax, name='hXY', title='XY Dist'+sForHistograms)
current_analysis.Draw('X:Y', hist=hXY)
hXY.GetXaxis().SetTitle('X [mm]')
hXY.GetYaxis().SetTitle('Y [mm]')
hXY.SetStats(0)

hUniformity = Hist2D(rNumBins, rMin**2, rMax**2, dtNumBins, dtMin, dtMax, name='hUniformity', title='Uniformity'+sForHistograms)
current_analysis.Draw('R*R:-dt', hist=hUniformity)
hUniformity.GetXaxis().SetTitle('R^2 [mm^2]')
hUniformity.GetYaxis().SetTitle('dt [#mus]')
hUniformity.SetStats(0)

hWidth = Hist2D(s2WidthNumBins, s2WidthMin, s2WidthMax, dtNumBins, dtMin, dtMax, name='hWidth', title='S2sLowWidth vs dt'+sForHistograms)
current_analysis.Draw('S2sLowWidth[0]:-dt', hist=hWidth)
hWidth.SetTitle('S2sLowWidth[0] [10 ns]')
hWidth.GetYaxis().SetTitle('dt [#mus]')
hWidth.SetStats(0)

hS1 = Hist(s1NumBins, s1Min, s1Max, name='hS1', title='S1 Spectrum'+sForHistograms)
hS1.SetMarkerSize(0)
current_analysis.Draw(s1Branch, hist=hS1)
hS1.GetXaxis().SetTitle('%s [PE]' % s1Branch)
hS1.GetYaxis().SetTitle('Counts')
#hS1.Scale(1./float(current_analysis.get_livetime()))
hS1.SetStats(0)

hS2 = Hist(s2NumBins, s2Min, s2Max, name='hS2', title='S2 Spectrum'+sForHistograms)
hS2.SetMarkerSize(0)
current_analysis.Draw(s2Branch, hist=hS2)
hS2.GetXaxis().SetTitle('%s [PE]' % s2Branch)
hS2.GetYaxis().SetTitle('Counts')
#hS2.Scale(1./float(current_analysis.get_livetime()))
hS2.SetStats(0)

hTOF = Hist(tofNumBins, tofMin, tofMax, name='hTOF', title='TOF Spectrum'+sForHistograms, drawstyle='hist')
current_analysis.Draw('TimeOfFlight', hist=hTOF)
hTOF.GetXaxis().SetTitle('TOF [ns]')
hTOF.GetYaxis().SetTitle('Counts')
hTOF.SetStats(1)

hPSD = Hist2D(lhNumBins, lhMin, lhMax, psdNumBins, psdMin, psdMax, name='hPSD', title='PSD vs Pulse Height'+sForHistograms)
current_analysis.Draw('LiqSciHeight[]:LiqSciTailRaw[]/LiqSciRaw[]', hist=hPSD)
hPSD.GetXaxis().SetTitle('LiqSciHeight [V]')
hPSD.GetYaxis().SetTitle('PSD Parameter')
hPSD.SetMarkerSize(0)
hPSD.SetStats(0)



#draw histograms onto canvases

c1 = root.TCanvas('c1','',1280/2,480)
hS1S2.Draw(colzOption)
#c1.SetLogz()
c1.Update()

c2 = root.TCanvas('c2','XY Dist',1280/2,480)
hXY.Draw(colzOption)
c2.Update()

c3 = root.TCanvas('c3','',1280/2,480)
hUniformity.Draw(colzOption)
c3.Update()

c4 = root.TCanvas('c4','',1280/2,480)
hWidth.Draw(colzOption)
c4.Update()

c5 = root.TCanvas('c5','',1280/2,480)
hS1.Draw('e')
c5.Update()

c6 = root.TCanvas('c6','',1280/2,480)
hS2.Draw('e')
c6.Update()

c7 = root.TCanvas('c7','',1280/2,480)
hTOF.Draw(colzOption)
c7.Update()

c8 = root.TCanvas('c8','',1280/2,480)
hPSD.Draw()
c8.Update()

raw_input('Enter to continue...')
response = raw_input('Would you like to save the S1 and S2 spectra for later analysis?  Press "y" to proceed, otherwise press anything: ')

if response == 'y':
	outputFile = root_open('./full_angle_files/results_%ddeg.root' % (degreeSetting), 'recreate')
	
	hS1S2.SetName('hS1S2')
	hS1S2.Write()
	
	hS1.SetName('hS1')
	hS1.Write()
	
	hS2.SetName('hS2')
	hS2.Write()

	sCuts = root.TObjString('cuts_used')
	sCuts.Write(current_analysis.get_cuts(), root.TObject.kOverwrite)
