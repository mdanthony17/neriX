import ROOT as root
from rootpy.plotting import Hist, Canvas
from math import log, exp
import array

lLimits = [3400, 5000]
g1 = 0.11

s1Mean = 259.765797552439
s1MeanErr = 8.297425307457331

recoilEnergy = 40000
wValue = 13.7

lVoltages = []
#lVoltages[i] = [voltage, g2, g2_err, s2_mean, s2_err, s1_mean, s1_err]
# with R < 0.7
"""
lVoltages.append([3500, 17.5345, 0.0795, 14548.319566802189, 502.3607425056298])
lVoltages.append([3700, 19.0962, 0.0761, 17098.705771614077, 287.5189630637724])
lVoltages.append([3900, 20.6182, 0.0778, 19843.281962811707, 817.8483042876414])
lVoltages.append([4100, 21.9425, 0.0725, 22102.541543597792, 691.3277442940998])
lVoltages.append([4300, 23.5606, 0.0872, 24733.970417942306, 328.4519458027171])
lVoltages.append([4500, 25.1832, 0.0955, 27210.714302912973, 290.2693805242634])
lVoltages.append([4500, 25.0004, 0.1117, 27006.72358232474, 543.8995418079867])
lVoltages.append([4600, 25.9771, 0.0896, 27168.548428118593, 601.7380308745232])
lVoltages.append([4700, 26.8092, 1.0299, 28970.474826112077, 424.3099894726183])
lVoltages.append([4800, 27.7353, 0.1142, 30766.23941865505, 1620.3644519754598])
"""
# no radial cut
"""
lVoltages.append([3500, 17.5345, 0.0795, 16104.565444206955, 162.62837169111754])
lVoltages.append([3700, 19.0962, 0.0761, 18166.45433686785, 182.54385620890207])
lVoltages.append([3900, 20.6182, 0.0778, 20294.423174537955, 221.63235418059776])
lVoltages.append([4100, 21.9425, 0.0725, 23091.842041594107, 250.8397049535106])
lVoltages.append([4300, 23.5606, 0.0872, 25370.06955961084, 264.4014119428739])
lVoltages.append([4500, 25.1832, 0.0955, 28100.510413677737, 387.05344078950475])
lVoltages.append([4500, 25.0004, 0.1117, 27485.236919426046, 384.9673022463976])
lVoltages.append([4600, 25.9771, 0.0896, 29166.941764267438, 387.94824091211376])
lVoltages.append([4700, 26.8092, 1.0299, 30781.927880398296, 446.2097542838037])
lVoltages.append([4800, 27.7353, 0.1142, 30995.620891254544, 371.5748391500056])
"""

#lVoltages[i] = [voltage, g2, g2_err, s2_mean, s2_err, s1_mean, s1_err]
# with R < 0.7

lVoltages.append([3700, 23.3039, 0.5113, 13154.073753328097, 186.7717928209786])
lVoltages.append([3900, 26.3724, 0.6933, 16964.792919528296, 196.83950947217818])
lVoltages.append([4100, 29.8313, 0.4551, 20488.00661327579, 197.35799922627666])
lVoltages.append([4300, 32.4923, 0.0017, 25113.99720701623, 265.27682943524815])
lVoltages.append([4500, 33.8670, 0.2721, 28186.28858200408, 146.86123662000864])
lVoltages.append([4700, 34.9010, 0.1919, 32276.008538995873, 288.2074339413193])
lVoltages.append([4900, 36.3926, 0.0412, 36047.00000159548, 471.3470374136523])

gainCorrectionFactor = 1.48/1.36

aVoltage = array.array('f')
aVoltageErr = array.array('f')
aExtraction = array.array('f')
aExtractionErr = array.array('f')

for i, lGain in enumerate(lVoltages):
	aVoltage.append(lGain[0])
	aVoltageErr.append(0)
	#print lGain[3]/lGain[1]
	#print recoilEnergy/wValue - s1Mean/g1 - lGain[3]/lGain[1]
	#print recoilEnergy/wValue
	#print s1Mean/g1
	#print lGain[3]/lGain[1]
	aExtraction.append(recoilEnergy/wValue - s1Mean/g1 - lGain[3]/lGain[1])
	#aExtractionErr.append((lGain[3]/lGain[1]) * ((lGain[4]/lGain[3])**2+(lGain[2]/lGain[1])**2)**0.5)
	aExtractionErr.append(0)

c1 = Canvas()

gVoltageVsGain = root.TGraphErrors(len(lVoltages), aVoltage, aExtraction, aVoltageErr, aExtractionErr)
gVoltageVsGain.SetTitle('Extraction Efficiency - run_15, Feedthrough=0.2635"')
gVoltageVsGain.GetXaxis().SetTitle('Anode Voltage [V]')
gVoltageVsGain.GetYaxis().SetTitle('S2 Peak / g2 [electrons]')
gVoltageVsGain.GetYaxis().SetTitleOffset(1.3)

"""
fSigmoid = root.TF1('fSigmoid', '[0] / (1 + exp([2]*([1]-x)))^[3]', lLimits[0], lLimits[1])
fSigmoid.SetParameters(1050, 0.5, 740, 0.1)
fSigmoid.SetParLimits(0, 900, 1200)
fSigmoid.SetParLimits(1, 2000, 5000)
fSigmoid.SetParLimits(2, 1e-3, 1e-2)
fSigmoid.SetParLimits(3, 0.01, 0.5)
#fSigmoid.FixParameter(0, 1050)
#fSigmoid.FixParameter(1, 0.03)
#fSigmoid.FixParameter(3, .1)
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
"""
operatingVoltage = 4500.
lOV = root.TLine()
lOV.SetLineColor(root.kOrange + 1)
lOV.SetLineStyle(7)
#print gVoltageVsGain.GetYaxis().GetXmin(),
#lOV.DrawLine(operatingVoltage, gVoltageVsGain.GetYaxis().GetXmin(), operatingVoltage, sigmoidMax)

gVoltageVsGain.Draw('ap')
#fSigmoid.Draw('same')
#fMaxLine.Draw('same')
#lOV.Draw('same')
c1.Update()


raw_input('Press enter to continue...')
