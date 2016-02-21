
#matplotlib.use('Agg')
import ROOT as root
from ROOT import gROOT
import sys, os, root_numpy, threading, random, emcee, corner, signal
from rootpy import stl
from rootpy.io import File
from rootpy.tree import Tree, TreeModel, TreeChain
from rootpy.plotting import Hist, Hist2D, Canvas, Graph, func
import neriX_simulation_datasets, neriX_simulation_config, neriX_analysis, neriX_datasets
import neriX_simulation_datasets
import numpy as np
from math import exp, factorial, erf, ceil, log, pow, floor
from scipy import optimize, misc, stats
from scipy.stats import norm
import copy_reg, types, pickle, click, time
from subprocess import call

import cuda_full_observables_production
from pycuda.compiler import SourceModule
import pycuda.driver as drv
import pycuda.tools
import pycuda.gpuarray
#import pycuda.autoinit
#mod = SourceModule(cuda_full_observables_production.cuda_full_observables_production_code, no_extern_c=True)

import matplotlib.pyplot as plt
import matplotlib.mlab as mlab
import rootpy.compiled as C
from numpy.random import normal, binomial, seed, poisson
from rootpy.plotting import root2matplotlib as rplt

#import gc
#gc.disable()

import warnings
#warnings.filterwarnings('error')
# this turns runtime warnings into errors to
# catch unexpected behavior
warnings.simplefilter('error', RuntimeWarning)
# this turns off a deprecation warning in general but aimed
# at corners plot creation (should try to only put there)
# and move back to all warnings as errors)
warnings.simplefilter('ignore', DeprecationWarning)
warnings.simplefilter('ignore', FutureWarning)

# MUST INCLUDE TYPES TO AVOID SEG FAULT
stl.vector(stl.vector('float'))
stl.vector(stl.vector('double'))
stl.vector(stl.vector('int'))


# create dot product in c++ for speed
# and thread safety

C.register_file('../../../python_modules/mc_code/c_safe_dot.C', ['safe_dot'])
C.register_file('../../../python_modules/mc_code/c_full_observables_production.C', ['full_matching_loop'])

c_full_matching_loop = C.full_matching_loop



def reduce_method(m):
	return (getattr, (m.__self__, m.__func__.__name__))



def integrate_normalized_gaussian(leftEdge, rightEdge, mu, sigma):
	try:
		return 0.5*(erf( (rightEdge - mu) / (2.**0.5 * sigma))  -  erf( (leftEdge - mu) / (2.**0.5 * sigma)))
	except:
		return 0.


def safe_poisson(mean):
	try:
		return True, poisson(mean)
	except:
		return False, 0



def safe_binomial(numberOfTrials, probabiltyOfSuccess):
	try:
		return True, binomial(numberOfTrials, probabiltyOfSuccess)
	except:
		return False, 0



def safe_normal(mean, sigma):
	try:
		return True, normal(mean, sigma)
	except:
		return False, 0



def smart_log_likelihood(aData, aMC, numMCEvents, confidenceIntervalLimit=0.95):
	totalLogLikelihood = 0.
	for (data, mc) in zip(aData, aMC):
		if mc == 0 and data != 0:
			# use 95% confidence interval
			# confidenceIntervalLimit = 1 - probability of the zero occuring
			probabiltyOfSuccess = 1. - (1.-confidenceIntervalLimit)**(1./numMCEvents)
			totalLogLikelihood += smart_log(smart_binomial(data, numMCEvents, probabiltyOfSuccess))
		else:
			totalLogLikelihood += data*smart_log(mc) - mc - smart_stirling(data)

	return totalLogLikelihood




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



def convert_2D_hist_to_matrix(h2D):
	numBinsX = h2D.nbins(0)
	numBinsY = h2D.nbins(1)
	
	a2D = np.zeros((numBinsX, numBinsY))

	for i in xrange(numBinsX):
		for j in xrange(numBinsY):
			a2D[i,j] = h2D[i+1,j+1].value

	return a2D





def create_graph_from_arrays(aX, aY):
	assert aX.size == aY.size
	graph = Graph(aX.size, type='asymm')
	chisqr = root.TMath.ChisquareQuantile
	npoints = 0
	
	ex = (aX[1] - aX[0])/2.
	for index, value in enumerate(aX):
		entries = aY[index]
		if entries < 0:
			continue
		elif entries == 0:
			ey_low = 0
			ey_high = 1.13943 # -ln(1-0.68)
		else:
			ey_low = entries - 0.5 * chisqr(0.1586555, 2. * entries)
			ey_high = 0.5 * chisqr(
				1. - 0.1586555, 2. * (entries + 1)) - entries
		graph.SetPoint(npoints, aX[index], entries)
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


def smart_binomial(numberOfSucceses, numberOfTrials, probabiltyOfSuccess):
	"""
	if not hasattr(numberOfSucceses, '__iter__'):
		numberOfSucceses = [numberOfSucceses]
	if not hasattr(numberOfTrials, '__iter__'):
		numberOfTrials = [numberOfTrials]
	"""
	nChooseK = smart_stirling(numberOfTrials) - smart_stirling(numberOfSucceses) - smart_stirling(numberOfTrials - numberOfSucceses)
	probabilityTerm = numberOfSucceses*smart_log(probabiltyOfSuccess) + (numberOfTrials-numberOfSucceses)*smart_log(1.-probabiltyOfSuccess)
	if nChooseK + probabilityTerm < -10:
		return 0
	else:
		return np.exp(nChooseK + probabilityTerm)


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
			likelihood *= norm.pdf(value)

		return likelihood, easy_graph(self.fill_graph_from_arrays())



	def make_graph_from_input(self, *inputParameters):
		if not len(inputParameters) == self.numParameters:
			print '\n\nFAILURE: Number of parameters given does not match the number of parameters of the function!'
			print 'Given %d parameters but require %d.' % (len(inputParameters), self.numParameters)
			sys.exit()

		likelihood = 1.
		for i, value in enumerate(inputParameters):
			self.lCurrentParameters[i] = self.lOriginalParameters[i] + (value*self.lOriginalParErrors[i])
			likelihood *= norm.pdf(value)

		return likelihood, easy_graph(self.fill_graph_from_arrays())




