#!/usr/bin/python

import ROOT as root
import sys
import neriX_analysis
from rootpy.plotting import Hist2D, Hist, Legend
from rootpy.io import root_open, File

#--------------- Start Parameters to Change ----------------

s1NumBins = 40
s1Min = -0.5
s1Max = 39.5

s2NumBins = 40
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

psdNumBins = 50
psdMin = 0
psdMax = 1

lhNumBins = 50
lhMin = 0
lhMax = 2.5

liqsciS1TimeDiffBins = 200
liqsciS1TimeDiffMin = -100
liqsciS1TimeDiffMax = 100


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
#current_analysis.add_z_cut()
current_analysis.add_radius_cut(0, 0.85)
current_analysis.add_single_scatter_cut()
#current_analysis.add_xs1asym_cut()
current_analysis.add_xs2asym_cut()
current_analysis.add_temp_neutron_cut(degreeSetting)



#s1Branch = 'cpS1sTotBottom[0] - S1sNoiseTrapezoid[0][16]'
#s1Branch = 'cpS1sTotBottom[]'
#s1Branch = 'S1sTotBottom[]'
s1Branch = 'S1IntegralBeforeLiqSci[0]'
s2Branch = 'cpS2sTotBottom[0]'

current_analysis.add_cut('%s > 0' % s1Branch)
current_analysis.add_cut('%s > 0' % s2Branch)
current_analysis.add_cut('%s < %f' % (s1Branch, s1Max))
current_analysis.add_cut('%s < %f' % (s2Branch, s2Max))
current_analysis.add_cut('log10((S1Tot + S2Tot)/(AreaTot - S1Tot - S2Tot)) > 0.')

#current_analysis.add_cut('(S2sPeak[0] - LiqSciPeak[])/100. > 3 && (S2sPeak[0] - LiqSciPeak[])/100. < 20')
#current_analysis.add_cut('(S2sPeak[0] - LiqSciPeak[])/100. > 3 && (S2sPeak[0] - LiqSciPeak[])/100. < 20 && LiqSciPeak[] > 0')
current_analysis.add_cut('S2sPeak[0] > 600 && S2sPeak[0] < 2000') # hacky cut since we know LiqSciPeak is around 400 samples
current_analysis.add_cut('((%s > 24.0) || (%s < (7.406e+02 + 6.240e+01*%s + -4.430e-01*pow(%s, 2.))))' % (s1Branch, s2Branch, s1Branch, s1Branch))
#current_analysis.add_cut('LiqSciPeak[] != -1')

#current_analysis.set_event_list()
current_analysis.multithread_set_event_list(7)



#create histograms for current_analysis

print '\nUsing S1 Branch: %s \n\n' % s1Branch
print '\nUsing S2 Branch: %s \n\n' % s2Branch


print '\nCreating LiqSci S1 Time Difference Histogram.\n'
hLiqSciS1TimeDiff = Hist(liqsciS1TimeDiffBins, liqsciS1TimeDiffMin, liqsciS1TimeDiffMax, name='hLiqSciS1TimeDiff', title='Time Between Liqsci and S1 Spectrum'+sForHistograms, drawstyle='hist')
current_analysis.Draw('LiqSciPeak[] - S1sPeak[0]', hist=hLiqSciS1TimeDiff, selection=current_analysis.get_cuts())
hLiqSciS1TimeDiff.GetXaxis().SetTitle('Time Between Liqsci and S1 Spectrum [samples]')
hLiqSciS1TimeDiff.GetYaxis().SetTitle('Counts')
hLiqSciS1TimeDiff.SetStats(1)


if s1Branch != 'S1IntegralBeforeLiqSci[0]':
	#print '\n\nNo liqscipeak cut!\n\n'
	current_analysis.add_s1_liqsci_peak_cut()
	print '\n\nAdded liqscipeak cut.\n\n'
else:
	current_analysis.add_cut('S1IntegralBeforeLiqSciHeight[0] > 0.001') # from looking at height vs area (0.001 cleans up 2.356kV)

print '\nCreating TOF Histogram.\n'
hTOF = Hist(tofNumBins, tofMin, tofMax, name='hTOF', title='TOF Spectrum'+sForHistograms, drawstyle='hist')
current_analysis.Draw('TimeOfFlight', hist=hTOF, selection=current_analysis.get_cuts())
hTOF.GetXaxis().SetTitle('TOF [ns]')
hTOF.GetYaxis().SetTitle('Counts')
hTOF.SetStats(1)

print '\nNo TOF cut!\n'
#current_analysis.add_temp_tof_cut(degreeSetting)
#print 'TOF cut!'

#current_analysis.add_cut('%s < 10 && %s < 900' % (s1Branch, s2Branch))
#current_analysis.add_cut('%s < 10 && %s < 900' % (s1Branch, s2Branch))
#current_analysis.add_cut('(%s > %f && %s < %f) && (%s > %f && %s < %f)' % (s1Branch, 6, s1Branch, 9, s2Branch, 800, s2Branch, 900))
#current_analysis.get_lT1()[0].Scan('EventId:S1sTotBottom[]:S1IntegralBeforeLiqSci[0]:S1IntegralBeforeLiqSciHeight[0]:S2sTotBottom[0]:S1sPeak[]:S1IntegralBeforeLiqSciPeak[0]:S2sPeak[0]', current_analysis.get_cuts())


print '\nCreating S1 vs S2 Histogram.\n'
hS1S2 = Hist2D(s1NumBins, s1Min, s1Max, s2NumBins, s2Min, s2Max, name='hS1S2', title='S1 vs S2'+sForHistograms)
current_analysis.Draw('%s:%s' % (s1Branch, s2Branch), hist=hS1S2, selection=current_analysis.get_cuts())
hS1S2.GetXaxis().SetTitle('%s [PE]' % s1Branch)
hS1S2.GetYaxis().SetTitle('%s [PE]' % s2Branch)
hS1S2.SetStats(0)

print '\nCreating Reconstructed Position Histogram.\n'
hXY = Hist2D(xyNumBins, xyMin, xyMax, xyNumBins, xyMin, xyMax, name='hXY', title='XY Dist'+sForHistograms)
current_analysis.Draw('X:Y', hist=hXY, selection=current_analysis.get_cuts())
hXY.GetXaxis().SetTitle('X [mm]')
hXY.GetYaxis().SetTitle('Y [mm]')
hXY.SetStats(0)

print '\nCreating Uniformity Histogram.\n'
hUniformity = Hist2D(rNumBins, rMin**2, rMax**2, dtNumBins, dtMin, dtMax, name='hUniformity', title='Uniformity'+sForHistograms)
current_analysis.Draw('R*R:-dt', hist=hUniformity, selection=current_analysis.get_cuts())
hUniformity.GetXaxis().SetTitle('R^2 [mm^2]')
hUniformity.GetYaxis().SetTitle('dt [#mus]')
hUniformity.SetStats(0)

print '\nCreating Width vs Time Histogram.\n'
hWidth = Hist2D(s2WidthNumBins, s2WidthMin, s2WidthMax, dtNumBins, dtMin, dtMax, name='hWidth', title='S2sLowWidth vs dt'+sForHistograms)
current_analysis.Draw('S2sLowWidth[0]:-dt', hist=hWidth, selection=current_analysis.get_cuts())
hWidth.SetTitle('S2sLowWidth[0] [10 ns]')
hWidth.GetYaxis().SetTitle('dt [#mus]')
hWidth.SetStats(0)

