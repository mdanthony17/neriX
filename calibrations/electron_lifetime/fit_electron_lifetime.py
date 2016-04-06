#!/usr/bin/python

import ROOT as root
import sys
import neriX_analysis
from rootpy.plotting import Hist2D, Hist, Legend, Canvas
import math, root_numpy
import numpy as np



# returns tgraphasym with medians and
def profile_y_median(hist_to_profile):
	num_bins_x = hist_to_profile.nbins(0)
	num_bins_y = hist_to_profile.nbins(1)
	
	bounds_x = hist_to_profile.bounds(0)
	bounds_y = hist_to_profile.bounds(1)
	
	step_size_x = (bounds_x[1] - bounds_x[0]) / float(num_bins_x)
	step_size_y = (bounds_y[1] - bounds_y[0]) / float(num_bins_y)
	
	bin_centers_x = np.linspace(bounds_x[0] + step_size_x/2., bounds_x[1] - step_size_x/2., num=num_bins_x)
	bin_centers_y = np.linspace(bounds_y[0] + step_size_y/2., bounds_y[1] - step_size_y/2., num=num_bins_y)

	aHist = root_numpy.hist2array(hist_to_profile, include_overflow=False)

	# outer loop are columns (what we want)!
	# inner array is small to large
	
	aXErrLow = np.asarray([step_size_x/2. for i in xrange(num_bins_x)])
	aXErrHigh = np.asarray([step_size_x/2. for i in xrange(num_bins_x)])
	
	aYValues = np.zeros(num_bins_x)
	aYErrLow = np.zeros(num_bins_x)
	aYErrHigh = np.zeros(num_bins_x)

	for slice_number, x_slice in enumerate(aHist):
		points_in_slice = np.zeros(np.sum(x_slice))
		index_counter = 0
		for i, bin_count in enumerate(x_slice):
			for j in xrange(bin_count):
				points_in_slice[index_counter] = bin_centers_y[i]
				index_counter += 1

		aYValues[slice_number] = np.median(points_in_slice)
		low_err, high_err = np.percentile(points_in_slice, [50-34.1, 50+34.1])
		aYErrLow[slice_number], aYErrHigh[slice_number] = aYValues[slice_number]-low_err, high_err-aYValues[slice_number]

	return root.TGraphAsymmErrors(num_bins_x, bin_centers_x, aYValues, aXErrLow, aXErrHigh, aYErrLow, aYErrHigh)






#--------------- Start Parameters to Change ----------------


s2NumBins = 40
s2Min = 0
s2Max = 900e3

dtNumBins = 120
dtMin = 3.
dtMax = 14.
dtMaxFit = 9.

numBinsCES = 50
upperBoundCES = 1200.
secondParameter = 'S2sTotBottom[0]'


#--------------- End Parameters to Change ----------------



if(len(sys.argv) != 2):
	print 'Usage is python coin_analysis <ROOT filename>'
	sys.exit(1)

file1 = sys.argv[1]
currentAnalysis = neriX_analysis.neriX_analysis(file1)


#choose options
option1 = 'COLZ'
optionSame = 'SAME'


#choose cuts for run 1
currentAnalysis.add_dt_cut(dtMin, dtMax)
currentAnalysis.add_single_scatter_cut()
#currentAnalysis.add_cut('S2sTotBottom[0] > 300e3')
#currentAnalysis.add_cut('(s1asym > 0)')
#currentAnalysis.add_radius_cut(0, 0.5)

# cut on 40 keV peak
#currentAnalysis.add_cut('S1sTotBottom[0] > 80 && S1sTotBottom[0] < 380')
#currentAnalysis.add_cut('S2sTotTop[0] > 20e3 && S2sTotTop[0] < 70e3')

# cut on 164 keV peak
#currentAnalysis.add_cut('S1sTotBottom[0] > 400 && S1sTotBottom[0] < 900')
#currentAnalysis.add_cut('S2sTotBottom[0] > 200e3 && S2sTotBottom[0] < 400e3')

#currentAnalysis.get_T1().Scan('EventId:S2sTot[0]:S2sSaturation[0]', currentAnalysis.get_cuts())

currentAnalysis.set_event_list()



# ---------- MAKE CES CUT ----------

g1 = 0.095 #0.14 #0.1
g2 = 17. #26.2 #24.
sigmaCES = 1.

cCES = Canvas(width=900, height=700, name='cCES')
cCES.SetGridx()
cCES.SetGridy()

hCES = Hist(numBinsCES, 0, upperBoundCES, name='hCES', title='hCES', drawstyle='hist')
currentAnalysis.Draw('0.0137*(S1sTotBottom[0]/%f + %s/%f)' % (g1, secondParameter, g2), hist=hCES)

hCES.Draw()
cCES.Update()

lbCES = float(raw_input('\n\nPlease enter lower bound on combined energy scale: '))
ubCES = float(raw_input('\n\nPlease enter upper bound on combined energy scale: '))


