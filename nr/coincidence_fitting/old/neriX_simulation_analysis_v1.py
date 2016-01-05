import ROOT as root
from ROOT import gROOT
import sys, os, root_numpy, threading, random, emcee, corner
from rootpy import stl
from rootpy.io import File
from rootpy.tree import Tree, TreeModel, TreeChain
from rootpy.plotting import Hist, Hist2D, Canvas, Graph, func
import neriX_simulation_datasets
import numpy as np
from math import exp, factorial
from iminuit import Minuit
from scipy import optimize, misc
import copy_reg, types, pickle
from subprocess import call
# for debugging
import matplotlib.pyplot as plt
import matplotlib.mlab as mlab

import warnings
warnings.filterwarnings('error')

# MUST INCLUDE TYPES TO AVOID SEG FAULT
stl.vector(stl.vector('float'))
stl.vector(stl.vector('int'))

def reduce_method(m):
	return (getattr, (m.__self__, m.__func__.__name__))



def produce_binomial_probabilities_range(probabiltyOfSuccuess, numberOfTrials, lowBound, highBound, stepSize = 1):
	# make sure that bounds and stepsize are integers
	assert isinstance(numberOfTrials, int) and isinstance(lowBound, int) and isinstance(highBound, int) and isinstance(stepSize, int) and (highBound - lowBound) % stepSize == 0
	
	if probabiltyOfSuccuess < 0 or probabiltyOfSuccuess > 1:
		print 'Probability takes on value less than zero or greater than one - return zeros.'
		return np.zeros(len(highBound-lowBound))
	
	if stepSize != 1:
		print 'WARNING: This function should be used with a step size of one unless one handles rebinning properly (currently does not).'
	
	aBinomial = np.zeros((highBound - lowBound) / stepSize)

	for i, numberOfSucceses in enumerate(xrange(lowBound, highBound, stepSize)):
		if numberOfSucceses < 0 or numberOfSucceses > numberOfTrials:
			aBinomial[i] = 0.
		else:
			nChooseK = misc.comb(numberOfTrials, numberOfSucceses)
			probabilityTerm = probabiltyOfSuccuess**numberOfSucceses * (1-probabiltyOfSuccuess)**(numberOfTrials-numberOfSucceses)
			aBinomial[i] = nChooseK*probabilityTerm

	return aBinomial



def produce_gaussian_probabilities_range(aXValues, mu, sigma):
	try:
		aProb = mlab.normpdf(aXValues, mu, sigma)
	except RuntimeWarning:
		aProb = np.zeros(aXValues.size)
	return aProb




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


def smart_stirling(lInput):
	if not hasattr(lInput, '__iter__'):
		lInput = [lInput]
	aOutput = np.array(lInput)
	for index, input in enumerate(lInput):
		if input < 10:
			aOutput[index] = np.log(factorial(input))
		else:
			aOutput[index] = input*np.log(input) - input
	return aOutput


def smart_log(lInput):
	if not hasattr(lInput, '__iter__'):
		lInput = [lInput]
	aOutput = np.array(lInput)
	for index, input in enumerate(lInput):
		if input < 1e-310:
			aOutput[index] = -1000
		else:
			aOutput[index] = np.log(input)
	return aOutput


# class to make arrays accessible by numpy
class easy_graph:
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

		self.size = self.get_y_values().size


	def get_num_elements(self):
		return self.numElements


	def create_list_from_graph(self, graphToConvert, parameterToGrab):
		if not isinstance(parameterToGrab, str):
			print 'Must give string parameter'
			sys.exit()
		
		functionToCall = self.dFunctions[parameterToGrab]
		tempArray = functionToCall()
		tempArray.SetSize(self.get_num_elements())
		self.dArrays[parameterToGrab] = np.array(tempArray, 'f', ndmin=2)


	def get_lower_bound(self):
		return self.dArrays['x'][0, 0] - 0.5*self.get_bin_size()


	def get_upper_bound(self):
		return self.dArrays['x'][0, -1] + 0.5*self.get_bin_size()


	def get_bin_size(self):
		return (self.dArrays['x'][0, 1] - self.dArrays['x'][0, 0])


	def get_x_values(self):
		return self.dArrays['x']


	def get_y_values(self):
		return self.dArrays['y']


	def get_x_bin_edges(self):
		return np.array([self.get_lower_bound() + i*self.get_bin_size() for i in xrange(self.get_num_elements() + 1)]) # + 1 to capture last bin