print '\nCreating S1 Histogram.\n'
hS1 = Hist(s1NumBins, s1Min, s1Max, name='hS1', title='S1 Spectrum'+sForHistograms)
hS1.SetMarkerSize(0)
current_analysis.Draw(s1Branch, hist=hS1, selection=current_analysis.get_cuts())
hS1.GetXaxis().SetTitle('%s [PE]' % s1Branch)
hS1.GetYaxis().SetTitle('Counts')
#hS1.Scale(1./float(current_analysis.get_livetime()))
hS1.SetStats(0)

print '\nCreating S2 Histogram.\n'
hS2 = Hist(s2NumBins, s2Min, s2Max, name='hS2', title='S2 Spectrum'+sForHistograms)
hS2.SetMarkerSize(0)
current_analysis.Draw(s2Branch, hist=hS2, selection=current_analysis.get_cuts())
hS2.GetXaxis().SetTitle('%s [PE]' % s2Branch)
hS2.GetYaxis().SetTitle('Counts')
#hS2.Scale(1./float(current_analysis.get_livetime()))
hS2.SetStats(0)

print '\nCreating PSD Histogram.\n'
hPSD = Hist2D(lhNumBins, lhMin, lhMax, psdNumBins, psdMin, psdMax, name='hPSD', title='PSD vs Pulse Height'+sForHistograms)
current_analysis.Draw('LiqSciHeight[]:LiqSciTailRaw[]/LiqSciRaw[]', hist=hPSD, selection=current_analysis.get_cuts())
hPSD.GetXaxis().SetTitle('LiqSciHeight [V]')
hPSD.GetYaxis().SetTitle('PSD Parameter')
hPSD.SetMarkerSize(0)
hPSD.SetStats(0)

print '\nCreating Pulse Height vs Area Histogram.\n'
hHeightArea = Hist2D(50, 0, 0.01, 50, s1Min, 15, name='hHeightArea', title='Area vs Pulse Height'+sForHistograms)
current_analysis.Draw('S1IntegralBeforeLiqSciHeight[0]:S1IntegralBeforeLiqSci[0]', hist=hHeightArea, selection=current_analysis.get_cuts())
hHeightArea.GetXaxis().SetTitle('S1IntegralBeforeLiqSciHeight [V]')
hHeightArea.GetYaxis().SetTitle('S1IntegralBeforeLiqSci [PE]')
hHeightArea.SetMarkerSize(0)
hHeightArea.SetStats(0)


print '\n\nTotal number of events: %s' % hS1.Integral()
print 'Total time: %.0f \n\n' % current_analysis.get_livetime()
print 'Rate: %.2f events per hour \n\n' % (float(hS1.Integral()) / current_analysis.get_livetime() * 3600.)


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

c9 = root.TCanvas('c9','',1280/2,480)
hLiqSciS1TimeDiff.Draw()
c9.Update()

c10 = root.TCanvas('c10','',1280/2,480)
hHeightArea.Draw('colz')
c10.Update()


neriX_analysis.save_plot(['full_angle_files', 'results', '%.3fkV_%ddeg' % (cathodeSetting, degreeSetting)], c1, 's1s2_%.3fkV_%ddeg' % (cathodeSetting, degreeSetting))
neriX_analysis.save_plot(['full_angle_files', 'results', '%.3fkV_%ddeg' % (cathodeSetting, degreeSetting)], c5, 's1_%.3fkV_%ddeg' % (cathodeSetting, degreeSetting))
neriX_analysis.save_plot(['full_angle_files', 'results', '%.3fkV_%ddeg' % (cathodeSetting, degreeSetting)], c6, 's2_%.3fkV_%ddeg' % (cathodeSetting, degreeSetting))
neriX_analysis.save_plot(['full_angle_files', 'results', '%.3fkV_%ddeg' % (cathodeSetting, degreeSetting)], c7, 'tof_%.3fkV_%ddeg' % (cathodeSetting, degreeSetting))
neriX_analysis.save_plot(['full_angle_files', 'results', '%.3fkV_%ddeg' % (cathodeSetting, degreeSetting)], c9, 'liqsci_s1_time_diff_%.3fkV_%ddeg' % (cathodeSetting, degreeSetting))



raw_input('Enter to continue...')
response = raw_input('Would you like to save the S1 and S2 spectra for later analysis?  Press "y" to proceed, otherwise press anything: ')

if response == 'y':
	outputFile = root_open('./full_angle_files/results_%.3fkV_%ddeg.root' % (cathodeSetting, degreeSetting), 'recreate')
	
	hS1S2.SetName('hS1S2')
	hS1S2.Write()
	
	hS1.SetName('hS1')
	hS1.Write()
	
	hS2.SetName('hS2')
	hS2.Write()
	
	hTOF.SetName('hTOF')
	hTOF.Write()
	
	hLiqSciS1TimeDiff.SetName('hLiqSciS1TimeDiff')
	hLiqSciS1TimeDiff.Write()

	sCuts = root.TObjString('cuts_used')
	sCuts.Write(current_analysis.get_cuts(), root.TObject.kOverwrite)




# 160504_0915 (high rate file): output from peakfinder
# event 34280
# want stuff before filename event number Processed
# that is printed after event is finished

