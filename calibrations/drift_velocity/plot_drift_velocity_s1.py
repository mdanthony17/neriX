import ROOT as root
import neriX_analysis

import os, math, sys
from array import array
from rootpy.plotting import Hist
import numpy as np
import math


if(len(sys.argv) != 2):
	print 'Usage is python plot_drift_velocity.py <ROOT filename>'
	sys.exit(1)

filename = sys.argv[1]

#--------------- Start Parameters to Change ----------------

sampleNumBins = 100
sampleMin = 0.
sampleMax = 20.

parameterToDraw = '(S2sPeak[]-S1sPeak[0])/100.'

#--------------- End Parameters to Change ----------------


run = neriX_analysis.neriX_analysis(filename)

RESULTS_PATH = './results/run_' + str(run.get_run()) + '/'
anodeVoltage = run.get_anode_setting()
cathodeVoltage = run.get_cathode_setting()
fileNameWithoutExt = run.get_filename()[:-5]

# old cuts
#run.add_cut('(S1sTot[0]>0 && S1sTot[0] < 5000)')
run.add_dt_cut(0, 20)
run.add_cut('(S2sTot[1]/S2sTot[0])<0.02')
#run.add_cut('NbS2Peaks < 3')
run.add_cut('S2sPeak[] != S2sPeak[0]')


#run.add_cut('S2sPeak[] < (S1sPeak[0] + 250)')
#run.add_cut('S2sPeak[] != S2sPeak[0]')
#run.add_cut('S2sLowWidth[] < 150')
#run.add_cut('(S2sTot[]/S2sTot[0])<0.02')
#run.add_xs1asym_cut()
#run.add_xs2asym_cut()
#run.add_radius_cut(0, 0.5)
#run.add_cut('NbS2Peaks > 4')
run.set_event_list()

#run.get_T1().Scan('EventId:%s:S2sLowWidth[]' % parameterToDraw, '%s > 0 && %s < 2' % (parameterToDraw, parameterToDraw))

c1 = root.TCanvas('c1','c1',640,480)

hDriftVelocity = Hist(sampleNumBins, sampleMin, sampleMax, name='hDriftVelocity', title='hDriftVelocity', drawstyle='hist')
run.Draw(parameterToDraw, hist=hDriftVelocity, selection=run.get_cuts())
hDriftVelocity.SetTitle('Drift Velocity: Anode ' + str(anodeVoltage) + ' kV, Cathode ' + str(cathodeVoltage) + ' kV')
hDriftVelocity.GetXaxis().SetTitle('Drift Time [us]')
hDriftVelocity.GetYaxis().SetTitle('Counts')
hDriftVelocity.SetStats(0)
hDriftVelocity.Draw('')


# search for peaks using TSpectrum
lPeakParameters = [[], []]
fitDistanceFromPeakGate = .8
fitDistanceFromPeakCathode = 1.0



spec2 = root.TSpectrum(2)
spec2.Search(hDriftVelocity, 2, 'nobackground new goff', 0.05)

bPeaks = spec2.GetPositionX()
bPeaks.SetSize(2) # two elements in buffer
lPeaks = np.array(bPeaks, 'f')
lPeaks = lPeaks[::-1]



integral = hDriftVelocity.Integral(hDriftVelocity.FindBin(3), hDriftVelocity.FindBin(13))
print '\nIntegral between 3 and 13: %d\n' % integral



c1.Update()

"""
print '\nNow select peaks in the spectrum:\n\n'

responseLeft = -1
responseRight = -1

while responseLeft < sampleMin or responseLeft > sampleMax:
	responseLeft = float(raw_input('Please approximate the peak from the gate (left peak): '))

while responseRight < sampleMin or responseLeft > sampleMax:
	responseRight = float(raw_input('Please approximate the peak from the cathode (right peak): '))

lPeaks = [responseLeft, responseRight]
"""
#print lPeaks
if len(lPeaks) == 2:
	for (i, lPeakToFill) in enumerate(lPeakParameters):
		if i == 1:
			leftEdge = lPeaks[i] - fitDistanceFromPeakGate
			rightEdge = lPeaks[i] + fitDistanceFromPeakGate
		else:
			leftEdge = lPeaks[i] - fitDistanceFromPeakCathode
			rightEdge = lPeaks[i] + fitDistanceFromPeakCathode
		
		if leftEdge < sampleMin:
			leftEdge = sampleMin
		if rightEdge > sampleMax:
			rightEdge = sampleMax
		
		lPeakToFill.append(leftEdge)
		lPeakToFill.append(rightEdge)
