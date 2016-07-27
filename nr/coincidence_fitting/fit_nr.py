
import matplotlib
matplotlib.use('QT4Agg')
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
from math import exp, factorial, erf, ceil, log, pow, floor, lgamma
from scipy import optimize, misc, stats
from scipy.stats import norm
import copy_reg, types, pickle, click, time
from subprocess import call
from pprint import pprint
from produce_nest_yields import nest_nr_mean_yields
from scipy.stats import multivariate_normal, binom

import cuda_full_observables_production
from pycuda.compiler import SourceModule
import pycuda.driver as drv
import pycuda.tools
import pycuda.gpuarray

#drv.init()
#dev = drv.Device(0)
#ctx = dev.make_context(drv.ctx_flags.SCHED_AUTO | drv.ctx_flags.MAP_HOST)
import pycuda.autoinit

#gpu_observables_func = SourceModule(cuda_full_observables_production.cuda_full_observables_production_code, no_extern_c=True).get_function('gpu_full_observables_production')
gpu_observables_func = SourceModule(cuda_full_observables_production.cuda_full_observables_production_code, no_extern_c=True).get_function('gpu_full_observables_production_with_log_hist_spline')




import matplotlib.pyplot as plt
import matplotlib.mlab as mlab
import rootpy.compiled as C
from numpy.random import normal, binomial, seed, poisson
from rootpy.plotting import root2matplotlib as rplt

path_to_this_module = neriX_simulation_config.path_to_this_module
print path_to_this_module
C.register_file('%s/../../../python_modules/mc_code/c_log_likelihood.C' % path_to_this_module, ['smart_log_likelihood'])
c_log_likelihood = C.smart_log_likelihood

#import gc
#gc.disable()

"""
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
"""

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



def smart_log_likelihood(aData, aMC, num_mc_events, confidenceIntervalLimit=0.95):
	totalLogLikelihood = 0.
	for (data, mc) in zip(aData, aMC):
		if mc == 0 and data != 0:
			# use 95% confidence interval
			# confidenceIntervalLimit = 1 - probability of the zero occuring
			probabiltyOfSuccess = 1. - (1.-confidenceIntervalLimit)**(1./num_mc_events)
			#totalLogLikelihood += smart_log(smart_binomial(data, num_mc_events, probabiltyOfSuccess))
			totalLogLikelihood += binom.logpmf(data, num_mc_events, probabiltyOfSuccess)
		else:
			#totalLogLikelihood += data*smart_log(mc) - mc - smart_stirling(data)
			totalLogLikelihood += data*np.log(mc) - mc - lgamma(data+1)

	return totalLogLikelihood



def smart_log_likelihood_quantiles(aData, aMC, num_mc_events, aS1BinCenters, aS2BinCenters, lowerQuantileS1, upperQuantileS1, lowerQuantileS2, upperQuantileS2, confidenceIntervalLimit=0.95):
	totalLogLikelihood = 0.
	
	try:
		aMC = aMC*np.sum(aData)/np.sum(aMC)
	except:
		return -np.inf
	
	for s1_index, s1_value in enumerate(aS1BinCenters):
		if s1_value < lowerQuantileS1:
			continue
		if s1_value > upperQuantileS1:
			break
		for s2_index, s2_value in enumerate(aS2BinCenters):
			if s2_value < lowerQuantileS2:
				continue
			if s2_value > upperQuantileS2:
				break

			# other wise calculate as you normally would
			data, mc = aData[s1_index, s2_index], aMC[s1_index, s2_index]
			if mc == 0 and data != 0:
				# use 95% confidence interval
				# confidenceIntervalLimit = 1 - probability of the zero occuring
				probabiltyOfSuccess = 1. - (1.-confidenceIntervalLimit)**(1./num_mc_events)
				totalLogLikelihood += smart_log(smart_binomial(data, num_mc_events, probabiltyOfSuccess))
			else:
				totalLogLikelihood += data*smart_log(mc) - mc - smart_stirling(data)

	return totalLogLikelihood




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






