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
from scipy.stats import multivariate_normal


import matplotlib.pyplot as plt
import matplotlib.mlab as mlab
import rootpy.compiled as C

stl.vector(stl.vector('float'))
stl.vector(stl.vector('double'))
stl.vector(stl.vector('int'))


C.register_file('../../../python_modules/mc_code/c_full_observables_production_spline.C', ['full_matching_loop'])

c_full_matching_loop = C.full_matching_loop


C.register_file('../../../python_modules/mc_code/c_log_likelihood.C', ['smart_log_likelihood'])
c_log_likelihood = C.smart_log_likelihood


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
			#print smart_log(smart_binomial(data, numMCEvents, probabiltyOfSuccess))
		else:
			#print data, mc, data*smart_log(mc) - mc - smart_stirling(data)
			#print data*smart_log(mc), mc, smart_stirling(data)
			#print data*smart_log(mc) - mc - smart_stirling(data)
			totalLogLikelihood += data*smart_log(mc) - mc - smart_stirling(data)
			#print data, mc, data*smart_log(mc), mc, smart_stirling(data), data*smart_log(mc) - mc - smart_stirling(data)

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
	def __init__(self, l_files, anodeVoltage, cathodeVoltage, degree_setting=-4, num_mc_events=1000000, name_notes=None, yields_free=False):

		# make class methods pickleable for multithreading process
		copy_reg.pickle(types.MethodType, reduce_method)
		
		# set number of mc events
		self.num_mc_events = int(num_mc_events)
		
		# get name notes
		self.name_notes = name_notes
		
		# set whether yields are free or not
		self.yields_free = yields_free
		
		# find mean field
		self.mean_field = d_cathode_voltage_to_field[cathodeVoltage]
	
		s1_branch = 'cpS1sTotBottom[0]'
		s2_branch = 'cpS2sTotBottom[0]'
		
		self.s1_settings = (40, 0, 30)
		self.s2_settings = (40, 0, 3000)
		self.log_settings = (40, 1, 3.5)

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
		#current_analysis.add_xs1asym_cut(any_peak=True)
		current_analysis.add_xs2asym_cut()
		current_analysis.add_cut('%s < %f' % (s1_branch, self.s1_settings[2]))
		current_analysis.add_cut('%s < %f' % (s2_branch, self.s2_settings[2]))
		current_analysis.add_cut('log10((S1Tot + S2Tot)/(AreaTot - S1Tot - S2Tot)) > 0.')
		current_analysis.add_cut('NbS1Peaks == 1')

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

		

		h_s1_s2 = Hist2D(*(self.s1_settings+self.s2_settings))
		current_analysis.Draw('%s:%s' % (s1_branch, s2_branch), hist=h_s1_s2, selection=current_analysis.get_cuts())
		
		#self.a_s1_s2 = neriX_analysis.convert_2D_hist_to_matrix(h_s1_s2)
		
		self.a_s1_bin_edges = np.linspace(self.s1_settings[1], self.s1_settings[2], num=self.s1_settings[0]+1, dtype=np.float32)
		self.a_s2_bin_edges = np.linspace(self.s2_settings[1], self.s2_settings[2], num=self.s2_settings[0]+1, dtype=np.float32)
		self.a_log_bin_edges = np.linspace(self.log_settings[1], self.log_settings[2], num=self.log_settings[0]+1, dtype=np.float32)
		
		
		# -----------------------------------------------
		# -----------------------------------------------
		# read in detector parameters from file
		# -----------------------------------------------
		# -----------------------------------------------
		
		neriX_analysis.warning_message('Detector parameters hard-coded')
		
		self.l_means_g1_g2 = [0.12961637894547148, 20.88715549107226]
		self.l_cov_matrix_g1_g2 = [[3.6039167387306653e-06, 0.00057442202683395834], [0.00057442202683395834, 0.093143781598001851]]
		
		self.g1_value = 0.129
		self.g1_uncertainty = 0.008
		
		self.g2_value = 20.9
		self.g2_uncertainty = 0.5
		
		self.gas_gain_value = 21.85
		self.gas_gain_uncertainty = 0.3
		
		self.gas_gain_width = 9.2
		self.gas_gain_width_uncertainty = 0.3
		
		self.spe_res_value = 0.66
		self.spe_res_uncertainty = 0.2
		
		self.l_means_pf_stdev_pars = [0.014673497158526188, 0.5284272187436192, 4.3229124008196411]
		self.l_cov_matrix_pf_stdev_pars = [[9.4927705182690691e-10, 2.352933249729148e-08, -2.4920029049639913e-07], [2.352933249729148e-08, 4.8381636138100317e-06, -1.7993529191388666e-05], [-2.4920029049639913e-07, -1.7993529191388666e-05, 0.00013121519103705991]]
		
		self.l_means_pf_eff_pars = [1.96178522, 0.46718076] #[7.95634366, 0.59582331]
		self.l_cov_matrix_pf_eff_pars = [[7.58956686e-05, 9.78759376e-07], [9.78759376e-07, 2.79732862e-05]]
		
		# only for producing initial distribution
		# NOT FOR LIKELIHOOD
		self.l_means_s1_eff_pars = [3, 1.2] #[7.95634366, 0.59582331]
		self.l_cov_matrix_s1_eff_pars = [[.3**2, 0], [0, .1**2]]
		
		#self.l_means_s2_eff_pars = [2.58150e+02, 5.93622e+01] #[7.95634366, 0.59582331]
		#self.l_cov_matrix_s2_eff_pars = [[ 274.4, -82.64], [-82.64, 218.2]]
		
		# only for producing initial distribution
		# NOT FOR LIKELIHOOD
		self.l_means_s2_eff_pars = [107.2, 321.1] #[7.95634366, 0.59582331]
		self.l_cov_matrix_s2_eff_pars = [[ 1274.4, -733.1], [-733.1, 991.2]]
		
		
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
		
		self.s1_fit_min_lower = 5.
		percent_from_median = 45.
		
		c_nr_band = Canvas(name='c_nr_band', title='c_nr_band', width=1600, height=500)
		c_nr_band.Divide(3, 1)

		c_nr_band.cd(1)
		
		h_s1_s2_band_fit = Hist2D(self.s1_settings[0], self.s1_settings[1], self.s1_settings[2], 200, self.log_settings[1], self.log_settings[2], name='h_s1_s2_band_fit', title='Log10(S2/S1) vs S1 - ' + current_analysis.get_filename_no_ext())
		current_analysis.Draw('%s:log10(%s/%s)' % (s1_branch, s2_branch, s1_branch), hist=h_s1_s2_band_fit, selection=current_analysis.get_cuts())
		h_s1_s2_band_fit.GetXaxis().SetTitle('Log(%s/%s)' % (s2_branch, s1_branch))
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
		g_lower_bounds.GetYaxis().SetRangeUser(self.log_settings[1], self.log_settings[2])
		g_lower_bounds.Draw('ap')
		
		
		c_nr_band.cd(2).SetGridx()
		c_nr_band.cd(2).SetGridy()


		c_nr_band.cd(3)

		g_upper_bounds = root.TGraph(num_points, x_values, y_upper_bound)
		g_upper_bounds.SetMarkerStyle(8)
		g_upper_bounds.SetTitle('Upper Bounds @ %.2f%% - %s' % (50 + percent_from_median, current_analysis.get_filename_no_ext()))
		g_upper_bounds.GetXaxis().SetRangeUser(self.s1_settings[1], self.s1_settings[2])
		g_upper_bounds.GetYaxis().SetRangeUser(self.log_settings[1], self.log_settings[2])
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
		f_upper_bound = root.TF1('f_upper_bound', '[0]*exp(-x/[1])+[2]', self.s1_fit_min_lower, self.s1_settings[2])
		f_upper_bound.SetParameters(2, 5, 2)
		
		fit_result_upper = g_upper_bounds.Fit('f_upper_bound', 'MELRS')
		f_upper_bound.SetLineStyle(1)
		f_upper_bound.SetLineWidth(2)
		f_upper_bound.SetLineColor(root.kRed)
		
		c_nr_band.cd(1)
		f_upper_bound.Draw('same')
		c_nr_band.cd(3)
		f_upper_bound.Draw('same')

		# draw upper bound
		f_upper_bound_draw = root.TF1('f_upper_bound_draw', '[0]*exp(-x/[1])+[2]', self.s1_settings[1], self.s1_settings[2])
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
		self.a_lower_bound_pars_nr_band = np.asarray([f_lower_bound.GetParameter(0), f_lower_bound.GetParameter(1)], dtype=np.float32)
		s_lower_bound_nr_cut = '(log10(%s/%s) > (%f + %f*%s))' % (s2_branch, s1_branch, f_lower_bound.GetParameter(0), f_lower_bound.GetParameter(1), s1_branch)
		
		# save fit values to an array
		self.a_upper_bound_pars_nr_band = np.asarray([f_upper_bound.GetParameter(0), f_upper_bound.GetParameter(1), f_upper_bound.GetParameter(2)], dtype=np.float32)

		# upper_bound_curve = [par0, par1, par2]
		# upper bound cut is used regardless of S1
		self.l_upper_bound_curve = [f_upper_bound.GetParameter(0), f_upper_bound.GetParameter(1), f_upper_bound.GetParameter(2)]
		s_upper_bound_nr_cut = '(log10(%s/%s) < (%f*exp(-%s/%f) + %f))' % (s2_branch, s1_branch, f_upper_bound.GetParameter(0), s1_branch, f_upper_bound.GetParameter(1), f_upper_bound.GetParameter(2))
		
		#raw_input('\n\ndebug\n')
		
		"""
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
		"""
		
		#raw_input('\nFits okay?\n')
		
		#print current_analysis.get_cuts() + '&& %s && %s' % (s_lower_bound_nr_cut, s_upper_bound_nr_cut)
		h_s1_s2_band_cut = Hist2D(*(self.s1_settings+self.log_settings))
		current_analysis.Draw('%s:log10(%s/%s)' % (s1_branch, s2_branch, s1_branch), hist=h_s1_s2_band_cut, selection=(current_analysis.get_cuts()))
		#current_analysis.Draw('%s:%s' % (s1_branch, s2_branch), hist=h_s1_s2_band_cut, selection=(current_analysis.get_cuts() + '&& %s && %s' % (s_lower_bound_nr_cut, s_upper_bound_nr_cut)))
		#current_analysis.Draw('%s:%s' % (s1_branch, s2_branch), hist=h_s1_s2_band_cut, selection=(current_analysis.get_cuts() + '&& %s' % (s_upper_bound_nr_cut)))
		
		self.a_s1_s2 = neriX_analysis.convert_2D_hist_to_matrix(h_s1_s2_band_cut, dtype=np.float32)
		self.num_data_points = np.sum(self.a_s1_s2)
		
		#raw_input('Press enter line 612')
		
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