else:
	lPeakParameters = [[1., 1.], [10., 20.]]

# list of fits: lFits = [ [fTrialFirstPeak, fFirstPeak], [fTrialSecondPeak, fSecondPeak] ]
lFits = [[], []]
colors = [2, 4]
lParameters = [] # [mean, sigma]
lParametersError = []

print lPeakParameters


for i in xrange(0, 2):
	lFits[i].append(root.TF1('trialFit', 'gaus', lPeakParameters[i][0], lPeakParameters[i][1]))
	hDriftVelocity.Fit(lFits[i][0], 'NRQ+')
	
	leftFitEdge = lFits[i][0].GetParameter(1) - 1.*lFits[i][0].GetParameter(2)
	rightFitEdge = lFits[i][0].GetParameter(1) + 1.*lFits[i][0].GetParameter(2)
	
	if leftFitEdge < sampleMin:
		leftFitEdge = sampleMin
	if rightFitEdge > sampleMax:
		rightFitEdge = sampleMax


	lFits[i].append(root.TF1('realFit', 'gaus', leftFitEdge, rightFitEdge))
	lFits[i][1].SetLineColor(colors[i])
	#lFits[i][1].SetLineWidth(1.5)
	hDriftVelocity.Fit(lFits[i][1], 'RQ+')
	lFits[i][1].Draw('same')
	
	lParameters.append([lFits[i][1].GetParameter(1), lFits[i][1].GetParameter(2)])
	lParametersError.append([lFits[i][1].GetParError(1), lFits[i][1].GetParError(2)])


	
mean1 = lParameters[0][0]
mean1_e = lParametersError[0][0]
sigma1 = lParameters[0][1]
sigma1_e = lParametersError[0][1]
mean2 = lParameters[1][0]
mean2_e = lParametersError[1][0]
sigma2 = lParameters[1][1]
sigma2_e = lParametersError[1][1]

sFitInfo1 = 'Mean_{1} = %.3f +/- %.3f #mus, #sigma_{1} = %.3f +/- %.3f #mus' % (mean1,mean1_e,sigma1,sigma1_e)
sFitInfo2 = 'Mean_{2} = %.3f +/- %.3f #mus, #sigma_{2} = %.3f +/- %.3f #mus' % (mean2,mean2_e,sigma2,sigma2_e)
sDtInfo = '#Delta t = %.3f +/- %.3f #mus' % (math.fabs(mean2-mean1),(mean2_e+mean1_e))
sVdInfo = 'v_{d} = %.3f +/- %.3f mm/#mus' % (math.fabs(23.39/(mean2-mean1)), (23.39/(mean2-mean1))*(mean2_e+mean1_e)/(mean2-mean1))

pt1 = root.TPaveText(.3,.8,.9,.85,'blNDC')
text1 = pt1.AddText(sFitInfo1)
pt1.SetTextColor(2)
pt1.SetFillStyle(0)
pt1.SetBorderSize(0)
pt1.Draw('same')
c1.Modified()
	
pt2 = root.TPaveText(.3,.75,.9,.8,'blNDC')
text2 = pt2.AddText(sFitInfo2)
pt2.SetTextColor(4)
pt2.SetFillStyle(0)
pt2.SetBorderSize(0)
pt2.Draw('same')
c1.Modified()

pt3 = root.TPaveText(.3,.7,.9,.75,'blNDC')
text3 = pt3.AddText(sDtInfo)
pt3.SetTextColor(1)
pt3.SetFillStyle(0)
pt3.SetBorderSize(0)
pt3.Draw('same')
c1.Modified()

pt4 = root.TPaveText(.3,.65,.9,.7,'blNDC')
text4 = pt4.AddText(sVdInfo)
pt4.SetTextColor(1)
pt4.SetFillStyle(0)
pt4.SetBorderSize(0)
pt4.Draw('same')
c1.Modified()

c1.Update()

raw_input('Please press enter to continue...')

c1.Print(RESULTS_PATH + 'vd_' + fileNameWithoutExt + '.png')

		