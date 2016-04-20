import ROOT as root
import neriX_analysis
from rootpy.plotting import Hist, Canvas
from math import log, exp
import array, sys
import numpy as np
sys.path.append('../')
#import fits_completed
#import fits_completed_co57
import fits_completed_cs137
import fits_completed_na22
import fits_completed_164keV_peak
#import fits_completed_197keV_peak

wValue = 13.7 # eV

lLimits = [-1500, -700]

currentRun = 15

dCathodeVoltages = {0.345:{'s1_mean':[], 's1_mean_err':[], 's2_mean':[], 's2_mean_err':[]},
					0.700:{'s1_mean':[], 's1_mean_err':[], 's2_mean':[], 's2_mean_err':[]},
					1.054:{'s1_mean':[], 's1_mean_err':[], 's2_mean':[], 's2_mean_err':[]},
					1.500:{'s1_mean':[], 's1_mean_err':[], 's2_mean':[], 's2_mean_err':[]},
					2.356:{'s1_mean':[], 's1_mean_err':[], 's2_mean':[], 's2_mean_err':[]},
					#5.500:{'s1_mean':[], 's1_mean_err':[], 's2_mean':[], 's2_mean_err':[]}
				   }


gainAdjustmentTerm = 1.#/1.034

#dFitParameters = fits_completed.dFitParameters
#energyOfPeak = 40e3 # eV
#dFitParameters = fits_completed_co57.dFitParameters
#energyOfPeak = 123e3
#dFitParameters = fits_completed_197keV_peak.dFitParameters
#energyOfPeak = 197.1e3 # eV
#dFitParameters = fits_completed_164keV_peak.dFitParameters
#energyOfPeak = 163.9e3 # eV
#dFitParameters = fits_completed_cs137.dFitParameters
#energyOfPeak = 662e3
dFitParameters = fits_completed_na22.dFitParameters
energyOfPeak = 511e3

lowestCathodeSettingFilename = ''

print 'Peak being examined: ' + str(energyOfPeak)

# gather paramters of interest from fits
for file in dFitParameters:
	dFile = dFitParameters[file]
	if dFile['run'] != currentRun or dFile['anode'] != 4.5:
		continue

	if dFile['cathode'] in dCathodeVoltages:
		if dFile['cathode']  == 0.345:
			lowestCathodeSettingFilename = file[:-5]
		for key in dCathodeVoltages[dFile['cathode']]:
			if key in dFile:
				dCathodeVoltages[dFile['cathode']][key].append(dFile[key])



lVoltages = sorted(dCathodeVoltages.keys())
aS1 = array.array('f')
aS1Err = array.array('f')
aElectrons = array.array('f')
aElectronsErr = array.array('f')
aS2 = array.array('f')
aS2Err = array.array('f')

for voltage in lVoltages:
	aS1.append(np.mean(dCathodeVoltages[voltage]['s1_mean'])*gainAdjustmentTerm)
	propS1Err = 1./len(dCathodeVoltages[voltage]['s1_mean_err']) * sum([x**2 for x in dCathodeVoltages[voltage]['s1_mean_err']])**0.5
	aS1Err.append(propS1Err)
	#aS1Err.append(np.mean(dCathodeVoltages[voltage]['s1_mean_err']))
	
	#print voltage, dCathodeVoltages[voltage]['s1_mean']
	
	avgS2 = np.mean(dCathodeVoltages[voltage]['s2_mean'])*gainAdjustmentTerm
	avgS2Err = np.mean(dCathodeVoltages[voltage]['s2_mean_err']) # incorrect!!!
	#g2 = dCathodeVoltages[voltage]['g2']
	#g2Err = dCathodeVoltages[voltage]['g2_err']

	aS2.append(avgS2)
	aS2Err.append(avgS2Err)

	#aElectrons.append(-avgS2/g2)
	#aElectronsErr.append(avgS2/g2 * ((avgS2Err/avgS2)**2 + (g2Err/g2)**2)**0.5)
	#aElectronsErr.append(0)



c1 = Canvas()

#gVoltageVsGain = root.TGraphErrors(len(lVoltages), aElectrons, aS1, aElectronsErr, aS1Err)
gVoltageVsGain = root.TGraphErrors(len(lVoltages), aS2, aS1, aS2Err, aS1Err)
gVoltageVsGain.SetTitle('')
gVoltageVsGain.GetYaxis().SetTitle('S1 at ' + str(int(energyOfPeak/1000)) + ' keV Peak [PE]')
#gVoltageVsGain.GetXaxis().SetTitle('- #frac{S2 at 40 keV Peak}{g_{2}}')
gVoltageVsGain.GetXaxis().SetTitle('S2 at ' + str(int(energyOfPeak/1000)) + ' keV Peak')
gVoltageVsGain.GetXaxis().SetTitleOffset(1.12)

fG1 = root.TF1('fG1', 'pol1', lLimits[0], lLimits[1])
fG1.SetParameters(1500, -0.1)
#fG1.FixParameter(0, 1233)
#fG1.FixParameter(1, .1529)
fG1.SetParLimits(0, 0, 100000)
fG1.SetParLimits(1, -0.2, 0.2)

