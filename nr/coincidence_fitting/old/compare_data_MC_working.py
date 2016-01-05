#!/usr/bin/python

import ROOT as root
import sys, array, root_numpy
import neriX_analysis
from rootpy.plotting import Hist2D, Hist, Legend, Canvas, Graph
from rootpy.io import root_open, File
import numpy as np


# based off of rootpy's implementation BUT includes
# the value = 0 case
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



# generator for floats
def drange(start, stop, step):
	r = start
	while r < stop:
		yield r
		r += step



def convert_energy_to_photons(aEnergy, aLightYield):
	MAX_LIGHT_YIELD = 15. # will set total number of bins (max*largest energy)
	
	assert aLightYield.size == aEnergy.get_num_elements()
	
	# create photon array such that each each photon has a bin
	# and is centered at the integer value
	numPhotonBins = int(MAX_LIGHT_YIELD*aEnergy.get_upper_bound())
	aPhotonCenters = np.matrix([i for i in xrange(numPhotonBins)])
	aPhotonBinEdges = np.array([i - 0.5 for i in xrange(numPhotonBins+1)]) # +1 to get right edge too!
	
	#print aEnergy.get_num_elements(), numPhotonBins
	#print [ [0. for i in xrange(aEnergy.get_num_elements())] for j in xrange(numPhotonBins) ]
	
	# initialize to all zeros
	aConversionMatrix = np.matrix([ [0. for i in xrange(aEnergy.get_num_elements())] for j in xrange(numPhotonBins) ])
	
	# for each energy create the corresponding Photon Yield
	# for the transformation matrix
	#print aEnergy.get_x_values()
	for energyBin, energy in np.ndenumerate(aEnergy.get_x_values()):
		# take second index of energy bin (column) since only one row
		energyBin = energyBin[1]
		lightYield = aLightYield[energyBin]
		aHistogram, aPhotonBinEdges = np.histogram([lightYield*energy], bins=aPhotonBinEdges)
		aConversionMatrix[:, energyBin] = np.matrix(aHistogram).T
	
	#print aConversionMatrix*aEnergy.get_y_values().T
	return aConversionMatrix*aEnergy.get_y_values().T




def convert_photons_to_PE(aPhotonSpectrum, g1, g1Err, peNumBins, peMin, peMax):
	NUM_RANDOM_SAMPLES = 10000
	
	# extra bins such that PDF normalized to 1
	# need to remove after finished
	extraBins = 15
	
	# create photon array such that each each photon has a bin
	# and is centered at the integer value
	peBinWidth = (peMax - peMin) / float(peNumBins)
	aPEBinEdges = np.array([i for i in drange(peMin-extraBins*peBinWidth+0.01, peMax + extraBins*peBinWidth, peBinWidth)])
	# +peBinWidth to get right edge too!
	# need to keep 0 PE bin for normalization!
	# will ditch in loop
	# .01 is to make sure values at edges go into propper bin
	
	# need to photon bins for histogram
	numPhotonBins = aPhotonSpectrum.size
	aPhotonCenters = np.matrix([i for i in xrange(numPhotonBins)])
	aPhotonBinEdges = np.array([i - 0.5 for i in xrange(numPhotonBins+1)]) # +1 to get right edge too!
	
	
	# initialize to all zeros
	aConversionMatrix = np.matrix([ [0. for i in xrange(numPhotonBins)] for j in xrange(peNumBins) ])
	# rows are bins in PE
	# columns are bins in photons
	
	# for each energy create the corresponding Photon Yield
	# for the transformation matrix
	for photonBin, numPhotons in np.ndenumerate(aPhotonCenters):
		# take second index of energy bin (column) since only one row
		photonBin = photonBin[1]
		# feed in g1 value distributed on gaussian by uncertainty
		muForPoisson = np.random.normal(g1, g1Err)*numPhotons
		aPoissonResults = np.random.poisson(muForPoisson, NUM_RANDOM_SAMPLES)
		
		# need to include zero PE bin and then drop it
		# also need to scale each value back up to probability
		# since all bins are normalized by bin width
		aHistogram, aPEBinEdges = np.histogram(aPoissonResults, bins=aPEBinEdges, density=True)
		aHistogram *= peBinWidth
		
		aHistogram = aHistogram[extraBins:-(extraBins-1)]
		aConversionMatrix[:, photonBin] = np.matrix(aHistogram).T
	
	#print aConversionMatrix*aPhotonSpectrum
	return aConversionMatrix*aPhotonSpectrum




