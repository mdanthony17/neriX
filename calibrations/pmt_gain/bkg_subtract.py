#!/usr/bin/python

import ROOT as root
import sys, array
#import neriX_analysis
from rootpy.io import File
from rootpy.tree import Tree, TreeModel, TreeChain
from rootpy.plotting import Hist, Hist2D, Canvas, Legend
import pickle, neriX_led_analysis
import numpy as np
from root_numpy import tree2array


if(len(sys.argv) != 4):
	print 'Usage is python model_independent_spe.py <low LED filename> <high LED filename> <pmt channel number (from 1)>'
	sys.exit(1)


filenameLow = '../../data/run_16/' + sys.argv[1]
filenameHigh = '../../data/run_16/' + sys.argv[2]

if filenameHigh[-5:] != '.root':
	filenameHigh += '.root'
if filenameLow[-5:] != '.root':
	filenameLow += '.root'

channel = int(sys.argv[3]) - 1

fLedLow = File(filenameLow)
fLedHigh = File(filenameHigh)

T0_low = fLedLow.T0
T0_high = fLedHigh.T0


#--------------- Start Parameters to Change ----------------

#constants for histogram
#parameter 1 will be on x-axis
integral_parameter ='SingleIntegral[%d]' % (channel)
integral_lower_bound = -2e6
integral_upper_bound = 2.5e6
integral_n_bins = 100

height_parameter = 'SinglePeak[%d]' % (channel)
height_lower_bound = -2e5
height_upper_bound = 4e5
height_n_bins = 100

sum_around_peak_parameter ='SinglePeak[%d] + SingleBefore[%d][0] + SingleBefore[%d][1] + SingleAfter[%d][0] + SingleAfter[%d][1]' % (5*(channel,))
sum_around_peak_lower_bound = -1e6
sum_around_peak_upper_bound = 1e6
sum_around_peak_n_bins = 100

peak_location_parameter ='SingleSample[%d]' % (channel)
peak_location_lower_bound = 148-5
peak_location_upper_bound = 198+5
peak_location_n_bins = int(peak_location_upper_bound-peak_location_lower_bound)

integration_window_lower = 170
integration_window_upper = 176

#--------------- End Parameters to Change ----------------

#run.get_tree().Scan('EventId:TrigArea:TrigLeftEdge-S1sPeak[0]:GeHeight[0]', run.get_cuts())


a_bkg = tree2array(T0_low, [integral_parameter, peak_location_parameter])
a_sig = tree2array(T0_high, [integral_parameter, peak_location_parameter])

len_bkg = len(a_bkg)
len_sig = len(a_sig)

assert len_bkg == len_sig

#a_bkg = np.array(filter(lambda x: x < upperBoundLow and x > lowerBound, a_bkg[parameterToExamine]))
#a_sig = np.array(filter(lambda x: x < upperBoundHigh and x > lowerBound, a_sig[parameterToExamine]))



c1 = root.TCanvas('c1','c1',200,10,1000,700)

hLow = Hist(peak_location_n_bins, peak_location_lower_bound, peak_location_upper_bound, name='hLow', title='Peak Location Signal vs. Bkg')
hLow.SetMarkerSize(0)
T0_low.Draw(peak_location_parameter, hist=hLow)
hLow.GetYaxis().SetTitle('Counts')
hLow.GetXaxis().SetTitle(peak_location_parameter)
hLow.SetLineWidth(2)
hLow.SetLineColor(root.kRed)

fLow = root.TF1('fLow','pol1', peak_location_lower_bound, peak_location_upper_bound)
hLow.Fit('fLow', 'EN')

# get integral in window and uncertainty
bkg_events = fLow.Integral(integration_window_lower+1, integration_window_upper-1)
bkg_events_err = fLow.IntegralError(integration_window_lower+1, integration_window_upper-1)


hHigh = Hist(peak_location_n_bins, peak_location_lower_bound, peak_location_upper_bound, name='hHigh', title='Peak Location Signal vs. Bkg')
hHigh.SetMarkerSize(0)
T0_high.Draw(peak_location_parameter, hist=hHigh)
hHigh.GetYaxis().SetTitle('Counts')
hHigh.GetXaxis().SetTitle(peak_location_parameter)
hHigh.SetLineWidth(2)
hHigh.SetStats(0)

hHigh.Draw()

