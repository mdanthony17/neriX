#!/usr/bin/python

import ROOT as root
import sys, array
import neriX_config
from rootpy.io import File
from rootpy.tree import Tree, TreeModel, TreeChain
from rootpy.plotting import Hist, Hist2D, Canvas, Legend
import pickle, neriX_led_analysis
import numpy as np
from root_numpy import tree2array


if(len(sys.argv) != 4):
	print 'Usage is python model_independent_spe.py <low LED filename> <high LED filename> <pmt channel number (from 1)>'
	sys.exit(1)


filenameLow = neriX_config.pathToData + 'run_16/' + sys.argv[1]
filenameHigh = neriX_config.pathToData + 'run_16/' + sys.argv[2]

if filenameHigh[-5:] != '.root':
	filenameHigh += '.root'
if filenameLow[-5:] != '.root':
	filenameLow += '.root'

channel = int(sys.argv[3]) - 1

fLedLow = File(filenameLow)
fLedHigh = File(filenameHigh)

T0_low = fLedLow.T0
T0_high = fLedHigh.T0

low_threshold_width = 4.
high_threshold_width = 1.


#--------------- Start Parameters to Change ----------------

#constants for histogram
#parameter 1 will be on x-axis
parameterToExamine ='SingleIntegral[%d]' % (channel)
lowerBound = -2e6
upperBoundHigh = 12e6
upperBoundLow = 2e6
nBins = 200


#--------------- End Parameters to Change ----------------

#run.get_tree().Scan('EventId:TrigArea:TrigLeftEdge-S1sPeak[0]:GeHeight[0]', run.get_cuts())


a_bkg = tree2array(T0_low, [parameterToExamine])
a_sig = tree2array(T0_high, [parameterToExamine])

a_bkg = np.array(filter(lambda x: x < upperBoundLow and x > lowerBound, a_bkg[parameterToExamine]))
a_sig = np.array(filter(lambda x: x < upperBoundHigh and x > lowerBound, a_sig[parameterToExamine]))

#print len(a_bkg)
#print len(a_sig)

#c1 = root.TCanvas('c1','c1',200,10,2000,700)
#c1.Divide(2)


hLow = Hist(nBins, lowerBound, upperBoundLow, name='hLow', title='hLow', drawstyle='hist')
T0_low.Draw(parameterToExamine, hist=hLow)

#c1.cd(1)
#c1.SetGridx(1)
hLow.SetTitle('Low LED Voltage')
hLow.GetYaxis().SetTitle('Counts')
hLow.GetXaxis().SetTitle(parameterToExamine)
hLow.SetLineWidth(2)

fLow = root.TF1('fLow','gaus', lowerBound, upperBoundLow)
hLow.Fit('fLow', 'LEN')

hLow.SetStats(0)
#hLow.Draw()
#c1.Update()


low_threshold_for_poisson_mean = fLow.GetParameter(1) - low_threshold_width*fLow.GetParameter(2)
high_threshold_for_poisson_mean = fLow.GetParameter(1) - high_threshold_width*fLow.GetParameter(2)
fraction_below_threshold_bkg = np.count_nonzero((a_bkg > low_threshold_for_poisson_mean) & (a_bkg < high_threshold_for_poisson_mean)) / float(hLow.GetEntries())


hHigh = Hist(nBins, lowerBound, upperBoundHigh, name='hHigh', title='hHigh', drawstyle='hist')
T0_high.Draw(parameterToExamine, hist=hHigh)

#c1.cd(2)
#c1.SetGridx(1)
hHigh.SetTitle('High LED Voltage')
hHigh.GetYaxis().SetTitle('Counts')
hHigh.GetXaxis().SetTitle(parameterToExamine)
hHigh.SetLineWidth(2)

#hHigh.SetStats(0)
#hHigh.Draw()
#c1.Update()


fraction_below_threshold_sig = np.count_nonzero((a_sig > low_threshold_for_poisson_mean) & (a_sig < high_threshold_for_poisson_mean)) / float(hHigh.GetEntries())

print 'Fraction below threshold bkg: %.3e' % (fraction_below_threshold_bkg)
print 'Fraction below threshold sig: %.3e' % (fraction_below_threshold_sig)

mean_number_pe = -np.log(fraction_below_threshold_sig/fraction_below_threshold_bkg)
var_mean_number_pe = (np.exp(mean_number_pe) + 1 - 2*fraction_below_threshold_bkg) / (fraction_below_threshold_bkg*len(a_bkg))

print '\nEstimated mean number of PE per trigger: %.3f +/- %.3f\n' % (mean_number_pe, var_mean_number_pe)

expectation_spe = (np.mean(a_sig) - np.mean(a_bkg)) / mean_number_pe
var_spe = (np.std(a_sig)**2 - np.std(a_bkg)**2) / mean_number_pe - expectation_spe**2
var_expection_spe = (mean_number_pe*(expectation_spe**2 + var_spe) + 2*np.std(a_bkg)**2) / (len(a_bkg)*mean_number_pe**2) + (expectation_spe**2*(np.exp(mean_number_pe) + 1 - 2*fraction_below_threshold_bkg)) / (fraction_below_threshold_bkg*len(a_bkg)*mean_number_pe**2)
#var_expection_spe = (1./mean_number_pe**2.) * ( (np.std(a_sig)**2/len(a_sig)) + (np.std(a_bkg)**2/len(a_bkg)) + expectation_spe**2*(np.exp(mean_number_pe) + 1 - 2*fraction_below_threshold_bkg)/(fraction_below_threshold_bkg*len(a_bkg)) )


print '\n\nSPE Gain Estiamte: %.3e +/- %.3e\n\n' % (expectation_spe, var_expection_spe**0.5)
print '\n\nSPE Resoltuion Estimate: %.2f\n\n' % (var_spe**0.5 / expectation_spe)

raw_input('If fits okay press enter and copy and paste line below to wiki...')



#print '\n\n$ \\frac{%.2E}{\\frac{%.2E}{%.2E}} = %.2E $\n\n' % (fLow.GetParameter(1), fHigh.GetParameter(1), speGainForChannel, fLow.GetParameter(1)*speGainForChannel/fHigh.GetParameter(1))

