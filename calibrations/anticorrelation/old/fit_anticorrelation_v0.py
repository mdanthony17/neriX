#!/usr/bin/python
import pickle
#print pickle.Pickler.dispatch
import dill
#print pickle.Pickler.dispatch

import ROOT as root
import sys

import matplotlib
matplotlib.use('QT4Agg')
import matplotlib.pyplot as plt

import emcee, corner, click
import neriX_analysis, neriX_datasets
from rootpy.plotting import Hist2D, Hist, Legend, Canvas
from rootpy.tree import Tree
import numpy as np
import tqdm, time, copy_reg, types, pickle
from root_numpy import tree2array, array2tree
import scipy.optimize as op
import scipy.special
import pymc3 as pm

from pymc3 import log, exp
import theano
import inspect

#print pickle.Pickler.dispatch

"""
def _pickle_method(method):
	print method.__dict__
	func_name = method.im_func.__name__
	obj = method.im_self
	cls = method.im_class
	return _unpickle_method, (func_name, obj, cls)

def _unpickle_method(func_name, obj, cls):
	for cls in cls.mro():
		try:
			func = cls.__dict__[func_name]
		except KeyError:
			pass
		else:
			break
	return func.__get__(obj, cls)

copy_reg.pickle(types.FunctionType, _pickle_method, _unpickle_method)
"""

#print pickle.Pickler.dispatch

def save_function(self, obj):
    # Save functions by value if they are defined interactively
    if obj.__module__ == '__main__' or obj.func_name == '<lambda>':
        args = (obj.func_code, obj.func_globals, obj.func_name, obj.func_defaults, obj.func_closure)
        self.save_reduce(types.FunctionType, args, obj=obj)
    else:
        pickle.Pickler.save_global(self, obj)
pickle.Pickler.dispatch[types.FunctionType] = save_function



#print pickle.Pickler.dispatch