def smear_PE_spectrum(aPESpectrum, lResolutionsForGaussians, peNumBins, peMin, peMax):
	NUM_RANDOM_SAMPLES = 10000
	
	for set in lResolutionsForGaussians:
		assert len(set) == 2
	
	resIndex = 0
	powerIndex = 1
	# power is the power that N is raised to in R*N**pow
	# for SPE pow=1/2
	
	# extra bins such that PDF normalized to 1
	# need to remove after finished
	extraBins = 5
	
	# create photon array such that each each photon has a bin
	# and is centered at the integer value
	peBinWidth = (peMax - peMin) / float(peNumBins)
	aPEBinCenters = np.matrix([i + peBinWidth/2. for i in drange(peMin, peMax, peBinWidth)])
	aPEBinEdges = np.array([i for i in drange(peMin - extraBins*peBinWidth + 0.01, peMax + extraBins*peBinWidth, peBinWidth)])
	# +peBinWidth to get right edge too!
	# need to keep 0 PE bin for normalization!
	# will ditch in loop
	# .01 is to make sure values at edges go into propper bin
	
	
	# initialize to all zeros
	aConversionMatrix = np.matrix([ [0. for i in xrange(peNumBins)] for j in xrange(peNumBins) ])
	# rows are bins in PE
	# columns are bins in photons
	
	# for each energy create the corresponding Photon Yield
	# for the transformation matrix
	for peBin, numPE in np.ndenumerate(aPEBinCenters):
		# take second index of energy bin (column) since only one row
		peBin = peBin[1]
		
		# get gaussian results (only spe for now!!!)
		#print numPE, lResolutionsForGaussians[0][resIndex], lResolutionsForGaussians[0][powerIndex], lResolutionsForGaussians[0][resIndex]*numPE**lResolutionsForGaussians[0][powerIndex]
		if numPE < 0.9:
			aGaussianResults = [0]
		else:
			aGaussianResults = np.random.normal(numPE, lResolutionsForGaussians[0][resIndex]*numPE**lResolutionsForGaussians[0][powerIndex], NUM_RANDOM_SAMPLES)
		
		# need to include zero PE bin and then drop it
		# also need to scale each value back up to probability
		# since all bins are normalized by bin width
		aHistogram, aPEBinEdges = np.histogram(aGaussianResults, bins=aPEBinEdges, density=True)
		
		#grab part of histogram we care about
		aHistogram = peBinWidth*aHistogram[extraBins:-(extraBins-1)]
		aConversionMatrix[:, peBin] = np.matrix(aHistogram).T
	
	#print aConversionMatrix*aPESpectrum
	return aConversionMatrix*aPESpectrum






class data_for_graphs:
	def __init__(self, graphToConvert):
		if not isinstance(graphToConvert, root.TGraphAsymmErrors):
			print 'Must input TGraphAsymmErrors to convert'
			sys.exit()
		
		self.numElements = int(graphToConvert.GetN())
	
		self.dFunctions = {'x':graphToConvert.GetX,
					  'y':graphToConvert.GetY,
					  'x_err_high':graphToConvert.GetEXhigh,
					  'x_err_low':graphToConvert.GetEXlow,
					  'y_err_high':graphToConvert.GetEYhigh,
					  'y_err_low':graphToConvert.GetEYlow
					   }
		self.dArrays = {'x':[],
					   'y':[],
					   'x_err_high':[],
					   'x_err_low':[],
					   'y_err_high':[],
					   'y_err_low':[]
					   }

		# create arrays
		for key in self.dArrays:
			self.create_list_from_graph(graphToConvert, key)

		self.aBinCenters = np.array([i for i in drange(self.get_lower_bound(), self.get_upper_bound(), self.get_bin_size())])


	def get_num_elements(self):
		return self.numElements


	def create_list_from_graph(self, graphToConvert, parameterToGrab):
		if not isinstance(parameterToGrab, str):
			print 'Must give string parameter'
			sys.exit()
		
		functionToCall = self.dFunctions[parameterToGrab]
		tempArray = functionToCall()
		tempArray.SetSize(self.get_num_elements())
		self.dArrays[parameterToGrab] = np.matrix(tempArray, 'd')


	def get_lower_bound(self):
		return self.dArrays['x'][0, 0]


	def get_upper_bound(self):
		return self.dArrays['x'][0, -1]


	def get_bin_size(self):
		return (self.get_upper_bound() - self.get_lower_bound()) / float(self.get_num_elements())


	def get_x_values(self):
		return self.dArrays['x']


	def get_y_values(self):
		return self.dArrays['y']







if(len(sys.argv) != 4):
	print 'Usage is python compare_data_MC.py <data ROOT file> <S1 trig efficiency ROOT file> <MC ROOT file>'
	sys.exit(1)

dNamesForFiles = {'data':sys.argv[1], 's1_trig':sys.argv[2], 'MC':sys.argv[3]}
dFilesForAnalysis = {}