class easy_function:
	def __init__(self, func_tf1, numBins, lowerBound, upperBound, type='f'):
		"""
		if not isinstance(func_tf1, root.TF1) or not isinstance(func_tf1, func.F1):
			print 'Must input TF1 to convert to easy_function'
			sys.exit()
		"""
		
		self.type = type
		self.originalFunction = func_tf1

		# get original parameters and uncertainties
		self.numParameters = func_tf1.GetNumberFreeParameters()
		self.lOriginalParameters = [0 for i in xrange(self.numParameters)]
		self.lOriginalParErrors = [0 for i in xrange(self.numParameters)]
		for i in xrange(self.numParameters):
			self.lOriginalParameters[i] = func_tf1.GetParameter(i)
			self.lOriginalParErrors[i] = func_tf1.GetParError(i)

		# copy orig parameters to current parameters
		# these will later be changed using gaussian
		# with mean of original parameter and stdev
		# of uncertainty
		self.lCurrentParameters = list(self.lOriginalParameters)

		# set parameters for graph
		self.numBins = numBins
		self.lowerBound = lowerBound
		self.upperBound = upperBound
		self.binWidth = (self.upperBound-self.lowerBound) / float(self.numBins)

		# get arrays ready
		self.fill_arrays_for_graph()



	def fill_arrays_for_graph(self):
		# create function
		function = root.TF1(self.originalFunction)
		for i, parameterValue in enumerate(self.lCurrentParameters):
			function.SetParameter(i, parameterValue)
		
	
		# reinitialize arrays
		self.aX = np.zeros(self.numBins, self.type)
		self.aY = np.zeros(self.numBins, self.type)
		self.aXUpper = np.zeros(self.numBins, self.type)
		self.aXLower = np.zeros(self.numBins, self.type)
		self.aYUpper = np.zeros(self.numBins, self.type)
		self.aYLower = np.zeros(self.numBins, self.type)

		for binNumber in xrange(self.numBins):
			binCenter = self.lowerBound + (binNumber*self.binWidth) + self.binWidth/2.
			
			# set X bin values
			self.aX[binNumber] = binCenter
			self.aXLower[binNumber] = binCenter - (self.binWidth/2.)
			self.aXUpper[binNumber] = binCenter + (self.binWidth/2.)

			# set Y bin values
			# yLower and yUpper are set by max in x range
			# since this is the most conservative approach
			self.aY[binNumber] = function.Eval(self.aX[binNumber])
			self.aYLower[binNumber] = function.GetMinimum(self.aXLower[binNumber], self.aXUpper[binNumber])
			self.aYUpper[binNumber] = function.GetMaximum(self.aXLower[binNumber], self.aXUpper[binNumber])
			# can optimize for speed by changing epsilon and
			# max number of iterations in GetMin/GetMax
			



	def fill_graph_from_arrays(self):
		self.fill_arrays_for_graph()
		return root.TGraphAsymmErrors(self.numBins, self.aX, self.aY, self.aXLower, self.aXUpper, self.aYLower, self.aYUpper)



	# returns likelihood of best fit and easy graph of best fit
	def get_eg_best_fit(self):
		self.lCurrentParameters = list(self.lOriginalParameters)
		self.fill_arrays_for_graph()
		gBestFit = self.fill_graph_from_arrays()
		likelihood = exp(0.**2/1.)
		return likelihood, easy_graph(gBestFit)



	def get_eg_normally_distributed_par(self):
		# potentially add random.seed argument

		# set current parameters
		lRandomVariables = [0 for i in xrange(self.numParameters)]
		for i in xrange(self.numParameters):
			lRandomVariables[i] = random.gauss(0., 1.) # normal distribution
			self.lCurrentParameters[i] = self.lOriginalParameters[i] + (lRandomVariables[i]*self.lOriginalParErrors[i])

		# multiply likelihoods of each variable
		likelihood = 1.
		for value in lRandomVariables:
			likelihood *= exp(-value**2/1.)

		return likelihood, easy_graph(self.fill_graph_from_arrays())



	def make_graph_from_input(self, *inputParameters):
		if not len(inputParameters) == self.numParameters:
			print '\n\nFAILURE: Number of parameters given does not match the number of parameters of the function!'
			print 'Given %d parameters but require %d.' % (len(inputParameters), self.numParameters)
			sys.exit()

		likelihood = 1.
		for i, value in enumerate(inputParameters):
			self.lCurrentParameters[i] = self.lOriginalParameters[i] + (value*self.lOriginalParErrors[i])
			likelihood *= exp(-value**2/1.)

		return likelihood, easy_graph(self.fill_graph_from_arrays())














