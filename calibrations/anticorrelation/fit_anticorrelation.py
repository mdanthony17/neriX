#!/usr/bin/python
import pickle
#print pickle.Pickler.dispatch
import dill
#print pickle.Pickler.dispatch

import ROOT as root
import sys, os

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
import gc

gc.disable()


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
	
	
	l_acceptable_energies = ['cs137', 'na22', 'bkg']
	
	# will have to change the setting based on the source otherwise
	# neriX_analysis won't accept the files
	# will do this within the loop
	l_settings = [[-1,0.345,4.5], [-1,0.7,4.5], [-1,1.054,4.5], [-1,1.500,4.5], [-1,2.356,4.5]]
	
	# check that files given are of a supported energy
	
	for source in d_files:
		if source in l_acceptable_energies:
			continue
		else:
			print 'File of type %s not supported yet!' % (source)
			sys.exit()

	hard_coded_correction_factor = 1.48e6 / 1.18e4 * (530./800.)**11.41
	neriX_analysis.warning_message('Using hard coded correction: %.2e' % hard_coded_correction_factor)

	s1_par = 'cpS1sTotBottom[0]/%f' % hard_coded_correction_factor
	s2_par = 'cpS2sTotBottom[0]/%f' % hard_coded_correction_factor

	#s1_par = 'cpS1sTotBottom[0]'
	#s2_par = 'cpS2sTotBottom[0]'

	column_names = ('S1', 'S2')
	
	bounds_width_inclusion = 1.5


	# use dictionary to store all arrays, histograms,
	# fits, etc.

	d_energy_information = {}
	d_s1_s2_arrays = {}
	
	base_save_name = 'ac_'

	for dummy_index, source in enumerate(d_files):
		base_save_name += '%s_' % source

		# create entry in dictionary
		d_energy_information[source] = {}
		
		
		if source == 'cs137':
			d_energy_information[source]['num_bins'] = 250
			d_energy_information[source]['ub_ces'] = 1200
		elif source == 'na22':
			d_energy_information[source]['num_bins'] = 250
			d_energy_information[source]['ub_ces'] = 800
		elif source == 'bkg':
			d_energy_information[source]['num_bins'] = 50
			d_energy_information[source]['ub_ces'] = 450
		elif source == 'co57':
			d_energy_information[source]['num_bins'] = 50
			d_energy_information[source]['ub_ces'] = 250


		a_combined_tree = -1

		column_dtype = -1

		l_columns = [np.empty(0) for i in xrange(len(column_names))]
		
		l_files = d_files[source]
		
		# must change l_settings based on current source
		if source == 'cs137':
			correct_degree_setting = -1
			d_energy_information[source]['peaks'] = 662.
			d_energy_information[source]['color'] = root.kBlue
		elif source == 'na22':
			correct_degree_setting = -5
			d_energy_information[source]['peaks'] = 511.
			d_energy_information[source]['color'] = root.kRed
		elif source == 'bkg':
			correct_degree_setting = -6
			d_energy_information[source]['peaks'] = 164.
			d_energy_information[source]['color'] = root.kGreen+1
		elif source == 'co57':
			correct_degree_setting = -2
			d_energy_information[source]['peaks'] = 122.
			d_energy_information[source]['color'] = root.kMagenta+1

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
			
			if cur_index == 0:
				if len(files_at_setting) == 1:
					directory_save_name = './results/run_%d/%s/' % (run_number, files_at_setting[0][:12])
				else:
					directory_save_name = './results/run_%d_combination/' % (run_number)
			
			
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

		d_energy_information[source]['hist'] = Hist(d_energy_information[source]['num_bins'], 0, d_energy_information[source]['ub_ces'], name='hCES_%s' % (source), title='hCES_%s' % (source), drawstyle='hist')
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
			
			d_energy_information[source]['func'] = root.TF1('fGausCES_%s' % (source), '[0]*( [1] + (1.-[1])/(2*3.141592)^0.5/[3]*exp(-0.5*pow((x-[2])/[3], 2)) )', d_energy_information[source]['lbCES'], d_energy_information[source]['ubCES'])
			d_energy_information[source]['func'].SetParameters(1000, 0.5, 164, 50)
			d_energy_information[source]['func'].SetParLimits(0, 1, 1e8)
			d_energy_information[source]['func'].SetParLimits(1, 0, 1)
			d_energy_information[source]['func'].SetParLimits(2, d_energy_information[source]['lbCES'], d_energy_information[source]['ubCES'])
			d_energy_information[source]['func'].SetParLimits(3, 1, 50)
		
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


		# set lower and upper bounds such that guassian is mostly included
		# but minimal bkg is included
		d_energy_information[source]['lbCES_gaus_only'] = d_energy_information[source]['func'].GetParameter(3) - bounds_width_inclusion*d_energy_information[source]['func'].GetParameter(4)
		d_energy_information[source]['ubCES_gaus_only'] = d_energy_information[source]['func'].GetParameter(3) + bounds_width_inclusion*d_energy_information[source]['func'].GetParameter(4)

		#print d_energy_information[source]['lbCES'], d_energy_information[source]['ubCES']


		# will fit background with gaussian
		# background can be either exponential or constant
		# exponential: Cs137, Na22
		# constant: Co57, bkg peaks
		sCES_arr = '(0.0137*(S1/%f + S2/%f)) > %f && (0.0137*(S1/%f + S2/%f)) < %f' % (g1_test, g2_test, d_energy_information[source]['lbCES_gaus_only'], g1_test, g2_test, d_energy_information[source]['ubCES_gaus_only'])


		d_s1_s2_arrays[source] = tree2array(d_energy_information[source]['combined_tree'], ['S1', 'S2'], selection=sCES_arr)


		
	for source in d_s1_s2_arrays:
		print source
		print d_s1_s2_arrays[source]['S1'].mean()
		print d_s1_s2_arrays[source]['S2'].mean()


	def ln_prior_g1_g2(g1, g2):
		if 0.09 < g1 < 0.15 and 15. < g2 < 33.:
			return 0.
		else:
			return -np.inf


	def ln_prior_w_normal(w_value):
		measured_value = 13.7
		measured_uncertainty = 0.2
		return np.log(1./(2*np.pi)**0.5/measured_uncertainty) - 0.5*(w_value-measured_value)**2/measured_uncertainty**2



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


	def ln_prior_cs137(s2_width_cs137):
		if 5e3 < s2_width_cs137 < 1e5:
			return 0.
		else:
			return -np.inf


	def ln_likelihood_cs137(a_s1, a_s2, g1, g2, w_value, s2_width_cs137):
		a_model_s2 = -(g2/g1)*a_s1 + 662.*g2/(w_value/1000.)
		#print g1, g2, w_value, s2_width_cs137
		return np.sum(np.log(1./(2*np.pi)**0.5/s2_width_cs137) - 0.5*(a_s2-a_model_s2)**2/s2_width_cs137**2)

	if 'cs137' in d_files:
		d_source_functions['cs137'] = {}
		d_source_functions['cs137']['lbCES'] = d_energy_information[source]['lbCES']
		d_source_functions['cs137']['ubCES'] = d_energy_information[source]['ubCES']
		d_source_functions['cs137']['ln_prior'] = ln_prior_cs137
		d_source_functions['cs137']['ln_likelihood'] = ln_likelihood_cs137
		d_source_functions['cs137']['pars'] = [0. for i in xrange(d_source_functions['cs137']['ln_prior'].__code__.co_argcount)]
		d_source_functions['cs137']['par_guesses'] = [4e4]

	# ------------------------------------------
	# ------------------------------------------
	# Prior and likelihood functions
	# for Na22
	# ------------------------------------------
	# ------------------------------------------


	def ln_prior_na22(s2_width_na22):
		if 5e3 < s2_width_na22 < 1e5:
			return 0.
		else:
			return -np.inf


	def ln_likelihood_na22(a_s1, a_s2, g1, g2, w_value, s2_width_na22):
		a_model_s2 = -(g2/g1)*a_s1 + 511.*g2/(w_value/1000.)
		#print g1, g2, w_value, s2_width_na22
		return np.sum(np.log(1./(2*np.pi)**0.5/s2_width_na22) - 0.5*(a_s2-a_model_s2)**2/s2_width_na22**2)



	if 'na22' in d_files:
		d_source_functions['na22'] = {}
		d_source_functions['na22']['lbCES'] = d_energy_information[source]['lbCES']
		d_source_functions['na22']['ubCES'] = d_energy_information[source]['ubCES']
		d_source_functions['na22']['ln_prior'] = ln_prior_na22
		d_source_functions['na22']['ln_likelihood'] = ln_likelihood_na22
		d_source_functions['na22']['pars'] = [0. for i in xrange(d_source_functions['na22']['ln_prior'].__code__.co_argcount)]
		d_source_functions['na22']['par_guesses'] = [3e4]


	def ln_posterior(a_parameters, d_s1_s2_arrays, d_source_functions):
		prev_index = 0
		cur_index = 3
		g1, g2, w_value = a_parameters[prev_index:cur_index]
		prev_index = cur_index
		
		
		# since keys have no particular order have to explicitly
		# give options and add the arguments in the correct order
		
		if 'cs137' in d_source_functions:
			# need -1 since have to feed in the estimate
			cur_index += d_source_functions['cs137']['ln_prior'].__code__.co_argcount
			d_source_functions['cs137']['pars'] = a_parameters[prev_index:cur_index]
			prev_index = cur_index
		
		if 'na22' in d_source_functions:
			# need -1 since have to feed in the estimate
			cur_index += d_source_functions['na22']['ln_prior'].__code__.co_argcount
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
			tot_ln_likelihood += ln_prior_w_normal(w_value)
			#tot_ln_likelihood += ln_prior_w_uniform(w_value)

		if not np.isfinite(tot_ln_likelihood):
			return -np.inf

		try:
			for source in d_source_functions:
				tot_ln_likelihood += d_source_functions[source]['ln_prior'](*d_source_functions[source]['pars'])
				#print source
				#print d_source_functions[source]['pars']
				#print 'after priors'
				#print 'new iteration'
				#print tot_ln_likelihood
				#print d_source_functions[source]['ln_likelihood'](d_s1_s2_arrays[source]['S1'], d_s1_s2_arrays[source]['S2'], g1, g2, w_value, *d_source_functions[source]['pars'])
				tot_ln_likelihood += d_source_functions[source]['ln_likelihood'](d_s1_s2_arrays[source]['S1'], d_s1_s2_arrays[source]['S2'], g1, g2, w_value, *d_source_functions[source]['pars'])
				#print tot_ln_likelihood
			#print 'end call'
			if np.isfinite(tot_ln_likelihood):
				return tot_ln_likelihood
			else:
				return -np.inf
		except:
			return -np.inf



	# independent of energies looked at
	num_walkers = 2000
	num_steps = 200
	num_dim = 3
	starting_values = [g1_test, g2_test, 13.7]
	par_names = ['g1', 'g2', 'w_value']
	

	# add in contributions that depend on the files
	for source in l_acceptable_energies:
		if source in d_files:
			num_dim += d_source_functions[source]['ln_prior'].__code__.co_argcount
			starting_values += d_source_functions[source]['par_guesses']
			par_names += d_source_functions[source]['ln_prior'].__code__.co_varnames
			#if source == 'cs137' or source=='na22':
			#	num_dim -= 2
			#	par_names = par_names[:-2]
	
	# randomize the start location around the guesses
	starting_pos = [(np.random.randn(num_dim)*starting_values)*(0.01) + starting_values for i in xrange(num_walkers)]

	#print starting_values

	sampler = emcee.EnsembleSampler(num_walkers, num_dim, ln_posterior, threads=1, kwargs={'d_s1_s2_arrays':d_s1_s2_arrays, 'd_source_functions':d_source_functions})

	with click.progressbar(sampler.sample(starting_pos, iterations=num_steps, ), length=num_steps) as mcmc_sampler:
			for pos, lnprob, state in mcmc_sampler:
				pass

	samples = sampler.chain[:, 100:, :].reshape((-1, num_dim))

	#print samples
	fig = corner.corner(samples, labels=par_names)

	if not os.path.exists(directory_save_name):
		os.makedirs(directory_save_name)
	fig.savefig(directory_save_name + base_save_name + 'corner.png')

	distance_from_median = 34.

	g1_low, g1_median, g1_high = np.percentile(samples[:, 0], [50.-distance_from_median, 50., 50.+distance_from_median])
	g1_low = g1_median - g1_low
	g1_high = g1_high - g1_median

	g2_low, g2_median, g2_high = np.percentile(samples[:, 1], [50.-distance_from_median, 50., 50.+distance_from_median])
	g2_low = g2_median - g2_low
	g2_high = g2_high - g2_median

	w_low, w_median, w_high = np.percentile(samples[:, 2], [50.-distance_from_median, 50., 50.+distance_from_median])
	w_low = w_median - w_low
	w_high = w_high - w_median

	#d_energy_information[source]['peaks']

	c_s1_s2 = Canvas(900, 700)
	
	s1_num_bins = 50
	s1_low = 200
	s1_high = 4000
	
	s2_num_bins = 50
	s2_low = 100e3
	s2_high = 900e3

	for index_2d_hists, source in enumerate(d_energy_information):
		d_energy_information[source]['hist_s1_s2'] = Hist2D(s1_num_bins, s1_low, s1_high, s2_num_bins, s2_low, s2_high, name='hist_s1_s2_%s' % (source), title='S1 and S2 after CES Cut')
		d_energy_information[source]['hist_s1_s2'].SetColor(d_energy_information[source]['color'])
		d_energy_information[source]['hist_s1_s2'].SetMarkerSize(0)
		d_energy_information[source]['hist_s1_s2'].SetStats(0)
		
		a_temp = np.column_stack([d_s1_s2_arrays[source]['S1'], d_s1_s2_arrays[source]['S2']])
		d_energy_information[source]['hist_s1_s2'].fill_array(a_temp)

		if index_2d_hists == 0:
			d_energy_information[source]['hist_s1_s2'].Draw()
		else:
			d_energy_information[source]['hist_s1_s2'].Draw('same')

		source_slope = -g2_median / g1_median
		source_intercept = d_energy_information[source]['peaks'] * g2_median / (w_median/1000.)

		d_energy_information[source]['f_ac_pol1'] = root.TF1('f_ac_pol1_%s' % (source), 'pol1', s1_low, s1_high)
		d_energy_information[source]['f_ac_pol1'].SetParameters(source_intercept, source_slope)
		d_energy_information[source]['f_ac_pol1'].SetLineColor(root.kBlack)
		d_energy_information[source]['f_ac_pol1'].Draw('same')

		c_s1_s2.Update()
		
	tpt_ac = root.TPaveText(.65,.7,.85,.85,'blNDC')
	tpt_ac.AddText('g_{1} = %.3f^{+%.3f}_{-%.3f} #frac{PE}{photon}' % (g1_median, g1_high, g1_low))
	tpt_ac.AddText('g_{2} = %.2f^{+%.2f}_{-%.2f} #frac{PE}{electron}' % (g2_median, g2_high, g2_low))
	tpt_ac.AddText('w = %.2f^{+%.2f}_{-%.2f} #frac{eV}{quanta}' % (w_median, w_high, w_low))
	tpt_ac.SetTextColor(root.kBlack)
	tpt_ac.SetFillStyle(0)
	tpt_ac.SetBorderSize(0)
	tpt_ac.Draw('same')
	c_s1_s2.Update()

	neriX_analysis.save_plot(directory_save_name, c_s1_s2, base_save_name + 's1_s2')

	# ------------------------------------------
	# ------------------------------------------
	# Perform second round of fitting to
	# determine resolution
	# ------------------------------------------
	# ------------------------------------------

	for source in d_energy_information:
		d_energy_information[source]['canvas_final_spec'] = Canvas(width=900, height=700, name='cCES_final_%s' % (source))
		d_energy_information[source]['canvas_final_spec'].SetGridx()
		d_energy_information[source]['canvas_final_spec'].SetGridy()

		d_energy_information[source]['hist_final_spec'] = Hist(d_energy_information[source]['num_bins'], 0, d_energy_information[source]['ub_ces'], name='hCES_final_%s' % (source), title='%s Energy Spectrum' % (source), drawstyle='hist')
		d_energy_information[source]['combined_tree'].Draw('%f*(S1/%f + S2/%f)>>hCES_final_%s' % (w_median/1000., g1_median, g2_median, source))

		d_energy_information[source]['hist_final_spec'].SetStats(0)
		d_energy_information[source]['hist_final_spec'].Draw()
		d_energy_information[source]['canvas_final_spec'].Update()


		if source == 'cs137':
			d_energy_information[source]['func_final_spec'] = root.TF1('fGausCES_final_%s' % (source), '[0]*( [1]/[2]*exp(-x/[2])/(exp(-%f/[2])-exp(-%f/[2])) + (1.-[1])/(2*3.141592)^0.5/[4]*exp(-0.5*pow((x-[3])/[4], 2)) )' % (d_energy_information[source]['lbCES'], d_energy_information[source]['ubCES']), d_energy_information[source]['lbCES'], d_energy_information[source]['ubCES'])
			
			d_energy_information[source]['func_final_spec'].SetParameters(d_energy_information[source]['func'].GetParameter(0), d_energy_information[source]['func'].GetParameter(1), d_energy_information[source]['func'].GetParameter(2), d_energy_information[source]['func'].GetParameter(3), d_energy_information[source]['func'].GetParameter(4))
			d_energy_information[source]['func_final_spec'].SetParLimits(0, 1, 1e8)
			d_energy_information[source]['func_final_spec'].SetParLimits(1, 0, 1)
			d_energy_information[source]['func_final_spec'].SetParLimits(2, 50, 1000)
			d_energy_information[source]['func_final_spec'].SetParLimits(3, d_energy_information[source]['lbCES'], d_energy_information[source]['ubCES'])
			d_energy_information[source]['func_final_spec'].SetParLimits(4, 10, 100)
		
		elif source == 'na22':
			d_energy_information[source]['func_final_spec'] = root.TF1('fGausCES_final_%s' % (source), '[0]*( [1]/[2]*exp(-x/[2])/(exp(-%f/[2])-exp(-%f/[2])) + (1.-[1])/(2*3.141592)^0.5/[4]*exp(-0.5*pow((x-[3])/[4], 2)) )' % (d_energy_information[source]['lbCES'], d_energy_information[source]['ubCES']), d_energy_information[source]['lbCES'], d_energy_information[source]['ubCES'])
			d_energy_information[source]['func_final_spec'].SetParameters(1000, 0.7, 170, 511, 20)
			d_energy_information[source]['func_final_spec'].SetParLimits(0, 1, 1e8)
			d_energy_information[source]['func_final_spec'].SetParLimits(1, 0, 1)
			d_energy_information[source]['func_final_spec'].SetParLimits(2, 10, 500)
			d_energy_information[source]['func_final_spec'].SetParLimits(3, d_energy_information[source]['lbCES'], d_energy_information[source]['ubCES'])
			d_energy_information[source]['func_final_spec'].SetParLimits(4, 10, 100)
		elif source == 'bkg':
			print 'Not ready!!'
			sys.exit()
			"""
			d_energy_information[source]['func_final_spec'] = root.TF1('fGausCES_final_%s' % (source), '[0]*( [1]/[2]*exp(-x/[2]) + (1.-[1])/(2*3.141592)^0.5/[4]*exp(-0.5*pow((x-[3])/[4], 2)) )', d_energy_information[source]['lbCES'], d_energy_information[source]['ubCES'])
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
			d_energy_information[source]['func_final_spec'] = root.TF1('fGausCES_final_%s' % (source), '[0]*( [1]/[2]*exp(-x/[2]) + (1.-[1])/(2*3.141592)^0.5/[4]*exp(-0.5*pow((x-[3])/[4], 2)) )', d_energy_information[source]['lbCES'], d_energy_information[source]['ubCES'])
			fGausCES.SetParameters(1000, 0.5, 100, 500, 511, 50)
			fGausCES.SetParLimits(0, 1, 1e8)
			fGausCES.SetParLimits(1, 0, 1)
			fGausCES.SetParLimits(2, 10, 500)
			fGausCES.SetParLimits(3, d_energy_information[source]['lbCES'], d_energy_information[source]['ubCES'])
			fGausCES.SetParLimits(4, 10, 100)
			"""


		
		d_energy_information[source]['hist_final_spec'].Fit(d_energy_information[source]['func_final_spec'], 'LR')
		d_energy_information[source]['func_final_spec'].Draw('same')


		# find resolution from fit
		mean_peak = d_energy_information[source]['func_final_spec'].GetParameter(3)
		mean_peak_uncertainty = d_energy_information[source]['func_final_spec'].GetParError(3)
		width_peak = d_energy_information[source]['func_final_spec'].GetParameter(4)
		width_peak_uncertainty = d_energy_information[source]['func_final_spec'].GetParError(4)
		resolution_peak = width_peak / mean_peak
		resolution_peak_uncertainty = abs(resolution_peak) * ( (mean_peak_uncertainty/mean_peak)**2 + (width_peak_uncertainty/width_peak)**2 )**0.5

		# create TPT for plot showing the resolution
		d_energy_information[source]['tpt_final_spec'] = root.TPaveText(.55,.6,.85,.85,'blNDC')
		d_energy_information[source]['tpt_final_spec'].AddText('R_{%s} = %.2e #pm %.2e' % (source, resolution_peak, resolution_peak_uncertainty))
		d_energy_information[source]['tpt_final_spec'].SetTextColor(root.kRed)
		d_energy_information[source]['tpt_final_spec'].SetFillStyle(0)
		d_energy_information[source]['tpt_final_spec'].SetBorderSize(0)
		d_energy_information[source]['tpt_final_spec'].Draw('same')


		d_energy_information[source]['canvas_final_spec'].Update()

		neriX_analysis.save_plot(directory_save_name, d_energy_information[source]['canvas_final_spec'], '%s_ces_spectrum' % (source))





	raw_input('press enter')





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
	
	#d_files['bkg'] = []
	#d_files['bkg'].append(['nerix_160404_1802', 'nerix_160405_0034', 'nerix_160405_0625', 'nerix_160405_0737', 'nerix_160405_0915'])
	
	
	
	fit_anticorrelation(d_files)


