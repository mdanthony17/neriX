#!/usr/bin/python
import matplotlib
matplotlib.use('QT4Agg')

import ROOT as root
import sys, os
import neriX_analysis, root_numpy
from rootpy.plotting import Hist2D, Hist, Legend, Canvas
from rootpy import stl
from rootpy.io import File
import numpy as np
from math import exp, factorial, erf, ceil, log, pow, floor
from scipy import optimize, misc, stats
from scipy.stats import norm
import copy_reg, types, pickle, click, time, emcee, corner
from pprint import pprint
from produce_nest_yields import nest_nr_mean_yields
import gc, nr_band_config

import matplotlib.pyplot as plt
import matplotlib.mlab as mlab
import rootpy.compiled as C

stl.vector(stl.vector('float'))
stl.vector(stl.vector('double'))
stl.vector(stl.vector('int'))


C.register_file('../../../python_modules/mc_code/c_full_observables_production_spline.C', ['full_matching_loop'])

c_full_matching_loop = C.full_matching_loop

def reduce_method(m):
	return (getattr, (m.__self__, m.__func__.__name__))


d_cathode_voltage_to_field = {0.345:210,
							  1.054:490,
							  2.356:1000}



def smart_log_likelihood(aData, aMC, numMCEvents, confidenceIntervalLimit=0.95):
	totalLogLikelihood = 0.
	for (data, mc) in zip(aData, aMC):
		if mc == 0 and data != 0:
			# use 95% confidence interval
			# confidenceIntervalLimit = 1 - probability of the zero occuring
			probabiltyOfSuccess = 1. - (1.-confidenceIntervalLimit)**(1./numMCEvents)
			totalLogLikelihood += smart_log(smart_binomial(data, numMCEvents, probabiltyOfSuccess))
			#print 'mc = 0 and data != 0'
		else:
			#print data, mc, data*smart_log(mc) - mc - smart_stirling(data)
			#print data*smart_log(mc), mc, smart_stirling(data)
			#print data*smart_log(mc) - mc - smart_stirling(data)
			totalLogLikelihood += data*smart_log(mc) - mc - smart_stirling(data)

	return totalLogLikelihood


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




def smart_log_likelihood_quantiles(aData, aMC, numMCEvents, aS1BinCenters, aS2BinCenters, lowerQuantileS1, upperQuantileS1, lowerQuantileS2, upperQuantileS2, confidenceIntervalLimit=0.95):
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
				probabiltyOfSuccess = 1. - (1.-confidenceIntervalLimit)**(1./numMCEvents)
				totalLogLikelihood += smart_log(smart_binomial(data, numMCEvents, probabiltyOfSuccess))
			else:
				totalLogLikelihood += data*smart_log(mc) - mc - smart_stirling(data)

	return totalLogLikelihood




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