fGausCES = root.TF1('fGausCES', 'gaus', lbCES, ubCES)
#fGausCES = root.TF1('fGausCES', '[0]*exp(-[1]*x) + [2]*exp(-0.5*pow((x-[3])/[4], 2))', lbCES, ubCES)
#fGausCES.SetParameters(500, 0.01, 500, 650, 50)
hCES.Fit(fGausCES, 'LR')
fGausCES.Draw('same')
cCES.Update()

lbCES = fGausCES.GetParameter(1) - sigmaCES*fGausCES.GetParameter(2)
ubCES = fGausCES.GetParameter(1) + sigmaCES*fGausCES.GetParameter(2)
raw_input('Fit okay?')


sCES = '(0.0137*(S1sTotBottom[0]/%f + %s/%f) > %f && 0.0137*(S1sTotBottom[0]/%f + %s/%f) < %f)' % (g1, secondParameter, g2, lbCES, g1, secondParameter, g2, ubCES)
currentAnalysis.add_cut(sCES)
#currentAnalysis.set_event_list()

del hCES
del cCES





# check spectrum

c2 = Canvas()

hS1S2 = Hist2D(dtNumBins, dtMin, dtMax, s2NumBins, s2Min, s2Max, name='ha2', title='dt vs S2 - ' + file1)






#create histograms for currentAnalysis

ha2 = Hist2D(dtNumBins, dtMin, dtMax, s2NumBins, s2Min, s2Max, name='ha2', title='dt vs S2 - ' + file1)
currentAnalysis.Draw('dt:S2sTotBottom[0]', hist=ha2, selection=currentAnalysis.get_cuts())
ha2.GetXaxis().SetTitle('dt [\mu s]')
ha2.GetYaxis().SetTitle('S2sTotBottom[0] [PE]')
ha2.SetStats(0)

# make much finer binning in Y for median profile
hForMedianProfile = Hist2D(dtNumBins, dtMin, dtMax, 10000, s2Min, s2Max, name='hhForMedianProfile')
currentAnalysis.Draw('dt:S2sTotBottom[0]', hist=hForMedianProfile, selection=currentAnalysis.get_cuts())


gProfileX = profile_y_median(hForMedianProfile)
gProfileX.SetTitle('dt vs S2 (Median Profile) - ' + file1)
gProfileX.GetXaxis().SetRangeUser(dtMin, dtMax)
gProfileX.GetYaxis().SetRangeUser(s2Min, s2Max)

"""
hProfile = ha2.ProfileX('hProfile')

hProfile.GetXaxis().SetTitle('dt [\mu s]')
hProfile.GetYaxis().SetTitle('S2sTotBottom[0] [PE]')
hProfile.GetYaxis().SetRangeUser(s2Min, s2Max)
hProfile.SetMarkerSize(0)
hProfile.SetStats(0)
"""

c1 = root.TCanvas('c1','',1100,480)
c1.Divide(2)
c1.cd(1)
ha2.Draw(option1)
c1.SetLogz()
c1.SetGridx()
c1.SetGridy()
c1.cd(2)
#hProfile.Draw()
gProfileX.Draw('ap')
c1.SetGridx()
c1.SetGridy()

c1.Update()


# 2d fit does not work well

f1 = root.TF1('f1', '[0]*exp(-[1]*x)', dtMin, dtMax)
f1.SetParameters(400e3, 1./50.)

#hProfile.Fit('f1', 'MELR')
gProfileX.Fit('f1', 'MELR')
f1.SetLineStyle(1)
f1.SetLineWidth(2)
f1.SetLineColor(root.kRed)

c1.cd(1)
f1.Draw('same')
c1.cd(2)
f1.Draw('same')


#sFitInfo1 = 'variation = (%.2e +/- %.2e) [(Change in S2 / Mean)/us]' % (math.fabs(f1.GetParameter(1)/ha2.GetMean(2)), f1.GetParError(1)/ha2.GetMean(2))

# linear fit
electronLifetime = (1/math.exp(1)-1)*f1.GetParameter(0)/f1.GetParameter(1)
sFitInfo1 = 'e^{-} lifetime = (%.2f +/- %.2f) us' % ( electronLifetime, math.fabs(electronLifetime * ((f1.GetParError(0)/(f1.GetParameter(0)*(1/math.exp(1)-1)))**2 + (f1.GetParError(1)/f1.GetParameter(1))**2)**0.5 ) )

# exponential fit
electronLifetime = 1./f1.GetParameter(1)
sFitInfo1 = 'e^{-} lifetime = (%.2f +/- %.2f) us' % ( electronLifetime, math.fabs(f1.GetParError(1)/f1.GetParameter(1))*math.fabs(electronLifetime) )


pt1 = root.TPaveText(.1,.65,.9,.9,'blNDC')
text1 = pt1.AddText(sFitInfo1)
pt1.SetTextColor(root.kBlue)
pt1.SetFillStyle(0)
pt1.SetBorderSize(0)
pt1.Draw('same')

c1.Update()

raw_input('\nEnter to continue...\n')

#c1.SaveAs('~/Desktop/electron_lifetime.png')

neriX_analysis.save_plot(['results', 'run_%d' % currentAnalysis.get_run(), currentAnalysis.get_filename()[:-5]], c1, 'electron_lifetime_%s' % currentAnalysis.get_filename()[:-5])