class neriX_simulation_analysis:
	def __init__(self, run, anodeVoltage, cathodeVoltage, angle):

		# ------------------------------------------------
		# Pull filenames and create save paths
		# ------------------------------------------------

		(data_root_file, mc_root_file, tac_efficiency_root_file, peak_finder_efficiency_root_file, g1_root_file, spe_root_file) = neriX_simulation_datasets.run_files[(run, anodeVoltage, cathodeVoltage, angle)]
		self.anodeSetting, self.cathodeSetting, self.degreeSetting = anodeVoltage, cathodeVoltage, angle
		
		self.sPathForSave = './mcmc_results/%ddeg_%.3fkV_%.1fkV/' % (self.degreeSetting, self.cathodeSetting, self.anodeSetting)
		self.sPathForOldFiles = self.sPathForSave + 'previous_results/'
		self.sPathForChains = self.sPathForSave + 'chains/'
		self.sNameOfCurrentChain = 'chain_'
		
		if not os.path.isdir(self.sPathForSave):
			call(['mkdir', self.sPathForSave])
			call(['mkdir', self.sPathForOldFiles])
			call(['mkdir', self.sPathForChains])
		else:
			for file in os.listdir(self.sPathForSave):
				if file == 'chains':
					continue
				call(['mv', self.sPathForSave + file, self.sPathForOldFiles + file])
			self.sNameOfCurrentChain += '%04d.p' % len(os.listdir(self.sPathForChains))
			
			

		# ------------------------------------------------
		# Set paths to files and grab constants
		# ------------------------------------------------

		dNamesForFiles = {'data':data_root_file, 'tac_efficiency':tac_efficiency_root_file, 'peak_finder_efficiency':peak_finder_efficiency_root_file, 'MC':mc_root_file}
		dFilesForAnalysis = {}
		
		# count function calls for tracking status
		self.photonYieldFuncCalls = 0
		
		# load constants for matching
		self.numRandomTrials = neriX_simulation_datasets.num_random_trials
		self.maxPhotonYield = neriX_simulation_datasets.max_photon_yield
		
		# convert PE refers to converting photons to PE
		self.extraBinsSmearPE = neriX_simulation_datasets.smear_pe_extra_bins
		
		# smear PE refers to spe smearing of PE spectrum
		self.extraBinsConvertPE = neriX_simulation_datasets.convert_pe_extra_bins
		

		for fileType in dNamesForFiles:
			fileName = dNamesForFiles[fileType]
			if fileName[-5:] != '.root':
				fileName += '.root'
			
			if fileType == 'data':
				fileName = neriX_simulation_datasets.pathToData + fileName
			elif fileType == 'tac_efficiency':
				fileName = neriX_simulation_datasets.pathToEfficiencyFiles + fileName
			elif fileType == 'peak_finder_efficiency':
				fileName = neriX_simulation_datasets.pathToEfficiencyFiles + fileName
			elif fileType == 'MC':
				fileName = fileName

			# add file to dictionary
			dFilesForAnalysis[fileType] = fileName


		# ------------------------------------------------
		# Either set g1 and uncertainty from file or set
		# them from hardcoded degaults in datasets
		# ------------------------------------------------

		if not g1_root_file:
			self.g1Value = neriX_simulation_datasets.default_g1
			self.g1Uncertainty = neriX_simulation_datasets.default_g1_uncertainty
			print '\n\nNOTE: No file given for g1 - will use hardcoded value of %.3f +/- %.3f \n\n' % (self.g1Value, self.g1Uncertainty)

		else:
			print '\n\nWARNING: Need to implement code to read g1 from file - for now using defaults \n\n'
			#self.g1Value, self.g1Uncertainty = self.read_g1_from_file(self, g1_root_file)
			self.g1Value = neriX_simulation_datasets.default_g1
			self.g1Uncertainty = neriX_simulation_datasets.default_g1_uncertainty
		
		
		
		# ------------------------------------------------
		# Either set spe resolution from file or set
		# them from hardcoded degaults in datasets
		# ------------------------------------------------

		if not spe_root_file:
			self.speResValue = neriX_simulation_datasets.default_spe_res
			self.speResUncertainty = neriX_simulation_datasets.default_spe_res_uncertainty
			print '\n\nNOTE: No file given for spe resoltuion - will use hardcoded value of %.3f +/- %.3f \n\n' % (self.speResValue, self.speResUncertainty)

		else:
			print '\n\nWARNING: Need to implement code to read spe resolution from file - for now using defaults \n\n'
			#self.g1Value, self.g1Uncertainty = self.read_g1_from_file(self, g1_root_file)
			self.speResValue = neriX_simulation_datasets.default_g1
			self.speResUncertainty = neriX_simulation_datasets.default_g1_uncertainty


		# ------------------------------------------------
		# Load histograms of data and convert them to
		# easy graphs
		# ------------------------------------------------

		# load S1
		fData = File(dFilesForAnalysis['data'])
		self.hS1 = fData.hS1
		self.hS1.Sumw2()
		gS1 = convert_hist_to_graph_with_poisson_errors(self.hS1)
		self.egS1 = easy_graph(gS1)
		
		self.s1NumBins = self.egS1.get_num_elements()
		self.s1LowerBound = self.egS1.get_lower_bound()
		self.s1UpperBound = self.egS1.get_upper_bound()
		
		self.numCountsData = self.hS1.Integral()
		if self.numCountsData <=0:
			print '\n\nFAILURE: Number of data points found is zero.  Please check S1 or S2 spectrum.'
			sys.exit()
		
		# load S2
		self.hS2 = fData.hS2
		self.hS2.Sumw2()
		gS2 = convert_hist_to_graph_with_poisson_errors(self.hS2)
		self.egS2 = easy_graph(gS2)
		
		self.s2NumBins = self.egS2.get_num_elements()
		self.s2LowerBound = self.egS2.get_lower_bound()
		self.s2UpperBound = self.egS2.get_upper_bound()

		# ------------------------------------------------
		# Load MC file or create reduced file if not present
		# and load into easy_graph
		# ------------------------------------------------

		self.egMC = self.load_mc_data(dFilesForAnalysis['MC'])

		# ------------------------------------------------
		# Load fSigmoid from tac efficiency file
		# and load into easy_function
		# ------------------------------------------------
		
		# load tac efficiency file and store in easy function
		fTACEfficiency = File(dFilesForAnalysis['tac_efficiency'])
		tf1TacEfficiency = fTACEfficiency.fSigmoid
		self.efTacEfficiency = easy_function(tf1TacEfficiency, self.s1NumBins, self.s1LowerBound, self.s1UpperBound)
			
		# ------------------------------------------------
		# Load peak finder efficiency from file
		# and load into easy_function
		# ------------------------------------------------

		fPFEfficiency = File(dFilesForAnalysis['peak_finder_efficiency'])
		tf1PFEfficiency = fPFEfficiency.fPeakFindEff
		self.efPFEfficiency = easy_function(tf1PFEfficiency, self.s1NumBins, self.s1LowerBound, self.s1UpperBound)
		
		# test cases
		#print self.efTacEfficiency.get_eg_best_fit()[1].get_y_values()
		#print self.efTacEfficiency.make_graph_from_input(-1, -1)[0]**0.5
		#print self.efTacEfficiency.make_graph_from_input(-1, -1)[1].get_y_values()
		
		



	def read_g1_from_file(self, g1_root_file):
		return (0, 0)



	def load_mc_data(self, mc_root_file):
		# check if reduced file exists
		pathToReducedSimulationFiles = neriX_simulation_datasets.pathToReducedSimulationFiles + mc_root_file
		if not os.path.exists(pathToReducedSimulationFiles):
			# load the raw data file
			pathToSimulatedFile = neriX_simulation_datasets.pathToSimulationFiles
			fSimulation = File(pathToSimulatedFile + mc_root_file)

			# set cuts
			xRadius = '(sqrt(xpos[0]**2+ypos[0]**2) < 15)'
			xZ = '(zpos[0]>-20 && zpos[0]<-4)'
			xSingleScatter = '(nsteps_target==1)'
			xLiqSciHeight = '(etotliqsci>700)'
			xLXeEnergy = '(etot_target>0)'

			xTOF = '(timeliqsci-tpos[0]>30 && timeliqsci-tpos[0]<50)'

			xAll = '%s && %s && %s && %s && %s && %s' % (xRadius, xZ, xSingleScatter, xLiqSciHeight, xLXeEnergy, xTOF)

			# pull min, max, and bins
			energyMin = neriX_simulation_datasets.energyMin
			energyMax = neriX_simulation_datasets.energyMax
			energyNumBins = neriX_simulation_datasets.energyNumBins

			# create and fill histogram
			tMC = fSimulation.t2
			hMC = Hist(energyNumBins, energyMin, energyMax, name='hMC', title='Simulated Spectrum', drawstyle='hist')
			tMC.Draw('etot_target', hist=hMC, selection=xAll)
			hMC.Sumw2()

			# draw histogram to new file for quick retrieval
			fReduced = File(pathToReducedSimulationFiles, 'recreate')
			hMC.Write()
			fReduced.Close()

		fReduced = File(pathToReducedSimulationFiles, 'read')
		hMC = fReduced.hMC
		gMC = convert_hist_to_graph_with_poisson_errors(hMC)
		return easy_graph(gMC)




	# get likelihood and g1 given random variable (nuissance parameter)
	# g1RV should be normally distributed
	def get_g1_default(self, g1RV):
		return exp(-g1RV**2/1.), self.g1Value + (g1RV*self.g1Uncertainty)
	
	
	
	def get_spe_res_default(self, speResRV):
		return exp(-speResRV**2/1.), self.speResValue + (speResRV*self.speResUncertainty)
	
	
	
	# set prior functions for photon yield and resolution
	# equivalent to setting bounds for fit
	def get_prior_log_likelihood_photon_yield(self, photonYield):
		if photonYield < 0 or photonYield > self.maxPhotonYield:
			return -np.inf
		else:
			return 0



	def get_prior_log_likelihood_resolution(self, intrinsicResolution):
		if intrinsicResolution < 0 or intrinsicResolution > 4:
			return -np.inf
		else:
			return 0



	def get_prior_log_likelihood_nuissance(self, likelihoodNuissance):
		if likelihoodNuissance > 1e-8:
			return np.log(likelihoodNuissance)
		else:
			return -np.inf



	# returns converted array convoluted with Gaussian probabilities
	# sMeanComponents: (a, m) s.t. mean = a*x^m where x is x-axis variable
	# ex for light yield ==> (light yield, 1)
	# sWidthComponents: (b, o) s.t. sigma = b*x^o
	# ex for resolution ==> (res, 0.5)
	def gaussian_step(self, aBeforeCenters, aBeforeValues, aAfterBinCenters, sMeanComponents, sWidthComponents):
	
		aConversionMatrix = np.array([ [0. for i in xrange(aBeforeCenters.size)] for j in xrange(aAfterBinCenters.size) ])
	
		for bin, center in np.ndenumerate(aBeforeCenters):
			# take second index of energy bin (column) since only one row
			bin = bin[1]
			
			aGausProb = produce_gaussian_probabilities_range(aAfterBinCenters, sMeanComponents[0]*center**sMeanComponents[1], sWidthComponents[0]*center**sWidthComponents[1])

			aConversionMatrix[:, bin] = aGausProb.flatten()

		return aConversionMatrix.dot(aBeforeValues)
		
		
		
		
	# returns convolution binomial distribution
	def binomial_step(self, aBeforeCenters, aBeforeValues, aAfterBinCenters, probabilityOfSuccuess, stepSize=1):
	
		aConversionMatrix = np.array([ [0. for i in xrange(aBeforeCenters.size)] for j in xrange(aAfterBinCenters.size) ])
		lowBound = aAfterBinCenters[0]
		highBound = aAfterBinCenters[-1] + stepSize
	
		for bin, center in np.ndenumerate(aBeforeCenters):
			# take second index of energy bin (column) since only one row
			bin = bin[1]
			
			aBinomialProb = produce_binomial_probabilities_range(probabilityOfSuccuess, center, lowBound, highBound, stepSize)

			aConversionMatrix[:, bin] = aBinomialProb.flatten()

		return aConversionMatrix.dot(aBeforeValues)

	



	# function will return both likelihood and graph with
	# matched spectrum
	# will simply call this for likelihood maximization
	# and take just the likelihood - can call after that with
	# best fit to get spectrum
	def perform_mc_match_photon_yield(self, photonYield, intrinsicResolution, g1RV, speResRV, par0TacEffRV, par1TacEffRV, par0PFEffRV, par1PFEffRV, drawFit=False):
	
		"""
		if self.photonYieldFuncCalls % 10 == 0:
			print '\n\nFunction call: %d\n\n' % self.photonYieldFuncCalls,
		self.photonYieldFuncCalls += 1
		"""

		# ------------------------------------------------
		# Load required constants, graphs, and likelihoods
		# for later use in matching code
		# ------------------------------------------------

		g1Likelihood, g1Value = self.get_g1_default(g1RV)
		g1LogLikelihood = self.get_prior_log_likelihood_nuissance(g1Likelihood)
		
		speResLikelihood, speRes = self.get_spe_res_default(speResRV)
		speResLogLikelihood = self.get_prior_log_likelihood_nuissance(speResLikelihood)

		tacEffLikelihood, egTacEff = self.efTacEfficiency.make_graph_from_input(par0TacEffRV, par1TacEffRV)
		tacEffLogLikelihood = self.get_prior_log_likelihood_nuissance(tacEffLikelihood)

		pfEffLikelihood, egPFEff = self.efPFEfficiency.make_graph_from_input(par0PFEffRV, par1PFEffRV)
		pfEffLogLikelihood = self.get_prior_log_likelihood_nuissance(pfEffLikelihood)

		pYLogLikelihood = self.get_prior_log_likelihood_photon_yield(photonYield)
		resLogLikelihood = self.get_prior_log_likelihood_resolution(intrinsicResolution)

		priorLogLikelihoods = g1LogLikelihood + speResLogLikelihood + tacEffLogLikelihood + pfEffLogLikelihood + pYLogLikelihood + resLogLikelihood

		# test cases
		#print priorLikelihoods
		#print egTacEff.get_y_values()
		#print egPFEff.get_y_values()


		# ------------------------------------------------
		# Initialize required constants, matrices, and arrays
		# ------------------------------------------------

		# will reuse aHistogram throughout to save memory

		# arrays and matrices for energy smearing
		#aEnergySmearingMatrix = np.array([ [0. for i in xrange(self.egMC.get_num_elements())] for j in xrange(self.egMC.get_num_elements()) ])

		# arrays and matrices for energy to photon conversion
		numPhotonBins = int(self.maxPhotonYield * self.egMC.get_upper_bound())
		aPhotonBinCenters = np.array([[i for i in xrange(numPhotonBins)]])
		aPhotonBinEdges = np.array([i - 0.5 for i in xrange(numPhotonBins+1)]) # +1 to get right edge too!
		aPhotonConversionMatrix = np.array([ [0. for i in xrange(self.egMC.get_num_elements())] for j in xrange(numPhotonBins) ])

		# arrays and matrices for photon to PE conversion
		
		peBinWidth = (self.s1UpperBound - self.s1LowerBound) / float(self.s1NumBins)
		aPEBinCenters = np.array([[i + peBinWidth/2. for i in drange(self.s1LowerBound, self.s1UpperBound, peBinWidth)]])
		aPEBinEdgesConversion = np.array([i for i in drange(self.s1LowerBound-self.extraBinsConvertPE*peBinWidth+0.001, self.s1UpperBound + self.extraBinsConvertPE*peBinWidth, peBinWidth)])
		aPEBinEdgesSmearing = np.array([i for i in drange(self.s1LowerBound-self.extraBinsSmearPE*peBinWidth+0.001, self.s1UpperBound + self.extraBinsSmearPE*peBinWidth, peBinWidth)])
		aPEConversionMatrix = np.array([ [0. for i in xrange(numPhotonBins)] for j in xrange(self.s1NumBins) ])
		
		# matrix for PE smearing with SPE res
		aPESmearingSPEMatrix = np.array([ [0. for i in xrange(self.s1NumBins)] for j in xrange(self.s1NumBins) ])
		
		# matrix for PE smearing with intrinsic res
		aPESmearingIntrinsicMatrix = np.array([ [0. for i in xrange(self.s1NumBins)] for j in xrange(self.s1NumBins) ])


		# ------------------------------------------------
		# Smear MC energy spec with intrinsic resolution
		# ------------------------------------------------
		"""
		for energyBin, energy in np.ndenumerate(self.egMC.get_x_values()):
			# take second index of energy bin (column) since only one row
			energyBin = energyBin[1]

			if energy > 0:
				try:
					aEnergySmearResults = np.random.normal(energy, energy**0.5*intrinsicResolution, self.numRandomTrials)
				except:
					aEnergySmearResults = [0]
			else:
				aEnergySmearResults = [0]

			# density is normalized and accounts for bin width so
			# need to multiply through bin width
			aHistogram, dummy = np.histogram(aEnergySmearResults, bins=self.egMC.get_x_bin_edges(), density=True)
			aHistogram *= self.egMC.get_bin_size()
			
			aEnergySmearingMatrix[:, energyBin] = np.array(aHistogram).T

		# final array of smeared energies with intrinsic resolution
		# to bypass simply set aSmearedEnergies equal
		# to self.egMC.get_y_values()
		aSmearedEnergies = aEnergySmearingMatrix.dot(self.egMC.get_y_values().T)
		#print aSmearedEnergies
		"""

		# ------------------------------------------------
		# Convert MC energy spec to photon spec
		# ------------------------------------------------

		for energyBin, energy in np.ndenumerate(self.egMC.get_x_values()):
			# take second index of energy bin (column) since only one row
			energyBin = energyBin[1]
			muForPoisson = photonYield*energy
			try:
				aPoissonResults = np.random.poisson(muForPoisson, self.numRandomTrials)
			except:
				aPoissonResults = [0]

			# histogram using poisson stats
			aHistogram, dummy = np.histogram(aPoissonResults, bins=aPhotonBinEdges, density=True)
			# do not need to normalize since bin size is 1 photon

			aPhotonConversionMatrix[:, energyBin] = np.array(aHistogram).T

		aPhotonSpectrum = aPhotonConversionMatrix.dot(self.egMC.get_y_values().T)
		#print aPhotonSpectrum


		# ------------------------------------------------
		# Convert photon spec to PE spec
		# ------------------------------------------------


		for photonBin, numPhotons in np.ndenumerate(aPhotonBinCenters):
			# take second index of energy bin (column) since only one row
			photonBin = photonBin[1]
			
			
			# use Poisson
			# feed in g1 value distributed on gaussian by uncertainty
			"""
			muForPoisson = g1Value*numPhotons
			try:
				aPoissonResults = np.random.poisson(muForPoisson, self.numRandomTrials)
			except:
				aPoissonResults = [0]
			"""
			
			# use binomial
			try:
				aPoissonResults = np.random.binomial(numPhotons, g1Value, self.numRandomTrials)
			except:
				aPoissonResults = [0]
			

			#print muForPoisson
			##print list(aPoissonResults)
			#n, bins, patches = plt.hist(aPoissonResults, len(aPEBinEdgesConversion), normed=1)
			#l = plt.plot(bins)
			#plt.show()
			
			# need to include zero PE bin and then drop it
			# also need to scale each value back up to probability
			# since all bins are normalized by bin width
			aHistogram, dummy = np.histogram(aPoissonResults, bins=aPEBinEdgesConversion, density=True)
			aHistogram *= peBinWidth
			
			aHistogram = aHistogram[self.extraBinsConvertPE:-(self.extraBinsConvertPE-1)]
			aPEConversionMatrix[:, photonBin] = np.array(aHistogram).T
		
		
		aConvertedPESpectrum = aPEConversionMatrix.dot(aPhotonSpectrum)
		#print aConvertedPESpectrum


		# ------------------------------------------------
		# Smear PE spectrum with spe resolution
		# ------------------------------------------------


		"""
		for peBin, numPE in np.ndenumerate(aPEBinCenters):
			# take second index of energy bin (column) since only one row
			peBin = peBin[1]
			
			# get gaussian results (only spe for now!!!)
			#print numPE, lResolutionsForGaussians[0][resIndex], lResolutionsForGaussians[0][powerIndex], lResolutionsForGaussians[0][resIndex]*numPE**lResolutionsForGaussians[0][powerIndex]
			if numPE < 0.1: # exclude 0 PE bin (should have no smearing (was 0.9)
				aGaussianResults = [0]
			else:
				try:
					aGaussianResults = np.random.normal(numPE, speRes*numPE**0.5, self.numRandomTrials)
				except:
					aGaussianResults = [0]
			
			# need to include zero PE bin and then drop it
			# also need to scale each value back up to probability
			# since all bins are normalized by bin width
			aHistogram, dummy = np.histogram(aGaussianResults, bins=aPEBinEdgesSmearing, density=True)
			
			#grab part of histogram we care about
			aHistogram = peBinWidth*aHistogram[self.extraBinsSmearPE:-(self.extraBinsSmearPE-1)]
			aPESmearingSPEMatrix[:, peBin] = np.array(aHistogram).T
		
		aSmearedPESpectrumSPE = aPESmearingSPEMatrix.dot(aConvertedPESpectrum)
		#print aSmearedPESpectrum
		"""
		aSmearedPESpectrumSPE = self.gaussian_step(aPEBinCenters, aConvertedPESpectrum, aPEBinCenters, (1., 1.), (speRes, 0.5))
		
		# ------------------------------------------------
		# Smear PE spectrum with intrinsic resolution
		# ------------------------------------------------


		"""
		for peBin, numPE in np.ndenumerate(aPEBinCenters):
			# take second index of energy bin (column) since only one row
			peBin = peBin[1]
			
			# get gaussian results (only spe for now!!!)
			#print numPE, lResolutionsForGaussians[0][resIndex], lResolutionsForGaussians[0][powerIndex], lResolutionsForGaussians[0][resIndex]*numPE**lResolutionsForGaussians[0][powerIndex]
			if numPE < 0.1: # exclude 0 PE bin (should have no smearing (was 0.9)
				aGaussianResults = [0]
			else:
				try:
					aGaussianResults = np.random.normal(numPE, intrinsicResolution*numPE**0.5, self.numRandomTrials)
					
					
					#print numPE, intrinsicResolution*numPE**0.5
					#print list(aGaussianResults)
					#n, bins, patches = plt.hist(aGaussianResults, len(aPEBinEdgesSmearing), normed=1)
					#y = mlab.normpdf( bins, numPE, intrinsicResolution*numPE**0.5)
					#print y
					#l = plt.plot(bins, y, 'r--', linewidth=1)
					#plt.show()
					
				
				except:
					aGaussianResults = [0]
			
			# need to include zero PE bin and then drop it
			# also need to scale each value back up to probability
			# since all bins are normalized by bin width
			aHistogram, dummy = np.histogram(aGaussianResults, bins=aPEBinEdgesSmearing, density=True)
			
			#grab part of histogram we care about
			aHistogram = peBinWidth*aHistogram[self.extraBinsSmearPE:-(self.extraBinsSmearPE-1)]
			aPESmearingIntrinsicMatrix[:, peBin] = np.array(aHistogram).T
		
		aSmearedPESpectrum = aPESmearingIntrinsicMatrix.dot(aSmearedPESpectrumSPE)
		#print aSmearedPESpectrum
		"""
		aSmearedPESpectrum = self.gaussian_step(aPEBinCenters, aConvertedPESpectrum, aPEBinCenters, (1., 1.), (intrinsicResolution, 0.5))


		# ------------------------------------------------
		# Apply TAC and PF efficiency
		# ------------------------------------------------

		assert egTacEff.size == aSmearedPESpectrum.size and egPFEff.size == aSmearedPESpectrum.size

		aMatchingMCSpectrum = egTacEff.get_y_values().flatten()*egPFEff.get_y_values().flatten()*aSmearedPESpectrum.flatten()
		
		# normalize spectrum to number of counts in data
		aMatchingMCSpectrum *= self.numCountsData / float(sum(aMatchingMCSpectrum))
		#print aMatchingMCSpectrum
		
		
		# ------------------------------------------------
		# Draw fit if requested
		# ------------------------------------------------
	
		if drawFit:
			c1 = Canvas()
			self.hS1.Draw()
			gMCS1Spec = root.TGraphAsymmErrors(len(aMatchingMCSpectrum), aPEBinCenters.flatten(), aMatchingMCSpectrum.flatten(), np.array([0.5 for i in xrange(len(aMatchingMCSpectrum))]), np.array([0.5 for i in xrange(len(aMatchingMCSpectrum))]),  0.2*aMatchingMCSpectrum.flatten(), 0.2*aMatchingMCSpectrum.flatten())
		
			gMCS1Spec.SetLineColor(root.kBlue)
			gMCS1Spec.SetFillColor(root.kBlue)
			gMCS1Spec.SetFillStyle(3005)
			gMCS1Spec.Draw('3 same')
			c1.Update()
		
			raw_input('Press enter to continue...')
			del c1


		# ------------------------------------------------
		# Compare MC and data to find likelihood
		# ------------------------------------------------

		logLikelihoodMatching = np.sum( self.egS1.get_y_values().flatten()*smart_log(aMatchingMCSpectrum) - aMatchingMCSpectrum - smart_stirling(self.egS1.get_y_values().flatten()) )
		#print logLikelihoodMatching, smart_log(priorLikelihoods)
		totalLogLikelihood = logLikelihoodMatching + priorLogLikelihoods
		#print totalLogLikelihood#, aMatchingMCSpectrum
		if np.isnan(totalLogLikelihood):
			return -np.inf, aMatchingMCSpectrum
		else:
			return totalLogLikelihood, aMatchingMCSpectrum



	def mcmc_func_photon_yield_mc_matching(self, aParametersToFit, drawFit=False):
		#print aParametersToFit
		assert len(aParametersToFit) == 8
		return self.perform_mc_match_photon_yield(*aParametersToFit, drawFit=drawFit)[0]



	def run_mcmc(self, sMeasurement, sParameters, numWalkers, numSteps, numThreads, fractionalDeviationStartPos = 1e-3):
		if sMeasurement == 'photon_yield':
			numDim = 8
			assert len(sParameters) == numDim
			func = self.mcmc_func_photon_yield_mc_matching
		else:
			print 'Currently not setup to handle the input measurement: %s' % sMeasurement
			sys.exit()


		initialParameters = np.zeros(numDim)
		lLabelsForCorner = ['' for i in xrange(numDim)]
		for i, set in enumerate(sParameters):
			lLabelsForCorner[i] = '%s' % set[0]
			initialParameters[i] = set[1]

		#numDim, numWalkers, numSteps = 8, 160, 1
		posWalkers = [(np.random.randn(numDim)+initialParameters)*fractionalDeviationStartPos + initialParameters for i in xrange(numWalkers)]
		
		

		print '\n\nBeginning MCMC sampler\n\n'
		print '\nNumber of walkers * number of steps = %d * %d = %d function calls\n' % (numWalkers, numSteps, numWalkers*numSteps)
		sampler = emcee.EnsembleSampler(numWalkers, numDim, func, threads=numThreads)
		
		# run mcmc sampler
		currentStep = 1
		for pos, lnprob, state in sampler.sample(posWalkers, iterations=numSteps):
			print '\nCompleted step %d of %d.\n' % (currentStep, numSteps)
			currentStep += 1
			

		#sampler.run_mcmc(posWalkers, numSteps) # shortcut of above method
		pickle.dump(sampler.chain, open(self.sPathForSave + 'sampler_chain.p', 'w'))
		pickle.dump(sampler.chain, open(self.sPathForChains + self.sNameOfCurrentChain, 'w'))
		pickle.dump(sampler.acceptance_fraction, open(self.sPathForSave + 'sampler_acceptance_fraction.p', 'w'))
		try:
			pickle.dump(sampler.acor, open(self.sPathForSave + 'sampler_acor.p', 'w'))
		except:
			print '\n\nWARNING: Not enough sample points to estimate the autocorrelation time - this likely means that the fit is bad since the burn-in time was not reached.\n\n'
		
		samples = sampler.chain[:, 5:, :].reshape((-1, numDim))
		try:
			fig = corner.corner(samples, labels=lLabelsForCorner)
			fig.savefig(self.sPathForSave + 'corner_plot.png')
		except AssertionError:
			print 'Too few samples (number of samples fewer than number of dimensions) - cannot reasonably create correlation plots.'
			sys.exit()





if __name__ == '__main__':
	copy_reg.pickle(types.MethodType, reduce_method)

	test = neriX_simulation_analysis(15, 4.5, 1.054, 45)
	test.perform_mc_match_photon_yield(9.5, 2.0, 0, 0, 0, 0, 0, 0, drawFit=True)
	
	sParametersPhotonYield = (('photon_yield', 9.0), ('res_intrinsic', 0.5), ('n_g1', 0), ('n_res_spe', 0), ('n_par0_tac_eff', 0), ('n_par1_tac_eff', 0), ('n_par0_pf_eff', 0), ('n_par1_pf_eff', 0))

	# try using emcee to fit
	#test.run_mcmc('photon_yield', sParametersPhotonYield, 160, 300, 15)


	# perform_mc_match_photon_yield(self, photonYield, intrinsicResolution, g1RV, speResRV, par0TacEffRV, par1TacEffRV, par0PFEffRV, par1PFEffRV, drawFit=False)