class neriX_simulation_analysis(object):
	def __init__(self, run, anodeVoltage, cathodeVoltage, angle, numMCEvents=50000, use_fake_data=False, create_fake_data=False, accidentalBkgAdjustmentTerm=0.0, assumeRelativeAccidentalRate=False):
	
		# if not in create data mode kill program trying
		# to run with adjusted accidental
	
		copy_reg.pickle(types.MethodType, reduce_method)

		# ------------------------------------------------
		# Pull filenames and create save paths
		# ------------------------------------------------

		# TOF will be used for accidentals if useTOF set to True, otherwise
		# will use liqscipeak - s1speak[0]
		(data_root_file, mc_root_file, mc_accidental_root_file, tac_efficiency_root_file, peak_finder_efficiency_root_file, trig_efficiency_root_file, g1_root_file, spe_root_file, gas_gain_root_file, g2_root_file, useTOF) = neriX_simulation_datasets.run_files[(run, anodeVoltage, cathodeVoltage, angle)]
		self.anodeSetting, self.cathodeSetting, self.degreeSetting, self.numMCEvents = anodeVoltage, cathodeVoltage, angle, numMCEvents
		
		
		self.useFakeData = use_fake_data
		if self.useFakeData:
			assert assumeRelativeAccidentalRate != False, 'Must assume a relative accidental rate for fake data!'
			

		# ------------------------------------------------
		# Set paths to files and grab constants
		# ------------------------------------------------

		dNamesForFiles = {'data':data_root_file, 'tac_efficiency':tac_efficiency_root_file, 'peak_finder_efficiency':peak_finder_efficiency_root_file, 'trig_efficiency':trig_efficiency_root_file, 'MC':mc_root_file, 'MC_accidentals':mc_accidental_root_file}
		dFilesForAnalysis = {}
		
		# count function calls for tracking status
		self.photonYieldFuncCalls = 0
		
		# load constants for matching
		self.numRandomTrials = neriX_simulation_datasets.num_random_trials
		self.maxPhotonYield = neriX_simulation_datasets.max_photon_yield
		self.maxChargeYield = neriX_simulation_datasets.max_charge_yield
		self.meanEnergy = neriX_simulation_datasets.dAngleEnergy[angle]
		self.meanField = neriX_simulation_datasets.dVoltageField[cathodeVoltage]
		
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
			elif fileType == 'trig_efficiency':
				fileName = neriX_simulation_datasets.pathToEfficiencyFiles + fileName
			elif fileType == 'MC':
				fileName = fileName
			elif fileType == 'MC_accidentals':
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
			
		if not g2_root_file:
			self.g2Value = neriX_simulation_datasets.default_g2
			self.g2Uncertainty = neriX_simulation_datasets.default_g2_uncertainty
			print '\n\nNOTE: No file given for g2 - will use hardcoded value of %.3f +/- %.3f \n\n' % (self.g2Value, self.g2Uncertainty)
		else:
			sys.exit()
		
		if not gas_gain_root_file:
			self.gasGainValue = neriX_simulation_datasets.default_gas_gain
			self.gasGainWidth = neriX_simulation_datasets.default_gas_gain_width
			self.gasGainUncertainty = neriX_simulation_datasets.default_gas_gain_uncertainty
			self.gasGainWidthUncertainty = neriX_simulation_datasets.default_gas_gain_uncertainty
			print '\n\nNOTE: No file given for gas gain - will use hardcoded value of %.3f +/- %.3f \n\n' % (self.gasGainValue, self.gasGainUncertainty)
		else:
			sys.exit()


		
		
		
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
		
		self.createFakeDataMode = create_fake_data
		self.useFakeData = use_fake_data
		
		if not self.createFakeDataMode:
			# load S1
			if not use_fake_data:
				fileToLoad = dFilesForAnalysis['data']
			else:
				print '\n\nNOTE: You are using fake data produced by this framework.\n\n'
				fileToLoad = neriX_simulation_datasets.pathToFakeData + '%ddeg_%.3fkV_%.1fkV.root' % (self.degreeSetting, self.cathodeSetting, self.anodeSetting)
			
			self.fData = File(fileToLoad)
			self.hS1 = self.fData.hS1
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
			self.hS2 = self.fData.hS2
			self.hS2.Sumw2()
			gS2 = convert_hist_to_graph_with_poisson_errors(self.hS2)
			self.egS2 = easy_graph(gS2)
			
			self.s2NumBins = self.egS2.get_num_elements()
			self.s2LowerBound = self.egS2.get_lower_bound()
			self.s2UpperBound = self.egS2.get_upper_bound()
			
			
			# load 2D histogram
			self.hS1S2 = self.fData.hS1S2
			self.hS1S2.Sumw2()
			self.aS1S2 = convert_2D_hist_to_matrix(self.hS1S2)
			
			if not self.useFakeData:
				# load TOF histogram
				self.hTOF = self.fData.hTOF
				self.hTOF.Sumw2()
				
				# load liqsci s1 time difference histogram
				self.hLiqSciS1TimeDiff = self.fData.hLiqSciS1TimeDiff
				self.hLiqSciS1TimeDiff.Sumw2()
			
			
			
		else:
			print '\n\nCurrently in creating fake data mode so cannot run analysis!\n\n'
			sFakeData = neriX_simulation_datasets.pathToFakeData + '%ddeg_%.3fkV_%.1fkV.root' % (self.degreeSetting, self.cathodeSetting, self.anodeSetting)
			self.fFakeData = File(sFakeData, 'recreate')
		
			self.s1NumBins = 20
			self.s1LowerBound =	-1
			self.s1UpperBound = 39
		
			self.s2NumBins = 20
			self.s2LowerBound = 0
			self.s2UpperBound = 4000
		
		

		# ------------------------------------------------
		# Load MC files or create reduced file if not present
		# and load into easy_graph
		# ------------------------------------------------

		self.egMC, self.hMC = self.load_mc_data(dFilesForAnalysis['MC'], 'MC')
		self.egAccidentalsMC, self.hAccidentalsMC = self.load_mc_data(dFilesForAnalysis['MC_accidentals'], 'MC_accidentals')
		
		# fill energy array
		self.fill_energy_array(useTOF, viewEnergySpectrum=False, adjustmentTerm=accidentalBkgAdjustmentTerm, assumeRelativeAccidentalRate=assumeRelativeAccidentalRate)

		# ------------------------------------------------
		# Load fSigmoid from tac efficiency file
		# and load into easy_function
		# ------------------------------------------------
		
		# load tac efficiency file and store in easy function
		fTACEfficiency = File(dFilesForAnalysis['tac_efficiency'])
		tf1TacEfficiency = fTACEfficiency.tof_efficiency
		self.efTacEfficiency = easy_function(tf1TacEfficiency, self.s1NumBins, self.s1LowerBound, self.s1UpperBound)

		# ------------------------------------------------
		# Load fSigmoid from tac efficiency file
		# and load into easy_function
		# ------------------------------------------------
		
		# load tac efficiency file and store in easy function
		fTrigEfficiency = File(dFilesForAnalysis['trig_efficiency'])
		tf1TrigEfficiency = fTrigEfficiency.eff_func
		self.efTrigEfficiency = easy_function(tf1TrigEfficiency, self.s2NumBins, self.s2LowerBound, self.s2UpperBound)
			
		# ------------------------------------------------
		# Load peak finder efficiency from file
		# and load into easy_function
		# ------------------------------------------------

		fPFEfficiency = File(dFilesForAnalysis['peak_finder_efficiency'])
		tf1PFEfficiency = fPFEfficiency.fPeakFindEff
		self.efPFEfficiency = easy_function(tf1PFEfficiency, self.s1NumBins, self.s1LowerBound, self.s1UpperBound)
		
		# test cases
		#print self.efTrigEfficiency.get_eg_best_fit()[1].get_x_values()
		#print self.efTrigEfficiency.get_eg_best_fit()[1].get_y_values()
		#print self.efTacEfficiency.make_graph_from_input(-1, -1)[0]**0.5
		#print self.efTacEfficiency.make_graph_from_input(-1, -1)[1].get_y_values()
		
		



	def read_g1_from_file(self, g1_root_file):
		return (0, 0)
	
	
	
	def read_g2_from_file(self, g2_root_file):
		return (0, 0)
	
	
	
	def read_gas_gain_from_file(self, gas_gain_root_file):
		return (0, 0)
	
	
	
	def fill_energy_array(self, useTOF, viewEnergySpectrum=False, adjustmentTerm=0., assumeRelativeAccidentalRate=False):
		#self.numMCEvents
		#self.hMC
		#self.hAccidentalsMC
		
		if assumeRelativeAccidentalRate == False:
			# first need to look at the percent of events
			# due to accidental coincidences
			if useTOF:
				lAccidentalRanges = neriX_datasets.lTOFAccidentalsRanges
				try:
					lTimeBounds = neriX_datasets.dTOFBounds[(self.degreeSetting, self.cathodeSetting)]
				except:
					neriX_analysis.failure_message('Could not find bound for current cathode and degree settings!')
					sys.exit()
				hTimeSpec = self.hTOF
			
			else:
				lAccidentalRanges = neriX_datasets.lLiqSciS1DtAccidentalsRanges
				lTimeBounds = neriX_datasets.lLiqSciS1DtRange
				hTimeSpec = self.hLiqSciS1TimeDiff

			# count number of accidentals in set ranges
			numAccidentalsInRanges = 0
			accidentalsWindowSize = 0
			for lRange in lAccidentalRanges:
				lowerBoundTime = lRange[0]
				upperBoundTime = lRange[1]
				
				lowerBoundBin = hTimeSpec.FindBin(lowerBoundTime)
				upperBoundBin = hTimeSpec.FindBin(upperBoundTime)
				
				numAccidentalsInRanges += hTimeSpec.Integral(lowerBoundBin, upperBoundBin)
				accidentalsWindowSize += (upperBoundTime - lowerBoundTime)


			# repeat for actual time spec
			lowerBoundTime = lTimeBounds[0]
			upperBoundTime = lTimeBounds[1]
			
			lowerBoundBin = hTimeSpec.FindBin(lowerBoundTime)
			upperBoundBin = hTimeSpec.FindBin(upperBoundTime)
			
			numEventsInTimeWindow = hTimeSpec.Integral(lowerBoundBin, upperBoundBin)
			timeWindowSize = (upperBoundTime - lowerBoundTime)

			scaledNumAccidentals = float(numAccidentalsInRanges)*timeWindowSize/accidentalsWindowSize
			scaledNumAccidentals *= (1. + adjustmentTerm)
			
			percentOfAccidentalsInTimeWindow = float(scaledNumAccidentals) / (numEventsInTimeWindow)
			
			#print scaledNumAccidentals, numEventsInTimeWindow, percentOfAccidentalsInTimeWindow
		else:
			percentOfAccidentalsInTimeWindow = assumeRelativeAccidentalRate / (1. + assumeRelativeAccidentalRate)
		
		# have the percent of events that are accidentals so
		# now fill energy array
		
		random.seed()
		self.aEnergy = np.zeros(self.numMCEvents, dtype=np.float32)
		for i in xrange(self.numMCEvents):
			randNum = random.random()
			if randNum < percentOfAccidentalsInTimeWindow:
				self.aEnergy[i] = self.hAccidentalsMC.GetRandom()
			else:
				self.aEnergy[i] = self.hMC.GetRandom()
				
		if viewEnergySpectrum:
			cEnergySpec = Canvas()
			hEnergySpec = self.hMC.empty_clone()
			hEnergySpec.fill_array(self.aEnergy)
			hEnergySpec.Draw()
			cEnergySpec.Update()
			raw_input('Press enter to continue...')
			
			del hEnergySpec
			del cEnergySpec


		


	def load_mc_data(self, mc_root_file, sFileType):
		# check if reduced file exists
		pathToReducedSimulationFiles = neriX_simulation_datasets.pathToReducedSimulationFiles + mc_root_file
		if not os.path.exists(pathToReducedSimulationFiles):
			
		
			# load the raw data file
			pathToSimulatedFile = neriX_simulation_datasets.pathToSimulationFiles
			fSimulation = File(pathToSimulatedFile + mc_root_file)

			if sFileType == 'MC':

				# set cuts
				xRadius = '(sqrt(xpos[0]**2+ypos[0]**2) < 18)'
				xZ = '(zpos[0]>-20 && zpos[0]<-4)'
				xSingleScatter = '(nsteps_target==1)'
				xLiqSciHeight = '(etotliqsci>700)'
				xLXeEnergy = '(etot_target>0)'

				#print 'No TOF cut!\n'
				neriX_analysis.warning_message('Need to finalize how tof is chosen in MC data!')
				xTOF = '(timeliqsci-tpos[0]>5 && timeliqsci-tpos[0]<40)'

				xAll = '%s && %s && %s && %s && %s && %s' % (xRadius, xZ, xSingleScatter, xLiqSciHeight, xLXeEnergy, xTOF)
			
			elif sFileType == 'MC_accidentals':
				# set cuts
				xRadius = '(sqrt(xpos[0]**2+ypos[0]**2) < 18)'
				xZ = '(zpos[0]>-20 && zpos[0]<-4)'
				xSingleScatter = '(nsteps_target==1)'
				xLXeEnergy = '(etot_target>0)'

				xAll = '%s && %s && %s && %s' % (xRadius, xZ, xSingleScatter, xLXeEnergy)

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
		
			neriX_analysis.success_message('Created new reduced MC file!')
		
		

		self.fReduced = File(pathToReducedSimulationFiles, 'read')
		hMC = self.fReduced.hMC
		gMC = convert_hist_to_graph_with_poisson_errors(hMC)
		return easy_graph(gMC), hMC




	# get likelihood and g1 given random variable (nuissance parameter)
	# g1RV should be normally distributed
	def get_g1_default(self, g1RV):
		return norm.pdf(g1RV), self.g1Value + (g1RV*self.g1Uncertainty)
	
	
	
	# get likelihood and g2 given random variable (nuissance parameter)
	# g2RV should be normally distributed
	def get_g2_default(self, g2RV):
		return norm.pdf(g2RV), self.g2Value + (g2RV*self.g2Uncertainty)
	
	
	
	# get likelihood and gas gain given random variable (nuissance parameter)
	# gasGainRV should be normally distributed
	def get_gas_gain_default(self, gasGainRV):
		return norm.pdf(gasGainRV), self.gasGainValue + (gasGainRV*self.gasGainUncertainty)
	
	
	
	def get_gas_gain_width_default(self, gasGainRV):
		return norm.pdf(gasGainRV), self.gasGainWidth + (gasGainRV*self.gasGainWidthUncertainty)
	
	
	
	def get_spe_res_default(self, speResRV):
		return norm.pdf(speResRV), self.speResValue + (speResRV*self.speResUncertainty)



	def get_exciton_ion_ratio(self, lParsRV):
		likelihood = 1.
		lParValues = [0. for i in xrange(len(lParsRV))]
		for parNumber, parRV in enumerate(lParsRV):
			likelihood *= norm.pdf(parRV)
			if parNumber == 0:
				# alpha
				meanFromNEST = 1.240
				sigmaLowFromNEST = 0.073
				sigmaHighFromNEST = 0.079
			elif parNumber == 1:
				# zeta
				meanFromNEST = 0.0472
				sigmaLowFromNEST = 0.0073
				sigmaHighFromNEST = 0.0088
			elif parNumber == 2:
				# beta
				meanFromNEST = 239.0
				sigmaLowFromNEST = 8.8
				sigmaHighFromNEST = 28.0
			else:
				print 'FAILURE: Incorrect number of parameters given for exciton to ion ratio.'
				sys.exit()

			if parRV >= 0:
				valueForPar = meanFromNEST + parRV*sigmaHighFromNEST
			else:
				valueForPar = meanFromNEST - parRV*sigmaLowFromNEST

			lParValues[parNumber] = valueForPar
		
		return likelihood, lParValues[0]*(self.meanField**(-lParValues[1])) * ( 1 - exp(-lParValues[2] * 11.5*self.meanEnergy*54**(-7./3.)) )
		
		
		
	def get_photon_quenching(self, lParsRV):
		likelihood = 1.
		lParValues = [0. for i in xrange(len(lParsRV))]
		for parNumber, parRV in enumerate(lParsRV):
			likelihood *= norm.pdf.pdf(parRV)
			if parNumber == 0:
				# eta
				meanFromNEST = 3.3
				sigmaLowFromNEST = -0.7
				sigmaHighFromNEST = 5.3
			elif parNumber == 1:
				# lambda
				meanFromNEST = 1.14
				sigmaLowFromNEST = -0.09
				sigmaHighFromNEST = 0.45
			else:
				print 'FAILURE: Incorrect number of parameters given for exciton to ion ratio.'
				sys.exit()

			if parRV >= 0:
				valueForPar = meanFromNEST + parRV*sigmaHighFromNEST
			else:
				valueForPar = meanFromNEST - parRV*sigmaLowFromNEST

			lParValues[parNumber] = valueForPar
			
		photonQuenching = 1./(1. + lParValues[0]*( 11.5*self.meanEnergy*54.**(-7./3.) )**lParValues[1])
		if photonQuenching < 0 or photonQuenching > 1:
			return 0., photonQuenching
		else:
			return likelihood, photonQuenching
	
	
	
	# set prior functions for photon yield and resolution
	# equivalent to setting bounds for fit
	def get_prior_log_likelihood_photon_yield(self, photonYield):
		if photonYield < 0 or photonYield > self.maxPhotonYield:
			return -np.inf
		else:
			return 0



	def get_prior_log_likelihood_charge_yield(self, chargeYield):
		if chargeYield < 0 or chargeYield > self.maxChargeYield:
			return -np.inf
		else:
			return 0



	def get_prior_log_likelihood_resolution(self, intrinsicResolution):
		if intrinsicResolution < 0 or intrinsicResolution > 4:
			return -np.inf
		else:
			return 0



	def get_prior_log_likelihood_lindhard_factor(self, lindhardFactor):
		if lindhardFactor < 0 or lindhardFactor > 1:
			return -np.inf
		else:
			return 0



	def get_prior_log_likelihood_recombination(self, recombinationProb):
		if recombinationProb < 0 or recombinationProb > 1:
			return -np.inf
		else:
			return 0



	def get_prior_log_likelihood_fano_factor(self, fanoFactor):
		if fanoFactor < 0 or fanoFactor > 0.2:
			return -np.inf
		else:
			return 0



	def get_prior_log_likelihood_photon_quenching(self, photonQuenching):
		if photonQuenching < 0 or photonQuenching > 1:
			return -np.inf
		else:
			return 0




	def get_prior_log_likelihood_nuissance(self, likelihoodNuissance):
		if likelihoodNuissance > 1e-8:
			return np.log(likelihoodNuissance)
		else:
			return -np.inf



	def get_indices_for_given_quantile(self, aBinCenters, aCountsInBins, leftPercentile, rightPercentile):
		aX = aBinCenters.flatten()
		aY = aCountsInBins.flatten()
		
		totalCounts = int(np.rint(np.sum(aCountsInBins)))
		aForQuantiles = np.zeros(totalCounts)
		
		# build array for quantile analysis
		# essentially add another index for each count in array
		currentIndex = 0
		
		for i in xrange(len(aX)):
			for j in xrange(int(np.rint(aY[i]))):
				aForQuantiles[currentIndex] = aX[i]
				currentIndex += 1

		valueLeft = np.percentile(aForQuantiles, leftPercentile*100)
		valueRight = np.percentile(aForQuantiles, rightPercentile*100)
		
		indexLeft = np.argmin(np.abs(aBinCenters - valueLeft))
		indexRight = np.argmin(np.abs(aBinCenters - valueRight))
		
		# add 1 to right index to keep it inclusive
		return (indexLeft, indexRight + 1)
	
	
	
	
	# ************************************************
	# ------------------------------------------------
	# ------------------------------------------------
	# ------------------------------------------------
	# ------------------------------------------------
	# ------------------------------------------------
	# Create fake data
	# ------------------------------------------------
	# ------------------------------------------------
	# ------------------------------------------------
	# ------------------------------------------------
	# ------------------------------------------------
	# ************************************************
	
	
	def create_fake_data(self, photonYield, chargeYield, intrinsicResolutionS1, intrinsicResolutionS2, g1RV, speResRV, par0TacEffRV, par0PFEffRV, par1PFEffRV, g2RV, gasGainRV, gasGainWidthRV, par0TrigEffRV, par1TrigEffRV, par0ExcitonToIonRV, par1ExcitonToIonRV, par2ExcitonToIonRV):


		# ------------------------------------------------
		# Load required constants, graphs, and likelihoods
		# for later use in matching code
		# ------------------------------------------------

		
		excitonToIonLikelihood, excitonToIonRatio = self.get_exciton_ion_ratio([par0ExcitonToIonRV, par1ExcitonToIonRV, par2ExcitonToIonRV])
		
		# S1 values (6 nuissnce parameters)
		g1Likelihood, g1Value = self.get_g1_default(g1RV)
		
		speResLikelihood, speRes = self.get_spe_res_default(speResRV)

		tacEffLikelihood, egTacEff = self.efTacEfficiency.make_graph_from_input(par0TacEffRV)
		
		pfEffLikelihood, egPFEff = self.efPFEfficiency.make_graph_from_input(par0PFEffRV, par1PFEffRV)


		# S2 values (5 nuissance parameters)
		g2Likelihood, g2Value = self.get_g2_default(g2RV)
		
		gasGainLikelihood, gasGainValue = self.get_gas_gain_default(gasGainRV)
		
		gasGainWidthLikelihood, gasGainWidth = self.get_gas_gain_default(gasGainWidthRV)
		
		trigEffLikelihood, egTrigEff = self.efTrigEfficiency.make_graph_from_input(par0TrigEffRV, par1TrigEffRV)
	
	
	
	
		# ------------------------------------------------
		# Initialize required constants, matrices, and arrays
		# ------------------------------------------------

		#aS1BinEdges = np.asarray([aS1BinCenters[0] - 0.5*binWidthS1 + i*binWidthS1 for i in xrange(numBinsS1+1)]) # need +1 to capture last bin!!
		#aS2BinEdges = np.asarray([aS2BinCenters[0] - 0.5*binWidthS2 + i*binWidthS2 for i in xrange(numBinsS2+1)])
		
		aS1BinEdges = np.linspace(self.s1LowerBound, self.s1UpperBound, num=self.s1NumBins+1)
		aS2BinEdges = np.linspace(self.s2LowerBound, self.s2UpperBound, num=self.s2NumBins+1)
		
		
		extractionEfficiency = g2Value / gasGainValue
	
	
	
		# ------------------------------------------------
		# Set seeds and number of trials
		# ------------------------------------------------
		
		numRandomTrials = self.numMCEvents
	
		aS1 = np.full(numRandomTrials, -1, dtype=np.float32)
		aS2 = np.full(numRandomTrials, -1, dtype=np.float32)
		
		seed = np.asarray(int(time.time()*1000), dtype=np.int32)
		num_trials = np.asarray(numRandomTrials, dtype=np.int32)
		photonYield = np.asarray(photonYield, dtype=np.float32)
		chargeYield = np.asarray(chargeYield, dtype=np.float32)
		excitonToIonRatio = np.asarray(excitonToIonRatio, dtype=np.float32)
		g1Value = np.asarray(g1Value, dtype=np.float32)
		extractionEfficiency = np.asarray(extractionEfficiency, dtype=np.float32)
		gasGainValue = np.asarray(gasGainValue, dtype=np.float32)
		gasGainWidth = np.asarray(gasGainWidth, dtype=np.float32)
		speRes = np.asarray(speRes, dtype=np.float32)
		intrinsicResS1 = np.asarray(intrinsicResolutionS1, dtype=np.float32)
		intrinsicResS2 = np.asarray(intrinsicResolutionS2, dtype=np.float32)
		
		c_full_matching_loop(seed, num_trials, aS1, aS2, self.aEnergy, photonYield, chargeYield, excitonToIonRatio, g1Value, extractionEfficiency, gasGainValue, gasGainWidth, speRes, intrinsicResS1, intrinsicResS2)
		
		
		# ------------------------------------------------
		# create 2D histogram of S1s and S2s
		# ------------------------------------------------
		
		
		aS1S2MC, xEdges, yEdges = np.histogram2d(aS1, aS2, bins=[aS1BinEdges, aS2BinEdges])
		
		

		# ------------------------------------------------
		# apply efficiencies
		# ------------------------------------------------

		# element wise multiplication to combine S1 or S2
		# need to use outer with multiply to make large matrix
		
		aS1Efficiency = egTacEff.get_y_values().flatten()*egPFEff.get_y_values().flatten()
		aS2Efficiency = egTrigEff.get_y_values()
		
		aFullEfficiencyMatrix = np.outer(aS1Efficiency, aS2Efficiency)
		
		#print aS1S2.shape, aFullEfficiencyMatrix.shape
		assert aS1S2MC.shape == aFullEfficiencyMatrix.shape
		
		aS1S2MC = binomial(aS1S2MC.astype('int64', copy=False), aFullEfficiencyMatrix)
		#aS1S2MC = np.multiply(aS1S2MC, aFullEfficiencyMatrix)
		

		self.fFakeData.cd()
		
		self.hS1 = Hist(self.s1NumBins, self.s1LowerBound, self.s1UpperBound, name='hS1')
		root_numpy.array2hist(aS1S2MC.sum(axis=1), self.hS1)
		self.hS1.Write()
		
		self.hS2 = Hist(self.s2NumBins, self.s2LowerBound, self.s2UpperBound, name='hS2')
		root_numpy.array2hist(aS1S2MC.sum(axis=0), self.hS2)
		self.hS2.Write()
		
		self.hS1S2 = Hist2D(self.s1NumBins, self.s1LowerBound, self.s1UpperBound, self.s2NumBins, self.s2LowerBound, self.s2UpperBound, name='hS1S2')
		root_numpy.array2hist(aS1S2MC, self.hS1S2)
		self.hS1S2.Write()
		
		#print aS1S2MC
		#print root_numpy.hist2array(self.hS1S2, include_overflow=False)

	
	
	
	
	
	
	# ************************************************
	# ------------------------------------------------
	# ------------------------------------------------
	# ------------------------------------------------
	# ------------------------------------------------
	# ------------------------------------------------
	# Full matching (S1 and S2)
	# ------------------------------------------------
	# ------------------------------------------------
	# ------------------------------------------------
	# ------------------------------------------------
	# ------------------------------------------------
	# ************************************************
		
		
		
	# function will return both log likelihood and graph with
	# matched spectrum
	# will simply call this for likelihood maximization
	# and take just the likelihood - can call after that with
	# best fit to get spectrum
	def perform_mc_match_full(self, photonYield, chargeYield, intrinsicResolutionS1, intrinsicResolutionS2, g1RV, speResRV, par0TacEffRV, par0PFEffRV, par1PFEffRV, g2RV, gasGainRV, gasGainWidthRV, par0TrigEffRV, par1TrigEffRV, par0ExcitonToIonRV, par1ExcitonToIonRV, par2ExcitonToIonRV, drawFit=False, lowerQuantile=0.0, upperQuantile=1.0, drawTracker=False, gpu_compute=False, d_gpu_scale={'block':(1024,1,1), 'grid':(64,1)}):
	
		#fullStartTime = time.time()
	
		#neriX_analysis.warning_message('Forcing grid')
		#d_gpu_scale={'block':(256,256,1), 'grid':(1,1)}
		#print d_gpu_scale
		
		num_mc_elements = self.numMCEvents
	
		# set default number of mc elements if using cpu and
		# not specified and set number if using gpu
		if gpu_compute:
			# need to do slightly fewer elements than
			# originally perscribed bc energies already
			# produced
			d_gpu_scale = {}
			d_gpu_scale['block'] = (1024,1,1)
			numBlocks = floor(num_mc_elements / 1024.)
			d_gpu_scale['grid'] = (int(numBlocks), 1)
			num_mc_elements = int(numBlocks*1024)
	
		#print num_mc_elements
		#initializationTime = time.time()


		# ------------------------------------------------
		# Load required constants, graphs, and likelihoods
		# for later use in matching code
		# ------------------------------------------------

		# free parameters (4 free parameters, 5 nuissance)
		photonYieldLogLikelihood = self.get_prior_log_likelihood_photon_yield(photonYield)
		chargeYieldLogLikelihood = self.get_prior_log_likelihood_charge_yield(chargeYield)
		resS1LogLikelihood = self.get_prior_log_likelihood_resolution(intrinsicResolutionS1)
		resS2LogLikelihood = self.get_prior_log_likelihood_resolution(intrinsicResolutionS2)
		
		excitonToIonLikelihood, excitonToIonRatio = self.get_exciton_ion_ratio([par0ExcitonToIonRV, par1ExcitonToIonRV, par2ExcitonToIonRV])
		excitonToIonLogLikelihood = self.get_prior_log_likelihood_nuissance(excitonToIonLikelihood)

		# S1 values (6 nuissnce parameters)
		g1Likelihood, g1Value = self.get_g1_default(g1RV)
		g1LogLikelihood = self.get_prior_log_likelihood_nuissance(g1Likelihood)
		
		speResLikelihood, speRes = self.get_spe_res_default(speResRV)
		speResLogLikelihood = self.get_prior_log_likelihood_nuissance(speResLikelihood)

		tacEffLikelihood, egTacEff = self.efTacEfficiency.make_graph_from_input(par0TacEffRV)
		tacEffLogLikelihood = self.get_prior_log_likelihood_nuissance(tacEffLikelihood)
		
		pfEffLikelihood, egPFEff = self.efPFEfficiency.make_graph_from_input(par0PFEffRV, par1PFEffRV)
		pfEffLogLikelihood = self.get_prior_log_likelihood_nuissance(pfEffLikelihood)


		# S2 values (5 nuissance parameters)
		g2Likelihood, g2Value = self.get_g2_default(g2RV)
		g2LogLikelihood = self.get_prior_log_likelihood_nuissance(g2Likelihood)
		
		gasGainLikelihood, gasGainValue = self.get_gas_gain_default(gasGainRV)
		gasGainLogLikelihood = self.get_prior_log_likelihood_nuissance(gasGainLikelihood)
		
		gasGainWidthLikelihood, gasGainWidth = self.get_gas_gain_default(gasGainWidthRV)
		gasGainWidthLogLikelihood = self.get_prior_log_likelihood_nuissance(gasGainWidthLikelihood)
		
		trigEffLikelihood, egTrigEff = self.efTrigEfficiency.make_graph_from_input(par0TrigEffRV, par1TrigEffRV)
		trigEffLogLikelihood = self.get_prior_log_likelihood_nuissance(trigEffLikelihood)
		

		# sum all likelihood terms (should be 13 terms)
		priorLogLikelihoods = photonYieldLogLikelihood + chargeYieldLogLikelihood + excitonToIonLogLikelihood + resS1LogLikelihood + resS2LogLikelihood + g1LogLikelihood + speResLogLikelihood + tacEffLogLikelihood + pfEffLogLikelihood + g2LogLikelihood + gasGainLogLikelihood + gasGainWidthLogLikelihood + trigEffLogLikelihood
	
		#print 'Priors: %f' % priorLogLikelihoods
	
	
		# ------------------------------------------------
		# Initialize required constants, matrices, and arrays
		# ------------------------------------------------


		aS1BinEdges = np.linspace(self.s1LowerBound, self.s1UpperBound, num=self.s1NumBins+1)
		aS2BinEdges = np.linspace(self.s2LowerBound, self.s2UpperBound, num=self.s2NumBins+1)
		#print aS1BinEdges

		extractionEfficiency = g2Value / gasGainValue
	
		#print 'Time for loading: %f' % (time.time() - initializationTime)
	
		# ------------------------------------------------
		# Set seeds and number of trials
		# ------------------------------------------------



		#startTime = time.time()
		
		
		# initialize pointers for both methods
		
		aS1 = np.full(num_mc_elements, -1, dtype=np.float32)
		aS2 = np.full(num_mc_elements, -1, dtype=np.float32)
		
		seed = np.asarray(int(time.time()*1000), dtype=np.int32)
		num_trials = np.asarray(num_mc_elements, dtype=np.int32)
		photonYield = np.asarray(photonYield, dtype=np.float32)
		chargeYield = np.asarray(chargeYield, dtype=np.float32)
		excitonToIonRatio = np.asarray(excitonToIonRatio, dtype=np.float32)
		g1Value = np.asarray(g1Value, dtype=np.float32)
		extractionEfficiency = np.asarray(extractionEfficiency, dtype=np.float32)
		gasGainValue = np.asarray(gasGainValue, dtype=np.float32)
		gasGainWidth = np.asarray(gasGainWidth, dtype=np.float32)
		speRes = np.asarray(speRes, dtype=np.float32)
		intrinsicResS1 = np.asarray(intrinsicResolutionS1, dtype=np.float32)
		intrinsicResS2 = np.asarray(intrinsicResolutionS2, dtype=np.float32)
		
		


		if gpu_compute:
		
			try:
				self.mem_pool
			except:
				import pycuda.autoinit
				self.mod = SourceModule(cuda_full_observables_production.cuda_full_observables_production_code, no_extern_c=True)
			
				self.mem_pool = pycuda.tools.DeviceMemoryPool()
				self.mem_pool = self.mem_pool.allocate(aS1.nbytes*4)
		
			try:
				self.gpu_aEnergy
			except:
				self.gpu_aEnergy = pycuda.gpuarray.to_gpu(self.aEnergy)
		
		
			observables_func = self.mod.get_function('gpu_full_observables_production')
			
			tArgs = (drv.In(seed), drv.In(num_trials), drv.Out(aS1), drv.Out(aS2), self.gpu_aEnergy, drv.In(photonYield), drv.In(chargeYield), drv.In(excitonToIonRatio), drv.In(g1Value), drv.In(extractionEfficiency), drv.In(gasGainValue), drv.In(gasGainWidth), drv.In(speRes), drv.In(intrinsicResS1), drv.In(intrinsicResS2))
			
			observables_func(*tArgs, **d_gpu_scale)
		
		else:
			observables_func = c_full_matching_loop(seed, num_trials, aS1, aS2, self.aEnergy, photonYield, chargeYield, excitonToIonRatio, g1Value, extractionEfficiency, gasGainValue, gasGainWidth, speRes, intrinsicResS1, intrinsicResS2)
		
		#print 'MC full loop time for %d elements: %f' % (num_mc_elements, time.time() - startTime)


		# ------------------------------------------------
		# create 2D histogram of S1s and S2s
		# ------------------------------------------------

		#analysisTime = time.time()
		
		aS1S2MC, xEdges, yEdges = np.histogram2d(aS1, aS2, bins=[aS1BinEdges, aS2BinEdges])
		
		#print aS1S2MC

		# ------------------------------------------------
		# apply efficiencies
		# ------------------------------------------------

		# element wise multiplication to combine S1 or S2
		# need to use outer with multiply to make large matrix
		
		aS1Efficiency = egTacEff.get_y_values().flatten()*egPFEff.get_y_values().flatten()
		aS2Efficiency = egTrigEff.get_y_values()
		
		aFullEfficiencyMatrix = np.outer(aS1Efficiency, aS2Efficiency)
		
		#print aS1S2.shape, aFullEfficiencyMatrix.shape
		assert aS1S2MC.shape == aFullEfficiencyMatrix.shape
		
		try:
			#aS1S2MC = np.multiply(aS1S2MC, aFullEfficiencyMatrix)
			aS1S2MC = binomial(aS1S2MC.astype('int64', copy=False), aFullEfficiencyMatrix)
			aS1S2MC = np.multiply(aS1S2MC, np.sum(self.aS1S2) / np.sum(aS1S2MC))
		except:
			return -np.inf, aS1S2MC

		if drawFit:

			f, (ax1, ax2) = plt.subplots(2, sharex=True, sharey=True)
			
			s1s2DataPlot = np.rot90(self.aS1S2)
			s1s2DataPlot = np.flipud(s1s2DataPlot)
			ax1.pcolormesh(aS1BinEdges, aS2BinEdges, s1s2DataPlot)
			
			s1s2MCPlot = np.rot90(aS1S2MC)
			s1s2MCPlot = np.flipud(s1s2MCPlot)
			ax2.pcolormesh(aS1BinEdges, aS2BinEdges, s1s2MCPlot)
			#plt.colorbar()
			
						
			c1 = Canvas(1400, 400)
			c1.Divide(2)
			
			hS1MC = Hist(self.s1NumBins, self.s1LowerBound, self.s1UpperBound, name='hS1_draw_mc', drawstyle='hist')
			#hS1MC.fill_array(aS1) # gives bad 1D histogram
			root_numpy.array2hist(aS1S2MC.sum(axis=1), hS1MC)
			hS1MC.Scale(self.hS1.Integral() / hS1MC.Integral())
			
			self.hS1.SetLineColor(root.kRed)
			self.hS1.SetMarkerSize(0)
			
			c1.cd(1)
			self.hS1.Draw()
			hS1MC.Draw('same')
			
			hS2MC = Hist(self.s2NumBins, self.s2LowerBound, self.s2UpperBound, name='hS2_draw_mc', drawstyle='hist')
			#hS2MC.fill_array(aS2)
			root_numpy.array2hist(aS1S2MC.sum(axis=0), hS2MC)
			hS2MC.Scale(self.hS2.Integral() / hS2MC.Integral())
			
			self.hS2.SetLineColor(root.kRed)
			self.hS2.SetMarkerSize(0)
			
			c1.cd(2)
			self.hS2.Draw()
			hS2MC.Draw('same')
			
			print 'Number of events in data: %f' % self.hS1.Integral()
			
			
			plt.show()
			#raw_input('Press enter to continue...')
		
			del hS1MC, hS2MC, c1
		
		flatS1S2Data = self.aS1S2.flatten()
		flatS1S2MC = aS1S2MC.flatten()


		logLikelihoodMatching = smart_log_likelihood(flatS1S2Data, flatS1S2MC, num_mc_elements)
		totalLogLikelihood = logLikelihoodMatching + priorLogLikelihoods
		
		#print 'Full time in function: %f' % (time.time() - fullStartTime)
		
		if np.isnan(totalLogLikelihood):
			return -np.inf, aS1S2MC
		else:
			return totalLogLikelihood, aS1S2MC
		
		
		
		
	def mcmc_func_full_matching(self, aParametersToFit, drawFit=False, gpu_compute=False, is_worker=False, d_gpu_scale={}):
		#print aParametersToFit
		assert len(aParametersToFit) == 17
		#startTime = time.time()
		if d_gpu_scale == {}:
			logLikelihood = self.perform_mc_match_full(*aParametersToFit, gpu_compute=gpu_compute, drawFit=drawFit)[0]
		else:
			logLikelihood = self.perform_mc_match_full(*aParametersToFit, gpu_compute=gpu_compute, drawFit=drawFit, d_gpu_scale=d_gpu_scale)[0]
		
		return logLikelihood
	
	
	
	
	


	# ************************************************
	# ------------------------------------------------
	# ------------------------------------------------
	# ------------------------------------------------
	# ------------------------------------------------
	# ------------------------------------------------
	# S2 Matching (Charge Yield)
	# ------------------------------------------------
	# ------------------------------------------------
	# ------------------------------------------------
	# ------------------------------------------------
	# ------------------------------------------------
	# ************************************************
	
	
	# function will return both log likelihood and graph with
	# matched spectrum
	# will simply call this for likelihood maximization
	# and take just the likelihood - can call after that with
	# best fit to get spectrum
	def perform_mc_match_charge_yield(self, chargeYield, intrinsicResolution, g2RV, gasGainRV, gasGainWidthRV, speResRV, par0TrigEffRV, par1TrigEffRV, drawFit=False, drawTracker=False, lowerQuantile=0.0, upperQuantile=1.0):


		# ------------------------------------------------
		# Load required constants, graphs, and likelihoods
		# for later use in matching code
		# ------------------------------------------------

		g2Likelihood, g2Value = self.get_g2_default(g2RV)
		g2LogLikelihood = self.get_prior_log_likelihood_nuissance(g2Likelihood)
		
		gasGainLikelihood, gasGainValue = self.get_gas_gain_default(gasGainRV)
		gasGainLogLikelihood = self.get_prior_log_likelihood_nuissance(gasGainLikelihood)
		
		gasGainWidthLikelihood, gasGainWidth = self.get_gas_gain_default(gasGainWidthRV)
		gasGainWidthLogLikelihood = self.get_prior_log_likelihood_nuissance(gasGainWidthLikelihood)
		
		speResLikelihood, speRes = self.get_spe_res_default(speResRV)
		speResLogLikelihood = self.get_prior_log_likelihood_nuissance(speResLikelihood)
		
		trigEffLikelihood, egTrigEff = self.efTrigEfficiency.make_graph_from_input(par0TrigEffRV, par1TrigEffRV)
		trigEffLogLikelihood = self.get_prior_log_likelihood_nuissance(trigEffLikelihood)

		qYLogLikelihood = self.get_prior_log_likelihood_charge_yield(chargeYield)
		resLogLikelihood = self.get_prior_log_likelihood_resolution(intrinsicResolution)

		priorLogLikelihoods = g2LogLikelihood + gasGainLogLikelihood + gasGainWidthLogLikelihood + speResLogLikelihood + trigEffLogLikelihood + qYLogLikelihood + resLogLikelihood

		# test cases
		#print priorLikelihoods
		#print egTacEff.get_y_values()
		#print egPFEff.get_y_values()
		
		
		return 0, 0



	def mcmc_func_charge_yield_mc_matching(self, aParametersToFit, drawFit=False):
		#print aParametersToFit
		assert len(aParametersToFit) == 8
		#startTime = time.time()
		logLikelihood = self.perform_mc_match_charge_yield(*aParametersToFit, drawFit=drawFit)[0]
		#print time.time() - startTime
		return logLikelihood
	
	
	
	
		
	
	# ************************************************
	# ------------------------------------------------
	# ------------------------------------------------
	# ------------------------------------------------
	# ------------------------------------------------
	# ------------------------------------------------
	# S1 Matching (Photon Yield)
	# ------------------------------------------------
	# ------------------------------------------------
	# ------------------------------------------------
	# ------------------------------------------------
	# ------------------------------------------------
	# ************************************************


	# function will return both log likelihood and graph with
	# matched spectrum
	# will simply call this for likelihood maximization
	# and take just the likelihood - can call after that with
	# best fit to get spectrum
	def perform_mc_match_photon_yield(self, photonYield, intrinsicResolution, g1RV, speResRV, par0TacEffRV, par1TacEffRV, par0PFEffRV, par1PFEffRV, drawFit=False, drawTracker=False, lowerQuantile=0.0, upperQuantile=1.0):


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

		return 0, 0



	def mcmc_func_photon_yield_mc_matching(self, aParametersToFit, drawFit=False):
		#print aParametersToFit
		assert len(aParametersToFit) == 8
		startTime = time.time()
		logLikelihood = self.perform_mc_match_photon_yield(*aParametersToFit, drawFit=drawFit)[0]
		print time.time() - startTime
		return logLikelihood



	def run_mcmc(self, sMeasurement, sParameters, numWalkers, numSteps, numThreads, fractionalDeviationStartPos = 1e-3, gpu_compute=False, d_gpu_scale={}):
		if sMeasurement == 'photon_yield':
			numDim = 8
			assert len(sParameters) == numDim
			func = self.mcmc_func_photon_yield_mc_matching
		elif sMeasurement == 'charge_yield':
			numDim = 8
			assert len(sParameters) == numDim
			func = self.mcmc_func_charge_yield_mc_matching
		elif sMeasurement == 'full_matching':
			numDim = 17
			assert len(sParameters) == numDim
			func = self.mcmc_func_full_matching
		else:
			print 'Currently not setup to handle the input measurement: %s' % sMeasurement
			sys.exit()
	
		# ------------------------------------------------
		# Setup save locations
		# ------------------------------------------------
		
		if not self.useFakeData:
			self.resultsDirectoryName = neriX_simulation_config.nameOfResultsDirectory
		else:
			self.resultsDirectoryName = neriX_simulation_datasets.pathToFakeData + 'results'
		
		self.sPathForSave = '%s/%ddeg_%.3fkV_%.1fkV/%s/' % (self.resultsDirectoryName, self.degreeSetting, self.cathodeSetting, self.anodeSetting, sMeasurement)
		self.sPathForOldFiles = self.sPathForSave + 'previous_results/'
		
		if not os.path.isdir(self.sPathForSave):
			os.makedirs(self.sPathForOldFiles)
		else:
			for file in os.listdir(self.sPathForSave):
				if file == 'previous_results' or 'sampler_dictionary.p':
					continue
				call(['mv', self.sPathForSave + file, self.sPathForOldFiles + file])
			
		
		
		# ------------------------------------------------
		# initialize walkers and corner plot
		# ------------------------------------------------


		initialParameters = np.zeros(numDim)
		lLabelsForCorner = ['' for i in xrange(numDim)]
		for i, set in enumerate(sParameters):
			lLabelsForCorner[i] = '%s' % set[0]
			initialParameters[i] = set[1]


		# chain dictionary will have the following format
		# dSampler[walkers] = [sampler_000, sampler_001, ...]

		loadedPreviousSampler = False
		try:
			# two things will fail potentially
			# 1. open if file doesn't exist
			# 2. posWalkers load since initialized to None
			with open(self.sPathForSave + 'sampler_dictionary.p', 'r') as fPrevSampler:
				dSampler = pickle.load(fPrevSampler)
				prevSampler = dSampler[numWalkers][-1]
				# need to load in weird way bc can't pickle
				# ensembler object
				posWalkers = prevSampler['_chain'][:,-1,:]
				randomState = prevSampler['_random']
			loadedPreviousSampler = True
			print '\nSuccessfully loaded previous chain!\n'
		except:
			print '\nCould not load previous sampler or none existed - starting new sampler.\n'


		if not loadedPreviousSampler:
			posWalkers = [(np.random.randn(numDim)+initialParameters)*fractionalDeviationStartPos + initialParameters for i in xrange(numWalkers)]
			randomState = None
			
			# create file if it doesn't exist
			if not os.path.exists(self.sPathForSave + 'sampler_dictionary.p'):
				with open(self.sPathForSave + 'sampler_dictionary.p', 'w') as fPrevSampler:
					dSampler = {}
					dSampler[numWalkers] = []
					pickle.dump(dSampler, fPrevSampler)
			else:
				with open(self.sPathForSave + 'sampler_dictionary.p', 'r') as fPrevSampler:
					dSampler = pickle.load(fPrevSampler)
				with open(self.sPathForSave + 'sampler_dictionary.p', 'w') as fPrevSampler:
					dSampler[numWalkers] = []
					pickle.dump(dSampler, fPrevSampler)


		print '\n\nBeginning MCMC sampler\n\n'
		print '\nNumber of walkers * number of steps = %d * %d = %d function calls\n' % (numWalkers, numSteps, numWalkers*numSteps)
		sampler = emcee.EnsembleSampler(numWalkers, numDim, func, threads=numThreads, kwargs={'d_gpu_scale':d_gpu_scale, 'gpu_compute':gpu_compute})
		
		# run mcmc sampler
		
		startTimeMCMC = time.time()
		
		currentStep = 1
		with click.progressbar(sampler.sample(posWalkers, iterations=numSteps, rstate0=randomState), length=numSteps) as mcmc_sampler:
			for pos, lnprob, state in mcmc_sampler:
				#print '\nCompleted step %d of %d.\n' % (currentStep, numSteps)
				currentStep += 1
			
		totalTimeMCMC = (time.time() - startTimeMCMC) / 3600.
		print '\n\n%d function calls took %.2f hours.\n\n' % (numWalkers*numSteps, totalTimeMCMC)


		"""
		for pos, lnprob, state in sampler.sample(posWalkers, iterations=numSteps):
			print '\nCompleted step %d of %d.\n' % (currentStep, numSteps)
			currentStep += 1
		"""


		# ------------------------------------------------
		# Prepare and save data and plots
		# ------------------------------------------------

		#print sampler.__dict__
		dictionary_for_sampler = sampler.__dict__
		if 'lnprobfn' in dictionary_for_sampler:
			del dictionary_for_sampler['lnprobfn']
		if 'pool' in dictionary_for_sampler:
			del dictionary_for_sampler['pool']

		with open(self.sPathForSave + 'sampler_dictionary.p', 'r') as fPrevSampler:
			dSampler = pickle.load(fPrevSampler)
		fPrevSampler.close()

		fPrevSampler = open(self.sPathForSave + 'sampler_dictionary.p', 'w')
		dSampler[numWalkers].append(sampler.__dict__)
		pickle.dump(dSampler, fPrevSampler)
		fPrevSampler.close()

		

		#sampler.run_mcmc(posWalkers, numSteps) # shortcut of above method
		pickle.dump(sampler.acceptance_fraction, open(self.sPathForSave + 'sampler_acceptance_fraction.p', 'w'))
		try:
			pickle.dump(sampler.acor, open(self.sPathForSave + 'sampler_acor.p', 'w'))
		except:
			print '\n\nWARNING: Not enough sample points to estimate the autocorrelation time - this likely means that the fit is bad since the burn-in time was not reached.\n\n'
		"""
		try:
			samples = sampler.chain[:, int(max(sampler.acor)):, :].reshape((-1, numDim))
			fig = corner.corner(samples, labels=lLabelsForCorner)
			fig.savefig(self.sPathForSave + 'corner_plot.png')
		except AssertionError:
			print 'Too few samples (number of samples fewer than number of dimensions) - cannot reasonably create correlation plots.'
			sys.exit()
		"""