# do linear interpolation for light and charge yield (works
# for both cpu and gpu and decent approximation)
# need to feed exciton to ion ratio parameters into
# observables code because it depends on the energy


	def likelihood_nr_band_no_nest(self, py_0, py_1, py_2, py_3, py_4, py_5, py_6, py_7, qy_0, qy_1, qy_2, qy_3, qy_4, qy_5, qy_6, qy_7, intrinsic_res_s1, intrinsic_res_s2, g1_value, spe_res_rv, g2_value, gas_gain_rv, gas_gain_width_rv, pf_eff_par0, pf_eff_par1, s1_eff_par0, s1_eff_par1, s2_eff_par0, s2_eff_par1, pf_stdev_par0, pf_stdev_par1, pf_stdev_par2, exciton_to_ion_par0_rv, exciton_to_ion_par1_rv, exciton_to_ion_par2_rv, scale_par, draw_fit=False, lowerQuantile=0.0, upperQuantile=1.0, gpu_compute=True):



		if gpu_compute:
			# need to do slightly fewer elements than
			# originally perscribed bc energies already
			# produced
			d_gpu_scale = {}
			d_gpu_scale['block'] = (1024,1,1)
			numBlocks = floor(self.num_mc_events / 1024.)
			d_gpu_scale['grid'] = (int(numBlocks), 1)
			num_mc_events = int(numBlocks*1024)


		# -----------------------------------------------
		# -----------------------------------------------
		# determine prior likelihood and variables
		# -----------------------------------------------
		# -----------------------------------------------


		prior_ln_likelihood = 0
		
		
		# priors of yields
		# can do them all together since uniform within
		# the same range
		prior_ln_likelihood += self.get_prior_log_likelihood_yields([py_0, py_1, py_2, py_3, py_4, py_5, py_6, py_7, qy_0, qy_1, qy_2, qy_3, qy_4, qy_5, qy_6, qy_7])
		
		
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


		# -----------------------------------------------
		# -----------------------------------------------
		# run MC
		# -----------------------------------------------
		# -----------------------------------------------

		# hard-code nuissance parameters
		#g1_rv, spe_res_rv, g2_rv, gas_gain_rv, gas_gain_width_rv, exciton_to_ion_par0_rv, exciton_to_ion_par1_rv, exciton_to_ion_par2_rv = 0, 0, 0, 0, 0, 0, 0, 0


		seed = np.asarray(int(time.time()*1000), dtype=np.int32)
		num_trials = np.asarray(num_mc_events, dtype=np.int32)
		mean_field = np.asarray(self.mean_field, dtype=np.float32)
		num_spline_points = np.asarray(len(self.a_spline_energies), dtype=np.int32)
		
		a_spline_energies = np.asarray(self.a_spline_energies, dtype=np.float32)
		a_spline_photon_yields = np.asarray([py_0, py_1, py_2, py_3, py_4, py_5, py_6, py_7], dtype=np.float32)
		#a_spline_photon_yields = np.asarray(self.a_nest_photon_yields, dtype=np.float32)
		a_spline_charge_yields = np.asarray([qy_0, qy_1, qy_2, qy_3, qy_4, qy_5, qy_6, qy_7], dtype=np.float32)
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
		num_bins_s1 = np.asarray(self.s1_settings[0], dtype=np.int32)
		gpu_bin_edges_s1 = pycuda.gpuarray.to_gpu(self.a_s1_bin_edges)
		num_bins_s2 = np.asarray(self.s2_settings[0], dtype=np.int32)
		gpu_bin_edges_s2 = pycuda.gpuarray.to_gpu(self.a_log_bin_edges)
		a_hist_2d = np.zeros(self.s1_settings[0]*self.log_settings[0], dtype=np.int32)
		
		#print a_spline_photon_yields
		#print self.a_nest_photon_yields
		#print a_spline_charge_yields
		#print self.a_nest_charge_yields
		
		#print s2_eff_par0, s2_eff_par1
		
		if gpu_compute:
		
			try:
				self.gpu_aEnergy
			except:
				self.gpu_aEnergy = pycuda.gpuarray.to_gpu(self.a_energy)
			
			
			# for histogram
			tArgs = (drv.In(seed), drv.In(num_trials), drv.In(mean_field), self.gpu_aEnergy, drv.In(num_spline_points), drv.In(a_spline_energies), drv.In(a_spline_photon_yields), drv.In(a_spline_charge_yields), drv.In(g1_value), drv.In(extraction_efficiency), drv.In(gas_gain_value), drv.In(gas_gain_width), drv.In(spe_res), drv.In(intrinsic_res_s1), drv.In(intrinsic_res_s2), drv.In(a_pf_stdev), drv.In(exciton_to_ion_par0_rv), drv.In(exciton_to_ion_par1_rv), drv.In(exciton_to_ion_par2_rv), drv.In(pf_eff_par0), drv.In(pf_eff_par1), drv.In(s1_eff_par0), drv.In(s1_eff_par1), drv.In(s2_eff_par0), drv.In(s2_eff_par1), drv.In(a_band_cut), drv.In(num_bins_s1), gpu_bin_edges_s1, drv.In(num_bins_s2), gpu_bin_edges_s2, drv.InOut(a_hist_2d))
			
			gpu_observables_func(*tArgs, **d_gpu_scale)
			#print a_hist_2d[:4]

			a_s1_s2_mc = np.reshape(a_hist_2d, (self.s1_settings[0], self.log_settings[0])).T
			
		
		else:
			# run the actual MC loop
			c_full_matching_loop(seed, num_trials, mean_field, aS1, aS2, self.a_energy, num_spline_points, a_spline_energies, a_spline_photon_yields, a_spline_charge_yields, g1_value, spe_res, extraction_efficiency, gas_gain_value, gas_gain_width, intrinsic_res_s1, intrinsic_res_s2, exciton_to_ion_par0_rv, exciton_to_ion_par1_rv, exciton_to_ion_par2_rv, pf_eff_par0, pf_eff_par1, s1_eff_par0, s1_eff_par1, s2_eff_par0, s2_eff_par1)
		
			a_s1_s2_mc, xEdges, yEdges = np.histogram2d(aS1, aS2, bins=[self.a_s1_bin_edges, self.a_s2_bin_edges])




		# -----------------------------------------------
		# -----------------------------------------------
		# Bin MC results and if requested draw
		# comparison of data and MC
		# -----------------------------------------------
		# -----------------------------------------------

		#sum_mc = np.sum(a_s1_s2_mc, dtype=np.float32)
		#if sum_mc == 0:
		#	return -np.inf

		#a_s1_s2_mc = np.multiply(a_s1_s2_mc, np.sum(self.a_s1_s2, dtype=np.float32) / sum_mc)
		a_s1_s2_mc = np.multiply(a_s1_s2_mc, float(scale_par)*self.num_data_points/float(self.num_mc_events))

		if draw_fit:

			f, (ax1, ax2) = plt.subplots(2, sharex=True, sharey=True)
			
			s1_s2_data_plot = np.rot90(self.a_s1_s2)
			s1_s2_data_plot = np.flipud(s1_s2_data_plot)
			ax1.pcolormesh(self.a_s1_bin_edges, self.a_log_bin_edges, s1_s2_data_plot)
			
			s1_s2_mc_plot = np.rot90(a_s1_s2_mc)
			s1_s2_mc_plot = np.flipud(s1_s2_mc_plot)
			ax2.pcolormesh(self.a_s1_bin_edges, self.a_log_bin_edges, s1_s2_mc_plot)
			#plt.colorbar()
			
			
			c1 = Canvas(1400, 400)
			c1.Divide(2)
			
			h_s1_data = Hist(*self.s1_settings, name='hS1_draw_data')
			root_numpy.array2hist(self.a_s1_s2.sum(axis=1), h_s1_data)
			
			hS1MC = Hist(*self.s1_settings, name='hS1_draw_mc')
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
			g_s1_data.GetXaxis().SetRangeUser(self.s1_settings[1], self.s1_settings[2])
			g_s1_data.GetYaxis().SetRangeUser(0, 1.2*max(h_s1_data.GetMaximum(), hS1MC.GetMaximum()))
			
			c1.cd(1)
			g_s1_data.Draw('ap')
			g_s1_mc.Draw('same')
			g_s1_mc_band = g_s1_mc.Clone()
			g_s1_mc_band.Draw('3 same')
			
			h_s2_data = Hist(*self.s2_settings, name='h_s2_draw_data')
			root_numpy.array2hist(self.a_s1_s2.sum(axis=0), h_s2_data)
			
			h_s2_mc = Hist(*self.s2_settings, name='h_s2_draw_mc')
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
			g_s2_data.GetXaxis().SetRangeUser(self.s2_settings[1], self.s2_settings[2])
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
			logLikelihoodMatching = smart_log_likelihood_quantiles(self.aS1S2, aS1S2MC, num_mc_elements, aS1BinCenters, aS2BinCenters, lowerQuantileS1, upperQuantileS1, lowerQuantileS2, upperQuantileS2)
		else:
			#print self.a_s1_s2
			#print a_s1_s2_mc
			#flatS1S2Data = self.a_s1_s2.flatten()
			#flatS1S2MC = a_s1_s2_mc.flatten()
			flatS1S2Data = np.asarray(self.a_s1_s2.flatten(), dtype=np.float32)
			flatS1S2MC = np.asarray(a_s1_s2_mc.flatten(), dtype=np.float32)
			#logLikelihoodMatching = smart_log_likelihood(flatS1S2Data, flatS1S2MC, self.num_mc_events)
			logLikelihoodMatching = c_log_likelihood(flatS1S2Data, flatS1S2MC, len(flatS1S2Data), self.num_mc_events, 0.95)
			#print logLikelihoodMatching

		total_ln_likelihood = logLikelihoodMatching + prior_ln_likelihood
		#print total_ln_likelihood

		if np.isnan(total_ln_likelihood):
			return -np.inf
		else:
			return total_ln_likelihood
			
			
			
	def wrapper_nr_band_no_nest(self, a_parameters, kwargs={}):
		#print a_parameters
		return self.likelihood_nr_band_no_nest(*a_parameters, **kwargs)
	
	
	
	def wrapper_nr_band_for_minimizer_fixed_nuissance(self, a_parameters, kwargs={}):
		#print a_parameters
		
		g1_value, g2_value = self.l_means_g1_g2
		gas_gain_rv = 0
		gas_gain_width_rv = 0
		spe_res_rv = 0
		pf_eff_par0, pf_eff_par1 = self.l_means_pf_eff_pars
		s2_eff_par0, s2_eff_par1 = self.l_means_s2_eff_pars
		pf_stdev_par0, pf_stdev_par1, pf_stdev_par2 = self.l_means_pf_stdev_pars
		exciton_to_ion_par0_rv, exciton_to_ion_par1_rv, exciton_to_ion_par2_rv = 0, 0, 0
		
		"""
		# since all measurements and models find py increases in this
		# range and qy strictly decreases in this range check that
		# this is the case
		if not (a_parameters[0] < a_parameters[1] < a_parameters[2] < a_parameters[3] < a_parameters[4] < a_parameters[5] < a_parameters[6] < a_parameters[7]):
			return -np.inf
		if not (a_parameters[0+8] > a_parameters[1+8] > a_parameters[2+8] > a_parameters[3+8] > a_parameters[4+8] > a_parameters[5+8] > a_parameters[6+8] > a_parameters[7+8]):
			return -np.inf
		"""
		
		
		# py_0, py_1, py_2, py_3, py_4, py_5, py_6, py_7, qy_0, qy_1, qy_2, qy_3, qy_4, qy_5, qy_6, qy_7, intrinsic_res_s1, intrinsic_res_s2, g1_value, spe_res_rv, g2_value, gas_gain_rv, gas_gain_width_rv, pf_eff_par0, pf_eff_par1, s1_eff_par0, s1_eff_par1, s2_eff_par0, s2_eff_par1, pf_stdev_par0, pf_stdev_par1, pf_stdev_par2, exciton_to_ion_par0_rv, exciton_to_ion_par1_rv, exciton_to_ion_par2_rv, scale_par
		
		# yields: 0-15
		# S1 res: 16
		# S2 res: 17
		# S1 efficiency: 18, 19
		# S2 efficiency: 20, 21
		# scale: 22
		
		return self.likelihood_nr_band_no_nest(py_0=a_parameters[0], py_1=a_parameters[1], py_2=a_parameters[2], py_3=a_parameters[3], py_4=a_parameters[4], py_5=a_parameters[5], py_6=a_parameters[6], py_7=a_parameters[7], qy_0=a_parameters[8], qy_1=a_parameters[9], qy_2=a_parameters[10], qy_3=a_parameters[11], qy_4=a_parameters[12], qy_5=a_parameters[13], qy_6=a_parameters[14], qy_7=a_parameters[15], intrinsic_res_s1=a_parameters[16], intrinsic_res_s2=a_parameters[17], g1_value=g1_value, spe_res_rv=spe_res_rv, g2_value=g2_value, gas_gain_rv=gas_gain_rv, gas_gain_width_rv=gas_gain_width_rv, pf_eff_par0=pf_eff_par0, pf_eff_par1=pf_eff_par1, s1_eff_par0=a_parameters[18], s1_eff_par1=a_parameters[19], s2_eff_par0=s2_eff_par0, s2_eff_par1=s2_eff_par1, pf_stdev_par0=pf_stdev_par0, pf_stdev_par1=pf_stdev_par1, pf_stdev_par2=pf_stdev_par2, exciton_to_ion_par0_rv=exciton_to_ion_par0_rv, exciton_to_ion_par1_rv=exciton_to_ion_par1_rv, exciton_to_ion_par2_rv=exciton_to_ion_par2_rv, scale_par=a_parameters[20], **kwargs)



	def wrapper_nr_band_force_yields(self, a_parameters, a_forced_light_yields, a_forced_charge_yields, kwargs={}):
		#print a_parameters
		assert len(a_parameters) == 14 and len(a_forced_light_yields) == 8 and len(a_forced_charge_yields) == 8
		return self.likelihood_nr_band_no_nest(*(np.concatenate([a_forced_light_yields, a_forced_charge_yields, a_parameters])), **kwargs)
	
	
	
	def likelihood_s2_efficiency(self, scale_parameter, s2_eff_par0, s2_eff_par1, deviation_from_nest, parameter_to_adjust=None, parameter_adjustment=0., draw_fit=False):



		# need to do slightly fewer elements than
		# originally perscribed bc energies already
		# produced
		d_gpu_scale = {}
		d_gpu_scale['block'] = (1024,1,1)
		numBlocks = floor(self.num_mc_events / 1024.)
		d_gpu_scale['grid'] = (int(numBlocks), 1)
		num_mc_events = int(numBlocks*1024)


		# -----------------------------------------------
		# -----------------------------------------------
		# set variables to means
		# -----------------------------------------------
		# -----------------------------------------------


		g1_value, g2_value = self.l_means_g1_g2
		
		gas_gain_value = self.gas_gain_value
		
		gas_gain_width = self.gas_gain_width
		
		spe_res_value = self.spe_res_value
		
		s1_eff_par0, s1_eff_par1 = self.l_means_s1_eff_pars
		
		extraction_efficiency = g2_value / gas_gain_value
		
		exciton_to_ion_par0_rv, exciton_to_ion_par1_rv, exciton_to_ion_par2_rv = 0, 0, 0
		
		intrinsic_res_s1 = 0.05
		intrinsic_res_s2 = 0.34



		# -----------------------------------------------
		# -----------------------------------------------
		# run MC
		# -----------------------------------------------
		# -----------------------------------------------

		# hard-code nuissance parameters
		#g1_rv, spe_res_rv, g2_rv, gas_gain_rv, gas_gain_width_rv, exciton_to_ion_par0_rv, exciton_to_ion_par1_rv, exciton_to_ion_par2_rv = 0, 0, 0, 0, 0, 0, 0, 0


		seed = np.asarray(int(time.time()*1000), dtype=np.int32)
		num_trials = np.asarray(num_mc_events, dtype=np.int32)
		mean_field = np.asarray(self.mean_field, dtype=np.float32)
		num_spline_points = np.asarray(len(self.a_spline_energies), dtype=np.int32)
		
		a_spline_energies = np.asarray(self.a_spline_energies, dtype=np.float32)
		a_spline_photon_yields = np.asarray(self.a_nest_photon_yields*(1.+deviation_from_nest), dtype=np.float32)
		a_spline_charge_yields = np.asarray(self.a_nest_charge_yields*(1.+deviation_from_nest), dtype=np.float32)
		
		g1_value = np.asarray(g1_value, dtype=np.float32)
		extraction_efficiency = np.asarray(extraction_efficiency, dtype=np.float32)
		spe_res = np.asarray(spe_res_value, dtype=np.float32)
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
		
		a_band_cut = np.concatenate([self.a_lower_bound_pars_nr_band, self.a_upper_bound_pars_nr_band])
		
		# for histogram binning
		num_bins_s1 = np.asarray(self.s1_settings[0], dtype=np.int32)
		gpu_bin_edges_s1 = pycuda.gpuarray.to_gpu(self.a_s1_bin_edges)
		num_bins_s2 = np.asarray(self.s2_settings[0], dtype=np.int32)
		gpu_bin_edges_s2 = pycuda.gpuarray.to_gpu(self.a_log_bin_edges)
		a_hist_2d = np.zeros(self.s1_settings[0]*self.log_settings[0], dtype=np.int32)
		
		#print a_spline_photon_yields
		#print self.a_nest_photon_yields
		#print a_spline_charge_yields
		#print self.a_nest_charge_yields
		
		#print s2_eff_par0, s2_eff_par1
		
	
		try:
			self.gpu_aEnergy
		except:
			self.gpu_aEnergy = pycuda.gpuarray.to_gpu(self.a_energy)
		
		
		# for histogram
		tArgs = (drv.In(seed), drv.In(num_trials), drv.In(mean_field), self.gpu_aEnergy, drv.In(num_spline_points), drv.In(a_spline_energies), drv.In(a_spline_photon_yields), drv.In(a_spline_charge_yields), drv.In(g1_value), drv.In(extraction_efficiency), drv.In(gas_gain_value), drv.In(gas_gain_width), drv.In(spe_res), drv.In(intrinsic_res_s1), drv.In(intrinsic_res_s2), drv.In(exciton_to_ion_par0_rv), drv.In(exciton_to_ion_par1_rv), drv.In(exciton_to_ion_par2_rv), drv.In(s1_eff_par0), drv.In(s1_eff_par1), drv.In(s2_eff_par0), drv.In(s2_eff_par1), drv.In(a_band_cut), drv.In(num_bins_s1), gpu_bin_edges_s1, drv.In(num_bins_s2), gpu_bin_edges_s2, drv.InOut(a_hist_2d))
		
		gpu_observables_func(*tArgs, **d_gpu_scale)
		#print a_hist_2d[:4]

		a_s1_s2_mc = np.reshape(a_hist_2d, (self.s1_settings[0], self.s2_settings[0])).T
		


		# -----------------------------------------------
		# -----------------------------------------------
		# Bin MC results and if requested draw
		# comparison of data and MC
		# -----------------------------------------------
		# -----------------------------------------------


		sum_mc = np.sum(a_s1_s2_mc, dtype=np.float32)
		if sum_mc == 0 or scale_parameter <= 0:
			return -np.inf

		a_s1_s2_mc = np.multiply(a_s1_s2_mc, 1./scale_parameter)
		

		if draw_fit:

			f, (ax1, ax2) = plt.subplots(2, sharex=True, sharey=True)
			
			s1_s2_data_plot = np.rot90(self.a_s1_s2)
			s1_s2_data_plot = np.flipud(s1_s2_data_plot)
			ax1.pcolormesh(self.a_s1_bin_edges, self.a_log_bin_edges, s1_s2_data_plot)
			
			s1_s2_mc_plot = np.rot90(a_s1_s2_mc)
			s1_s2_mc_plot = np.flipud(s1_s2_mc_plot)
			ax2.pcolormesh(self.a_s1_bin_edges, self.a_log_bin_edges, s1_s2_mc_plot)
			#plt.colorbar()
			
			
			c1 = Canvas(1400, 400)
			c1.Divide(2)
			
			h_s1_data = Hist(*self.s1_settings, name='hS1_draw_data')
			root_numpy.array2hist(self.a_s1_s2.sum(axis=1), h_s1_data)
			
			hS1MC = Hist(*self.s1_settings, name='hS1_draw_mc')
			root_numpy.array2hist(a_s1_s2_mc.sum(axis=1), hS1MC)
			
			s1_scale_factor = h_s1_data.Integral() / float(hS1MC.Integral())
			
			g_s1_data = neriX_analysis.convert_hist_to_graph_with_poisson_errors(h_s1_data)
			#g_s1_mc = neriX_analysis.convert_hist_to_graph_with_poisson_errors(hS1MC, scale=s1_scale_factor)
			g_s1_mc = neriX_analysis.convert_hist_to_graph_with_poisson_errors(hS1MC, scale=1.)
			
			g_s1_mc.SetFillColor(root.kBlue)
			g_s1_mc.SetMarkerColor(root.kBlue)
			g_s1_mc.SetLineColor(root.kBlue)
			g_s1_mc.SetFillStyle(3005)
			
			g_s1_data.SetTitle('S1 Comparison')
			g_s1_data.GetXaxis().SetTitle('S1 [PE]')
			g_s1_data.GetYaxis().SetTitle('Counts')
			
			g_s1_data.SetLineColor(root.kRed)
			g_s1_data.SetMarkerSize(0)
			g_s1_data.GetXaxis().SetRangeUser(self.s1_settings[1], self.s1_settings[2])
			g_s1_data.GetYaxis().SetRangeUser(0, 1.2*max(h_s1_data.GetMaximum(), hS1MC.GetMaximum()))
			
			c1.cd(1)
			g_s1_data.Draw('ap')
			g_s1_mc.Draw('same')
			g_s1_mc_band = g_s1_mc.Clone()
			g_s1_mc_band.Draw('3 same')
			
			h_s2_data = Hist(*self.s2_settings, name='h_s2_draw_data')
			root_numpy.array2hist(self.a_s1_s2.sum(axis=0), h_s2_data)
			
			h_s2_mc = Hist(*self.s2_settings, name='h_s2_draw_mc')
			root_numpy.array2hist(a_s1_s2_mc.sum(axis=0), h_s2_mc)
			
			s2_scale_factor = h_s2_data.Integral() / float(h_s2_mc.Integral())
			
			g_s2_data = neriX_analysis.convert_hist_to_graph_with_poisson_errors(h_s2_data)
			#g_s2_mc = neriX_analysis.convert_hist_to_graph_with_poisson_errors(h_s2_mc, scale=s2_scale_factor)
			g_s2_mc = neriX_analysis.convert_hist_to_graph_with_poisson_errors(h_s2_mc, scale=1.)
			
			g_s2_mc.SetFillColor(root.kBlue)
			g_s2_mc.SetMarkerColor(root.kBlue)
			g_s2_mc.SetLineColor(root.kBlue)
			g_s2_mc.SetFillStyle(3005)
			
			g_s2_data.SetTitle('S2 Comparison')
			g_s2_data.GetXaxis().SetTitle('S2 [PE]')
			g_s2_data.GetYaxis().SetTitle('Counts')
			
			g_s2_data.SetLineColor(root.kRed)
			g_s2_data.SetMarkerSize(0)
			g_s2_data.GetXaxis().SetRangeUser(self.s2_settings[1], self.s2_settings[2])
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
		
		total_ln_likelihood = smart_log_likelihood(self.a_s1_s2.sum(axis=0), a_s1_s2_mc.sum(axis=0), num_mc_events)
		total_ln_likelihood = total_ln_likelihood[0]
		#print total_ln_likelihood
		
		if np.isnan(total_ln_likelihood):
			return -np.inf
		else:
			return total_ln_likelihood




	def wrapper_s2_trigger_efficiency_likeilihood(self, a_parameters, kwargs={}):
		#print a_parameters
		assert 'deviation_from_nest' in kwargs
		return self.likelihood_s2_efficiency(*a_parameters, **kwargs)

	



	# find initial positions
	# no NEST is a misnomer because we will use NEST to initialize
	# the location of the walkers
	def initial_positions_nr_band_no_nest(self, num_walkers):
		l_par_names = ['py_0', 'py_1', 'py_2', 'py_3', 'py_4', 'py_5', 'py_6', 'py_7', 'qy_0', 'qy_1', 'qy_2', 'qy_3', 'qy_4', 'qy_5', 'qy_6', 'qy_7', 'intrinsic_res_s1', 'intrinsic_res_s2', 'g1_value', 'spe_res_rv', 'g2_value', 'gas_gain_rv', 'gas_gain_width_rv', 'pf_eff_par0', 'pf_eff_par1', 's1_eff_par0', 's1_eff_par1', 's2_eff_par0', 's2_eff_par1', 'pf_stdev_par0', 'pf_stdev_par1', 'pf_stdev_par2', 'exciton_to_ion_par0_rv', 'exciton_to_ion_par1_rv', 'exciton_to_ion_par2_rv', 'scale_par']
		
		d_variable_arrays = {}
		
		# position array should be (num_walkers, num_dim)
	
		for par_name in l_par_names:
			# handle photon and charge yield initial positions
			if (par_name[0] == 'p' or par_name[0] == 'q') and par_name[1] == 'y':
				spline_number = int(par_name[-1])
				l_yields = nest_nr_mean_yields(self.a_spline_energies[spline_number], self.mean_field)
				if par_name[0] == 'p':
					yields_index = 0
				else:
					yields_index = 1
				
				d_variable_arrays[par_name] = np.random.normal(l_yields[yields_index], 0.2*l_yields[yields_index], size=num_walkers)

			elif par_name == 'intrinsic_res_s1':
				d_variable_arrays[par_name] = np.random.normal(.10, .04, size=num_walkers)
				
			elif par_name == 'intrinsic_res_s2':
				d_variable_arrays[par_name] = np.random.normal(.34, .04, size=num_walkers)
			
			# handle g1 and g2 with g1 only
			elif par_name == 'g1_value':
				#a_g1_g2_draws = np.random.multivariate_normal(self.l_means_g1_g2, self.l_cov_matrix_g1_g2, size=(2, num_walkers))
				d_variable_arrays['g1_value'], d_variable_arrays['g2_value'] = np.random.multivariate_normal(self.l_means_g1_g2, self.l_cov_matrix_g1_g2, size=num_walkers).T
				#print d_variable_arrays['g1_value']
				#print d_variable_arrays['g2_value']


			# catch both parameters of s1 efficiency prior
			elif par_name == 'pf_eff_par0':
				d_variable_arrays['pf_eff_par0'], d_variable_arrays['pf_eff_par1'] = np.random.multivariate_normal(self.l_means_pf_eff_pars, self.l_cov_matrix_pf_eff_pars, size=num_walkers).T
				
			
			elif par_name == 's1_eff_par0':
				d_variable_arrays['s1_eff_par0'], d_variable_arrays['s1_eff_par1'] = np.random.multivariate_normal(self.l_means_s1_eff_pars, self.l_cov_matrix_s1_eff_pars, size=num_walkers).T


			elif par_name == 's2_eff_par0':
				d_variable_arrays['s2_eff_par0'], d_variable_arrays['s2_eff_par1'] = np.random.multivariate_normal(self.l_means_s2_eff_pars, self.l_cov_matrix_s2_eff_pars, size=num_walkers).T
				
				
			elif par_name == 'pf_stdev_par0':
				d_variable_arrays['pf_stdev_par0'], d_variable_arrays['pf_stdev_par1'], d_variable_arrays['pf_stdev_par2'] = np.random.multivariate_normal(self.l_means_pf_stdev_pars, self.l_cov_matrix_pf_stdev_pars, size=num_walkers).T
				
				
			elif par_name == 'scale_par':
				d_variable_arrays[par_name] = np.random.normal(self.num_mc_events/self.num_data_points/2.5, self.num_mc_events/self.num_data_points/2.5*0.1, size=num_walkers)
			
			
			# catch all normally distributed RVs
			else:
				if par_name == 'g2_value' or par_name == 'pf_eff_par1' or par_name == 's1_eff_par1' or par_name == 's2_eff_par1'  or par_name == 'pf_stdev_par1' or par_name == 'pf_stdev_par2':
					continue
				d_variable_arrays[par_name] = np.random.randn(num_walkers)
				
				
		l_arrays_for_stacking = []
		
		for par_name in l_par_names:
			l_arrays_for_stacking.append(d_variable_arrays[par_name])
			#print par_name
			#print d_variable_arrays[par_name]
		
		l_arrays_for_stacking = np.asarray(l_arrays_for_stacking).T
		#print l_arrays_for_stacking[5]

		return l_arrays_for_stacking
				
	


	# default is to do a yields fit
	def fit_nr_band_no_nest(self, num_steps=200, num_walkers=1000, num_threads=1, fractional_deviation_start_pos=0.01, efficiency_fit=False, deviation_from_nest=None):
	
		if efficiency_fit:
			assert deviation_from_nest != None, '\nMust give deviation from NEST values!\n'
	
		if not efficiency_fit:
			num_dim = 36
			print '\n\nFitting yields...\n\n'
		else:
			num_dim = 3
			print '\n\nFitting trigger efficiency...\n\n'
		
		#l_guesses = [0. for j in xrange(num_dim)]
		#for i, name in enumerate(l_par_names):
		#	l_guesses[i] = d_variable_guesses[name]
		
		# before emcee, setup save locations
		dir_specifier_name = '%.3fkV_%.1fkV' % (self.cathode_setting, self.anode_setting)
		self.results_directory_name = nr_band_config.results_directory_name
		if not efficiency_fit:
			self.path_for_save = '%s/yields_fit/%s/%s/' % (self.results_directory_name, dir_specifier_name, self.filename)
		else:
			#assert self.name_notes != None, 'name note must be used'
			self.path_for_save = '%s/efficiency_fit/%.2f_deviation_from_nest/%s/%s/' % (self.results_directory_name, deviation_from_nest, dir_specifier_name, self.filename)
		
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
			starting_pos = self.initial_positions_nr_band_no_nest(num_walkers)
			if efficiency_fit:
				#starting_pos = starting_pos[:, 16:]
				ratio_mc_data = self.num_mc_events/float(self.num_data_points)
				starting_pos = emcee.utils.sample_ball([ratio_mc_data, 75, 75], [0.2*ratio_mc_data, 5, 5], size=num_walkers)
				#print starting_pos
			
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
		if not efficiency_fit:
			sampler = emcee.EnsembleSampler(num_walkers, num_dim, self.wrapper_nr_band_no_nest, a=1.15, threads=num_threads, kwargs={})
		else:
			"""
			a_forced_light_yields = self.a_nest_photon_yields*(1.+deviation_from_nest)
			a_forced_charge_yields = self.a_nest_charge_yields*(1.+deviation_from_nest)
			
			sampler = emcee.EnsembleSampler(num_walkers, num_dim, self.wrapper_nr_band_force_yields, a=2., threads=num_threads, kwargs={'a_forced_light_yields':a_forced_light_yields, 'a_forced_charge_yields':a_forced_charge_yields})
			"""

			sampler = emcee.EnsembleSampler(num_walkers, num_dim, self.wrapper_s2_trigger_efficiency_likeilihood, a=2., threads=num_threads, kwargs={'kwargs':{'deviation_from_nest':deviation_from_nest}})
		


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



	def minimize_nll_free_pars(self, a_guesses):
		def neg_log_likelihood(a_guesses):
			return -self.wrapper_nr_band_no_nest(a_guesses, {'gpu_compute':True})
		result = optimize.minimize(neg_log_likelihood, a_guesses, method='Nelder-Mead', options={'disp':True})
		print result



	def differential_evolution_minimizer_free_pars(self, a_bounds, maxiter=250, tol=0.05, popsize=15):
		def neg_log_likelihood_diff_ev(a_guesses):
			return -self.wrapper_nr_band_for_minimizer_fixed_nuissance(a_guesses, {'gpu_compute':True})
		print '\n\nStarting differential evolution minimizer...\n\n'
		result = optimize.differential_evolution(neg_log_likelihood_diff_ev, a_bounds, disp=True, maxiter=maxiter, tol=tol, popsize=popsize)
		print result




