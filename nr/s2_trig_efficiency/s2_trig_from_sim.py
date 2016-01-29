#!/usr/bin/python

import ROOT as root
import sys, math, os, root_numpy
import neriX_analysis
from rootpy.plotting import Hist, Hist2D, Canvas, Legend, Graph
from rootpy.io import root_open, File
from rootpy.tree import TreeChain
import rootpy.compiled as C
import numpy as np
from tqdm import tqdm
from produce_nest_yields import *


def convert_hist_to_graph_with_poisson_errors(inputHist):
	graph = Graph(inputHist.nbins(axis=0), type='asymm')
	graph.SetLineWidth(inputHist.GetLineWidth())
	graph.SetMarkerSize(inputHist.GetMarkerSize())
	chisqr = root.TMath.ChisquareQuantile
	npoints = 0
	for bin in inputHist.bins(overflow=False):
		entries = bin.effective_entries
		if entries < 0:
			continue
		elif entries == 0:
			ey_low = 0
			ey_high = 1.13943 # -ln(1-0.68)
		else:
			ey_low = entries - 0.5 * chisqr(0.1586555, 2. * entries)
			ey_high = 0.5 * chisqr(
				1. - 0.1586555, 2. * (entries + 1)) - entries
		ex = bin.x.width / 2.
		graph.SetPoint(npoints, bin.x.center, bin.value)
		graph.SetPointEXlow(npoints, ex)
		graph.SetPointEXhigh(npoints, ex)
		graph.SetPointEYlow(npoints, ey_low)
		graph.SetPointEYhigh(npoints, ey_high)
		npoints += 1
	graph.Set(npoints)
	return graph