class nr_band_fitter(object):
	def __init__(self, l_files, anodeVoltage, cathodeVoltage, degree_setting=-4, num_mc_events=500000, name_notes=None, yields_free=False):

		# make class methods pickleable for multithreading process
		copy_reg.pickle(types.MethodType, reduce_method)
		
		# set number of mc events
		self.num_mc_events = int(num_mc_events)
		
		# set whether yields are free or not
		self.yields_free = yields_free
		
		# find mean field
		self.mean_field = d_cathode_voltage_to_field[cathodeVoltage]

		# -----------------------------------------------
		# -----------------------------------------------
		# instantiate neriX_analysis and make cuts
		# -----------------------------------------------
		# -----------------------------------------------

		current_analysis = neriX_analysis.neriX_analysis(l_files, degree_setting, cathodeVoltage, anodeVoltage)

		current_analysis.add_z_cut()
		current_analysis.add_radius_cut(0, 0.85)
		current_analysis.add_single_scatter_cut()
		current_analysis.add_xs1asym_cut()
		current_analysis.add_xs2asym_cut()

		current_analysis.set_event_list()

		self.filename = current_analysis.get_filename_no_ext()
		self.cathode_setting = current_analysis.get_cathode_setting()
		self.anode_setting = current_analysis.get_anode_setting()

		# -----------------------------------------------
		# -----------------------------------------------
		# instantiate histogram and its settings and
		# then convert to 2d array
		# -----------------------------------------------
		# -----------------------------------------------

		self.s1_settings = (40, 0, 30)
		self.s2_settings = (40, 0, 3000)
		
		s1_branch = 'cpS1sTotBottom[0]'
		s2_branch = 'cpS2sTotBottom[0]'

		h_s1_s2 = Hist2D(*(self.s1_settings+self.s2_settings))
		current_analysis.Draw('%s:%s' % (s1_branch, s2_branch), hist=h_s1_s2, selection=current_analysis.get_cuts())
		
		#self.a_s1_s2 = neriX_analysis.convert_2D_hist_to_matrix(h_s1_s2)
		
		self.a_s1_bin_edges = np.linspace(self.s1_settings[1], self.s1_settings[2], num=self.s1_settings[0]+1)
		self.a_s2_bin_edges = np.linspace(self.s2_settings[1], self.s2_settings[2], num=self.s2_settings[0]+1)
		
		
		# -----------------------------------------------
		# -----------------------------------------------
		# read in detector parameters from file
		# -----------------------------------------------
		# -----------------------------------------------
		
		neriX_analysis.warning_message('Detector parameters hard-coded')
		
		self.g1_value = 0.129
		self.g1_uncertainty = 0.008
		
		self.g2_value = 20.9
		self.g2_uncertainty = 0.5
		
		self.gas_gain_value = 21.85
		self.gas_gain_uncertainty = 0.3
		
		self.gas_gain_width = 9.2
		self.gas_gain_width_uncertainty = 0.3
		
		self.spe_res_value = 0.7
		self.spe_res_uncertainty = 0.2
		
		
		
		# -----------------------------------------------
		# -----------------------------------------------
		# read in energy spectrum and fill array
		# -----------------------------------------------
		# -----------------------------------------------
		
		f_reduced_spectrum = File('../coincidence_fitting/reduced_simulation_data/nerixsim-pure_nr_spec.root', 'read')
		h_mc = f_reduced_spectrum.hMC
		self.a_energy = np.zeros(self.num_mc_events, dtype=np.float32)
		
		for i in xrange(self.num_mc_events):
			self.a_energy[i] = h_mc.GetRandom()


		
		# -----------------------------------------------
		# -----------------------------------------------
		# set spline points in energy
		# -----------------------------------------------
		# -----------------------------------------------
		
		self.a_spline_energies = np.asarray([0.5, 2.96, 4.93, 6.61, 9.76, 13.88, 17.5, 25], dtype=np.float32)
		
		print '\n\nEnergy Spline Points:\n'
		pprint(list(self.a_spline_energies))
		print '\n\n'
	
		self.a_nest_photon_yields = np.zeros(len(self.a_spline_energies), dtype=np.float32)
		self.a_nest_charge_yields = np.zeros(len(self.a_spline_energies), dtype=np.float32)
		for i in xrange(len(self.a_spline_energies)):
			l_yields = nest_nr_mean_yields(self.a_spline_energies[i], self.mean_field)
			self.a_nest_photon_yields[i] = l_yields[0]
			self.a_nest_charge_yields[i] = l_yields[1]




		# -----------------------------------------------
		# -----------------------------------------------
		# fill data histograms for later use
		# -----------------------------------------------
		# -----------------------------------------------
		
		
		#self.h_s1_data = Hist(*self.s1_settings, name='hS1_data')
		#root_numpy.array2hist(self.a_s1_s2.sum(axis=1), self.h_s1_data)
		
		#self.h_s2_data = Hist(*self.s2_settings, name='hS2_data')
		#root_numpy.array2hist(self.a_s1_s2.sum(axis=0), self.h_s2_data)
	
	
		# -----------------------------------------------
		# -----------------------------------------------
		# create upper and lower bands around
		# NR band for likelihood computation
		# in order to exclude outliers
		# -----------------------------------------------
		# -----------------------------------------------
		
		self.s1_fit_min_lower = 15.
		self.s1_fit_min_upper = 7.
		percent_from_median = 45.
		
		c_nr_band = Canvas(name='c_nr_band', title='c_nr_band', width=1600, height=500)
		c_nr_band.Divide(3, 1)

		c_nr_band.cd(1)
		
		h_s1_s2_band_fit = Hist2D(self.s1_settings[0], self.s1_settings[1], self.s1_settings[2], 200, self.s2_settings[1], self.s2_settings[2], name='h_s1_s2_band_fit', title='S1 vs S2 - ' + current_analysis.get_filename_no_ext())
		current_analysis.Draw('%s:%s' % (s1_branch, s2_branch), hist=h_s1_s2_band_fit, selection=current_analysis.get_cuts())
		h_s1_s2_band_fit.GetXaxis().SetTitle('%s [PE]' % (s1_branch))
		h_s1_s2_band_fit.GetYaxis().SetTitle('%s [PE]' % (s2_branch))
		h_s1_s2_band_fit.SetStats(0)
		h_s1_s2_band_fit.Draw('colz')
		
		t_profile = neriX_analysis.profile_y_median(h_s1_s2_band_fit, percent_from_median=percent_from_median)
		g_profile_s1 = root.TGraphAsymmErrors(*t_profile)
		#gProfileS1.SetFillStyle(3005)
		g_profile_s1.Draw('same p')

		c_nr_band.cd(1).SetLogz()
		c_nr_band.cd(1).SetGridx()
		c_nr_band.cd(1).SetGridy()
		c_nr_band.Update()
		
		
		num_points = t_profile[0]
		x_values = t_profile[1]
		y_lower_bound = t_profile[5]
		y_upper_bound = t_profile[6]
		for i, y_value in enumerate(t_profile[2]):
			y_lower_bound[i] = y_value - y_lower_bound[i]
			y_upper_bound[i] += y_value

		c_nr_band.cd(2)
			
		g_lower_bounds = root.TGraph(num_points, x_values, y_lower_bound)
		g_lower_bounds.SetMarkerStyle(8)
		g_lower_bounds.SetTitle('Lower Bounds @ %.2f%% - %s' % (50 + percent_from_median, current_analysis.get_filename_no_ext()))
		g_lower_bounds.GetXaxis().SetRangeUser(self.s1_settings[1], self.s1_settings[2])
		g_lower_bounds.GetYaxis().SetRangeUser(self.s2_settings[1], self.s2_settings[2])
		g_lower_bounds.Draw('ap')
		
		
		c_nr_band.cd(2).SetGridx()
		c_nr_band.cd(2).SetGridy()


		c_nr_band.cd(3)

		g_upper_bounds = root.TGraph(num_points, x_values, y_upper_bound)
		g_upper_bounds.SetMarkerStyle(8)
		g_upper_bounds.SetTitle('Upper Bounds @ %.2f%% - %s' % (50 + percent_from_median, current_analysis.get_filename_no_ext()))
		g_upper_bounds.GetXaxis().SetRangeUser(self.s1_settings[1], self.s1_settings[2])
		g_upper_bounds.GetYaxis().SetRangeUser(self.s2_settings[1], self.s2_settings[2])
		g_upper_bounds.Draw('ap')
		
		
		c_nr_band.cd(3).SetLogz()
		c_nr_band.cd(3).SetGridx()
		c_nr_band.cd(3).SetGridy()
		
		c_nr_band.Update()


		# fit lower bound
		f_lower_bound = root.TF1('f_lower_bound', 'pol1', self.s1_fit_min_lower, self.s1_settings[2])
		
		fit_result_lower = g_lower_bounds.Fit('f_lower_bound', 'MELRS')
		f_lower_bound.SetLineStyle(1)
		f_lower_bound.SetLineWidth(2)
		f_lower_bound.SetLineColor(root.kRed)
		
		c_nr_band.cd(1)
		f_lower_bound.Draw('same')
		c_nr_band.cd(2)
		f_lower_bound.Draw('same')

		# draw lower bound
		f_lower_bound_draw = root.TF1('f_lower_bound_draw', 'pol1', self.s1_settings[1], self.s1_settings[2])
		f_lower_bound_draw.SetParameters(f_lower_bound.GetParameter(0), f_lower_bound.GetParameter(1))
		f_lower_bound_draw.SetLineStyle(2)
		f_lower_bound_draw.SetLineWidth(2)
		f_lower_bound_draw.SetLineColor(root.kRed)
		
		c_nr_band.cd(1)
		f_lower_bound_draw.Draw('same')
		c_nr_band.cd(2)
		f_lower_bound_draw.Draw('same')
		
		
		
		# fit upper bound
		f_upper_bound = root.TF1('f_upper_bound', 'pol2', self.s1_fit_min_upper, self.s1_settings[2])
		
		fit_result_upper = g_upper_bounds.Fit('f_upper_bound', 'MELRS')
		f_upper_bound.SetLineStyle(1)
		f_upper_bound.SetLineWidth(2)
		f_upper_bound.SetLineColor(root.kRed)
		
		c_nr_band.cd(1)
		f_upper_bound.Draw('same')
		c_nr_band.cd(3)
		f_upper_bound.Draw('same')

		# draw upper bound
		f_upper_bound_draw = root.TF1('f_upper_bound_draw', 'pol2', self.s1_settings[1], self.s1_settings[2])
		f_upper_bound_draw.SetParameters(f_upper_bound.GetParameter(0), f_upper_bound.GetParameter(1), f_upper_bound.GetParameter(2))
		f_upper_bound_draw.SetLineStyle(2)
		f_upper_bound_draw.SetLineWidth(2)
		f_upper_bound_draw.SetLineColor(root.kRed)
		
		c_nr_band.cd(1)
		f_upper_bound_draw.Draw('same')
		c_nr_band.cd(3)
		f_upper_bound_draw.Draw('same')

		c_nr_band.Update()

		# lower_bound curve = [s1 lower limit (for application), par0, par1, ...]
		# it is important NOT to cut below the s1 lower limit
		# since otherwise will miss the interesting behavior
		self.l_lower_bound_curve = [self.s1_fit_min_lower, f_lower_bound.GetParameter(0), f_lower_bound.GetParameter(1)]
		s_lower_bound_nr_cut = '((%s < %f) || (%s > (%f + %f*%s)))' % (s1_branch, self.s1_fit_min_lower, s2_branch, f_lower_bound.GetParameter(0), f_lower_bound.GetParameter(1), s1_branch)

		# upper_bound_curve = [par0, par1, par2]
		# upper bound cut is used regardless of S1
		self.l_upper_bound_curve = [f_upper_bound.GetParameter(0), f_upper_bound.GetParameter(1), f_upper_bound.GetParameter(2)]
		s_upper_bound_nr_cut = '((%s > %f) || (%s < (%f + %f*%s + %f*%s^2)))' % (s1_branch, self.s1_settings[2], s2_branch, f_upper_bound.GetParameter(0), f_upper_bound.GetParameter(1), s1_branch, f_upper_bound.GetParameter(2), s1_branch)
		
		#raw_input('\nFits okay?\n')
		
		#print current_analysis.get_cuts() + '&& %s && %s' % (s_lower_bound_nr_cut, s_upper_bound_nr_cut)
		h_s1_s2_band_cut = Hist2D(*(self.s1_settings+self.s2_settings))
		current_analysis.Draw('%s:%s' % (s1_branch, s2_branch), hist=h_s1_s2_band_cut, selection=(current_analysis.get_cuts() + '&& %s && %s' % (s_lower_bound_nr_cut, s_upper_bound_nr_cut)))
		
		self.a_s1_s2 = neriX_analysis.convert_2D_hist_to_matrix(h_s1_s2_band_cut)
		
		c_nr_band.Clear()
		c_nr_band.Close()
		#del c_nr_band
		#root.gApplication.Terminate(0)
		root.gSystem.ProcessEvents()
		


	# get likelihood and g1 given random variable (nuissance parameter)
	# g1RV should be normally distributed
	def get_g1_default(self, g1RV):
		return norm.pdf(g1RV), self.g1_value + (g1RV*self.g1_uncertainty)
	
	
	
	# get likelihood and g2 given random variable (nuissance parameter)
	# g2RV should be normally distributed
	def get_g2_default(self, g2RV):
		return norm.pdf(g2RV), self.g2_value + (g2RV*self.g2_uncertainty)
	
	
	
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
	
	
	
	"""
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
		
	"""
	
	def get_log_likelihood_exciton_to_ion(self, par0, par1, par2):
		return np.log(norm.pdf(par0)*norm.pdf(par1)*norm.pdf(par2))



	def get_prior_log_likelihood_nuissance(self, likelihoodNuissance):
		if likelihoodNuissance > 1e-8:
			return np.log(likelihoodNuissance)
		else:
			return -np.inf



	def get_log_likelihood_s1_eff(self, l_s1_eff_pars):
		# first par is center, second is shape
		if -5 < l_s1_eff_pars[0] < 15 and 1e-3 < l_s1_eff_pars[1] < 30:
			return 0.
		else:
			return -np.inf



	def get_log_likelihood_s2_eff(self, l_s2_eff_pars):
		# first par is center, second is shape
		if -100 < l_s2_eff_pars[0] < 1000 and 10 < l_s2_eff_pars[1] < 1000:
			return 0.
		else:
			return -np.inf



	def get_prior_log_likelihood_resolution(self, intrinsicResolution):
		if intrinsicResolution < 0 or intrinsicResolution > 4:
			return -np.inf
		else:
			return 0



