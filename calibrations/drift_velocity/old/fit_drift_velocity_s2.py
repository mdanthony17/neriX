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

sampleNumBins = 400
sampleMin = 0.
sampleMax = 20.

parameterToDraw = '(S2sPeak[]-S2sPeak[0])/100.'

#--------------- End Parameters to Change ----------------


current_analysis = neriX_analysis.neriX_analysis(filename)

anodeVoltage = current_analysis.get_anode_setting()
cathodeVoltage = current_analysis.get_cathode_setting()
fileNameWithoutExt = current_analysis.get_filename()[:-5]

# S2 cuts
#current_analysis.add_cut('(S1sTot[0]>0 && S1sTot[0] < 5000)')
current_analysis.add_dt_cut(0, 20)
current_analysis.add_cut('(S2sTot[1]/S2sTot[0])<0.02')
#current_analysis.add_cut('NbS2Peaks < 3')
current_analysis.add_cut('S2sPeak[] != S2sPeak[0]')

# S1 cuts
#current_analysis.add_cut('S2sPeak[] < (S1sPeak[0] + 250)')
#current_analysis.add_cut('S2sPeak[] != S2sPeak[0]')
#current_analysis.add_cut('S2sLowWidth[] < 150')
#current_analysis.add_cut('(S2sTot[]/S2sTot[0])<0.02')
#current_analysis.add_xs1asym_cut()
#current_analysis.add_xs2asym_cut()
#current_analysis.add_radius_cut(0, 0.5)
#current_analysis.add_cut('NbS2Peaks > 4')


current_analysis.set_event_list()

#current_analysis.get_T1().Scan('EventId:%s:S2sLowWidth[]' % parameterToDraw, '%s > 0 && %s < 2' % (parameterToDraw, parameterToDraw))

c1 = root.TCanvas('c1','c1',640,480)

hDriftVelocity = Hist(sampleNumBins, sampleMin, sampleMax, name='hDriftVelocity', title='hDriftVelocity', drawstyle='hist')
current_analysis.Draw(parameterToDraw, hist=hDriftVelocity, selection=current_analysis.get_cuts())
hDriftVelocity.SetTitle('Drift Velocity: Anode ' + str(anodeVoltage) + ' kV, Cathode ' + str(cathodeVoltage) + ' kV')
hDriftVelocity.GetXaxis().SetTitle('Drift Time [us]')
hDriftVelocity.GetYaxis().SetTitle('Counts')
hDriftVelocity.SetStats(0)
hDriftVelocity.Draw('')


# search for peaks using TSpectrum
lPeakParameters = [[], []]
fitDistanceFromPeakGate = 1.0
fitDistanceFromPeakCathode = 1.0



spec2 = root.TSpectrum(2)
spec2.Search(hDriftVelocity, 2, 'nobackground new goff', 0.05)

bPeaks = spec2.GetPositionX()
bPeaks.SetSize(2) # two elements in buffer
lPeaks = np.array(bPeaks, 'f')
lPeaks = lPeaks[::-1]

lPeaks.sort()

#integral = hDriftVelocity.Integral(hDriftVelocity.FindBin(3), hDriftVelocity.FindBin(13))
#print '\nIntegral between 3 and 13: %d\n' % integral



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
	
	leftFitEdge = lFits[i][0].GetParameter(1) - 1.5*lFits[i][0].GetParameter(2)
	rightFitEdge = lFits[i][0].GetParameter(1) + 1.5*lFits[i][0].GetParameter(2)
	
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

drift_time = math.fabs(mean2-mean1)
drift_time_err = (mean1_e**2 + mean2_e**2)**0.5 # mean1_e + mean2_e
drift_velocity = math.fabs(23.39/(mean2-mean1))
drift_velocity_err = drift_velocity * math.fabs(drift_time_err/drift_time) #(23.39/(mean2-mean1))*(mean2_e+mean1_e)/(mean2-mean1)

sFitInfo1 = 'Mean_{1} = %.3f +/- %.3f #mus, #sigma_{1} = %.3f +/- %.3f #mus' % (mean1,mean1_e,sigma1,sigma1_e)
sFitInfo2 = 'Mean_{2} = %.3f +/- %.3f #mus, #sigma_{2} = %.3f +/- %.3f #mus' % (mean2,mean2_e,sigma2,sigma2_e)
sDtInfo = '#Delta t = %.4f +/- %.4f #mus' % (drift_time, drift_time_err)
sVdInfo = 'v_{d} = %.4f +/- %.4f mm/#mus' % (drift_velocity, drift_velocity_err)

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

dParametersToPrint = {'start_time':neriX_analysis.convert_name_to_unix_time(filename), 'run':current_analysis.get_run(), 'anode':anodeVoltage, 'cathode':cathodeVoltage, 'degree':current_analysis.get_degree_setting(), 'mean_gate':mean1, 'mean_gate_err':mean1_e, 'width_gate':sigma1, 'width_gate_err':sigma1_e, 'mean_cathode':mean2, 'mean_cathode_err':mean2_e, 'width_cathode':sigma2, 'width_cathode_err':sigma2_e, 'drift_time':drift_time, 'drift_time_err':drift_time_err, 'drift_velocity':drift_velocity, 'drift_velocity_err':drift_velocity_err}

print '\n\nPlease copy below into fits_completed.py\n'
print '# ' + str(current_analysis.get_filename_no_ext())
print 'd_electron_lifetime[\'' + str(filename) + '\'] =', dParametersToPrint
print '\n\n'


neriX_analysis.save_plot(['results', 'run_%d' % current_analysis.get_run(), current_analysis.get_filename_no_ext()], c1, 'drift_velocity_%s' % current_analysis.get_filename_no_ext())