C.register_code(
"""
#include <vector>
#include <TH1.h>
#include <TRandom3.h>
#include <stdio.h>
#include <math.h>

void full_matching_loop_nest_yields(int numTrials, double *aS1, double *aS2, TH1F &hEnergy, float dfVcm, float percentAdjustment, float g1Value, float extractionEfficiency, float gasGainValue, float gasGainWidth, float speRes, float intrinsicResS1, float intrinsicResS2)
{
	TRandom r3 = TRandom3();
	float wValue = 13.7e-3;
	
	
	// variables needed in loop
	float mcEnergy;
	float excitonToIonRatio;
	int numQuantaBeforeLindhard;
	int mcQuanta;
	float probExcitonSuccess;
	int mcExcitons;
	int mcIons;
	int mcRecombined;
	int mcPhotons;
	int mcElectrons;
	int mcExtractedElectrons;
	float mcS1;
	float mcS2;
	
	
	// constants for NEST predictions
	float rho = 2.9; // g/cc
	float tiParameter = 3.77e-2;
	float atomicNumber = 131.;
	float epsilon;
	float kFactor;
	float g;
	float lindhardFactor;
	float nest_numQuanta;
	float nest_numIons;
	float nest_recombinationProb;
	float photonYield;
	float chargeYield;
	
	
	
	
	
	float probRecombination = ( (excitonToIonRatio+1) * photonYield )/(photonYield+chargeYield) - excitonToIonRatio;
	//printf("%f \\n", excitonToIonRatio);

	for (int i = 0; i < numTrials; i++)
	{
		// ------------------------------------------------
		//  Draw random energy from distribution
		// ------------------------------------------------
		
		mcEnergy = hEnergy.GetRandom();
		//printf("%f \\n", mcEnergy);
		
		
		
		// ------------------------------------------------
		//  Get photon and charge yield from NEST
		// ------------------------------------------------
		
		
		epsilon = 11.5*mcEnergy*(pow(54., -7./3.));
		kFactor = 0.1394*pow(atomicNumber/131.293,0.5);
		g = 3.*pow(epsilon,0.15)+0.7*pow(epsilon,0.6)+epsilon;
		lindhardFactor = (kFactor*g)/(1.+kFactor*g);
		nest_numQuanta = mcEnergy*lindhardFactor*1000./(21.717-2.7759*rho);
		excitonToIonRatio = 1.24*pow(dfVcm,-0.0472)*(1.-exp(-239.*epsilon));
		nest_numIons = nest_numQuanta/(1.+excitonToIonRatio);
		nest_recombinationProb = 1.-log(1.+(tiParameter/4.)*nest_numIons)/((tiParameter/4.)*nest_numIons);

		photonYield = ((nest_numQuanta*(excitonToIonRatio/(1.+excitonToIonRatio))+nest_numIons*nest_recombinationProb)/(1+3.32*pow(epsilon,1.141))) / mcEnergy;
		chargeYield = ((1.-nest_recombinationProb)*nest_numIons) / mcEnergy;
		
		photonYield *= (1. + percentAdjustment);
		chargeYield *= (1. + percentAdjustment);


		// ------------------------------------------------
		//  Find number of quanta
		// ------------------------------------------------
		
		
		mcQuanta = r3.Poisson(mcEnergy*(photonYield + chargeYield));
		//printf("%d \\n", mcQuanta);
		
		
		// ------------------------------------------------
		//  Convert to excitons and ions
		// ------------------------------------------------
		
		
		probExcitonSuccess = 1. - 1./(1. + excitonToIonRatio);
		if (probExcitonSuccess < 0 || probExcitonSuccess > 1) continue;
		
		mcExcitons = r3.Binomial(mcQuanta, probExcitonSuccess);
		mcIons = mcQuanta - mcExcitons;
		//printf("%d \\n", mcExcitons);
		
		
		// ------------------------------------------------
		//  Ion recombination
		// ------------------------------------------------

		if (mcIons < 1 || nest_recombinationProb < 0 || nest_recombinationProb > 1) continue;
		
		mcRecombined = r3.Binomial(mcIons, nest_recombinationProb);
		mcPhotons = mcExcitons + mcRecombined;
		mcElectrons = mcIons - mcRecombined;
		//printf("%d \\n", mcRecombined);
		
		
		// ------------------------------------------------
		//  Convert to S1 and S2 BEFORE smearing
		// ------------------------------------------------
		
		if (mcPhotons < 1 || g1Value < 0 || g1Value > 1) continue;
		if (mcElectrons < 1 || extractionEfficiency < 0 || extractionEfficiency > 1) continue;
		if (gasGainWidth <= 0) continue;
		
		mcS1 = r3.Binomial(mcPhotons, g1Value);
		mcExtractedElectrons = r3.Binomial(mcElectrons, extractionEfficiency);
		mcS2 = r3.Gaus(mcExtractedElectrons*gasGainValue, gasGainWidth*pow(mcExtractedElectrons, 0.5));
		
		if (mcS1 < 0) continue;
		if (mcS2 < 0) continue;
		
		//printf("%f \\n", mcS1);
		//printf("%f \\n", mcS2);
		
		
		// ------------------------------------------------
		//  Smear S1 and S2
		// ------------------------------------------------
		
		if (speRes <= 0 || intrinsicResS1 <= 0 || intrinsicResS2 <= 0) continue;
		
		mcS1 = r3.Gaus(mcS1, speRes*pow(mcS1, 0.5));
		if (mcS1 < 0) continue;
		mcS1 = r3.Gaus(mcS1, intrinsicResS1*pow(mcS1, 0.5));
		if (mcS1 < 0) continue;
		
		mcS2 = r3.Gaus(mcS2, speRes*pow(mcS2, 0.5));
		if (mcS2 < 0) continue;
		mcS2 = r3.Gaus(mcS2, intrinsicResS1*pow(mcS2, 0.5));
		if (mcS2 < 0) continue;
		
		if (mcS1 < 5) continue;
		
		aS1[i] = mcS1;
		aS2[i] = mcS2;
		
		//printf("%f \\n", mcS1);
		
		
	}
}

""", ['full_matching_loop_nest_yields'])

c_full_matching_loop_nest_yields = C.full_matching_loop_nest_yields












if(len(sys.argv) != 2):
	print 'Usage is s2_trig_from_sim.py <ROOT filename>'
	sys.exit(1)


# setup data file
	
filename = sys.argv[1]

#current_analysis = neriX_analysis.neriX_analysis(['nerix_160118_0922', 'nerix_160125_1023'], 45, 1.054, 4.5)
current_analysis = neriX_analysis.neriX_analysis(filename)
current_analysis.add_z_cut()
current_analysis.add_radius_cut(0, 0.85)
current_analysis.add_xs1asym_cut()
current_analysis.add_xs2asym_cut()
current_analysis.add_single_scatter_cut()


s1Branch = 'cpS1sTotBottom[0]'
s2Branch = 'cpS2sTotBottom[0]'

current_analysis.add_cut('TMath::Log10(%s/%s) < 3' % (s1Branch, s2Branch))
current_analysis.add_cut('%s > 5' % (s1Branch))

current_analysis.set_event_list()

s1NumBins = 15
s1LowerBound = 0
s1UpperBound = 30

s2NumBins = 15
s2LowerBound = 0
s2UpperBound = 3000

s2IntegralLowerBound = 2000