class fit_nr(object):
	def __init__(self, d_coincidence_data, d_band_data=None, free_exciton_to_ion_ratio=False, num_mc_events=500000, name_notes=None):
	
		# d_coincidence_data should be in the following form:
		# d_coincidence_data['degree_settings'] = [2300, 3000, ...]
		# d_coincidence_data['cathode_settings'] = [0.345, 1.054, ...]
		# d_coincidence_data['cathode_settings'] = [4.5]
	
		copy_reg.pickle(types.MethodType, reduce_method)
		
		self.num_mc_events = int(num_mc_events)
		
		
		# ------------------------------------------------
		# ------------------------------------------------
		# Set paths and allowed values
		# ------------------------------------------------
		# ------------------------------------------------
		

		l_allowed_degree_settings = [2300, 3000, 3500, 4500, 5300, 6200]
		l_allowed_cathode_settings = [0.345, 1.054, 2.356]
		l_allowed_anode_settings = [4.5]

		self.d_cathode_voltages_to_field = {0.345:190,
									   1.054:490,
									   2.356:1020} # in kV:V/cm
		self.d_degree_setting_to_energy_name = {2300:3,
										   3000:5,
										   3500:7,
										   4500:10,
										   5300:15,
										   6200:20} # colloquial names of each
		self.d_degree_setting_to_mean_energy = {2300:2.96,
										   3000:4.93,
										   3500:6.61,
										   4500:9.76,
										   5300:13.88,
										   6200:17.50} # actual energies that degree corresponds to

		self.name_notes = name_notes
		self.path_to_coincidence_data = '%s/../analysis/full_angle_files/' % (neriX_simulation_config.path_to_this_module)
		self.path_to_energy_spectra = '%s/simulated_data/' % (neriX_simulation_config.path_to_this_module)
		self.path_to_reduced_energy_spectra = '%s/reduced_simulation_data/' % (neriX_simulation_config.path_to_this_module)

		self.l_energy_settings = [300, 0, 30]


		# ------------------------------------------------
		# ------------------------------------------------
		# Get information for data
		# ------------------------------------------------
		# ------------------------------------------------


		self.d_coincidence_data_information = {}
		
		# get all degrees and cathode settings and sort
		self.l_degree_settings_in_use = sorted(d_coincidence_data['degree_settings'])
		self.l_cathode_settings_in_use = sorted(d_coincidence_data['cathode_settings'])
		
		for cathode_setting in self.l_cathode_settings_in_use:
			self.d_coincidence_data_information[cathode_setting] = {}
			for degree_setting in self.l_degree_settings_in_use:
				self.d_coincidence_data_information[cathode_setting][degree_setting] = {}
				
				# add basic information to dictionary
				self.d_coincidence_data_information[cathode_setting][degree_setting]['file'] = File('%sresults_%.3fkV_%ddeg.root' % (self.path_to_coincidence_data, cathode_setting, degree_setting), 'read')
				self.d_coincidence_data_information[cathode_setting][degree_setting]['s1_hist'] = self.d_coincidence_data_information[cathode_setting][degree_setting]['file'].hS1
				self.d_coincidence_data_information[cathode_setting][degree_setting]['s2_hist'] = self.d_coincidence_data_information[cathode_setting][degree_setting]['file'].hS2
				self.d_coincidence_data_information[cathode_setting][degree_setting]['s1_s2_hist'] = self.d_coincidence_data_information[cathode_setting][degree_setting]['file'].hS1S2
				self.d_coincidence_data_information[cathode_setting][degree_setting]['log_s2_s1_hist'] = self.d_coincidence_data_information[cathode_setting][degree_setting]['file'].hLogS2S1
				self.d_coincidence_data_information[cathode_setting][degree_setting]['a_log_s2_s1'] = neriX_analysis.convert_2D_hist_to_matrix(self.d_coincidence_data_information[cathode_setting][degree_setting]['log_s2_s1_hist'], dtype=np.float32)
		
		# grab S1, S2, and log settings of any histogram
		# since they should all be the same
		h_first_s1 = self.d_coincidence_data_information[self.l_cathode_settings_in_use[0]][self.l_degree_settings_in_use[0]]['s1_hist']
		h_first_s2 = self.d_coincidence_data_information[self.l_cathode_settings_in_use[0]][self.l_degree_settings_in_use[0]]['s2_hist']
		h_first_log_s2_s1 = self.d_coincidence_data_information[self.l_cathode_settings_in_use[0]][self.l_degree_settings_in_use[0]]['log_s2_s1_hist']
		
		self.l_s1_settings = [h_first_s1.GetNbinsX(), h_first_s1.GetXaxis().GetXmin(), h_first_s1.GetXaxis().GetXmax()]
		self.l_s2_settings = [h_first_s2.GetNbinsX(), h_first_s2.GetXaxis().GetXmin(), h_first_s2.GetXaxis().GetXmax()]
		self.l_log_settings = [h_first_log_s2_s1.nbins(1), h_first_log_s2_s1.GetYaxis().GetXmin(), h_first_log_s2_s1.GetYaxis().GetXmax()]
		
		
		# define bin edges for use in MC
		self.a_s1_bin_edges = np.linspace(self.l_s1_settings[1], self.l_s1_settings[2], num=self.l_s1_settings[0]+1, dtype=np.float32)
		self.a_s2_bin_edges = np.linspace(self.l_s2_settings[1], self.l_s2_settings[2], num=self.l_s2_settings[0]+1, dtype=np.float32)
		self.a_log_bin_edges = np.linspace(self.l_log_settings[1], self.l_log_settings[2], num=self.l_log_settings[0]+1, dtype=np.float32)
		
		#print self.l_s1_settings, self.l_s2_settings, self.l_log_settings
		
		# ------------------------------------------------
		# ------------------------------------------------
		# Check that given settings are appropriate
		# ------------------------------------------------
		# ------------------------------------------------
		
		
		for cathode_setting in self.l_cathode_settings_in_use:
			assert cathode_setting in l_allowed_cathode_settings, '%.3f kV is not an allowed cathode setting: %s' % (cathode_setting, str(self.l_allowed_cathode_settings))
		
		for degree_setting in self.l_degree_settings_in_use:
			assert degree_setting in l_allowed_degree_settings, '%d deg is not an allowed degree setting: %s' % (degree_setting, str(self.l_allowed_degree_settings))
		
		
		
		# set whether or not we can fit electron to ion ratio
		if len(self.l_cathode_settings_in_use) == 1 and not free_exciton_to_ion_ratio:
			print '\nCannot fit exciton to ion ratio with only one field - setting as a nuissance parameter.\n'
			self.free_exciton_to_ion_ratio = False
		else:
			print '\nCurrently not setup to handle free exciton to ion ratio so setting to nuissance parameter.\n'
			self.free_exciton_to_ion_ratio = False
		
		
		
		# ------------------------------------------------
		# ------------------------------------------------
		# Load MC files or create reduced file if not present
		# and load into easy_graph
		# ------------------------------------------------
		# ------------------------------------------------

		self.load_mc_data('MC')
		
		# fill energy array
		self.fill_energy_array(view_energy_spectrum=False)


		#print self.d_coincidence_data_information[self.l_cathode_settings_in_use[0]][self.l_degree_settings_in_use[0]]



		# ------------------------------------------------
		# ------------------------------------------------
		# Set default parameter values for use in MC
		# ------------------------------------------------
		# ------------------------------------------------
		
		
		neriX_analysis.warning_message('Detector parameters hard-coded')
		
		self.l_means_g1_g2 = [0.12961637894547148, 20.88715549107226]
		self.l_cov_matrix_g1_g2 = [[3.6039167387306653e-06, 0.00057442202683395834], [0.00057442202683395834, 0.093143781598001851]]
		
		self.gas_gain_value = 21.85
		self.gas_gain_uncertainty = 0.3
		
		self.gas_gain_width = 9.2
		self.gas_gain_width_uncertainty = 0.3
		
		self.spe_res_value = 0.66
		self.spe_res_uncertainty = 0.2
		
		self.l_means_pf_stdev_pars = [0.014673497158526188, 0.5284272187436192, 4.3229124008196411]
		self.l_cov_matrix_pf_stdev_pars = [[9.4927705182690691e-10, 2.352933249729148e-08, -2.4920029049639913e-07], [2.352933249729148e-08, 4.8381636138100317e-06, -1.7993529191388666e-05], [-2.4920029049639913e-07, -1.7993529191388666e-05, 0.00013121519103705991]]
		
		self.l_means_pf_eff_pars = [1.96178522, 0.46718076]
		self.l_cov_matrix_pf_eff_pars = [[7.58956686e-05, 9.78759376e-07], [9.78759376e-07, 2.79732862e-05]]
		
		# only for producing initial distribution
		# NOT FOR LIKELIHOOD
		self.l_means_s1_eff_pars = [3, 1.2] #[7.95634366, 0.59582331]
		self.l_cov_matrix_s1_eff_pars = [[.3**2, 0], [0, .1**2]]
		
		# only for producing initial distribution
		# NOT FOR LIKELIHOOD
		self.l_means_s2_eff_pars = [150, 60]
		self.l_cov_matrix_s2_eff_pars = [[ 80**2, 0], [0, 20**2]]
		
		
		
		
		# -----------------------------------------------
		# -----------------------------------------------
		# set spline points in energy
		# -----------------------------------------------
		# -----------------------------------------------
		
		
		#
		
		# will fill spline points dynamically since depends on
		# energies given
		l_spline_energies = [0.5]
		for degree_setting in self.l_degree_settings_in_use:
			l_spline_energies.append(self.d_degree_setting_to_mean_energy[degree_setting])
		l_spline_energies.append(25.)
		
		self.a_spline_energies = np.asarray(l_spline_energies, dtype=np.float32)
		
		
		print '\n\nEnergy Spline Points:\n'
		pprint(list(self.a_spline_energies))
		print '\n\n'
	
		self.a_nest_photon_yields = np.zeros(len(self.a_spline_energies), dtype=np.float32)
		self.a_nest_charge_yields = np.zeros(len(self.a_spline_energies), dtype=np.float32)
		for i in xrange(len(self.a_spline_energies)):
			l_yields = nest_nr_mean_yields(self.a_spline_energies[i], self.d_cathode_voltages_to_field[cathode_setting])
			self.a_nest_photon_yields[i] = l_yields[0]
			self.a_nest_charge_yields[i] = l_yields[1]
		
		
		
		



	def read_g1_from_file(self, g1_root_file):
		return (0, 0)
	
	
	
	def read_g2_from_file(self, g2_root_file):
		return (0, 0)
	
	
	
	def read_gas_gain_from_file(self, gas_gain_root_file):
		return (0, 0)
	
	
	
	def fill_energy_array(self, view_energy_spectrum=False):
		
		random.seed()
		
		for cathode_setting in self.l_cathode_settings_in_use:
			for degree_setting in self.l_degree_settings_in_use:
		
				self.d_coincidence_data_information[cathode_setting][degree_setting]['a_energy'] = np.zeros(self.num_mc_events, dtype=np.float32)
				for i in xrange(self.num_mc_events):
					self.d_coincidence_data_information[cathode_setting][degree_setting]['a_energy'][i] = self.d_coincidence_data_information[cathode_setting][degree_setting]['h_energy'].GetRandom()
						
				if view_energy_spectrum:
					cEnergySpec = Canvas()
					hEnergySpec = self.hMC.empty_clone()
					hEnergySpec.fill_array(self.aEnergy)
					hEnergySpec.Draw()
					cEnergySpec.Update()
					raw_input('Press enter to continue...')
					
					del hEnergySpec
					del cEnergySpec


		


	def load_mc_data(self, sFileType):
	
		for cathode_setting in self.l_cathode_settings_in_use:
			for degree_setting in self.l_degree_settings_in_use:
	
	
				# check if reduced file exists
				current_path_to_reduced_energy_spectra = '%snerixsim-%dkeV.root' % (self.path_to_reduced_energy_spectra, self.d_degree_setting_to_energy_name[degree_setting])
				
				
				if not os.path.exists(current_path_to_reduced_energy_spectra):
					# if the file doesn't exist we need to create it!
				
					# load the raw data file
					current_path_to_energy_spectra = '%snerixsim-%dkeV.root' % (self.path_to_energy_spectra, self.d_degree_setting_to_energy_name[degree_setting])
					f_simulation = File(current_path_to_energy_spectra, 'read')

					if sFileType == 'MC':

						# set cuts
						xRadius = '(sqrt(xpos[0]**2+ypos[0]**2) < 18)'
						xZ = '(zpos[0]>-20 && zpos[0]<-4)'
						xSingleScatter = '(nsteps_target==1)'
						xLiqSciHeight = '(etotliqsci>700)'
						xLXeEnergy = '(etot_target>0)'
						#xLXeEnergy = '(etot_target>0)'
						xLXeEnergy = '(Alt$(ed_nr_target[1],-1)==-1 && ed_nr_target[0] > 0)'

						#print 'No TOF cut!\n'
						neriX_analysis.warning_message('Need to finalize how tof is chosen in MC data!')
						
						low_time_tof = -1
						high_time_tof = -1
						if degree_setting == 2300:
							low_time_tof = 24
							high_time_tof = 34
						elif degree_setting == 3000:
							low_time_tof = 31
							high_time_tof = 41
						elif degree_setting == 3500:
							low_time_tof = 15
							high_time_tof = 25
						elif degree_setting == 4500:
							low_time_tof = 22
							high_time_tof = 34
						elif degree_setting == 5300:
							low_time_tof = 14
							high_time_tof = 24
						elif degree_setting == 6200:
							low_time_tof = 8
							high_time_tof = 25
						
						
						xTOF = '(timeliqsci-tpos[0]> %d && timeliqsci-tpos[0] < %d)' % (low_time_tof, high_time_tof)

						xAll = '%s && %s && %s && %s && %s && %s' % (xRadius, xZ, xSingleScatter, xLiqSciHeight, xLXeEnergy, xTOF)
					
					elif sFileType == 'MC_accidentals':
						# set cuts
						xRadius = '(sqrt(xpos[0]**2+ypos[0]**2) < 18)'
						xZ = '(zpos[0]>-20 && zpos[0]<-4)'
						xSingleScatter = '(nsteps_target==1)'
						#xLXeEnergy = '(etot_target>0)' # this includes ER
						xSingleScatter_2 = '(Alt$(ed_nr_target[1],-1)==-1)'
						xLXeEnergy = '(ed_nr_target[0] > 0.)'

						xAll = '%s && %s && %s && %s && %s' % (xRadius, xZ, xSingleScatter, xLXeEnergy, xSingleScatter_2)


					# create and fill histogram
					t_mc = f_simulation.t2
					h_mc = Hist(*self.l_energy_settings, name='h_mc', title='Simulated Spectrum', drawstyle='hist')
					#t_mc.Draw('etot_target', hist=hMC, selection=xAll)
					t_mc.Draw('ed_nr_target[0]', hist=h_mc, selection=xAll)
					h_mc.Sumw2()

					# draw histogram to new file for quick retrieval
					f_reduced = File(current_path_to_reduced_energy_spectra, 'recreate')
					h_mc.Write()
					f_reduced.Close()
				
					neriX_analysis.success_message('Created new reduced MC file!')
				
				

				self.f_reduced = File(current_path_to_reduced_energy_spectra, 'read')
				h_mc = self.f_reduced.h_mc
				#g_mc = neriX_analysis.convert_hist_to_graph_with_poisson_errors(h_mc)
	
				self.d_coincidence_data_information[cathode_setting][degree_setting]['h_energy'] = h_mc
				#self.d_coincidence_data_information[cathode_setting][degree_setting]['g_energy'] = g_mc




	def get_g1_g2_default(self, g1, g2):
		return multivariate_normal.pdf([g1, g2], self.l_means_g1_g2, self.l_cov_matrix_g1_g2), g1, g2
	
	
	
	
	# get likelihood and gas gain given random variable (nuissance parameter)
	# gasGainRV should be normally distributed
	def get_gas_gain_default(self, gasGainRV):
		return norm.pdf(gasGainRV), self.gas_gain_value + (gasGainRV*self.gas_gain_uncertainty)
	
	
	# get likelihood and gas gain width given random variable (nuissance parameter)
	# gasGainWidthRV should be normally distributed
	def get_gas_gain_width_default(self, gasGainWidthRV):
		return norm.pdf(gasGainWidthRV), self.gas_gain_width + (gasGainWidthRV*self.gas_gain_width_uncertainty)
	
	
	# get likelihood and spe res width given random variable (nuissance parameter)
	# gasGainWidthRV should be normally distributed
	def get_spe_res_default(self, speResRV):
		return norm.pdf(speResRV), self.spe_res_value + (speResRV*self.spe_res_uncertainty)
	
	
	# get likelihood and s1 efficiency parameters given random variable (nuissance parameter)
	def get_pf_eff_default(self, pf_eff_par0, pf_eff_par1):
		return multivariate_normal.pdf([pf_eff_par0, pf_eff_par1], self.l_means_pf_eff_pars, self.l_cov_matrix_pf_eff_pars), pf_eff_par0, pf_eff_par1
	
	
	
	def get_pf_stdev_default(self, pf_stdev_par0, pf_stdev_par1, pf_stdev_par2):
		return multivariate_normal.pdf([pf_stdev_par0, pf_stdev_par1, pf_stdev_par2], self.l_means_pf_stdev_pars, self.l_cov_matrix_pf_stdev_pars), pf_stdev_par0, pf_stdev_par1, pf_stdev_par2
	


	def get_s2_eff_default(self, s2_eff_par0, s2_eff_par1):
		return multivariate_normal.pdf([s2_eff_par0, s2_eff_par1], self.l_means_s2_eff_pars, self.l_cov_matrix_s2_eff_pars), s2_eff_par0, s2_eff_par1
	
	
	
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
		
	
	
	def get_likelihood_exciton_to_ion(self, par0, par1, par2):
		return norm.pdf(par0)*norm.pdf(par1)*norm.pdf(par2)



	def get_prior_log_likelihood_nuissance(self, likelihoodNuissance):
		if likelihoodNuissance > 1e-65:
			return np.log(likelihoodNuissance)
		else:
			return -np.inf



	def get_log_likelihood_s1_eff(self, l_s1_eff_pars):
		# first par is center, second is shape
		if 0 < l_s1_eff_pars[0] < 10 and 0.1 < l_s1_eff_pars[1] < 6:
			return 0.
		else:
			return -np.inf



	def get_log_likelihood_s2_eff(self, l_s2_eff_pars):
		# first par is center, second is shape
		if -100 < l_s2_eff_pars[0] < 1000 and 1 < l_s2_eff_pars[1] < 1000:
			return 0.
		else:
			return -np.inf



	def get_prior_log_likelihood_resolution(self, intrinsicResolution):
		if intrinsicResolution < 0 or intrinsicResolution > 0.5:
			return -np.inf
		else:
			return 0



	def get_prior_log_likelihood_scale_par(self, scale_par):
		if scale_par < 0:
			return -np.inf
		else:
			return 0



	def get_prior_log_likelihood_yields(self, l_yields):
		for value in l_yields:
			if value < 0. or value > 18.:
				return -np.inf
		return 0.



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
	def ln_likelihood_coincidence_matching(self, a_py, a_qy, intrinsic_res_s1, intrinsic_res_s2, g1_value, spe_res_rv, g2_value, gas_gain_rv, gas_gain_width_rv, pf_eff_par0, pf_eff_par1, s1_eff_par0, s1_eff_par1, s2_eff_par0, s2_eff_par1, pf_stdev_par0, pf_stdev_par1, pf_stdev_par2, exciton_to_ion_par0_rv, exciton_to_ion_par1_rv, exciton_to_ion_par2_rv, d_scale_pars, draw_fit=False, lowerQuantile=0.0, upperQuantile=1.0, gpu_compute=True):
	
		#start_time_priors = time.time()

		assert len(self.a_spline_energies) == len(a_py) == len(a_qy), '\nNumber of spline points (%d) must match the number of photon yields (%d) and charge yields (%d) given.\n' % (len(self.a_spline_energies), len(a_py), len(a_qy))


		if gpu_compute:
			# need to do slightly fewer elements than
			# originally perscribed bc energies already
			# produced
			d_gpu_scale = {}
			d_gpu_scale['block'] = (1024,1,1)
			numBlocks = floor(self.num_mc_events / 1024.)
			d_gpu_scale['grid'] = (int(numBlocks), 1)
			num_mc_events = int(numBlocks*1024)

		#print d_gpu_scale

		# -----------------------------------------------
		# -----------------------------------------------
		# determine prior likelihood and variables
		# -----------------------------------------------
		# -----------------------------------------------


		prior_ln_likelihood = 0
		
		
		# priors of yields
		# can do them all together since uniform within
		# the same range
		prior_ln_likelihood += self.get_prior_log_likelihood_yields(a_py + a_qy)
		
		
		# priors of resolutions
		prior_ln_likelihood += self.get_prior_log_likelihood_resolution(intrinsic_res_s1)
		prior_ln_likelihood += self.get_prior_log_likelihood_resolution(intrinsic_res_s2)
		
		
		# get exciton to ion prior
		current_likelihood = self.get_likelihood_exciton_to_ion(exciton_to_ion_par0_rv, exciton_to_ion_par1_rv, exciton_to_ion_par2_rv)
		prior_ln_likelihood += self.get_prior_log_likelihood_nuissance(current_likelihood)


		# priors of detector variables
		current_likelihood, g1_value, g2_value = self.get_g1_g2_default(g1_value, g2_value)
		prior_ln_likelihood += self.get_prior_log_likelihood_nuissance(current_likelihood)
		
		current_likelihood, spe_res = self.get_spe_res_default(spe_res_rv)
		prior_ln_likelihood += self.get_prior_log_likelihood_nuissance(current_likelihood)


		current_likelihood, gas_gain_value = self.get_gas_gain_default(gas_gain_rv)
		prior_ln_likelihood += self.get_prior_log_likelihood_nuissance(current_likelihood)

		current_likelihood, gas_gain_width = self.get_gas_gain_width_default(gas_gain_width_rv)
		prior_ln_likelihood += self.get_prior_log_likelihood_nuissance(current_likelihood)


		# priors of efficiencies
		current_likelihood, pf_eff_par0, pf_eff_par1 = self.get_pf_eff_default(pf_eff_par0, pf_eff_par1)
		prior_ln_likelihood += self.get_prior_log_likelihood_nuissance(current_likelihood)
		
		#current_likelihood, s2_eff_par0, s2_eff_par1 = self.get_s2_eff_default(s2_eff_par0, s2_eff_par1)
		#prior_ln_likelihood += self.get_prior_log_likelihood_nuissance(current_likelihood)
		
		
		prior_ln_likelihood += self.get_log_likelihood_s1_eff([s1_eff_par0, s1_eff_par1])
		
		prior_ln_likelihood += self.get_log_likelihood_s2_eff([s2_eff_par0, s2_eff_par1])
		
		current_likelihood, pf_stdev_par0, pf_stdev_par1, pf_stdev_par2 = self.get_pf_stdev_default(pf_stdev_par0, pf_stdev_par1, pf_stdev_par2)
		prior_ln_likelihood += self.get_prior_log_likelihood_nuissance(current_likelihood)

		extraction_efficiency = g2_value / float(gas_gain_value)


		# if prior is -inf then don't bother with MC
		if not np.isfinite(prior_ln_likelihood) and not draw_fit:
			return -np.inf

		total_ln_likelihood = prior_ln_likelihood

		# -----------------------------------------------
		# -----------------------------------------------
		# run MC
		# -----------------------------------------------
		# -----------------------------------------------


		seed = np.asarray(int(time.time()*1000), dtype=np.int32)
		num_trials = np.asarray(num_mc_events, dtype=np.int32)
		num_spline_points = np.asarray(len(self.a_spline_energies), dtype=np.int32)
		
		a_spline_energies = np.asarray(self.a_spline_energies, dtype=np.float32)
		a_spline_photon_yields = np.asarray(a_py, dtype=np.float32)
		#a_spline_photon_yields = np.asarray(self.a_nest_photon_yields, dtype=np.float32)
		a_spline_charge_yields = np.asarray(a_qy, dtype=np.float32)
		#a_spline_charge_yields = np.asarray(self.a_nest_charge_yields, dtype=np.float32)
		
		g1_value = np.asarray(g1_value, dtype=np.float32)
		extraction_efficiency = np.asarray(extraction_efficiency, dtype=np.float32)
		spe_res = np.asarray(spe_res, dtype=np.float32)
		gas_gain_value = np.asarray(gas_gain_value, dtype=np.float32)
		gas_gain_width = np.asarray(gas_gain_width, dtype=np.float32)
		
		intrinsic_res_s1 = np.asarray(intrinsic_res_s1, dtype=np.float32)
		intrinsic_res_s2 = np.asarray(intrinsic_res_s2, dtype=np.float32)

		exciton_to_ion_par0_rv = np.asarray(exciton_to_ion_par0_rv, dtype=np.float32)
		exciton_to_ion_par1_rv = np.asarray(exciton_to_ion_par1_rv, dtype=np.float32)
		exciton_to_ion_par2_rv = np.asarray(exciton_to_ion_par2_rv, dtype=np.float32)
		
		pf_eff_par0 = np.asarray(pf_eff_par0, dtype=np.float32)
		pf_eff_par1 = np.asarray(pf_eff_par1, dtype=np.float32)
		
		s1_eff_par0 = np.asarray(s1_eff_par0, dtype=np.float32)
		s1_eff_par1 = np.asarray(s1_eff_par1, dtype=np.float32)

		s2_eff_par0 = np.asarray(s2_eff_par0, dtype=np.float32)
		s2_eff_par1 = np.asarray(s2_eff_par1, dtype=np.float32)
		
		a_pf_stdev = np.asarray([pf_stdev_par0, pf_stdev_par1, pf_stdev_par2], dtype=np.float32)
		
		a_band_cut = np.asarray([-5, 0, 0, 1, 10], dtype=np.float32)
		
		# for histogram binning
		num_bins_s1 = np.asarray(self.l_s1_settings[0], dtype=np.int32)
		gpu_bin_edges_s1 = pycuda.gpuarray.to_gpu(self.a_s1_bin_edges)
		num_bins_s2 = np.asarray(self.l_log_settings[0], dtype=np.int32)
		gpu_bin_edges_s2 = pycuda.gpuarray.to_gpu(self.a_log_bin_edges)
		a_hist_2d = np.zeros(self.l_s1_settings[0]*self.l_log_settings[0], dtype=np.int32)
		
		#print a_spline_photon_yields
		#print self.a_nest_photon_yields
		#print a_spline_charge_yields
		#print self.a_nest_charge_yields
		#print s2_eff_par0, s2_eff_par1


		#print '\nTime for priors: %.4f\n' % (time.time() - start_time_priors)

		for cathode_setting in self.l_cathode_settings_in_use:
			mean_field = np.asarray(self.d_cathode_voltages_to_field[cathode_setting], dtype=np.float32)
				
			for degree_setting in self.l_degree_settings_in_use:

				#start_time_mc = time.time()
				if gpu_compute:
				
					try:
						self.d_coincidence_data_information[cathode_setting][degree_setting]['gpu_a_energy']
					except:
						self.d_coincidence_data_information[cathode_setting][degree_setting]['gpu_a_energy'] = pycuda.gpuarray.to_gpu(self.d_coincidence_data_information[cathode_setting][degree_setting]['a_energy'])
					
					
					
					# for histogram
					tArgs = (drv.In(seed), drv.In(num_trials), drv.In(mean_field), self.d_coincidence_data_information[cathode_setting][degree_setting]['gpu_a_energy'], drv.In(num_spline_points), drv.In(a_spline_energies), drv.In(a_spline_photon_yields), drv.In(a_spline_charge_yields), drv.In(g1_value), drv.In(extraction_efficiency), drv.In(gas_gain_value), drv.In(gas_gain_width), drv.In(spe_res), drv.In(intrinsic_res_s1), drv.In(intrinsic_res_s2), drv.In(a_pf_stdev), drv.In(exciton_to_ion_par0_rv), drv.In(exciton_to_ion_par1_rv), drv.In(exciton_to_ion_par2_rv), drv.In(pf_eff_par0), drv.In(pf_eff_par1), drv.In(s1_eff_par0), drv.In(s1_eff_par1), drv.In(s2_eff_par0), drv.In(s2_eff_par1), drv.In(a_band_cut), drv.In(num_bins_s1), gpu_bin_edges_s1, drv.In(num_bins_s2), gpu_bin_edges_s2, drv.InOut(a_hist_2d))
					
					gpu_observables_func(*tArgs, **d_gpu_scale)
					#print a_hist_2d[:4]

					a_s1_s2_mc = np.reshape(a_hist_2d, (self.l_log_settings[0], self.l_s1_settings[0])).T
					
				
				else:
					print '\nC loop not implemented!\n'
					sys.exit()

				#print '\nTime for MC loop %d deg: %.4f\n' % (degree_setting, time.time() - start_time_mc)


				# -----------------------------------------------
				# -----------------------------------------------
				# Bin MC results and if requested draw
				# comparison of data and MC
				# -----------------------------------------------
				# -----------------------------------------------


				#start_time_post_mc = time.time()

				#print d_scale_pars[cathode_setting][degree_setting]
				a_s1_s2_mc = np.asarray(np.multiply(a_s1_s2_mc, float(d_scale_pars[cathode_setting][degree_setting])/float(self.num_mc_events)), dtype=np.float32)


				if draw_fit:
				
					# normalizing distributions for drawing only
					#print '\nNormalizing spectra for drawing only.\n'
					#a_s1_s2_mc *= (np.sum(self.d_coincidence_data_information[cathode_setting][degree_setting]['a_log_s2_s1']) / float(np.sum(a_s1_s2_mc)))

					f, (ax1, ax2) = plt.subplots(2, sharex=True, sharey=True)
					
					s1_s2_data_plot = np.rot90(self.d_coincidence_data_information[cathode_setting][degree_setting]['a_log_s2_s1'])
					s1_s2_data_plot = np.flipud(s1_s2_data_plot)
					ax1.pcolormesh(self.a_s1_bin_edges, self.a_log_bin_edges, s1_s2_data_plot)
					
					s1_s2_mc_plot = np.rot90(a_s1_s2_mc)
					s1_s2_mc_plot = np.flipud(s1_s2_mc_plot)
					
					
					ax2.pcolormesh(self.a_s1_bin_edges, self.a_log_bin_edges, s1_s2_mc_plot)
					#plt.colorbar()
					
					
					c1 = Canvas(1400, 400)
					c1.Divide(2)
					
					h_s1_data = Hist(*self.l_s1_settings, name='hS1_draw_data')
					root_numpy.array2hist(self.d_coincidence_data_information[cathode_setting][degree_setting]['a_log_s2_s1'].sum(axis=1), h_s1_data)
					
					hS1MC = Hist(*self.l_s1_settings, name='hS1_draw_mc')
					root_numpy.array2hist(a_s1_s2_mc.sum(axis=1), hS1MC)
					
					s1_scale_factor = h_s1_data.Integral() / float(hS1MC.Integral())
					
					g_s1_data = neriX_analysis.convert_hist_to_graph_with_poisson_errors(h_s1_data)
					g_s1_mc = neriX_analysis.convert_hist_to_graph_with_poisson_errors(hS1MC, scale=s1_scale_factor)
					
					g_s1_mc.SetFillColor(root.kBlue)
					g_s1_mc.SetMarkerColor(root.kBlue)
					g_s1_mc.SetLineColor(root.kBlue)
					g_s1_mc.SetFillStyle(3005)
					
					g_s1_data.SetTitle('S1 Comparison')
					g_s1_data.GetXaxis().SetTitle('S1 [PE]')
					g_s1_data.GetYaxis().SetTitle('Counts')
					
					g_s1_data.SetLineColor(root.kRed)
					g_s1_data.SetMarkerSize(0)
					g_s1_data.GetXaxis().SetRangeUser(self.l_s1_settings[1], self.l_s1_settings[2])
					g_s1_data.GetYaxis().SetRangeUser(0, 1.2*max(h_s1_data.GetMaximum(), hS1MC.GetMaximum()))
					
					c1.cd(1)
					g_s1_data.Draw('ap')
					g_s1_mc.Draw('same')
					g_s1_mc_band = g_s1_mc.Clone()
					g_s1_mc_band.Draw('3 same')
					
					h_s2_data = Hist(*self.l_log_settings, name='h_s2_draw_data')
					root_numpy.array2hist(self.d_coincidence_data_information[cathode_setting][degree_setting]['a_log_s2_s1'].sum(axis=0), h_s2_data)
					
					h_s2_mc = Hist(*self.l_log_settings, name='h_s2_draw_mc')
					root_numpy.array2hist(a_s1_s2_mc.sum(axis=0), h_s2_mc)
					
					s2_scale_factor = h_s2_data.Integral() / float(h_s2_mc.Integral())
					
					g_s2_data = neriX_analysis.convert_hist_to_graph_with_poisson_errors(h_s2_data)
					g_s2_mc = neriX_analysis.convert_hist_to_graph_with_poisson_errors(h_s2_mc, scale=s2_scale_factor)
					
					g_s2_mc.SetFillColor(root.kBlue)
					g_s2_mc.SetMarkerColor(root.kBlue)
					g_s2_mc.SetLineColor(root.kBlue)
					g_s2_mc.SetFillStyle(3005)
					
					g_s2_data.SetTitle('S2 Comparison')
					g_s2_data.GetXaxis().SetTitle('S2 [PE]')
					g_s2_data.GetYaxis().SetTitle('Counts')
					
					g_s2_data.SetLineColor(root.kRed)
					g_s2_data.SetMarkerSize(0)
					g_s2_data.GetXaxis().SetRangeUser(self.l_s2_settings[1], self.l_s2_settings[2])
					g_s2_data.GetYaxis().SetRangeUser(0, 1.2*max(h_s2_data.GetMaximum(), h_s2_mc.GetMaximum()))
					
					c1.cd(2)
					g_s2_data.Draw('ap')
					g_s2_mc.Draw('same')
					g_s2_mc_band = g_s2_mc.Clone()
					g_s2_mc_band.Draw('3 same')
					
					c1.Update()
					
					print 'Number of events in data: %f' % h_s1_data.Integral()
					
					plt.show()
					#raw_input('Press enter to continue...')
				



				if lowerQuantile != 0. or upperQuantile != 1.:
					a_quantiles_results = np.zeros(2, dtype=np.float64)
					a_quantiles = np.asarray([lowerQuantile, upperQuantile], dtype=np.float64)
					
					self.h_s1_data.GetQuantiles(2, a_quantiles_results, a_quantiles)
					lowerQuantileS1 = a_quantiles_results[0]
					upperQuantileS1 = a_quantiles_results[1]
					
					self.h_s2_data.GetQuantiles(2, a_quantiles_results, a_quantiles)
					lowerQuantileS2 = a_quantiles_results[0]
					upperQuantileS2 = a_quantiles_results[1]
					
					#print lowerQuantileS1, upperQuantileS1
					#print lowerQuantileS2, upperQuantileS2
				

				if lowerQuantile != 0. or upperQuantile != 1.:
					log_likelihood_matching = smart_log_likelihood_quantiles(self.aS1S2, aS1S2MC, num_mc_elements, aS1BinCenters, aS2BinCenters, lowerQuantileS1, upperQuantileS1, lowerQuantileS2, upperQuantileS2)
				else:
					#print self.d_coincidence_data_information[cathode_setting][degree_setting]['a_log_s2_s1']
					#print a_s1_s2_mc
					flatS1S2Data = self.d_coincidence_data_information[cathode_setting][degree_setting]['a_log_s2_s1'].flatten()
					flatS1S2MC = a_s1_s2_mc.flatten()
					#log_likelihood_matching = smart_log_likelihood(flatS1S2Data, flatS1S2MC, self.num_mc_events)
					log_likelihood_matching = c_log_likelihood(flatS1S2Data, flatS1S2MC, len(flatS1S2Data), self.num_mc_events, 0.95)

				total_ln_likelihood += log_likelihood_matching

				#print '\nTime for post MC loop %d deg: %.4f\n' % (degree_setting, time.time() - start_time_post_mc)

				#print total_ln_likelihood
				if np.isnan(total_ln_likelihood):
					return -np.inf



		return total_ln_likelihood
		
		
		
	def wrapper_ln_likelihood_coincidence_matching(self, a_parameters, kwargs={}):
		#print a_parameters
		
		#start_time_wrapper = time.time()
		
		num_yields = len(self.a_spline_energies)
		
		offset_for_scales = 2*num_yields+19
		d_scale_pars = {}
		for cathode_setting in self.l_cathode_settings_in_use:
			d_scale_pars[cathode_setting] = {}
			for degree_setting in self.l_degree_settings_in_use:
				d_scale_pars[cathode_setting][degree_setting] = a_parameters[offset_for_scales]
				offset_for_scales += 1
		
		likelihood =  self.ln_likelihood_coincidence_matching(a_py=a_parameters[:num_yields], a_qy=a_parameters[num_yields:num_yields*2], intrinsic_res_s1=a_parameters[2*num_yields+0], intrinsic_res_s2=a_parameters[2*num_yields+1], g1_value=a_parameters[2*num_yields+2], spe_res_rv=a_parameters[2*num_yields+3], g2_value=a_parameters[2*num_yields+4], gas_gain_rv=a_parameters[2*num_yields+5], gas_gain_width_rv=a_parameters[2*num_yields+6], pf_eff_par0=a_parameters[2*num_yields+7], pf_eff_par1=a_parameters[2*num_yields+8], s1_eff_par0=a_parameters[2*num_yields+9], s1_eff_par1=a_parameters[2*num_yields+10], s2_eff_par0=a_parameters[2*num_yields+11], s2_eff_par1=a_parameters[2*num_yields+12], pf_stdev_par0=a_parameters[2*num_yields+13], pf_stdev_par1=a_parameters[2*num_yields+14], pf_stdev_par2=a_parameters[2*num_yields+15], exciton_to_ion_par0_rv=a_parameters[2*num_yields+16], exciton_to_ion_par1_rv=a_parameters[2*num_yields+17], exciton_to_ion_par2_rv=a_parameters[2*num_yields+18], d_scale_pars=d_scale_pars, **kwargs)
		#print '\nTime for total call: %.4f\n' % (time.time() - start_time_wrapper)
		return likelihood
	
	
	def wrapper_ln_likelihood_coincidence_matching_fixed_nuissance(self, a_parameters, kwargs={}):
		#print a_parameters
		
		g1_value, g2_value = self.l_means_g1_g2
		gas_gain_rv = 0
		gas_gain_width_rv = 0
		spe_res_rv = 0
		pf_eff_par0, pf_eff_par1 = self.l_means_pf_eff_pars
		pf_stdev_par0, pf_stdev_par1, pf_stdev_par2 = self.l_means_pf_stdev_pars
		exciton_to_ion_par0_rv, exciton_to_ion_par1_rv, exciton_to_ion_par2_rv = 0, 0, 0
		
		num_yields = len(self.a_spline_energies)
		
		# scales are tricky since changes per dataset
		# scales MUST be given (and interpreted) in order of cathode
		# and then degree setting
		d_scale_pars = {}
		offset_for_scales = 2*num_yields+6
		for cathode_setting in self.l_cathode_settings_in_use:
			d_scale_pars[cathode_setting] = {}
			for degree_setting in self.l_degree_settings_in_use:
				#print degree_setting
				d_scale_pars[cathode_setting][degree_setting] = a_parameters[offset_for_scales]
				offset_for_scales += 1
	
		#print d_scale_pars
	
		# ln_likelihood_coincidence_matching(self, a_py, a_qy, intrinsic_res_s1, intrinsic_res_s2, g1_value, spe_res_rv, g2_value, gas_gain_rv, gas_gain_width_rv, pf_eff_par0, pf_eff_par1, s1_eff_par0, s1_eff_par1, s2_eff_par0, s2_eff_par1, pf_stdev_par0, pf_stdev_par1, pf_stdev_par2, exciton_to_ion_par0_rv, exciton_to_ion_par1_rv, exciton_to_ion_par2_rv, d_scale_pars, draw_fit=False, lowerQuantile=0.0, upperQuantile=1.0, gpu_compute=True)

	
		return self.ln_likelihood_coincidence_matching(a_py=a_parameters[:num_yields], a_qy=a_parameters[num_yields:num_yields*2], intrinsic_res_s1=a_parameters[2*num_yields+0], intrinsic_res_s2=a_parameters[2*num_yields+1], g1_value=g1_value, spe_res_rv=spe_res_rv, g2_value=g2_value, gas_gain_rv=gas_gain_rv, gas_gain_width_rv=gas_gain_width_rv, pf_eff_par0=pf_eff_par0, pf_eff_par1=pf_eff_par1, s1_eff_par0=a_parameters[2*num_yields+2], s1_eff_par1=a_parameters[2*num_yields+3], s2_eff_par0=a_parameters[2*num_yields+4], s2_eff_par1=a_parameters[2*num_yields+5], pf_stdev_par0=pf_stdev_par0, pf_stdev_par1=pf_stdev_par1, pf_stdev_par2=pf_stdev_par2, exciton_to_ion_par0_rv=exciton_to_ion_par0_rv, exciton_to_ion_par1_rv=exciton_to_ion_par1_rv, exciton_to_ion_par2_rv=exciton_to_ion_par2_rv, d_scale_pars=d_scale_pars, **kwargs)
		
		
		
	
	def initial_positions_for_ensemble(self, a_free_parameters, num_walkers, percent_deviation):
		l_par_names = ['a_py', 'a_qy', 'intrinsic_res_s1', 'intrinsic_res_s2', 'g1_value', 'spe_res_rv', 'g2_value', 'gas_gain_rv', 'gas_gain_width_rv', 'pf_eff_par0', 'pf_eff_par1', 's1_eff_par0', 's1_eff_par1', 's2_eff_par0', 's2_eff_par1', 'pf_stdev_par0', 'pf_stdev_par1', 'pf_stdev_par2', 'exciton_to_ion_par0_rv', 'exciton_to_ion_par1_rv', 'exciton_to_ion_par2_rv', 'd_scale_pars']
		
		d_variable_arrays = {}
		num_yields = len(self.a_spline_energies)
		
		# position array should be (num_walkers, num_dim)
		
		"""
		g1_value, g2_value = self.l_means_g1_g2
		gas_gain_rv = 0
		gas_gain_width_rv = 0
		spe_res_rv = 0
		pf_eff_par0, pf_eff_par1 = self.l_means_pf_eff_pars
		pf_stdev_par0, pf_stdev_par1, pf_stdev_par2 = self.l_means_pf_stdev_pars
		exciton_to_ion_par0_rv, exciton_to_ion_par1_rv, exciton_to_ion_par2_rv = 0, 0, 0
		"""
	
		for par_name in l_par_names:
			# handle photon and charge yield initial positions
			if par_name == 'a_py':
				d_variable_arrays[par_name] = a_free_parameters[:num_yields]
			
			elif par_name == 'a_qy':
				d_variable_arrays[par_name] = a_free_parameters[num_yields:2*num_yields]

			elif par_name == 'intrinsic_res_s1':
				d_variable_arrays[par_name] = a_free_parameters[2*num_yields+0]
				
			elif par_name == 'intrinsic_res_s2':
				d_variable_arrays[par_name] = a_free_parameters[2*num_yields+1]
			
			# handle g1 and g2 with g1 only
			elif par_name == 'g1_value':
				d_variable_arrays['g1_value'], d_variable_arrays['g2_value'] = self.l_means_g1_g2


			# catch both parameters of s1 efficiency prior
			elif par_name == 'pf_eff_par0':
				d_variable_arrays['pf_eff_par0'], d_variable_arrays['pf_eff_par1'] = self.l_means_pf_eff_pars
				
			
			elif par_name == 's1_eff_par0':
				d_variable_arrays['s1_eff_par0'], d_variable_arrays['s1_eff_par1'] = a_free_parameters[2*num_yields+2], a_free_parameters[2*num_yields+3]


			elif par_name == 's2_eff_par0':
				d_variable_arrays['s2_eff_par0'], d_variable_arrays['s2_eff_par1'] = a_free_parameters[2*num_yields+4], a_free_parameters[2*num_yields+5]
				
				
			elif par_name == 'pf_stdev_par0':
				d_variable_arrays['pf_stdev_par0'], d_variable_arrays['pf_stdev_par1'], d_variable_arrays['pf_stdev_par2'] = self.l_means_pf_stdev_pars
				
				
			elif par_name == 'd_scale_pars':
				d_variable_arrays[par_name] = a_free_parameters[2*num_yields+6:]
			
			
			# catch all normally distributed RVs
			else:
				if par_name == 'g2_value' or par_name == 'pf_eff_par1' or par_name == 's1_eff_par1' or par_name == 's2_eff_par1'  or par_name == 'pf_stdev_par1' or par_name == 'pf_stdev_par2':
					continue
				d_variable_arrays[par_name] = np.random.randn()
				
				
		l_parameters = []
		
		for par_name in l_par_names:
			# check if item in list is iterable and if not
			# just append it, otherwise append one value
			# at a time
			try:
				for value in d_variable_arrays[par_name]:
					l_parameters.append(value)
			except:
				l_parameters.append(d_variable_arrays[par_name])
		
		l_parameters = np.asarray(l_parameters)
		#print l_parameters

		return emcee.utils.sample_ball(l_parameters, l_parameters*percent_deviation, num_walkers)
	
	


	def run_mcmc(self, num_steps=200, num_walkers=1000, num_threads=1, fractional_deviation_start_pos=0.01, proposal_scale=2.0):
	
		num_dim = 3*len(self.a_spline_energies) - 2 + 19 # 41
		print '\n\nFitting yields...\n\n'
		
		#l_guesses = [0. for j in xrange(num_dim)]
		#for i, name in enumerate(l_par_names):
		#	l_guesses[i] = d_variable_guesses[name]
		
		# get string for cathode voltages in use
		s_cathode_voltages = ''
		for cathode_setting in self.l_cathode_settings_in_use:
			s_cathode_voltages += '%s,' % (cathode_setting)
		s_cathode_voltages = s_cathode_voltages[:-1]
		
		# get string for degree settings in use
		s_degree_settings = ''
		for degree_setting in self.l_degree_settings_in_use:
			s_degree_settings += '%s,' % (degree_setting)
		s_degree_settings = s_degree_settings[:-1]
		
		# before emcee, setup save locations
		dir_specifier_name = '%s_kV_%s_deg' % (s_cathode_voltages, s_degree_settings)
		self.results_directory_name = neriX_simulation_config.results_directory_name
		self.path_for_save = '%s/yields_fit/%s/' % (self.results_directory_name, dir_specifier_name)
		
		
		if not os.path.isdir(self.path_for_save):
			os.makedirs(self.path_for_save)
		
		
		# chain dictionary will have the following format
		# d_sampler[walkers] = [sampler_000, sampler_001, ...]

		loaded_prev_sampler = False
		try:
			# two things will fail potentially
			# 1. open if file doesn't exist
			# 2. posWalkers load since initialized to None
			with open(self.path_for_save + 'sampler_dictionary.p', 'r') as f_prev_sampler:
				d_sampler = pickle.load(f_prev_sampler)
				prevSampler = d_sampler[num_walkers][-1]
				# need to load in weird way bc can't pickle
				# ensembler object
				starting_pos = prevSampler['_chain'][:,-1,:]
				random_state = prevSampler['_random']
			loaded_prev_sampler = True
			print '\nSuccessfully loaded previous chain!\n'
		except:
			print '\nCould not load previous sampler or none existed - starting new sampler.\n'
		
		if not loaded_prev_sampler:
		
			if num_dim == 41:
				a_free_parameter_guesses = [1.27, 6.71, 8.19, 6.66, 8.45, 10.28, 10.69, 12.67,
											9.20, 5.42, 6.65, 6.87, 5.65, 5.64, 5.04, 4.93,
											0.20, 0.074, 6.56, 2.23, 370.24, 651.12,
											4481, 1923, 1705, 694, 1728, 1901]
			else:
				print '\nPlease run differential evolution minimizer for this setup and implement results in source code.\n'
				sys.exit()
		
		
			#starting_pos = self.initial_positions_nr_band_no_nest(num_walkers)
			starting_pos = self.initial_positions_for_ensemble(a_free_parameter_guesses, num_walkers=num_walkers, percent_deviation=fractional_deviation_start_pos)
			
			random_state = None
			
			# create file if it doesn't exist
			if not os.path.exists(self.path_for_save + 'sampler_dictionary.p'):
				with open(self.path_for_save + 'sampler_dictionary.p', 'w') as f_prev_sampler:
					d_sampler = {}
					d_sampler[num_walkers] = []
					pickle.dump(d_sampler, f_prev_sampler)
			else:
				with open(self.path_for_save + 'sampler_dictionary.p', 'r') as f_prev_sampler:
					d_sampler = pickle.load(f_prev_sampler)
				with open(self.path_for_save + 'sampler_dictionary.p', 'w') as f_prev_sampler:
					d_sampler[num_walkers] = []
					pickle.dump(d_sampler, f_prev_sampler)
	
	
	

		#print starting_pos

		sampler = emcee.EnsembleSampler(num_walkers, num_dim, self.wrapper_ln_likelihood_coincidence_matching, a=proposal_scale, threads=num_threads)



		print '\n\nBeginning MCMC sampler\n\n'
		print '\nNumber of walkers * number of steps = %d * %d = %d function calls\n' % (num_walkers, num_steps, num_walkers*num_steps)
		
		start_time_mcmc = time.time()

		with click.progressbar(sampler.sample(p0=starting_pos, iterations=num_steps, rstate0=random_state), length=num_steps) as mcmc_sampler:
			for pos, lnprob, state in mcmc_sampler:
				pass
				
		total_time_mcmc = (time.time() - start_time_mcmc) / 3600.
		print '\n\n%d function calls took %.2f hours.\n\n' % (num_walkers*num_steps, total_time_mcmc)

		#samples = sampler.chain[:, 10:, :].reshape((-1, num_dim))
		#print samples

		#fig = corner.corner(samples)
		#fig.savefig(self.path_for_save + 'corner_plot.png')


		# ------------------------------------------------
		# Prepare and save data and plots
		# ------------------------------------------------

		#print sampler.__dict__
		dictionary_for_sampler = sampler.__dict__
		if 'lnprobfn' in dictionary_for_sampler:
			del dictionary_for_sampler['lnprobfn']
		if 'pool' in dictionary_for_sampler:
			del dictionary_for_sampler['pool']

		with open(self.path_for_save + 'sampler_dictionary.p', 'r') as f_prev_sampler:
			d_sampler = pickle.load(f_prev_sampler)
		#f_prev_sampler.close()

		f_prev_sampler = open(self.path_for_save + 'sampler_dictionary.p', 'w')
		d_sampler[num_walkers].append(sampler.__dict__)
		pickle.dump(d_sampler, f_prev_sampler)
		f_prev_sampler.close()

		

		#sampler.run_mcmc(posWalkers, numSteps) # shortcut of above method
		pickle.dump(sampler.acceptance_fraction, open(self.path_for_save + 'sampler_acceptance_fraction.p', 'w'))
		try:
			pickle.dump(sampler.acor, open(self.path_for_save + 'sampler_acor.p', 'w'))
		except:
			print '\n\nWARNING: Not enough sample points to estimate the autocorrelation time - this likely means that the fit is bad since the burn-in time was not reached.\n\n'



	def differential_evolution_minimizer_free_pars(self, a_bounds, maxiter=250, tol=0.05, popsize=15):
		def neg_log_likelihood_diff_ev(a_guesses):
			return -self.wrapper_ln_likelihood_coincidence_matching_fixed_nuissance(a_guesses, {'gpu_compute':True})
		print '\n\nStarting differential evolution minimizer...\n\n'
		result = optimize.differential_evolution(neg_log_likelihood_diff_ev, a_bounds, disp=True, maxiter=maxiter, tol=tol, popsize=popsize)
		print result