fHigh = root.TF1('fHigh','[0] + [1]*x + [2]*exp(-0.5*((x-[3])/[4])^2)', peak_location_lower_bound, peak_location_upper_bound)

fHigh.SetParameters(1000, -0.5, 1000, 174, 5)

hHigh.Fit('fHigh', 'LE')

f_lin_sig_plus_bkg = root.TF1('fLow','pol1', peak_location_lower_bound, peak_location_upper_bound)
f_lin_sig_plus_bkg.SetParameters(fHigh.GetParameter(0), fHigh.GetParameter(1))
bkg_events_in_sig_file = f_lin_sig_plus_bkg.Integral(integration_window_lower+1, integration_window_upper-1)

# scale hLow such that bkgs match
ratio_const_sig_to_bkg = bkg_events_in_sig_file / bkg_events
hLow.Scale(ratio_const_sig_to_bkg)

fHigh.Draw('same')
hLow.Draw('same')


c1.Update()

scaled_bkg_events = bkg_events*ratio_const_sig_to_bkg
scaled_bkg_events_err = bkg_events_err*ratio_const_sig_to_bkg
print scaled_bkg_events_err
sig_plus_bkg_events = fHigh.Integral(integration_window_lower+1, integration_window_upper-1)
print scaled_bkg_events

print 'Percent of signal events with peak in reduced integration window: %0.f / %0.f' % (sig_plus_bkg_events-scaled_bkg_events, sig_plus_bkg_events)


# since our statistics are large in this window, we can
# approximate the RV for number of counts in this
# range to be N(integral, integral_err)



c5 = root.TCanvas('c5','c5',200,10,1000,700)

hBkgNoCuts = Hist(integral_n_bins, integral_lower_bound, integral_upper_bound, name='hBkgNoCuts', title='Integral Bkg, Peak in Reduced Window')
hBkgNoCuts.SetMarkerSize(0)
T0_low.Draw(integral_parameter, hist=hBkgNoCuts)
hBkgNoCuts.GetYaxis().SetTitle('Counts')
hBkgNoCuts.GetXaxis().SetTitle(integral_parameter)
hBkgNoCuts.SetLineWidth(2)

hBkgNoCuts.Draw()

# find background mean and width for convolution

fBkgTrial = root.TF1('fBkgTrial', 'gaus', height_lower_bound, height_upper_bound)
fBkgTrial.SetParameters(50000, 1e5, 1e5)
hBkgNoCuts.Fit('fBkgTrial', 'LN')

left_edge_trial = fBkgTrial.GetParameter(1) - 1*fBkgTrial.GetParameter(2)
right_edge_trial = fBkgTrial.GetParameter(1) + 1*fBkgTrial.GetParameter(2)

fBkg = root.TF1('fBkg', 'gaus', left_edge_trial, right_edge_trial)
fBkg.SetParameters(50000, 1e5, 1e5)
hBkgNoCuts.Fit('fBkg', 'RL')

mean_bkg = fBkg.GetParameter(1)
width_bkg = fBkg.GetParameter(2)




c2 = root.TCanvas('c2','c2',200,10,2000,700)
c2.Divide(2)

c2.cd(1)

hBkg = Hist(integral_n_bins, integral_lower_bound, integral_upper_bound, name='hBkg', title='Integral Bkg, Peak in Reduced Window')
hBkg.SetMarkerSize(0)
T0_low.Draw(integral_parameter, hist=hBkg, selection='%s > %d && %s < %d' % (peak_location_parameter, integration_window_lower+1, peak_location_parameter, integration_window_upper-1))
#T0_low.Draw(integral_parameter, hist=hBkg)
hBkg.GetYaxis().SetTitle('Counts')
hBkg.GetXaxis().SetTitle(integral_parameter)
hBkg.SetLineWidth(2)
hBkg.SetLineColor(root.kRed)

hBkg.Scale(ratio_const_sig_to_bkg)

hBkg.Draw()





c2.cd(2)

hSig = Hist(integral_n_bins, integral_lower_bound, integral_upper_bound, name='hSig', title='Integral Bkg, Peak in Reduced Window')
hSig.SetMarkerSize(0)
T0_high.Draw(integral_parameter, hist=hSig, selection='%s > %d && %s < %d' % (peak_location_parameter, integration_window_lower+1, peak_location_parameter, integration_window_upper-1))
#T0_high.Draw(integral_parameter, hist=hSig)
hSig.GetYaxis().SetTitle('Counts')
hSig.GetXaxis().SetTitle(peak_location_parameter)
hSig.SetLineWidth(2)