# setup MC files

# m40 is minitron at 40 cm
# actual distance is 43 cm
mcDataDir = './simulated_data_m40/'

# get list of sim files
lMCFiles = os.listdir(mcDataDir)
for i, file in enumerate(lMCFiles):
	lMCFiles[i] = mcDataDir + file

#print lMCFiles

# put the MC files into a chain
cMC = TreeChain(name='t2', files=lMCFiles)

# test

hEnergy = Hist(80, 0, 80, name='hEnergy')
cMC.Draw('etot_target', 'sqrt(xpos[0]**2+ypos[0]**2)<18. && zpos[0]>-20 && zpos[0]<-4', hist=hEnergy)
#c1 = Canvas()
#hEnergy.Draw()
#c1.Update()

numMCTrials = int(1e6)
root.gRandom.SetSeed(0)
aS1 = np.full(numMCTrials, -1.)
aS2 = np.full(numMCTrials, -1.)

dfVcm = 500
yieldAdjustment = 0.
g1Value = 0.1
extractionEfficiency = 22./31.
gasGainValue = 30.
gasGainWidth = 10.
speRes = 0.7
intrinsicResolutionS1 = 0.5
intrinsicResolutionS2 = 0.5

print '\n\nStarting MC'

c_full_matching_loop_nest_yields(numMCTrials, aS1, aS2, hEnergy, dfVcm, yieldAdjustment, g1Value, extractionEfficiency, gasGainValue, gasGainWidth, speRes, intrinsicResolutionS1, intrinsicResolutionS2)

print 'Finished MC\n\n'

# initialize histograms
hMCS2 = Hist(s2NumBins, s2LowerBound, s2UpperBound, name='hMCS2')
hMCS2.SetStats(0)
hMCS2.SetMarkerSize(0)
hMCS2.Sumw2()
hDataS2 = Hist(s2NumBins, s2LowerBound, s2UpperBound, name='hDataS2', title='S2 Efficiency from MC Comparison')
hDataS2.SetStats(0)
hDataS2.SetMarkerSize(0)
hDataS2.Sumw2()

hMC2D = Hist2D(s1NumBins, s1LowerBound, s1UpperBound, s2NumBins, s2LowerBound, s2UpperBound, name='hMC2D', title='S1 vs. S2 - Data')
hData2D = Hist2D(s1NumBins, s1LowerBound, s1UpperBound, s2NumBins, s2LowerBound, s2UpperBound, name='hData2D', title='S1 vs. S2 - MC')
hMC2D.SetStats(0)
hData2D.SetStats(0)


# fill histograms
hMCS2.fill_array(aS2)
current_analysis.Draw(s2Branch, current_analysis.get_cuts(), hist=hDataS2)

# integrate high energy end and scale accordingly
numEntriesMCHighS2 = hMCS2.Integral(hMCS2.FindBin(s2IntegralLowerBound), hMCS2.FindBin(s2UpperBound)-1)
numEntriesDataHighS2 = hDataS2.Integral(hDataS2.FindBin(s2IntegralLowerBound), hDataS2.FindBin(s2UpperBound)-1)
hMCS2.Scale(numEntriesDataHighS2 / float(numEntriesMCHighS2))


# concatenate S1 and S2 arrays and then reshape
#aFull = np.append(aS1, aS2)
#aFull = aFull.reshape(numMCTrials, 2)
aFull = np.column_stack((aS1, aS2))

print aFull

hMC2D.fill_array(aFull)
current_analysis.Draw('%s:%s' % (s1Branch, s2Branch), current_analysis.get_cuts(), hist=hData2D)
hMC2D.Scale(numEntriesDataHighS2 / float(numEntriesMCHighS2))


cScaleHistograms = Canvas()

hDataS2.SetMarkerSize(0)
hDataS2.Draw()
hMCS2.SetLineColor(root.kBlue)
hMCS2.SetMarkerSize(0)
hMCS2.Draw('same')




c2DHistograms = Canvas()
c2DHistograms.Divide(2)

c2DHistograms.cd(1)
hData2D.SetMarkerSize(0)
hData2D.Draw('colz')
c2DHistograms.cd(2)
hMC2D.SetLineColor(root.kBlue)
hMC2D.SetMarkerSize(0)
hMC2D.Draw('colz')





c1 = Canvas()

# convert now binned data to arrays for analysis
# this is only if likelihood analysis is done
#aBinnedMCS2 = root_numpy.hist2array(hMCS2, include_overflow=False)
#aBinnedDataS2 = root_numpy.hist2array(hDataS2, include_overflow=False)

