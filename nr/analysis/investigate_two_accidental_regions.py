#!/usr/bin/python

import ROOT as root
import sys
import neriX_analysis
from rootpy.plotting import Hist2D, Hist, Legend, Canvas
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




#s1Branch = 'S1IntegralBeforeLiqSci[0]'
s1Branch = 'cpS1sTotBottom[0]'
s2Branch = 'cpS2sTotBottom[0]'
liqSciIndex = 0

current_analysis.add_cut('%s > 10' % s1Branch)
current_analysis.add_cut('%s > 0' % s2Branch)
#current_analysis.add_cut('LiqSciPeak[] != -1')
current_analysis.add_cut('(S2sPeak[0] - LiqSciPeak[%d])/100. > 3 && (S2sPeak[0] - LiqSciPeak[%d])/100. < 20 && LiqSciPeak[%d] > 0' % (liqSciIndex, liqSciIndex, liqSciIndex))
#current_analysis.add_cut('S2sPeak[0] > 600 && S2sPeak[0] < 2400') # hacky cut since we know LiqSciPeak is around 400 samples
current_analysis.add_cut('((%s > 24.0) || (%s < (7.406e+02 + 6.240e+01*%s + -4.430e-01*pow(%s, 2.))))' % (s1Branch, s2Branch, s1Branch, s1Branch))


# TOF cut if wanted
#current_analysis.add_cut('TimeOfFlight > 10 && TimeOfFlight < 32')

#current_analysis.set_event_list()
current_analysis.multithread_set_event_list(7)


#create histograms for current_analysis

print '\nUsing S1 Branch: %s \n\n' % s1Branch
print '\nUsing S2 Branch: %s \n\n' % s2Branch

c1 = Canvas()

hLiqSciS1TimeDiff = Hist(liqsciS1TimeDiffBins, liqsciS1TimeDiffMin, liqsciS1TimeDiffMax, name='hLiqSciS1TimeDiff', title='Time Between Liqsci and S1 Spectrum'+sForHistograms, drawstyle='hist')
current_analysis.Draw('LiqSciPeak[%d] - S1sPeak[0]' % (liqSciIndex), hist=hLiqSciS1TimeDiff, selection=current_analysis.get_cuts())
hLiqSciS1TimeDiff.GetXaxis().SetTitle('LiqSciPeak[%d] - S1sPeak[0] [samples]' % (liqSciIndex))
hLiqSciS1TimeDiff.GetYaxis().SetTitle('Counts')
hLiqSciS1TimeDiff.SetStats(1)

hLiqSciS1TimeDiff.Draw()
c1.Update()

c2 = Canvas()

hTOF = Hist(tofNumBins, tofMin, tofMax, name='hTOF', title='TOF Spectrum'+sForHistograms, drawstyle='hist')
current_analysis.Draw('TimeOfFlight', hist=hTOF, selection=current_analysis.get_cuts())
hTOF.GetXaxis().SetTitle('TOF [ns]')
hTOF.GetYaxis().SetTitle('Counts')
hTOF.SetStats(1)

hTOF.Draw()
c2.Update()


c3 = Canvas()

hLiqSciS1TimeDiff = Hist(liqsciS1TimeDiffBins, liqsciS1TimeDiffMin, liqsciS1TimeDiffMax, name='hLiqSciS1TimeDiffLeftEdge', title='Time Between Liqsci and S1 Spectrum'+sForHistograms, drawstyle='hist')
current_analysis.Draw('LiqSciPeak[%d] - S1sLeftEdge[0]' % (liqSciIndex), hist=hLiqSciS1TimeDiff, selection=current_analysis.get_cuts())
hLiqSciS1TimeDiff.GetXaxis().SetTitle('LiqSciPeak[] - S1sLeftEdge[0] [samples]')
hLiqSciS1TimeDiff.GetYaxis().SetTitle('Counts')
hLiqSciS1TimeDiff.SetStats(1)

hLiqSciS1TimeDiff.Draw()
c3.Update()


c4 = Canvas()

hLiqSciS1TimeDiff = Hist(liqsciS1TimeDiffBins, liqsciS1TimeDiffMin, liqsciS1TimeDiffMax, name='hLiqSciS1TimeDiffRightEdge', title='Time Between Liqsci and S1 Spectrum'+sForHistograms, drawstyle='hist')
current_analysis.Draw('LiqSciPeak[%d] - S1sRightEdge[0]' % (liqSciIndex), hist=hLiqSciS1TimeDiff, selection=current_analysis.get_cuts())
hLiqSciS1TimeDiff.GetXaxis().SetTitle('LiqSciPeak[] - S1sRightEdge[0] [samples]')
hLiqSciS1TimeDiff.GetYaxis().SetTitle('Counts')
hLiqSciS1TimeDiff.SetStats(1)

hLiqSciS1TimeDiff.Draw()
c4.Update()


raw_input('Press enter to continue..')


current_analysis.add_cut('(LiqSciPeak[%d] - S1sPeak[0]) > 8 && (LiqSciPeak[] - S1sPeak[0]) < 14' % (liqSciIndex))
current_analysis.get_lT1()[0].Scan('EventId:S1IntegralBeforeLiqSci[0]:LiqSciPeak[]:', current_analysis.get_cuts())







"""
# region 1
current_analysis.add_cut('(LiqSciPeak[] - S1sPeak[0]) > -80 && (LiqSciPeak[] - S1sPeak[0]) < -40')

# region 2
#current_analysis.add_cut('(LiqSciPeak[] - S1sPeak[0])/100. > 40 && (LiqSciPeak[] - S1sPeak[0])/100. < 80')



current_analysis.get_lT1()[0].Scan('EventId:S1IntegralBeforeLiqSci[0]:LiqSciPeak[]:', current_analysis.get_cuts())
"""



