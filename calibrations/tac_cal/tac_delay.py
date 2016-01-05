import ROOT as root
from rootpy.plotting import Hist, Canvas
from math import log, exp
import array, sys
import numpy as np

lLimits = [0, 6000]

dTacDelays = {0:{'height':2.68665e-01, 'width':5.28788e-03},
			  8:{'height':3.06615e-01, 'width':5.19475e-03},
			  16:{'height':3.43537e-01, 'width':4.54747e-03},
			  24:{'height':3.82137e-01, 'width':5.52308e-03},
			  32:{'height':4.19476e-01, 'width':5.43046e-03},
			  64:{'height':5.32018e-01, 'width':4.95080e-03},
			  96:{'height':6.82914e-01, 'width':5.30571e-03},
			  112:{'height':7.57700e-01, 'width':4.51142e-03},
			  }


lDelays = sorted(dTacDelays.keys())
aDelays = array.array('f', lDelays)
aDelaysErr = array.array('f', [0 for i in xrange(len(lDelays))])
aHeight = array.array('f')
aHeightErr = array.array('f')

for delay in lDelays:
	height = dTacDelays[delay]['height']
	heightErr = dTacDelays[delay]['width']
	aHeight.append(height)
	aHeightErr.append(heightErr)



c1 = Canvas()

gTacHeight = root.TGraphErrors(len(lDelays), aDelays, aHeight, aDelaysErr, aHeightErr)
gTacHeight.SetTitle('')
gTacHeight.GetXaxis().SetTitle('Delay [ns]')
gTacHeight.GetXaxis().SetTitleOffset(1.12)
gTacHeight.GetXaxis().SetRangeUser(-10, 120)
gTacHeight.GetYaxis().SetTitle('TAC Height [V]')

fCal = root.TF1('fG1', 'pol1', lLimits[0], lLimits[1])
#fG1.FixParameter(0, 0)

fCal.SetLineStyle(9)
fCal.SetLineWidth(1)


gTacHeight.Draw('ap')
gTacHeight.Fit('fG1')

fCal.Draw('same')

slope = fCal.GetParameter(1)*1000
slopeErr = fCal.GetParError(1)*1000
intercept = fCal.GetParameter(0)*1000
interceptErr = fCal.GetParError(0)*1000

sFitInfo1 = 'slope = %.2f +/- %.2f [mV/ns]' % (slope, slopeErr)
sFitInfo2 = 'intercept = %.2f +/- %.2f [mV]' % (intercept, interceptErr)
pt1 = root.TPaveText(.2,.65,.6,.75,'blNDC')
text1 = pt1.AddText(sFitInfo1)
text2 = pt1.AddText(sFitInfo2)
pt1.SetTextColor(root.kBlue)
pt1.SetFillStyle(0)
pt1.SetBorderSize(0)
pt1.Draw('same')







c1.Update()


raw_input('Press enter to continue...')
