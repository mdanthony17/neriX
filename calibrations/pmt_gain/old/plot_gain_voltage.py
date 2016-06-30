import ROOT as root
from rootpy.plotting import Hist, Canvas
from math import log, exp
import array, sys
import numpy as np



dPMTVoltages = {845: 2.77e6,
				820: 1.95e6,
				800: 1.36e6,
				795: 1.26e6,
				770: 8.66e5,
				745: 5.84e5,
				700: 2.78e5,
				670: 1.47e5,
				640: 1.03e5,
				610: 5.69e4,
				590: 3.88e4,
				570: 2.64e4
				}



lVoltages = sorted(dPMTVoltages.keys())
aVoltages = array.array('f', lVoltages)
aVoltagesErr = array.array('f', [0 for i in xrange(len(lVoltages))])
aGain = array.array('f')
aGainErr = array.array('f')


for voltage in lVoltages:
	aGain.append(dPMTVoltages[voltage])
	aGainErr.append(dPMTVoltages[voltage]*0.05)



c1 = Canvas()
c1.SetLogy()

#gVoltageVsGain = root.TGraphErrors(len(lVoltages), aElectrons, aS1, aElectronsErr, aS1Err)
gVoltageVsGain = root.TGraphErrors(len(lVoltages), aVoltages, aGain, aVoltagesErr, aGainErr)
gVoltageVsGain.SetTitle('Gain vs. Voltage')
gVoltageVsGain.GetYaxis().SetTitle('Gain [e^{-}]')
gVoltageVsGain.GetXaxis().SetTitle('PMT 5 Voltage [V]')
#gVoltageVsGain.GetXaxis().SetTitleOffset(1.12)

#fG1 = root.TF1('fG1', '[0] * pow(x, [1])', lVoltages[0], lVoltages[-1])
fG1 = root.TF1('fG1', '[0] * pow(x/610., [1])', lVoltages[0], lVoltages[-1])
fG1.SetParameters(1e-31, 12.5)
fG1.FixParameter(0, 5.69e4)
#fG1.FixParameter(1, .1529)
#fG1.SetParLimits(0, 1e-33, 1e-25)
#fG1.SetParLimits(1, 1, 30)
#fG1.SetParLimits(0, 0, 2)
fG1.SetParLimits(1, 0, 20)

fG1.SetLineStyle(9)
fG1.SetLineWidth(1)



gVoltageVsGain.Draw('ap')
gVoltageVsGain.Fit('fG1', 'MELL')

fG1.Draw('same')


sFitInfo1 = 'gain = (%.4e +/- %.4e) * Voltage^{%.4f +/- %.4f}' % (fG1.GetParameter(0), fG1.GetParError(0), fG1.GetParameter(1), fG1.GetParError(1))
pt1 = root.TPaveText(.12,.65,.7,.9,'blNDC')
text1 = pt1.AddText(sFitInfo1)
pt1.SetTextColor(root.kBlue)
pt1.SetFillStyle(0)
pt1.SetBorderSize(0)
pt1.Draw('same')




c1.Update()


raw_input('Press enter to continue...')

print 'Printing canvas...'
c1.SaveAs('/Users/Matt/Desktop/power_law_pmt5.png')