hSig.Add(hBkg, -1)

hSig.Draw()
#hBkg.Draw('same')
"""
# want to constrain bkg integral to one found above so need full form
# also need to convolve bkg into signal
fSig = root.TF1('fSig','[0] * (  [1]/(2*3.1415)^0.5/[3]*exp(-0.5*((x-[2])/[3])^2) + 1./(1+exp(-(x-[7])/[4]))*1./(2*3.1415)^0.5/([3]^2+[6]^2)^0.5*exp(-0.5*((x-[2]-[5])/([3]^2+[6]^2)^0.5)^2)  )', peak_location_lower_bound, peak_location_upper_bound)

fSig.SetParameters(1e9, 0.33, 2e5, 1e5, 1e5, 8e5, 5e5, 1e5)
#fSig.SetParLimits(1, 0, 1)
fSig.FixParameter(1, 1 - (sig_plus_bkg_events-scaled_bkg_events)/sig_plus_bkg_events)
fSig.SetParLimits(2, 0, 3e5)
fSig.SetParLimits(3, 0, 2e5)
fSig.SetParLimits(4, 0, 2e5)
fSig.SetParLimits(5, 3e5, 11e5)
fSig.SetParLimits(6, 3e5, 9e5)
fSig.SetParLimits(7, 0, 7e5)

hSig.Fit('fSig', 'LEM')
"""

c2.Update()


c3 = root.TCanvas('c3','c3',200,10,2000,700)

c3.Divide(3)

c3.cd(1)

hSinglePeakBkg = Hist(height_n_bins, height_lower_bound, height_upper_bound, name='hSinglePeakBkg', title='SinglePeak Bkg, Outside Reduced Window')
hSinglePeakBkg.SetMarkerSize(0)
T0_low.Draw(height_parameter, hist=hSinglePeakBkg, selection='!(%s > %d && %s < %d)' % (peak_location_parameter, integration_window_lower+1, peak_location_parameter, integration_window_upper-1))
hSinglePeakBkg.GetYaxis().SetTitle('Counts')
hSinglePeakBkg.GetXaxis().SetTitle(peak_location_parameter)
hSinglePeakBkg.SetLineWidth(2)
hSinglePeakBkg.SetStats(0)

hSinglePeakBkg.Draw()

fSinglePeakBkgTrial = root.TF1('fSinglePeakBkgTrial', 'gaus', height_lower_bound, height_upper_bound)
fSinglePeakBkgTrial.SetParameters(50000, 1e5, 1e5)
hSinglePeakBkg.Fit('fSinglePeakBkgTrial', 'LN')

left_edge_trial = fSinglePeakBkgTrial.GetParameter(1) - 1*fSinglePeakBkgTrial.GetParameter(2)
right_edge_trial = fSinglePeakBkgTrial.GetParameter(1) + 1*fSinglePeakBkgTrial.GetParameter(2)

fSinglePeakBkg = root.TF1('fSinglePeakBkg', 'gaus', left_edge_trial, right_edge_trial)
fSinglePeakBkg.SetParameters(50000, 1e5, 1e5)
hSinglePeakBkg.Fit('fSinglePeakBkg', 'RL')

left_edge_profile = fSinglePeakBkg.GetParameter(1) - 1.5*fSinglePeakBkg.GetParameter(2)
right_edge_profile = fSinglePeakBkg.GetParameter(1) + 1.5*fSinglePeakBkg.GetParameter(2)


c3.cd(2)

# draw height vs area to make sure the relationship is linear

hSinglePeakVsAreaBkg = Hist2D(height_n_bins, left_edge_profile, right_edge_profile, sum_around_peak_n_bins, sum_around_peak_lower_bound, sum_around_peak_upper_bound, name='hSinglePeakVsAreaBkg', title='SinglePeak vs Area Bkg, Outside Reduced Window')
T0_low.Draw('%s:%s' % (height_parameter, sum_around_peak_parameter), hist=hSinglePeakVsAreaBkg, selection='!(%s > %d && %s < %d)' % (peak_location_parameter, integration_window_lower+1, peak_location_parameter, integration_window_upper-1))
hSinglePeakVsAreaBkg.GetYaxis().SetTitle(sum_around_peak_parameter)
hSinglePeakVsAreaBkg.GetXaxis().SetTitle(peak_location_parameter)
hSinglePeakVsAreaBkg.SetLineWidth(2)
hSinglePeakVsAreaBkg.SetStats(0)