def fit_anticorrelation(d_files, batch_mode=False):
	
	
	l_acceptable_energies = ['cs137', 'na22']
	
	# will have to change the setting based on the source otherwise
	# neriX_analysis won't accept the files
	# will do this within the loop
	l_settings = [[-1,0.345,4.5], [-1,0.7,4.5], [-1,1.054,4.5], [-1,1.500,4.5], [-1,2.356,4.5]]
	
	# check that files given are of a supported energy
	
	for source in d_files:
		if source in l_acceptable_energies:
			continue
		else:
			print 'File of type %s not supported yet!' % (energy)
			sys.exit()

	hard_coded_correction_factor = 1.48e6 / 1.18e4 * (530./800.)**11.41
	neriX_analysis.warning_message('Using hard coded correction: %.2e' % hard_coded_correction_factor)

	s1_par = 'cpS1sTotBottom[0]/%f' % hard_coded_correction_factor
	s2_par = 'cpS2sTotBottom[0]/%f' % hard_coded_correction_factor

	#s1_par = 'cpS1sTotBottom[0]'
	#s2_par = 'cpS2sTotBottom[0]'

	column_names = ('S1', 'S2')


	# use dictionary to store all arrays, histograms,
	# fits, etc.

	d_energy_information = {}
	d_s1_s2_arrays = {}

	for source in d_files:

		if source == 'cs137':
			numBinsCES = 50
			upperBoundCES = 1200.
		elif source == 'na22':
			numBinsCES = 50
			upperBoundCES = 800.
		elif source == 'bkg':
			numBinsCES = 50
			upperBoundCES = 450.
		elif source == 'co57':
			numBinsCES = 50
			upperBoundCES = 250.
		
		# create entry in dictionary
		d_energy_information[source] = {}

		a_combined_tree = -1

		column_dtype = -1

		l_columns = [np.empty(0) for i in xrange(len(column_names))]
		
		l_files = d_files[source]
		
		# must change l_settings based on current source
		if source == 'cs137':
			correct_degree_setting = -1
			d_energy_information[source]['peaks'] = (662.)
		elif source == 'na22':
			correct_degree_setting = -5
			d_energy_information[source]['peaks'] = (511.)
		elif source == 'bkg':
			correct_degree_setting = -6
			d_energy_information[source]['peaks'] = (164., 236)
		elif source == 'co57':
			correct_degree_setting = -2
			d_energy_information[source]['peaks'] = (122.)

		for i in xrange(len(l_settings)):
			l_settings[i][0] = correct_degree_setting
		
		
		
		# transpose list of files such that
		# files of the same cathode voltage are together
		l_files = map(list, zip(*l_files))
		

		for cur_index, files_at_setting in enumerate(l_files):
			# if fitting a full run, "file" is actually a list of
			# files for a given cathode setting
			# name is from older version
			
			current_analysis = neriX_analysis.neriX_analysis(list(files_at_setting), *l_settings[cur_index])
			
			current_analysis.add_z_cut()
			current_analysis.add_single_scatter_cut()
			current_analysis.add_radius_cut(0, 0.85)
			
			run_number = current_analysis.get_run()
			
			current_analysis.set_event_list()
			
			for j in xrange(len(list(files_at_setting))):
				current_tree = tree2array(current_analysis.get_T1(j), [s1_par, s2_par], selection=current_analysis.get_cuts())
				current_tree.dtype.names = column_names
				column_dtype = current_tree.dtype
				for i, column_name in enumerate(current_tree.dtype.names):
					l_columns[i] = np.append(l_columns[i], current_tree[column_name])

			del current_analysis

		a_combined_tree = np.empty(len(l_columns[0]), dtype=column_dtype)
		for i, column_name in enumerate(column_names):
			a_combined_tree[column_name] = l_columns[i]

		d_energy_information[source]['combined_tree'] = array2tree(a_combined_tree, name='combined_tree_%s' % (source))


		# ---------- MAKE CES CUT ----------

		# no time correction: 0.08, 14.
		g1_test = 0.13 #0.12
		g2_test = 21. #28.
		sigmaCES = 2.0

		d_energy_information[source]['canvas'] = Canvas(width=900, height=700, name='cCES')
		d_energy_information[source]['canvas'].SetGridx()
		d_energy_information[source]['canvas'].SetGridy()

		d_energy_information[source]['hist'] = Hist(numBinsCES, 0, upperBoundCES, name='hCES_%s' % (source), title='hCES_%s' % (source), drawstyle='hist')
		d_energy_information[source]['combined_tree'].Draw('0.0137*(S1/%f + S2/%f)>>hCES_%s' % (g1_test, g2_test, source))

		d_energy_information[source]['hist'].Draw()
		d_energy_information[source]['canvas'].Update()

		d_energy_information[source]['lbCES'] = float(raw_input('\n\nPlease enter lower bound on combined energy scale: '))
		d_energy_information[source]['ubCES'] = float(raw_input('\n\nPlease enter upper bound on combined energy scale: '))

		#print '\n\nDEBUG MODE SKIPPING LIMIT PICKS\n\n'
		#d_energy_information[source]['lbCES'] = 400
		#d_energy_information[source]['ubCES'] = 700

		if source == 'cs137':
			d_energy_information[source]['func'] = root.TF1('fGausCES_%s' % (source), '[0]*( [1]/[2]*exp(-x/[2])/(exp(-%f/[2])-exp(-%f/[2])) + (1.-[1])/(2*3.141592)^0.5/[4]*exp(-0.5*pow((x-[3])/[4], 2)) )' % (d_energy_information[source]['lbCES'], d_energy_information[source]['ubCES']), d_energy_information[source]['lbCES'], d_energy_information[source]['ubCES'])
			d_energy_information[source]['func'].SetParameters(1000, 0.5, 150, 500, 662, 50)
			d_energy_information[source]['func'].SetParLimits(0, 1, 1e8)
			d_energy_information[source]['func'].SetParLimits(1, 0, 1)
			d_energy_information[source]['func'].SetParLimits(2, 50, 1000)
			d_energy_information[source]['func'].SetParLimits(3, d_energy_information[source]['lbCES'], d_energy_information[source]['ubCES'])
			d_energy_information[source]['func'].SetParLimits(4, 10, 100)
		elif source == 'na22':
			d_energy_information[source]['func'] = root.TF1('fGausCES_%s' % (source), '[0]*( [1]/[2]*exp(-x/[2])/(exp(-%f/[2])-exp(-%f/[2])) + (1.-[1])/(2*3.141592)^0.5/[4]*exp(-0.5*pow((x-[3])/[4], 2)) )' % (d_energy_information[source]['lbCES'], d_energy_information[source]['ubCES']), d_energy_information[source]['lbCES'], d_energy_information[source]['ubCES'])
			d_energy_information[source]['func'].SetParameters(1000, 0.7, 170, 511, 20)
			d_energy_information[source]['func'].SetParLimits(0, 1, 1e8)
			d_energy_information[source]['func'].SetParLimits(1, 0, 1)
			d_energy_information[source]['func'].SetParLimits(2, 10, 500)
			d_energy_information[source]['func'].SetParLimits(3, d_energy_information[source]['lbCES'], d_energy_information[source]['ubCES'])
			d_energy_information[source]['func'].SetParLimits(4, 10, 100)
		elif source == 'bkg':
			print 'Not ready!!'
			sys.exit()
			"""
			d_energy_information[source]['func'] = root.TF1('fGausCES_%s' % (source), '[0]*( [1]/[2]*exp(-x/[2]) + (1.-[1])/(2*3.141592)^0.5/[4]*exp(-0.5*pow((x-[3])/[4], 2)) )', d_energy_information[source]['lbCES'], d_energy_information[source]['ubCES'])
			fGausCES.SetParameters(1000, 0.5, 100, 500, 511, 50)
			fGausCES.SetParLimits(0, 1, 1e8)
			fGausCES.SetParLimits(1, 0, 1)
			fGausCES.SetParLimits(2, 10, 500)
			fGausCES.SetParLimits(3, d_energy_information[source]['lbCES'], d_energy_information[source]['ubCES'])
			fGausCES.SetParLimits(4, 10, 100)
			"""
		elif source == 'co57':
			print 'Not ready!!'
			sys.exit()
			"""
			d_energy_information[source]['func'] = root.TF1('fGausCES_%s' % (source), '[0]*( [1]/[2]*exp(-x/[2]) + (1.-[1])/(2*3.141592)^0.5/[4]*exp(-0.5*pow((x-[3])/[4], 2)) )', d_energy_information[source]['lbCES'], d_energy_information[source]['ubCES'])
			fGausCES.SetParameters(1000, 0.5, 100, 500, 511, 50)
			fGausCES.SetParLimits(0, 1, 1e8)
			fGausCES.SetParLimits(1, 0, 1)
			fGausCES.SetParLimits(2, 10, 500)
			fGausCES.SetParLimits(3, d_energy_information[source]['lbCES'], d_energy_information[source]['ubCES'])
			fGausCES.SetParLimits(4, 10, 100)
			"""


		
		d_energy_information[source]['hist'].Fit(d_energy_information[source]['func'], 'LR')
		d_energy_information[source]['func'].Draw('same')
		d_energy_information[source]['canvas'].Update()

		raw_input('\nFit okay?\n')


		# will fit background with gaussian
		# background can be either exponential or constant
		# exponential: Cs137, Na22
		# constant: Co57, bkg peaks
		sCES_arr = '(0.0137*(S1/%f + S2/%f)) > %f && (0.0137*(S1/%f + S2/%f)) < %f' % (g1_test, g2_test, d_energy_information[source]['lbCES'], g1_test, g2_test, d_energy_information[source]['ubCES'])
		#sCES_tree = '(0.0137*(%s/%f + %s/%f) > %f && 0.0137*(%s/%f + %s/%f) < %f)' % (s1_par, g1, s2_par, g2, lbCES, s1_par, g1, s2_par, g2, ubCES)
		#print '\n\nCES Cut: %s\n\n' % (sCES_arr)


		d_s1_s2_arrays[source] = tree2array(d_energy_information[source]['combined_tree'], ['S1', 'S2'], selection=sCES_arr)

		#print d_s1_s2_arrays[source]['S1'].mean()
		#print d_s1_s2_arrays[source]['S2'].mean()




		# below is for debugging Cs137
		"""
		f, ax = plt.subplots()
		def np_likelihood(energy, g1, g2, w_value, delta, gamma_low, gamma_high, tau, resolution, peak_energy):
			return (delta/tau*np.exp((500-energy)/tau) + (1-delta)/(2*3.141592)**0.5/(resolution*peak_energy)*np.exp(-0.5*(energy-peak_energy)**2/(resolution*peak_energy)**2))/(1+np.exp((500-energy)/gamma_low))/(1+np.exp((energy-900.)/gamma_high))
		
		
		energy = np.linspace(450, 1000)
		plt.plot(energy, np_likelihood(energy, 0.12, 27., 13.46, 0.505, 5.8, 1, 103, 0.044, peak_energy))
		plt.show()
		"""
		



	def ln_prior_g1_g2(g1, g2):
		if 0.09 < g1 < 0.15 and 17. < g2 < 33.:
			return 0.
		else:
			return -np.inf


	def ln_prior_w_normal(w_value):
		measured_value = 13.7
		measured_uncertainty = 0.2
		return np.log(1./(2*np.pi)**0.5/measured_uncertainty*np.exp(-0.5*(w_value-measured_value)**2/measured_uncertainty**2))



	def ln_prior_w_uniform(w_value):
		if 8. < w_value < 19.:
			return 0.
		else:
			return -np.inf


	# create dictionary for storing info about functions
	# for each source and required parameters
	d_source_functions = {}

	# ------------------------------------------
	# ------------------------------------------
	# Prior and likelihood functions
	# for Cs137
	# ------------------------------------------
	# ------------------------------------------

	def ln_prior_cs137(delta_cs137, low_edge_rolloff_cs137, high_edge_rolloff_cs137, exp_bkg_cs137, res_cs137, delta_estimate):
		if delta_estimate*0.9 < delta_cs137 < delta_estimate*1.1 and 0.01 < low_edge_rolloff_cs137 < 50 and 0.01 < high_edge_rolloff_cs137 < 50 and 100 < exp_bkg_cs137 < 200 and 0.005 < res_cs137 < 0.20:
			return 0.
		else:
			return -np.inf


	def ln_likelihood_cs137(energy, delta_cs137, low_edge_rolloff_cs137, high_edge_rolloff_cs137, exp_bkg_cs137, res_cs137, lbCES, ubCES):
	
		ratio_gaus_to_exp = (1-delta_cs137)/delta_cs137*exp_bkg_cs137*(1 - np.exp((lbCES-ubCES)/exp_bkg_cs137))/(2*3.141592)**0.5/(res_cs137*662.)*np.exp(-0.5*(energy-662.)**2/(res_cs137*662.)**2 - (lbCES-energy)/exp_bkg_cs137)
		
		return np.sum(np.log(delta_cs137/exp_bkg_cs137/(1 - np.exp((lbCES-ubCES)/exp_bkg_cs137))) + (lbCES-energy)/exp_bkg_cs137 + np.log1p(ratio_gaus_to_exp) - np.log1p(np.exp((lbCES-energy)/low_edge_rolloff_cs137)) - np.log1p(np.exp((energy-ubCES)/high_edge_rolloff_cs137)))
	
		#return np.sum(np.log((delta_cs137/exp_bkg_cs137*np.exp((lbCES-energy)/exp_bkg_cs137) + (1-delta_cs137)/(2*3.141592)**0.5/(res_cs137*662.)*np.exp(-0.5*(energy-662.)**2/(res_cs137*662.)**2))/(1+np.exp((lbCES-energy)/low_edge_rolloff_cs137))/(1+np.exp((energy-ubCES)/high_edge_rolloff_cs137))))

	if 'cs137' in d_files:
		d_source_functions['cs137'] = {}
		d_source_functions['cs137']['lbCES'] = d_energy_information[source]['lbCES']
		d_source_functions['cs137']['ubCES'] = d_energy_information[source]['ubCES']
		d_source_functions['cs137']['ln_prior'] = ln_prior_cs137
		d_source_functions['cs137']['ln_likelihood'] = ln_likelihood_cs137
		d_source_functions['cs137']['pars'] = [0. for i in xrange(d_source_functions['cs137']['ln_prior'].__code__.co_argcount)]
		d_source_functions['cs137']['par_guesses'] = [d_energy_information['cs137']['func'].GetParameter(1), 5., 5., d_energy_information['cs137']['func'].GetParameter(2), d_energy_information['cs137']['func'].GetParameter(4)/d_energy_information['cs137']['func'].GetParameter(3)]

	# ------------------------------------------
	# ------------------------------------------
	# Prior and likelihood functions
	# for Na22
	# ------------------------------------------
	# ------------------------------------------

	def ln_prior_na22(delta_na22, low_edge_rolloff_na22, high_edge_rolloff_na22, exp_bkg_na22, res_na22, delta_estimate):
		if delta_estimate*0.9 < delta_na22 < delta_estimate*1.1 and 0.01 < low_edge_rolloff_na22 < 50 and 0.01 < high_edge_rolloff_na22 < 50 and 140 < exp_bkg_na22 < 210 and 0.01 < res_na22 < 0.10:
			return 0.
		else:
			return -np.inf


	def ln_likelihood_na22(energy, delta_na22, low_edge_rolloff_na22, high_edge_rolloff_na22, exp_bkg_na22, res_na22, lbCES, ubCES):
	
		ratio_gaus_to_exp = (1-delta_na22)/delta_na22*exp_bkg_na22*(1 - np.exp((lbCES-ubCES)/exp_bkg_na22))/(2*3.141592)**0.5/(res_na22*511.)*np.exp(-0.5*(energy-511.)**2/(res_na22*511.)**2 - (lbCES-energy)/exp_bkg_na22)
		
		#print np.sum(np.isfinite(np.log(delta_na22/exp_bkg_na22/(1 - np.exp((lbCES-ubCES)/exp_bkg_na22))) + (lbCES-energy)/exp_bkg_na22 + np.log1p(ratio_gaus_to_exp) - np.log1p(np.exp((lbCES-energy)/low_edge_rolloff_na22)) - np.log1p(np.exp((energy-ubCES)/high_edge_rolloff_na22))))
		#print len(energy)
		
		return np.sum(np.log(delta_na22/exp_bkg_na22/(1 - np.exp((lbCES-ubCES)/exp_bkg_na22))) + (lbCES-energy)/exp_bkg_na22 + np.log1p(ratio_gaus_to_exp) - np.log1p(np.exp((lbCES-energy)/low_edge_rolloff_na22)) - np.log1p(np.exp((energy-ubCES)/high_edge_rolloff_na22)))
	
	
		#return np.sum(np.log((delta_na22/exp_bkg_na22*np.exp((lbCES-energy)/exp_bkg_na22)/(1 - np.exp((lbCES-ubCES)/exp_bkg_na22)) + (1-delta_na22)/(2*3.141592)**0.5/(res_na22*511.)*np.exp(-0.5*(energy-511.)**2/(res_na22*511.)**2))/(1+np.exp((lbCES-energy)/low_edge_rolloff_na22))/(1+np.exp((energy-ubCES)/high_edge_rolloff_na22))))

	if 'na22' in d_files:
		d_source_functions['na22'] = {}
		d_source_functions['na22']['lbCES'] = d_energy_information[source]['lbCES']
		d_source_functions['na22']['ubCES'] = d_energy_information[source]['ubCES']
		d_source_functions['na22']['ln_prior'] = ln_prior_na22
		d_source_functions['na22']['ln_likelihood'] = ln_likelihood_na22
		d_source_functions['na22']['pars'] = [0. for i in xrange(d_source_functions['na22']['ln_prior'].__code__.co_argcount)]
		d_source_functions['na22']['par_guesses'] = [d_energy_information['na22']['func'].GetParameter(1), 2., 2., d_energy_information['na22']['func'].GetParameter(2), d_energy_information['na22']['func'].GetParameter(4)/d_energy_information['na22']['func'].GetParameter(3)]


	def ln_posterior(a_parameters, d_s1_s2_arrays, d_source_functions):
		prev_index = 0
		cur_index = 3
		g1, g2, w_value = a_parameters[prev_index:cur_index]
		prev_index = cur_index
		
		
		# since keys have no particular order have to explicitly
		# give options and add the arguments in the correct order
		
		if 'cs137' in d_source_functions:
			# need -1 since have to feed in the estimate
			cur_index += d_source_functions['cs137']['ln_prior'].__code__.co_argcount-1
			d_source_functions['cs137']['pars'] = a_parameters[prev_index:cur_index]
			prev_index = cur_index
		
		if 'na22' in d_source_functions:
			# need -1 since have to feed in the estimate
			cur_index += d_source_functions['na22']['ln_prior'].__code__.co_argcount-1
			d_source_functions['na22']['pars'] = a_parameters[prev_index:cur_index]
			prev_index = cur_index

		if 'bkg' in d_source_functions:
			cur_index += d_source_functions['bkg']['ln_prior'].__code__.co_argcount
			d_source_functions['bkg']['pars'] = a_parameters[prev_index:cur_index]
			prev_index = cur_index

		if 'co57' in d_source_functions:
			cur_index += d_source_functions['co57']['ln_prior'].__code__.co_argcount
			d_source_functions['co57']['pars'] = a_parameters[prev_index:cur_index]
			prev_index = cur_index
		
		
		
		#print a_parameters
	
		tot_ln_likelihood = 0
		tot_ln_likelihood += ln_prior_g1_g2(g1, g2)
		
		if len(d_source_functions) == 1:
			tot_ln_likelihood += ln_prior_w_normal(w_value)
		else:
			#tot_ln_likelihood += ln_prior_w_normal(w_value)
			tot_ln_likelihood += ln_prior_w_uniform(w_value)

		if not np.isfinite(tot_ln_likelihood):
			return -np.inf

		try:
			for source in d_source_functions:
				energy = (d_s1_s2_arrays[source]['S1']/g1 + d_s1_s2_arrays[source]['S2']/g2)*w_value/1000.
				#print 'after energy'
				#print energy
				tot_ln_likelihood += d_source_functions[source]['ln_prior'](*d_source_functions[source]['pars'], delta_estimate=d_energy_information[source]['func'].GetParameter(1))
				#print source
				#print d_source_functions[source]['pars']
				#print 'after priors'
				#print 'new iteration'
				#print tot_ln_likelihood
				#print d_source_functions[source]['ln_likelihood'](energy, *d_source_functions[source]['pars'], lbCES=d_source_functions[source]['lbCES'], ubCES=d_source_functions[source]['ubCES'])
				tot_ln_likelihood += d_source_functions[source]['ln_likelihood'](energy, *d_source_functions[source]['pars'], lbCES=d_source_functions[source]['lbCES'], ubCES=d_source_functions[source]['ubCES'])
			#print tot_ln_likelihood
			if np.isfinite(tot_ln_likelihood):
				return tot_ln_likelihood
			else:
				return -np.inf
		except:
			return -np.inf

	
	#print ln_likelihood_na22(511, 0.75, 30, 30., 175, 0.04, 400, 700)
	#print ln_likelihood_na22(511, 0.75, 30, 30., 100, 0.04, 400, 700)
	#print ln_likelihood_na22(511, 0.75, 30, 30., 10, 0.04, 400, 700)

	#print ln_posterior([g1_test, g2_test, 13.7]+d_source_functions['cs137']['par_guesses']+d_source_functions['na22']['par_guesses'], d_s1_s2_arrays, d_source_functions)
	#print d_source_functions['na22']['par_guesses']
	#print d_source_functions['na22']['ln_likelihood']([g1_test, g2_test, 13.7]+d_source_functions['na22']['par_guesses'], d_s1_s2_arrays, d_source_functions)
	#print d_source_functions['na22']['ln_likelihood']([g1_test, g2_test, 13.7, 0.75, 30, 30., 175, 0.04], d_s1_s2_arrays, d_source_functions)
	#print d_source_functions['na22']['ln_likelihood']([0.2, 40., 6, 0.75, 30, 30., 142, 0.04], d_s1_s2_arrays, d_source_functions)
	#print d_source_functions['na22']['ln_likelihood']([g1_test, g2_test, 13.7, 0.9, 30, 30., 199, 0.04], d_s1_s2_arrays, d_source_functions)
	#print d_source_functions['na22']['ln_likelihood']([0.11, 27., 10., 0.95, 141, 12., 75, 0.04], d_s1_s2_arrays, d_source_functions)
	#raw_input('exit')
	

	"""
	f, ax = plt.subplots()
	def np_likelihood(energy, g1, g2, w_value, delta, gamma_low, gamma_high, tau, resolution, peak_energy):
		return (delta/tau*np.exp((400-energy)/tau)/(1 - np.exp((400-700)/tau)) + (1-delta)/(2*3.141592)**0.5/(resolution*peak_energy)*np.exp(-0.5*(energy-peak_energy)**2/(resolution*peak_energy)**2))/(1+np.exp((400-energy)/gamma_low))/(1+np.exp((energy-700.)/gamma_high))
	
	
	energy = np.linspace(350, 750)
	plt.plot(energy, np_likelihood(energy, 0.12, 28., 13.7, 0.8, 0.9, 0.6, 175, 0.044, 511.))
	plt.show()
	"""


	# independent of energies looked at
	num_walkers = 4000
	num_steps = 300
	num_dim = 3
	starting_values = [g1_test, g2_test, 13.7]
	par_names = ['g1', 'g2', 'w_value']
	

	# add in contributions that depend on the files
	for source in l_acceptable_energies:
		if source in d_files:
			num_dim += d_source_functions[source]['ln_prior'].__code__.co_argcount
			starting_values += d_source_functions[source]['par_guesses']
			par_names += d_source_functions[source]['ln_prior'].__code__.co_varnames
			if source == 'cs137' or source=='na22':
				num_dim -= 1
				par_names = par_names[:-1]
	
	# randomize the start location around the guesses
	starting_pos = [(np.random.randn(num_dim)*starting_values)*(0.05) + starting_values for i in xrange(num_walkers)]

	print ln_posterior.__module__

	sampler = emcee.EnsembleSampler(num_walkers, num_dim, ln_posterior, threads=1, kwargs={'d_s1_s2_arrays':d_s1_s2_arrays, 'd_source_functions':d_source_functions})

	with click.progressbar(sampler.sample(starting_pos, iterations=num_steps, ), length=num_steps) as mcmc_sampler:
			for pos, lnprob, state in mcmc_sampler:
				pass

	samples = sampler.chain[:, 100:, :].reshape((-1, num_dim))

	#print samples
	fig = corner.corner(samples, labels=par_names)
	#fig = corner.corner(samples, labels=['' for i in xrange(len(starting_values))])
	fig.savefig('./corner_plot.png')
	#plt.show()






