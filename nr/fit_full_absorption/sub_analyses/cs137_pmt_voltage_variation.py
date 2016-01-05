import ROOT as root
from rootpy.plotting import Hist, Canvas
from math import log, exp
import array

def gain_from_mpe(voltage):
	power = 11.369
	refVoltage = 800.
	speRefGain = 1.4e6
	return speRefGain*(voltage/refVoltage)**power

lLimits = [500, 590]
g1 = 0.11

lVoltages = []

#lVoltages[i] = [voltage, g2, g2_err, s2_mean, s2_err, s1_mean, s1_err]
# with R < 0.85

#lVoltages.append([470, 641.8682433082823, 2.177408718868378, 123725.98373320028, 547.2990050707522])
lVoltages.append([485, 855.1337377815175, 2.7903394593442385, 192431.17063188076, 749.7979318294529])
lVoltages.append([500, 1206.2382454896242, 4.032566826740435, 295134.46830213274, 822.2406133736367])
lVoltages.append([515, 1664.3187138683502, 4.925795479050976, 419563.6081928968, 1154.6635002342518])
lVoltages.append([530, 2286.3229539258627, 7.721902318629873, 596445.2424769227, 1618.4324213283835])
lVoltages.append([545, 3143.0438158264856, 9.87293761463252, 841009.0633707852, 1913.430473406217])
lVoltages.append([560, 4231.423542825646, 14.925162420185188, 1161271.3001860054, 2729.543820014922])
lVoltages.append([575, 5655.05494002603, 30.022165236618548, 1594418.150913382, 4821.0280952650355])
lVoltages.append([590, 7652.4272448530755, 54.59244174620744, 2131726.092058486, 6592.91968381824])





voltageUsedForNormalizationS1 = 485
voltageUsedForNormalizationS2 = 485
voltageUsedDuringAC = 530

for vSet in lVoltages:
	if vSet[0] == voltageUsedForNormalizationS1:
		valueToNormalizeS1 = vSet[1]
	if vSet[0] == voltageUsedForNormalizationS2:
		valueToNormalizeS2 = vSet[3]

processorGain = 1.18e4

systematicUncertaintyAdded = 0.05

aVoltage = array.array('f')
aVoltageErr = array.array('f')
aS1 = array.array('f')
aS1Err = array.array('f')
aS2 = array.array('f')
aS2Err = array.array('f')

aLinearity = array.array('f')
aLinearityErr = array.array('f')
aCharge = array.array('f')
aChargeErr = array.array('f')


# lowest voltage is last element so use -1
# these are the "expected" signals
# S2 is questionable because it is so large but it still should
# be very far from saturation
numPES1 = valueToNormalizeS1 * processorGain / gain_from_mpe(voltageUsedForNormalizationS1)
numPES2 = valueToNormalizeS2 * processorGain / gain_from_mpe(voltageUsedForNormalizationS2)


for i, lPeak in enumerate(lVoltages):
	aVoltage.append(lPeak[0])
	aVoltageErr.append(0)
	aS1.append(lPeak[1]/valueToNormalizeS1)
	aS2.append(lPeak[3]/valueToNormalizeS2)
	
	if lPeak[0] == voltageUsedForNormalizationS1:
		aS1Err.append(0)
		aS2Err.append(0)
	else:
		aS1Err.append((lPeak[2] + systematicUncertaintyAdded*2**0.5*lPeak[1])/valueToNormalizeS1)
		aS2Err.append((lPeak[4] + systematicUncertaintyAdded*2**0.5*lPeak[3])/valueToNormalizeS2)

	gainCorrection = processorGain / gain_from_mpe(lPeak[0])
	
	# S1 Charge and Linearity
	aCharge.append(lPeak[1] * processorGain)
	aChargeErr.append((lPeak[1] * processorGain)*systematicUncertaintyAdded)
	linearity = lPeak[1]*gainCorrection/numPES1
	aLinearity.append(linearity)
	aLinearityErr.append(linearity*systematicUncertaintyAdded)
	

	# repeat for S2
	aCharge.append(lPeak[3] * processorGain)
	aChargeErr.append((lPeak[3] * processorGain)*systematicUncertaintyAdded)
	linearity = lPeak[3]*gainCorrection/numPES2
	aLinearity.append(linearity)
	aLinearityErr.append(linearity*systematicUncertaintyAdded)
	
	if lPeak[0] == voltageUsedForNormalizationS1:
		xNormalizationVoltageS1 = aCharge[-2]
	
	if lPeak[0] == voltageUsedForNormalizationS2:
		xNormalizationVoltageS2 = aCharge[-1]
	
	if lPeak[0] == 530:
		xACVoltageS1 = aCharge[-2]
		xACVoltageS2 = aCharge[-1]

	print 'Uncertainty needs to be fixed!!!'






c1 = Canvas()
c1.Divide(2)

c1.cd(1).SetLogy()
gS1 = root.TGraphErrors(len(lVoltages), aVoltage, aS1, aVoltageErr, aS1Err)
gS1.SetTitle('PMT 5 Power Law - S1')
gS1.GetXaxis().SetTitle('PMT 5 Voltage [V]')
gS1.GetYaxis().SetTitle('S1 Relative to 662keV Peak at V5=530V, Vc=0.345kV [PE]')
gS1.GetYaxis().SetTitleOffset(1.3)
gS1.Draw('AP')


fS1 = root.TF1('fS1', '(x/%f)^[0]' % voltageUsedForNormalizationS1, lLimits[0], lLimits[1])
gS1.Fit('fS1')
fS1.Draw('same')