hSinglePeakVsAreaBkgProfile = hSinglePeakVsAreaBkg.ProfileX()
hSinglePeakVsAreaBkgProfile.SetStats(0)

hSinglePeakVsAreaBkgProfile.Draw()

fSinglePeakVsAreaBkg = root.TF1('fSinglePeakVsAreaBkg', 'pol1', left_edge_trial, right_edge_trial)
hSinglePeakVsAreaBkgProfile.Fit('fSinglePeakVsAreaBkg')

# finally draw "integral" spectrum for bkg file
# where peaks were outside of the window
# this essentially corresponds to the PDF
# of

c3.cd(3)

hSumFromHeight = Hist(sum_around_peak_n_bins, sum_around_peak_lower_bound, sum_around_peak_upper_bound, name='hSumFromHeight', title='Approximate area from height, Outside Reduced Window')
hSumFromHeight.SetMarkerSize(0)
T0_low.Draw('%f + %f*%s' % (fSinglePeakVsAreaBkg.GetParameter(0), fSinglePeakVsAreaBkg.GetParameter(1), height_parameter), hist=hSumFromHeight, selection='!(%s > %d && %s < %d)' % (peak_location_parameter, integration_window_lower+1, peak_location_parameter, integration_window_upper-1))
hSumFromHeight.GetYaxis().SetTitle('Counts')
hSumFromHeight.GetXaxis().SetTitle(peak_location_parameter)
hSumFromHeight.SetLineWidth(2)
hSumFromHeight.SetStats(0)

hSumFromHeight.Draw()

fSumFromHeightTrial = root.TF1('fSumFromHeightTrial', 'gaus', height_lower_bound, height_upper_bound)
fSumFromHeightTrial.SetParameters(50000, 1e5, 1e5)
hSumFromHeight.Fit('fSumFromHeightTrial', 'LN')

left_edge_trial = fSumFromHeightTrial.GetParameter(1) - 1*fSumFromHeightTrial.GetParameter(2)
right_edge_trial = fSumFromHeightTrial.GetParameter(1) + 1*fSumFromHeightTrial.GetParameter(2)

fSumFromHeight = root.TF1('fSumFromHeight', 'gaus', left_edge_trial, right_edge_trial)
fSumFromHeight.SetParameters(50000, 1e5, 1e5)
hSumFromHeight.Fit('fSumFromHeight', 'RL')

c3.Update()


# draw bkg subtracted histogram and fit to (1-CDF(bkg area dist))*gaus

c4 = root.TCanvas('c4','c4',200,10,1000,700)

hSig.Draw()

s_mean = '([1]+%f)' % mean_bkg
s_width = '([2]^2+%f^2)^0.5' % width_bkg

fSig = root.TF1('fSig','[0]*exp(-0.5*((x-%s)/%s)^2) * (0.5*(1+TMath::Erf((x-%f)/%f/2^0.5)) )' % (s_mean, s_width, fSumFromHeight.GetParameter(1), fSumFromHeight.GetParameter(2)), integral_lower_bound, integral_upper_bound)


fSig.SetParameters(500, 8e5, 5e5)
fSig.SetParLimits(0, 0, 1e4)
fSig.SetParLimits(1, 2e5, 10e5)
fSig.SetParLimits(2, 1e5, 10e5)

hSig.Fit('fSig', 'LEMN')

fBiasFunction = root.TF1('fSig','(0.5*(1+TMath::Erf((x-%f)/%f/2^0.5)) )' % (fSumFromHeight.GetParameter(1), fSumFromHeight.GetParameter(2)), integral_lower_bound, integral_upper_bound)

hSigCorrected = hSig.Clone('hSigCorrected')


result = hSigCorrected.Divide(fBiasFunction)
hSigCorrected.SetLineColor(root.kBlue)
hSigCorrected.Draw('same')
fSig.Draw('same')



c4.Update()

raw_input('Press enter to continue')

