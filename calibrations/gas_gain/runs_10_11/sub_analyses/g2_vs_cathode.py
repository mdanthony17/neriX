import ROOT as root
from rootpy.plotting import Hist, Canvas
from math import log, exp
import array, sys
import numpy as np
sys.path.append('../')
import 

lLimits = [0, 6000]

dCathodeVoltages = {0.345:{'g2':25.3964, 'g2_err':0.0395},
					0.700:{'g2':25.4057, 'g2_err':0.0944},
					1.054:{'g2':25.1832, 'g2_err':0.0955},
					1.500:{'g2':24.6287, 'g2_err':0.0934},
					2.356:{'g2':24.7086, 'g2_err':0.0976},
					5.500:{'g2':25.5037, 'g2_err':0.0941}
				   }


lVoltages = sorted(dCathodeVoltages.keys())
aVoltages = array.array('f', lVoltages)
aVoltagesErr = array.array('f', [0 for i in xrange(len(lVoltages))])
ag2 = array.array('f')
ag2Err = array.array('f')

for voltage in lVoltages:
	g2 = dCathodeVoltages[voltage]['g2']
	g2Err = dCathodeVoltages[voltage]['g2_err']
	ag2.append(g2)
	ag2Err.append(g2Err)



c1 = Canvas()

gVoltageVsGain = root.TGraphErrors(len(lVoltages), aVoltages, ag2, aVoltagesErr, ag2Err)
gVoltageVsGain.SetTitle('')
gVoltageVsGain.GetXaxis().SetTitle('- #frac{S2 at 40 keV Peak}{g_{2}}')
gVoltageVsGain.GetXaxis().SetTitleOffset(1.12)
gVoltageVsGain.GetYaxis().SetTitle('S1 at 40 keV Peak [PE]')

fG1 = root.TF1('fG1', 'pol0', lLimits[0], lLimits[1])
#fG1.FixParameter(0, 0)

fG1.SetLineStyle(9)
fG1.SetLineWidth(1)


gVoltageVsGain.Draw('ap')
gVoltageVsGain.Fit('fG1')

fG1.Draw('same')

g1Value = fG1.GetParameter(1)
g1Err = fG1.GetParError(1)

sFitInfo1 = 'g1 = %.4f +/- %.4f' % (g1Value, g1Err)
pt1 = root.TPaveText(.2,.75,.6,.85,'blNDC')
text1 = pt1.AddText(sFitInfo1)
pt1.SetTextColor(root.kBlue)
pt1.SetFillStyle(0)
pt1.SetBorderSize(0)
pt1.Draw('same')




c1.Update()


raw_input('Press enter to continue...')