if __name__ == '__main__':
	test = nr_band_fitter('nerix_160419_1331', 4.5, 0.345, num_mc_events=int(1e6))

	a_free_par_guesses = [0.95, 5.49, 7.73, 8.21, 9.08, 10.04, 10.81, 10.54, 11.37, 7.31, 5.99, 6.31, 5.88, 5.46, 5.71, 4.51, 0.04, 0.33, 0.13, -1.84, 20.80, 0.08, -0.15, 1.96, 0.46, 200, 200, 0.09, -0.20, -0.22]
	#print len(a_free_par_guesses)
	#test.minimize_nll_free_pars(a_free_par_guesses)
	
	# enerigies: [0.5, 2.96, 4.93, 6.61, 9.76, 13.88, 17.5, 25]
	# py_nest: [1.03, 4.41, 5.80, 6.60, 7.64, 8.57, 9.19, 10.15]
	# qy_nest: [7.69, 6.67, 6.06, 5.72, 5.30, 4.93, 4.68, 4.25]
	
	# test differential evolution minimizer
	a_free_par_bounds = [(0.5, 2.), (2.5, 6.5), (4.5, 9), (5, 10), (5.5, 12), (5.5, 13), (5.5, 13), (6, 14),
						(4, 11), (3.5, 10.5), (3, 10), (2.5, 9.5), (2.5, 9.5), (2, 9), (2, 9), (1.5, 8),
						(0.01, 0.5), (0.01, 0.5), (-5, 5), (0.1, 10), (1, 10)]
	test.differential_evolution_minimizer_free_pars(a_free_par_bounds, maxiter=150, popsize=50, tol=0.01)
	
	# py_0, py_1, py_2, py_3, py_4, py_5, py_6, py_7, qy_0, qy_1, qy_2, qy_3, qy_4, qy_5, qy_6, qy_7, intrinsic_res_s1, intrinsic_res_s2, g1_value, spe_res_rv, g2_value, gas_gain_rv, gas_gain_width_rv, pf_eff_par0, pf_eff_par1, s1_eff_par0, s1_eff_par1, s2_eff_par0, s2_eff_par1, pf_stdev_par0, pf_stdev_par1, pf_stdev_par2, exciton_to_ion_par0_rv, exciton_to_ion_par1_rv, exciton_to_ion_par2_rv, scale_par
	#test.likelihood_nr_band_no_nest(py_0=1.88, py_1=6.57, py_2=6.01, py_3=5.06, py_4=6.49, py_5=6.43, py_6=12.17, py_7=12.39, qy_0=6.88, qy_1=5.20, qy_2=4.79, qy_3=4.64, qy_4=4.77, qy_5=4.90, qy_6=5.09, qy_7=4.93, intrinsic_res_s1=0.46, intrinsic_res_s2=0.02, g1_value=0.13, spe_res_rv=0., g2_value=20.89, gas_gain_rv=0, gas_gain_width_rv=0., pf_eff_par0=test.l_means_pf_eff_pars[0], pf_eff_par1=test.l_means_pf_eff_pars[1], s1_eff_par0=3.46, s1_eff_par1=5.4, s2_eff_par0=test.l_means_s2_eff_pars[0], s2_eff_par1=test.l_means_s2_eff_pars[1], pf_stdev_par0=test.l_means_pf_stdev_pars[0], pf_stdev_par1=test.l_means_pf_stdev_pars[1], pf_stdev_par2=test.l_means_pf_stdev_pars[2], exciton_to_ion_par0_rv=0., exciton_to_ion_par1_rv=0., exciton_to_ion_par2_rv=0., scale_par=3.75, draw_fit=True, lowerQuantile=0.0, upperQuantile=1.0, gpu_compute=True)
	#test.likelihood_nr_band_no_nest(py_0=1.56, py_1=5.50, py_2=7.22, py_3=7.40, py_4=6.56, py_5=8.40, py_6=9.02, py_7=11.7, qy_0=7.46, qy_1=4.27, qy_2=9.166, qy_3=2.54, qy_4=5.35, qy_5=4.45, qy_6=5.25, qy_7=4.16, intrinsic_res_s1=0.22, intrinsic_res_s2=0.15, g1_value=0.13, spe_res_rv=0., g2_value=20.89, gas_gain_rv=0, gas_gain_width_rv=0., pf_eff_par0=test.l_means_pf_eff_pars[0], pf_eff_par1=test.l_means_pf_eff_pars[1], s1_eff_par0=3.46, s1_eff_par1=7.4, s2_eff_par0=test.l_means_s2_eff_pars[0], s2_eff_par1=test.l_means_s2_eff_pars[1], pf_stdev_par0=test.l_means_pf_stdev_pars[0], pf_stdev_par1=test.l_means_pf_stdev_pars[1], pf_stdev_par2=test.l_means_pf_stdev_pars[2], exciton_to_ion_par0_rv=0., exciton_to_ion_par1_rv=0., exciton_to_ion_par2_rv=0., scale_par= 3., draw_fit=True, lowerQuantile=0.0, upperQuantile=1.0, gpu_compute=True)
	

	# enerigies: [0.5, 2.96, 4.93, 6.61, 9.76, 13.88, 17.5, 25]
	# py_nest: [1.03, 4.41, 5.80, 6.60, 7.64, 8.57, 9.19, 10.15]
	# qy_nest: [7.69, 6.67, 6.06, 5.72, 5.30, 4.93, 4.68, 4.25]
	#test.likelihood_nr_band_no_nest(py_0=1.03, py_1=4.41, py_2=5.80, py_3=6.60, py_4=7.64, py_5=8.57, py_6=9.19, py_7=10.15, qy_0=7.69, qy_1=6.67, qy_2=6.06, qy_3=5.72, qy_4=5.30, qy_5=4.93, qy_6=4.68, qy_7=4.25, intrinsic_res_s1=0.1, intrinsic_res_s2=0.25, g1_value=0.13, spe_res_rv=0, g2_value=20.9, gas_gain_rv=0, gas_gain_width_rv=0, pf_eff_par0=1.961, pf_eff_par1=0.46, s1_eff_par0=1.1, s1_eff_par1=3.5, s2_eff_par0=test.l_means_s2_eff_pars[0], s2_eff_par1=test.l_means_s2_eff_pars[1], pf_stdev_par0=0.0146, pf_stdev_par1=0.528, pf_stdev_par2=4.32, exciton_to_ion_par0_rv=0, exciton_to_ion_par1_rv=0, exciton_to_ion_par2_rv=0, scale_par=test.num_mc_events/test.num_data_points/2.5, draw_fit=True, lowerQuantile=0.0, upperQuantile=1.0, gpu_compute=True)