#hDataS2.Divide(hDataS2, hMCS2, 1.0, 1.0, 'B')
hDataS2.Divide(hDataS2, hMCS2)

#hDataS2Copy = hDataS2.Clone()
#hDataS2 = root.TGraphAsymmErrors()
#hDataS2.Divide(hDataS2Copy,hMCS2,'cl=0.683 b(1,1) mode e0')

hDataS2.Draw()
c1.Update()


hDataS2 = convert_hist_to_graph_with_poisson_errors(hDataS2)


fSigmoid = root.TF1('fSigmoid', '(1. / (1 + exp([1]*([0]-x))))', s2LowerBound, s2UpperBound)
fSigmoid.SetParameters(500, 0.005)
#fSigmoid.FixParameter(0, 200)
#fSigmoid.FixParameter(1, 0.004)
#fSigmoid.SetParLimits(0, 0, 800)
#fSigmoid.SetParLimits(1, 0.003, 0.006)

"""
fSigmoid = root.TF1('fSigmoid', '(1. - exp(-1.*[0]*x))', s2LowerBound, s2UpperBound)
fSigmoid.SetParameter(0, 0.0017)
#fSigmoid.FixParameter(0, 0.0025)
#fSigmoid.SetParLimits(0, 0.001, 0.003)
"""

fSigmoid.SetLineStyle(9)
fSigmoid.SetLineWidth(1)
fSigmoid.SetLineColor(root.kBlue)

c1.cd()
#hEffForFitting.Fit(fSigmoid, 'NL')
hDataS2.Fit(fSigmoid, 'MELLSNR')

fSigmoid.Draw('same')

gConfInterval = root.TGraphAsymmErrors(*neriX_analysis.create_graph_with_confidence_interval_for_fit(hDataS2, root.TVirtualFitter.GetFitter()))
gConfInterval.SetLineColor(root.kBlue)
gConfInterval.SetFillColor(root.kBlue)
gConfInterval.SetFillStyle(3005)
gConfInterval.Draw('3 same')

# sigmoid

sFitInfo1 = '#epsilon_{trig} = #frac{1}{1 + e^{#alpha (#beta-x)}}'
pt1 = root.TPaveText(.5,.35,.8,.45,'blNDC')
text1 = pt1.AddText(sFitInfo1)
pt1.SetTextColor(root.kBlack)
pt1.SetFillStyle(0)
pt1.SetBorderSize(0)
pt1.Draw('same')

sFitInfo2 = '#alpha = %.4f #pm %.4f' % (fSigmoid.GetParameter(1), fSigmoid.GetParError(1))
pt2 = root.TPaveText(.5,.25,.8,.35,'blNDC')
text2 = pt2.AddText(sFitInfo2)
pt2.SetTextColor(root.kBlack)
pt2.SetFillStyle(0)
pt2.SetBorderSize(0)
pt2.Draw('same')

sFitInfo3 = '#beta = %.4f #pm %.4f' % (fSigmoid.GetParameter(0), fSigmoid.GetParError(0))
pt3 = root.TPaveText(.5,.15,.8,.25,'blNDC')
text3 = pt3.AddText(sFitInfo3)
pt3.SetTextColor(root.kBlack)
pt3.SetFillStyle(0)
pt3.SetBorderSize(0)
pt3.Draw('same')


# exponential
"""
sFitInfo1 = '#epsilon_{trig} = #frac{1}{1 + e^{#alpha (#beta-x)}}'
pt1 = root.TPaveText(.5,.35,.8,.45,'blNDC')
text1 = pt1.AddText(sFitInfo1)
pt1.SetTextColor(root.kBlack)
pt1.SetFillStyle(0)
pt1.SetBorderSize(0)
pt1.Draw('same')

sFitInfo2 = '#alpha = %.4f #pm %.4f' % (fSigmoid.GetParameter(0), fSigmoid.GetParError(0))
pt2 = root.TPaveText(.5,.25,.8,.35,'blNDC')
text2 = pt2.AddText(sFitInfo2)
pt2.SetTextColor(root.kBlack)
pt2.SetFillStyle(0)
pt2.SetBorderSize(0)
pt2.Draw('same')
"""

thresholdX = fSigmoid.GetX(0.5, s2LowerBound, s2UpperBound)
lThreshold = root.TLine()
lThreshold.SetLineColor(root.kOrange + 1)
lThreshold.SetLineStyle(7)
lThreshold.DrawLine(thresholdX, 0, thresholdX, 1)


c1.Update()


raw_input('Press enter to continue...')






