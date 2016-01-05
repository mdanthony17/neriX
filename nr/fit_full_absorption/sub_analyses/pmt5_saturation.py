import ROOT as root
from rootpy.plotting import Hist, Canvas
from math import log, exp
import array

lLimits = [725, 850]

lVoltages = []
"""
lVoltages.append([750, 7423.812583610489, 47.834987411826205])
lVoltages.append([770, 8661.29668224795, 30.85639405978509])
lVoltages.append([790, 10505.581639504415, 75.032951468188])
lVoltages.append([800, 11391.961056584845, 63.08456687299076])
lVoltages.append([810, 12736.354779670854, 139.7178705287597])
lVoltages.append([820, 13630.57064715855, 82.29428057574205])
lVoltages.append([830, 14715.74861182745, 62.000794843135736])
lVoltages.append([840, 13630.57064715855, 82.29428057574205])
"""
lVoltages.append([750, 11683.592744556008, 126.3062940796408])
lVoltages.append([770, 16200.234572354493, 92.5069063001456])
lVoltages.append([790, 22052.353921540962, 252.08281015325338])
lVoltages.append([800, 25598.04042809551, 242.28754583025147])
lVoltages.append([810, 30662.112203993747, 310.6023214010529])
lVoltages.append([820, 34345.60786888149, 434.1906932733])
lVoltages.append([830, 40074.83137666833, 475.85940804779966])
lVoltages.append([840, 50634.7470326234, 346.5037355190143])


aVoltage = array.array('f')
aVoltageErr = array.array('f')
aLogGain = array.array('f')
aLogGainErr = array.array('f')

for i, lGain in enumerate(lVoltages):
	aVoltage.append(lGain[0])
	aVoltageErr.append(0)
	aLogGain.append(log(lGain[1]))
	aLogGainErr.append(lGain[2]/lGain[1])

c1 = Canvas()

gVoltageVsGain = root.TGraphErrors(len(lVoltages), aVoltage, aLogGain, aVoltageErr, aLogGainErr)
gVoltageVsGain.SetTitle('PMT 5 Saturation - 40 keV')
gVoltageVsGain.GetXaxis().SetTitle('PMT 5 Voltage [V]')
gVoltageVsGain.GetYaxis().SetTitle('Log(40 keV S2 peak)')

fSigmoid = root.TF1('fSigmoid', '[0] / (1 + exp(([1]-x)))^([2])', lLimits[0], lLimits[1])
fSigmoid.SetParameters(9, 0.5, 740, 2e-4)
fSigmoid.SetParLimits(0, 9, 15)
fSigmoid.SetParLimits(1, 700, 850)
fSigmoid.SetParLimits(2, 1e-4, 7e-3)
#fSigmoid.FixParameter(0, 9.55)
#fSigmoid.FixParameter(1, 0.03)
#fSigmoid.FixParameter(3, 1)
fSigmoid.SetLineStyle(9)
fSigmoid.SetLineWidth(1)



gVoltageVsGain.Draw('ap')
gVoltageVsGain.Fit('fSigmoid', 'LL', '', lLimits[0], lLimits[1])

sigmoidMax = float(fSigmoid.GetParameter(0))
fMaxLine = root.TF1('fMaxLine', 'pol0', lLimits[0], lLimits[1])
fMaxLine.SetParameter(0, sigmoidMax)
fMaxLine.SetLineColor(root.kBlue)
fMaxLine.SetLineStyle(7)
fMaxLine.SetLineWidth(1)

fSigmoid.Draw('same')
fMaxLine.Draw('same')
c1.Update()


raw_input('Press enter to continue...')