if __name__ == '__main__':
	copy_reg.pickle(types.MethodType, reduce_method)

	# create fake data
	#test = neriX_simulation_analysis(15, 4.5, 1.054, 62, use_fake_data=False, create_fake_data=True, numMCEvents=4100, assumeRelativeAccidentalRate=0.2)
	#test.create_fake_data(9.08, 4.82, 0.3, 0.1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
	
	# create test data
	test = neriX_simulation_analysis(15, 4.5, 1.054, 45, use_fake_data=False, accidentalBkgAdjustmentTerm=0.0, numMCEvents=50000)
	test.perform_mc_match_full(12.05, 6.5, 0.3, 0.001, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, drawFit=True, gpu_compute=False)
	
	sParametersFullMatching = (('photon_yield', 10.), ('charge_yield', 8.), ('res_s1', 0.3), ('res_s2', 0.1), ('n_g1', 0), ('n_res_spe', 0), ('n_par0_tac_eff', 0), ('n_par0_pf_eff', 0), ('n_par1_pf_eff', 0), ('n_g2', 0), ('n_gas_gain_mean', 0), ('n_gas_gain_width', 0), ('n_par0_trig_eff', 0), ('n_par1_trig_eff', 0), ('n_par0_e_to_i', 0), ('n_par1_e_to_i', 0), ('n_par2_e_to_i', 0))

	# try using emcee to fit
	#mcRuntime = time.time()
	#test.run_mcmc('full_matching', sParametersFullMatching, 64, 10, 8, gpu_compute=False) #10240
	#test.run_mcmc('full_matching', sParametersFullMatching, 64, 10, 2, gpu_compute=True) #10240
	#print 'mcmc time: %f s' % (time.time() - mcRuntime)