if __name__ == '__main__':
	copy_reg.pickle(types.MethodType, reduce_method)

	# d_coincidence_data should be in the following form:
	# d_coincidence_data['degree_settings'] = [2300, 3000, ...]
	# d_coincidence_data['cathode_settings'] = [0.345, 1.054, ...]
	# d_coincidence_data['cathode_settings'] = [4.5]

	d_coincidence_data = {}
	d_coincidence_data['degree_settings'] = [2300, 3000, 3500, 4500, 5300, 6200]
	d_coincidence_data['cathode_settings'] = [0.345]
	
	d_scale_pars = {}
	d_scale_pars[0.345] = {}
	d_scale_pars[0.345][2300] = 3000
	d_scale_pars[0.345][3000] = 1018
	d_scale_pars[0.345][3500] = 1018
	d_scale_pars[0.345][4500] = 390
	d_scale_pars[0.345][5300] = 1200
	d_scale_pars[0.345][6200] = 1200
	
	test = fit_nr(d_coincidence_data, num_mc_events=100000)


	#test.ln_likelihood_coincidence_matching(a_py=[1.03, 4.41, 5.80, 6.60, 7.64, 8.57, 9.19, 10.15], a_qy=[7.69, 6.67, 6.06, 5.72, 5.30, 4.93, 4.68, 4.25], intrinsic_res_s1=0.04, intrinsic_res_s2=0.3, g1_value=0.13, spe_res_rv=0., g2_value=20.89, gas_gain_rv=0, gas_gain_width_rv=0., pf_eff_par0=test.l_means_pf_eff_pars[0], pf_eff_par1=test.l_means_pf_eff_pars[1], s1_eff_par0=0.474, s1_eff_par1=3.94, s2_eff_par0=358, s2_eff_par1=576, pf_stdev_par0=test.l_means_pf_stdev_pars[0], pf_stdev_par1=test.l_means_pf_stdev_pars[1], pf_stdev_par2=test.l_means_pf_stdev_pars[2], exciton_to_ion_par0_rv=0., exciton_to_ion_par1_rv=0., exciton_to_ion_par2_rv=0., d_scale_pars=d_scale_pars , draw_fit=True, lowerQuantile=0.0, upperQuantile=1.0, gpu_compute=True)

	a_free_par_bounds = [(0.5, 2.5), (3.5, 9), (4, 9), (5, 10), (4.5, 12), (6, 14), (6, 14), (6, 14),
						(4, 11), (2.5, 9.5), (2.5, 9.5), (2.5, 9.5), (2.5, 9.5), (1.5, 8), (1.5, 8), (1.5, 8),
						(0.01, 0.5), (0.01, 0.5), (-5, 7), (0.1, 8), (150, 500), (10, 700), (100, 5000), (100, 5000), (100, 5000), (100, 5000), (100, 5000), (100, 5000)]

	test.differential_evolution_minimizer_free_pars(a_free_par_bounds, maxiter=150, popsize=30, tol=0.01)

	d_scale_pars = {}
	d_scale_pars[0.345] = {}
	d_scale_pars[0.345][2300] = 4480
	d_scale_pars[0.345][3000] = 1923
	d_scale_pars[0.345][3500] = 1705
	d_scale_pars[0.345][4500] = 693
	d_scale_pars[0.345][5300] = 1728
	d_scale_pars[0.345][6200] = 1901
	#test.ln_likelihood_coincidence_matching(a_py=[1.3, 6.7, 8.2, 6.7, 8.4, 10.3, 10.7, 12.7], a_qy=[9.2, 5.4, 6.6, 6.9, 5.6, 5.6, 5.0, 4.9], intrinsic_res_s1=0.20, intrinsic_res_s2=0.07, g1_value=0.13, spe_res_rv=0., g2_value=20.89, gas_gain_rv=0, gas_gain_width_rv=0., pf_eff_par0=test.l_means_pf_eff_pars[0], pf_eff_par1=test.l_means_pf_eff_pars[1], s1_eff_par0=6.56, s1_eff_par1=2.23, s2_eff_par0=370, s2_eff_par1=651, pf_stdev_par0=test.l_means_pf_stdev_pars[0], pf_stdev_par1=test.l_means_pf_stdev_pars[1], pf_stdev_par2=test.l_means_pf_stdev_pars[2], exciton_to_ion_par0_rv=0., exciton_to_ion_par1_rv=0., exciton_to_ion_par2_rv=0., d_scale_pars=d_scale_pars , draw_fit=True, lowerQuantile=0.0, upperQuantile=1.0, gpu_compute=True)

	"""
	test.wrapper_ln_likelihood_coincidence_matching_fixed_nuissance(np.array([  1.27290620e+00,   6.71207698e+00,   8.19298167e+00,

         6.66084841e+00,   8.44818991e+00,   1.02776487e+01,

         1.06877811e+01,   1.26692395e+01,   9.20009512e+00,

         5.41908097e+00,   6.65437281e+00,   6.87179658e+00,

         5.64982943e+00,   5.63923643e+00,   5.04561272e+00,

         4.93656201e+00,   2.04212864e-01,   7.42218324e-02,

         6.56276373e+00,   2.23638205e+00,   3.70238838e+02,

         6.51125214e+02,   4.48067786e+03,   1.92318162e+03,

         1.70534637e+03,   6.93589526e+02,   1.72821055e+03,

         1.90168983e+03,   4.89954856e+03,   4.14771204e+03]))
	"""

	#test.run_mcmc(num_steps=10, num_walkers=128, num_threads=1, fractional_deviation_start_pos=0.01)


	# enerigies: [0.5, 2.96, 4.93, 6.61, 9.76, 13.88, 17.5, 25]
	# py_nest: [1.03, 4.41, 5.80, 6.60, 7.64, 8.57, 9.19, 10.15]
	# qy_nest: [7.69, 6.67, 6.06, 5.72, 5.30, 4.93, 4.68, 4.25]