sFitInfo1 = '#frac{S1 at V_{PMT 5}}{S1 at V_{PMT 5}=%.0f V} = #left(#frac{V_{PMT 5}}{%.0f V}#right)^{%.2f #pm %.2f}' % (voltageUsedForNormalizationS1, voltageUsedForNormalizationS1, fS1.GetParameter(0), fS1.GetParError(0))
pt1 = root.TPaveText(.1,.6,.7,.95,'blNDC')
text1 = pt1.AddText(sFitInfo1)
pt1.SetTextColor(root.kBlack)
pt1.SetFillStyle(0)
pt1.SetBorderSize(0)
pt1.Draw('same')


c1.Update()






c1.cd(2).SetLogy()
gS2 = root.TGraphErrors(len(lVoltages), aVoltage, aS2, aVoltageErr, aS2Err)
gS2.SetTitle('PMT 5 Power Law - S2')
gS2.GetXaxis().SetTitle('PMT 5 Voltage [V]')
gS2.GetYaxis().SetTitle('S2 Relative to 662keV Peak at V5=530V, Vc=0.345kV [PE]')
gS2.GetYaxis().SetTitleOffset(1.3)
gS2.Draw('AP')

fS2 = root.TF1('fS2', '(x/%f)^[0]' % voltageUsedForNormalizationS2, lLimits[0], lLimits[1])
gS2.Fit('fS2')
fS2.Draw('same')

sFitInfo2 = '#frac{S2 at V_{PMT 5}}{S2 at V_{PMT 5}=%.0f V} = #left(#frac{V_{PMT 5}}{%.0f V}#right)^{%.2f #pm %.2f}' % (voltageUsedForNormalizationS2, voltageUsedForNormalizationS2, fS2.GetParameter(0), fS2.GetParError(0))
pt2 = root.TPaveText(.1,.6,.7,.95,'blNDC')
text2 = pt2.AddText(sFitInfo2)
pt2.SetTextColor(root.kBlack)
pt2.SetFillStyle(0)
pt2.SetBorderSize(0)
pt2.Draw('same')

c1.Update()


c2 = Canvas()

gLinearity = root.TGraphErrors(len(lVoltages)*2, aCharge, aLinearity, aChargeErr, aLinearityErr)
gLinearity.SetTitle('PMT 5 Linearity')
gLinearity.GetXaxis().SetTitle('Charge = Integrated Signal * Gain Used in Processor')
gLinearity.GetYaxis().SetTitle('Linearity = #frac{Measured Signal}{Expected Signal from Lowest Voltage}')
gLinearity.GetYaxis().SetTitleOffset(1.3)
gLinearity.Draw('AP')

# find y max and min of graph
yMin = gLinearity.GetYaxis().GetXmin()
yMax = gLinearity.GetYaxis().GetXmax()

# find x max and min for each signal type
lChargesS1 = aCharge[0::2]
lChargesS2 = aCharge[1::2]

xMinS1 = gLinearity.GetXaxis().GetXmin()
xMaxS1 = max(lChargesS1) * 1.1
xMinS2 = min(lChargesS2) * 0.9
xMaxS2 = gLinearity.GetXaxis().GetXmax()


bS1 = root.TBox(xMinS1, yMin, xMaxS1, yMax)
bS1.SetFillColor(root.kBlue)
bS1.SetFillStyle(3004)
bS1.Draw('same')


bS2 = root.TBox(xMinS2, yMin, xMaxS2, yMax)
bS2.SetFillColor(root.kRed)
bS2.SetFillStyle(3004)
bS2.Draw('same')

lineRelativeS1 = root.TLine(xNormalizationVoltageS1, yMin, xNormalizationVoltageS1, yMax)
lineRelativeS1.SetLineColor(root.kOrange)
lineRelativeS1.SetLineStyle(2)
lineRelativeS1.SetLineWidth(3)
lineRelativeS1.Draw('same')
lineRelativeS2 = root.TLine(xNormalizationVoltageS2, yMin, xNormalizationVoltageS2, yMax)
lineRelativeS2.SetLineColor(root.kOrange)
lineRelativeS2.SetLineStyle(2)
lineRelativeS2.SetLineWidth(3)
lineRelativeS2.Draw('same')

lineRelativeX = root.TLine(xMinS1, 1., xMaxS2, 1.)
lineRelativeX.SetLineColor(root.kOrange)
lineRelativeX.SetLineStyle(2)
lineRelativeX.SetLineWidth(3)
lineRelativeX.Draw('same')

lineACVoltageS1 = root.TLine(xACVoltageS1, yMin, xACVoltageS1, yMax)
lineACVoltageS1.SetLineColor(root.kGreen)
lineACVoltageS1.SetLineStyle(2)
lineACVoltageS1.SetLineWidth(3)
lineACVoltageS1.Draw('same')
lineACVoltageS2 = root.TLine(xACVoltageS2, yMin, xACVoltageS2, yMax)
lineACVoltageS2.SetLineColor(root.kGreen)
lineACVoltageS2.SetLineStyle(2)
lineACVoltageS2.SetLineWidth(3)
lineACVoltageS2.Draw('same')

legendLinearity = root.TLegend(0.35, 0.5, 0.63, 0.9)
legendLinearity.AddEntry(bS1, 'S1 of 662 keV', 'f')
legendLinearity.AddEntry(bS2, 'S2 of 662 keV', 'f')
legendLinearity.AddEntry(lineRelativeS1, 'Voltages used for "expected" signal in linearity', 'l')
legendLinearity.AddEntry(lineACVoltageS1, 'Voltages used for anti-correlation', 'l')


legendLinearity.Draw('same')


c2.SetLogx()
c2.Update()




raw_input('Press enter to continue...')