# do linear interpolation for light and charge yield (works
# for both cpu and gpu and decent approximation)
# need to feed exciton to ion ratio parameters into
# observables code because it depends on the energy

# ORRRRRRRR

# used closed form model since use exciton to ion ratio anyways


	def likelihood_nr_band_nest(self, intrinsic_res_s1, intrinsic_res_s2, g1_rv, spe_res_rv, g2_rv, gas_gain_rv, gas_gain_width_rv, s1_eff_par0, s1_eff_par1, s2_eff_par0, s2_eff_par1, exciton_to_ion_par0_rv, exciton_to_ion_par1_rv, exciton_to_ion_par2_rv, draw_fit=False, lowerQuantile=0.0, upperQuantile=1.0):


		# -----------------------------------------------
		# -----------------------------------------------
		# determine prior likelihood and variables
		# -----------------------------------------------
		# -----------------------------------------------


		prior_ln_likelihood = 0
		
		
		# priors of resolutions
		prior_ln_likelihood += self.get_prior_log_likelihood_resolution(intrinsic_res_s1)
		prior_ln_likelihood += self.get_prior_log_likelihood_resolution(intrinsic_res_s2)
		
		
		# get exciton to ion prior
		prior_ln_likelihood += self.get_log_likelihood_exciton_to_ion(exciton_to_ion_par0_rv, exciton_to_ion_par1_rv, exciton_to_ion_par2_rv)


		# priors of detector variables
		current_likelihood, g1_value = self.get_g1_default(g1_rv)
		prior_ln_likelihood += self.get_prior_log_likelihood_nuissance(current_likelihood)
		
		current_likelihood, spe_res = self.get_spe_res_default(spe_res_rv)
		prior_ln_likelihood += self.get_prior_log_likelihood_nuissance(current_likelihood)

		current_likelihood, g2_value = self.get_g2_default(g2_rv)
		prior_ln_likelihood += self.get_prior_log_likelihood_nuissance(current_likelihood)

		current_likelihood, gas_gain_value = self.get_gas_gain_default(gas_gain_rv)
		prior_ln_likelihood += self.get_prior_log_likelihood_nuissance(current_likelihood)

		current_likelihood, gas_gain_width = self.get_gas_gain_width_default(gas_gain_width_rv)
		prior_ln_likelihood += self.get_prior_log_likelihood_nuissance(current_likelihood)


		# priors of efficiencies
		prior_ln_likelihood += self.get_log_likelihood_s1_eff([s1_eff_par0, s1_eff_par1])
		prior_ln_likelihood += self.get_log_likelihood_s2_eff([s2_eff_par0, s2_eff_par1])

		extraction_efficiency = g2_value / float(gas_gain_value)


		# if prior is -inf then don't bother with MC
		if not np.isfinite(prior_ln_likelihood):
			return -np.inf


		# -----------------------------------------------
		# -----------------------------------------------
		# run MC
		# -----------------------------------------------
		# -----------------------------------------------


		aS1 = np.full(self.num_mc_events, -1, dtype=np.float32)
		aS2 = np.full(self.num_mc_events, -1, dtype=np.float32)
		
		seed = np.asarray(int(time.time()*1000), dtype=np.int32)
		num_trials = np.asarray(self.num_mc_events, dtype=np.int32)
		mean_field = np.asarray(self.mean_field, dtype=np.float32)
		num_spline_points = np.asarray(len(self.a_spline_energies), dtype=np.int32)
		
		a_spline_energies = np.asarray(self.a_spline_energies, dtype=np.float32)
		a_spline_photon_yields = np.asarray(self.a_nest_photon_yields, dtype=np.float32)
		a_spline_charge_yields = np.asarray(self.a_nest_charge_yields, dtype=np.float32)
		
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
		
		s1_eff_par0 = np.asarray(s1_eff_par0, dtype=np.float32)
		s1_eff_par1 = np.asarray(s1_eff_par1, dtype=np.float32)

		s2_eff_par0 = np.asarray(s2_eff_par0, dtype=np.float32)
		s2_eff_par1 = np.asarray(s2_eff_par1, dtype=np.float32)
		
		
		# run the actual MC loop
		c_full_matching_loop(seed, num_trials, mean_field, aS1, aS2, self.a_energy, num_spline_points, a_spline_energies, a_spline_photon_yields, a_spline_charge_yields, g1_value, spe_res, extraction_efficiency, gas_gain_value, gas_gain_width, intrinsic_res_s1, intrinsic_res_s2, exciton_to_ion_par0_rv, exciton_to_ion_par1_rv, exciton_to_ion_par2_rv, s1_eff_par0, s1_eff_par1, s2_eff_par0, s2_eff_par1)




		# -----------------------------------------------
		# -----------------------------------------------
		# Bin MC results and if requested draw
		# comparison of data and MC
		# -----------------------------------------------
		# -----------------------------------------------


		a_s1_s2_mc, xEdges, yEdges = np.histogram2d(aS1, aS2, bins=[self.a_s1_bin_edges, self.a_s2_bin_edges])
		#a_s1_s2_mc *= np.sum(self.a_s1_s2) / np.sum(a_s1_s2_mc)

		if draw_fit:

			f, (ax1, ax2) = plt.subplots(2, sharex=True, sharey=True)
			
			s1_s2_data_plot = np.rot90(self.a_s1_s2)
			s1_s2_data_plot = np.flipud(s1_s2_data_plot)
			ax1.pcolormesh(self.a_s1_bin_edges, self.a_s2_bin_edges, s1_s2_data_plot)
			
			s1_s2_mc_plot = np.rot90(a_s1_s2_mc)
			s1_s2_mc_plot = np.flipud(s1_s2_mc_plot)
			ax2.pcolormesh(self.a_s1_bin_edges, self.a_s2_bin_edges, s1_s2_mc_plot)
			#plt.colorbar()
			
			
			c1 = Canvas(1400, 400)
			c1.Divide(2)
			
			h_s1_data = Hist(*self.s1_settings, name='hS1_draw_data', drawstyle='E1')
			root_numpy.array2hist(self.a_s1_s2.sum(axis=1), h_s1_data)
			
			hS1MC = Hist(*self.s1_settings, name='hS1_draw_mc', drawstyle='hist')
			root_numpy.array2hist(a_s1_s2_mc.sum(axis=1), hS1MC)
			
			hS1MC.Scale(h_s1_data.Integral() / hS1MC.Integral())
			
			h_s1_data.SetLineColor(root.kRed)
			h_s1_data.SetMarkerSize(0)
			
			c1.cd(1)
			h_s1_data.Draw()
			hS1MC.Draw('same')
			
			h_s2_data = Hist(*self.s2_settings, name='hS2_draw_data')
			root_numpy.array2hist(self.a_s1_s2.sum(axis=0), h_s2_data)
			
			hS2MC = Hist(*self.s2_settings, name='hS2_draw_mc', drawstyle='hist')
			root_numpy.array2hist(a_s1_s2_mc.sum(axis=0), hS2MC)
			hS2MC.Scale(h_s2_data.Integral() / hS2MC.Integral())
			
			h_s2_data.SetLineColor(root.kRed)
			h_s2_data.SetMarkerSize(0)
			
			c1.cd(2)
			h_s2_data.Draw()
			hS2MC.Draw('same')
			
			print 'Number of events in data: %f' % h_s1_data.Integral()
			
			plt.show()
			#raw_input('Press enter to continue...')
		
			del hS1MC, hS2MC, c1



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
			logLikelihoodMatching = smart_log_likelihood_quantiles(self.aS1S2, aS1S2MC, num_mc_elements, aS1BinCenters, aS2BinCenters, lowerQuantileS1, upperQuantileS1, lowerQuantileS2, upperQuantileS2)
		else:
			#print self.a_s1_s2
			#print a_s1_s2_mc
			flatS1S2Data = self.a_s1_s2.flatten()
			flatS1S2MC = a_s1_s2_mc.flatten()
			logLikelihoodMatching = smart_log_likelihood(flatS1S2Data, flatS1S2MC, self.num_mc_events)

		total_ln_likelihood = logLikelihoodMatching + prior_ln_likelihood
		total_ln_likelihood = total_ln_likelihood[0]
		#print total_ln_likelihood

		if np.isnan(total_ln_likelihood):
			return -np.inf
		else:
			return total_ln_likelihood
			
			
			
	def wrapper_nr_band_nest(self, a_parameters, kwargs={}):
		#print a_parameters
		return self.likelihood_nr_band_nest(*a_parameters, **kwargs)



	def fit_nr_band_nest(self, num_steps=200, num_walkers=1000, num_threads=1, fractional_deviation_start_pos=0.05):
	
		# number of dimensions fixed
		num_dim = 14
	
		# don't give zeros as starting values otherwise will be stuck
		starting_values = [0.1, 0.18, (np.random.rand() - 0.5)/2., (np.random.rand() - 0.5)/2., (np.random.rand() - 0.5)/2., (np.random.rand() - 0.5)/2., (np.random.rand() - 0.5)/2., 1.1, 3.2, 0, 75, (np.random.rand() - 0.5)/2., (np.random.rand() - 0.5)/2., (np.random.rand() - 0.5)/2.]
	
		# before emcee, setup save locations
		dir_specifier_name = '%.3fkV_%.1fkV' % (self.cathode_setting, self.anode_setting)
		self.results_directory_name = nr_band_config.results_directory_name
		self.path_for_save = '%s/%s/%s/' % (self.results_directory_name, dir_specifier_name, self.filename)
		
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
			starting_pos = [(np.random.randn(num_dim)+starting_values)*fractional_deviation_start_pos + starting_values for i in xrange(num_walkers)]
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
	
	
	

		#starting_pos = [(np.random.randn(num_dim)*starting_values)*(0.05) + starting_values for i in xrange(num_walkers)]

		sampler = emcee.EnsembleSampler(num_walkers, num_dim, self.wrapper_nr_band_nest, threads=num_threads, kwargs={})
		
		print '\n\nBeginning MCMC sampler\n\n'
		print '\nNumber of walkers * number of steps = %d * %d = %d function calls\n' % (num_walkers, num_steps, num_walkers*num_steps)
		
		start_time_mcmc = time.time()

		with click.progressbar(sampler.sample(starting_pos, iterations=num_steps, ), length=num_steps) as mcmc_sampler:
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
		f_prev_sampler.close()

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


if __name__ == '__main__':
	test = nr_band_fitter('nerix_160419_1331', 4.5, 0.345)
	#test.likelihood_nr_band_nest(intrinsic_res_s1=0.1, intrinsic_res_s2=0.18, g1_rv=0, spe_res_rv=0, g2_rv=0, gas_gain_rv=0, gas_gain_width_rv=0, s1_eff_par0=1.1, s1_eff_par1=3.2, s2_eff_par0=0, s2_eff_par1=75, exciton_to_ion_par0_rv=0, exciton_to_ion_par1_rv=0, exciton_to_ion_par2_rv=0, draw_fit=True)
	#test.likelihood_nr_band_nest(intrinsic_res_s1=0.22, intrinsic_res_s2=0.10, g1_rv=-3, spe_res_rv=1.5, g2_rv=-5, gas_gain_rv=1, gas_gain_width_rv=2.5, s1_eff_par0=0.5, s1_eff_par1=16., s2_eff_par0=1.5, s2_eff_par1=108., exciton_to_ion_par0_rv=-1.2, exciton_to_ion_par1_rv=4.5, exciton_to_ion_par2_rv=-2.2, draw_fit=True)
	#test.fit_nr_band_nest(num_steps=20, num_walkers=100, num_threads=6)