if __name__ == '__main__':

	d_files = {}


	d_files['cs137'] = []
	d_files['cs137'].append(['nerix_160404_1204', 'nerix_160404_1232', 'nerix_160404_1259', 'nerix_160404_1325', 'nerix_160404_1350'])
	
	"""
	lFiles.append(['nerix_160411_0612', 'nerix_160411_0644', 'nerix_160411_0712', 'nerix_160411_0739', 'nerix_160411_0925'])
	lFiles.append(['nerix_160418_1026', 'nerix_160418_1052', 'nerix_160418_1120', 'nerix_160418_1145', 'nerix_160418_1210'])
	lFiles.append(['nerix_160425_1206', 'nerix_160425_1234', 'nerix_160425_1327', 'nerix_160425_1355', 'nerix_160425_1442'])
	lFiles.append(['nerix_160502_1059', 'nerix_160502_1134', 'nerix_160502_1208', 'nerix_160502_1234', 'nerix_160502_1302'])
	lFiles.append(['nerix_160509_1157', 'nerix_160509_1226', 'nerix_160509_1252', 'nerix_160509_1320', 'nerix_160509_1350'])
	lFiles.append(['nerix_160516_1226', 'nerix_160516_1255', 'nerix_160516_1322', 'nerix_160516_1351', 'nerix_160516_1418'])
	
	
	# not time corrected
	#lFiles = ['nerix_160523_1215', 'nerix_160523_1242', 'nerix_160523_1308', 'nerix_160523_1337', 'nerix_160523_1405']
	#lFiles = ['nerix_160531_1144', 'nerix_160531_1234', 'nerix_160531_1303', 'nerix_160531_1331', 'nerix_160531_1357']
	#lFiles = ['nerix_160606_1155', 'nerix_160606_1225', 'nerix_160606_1254', 'nerix_160606_1347', 'nerix_160606_1421']
	#lFiles = ['', '', '', '', '']
	
	#lFiles = ['nerix_160606_1155', 'nerix_160606_1225', 'nerix_160606_1254', 'nerix_160606_1347', 'nerix_160606_1421']
	
	"""
	
	d_files['na22'] = []
	d_files['na22'].append(['nerix_160404_1421', 'nerix_160404_1447', 'nerix_160404_1530', 'nerix_160404_1555', 'nerix_160404_1621'])
	
	
	
	fit_anticorrelation(d_files)