"""

    S2TinyPeak? [1790, 1812]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 22)
      PeakFinder::computeS2TinyPeakExtent(1876, 1401, 2130)
    S2TinyPeak? [1862, 1897]
      -> Failed S2TinyPeakPostPeakAvgThreshold (fPostPeakAvg: 0.0540865)
      PeakFinder::computeS2TinyPeakExtent(1924, 1401, 2130)
    S2TinyPeak? [1913, 1937]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 24)
  Region 1 [2482, 4096]
    S2TinyPeakRightLimitHeightThreshold reached (15.345 V)
PeakFinder::findS1Peaks()
  Region 0 [12, 1165]
    Sample: 791
      PeakFinder::computeS1Extent(791, 779, 851)
      S1? (iLeftLimit: 779, iRightLimit:851, iLeftExtent: 790, iRightExtent: 793)
        -> Accepted
    Sample: 820
      PeakFinder::computeS1Extent(828, 816, 888)
      S1? (iLeftLimit: 816, iRightLimit:888, iLeftExtent: 827, iRightExtent: 830)
        -> Accepted
  Region 1 [1401, 2130]
PeakFinder::findS1Peaks()
  Region 0 [12, 4041]
  nerix_160504_0915_000342  Processed: 78PeakFinder::findS2Peaks()
  Top Interval: [222, 457]
    Interval? [222, 457]
      -> Accepted (iRightEdge-iLeftEdge: 235)
      PeakFinder::computeS2LargePeakExtent(396, 122, 557)
      Quiet? (fMaxValue: 2.72617, iMinLeftEdge: 222, iMaxRightEdge: 463)
        -> Accepted
        S2? (fS2Width: 28.7947)
          -> Accepted
    Interval? [222, 369]
      -> Accepted (iRightEdge-iLeftEdge: 147)
      PeakFinder::computeS2LargePeakExtent(263, 122, 369)
        S2? (fS2Width: 75.5851)
          -> Accepted
    Interval? [222, 219]
      -> Failed S2LargePeakMinIntervalWidth (iRightEdge-iLeftEdge: -3)
    Interval? [318, 369]
      -> Accepted (iRightEdge-iLeftEdge: 51)
      PeakFinder::computeS2LargePeakExtent(357, 318, 369)
        S2? (fS2Width: 51)
          -> Accepted
    Interval? [318, 318]
      -> Failed S2LargePeakMinIntervalWidth (iRightEdge-iLeftEdge: 0)
    Interval? [369, 369]
      -> Failed S2LargePeakMinIntervalWidth (iRightEdge-iLeftEdge: 0)
    Interval? [463, 457]
      -> Failed S2LargePeakMinIntervalWidth (iRightEdge-iLeftEdge: -6)
  Top Interval: [513, 543]
    Interval? [513, 543]
      -> Failed S2LargePeakMinIntervalWidth (iRightEdge-iLeftEdge: 30)
  Top Interval: [1467, 1480]
    Interval? [1467, 1480]
      -> Failed S2LargePeakMinIntervalWidth (iRightEdge-iLeftEdge: 13)
  Top Interval: [2766, 2780]
    Interval? [2766, 2780]
      -> Failed S2LargePeakMinIntervalWidth (iRightEdge-iLeftEdge: 14)
  Top Interval: [3556, 3559]
    Interval? [3556, 3559]
      -> Failed S2LargePeakMinIntervalWidth (iRightEdge-iLeftEdge: 3)
  Region 0 [0, 219]
      PeakFinder::computeS2TinyPeakExtent(15, 0, 219)
    S2TinyPeak? [12, 17]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 5)
      PeakFinder::computeS2TinyPeakExtent(47, 0, 219)
    S2TinyPeak? [47, 50]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 3)
      PeakFinder::computeS2TinyPeakExtent(109, 0, 219)
    S2TinyPeak? [73, 119]
      -> Failed S2TinyPeakPrePeakAvgThreshold (fPrePeakAvg: 0.0727355)
      PeakFinder::computeS2TinyPeakExtent(201, 0, 219)
    S2TinyPeak? [193, 211]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 18)
  Region 1 [318, 318]
  Region 2 [369, 369]
  Region 3 [463, 4096]
      PeakFinder::computeS2TinyPeakExtent(463, 463, 4096)
    S2TinyPeak? [463, 471]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 8)
      PeakFinder::computeS2TinyPeakExtent(487, 463, 4096)
    S2TinyPeak? [482, 503]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 21)
      PeakFinder::computeS2TinyPeakExtent(525, 463, 4096)
    S2TinyPeak? [512, 563]
      -> Failed S2TinyPeakAspectRatioThreshold (fAspectRatio: 0.00107957)
      PeakFinder::computeS2TinyPeakExtent(614, 463, 4096)
    S2TinyPeak? [604, 624]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 20)
      PeakFinder::computeS2TinyPeakExtent(658, 463, 4096)
    S2TinyPeak? [647, 669]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 22)
      PeakFinder::computeS2TinyPeakExtent(724, 463, 4096)
    S2TinyPeak? [685, 742]
      -> Failed S2TinyPeakPrePeakAvgThreshold (fPrePeakAvg: 0.058732)
      PeakFinder::computeS2TinyPeakExtent(866, 463, 4096)
    S2TinyPeak? [823, 878]
      -> Failed S2TinyPeakPostPeakAvgThreshold (fPostPeakAvg: 1.40042)
      PeakFinder::computeS2TinyPeakExtent(943, 463, 4096)
    S2TinyPeak? [936, 956]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 20)
      PeakFinder::computeS2TinyPeakExtent(1060, 463, 4096)
    S2TinyPeak? [1047, 1069]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 22)
      PeakFinder::computeS2TinyPeakExtent(1149, 463, 4096)
    S2TinyPeak? [1136, 1169]
      -> Accepted
      PeakFinder::computeS2TinyPeakExtent(1473, 1349, 4096)
    S2TinyPeak? [1366, 1495]
      -> Accepted
      PeakFinder::computeS2TinyPeakExtent(1583, 1499, 4096)
    S2TinyPeak? [1542, 1680]
      -> Failed S2TinyPeakPostPeakAvgThreshold (fPostPeakAvg: 0.163809)
      PeakFinder::computeS2TinyPeakExtent(1749, 1499, 4096)
    S2TinyPeak? [1737, 1764]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 27)
      PeakFinder::computeS2TinyPeakExtent(1930, 1499, 4096)
    S2TinyPeak? [1930, 1950]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 20)
      PeakFinder::computeS2TinyPeakExtent(2005, 1499, 4096)
    S2TinyPeak? [2000, 2019]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 19)
      PeakFinder::computeS2TinyPeakExtent(2124, 1499, 4096)
    S2TinyPeak? [2113, 2139]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 26)
      PeakFinder::computeS2TinyPeakExtent(2192, 1499, 4096)
    S2TinyPeak? [2188, 2236]
      -> Accepted
      PeakFinder::computeS2TinyPeakExtent(2432, 2396, 4096)
    S2TinyPeak? [2420, 2445]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 25)
      PeakFinder::computeS2TinyPeakExtent(2471, 2396, 4096)
    S2TinyPeak? [2463, 2480]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 17)
      PeakFinder::computeS2TinyPeakExtent(2497, 2396, 4096)
    S2TinyPeak? [2486, 2509]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 23)
      PeakFinder::computeS2TinyPeakExtent(2581, 2396, 4096)
    S2TinyPeak? [2578, 2582]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 4)
      PeakFinder::computeS2TinyPeakExtent(2606, 2396, 4096)
    S2TinyPeak? [2605, 2607]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 2)
      PeakFinder::computeS2TinyPeakExtent(2609, 2396, 4096)
    S2TinyPeak? [2609, 2614]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 5)
      PeakFinder::computeS2TinyPeakExtent(2631, 2396, 4096)
    S2TinyPeak? [2621, 2640]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 19)
      PeakFinder::computeS2TinyPeakExtent(2772, 2396, 4096)
    S2TinyPeak? [2722, 2788]
      -> Failed S2TinyPeakAspectRatioThreshold (fAspectRatio: 0.00142661)
      PeakFinder::computeS2TinyPeakExtent(2797, 2396, 4096)
    S2TinyPeak? [2797, 2798]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 1)
      PeakFinder::computeS2TinyPeakExtent(2823, 2396, 4096)
    S2TinyPeak? [2815, 2833]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 18)
      PeakFinder::computeS2TinyPeakExtent(2891, 2396, 4096)
    S2TinyPeak? [2871, 2906]
      -> Accepted
      PeakFinder::computeS2TinyPeakExtent(2958, 2910, 4096)
    S2TinyPeak? [2952, 2962]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 10)
      PeakFinder::computeS2TinyPeakExtent(3026, 2910, 4096)
    S2TinyPeak? [2988, 3093]
      -> Failed S2TinyPeakPostPeakAvgThreshold (fPostPeakAvg: 1.49726)
      PeakFinder::computeS2TinyPeakExtent(3273, 2910, 4096)
    S2TinyPeak? [3264, 3286]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 22)
      PeakFinder::computeS2TinyPeakExtent(3461, 2910, 4096)
    S2TinyPeak? [3445, 3485]
      -> Accepted
      PeakFinder::computeS2TinyPeakExtent(3515, 3493, 4096)
    S2TinyPeak? [3505, 3525]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 20)
      PeakFinder::computeS2TinyPeakExtent(3557, 3493, 4096)
    S2TinyPeak? [3546, 3615]
      -> Failed S2TinyPeakAspectRatioThreshold (fAspectRatio: 0.00104823)
      PeakFinder::computeS2TinyPeakExtent(3678, 3493, 4096)
    S2TinyPeak? [3663, 3691]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 28)
      PeakFinder::computeS2TinyPeakExtent(3823, 3493, 4096)
    S2TinyPeak? [3823, 3824]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 1)
      PeakFinder::computeS2TinyPeakExtent(3851, 3493, 4096)
    S2TinyPeak? [3840, 3863]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 23)
      PeakFinder::computeS2TinyPeakExtent(4011, 3493, 4096)
    S2TinyPeak? [4001, 4021]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 20)
      PeakFinder::computeS2TinyPeakExtent(4044, 3493, 4096)
    S2TinyPeak? [4039, 4059]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 20)
      PeakFinder::computeS2TinyPeakExtent(4081, 3493, 4096)
    S2TinyPeak? [4081, 4083]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 2)
PeakFinder::findS1Peaks()
  Region 0 [12, 219]
    Sample: 194
      PeakFinder::computeS1Extent(194, 182, 254)
      S1? (iLeftLimit: 182, iRightLimit:254, iLeftExtent: 193, iRightExtent: 195)
        -> Accepted
  Region 1 [318, 318]
    S1RightLimitHeightThreshold reached (0.172623 V)
PeakFinder::findS1Peaks()
  Region 0 [12, 4041]
  nerix_160504_0915_000342  Processed: 79PeakFinder::findS2Peaks()
  Top Interval: [1438, 1456]
    Interval? [1438, 1456]
      -> Failed S2LargePeakMinIntervalWidth (iRightEdge-iLeftEdge: 18)
  Top Interval: [1893, 2031]
    Interval? [1893, 2031]
      -> Accepted (iRightEdge-iLeftEdge: 138)
      PeakFinder::computeS2LargePeakExtent(1953, 1793, 2131)
      Quiet? (fMaxValue: 0.164463, iMinLeftEdge: 1866, iMaxRightEdge: 2123)
        -> Accepted
        S2? (fS2Width: 56.713)
          -> Accepted
    Interval? [1893, 1866]
      -> Failed S2LargePeakMinIntervalWidth (iRightEdge-iLeftEdge: -27)
    Interval? [2123, 2031]
      -> Failed S2LargePeakMinIntervalWidth (iRightEdge-iLeftEdge: -92)
  Top Interval: [2871, 2882]
    Interval? [2871, 2882]
      -> Failed S2LargePeakMinIntervalWidth (iRightEdge-iLeftEdge: 11)
  Region 0 [0, 1866]
      PeakFinder::computeS2TinyPeakExtent(45, 0, 1866)
    S2TinyPeak? [33, 54]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 21)
      PeakFinder::computeS2TinyPeakExtent(125, 0, 1866)
    S2TinyPeak? [123, 126]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 3)
      PeakFinder::computeS2TinyPeakExtent(152, 0, 1866)
    S2TinyPeak? [152, 154]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 2)
      PeakFinder::computeS2TinyPeakExtent(306, 0, 1866)
    S2TinyPeak? [306, 310]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 4)
      PeakFinder::computeS2TinyPeakExtent(388, 0, 1866)
    S2TinyPeak? [370, 435]
      -> Accepted
      PeakFinder::computeS2TinyPeakExtent(499, 475, 1866)
    S2TinyPeak? [496, 503]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 7)
      PeakFinder::computeS2TinyPeakExtent(567, 475, 1866)
    S2TinyPeak? [557, 579]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 22)
      PeakFinder::computeS2TinyPeakExtent(631, 475, 1866)
    S2TinyPeak? [591, 644]
      -> Failed S2TinyPeakPostPeakAvgThreshold (fPostPeakAvg: 0.0726244)
      PeakFinder::computeS2TinyPeakExtent(812, 475, 1866)
    S2TinyPeak? [786, 865]
      -> Failed S2TinyPeakPostPeakAvgThreshold (fPostPeakAvg: 1.08533)
      PeakFinder::computeS2TinyPeakExtent(925, 475, 1866)
    S2TinyPeak? [921, 945]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 24)
      PeakFinder::computeS2TinyPeakExtent(1063, 475, 1866)
    S2TinyPeak? [1056, 1109]
      -> Accepted
      PeakFinder::computeS2TinyPeakExtent(1223, 1178, 1866)
    S2TinyPeak? [1210, 1237]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 27)
      PeakFinder::computeS2TinyPeakExtent(1325, 1178, 1866)
    S2TinyPeak? [1313, 1333]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 20)
      PeakFinder::computeS2TinyPeakExtent(1359, 1178, 1866)
    S2TinyPeak? [1349, 1376]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 27)
      PeakFinder::computeS2TinyPeakExtent(1447, 1178, 1866)
    S2TinyPeak? [1434, 1461]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 27)
      PeakFinder::computeS2TinyPeakExtent(1517, 1178, 1866)
    S2TinyPeak? [1506, 1527]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 21)
      PeakFinder::computeS2TinyPeakExtent(1608, 1178, 1866)
    S2TinyPeak? [1598, 1641]
      -> Accepted
      PeakFinder::computeS2TinyPeakExtent(1818, 1742, 1866)
    S2TinyPeak? [1813, 1825]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 12)
  Region 1 [2123, 4096]
      PeakFinder::computeS2TinyPeakExtent(2180, 2123, 4096)
    S2TinyPeak? [2142, 2223]
      -> Accepted
      PeakFinder::computeS2TinyPeakExtent(2258, 2227, 4096)
    S2TinyPeak? [2243, 2278]
      -> Accepted
      PeakFinder::computeS2TinyPeakExtent(2463, 2365, 4096)
    S2TinyPeak? [2456, 2471]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 15)
      PeakFinder::computeS2TinyPeakExtent(2495, 2365, 4096)
    S2TinyPeak? [2483, 2510]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 27)
      PeakFinder::computeS2TinyPeakExtent(2565, 2365, 4096)
    S2TinyPeak? [2554, 2579]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 25)
      PeakFinder::computeS2TinyPeakExtent(2610, 2365, 4096)
    S2TinyPeak? [2601, 2621]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 20)
      PeakFinder::computeS2TinyPeakExtent(2679, 2365, 4096)
    S2TinyPeak? [2676, 2686]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 10)
      PeakFinder::computeS2TinyPeakExtent(2716, 2365, 4096)
    S2TinyPeak? [2706, 2727]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 21)
      PeakFinder::computeS2TinyPeakExtent(2778, 2365, 4096)
    S2TinyPeak? [2769, 2792]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 23)
      PeakFinder::computeS2TinyPeakExtent(2876, 2365, 4096)
    S2TinyPeak? [2838, 2908]
      -> Accepted
      PeakFinder::computeS2TinyPeakExtent(2963, 2947, 4096)
    S2TinyPeak? [2959, 2964]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 5)
      PeakFinder::computeS2TinyPeakExtent(2975, 2947, 4096)
    S2TinyPeak? [2966, 2987]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 21)
      PeakFinder::computeS2TinyPeakExtent(3142, 2947, 4096)
    S2TinyPeak? [3135, 3150]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 15)
      PeakFinder::computeS2TinyPeakExtent(3168, 2947, 4096)
    S2TinyPeak? [3158, 3178]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 20)
      PeakFinder::computeS2TinyPeakExtent(3296, 2947, 4096)
    S2TinyPeak? [3287, 3309]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 22)
      PeakFinder::computeS2TinyPeakExtent(3422, 2947, 4096)
    S2TinyPeak? [3409, 3450]
      -> Failed S2TinyPeakPostPeakAvgThreshold (fPostPeakAvg: 0.0720881)
      PeakFinder::computeS2TinyPeakExtent(3524, 2947, 4096)
    S2TinyPeak? [3509, 3537]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 28)
      PeakFinder::computeS2TinyPeakExtent(3610, 2947, 4096)
    S2TinyPeak? [3579, 3673]
      -> Accepted
      PeakFinder::computeS2TinyPeakExtent(3715, 3682, 4096)
    S2TinyPeak? [3704, 3726]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 22)
      PeakFinder::computeS2TinyPeakExtent(3795, 3682, 4096)
    S2TinyPeak? [3787, 3807]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 20)
      PeakFinder::computeS2TinyPeakExtent(3892, 3682, 4096)
    S2TinyPeak? [3890, 3893]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 3)
      PeakFinder::computeS2TinyPeakExtent(3899, 3682, 4096)
    S2TinyPeak? [3899, 3905]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 6)
      PeakFinder::computeS2TinyPeakExtent(3933, 3682, 4096)
    S2TinyPeak? [3933, 3937]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 4)
      PeakFinder::computeS2TinyPeakExtent(3985, 3682, 4096)
    S2TinyPeak? [3957, 3998]
      -> Failed S2TinyPeakPrePeakAvgThreshold (fPrePeakAvg: 1.565)
      PeakFinder::computeS2TinyPeakExtent(4079, 3682, 4096)
    S2TinyPeak? [4071, 4083]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 12)
      PeakFinder::computeS2TinyPeakExtent(4081, 3682, 4096)
    S2TinyPeak? [4081, 4083]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 2)
PeakFinder::findS1Peaks()
  Region 0 [12, 341]
  Region 1 [475, 778]
    Sample: 671
      PeakFinder::computeS1Extent(710, 698, 770)
      S1? (iLeftLimit: 698, iRightLimit:770, iLeftExtent: 709, iRightExtent: 711)
        -> Accepted
  Region 2 [1178, 1501]
    Sample: 1332
      PeakFinder::computeS1Extent(1333, 1321, 1393)
      S1? (iLeftLimit: 1321, iRightLimit:1393, iLeftExtent: 1331, iRightExtent: 1334)
        -> Accepted
  Region 3 [1742, 1866]
PeakFinder::findS1Peaks()
  Region 0 [12, 4041]
  nerix_160504_0915_000342  Processed: 80PeakFinder::findS2Peaks()
  Top Interval: [361, 559]
    Interval? [361, 559]
      -> Accepted (iRightEdge-iLeftEdge: 198)
      PeakFinder::computeS2LargePeakExtent(494, 261, 659)
      Quiet? (fMaxValue: 3.4055, iMinLeftEdge: 361, iMaxRightEdge: 561)
        -> Accepted
        S2? (fS2Width: 25.6669)
          -> Accepted
    Interval? [361, 464]
      -> Accepted (iRightEdge-iLeftEdge: 103)
      PeakFinder::computeS2LargePeakExtent(390, 261, 464)
        S2? (fS2Width: 42.0846)
          -> Accepted
    Interval? [361, 359]
      -> Failed S2LargePeakMinIntervalWidth (iRightEdge-iLeftEdge: -2)
    Interval? [464, 464]
      -> Failed S2LargePeakMinIntervalWidth (iRightEdge-iLeftEdge: 0)
    Interval? [561, 559]
      -> Failed S2LargePeakMinIntervalWidth (iRightEdge-iLeftEdge: -2)
  Top Interval: [772, 775]
    Interval? [772, 775]
      -> Failed S2LargePeakMinIntervalWidth (iRightEdge-iLeftEdge: 3)
  Top Interval: [1914, 1922]
    Interval? [1914, 1922]
      -> Failed S2LargePeakMinIntervalWidth (iRightEdge-iLeftEdge: 8)
  Top Interval: [2205, 2216]
    Interval? [2205, 2216]
      -> Failed S2LargePeakMinIntervalWidth (iRightEdge-iLeftEdge: 11)
  Region 0 [0, 359]
      PeakFinder::computeS2TinyPeakExtent(15, 0, 359)
    S2TinyPeak? [12, 23]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 11)
      PeakFinder::computeS2TinyPeakExtent(53, 0, 359)
    S2TinyPeak? [52, 54]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 2)
      PeakFinder::computeS2TinyPeakExtent(63, 0, 359)
    S2TinyPeak? [56, 70]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 14)
      PeakFinder::computeS2TinyPeakExtent(191, 0, 359)
    S2TinyPeak? [184, 199]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 15)
      PeakFinder::computeS2TinyPeakExtent(268, 0, 359)
    S2TinyPeak? [259, 279]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 20)
  Region 1 [464, 464]
  Region 2 [561, 4096]
      PeakFinder::computeS2TinyPeakExtent(561, 561, 4096)
    S2TinyPeak? [561, 597]
      -> Failed S2TinyPeakPrePeakAvgThreshold (fPrePeakAvg: 3.26112)
      PeakFinder::computeS2TinyPeakExtent(687, 561, 4096)
    S2TinyPeak? [683, 688]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 5)
      PeakFinder::computeS2TinyPeakExtent(690, 561, 4096)
    S2TinyPeak? [690, 694]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 4)
      PeakFinder::computeS2TinyPeakExtent(773, 561, 4096)
    S2TinyPeak? [731, 807]
      -> Accepted
      PeakFinder::computeS2TinyPeakExtent(855, 811, 4096)
    S2TinyPeak? [855, 856]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 1)
      PeakFinder::computeS2TinyPeakExtent(868, 811, 4096)
    S2TinyPeak? [861, 869]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 8)
      PeakFinder::computeS2TinyPeakExtent(882, 811, 4096)
    S2TinyPeak? [872, 893]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 21)
      PeakFinder::computeS2TinyPeakExtent(999, 811, 4096)
    S2TinyPeak? [988, 1013]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 25)
      PeakFinder::computeS2TinyPeakExtent(1108, 811, 4096)
    S2TinyPeak? [1088, 1127]
      -> Accepted
      PeakFinder::computeS2TinyPeakExtent(1268, 1130, 4096)
    S2TinyPeak? [1228, 1335]
      -> Accepted
      PeakFinder::computeS2TinyPeakExtent(1403, 1352, 4096)
    S2TinyPeak? [1393, 1422]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 29)
      PeakFinder::computeS2TinyPeakExtent(1529, 1352, 4096)
    S2TinyPeak? [1519, 1541]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 22)
      PeakFinder::computeS2TinyPeakExtent(1591, 1352, 4096)
    S2TinyPeak? [1583, 1602]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 19)
      PeakFinder::computeS2TinyPeakExtent(1637, 1352, 4096)
    S2TinyPeak? [1622, 1657]
      -> Accepted
      PeakFinder::computeS2TinyPeakExtent(1806, 1662, 4096)
    S2TinyPeak? [1790, 1821]
      -> Accepted
      PeakFinder::computeS2TinyPeakExtent(1916, 1875, 4096)
    S2TinyPeak? [1892, 1977]
      -> Accepted
      PeakFinder::computeS2TinyPeakExtent(1986, 1985, 4096)
    S2TinyPeak? [1986, 1987]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 1)
      PeakFinder::computeS2TinyPeakExtent(2013, 1985, 4096)
    S2TinyPeak? [2007, 2014]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 7)
      PeakFinder::computeS2TinyPeakExtent(2015, 1985, 4096)
    S2TinyPeak? [2015, 2022]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 7)
      PeakFinder::computeS2TinyPeakExtent(2054, 1985, 4096)
    S2TinyPeak? [2043, 2066]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 23)
      PeakFinder::computeS2TinyPeakExtent(2145, 1985, 4096)
    S2TinyPeak? [2144, 2146]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 2)
      PeakFinder::computeS2TinyPeakExtent(2210, 1985, 4096)
    S2TinyPeak? [2198, 2240]
      -> Failed S2TinyPeakAspectRatioThreshold (fAspectRatio: 0.00162905)
      PeakFinder::computeS2TinyPeakExtent(2309, 1985, 4096)
    S2TinyPeak? [2299, 2319]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 20)
      PeakFinder::computeS2TinyPeakExtent(2330, 1985, 4096)
    S2TinyPeak? [2323, 2343]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 20)
      PeakFinder::computeS2TinyPeakExtent(2461, 1985, 4096)
    S2TinyPeak? [2451, 2472]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 21)
      PeakFinder::computeS2TinyPeakExtent(2501, 1985, 4096)
    S2TinyPeak? [2491, 2515]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 24)
      PeakFinder::computeS2TinyPeakExtent(2745, 1985, 4096)
    S2TinyPeak? [2741, 2746]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 5)
      PeakFinder::computeS2TinyPeakExtent(2773, 1985, 4096)
    S2TinyPeak? [2750, 2788]
      -> Failed S2TinyPeakPrePeakAvgThreshold (fPrePeakAvg: 0.17681)
      PeakFinder::computeS2TinyPeakExtent(2839, 1985, 4096)
    S2TinyPeak? [2826, 2881]
      -> Accepted
      PeakFinder::computeS2TinyPeakExtent(2964, 2918, 4096)
    S2TinyPeak? [2940, 2979]
      -> Failed S2TinyPeakPostPeakAvgThreshold (fPostPeakAvg: 1.21766)
      PeakFinder::computeS2TinyPeakExtent(3044, 2918, 4096)
    S2TinyPeak? [3039, 3062]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 23)
      PeakFinder::computeS2TinyPeakExtent(3114, 2918, 4096)
    S2TinyPeak? [3095, 3127]
      -> Accepted
      PeakFinder::computeS2TinyPeakExtent(3168, 3132, 4096)
    S2TinyPeak? [3155, 3177]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 22)
      PeakFinder::computeS2TinyPeakExtent(3195, 3132, 4096)
    S2TinyPeak? [3183, 3223]
      -> Accepted
      PeakFinder::computeS2TinyPeakExtent(3381, 3353, 4096)
    S2TinyPeak? [3379, 3382]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 3)
      PeakFinder::computeS2TinyPeakExtent(3430, 3353, 4096)
    S2TinyPeak? [3422, 3440]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 18)
      PeakFinder::computeS2TinyPeakExtent(3493, 3353, 4096)
    S2TinyPeak? [3484, 3501]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 17)
      PeakFinder::computeS2TinyPeakExtent(3562, 3353, 4096)
    S2TinyPeak? [3550, 3576]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 26)
      PeakFinder::computeS2TinyPeakExtent(3652, 3353, 4096)
    S2TinyPeak? [3645, 3668]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 23)
      PeakFinder::computeS2TinyPeakExtent(3721, 3353, 4096)
    S2TinyPeak? [3713, 3730]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 17)
      PeakFinder::computeS2TinyPeakExtent(3761, 3353, 4096)
    S2TinyPeak? [3751, 3773]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 22)
      PeakFinder::computeS2TinyPeakExtent(3832, 3353, 4096)
    S2TinyPeak? [3812, 3845]
      -> Failed S2TinyPeakPostPeakAvgThreshold (fPostPeakAvg: 0.0561435)
      PeakFinder::computeS2TinyPeakExtent(3867, 3353, 4096)
    S2TinyPeak? [3857, 3876]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 19)
      PeakFinder::computeS2TinyPeakExtent(3946, 3353, 4096)
    S2TinyPeak? [3924, 3976]
      -> Accepted
      PeakFinder::computeS2TinyPeakExtent(4046, 3982, 4096)
    S2TinyPeak? [4034, 4054]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 20)
      PeakFinder::computeS2TinyPeakExtent(4081, 3982, 4096)
    S2TinyPeak? [4081, 4083]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 2)
PeakFinder::findS1Peaks()
  Region 0 [12, 359]
    Sample: 354
      PeakFinder::computeS1Extent(354, 342, 414)
      S1? (iLeftLimit: 342, iRightLimit:414, iLeftExtent: 353, iRightExtent: 356)
        -> Failed FilteredWidth (iFilteredWidth: 72)
  Region 1 [464, 464]
    S1RightLimitHeightThreshold reached (0.235158 V)
PeakFinder::findS1Peaks()
  Region 0 [12, 4041]
  nerix_160504_0915_000342  Processed: 81PeakFinder::findS2Peaks()
  Top Interval: [490, 522]
    Interval? [490, 522]
      -> Failed S2LargePeakMinIntervalWidth (iRightEdge-iLeftEdge: 32)
  Top Interval: [530, 644]
    Interval? [530, 644]
      -> Accepted (iRightEdge-iLeftEdge: 114)
      PeakFinder::computeS2LargePeakExtent(562, 430, 744)
      Quiet? (fMaxValue: 9.02377, iMinLeftEdge: 530, iMaxRightEdge: 644)
        -> Accepted
        S2? (fS2Width: 27.7781)
          -> Accepted
    Interval? [530, 532]
      -> Failed S2LargePeakMinIntervalWidth (iRightEdge-iLeftEdge: 2)
    Interval? [638, 644]
      -> Failed S2LargePeakMinIntervalWidth (iRightEdge-iLeftEdge: 6)
  Top Interval: [664, 736]
    Interval? [664, 736]
      -> Accepted (iRightEdge-iLeftEdge: 72)
      PeakFinder::computeS2LargePeakExtent(698, 638, 836)
      Quiet? (fMaxValue: 0.0570462, iMinLeftEdge: 655, iMaxRightEdge: 760)
        -> Accepted
        S2? (fS2Width: 43.4957)
          -> Accepted
    Interval? [664, 655]
      -> Failed S2LargePeakMinIntervalWidth (iRightEdge-iLeftEdge: -9)
    Interval? [760, 736]
      -> Failed S2LargePeakMinIntervalWidth (iRightEdge-iLeftEdge: -24)
  Top Interval: [832, 862]
    Interval? [832, 862]
      -> Failed S2LargePeakMinIntervalWidth (iRightEdge-iLeftEdge: 30)
  Top Interval: [1022, 1040]
    Interval? [1022, 1040]
      -> Failed S2LargePeakMinIntervalWidth (iRightEdge-iLeftEdge: 18)
  Top Interval: [1103, 1119]
    Interval? [1103, 1119]
      -> Failed S2LargePeakMinIntervalWidth (iRightEdge-iLeftEdge: 16)
  Top Interval: [1156, 1177]
    Interval? [1156, 1177]
      -> Failed S2LargePeakMinIntervalWidth (iRightEdge-iLeftEdge: 21)
  Top Interval: [1183, 1254]
    Interval? [1183, 1254]
      -> Accepted (iRightEdge-iLeftEdge: 71)
      PeakFinder::computeS2LargePeakExtent(1203, 1083, 1354)
      Quiet? (fMaxValue: 0.0151432, iMinLeftEdge: 1134, iMaxRightEdge: 1326)
        -> Failed PreTopLevelIntervalAvg (fPreIntervalAvg: 0.533079)
  Top Interval: [1418, 1442]
    Interval? [1418, 1442]
      -> Failed S2LargePeakMinIntervalWidth (iRightEdge-iLeftEdge: 24)
  Top Interval: [1548, 1611]
    Interval? [1548, 1611]
      -> Accepted (iRightEdge-iLeftEdge: 63)
      PeakFinder::computeS2LargePeakExtent(1574, 1448, 1711)
      Quiet? (fMaxValue: 0.0167378, iMinLeftEdge: 1507, iMaxRightEdge: 1678)
        -> Failed PreTopLevelIntervalAvg (fPreIntervalAvg: 0.414061)
  Top Interval: [1744, 1781]
    Interval? [1744, 1781]
      -> Failed S2LargePeakMinIntervalWidth (iRightEdge-iLeftEdge: 37)
  Top Interval: [1923, 1966]
    Interval? [1923, 1966]
      -> Accepted (iRightEdge-iLeftEdge: 43)
      PeakFinder::computeS2LargePeakExtent(1943, 1823, 2066)
      Quiet? (fMaxValue: 0.0194511, iMinLeftEdge: 1872, iMaxRightEdge: 1995)
        -> Accepted
        S2? (fS2Width: 44.6741)
          -> Accepted
    Interval? [1923, 1872]
      -> Failed S2LargePeakMinIntervalWidth (iRightEdge-iLeftEdge: -51)
    Interval? [1995, 1966]
      -> Failed S2LargePeakMinIntervalWidth (iRightEdge-iLeftEdge: -29)
  Top Interval: [2115, 2125]
    Interval? [2115, 2125]
      -> Failed S2LargePeakMinIntervalWidth (iRightEdge-iLeftEdge: 10)
  Top Interval: [3501, 3567]
    Interval? [3501, 3567]
      -> Accepted (iRightEdge-iLeftEdge: 66)
      PeakFinder::computeS2LargePeakExtent(3522, 3401, 3667)
      Quiet? (fMaxValue: 0.395416, iMinLeftEdge: 3458, iMaxRightEdge: 3579)
        -> Accepted
        S2? (fS2Width: 24.1917)
          -> Accepted
    Interval? [3501, 3458]
      -> Failed S2LargePeakMinIntervalWidth (iRightEdge-iLeftEdge: -43)
    Interval? [3579, 3567]
      -> Failed S2LargePeakMinIntervalWidth (iRightEdge-iLeftEdge: -12)
  Top Interval: [3650, 3831]
    Interval? [3650, 3831]
      -> Accepted (iRightEdge-iLeftEdge: 181)
      PeakFinder::computeS2LargePeakExtent(3705, 3579, 3931)
      Quiet? (fMaxValue: 0.503249, iMinLeftEdge: 3620, iMaxRightEdge: 3881)
        -> Accepted
        S2? (fS2Width: 60.207)
          -> Accepted
    Interval? [3650, 3620]
      -> Failed S2LargePeakMinIntervalWidth (iRightEdge-iLeftEdge: -30)
    Interval? [3881, 3831]
      -> Failed S2LargePeakMinIntervalWidth (iRightEdge-iLeftEdge: -50)
  Region 0 [0, 532]
      PeakFinder::computeS2TinyPeakExtent(66, 0, 532)
    S2TinyPeak? [49, 78]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 29)
      PeakFinder::computeS2TinyPeakExtent(119, 0, 532)
    S2TinyPeak? [107, 142]
      -> Accepted
      PeakFinder::computeS2TinyPeakExtent(239, 164, 532)
    S2TinyPeak? [226, 254]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 28)
      PeakFinder::computeS2TinyPeakExtent(281, 164, 532)
    S2TinyPeak? [270, 352]
      -> Failed S2TinyPeakPostPeakAvgThreshold (fPostPeakAvg: 0.0568401)
      PeakFinder::computeS2TinyPeakExtent(506, 164, 532)
    S2TinyPeak? [460, 529]
      -> Failed S2TinyPeakAspectRatioThreshold (fAspectRatio: 0.00320308)
  Region 1 [638, 655]
  Region 2 [760, 1872]
      PeakFinder::computeS2TinyPeakExtent(1574, 760, 1872)
    S2TinyPeak? [760, 1671]
      -> Failed S2TinyPeakMaxIntervalWidth (iIntervalWidth: 911)
      PeakFinder::computeS2TinyPeakExtent(1764, 760, 1872)
    S2TinyPeak? [1755, 1801]
      -> Failed S2TinyPeakPrePeakAvgThreshold (fPrePeakAvg: 0.166743)
  Region 3 [1995, 3458]
      PeakFinder::computeS2TinyPeakExtent(2063, 1995, 3458)
    S2TinyPeak? [2041, 2083]
      -> Accepted
      PeakFinder::computeS2TinyPeakExtent(2119, 2085, 3458)
    S2TinyPeak? [2106, 2144]
      -> Failed S2TinyPeakAspectRatioThreshold (fAspectRatio: 0.00187095)
      PeakFinder::computeS2TinyPeakExtent(2237, 2085, 3458)
    S2TinyPeak? [2206, 2259]
      -> Accepted
      PeakFinder::computeS2TinyPeakExtent(2333, 2265, 3458)
    S2TinyPeak? [2328, 2334]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 6)
      PeakFinder::computeS2TinyPeakExtent(2339, 2265, 3458)
    S2TinyPeak? [2339, 2346]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 7)
      PeakFinder::computeS2TinyPeakExtent(2374, 2265, 3458)
    S2TinyPeak? [2359, 2390]
      -> Failed S2TinyPeakPostPeakAvgThreshold (fPostPeakAvg: 1.46292)
      PeakFinder::computeS2TinyPeakExtent(2418, 2265, 3458)
    S2TinyPeak? [2411, 2430]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 19)
      PeakFinder::computeS2TinyPeakExtent(2558, 2265, 3458)
    S2TinyPeak? [2549, 2567]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 18)
      PeakFinder::computeS2TinyPeakExtent(2601, 2265, 3458)
    S2TinyPeak? [2588, 2617]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 29)
      PeakFinder::computeS2TinyPeakExtent(2674, 2265, 3458)
    S2TinyPeak? [2663, 2684]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 21)
      PeakFinder::computeS2TinyPeakExtent(2856, 2265, 3458)
    S2TinyPeak? [2844, 2870]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 26)
      PeakFinder::computeS2TinyPeakExtent(2942, 2265, 3458)
    S2TinyPeak? [2905, 2956]
      -> Accepted
      PeakFinder::computeS2TinyPeakExtent(3208, 3136, 3458)
    S2TinyPeak? [3202, 3212]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 10)
      PeakFinder::computeS2TinyPeakExtent(3270, 3136, 3458)
    S2TinyPeak? [3260, 3283]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 23)
      PeakFinder::computeS2TinyPeakExtent(3324, 3136, 3458)
    S2TinyPeak? [3315, 3343]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 28)
      PeakFinder::computeS2TinyPeakExtent(3362, 3136, 3458)
    S2TinyPeak? [3355, 3377]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 22)
  Region 4 [3579, 3620]
  Region 5 [3881, 4096]
      PeakFinder::computeS2TinyPeakExtent(3955, 3881, 4096)
    S2TinyPeak? [3945, 3967]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 22)
      PeakFinder::computeS2TinyPeakExtent(4010, 3881, 4096)
    S2TinyPeak? [3991, 4026]
      -> Accepted
PeakFinder::findS1Peaks()
  Region 0 [12, 14]
  Region 1 [164, 532]
    Sample: 294
      PeakFinder::computeS1Extent(295, 283, 355)
      S1? (iLeftLimit: 283, iRightLimit:355, iLeftExtent: 293, iRightExtent: 299)
        -> Accepted
    Sample: 480
      PeakFinder::computeS1Extent(503, 491, 563)
      S1? (iLeftLimit: 491, iRightLimit:563, iLeftExtent: 502, iRightExtent: 506)
        -> Accepted
    Sample: 507
      PeakFinder::computeS1Extent(507, 495, 567)
      S1? (iLeftLimit: 495, iRightLimit:567, iLeftExtent: 506, iRightExtent: 512)
        -> Failed PrePeakAvg (fPrePeakAvg: 1.26476)
    Sample: 513
      PeakFinder::computeS1Extent(513, 501, 573)
      S1? (iLeftLimit: 501, iRightLimit:573, iLeftExtent: 512, iRightExtent: 514)
        -> Failed PrePeakAvg (fPrePeakAvg: 6.3434)
    Sample: 515
      PeakFinder::computeS1Extent(515, 503, 575)
      S1? (iLeftLimit: 503, iRightLimit:575, iLeftExtent: 513, iRightExtent: 518)
        -> Failed PrePeakAvg (fPrePeakAvg: 17.1488)
    Sample: 519
      PeakFinder::computeS1Extent(519, 507, 579)
      S1? (iLeftLimit: 507, iRightLimit:579, iLeftExtent: 517, iRightExtent: 520)
        -> Failed PrePeakAvg (fPrePeakAvg: 32.8284)
PeakFinder::findS1Peaks()
  Region 0 [12, 4041]
  nerix_160504_0915_000342  Processed: 82PeakFinder::findS2Peaks()
  Top Interval: [413, 1541]
    Interval? [413, 1541]
      -> Accepted (iRightEdge-iLeftEdge: 1128)
      PeakFinder::computeS2LargePeakExtent(467, 313, 1641)
      Quiet? (fMaxValue: 15.4085, iMinLeftEdge: 413, iMaxRightEdge: 1541)
        -> Accepted
        S2? (fS2Width: 41.1332)
          -> Accepted
    Interval? [413, 421]
      -> Failed S2LargePeakMinIntervalWidth (iRightEdge-iLeftEdge: 8)
    Interval? [752, 1541]
      -> Accepted (iRightEdge-iLeftEdge: 789)
      PeakFinder::computeS2LargePeakExtent(1326, 752, 1641)
      Quiet? (fMaxValue: 0.0462465, iLeftExtent: 1253, iRightExtent: 1626)
        -> Failed PreIntervalAvg (fPreIntervalAvg: 0.491876
        S2? (fS2Width: 95.3724)
          -> Failed S2Width (fS2Width: 95.3724)
  Top Interval: [1654, 1706]
    Interval? [1654, 1706]
      -> Accepted (iRightEdge-iLeftEdge: 52)
      PeakFinder::computeS2LargePeakExtent(1675, 1554, 1806)
      Quiet? (fMaxValue: 0.0324939, iMinLeftEdge: 1626, iMaxRightEdge: 1729)
        -> Failed PreTopLevelIntervalAvg (fPreIntervalAvg: 0.194312)
  Top Interval: [1752, 1756]
    Interval? [1752, 1756]
      -> Failed S2LargePeakMinIntervalWidth (iRightEdge-iLeftEdge: 4)
  Top Interval: [1959, 1983]
    Interval? [1959, 1983]
      -> Failed S2LargePeakMinIntervalWidth (iRightEdge-iLeftEdge: 24)
  Top Interval: [2104, 2148]
    Interval? [2104, 2148]
      -> Accepted (iRightEdge-iLeftEdge: 44)
      PeakFinder::computeS2LargePeakExtent(2132, 2004, 2248)
      Quiet? (fMaxValue: 0.0274073, iMinLeftEdge: 2045, iMaxRightEdge: 2177)
        -> Accepted
        S2? (fS2Width: 47.9463)
          -> Accepted
    Interval? [2104, 2045]
      -> Failed S2LargePeakMinIntervalWidth (iRightEdge-iLeftEdge: -59)
    Interval? [2177, 2148]
      -> Failed S2LargePeakMinIntervalWidth (iRightEdge-iLeftEdge: -29)
  Region 0 [0, 421]
      PeakFinder::computeS2TinyPeakExtent(20, 0, 421)
    S2TinyPeak? [19, 23]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 4)
      PeakFinder::computeS2TinyPeakExtent(59, 0, 421)
    S2TinyPeak? [49, 70]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 21)
      PeakFinder::computeS2TinyPeakExtent(93, 0, 421)
    S2TinyPeak? [84, 101]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 17)
      PeakFinder::computeS2TinyPeakExtent(195, 0, 421)
    S2TinyPeak? [182, 225]
      -> Accepted
      PeakFinder::computeS2TinyPeakExtent(268, 230, 421)
    S2TinyPeak? [258, 305]
      -> Failed S2TinyPeakPostPeakAvgThreshold (fPostPeakAvg: 0.897255)
      PeakFinder::computeS2TinyPeakExtent(375, 230, 421)
    S2TinyPeak? [375, 388]
      -> Failed S2TinyPeakMinIntervalWidth (iIntervalWidth: 13)
  Region 1 [752, 2045]
    S2TinyPeakRightLimitHeightThreshold reached (15.4085 V)
PeakFinder::findS1Peaks()
  Region 0 [12, 166]
  Region 1 [230, 421]
    Sample: 356
      PeakFinder::computeS1Extent(382, 370, 442)
      S1? (iLeftLimit: 370, iRightLimit:442, iLeftExtent: 381, iRightExtent: 384)
        -> Accepted
PeakFinder::findS1Peaks()
  Region 0 [12, 4041]



"""