for fileType in dNamesForFiles:
	fileName = dNamesForFiles[fileType]
	if fileName[-5:] != '.root':
		fileName += '.root'
	if fileType == 'data':
		fileName = '../analysis/full_angle_files/' + fileName
	elif fileType == 's1_trig':
		fileName = 'efficiency_files/' + fileName
	elif fileType == 'MC':
		fileName = 'reduced_simulation_data/' + fileName

	# add file to dictionary
	dFilesForAnalysis[fileType] = File(fileName)


# gather needed histograms

# data histograms
hS1S2 = dFilesForAnalysis['data'].hS1S2
hS1S2.Sumw2()
hS1 = dFilesForAnalysis['data'].hS1
hS1.Sumw2()
gS1 = convert_hist_to_graph_with_poisson_errors(hS1)

aoS1 = data_for_graphs(gS1)

# below is example of how to get arrays from graphs
#bD = gS1.GetX()
#bD.SetSize(gS1.GetN())
#print np.array(bD, 'd')

hS2 = dFilesForAnalysis['data'].hS2
hS2.Sumw2()
gS2 = convert_hist_to_graph_with_poisson_errors(hS2)

# also getting binning, mins, and maxes from histograms

# HISTOGRAM BINNING
s1NumBins = len(hS1.bins_range())
s1Min = hS1.lowerbound()
s1Max = hS1.upperbound()

s2NumBins = len(hS2.bins_range())
s2Min = hS2.lowerbound()
s2Max = hS2.upperbound()



# s1 trigger histograms
hNoTrigCut = dFilesForAnalysis['s1_trig'].s1_spec_no_trig_cut
hNoTrigCut.Sumw2()
hNoTrigCut.SetBins(s1NumBins, s1Min, s1Max)
hWithTrigCut = dFilesForAnalysis['s1_trig'].s1_spec_with_trig_cut
hWithTrigCut.Sumw2()
hWithTrigCut.SetBins(s1NumBins, s1Min, s1Max)

gS1TrigEfficiency = root.TEfficiency(hWithTrigCut, hNoTrigCut)

aoTACEfficiency = data_for_graphs(gS1TrigEfficiency.CreateGraph())

# must manually loop through graph buffers

# left off here

#lValues = [element.value for element in hS1[:]]
#print lValues

c1 = Canvas()

#hS1.Multiply(hNoTrigCut)
#hS1.Divide(hWithTrigCut)
hS1.Draw()
c1.Update()



# open simulation information

xRadius = '(sqrt(xpos[0]**2+ypos[0]**2) < 15)'
xZ = '(zpos[0]>-20 && zpos[0]<-4)'
xSingleScatter = '(nsteps_target==1)'
xLiqSciHeight = '(etotliqsci>700)'
xLXeEnergy = '(etot_target>0)'

xTOF = '(timeliqsci-tpos[0]>30 && timeliqsci-tpos[0]<50)'

xAll = '%s && %s && %s && %s && %s && %s' % (xRadius, xZ, xSingleScatter, xLiqSciHeight, xLXeEnergy, xTOF)


fMC = dFilesForAnalysis['MC']
tMC = fMC.t2

energyNumBins = 40
energyMin = -0.5
energyMax = 39.5

hMC = Hist(energyNumBins, energyMin, energyMax, name='hSim', title='Simulated Spectrum', drawstyle='hist')

tMC.Draw('etot_target', hist=hMC, selection=xAll)
hMC.Sumw2()
gMC = convert_hist_to_graph_with_poisson_errors(hMC)

aoEnergy = data_for_graphs(gMC)
# light yield for each energy
lLightYields = np.array([10.0 for i in xrange(aoEnergy.get_num_elements())])
aPhotonSpectrum = convert_energy_to_photons(aoEnergy, lLightYields)
aIdealPE = convert_photons_to_PE(aPhotonSpectrum, .118, .005, s1NumBins, s1Min, s1Max)
aSmearedPE = smear_PE_spectrum(aIdealPE, [(0.6, 0.5)], s1NumBins, s1Min, s1Max)

aS1MCSpec = np.multiply(aSmearedPE[:,0].T, aoTACEfficiency.get_y_values()[0])[0]
aS1MCSpec = np.squeeze(np.asarray(aS1MCSpec))

numCountsRealS1Spec = sum(aoS1.get_y_values().A1)
numCountsMCS1Spec = sum(np.asarray(aS1MCSpec))
aS1MCSpec = aS1MCSpec*numCountsRealS1Spec/numCountsMCS1Spec

#print len(aS1MCSpec)
gMCS1Spec = root.TGraph(len(aS1MCSpec), aoS1.get_x_values().A1, aS1MCSpec)
gMCS1Spec.SetLineColor(root.kRed)
gMCS1Spec.Draw('same')
c1.Update()

"""
c2 = Canvas()
gMC.Draw()
c2.Update()
"""

raw_input('Press enter to continue')