fG1.SetLineStyle(9)
fG1.SetLineWidth(1)



gVoltageVsGain.Draw('ap')
gVoltageVsGain.Fit('fG1', 'fMELL')

#fG1.Draw('same')

wErr = 0.6
#g1Value = fG1.GetParameter(1)
#g1Err = fG1.GetParError(1)
g1Value = fG1.GetParameter(0)*wValue/energyOfPeak
g1Err = g1Value * ((fG1.GetParError(0)/fG1.GetParameter(0))**2 + (wErr/wValue)**2)**0.5
g2Value = (-1./fG1.GetParameter(1))*g1Value
g2Err = g2Value * ((fG1.GetParError(1)/fG1.GetParameter(1))**2 + (g1Err/g1Value)**2)**0.5
print wValue
print energyOfPeak
print 'g2 = ' + str((-1./fG1.GetParameter(1))*g1Value)

sFitInfo1 = 'g1 = %.4f +/- %.4f' % (g1Value, g1Err)
pt1 = root.TPaveText(.2,.75,.6,.85,'blNDC')
text1 = pt1.AddText(sFitInfo1)
pt1.SetTextColor(root.kBlue)
pt1.SetFillStyle(0)
pt1.SetBorderSize(0)
pt1.Draw('same')

sFitInfo2 = 'g2 = %.4f +/- %.4f' % (g2Value, g2Err)
pt2 = root.TPaveText(.2,.65,.6,.75,'blNDC')
text2 = pt1.AddText(sFitInfo2)
pt2.SetTextColor(root.kBlue)
pt2.SetFillStyle(0)
pt2.SetBorderSize(0)
pt2.Draw('same')



# --------------------------------
# NEST Comparisons
# --------------------------------

# the following are the NEST values at 210, 500, and 1000 V/cm
# given (photons, electrons)
dNestComparison = {662e3:{0.345:(25909, 22528.58301, 33, 9),
					      1.054:(19548.3465, 28889.88634, 22, 9),
						  2.356:(14378.94726, 34059.28558, 23, 9)},
				   511e3:{0.345:(20426.71554, 16962.91737, 33, 9),
					      1.054:(15654.07194, 21735.56096, 22, 9),
						  2.356:(11652.56428, 25737.06863, 23, 9)},
				   163.9e3:{0.345:(7149.08086, 4843.406092, 33, 9),
						    1.054:(5856.81485, 6135.672102, 22, 9),
							2.356:(4634.884471, 7357.602481, 23, 9)}
				  }

dNestMarkers = {}
nestS1s = []
nestS2s = []
if energyOfPeak in dNestComparison:
	for voltage in dNestComparison[energyOfPeak]:
		# don't draw if not looking at that field
		if not (voltage in dCathodeVoltages):
			continue

		nestS1 = dNestComparison[energyOfPeak][voltage][0]*g1Value
		nestS2 = dNestComparison[energyOfPeak][voltage][1]*g2Value
		markerStyle = dNestComparison[energyOfPeak][voltage][2]

		nestS1s.append(nestS1)
		nestS2s.append(nestS2)
		#print nestS1, nestS2

		dNestMarkers[voltage] = root.TMarker(nestS2, nestS1, markerStyle)
		dNestMarkers[voltage].SetMarkerColor(dNestComparison[energyOfPeak][voltage][3])
		dNestMarkers[voltage].SetMarkerSize(1.5)
		#print dNestMarkers
		dNestMarkers[voltage].Draw('same')

# append axis limits nest lists to draw graph in proper range
nestS1s.append(gVoltageVsGain.GetYaxis().GetXmin())
nestS1s.append(gVoltageVsGain.GetYaxis().GetXmax())

nestS2s.append(gVoltageVsGain.GetXaxis().GetXmin())
nestS2s.append(gVoltageVsGain.GetXaxis().GetXmax())

xRangeMin = min(nestS2s)*0.9
xRangeMax = max(nestS2s)*1.1
yRangeMin = min(nestS1s)*0.9
yRangeMax = max(nestS1s)*1.1

#print xRangeMin, xRangeMax, yRangeMin, yRangeMax

gVoltageVsGain.GetXaxis().SetLimits(xRangeMin, xRangeMax)
gVoltageVsGain.GetHistogram().SetMinimum(yRangeMin)
gVoltageVsGain.GetHistogram().SetMaximum(yRangeMax)

fDraw = root.TF1('fDraw', 'pol1', xRangeMin, xRangeMax)
fDraw.SetParameters(fG1.GetParameter(0), fG1.GetParameter(1))
fDraw.SetLineStyle(9)
fDraw.SetLineWidth(1)

fDraw.Draw('same')

c1.Update()

neriX_analysis.save_plot(['ac_results', 'run_%d' % (15), lowestCathodeSettingFilename], c1, 'g1_g2_anticorrelation_%s' % lowestCathodeSettingFilename)

raw_input('Press enter to continue...')
