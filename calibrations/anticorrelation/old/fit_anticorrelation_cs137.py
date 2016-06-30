#!/usr/bin/python

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
import tqdm, time, copy_reg, types
from root_numpy import tree2array, array2tree
import scipy.optimize as op
import scipy.special
import pymc3 as pm

from pymc3 import log, exp
import theano
import inspect



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

copy_reg.pickle(types.MethodType, _pickle_method, _unpickle_method)





def fit_position_corrections(lFiles, batch_mode=False, fit_full_run=False, use_mcmc=False):

	#--------------- Start Parameters to Change ----------------


	s1NumBins = 40
	s1Min = 1200
	s1Max = 4500


	s2NumBins = 40
	s2Min = 3e5
	s2Max = 10e5


	numBinsCES = 50
	upperBoundCES = 1200.
	
	s1_par = 'cpS1sTotBottom[0]'
	s2_par = 'cpS2sTotBottom[0]'



	#--------------- End Parameters to Change ----------------


	column_names = ('S1', 'S2')

	a_combined_tree = -1

	column_dtype = -1

	l_columns = [np.empty(0) for i in xrange(len(column_names))]
	
	l_settings = [[-1,0.345,4.5], [-1,0.7,4.5], [-1,1.054,4.5], [-1,1.500,4.5], [-1,2.356,4.5]]
	
	
	# transpose list of files such that
	# files of the same cathode voltage are together
	lFiles = map(list, zip(*lFiles))
	

	for cur_index, files_at_setting in enumerate(lFiles):
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
			current_tree = tree2array(current_analysis.get_T1(j), ['cpS1sTotBottom[0]', 'cpS2sTotBottom[0]'], selection=current_analysis.get_cuts())
			current_tree.dtype.names = column_names
			column_dtype = current_tree.dtype
			for i, column_name in enumerate(current_tree.dtype.names):
				l_columns[i] = np.append(l_columns[i], current_tree[column_name])

		del current_analysis

	a_combined_tree = np.empty(len(l_columns[0]), dtype=column_dtype)
	for i, column_name in enumerate(column_names):
		a_combined_tree[column_name] = l_columns[i]

	combined_tree = array2tree(a_combined_tree, name='combined_tree')

	d_fits = {}

	# ---------- MAKE CES CUT ----------

	# no time correction: 0.08, 14.
	g1_test = 0.12
	g2_test = 28. #26.2 #24.
	sigmaCES = 2.0

	cCES = Canvas(width=900, height=700, name='cCES')
	cCES.SetGridx()
	cCES.SetGridy()

	hCES = Hist(numBinsCES, 0, upperBoundCES, name='hCES', title='hCES', drawstyle='hist')
	combined_tree.Draw('0.0137*(S1/%f + S2/%f)>>hCES' % (g1_test, g2_test))

	hCES.Draw()
	cCES.Update()

	#lbCES = float(raw_input('\n\nPlease enter lower bound on combined energy scale: '))
	#ubCES = float(raw_input('\n\nPlease enter upper bound on combined energy scale: '))

	print '\n\nDEBUG MODE SKIPPING LIMIT PICKS\n\n'
	lbCES = 500
	ubCES = 900

	#fGausCES = root.TF1('fGausCES', 'gaus', lbCES, ubCES)
	fGausCES = root.TF1('fGausCES', '[0]*( [1]/[2]*exp(-x/[2]) + (1.-[1])/(2*3.141592)^0.5/[4]*exp(-0.5*pow((x-[3])/[4], 2)) )', lbCES, ubCES)
	fGausCES.SetParameters(500, 0.5, 100, 500, 650, 50)
	fGausCES.SetParLimits(0, 1, 1e8)
	fGausCES.SetParLimits(1, 0, 1)
	fGausCES.SetParLimits(2, 50, 1000)
	fGausCES.SetParLimits(3, lbCES, ubCES)
	fGausCES.SetParLimits(4, 10, 100)
	hCES.Fit(fGausCES, 'LR')
	fGausCES.Draw('same')
	cCES.Update()

	#raw_input('Fit okay?')


	# will fit background with gaussian
	# background can be either exponential or constant
	# exponential: Cs137, Na22
	# constant: Co57, bkg peaks
	sCES_arr = '(0.0137*(S1/%f + S2/%f)) > %f && (0.0137*(S1/%f + S2/%f)) < %f' % (g1_test, g2_test, lbCES, g1_test, g2_test, ubCES)
	#sCES_tree = '(0.0137*(%s/%f + %s/%f) > %f && 0.0137*(%s/%f + %s/%f) < %f)' % (s1_par, g1, s2_par, g2, lbCES, s1_par, g1, s2_par, g2, ubCES)


	percent_from_median = 34.

	print '\n\nCES Cut: %s\n\n' % (sCES_arr)
	a_s1_s2_data = tree2array(combined_tree, ['S1', 'S2'], selection=sCES_arr)

	#print a_s1_s2_data['S1'].mean()
	#print a_s1_s2_data['S2'].mean()

	peak_energy = 662.
	

	
	f, ax = plt.subplots()
	def np_likelihood(energy, g1, g2, w_value, delta, gamma_low, gamma_high, tau, resolution, peak_energy):
		return (delta/tau*np.exp((500-energy)/tau) + (1-delta)/(2*3.141592)**0.5/(resolution*peak_energy)*np.exp(-0.5*(energy-peak_energy)**2/(resolution*peak_energy)**2))/(1+np.exp((500-energy)/gamma_low))/(1+np.exp((energy-900.)/gamma_high))
	
	
	energy = np.linspace(450, 1000)
	plt.plot(energy, np_likelihood(energy, 0.12, 27., 13.46, 0.505, 5.8, 1, 103, 0.044, peak_energy))
	plt.show()
	
	

	#d_guesses = {'g1_interval':g1, 'g2_interval':g2, 'w_value':13.46, 'alpha_interval':fGausCES.GetParameter(1), 'tau_interval':fGausCES.GetParameter(2), 'resolution_interval':(fGausCES.GetParameter(4)/fGausCES.GetParameter(3))}

	s1 = a_s1_s2_data['S1']
	s2 = a_s1_s2_data['S2']
	
	#plt.plot(s1, s2, '.')
	#plt.show()
	
	
	def ln_prior_g1_g2(g1, g2):
		if 0.05 < g1 < 0.20 and 10. < g2 < 40.:
			return 0.
		else:
			return -np.inf

	def ln_prior_w_normal(w_value):
		measured_value = 13.7
		measured_uncertainty = 0.2
		return np.log(1./(2*np.pi)**0.5/measured_uncertainty*np.exp(-0.5*(w_value-measured_value)**2/measured_uncertainty**2))
	
	"""
	def ln_prior_exponential(tau):
		measured_value = fGausCES.GetParameter(2)
		measured_uncertainty = fGausCES.GetParError(2)
		return np.log(1./(2*np.pi)**0.5/measured_uncertainty*np.exp(-0.5*(tau-measured_value)**2/measured_uncertainty**2))
	"""

	def ln_prior_dist_func(delta, gamma_low, gamma_high, tau, resolution):
		if 0 < delta < 1 and 0.01 < gamma_low < 50 and 0.01 < gamma_high < 50 and 50 < tau < 250 and 0.005 < resolution < 0.20:
			return 0.
		else:
			return -np.inf

	def ln_likelihood(a_parameters, aS1, aS2, peak_energy):
		g1, g2, w_value, delta, gamma_low, gamma_high, tau, resolution = a_parameters
		#print a_parameters
	
		tot_ln_likelihood = 0
		tot_ln_likelihood += ln_prior_g1_g2(g1, g2)
		tot_ln_likelihood += ln_prior_w_normal(w_value)
		#tot_ln_likelihood += ln_prior_exponential(tau)
		tot_ln_likelihood += ln_prior_dist_func(delta, gamma_low, gamma_high, tau, resolution)
		
		energy = (aS1/g1 + aS2/g2)*w_value/1000.
		
		if not np.isfinite(tot_ln_likelihood):
			return -np.inf
		
		try:
			#tot_ln_likelihood += np.sum(np.log(delta/(1+np.exp((500-energy)/gamma))*np.exp((500-energy)/tau) + (1-delta)/(2*3.141592)**0.5/(resolution*peak_energy)*np.exp(-0.5*(energy-peak_energy)**2/(resolution*peak_energy)**2)))
			tot_ln_likelihood += np.sum(np.log((delta/tau*np.exp((500-energy)/tau) + (1-delta)/(2*3.141592)**0.5/(resolution*peak_energy)*np.exp(-0.5*(energy-peak_energy)**2/(resolution*peak_energy)**2))/(1+np.exp((500-energy)/gamma_low))/(1+np.exp((energy-900)/gamma_high))))
			
			#print tot_ln_likelihood

			return tot_ln_likelihood
		except:
			return -np.inf


	#print ln_likelihood([g1_test, g2_test, 13.46, 0.1, 32., 230, 0.1], s1, s2, peak_energy)
	#raw_input('exit')

	num_dim = 8
	num_walkers = 1000
	num_steps = 1500
	starting_values = [g1_test, g2_test, 13.46, 0.6, 10., 30., fGausCES.GetParameter(2), fGausCES.GetParameter(4)/fGausCES.GetParameter(3)]
	starting_pos = [(np.random.randn(num_dim)*starting_values)*(0.1) + starting_values for i in xrange(num_walkers)]

	sampler = emcee.EnsembleSampler(num_walkers, num_dim, ln_likelihood, threads=1, args=(s1, s2), kwargs={'peak_energy':peak_energy})
	#sampler.run_mcmc(starting_pos, num_steps)

	with click.progressbar(sampler.sample(starting_pos, iterations=num_steps, ), length=num_steps) as mcmc_sampler:
			for pos, lnprob, state in mcmc_sampler:
				pass

	samples = sampler.chain[:, 500:, :].reshape((-1, num_dim))

	print samples
	fig = corner.corner(samples)
	#fig = corner.corner(samples, labels=['' for i in xrange(len(starting_values))])
	fig.savefig('./corner_plot.png')
	#plt.show()
	
	"""
	a_lb = np.asarray([lbCES for i in xrange(len(s1))])
	with pm.Model() as model:
	
		# '[0]*( [1]/[2]*exp(-x/[2]) + (1.-[1])/(2*3.141592)^0.5/[4]*exp(-0.5*pow((x-[3])/[4], 2)) )'
		g1 = pm.Uniform('g1', lower=0.05, upper=0.20, testval=g1_test)
		g2 = pm.Uniform('g2', lower=10., upper=40., testval=g2_test)
		w_value = pm.Normal('w_value', mu=13.46, sd=0.3)
		
		delta = pm.Uniform('delta', lower=0, upper=1, testval=0.01)
		gamma = pm.Uniform('gamma', lower=0.01, upper=50, testval=5.)
		tau = pm.Uniform('tau', lower=100, upper=1000, testval=fGausCES.GetParameter(2))
		resolution = pm.Uniform('resolution', lower=0.005, upper=0.1, testval=fGausCES.GetParameter(4)/fGausCES.GetParameter(3))
	

		# integral is not 1 because of bounds
		def log_likelihod(s1, s2):
		
			energy = (s1/g1 + s2/g2)*w_value/1000.
			
			return log(delta/(1+exp((lbCES-energy)/gamma))*exp((lbCES-energy)/tau) + (1-delta)/(2*3.141592)**0.5/(resolution*peak_energy)*exp(-0.5*(energy-peak_energy)**2/(resolution*peak_energy)**2))
		
		
		print 'Creating likelihood distribution'
		likelihood = pm.DensityDist('likelihood', log_likelihod, observed={'s1':s1, 's2':s2}, shape=2)
		
		print model.test_point
		
		print 'Finding MAP'
		start = pm.find_MAP()
		print 'MAP: %s' % (str(start))
		
		step = pm.NUTS(state=start)
		trace = pm.sample(200, step, start=start, progressbar=True, njobs=1, random_seed=int(time.time()))
		
		#step = pm.NUTS()
		#trace = pm.sample(200, step, progressbar=True, njobs=1)

		pm.summary(trace)
		print trace['resolution']
		print trace['g1']
				
		f, ax = plt.subplots(7, 2)
		pm.traceplot(trace, ax=ax)
		plt.show()

	"""



if __name__ == '__main__':

	lFiles = []

	lFiles.append(['nerix_160404_1204', 'nerix_160404_1232', 'nerix_160404_1259', 'nerix_160404_1325', 'nerix_160404_1350'])
	"""
	lFiles.append(['nerix_160411_0612', 'nerix_160411_0644', 'nerix_160411_0712', 'nerix_160411_0739', 'nerix_160411_0925'])
	lFiles.append(['nerix_160418_1026', 'nerix_160418_1052', 'nerix_160418_1120', 'nerix_160418_1145', 'nerix_160418_1210'])
	lFiles.append(['nerix_160425_1206', 'nerix_160425_1234', 'nerix_160425_1327', 'nerix_160425_1355', 'nerix_160425_1442'])
	lFiles.append(['nerix_160502_1059', 'nerix_160502_1134', 'nerix_160502_1208', 'nerix_160502_1234', 'nerix_160502_1302'])
	lFiles.append(['nerix_160509_1157', 'nerix_160509_1226', 'nerix_160509_1252', 'nerix_160509_1320', 'nerix_160509_1350'])
	lFiles.append(['nerix_160516_1226', 'nerix_160516_1255', 'nerix_160516_1322', 'nerix_160516_1351', 'nerix_160516_1418'])
	"""
	
	# not time corrected
	#lFiles = ['nerix_160523_1215', 'nerix_160523_1242', 'nerix_160523_1308', 'nerix_160523_1337', 'nerix_160523_1405']
	#lFiles = ['nerix_160531_1144', 'nerix_160531_1234', 'nerix_160531_1303', 'nerix_160531_1331', 'nerix_160531_1357']
	#lFiles = ['nerix_160606_1155', 'nerix_160606_1225', 'nerix_160606_1254', 'nerix_160606_1347', 'nerix_160606_1421']
	#lFiles = ['', '', '', '', '']
	
	#lFiles = ['nerix_160606_1155', 'nerix_160606_1225', 'nerix_160606_1254', 'nerix_160606_1347', 'nerix_160606_1421']
	
	if len(sys.argv) == 1:

		batch_mode = False
		s_for_file = ''

		s_for_file += fit_position_corrections(lFiles, batch_mode)


		print '\n\nCopy and paste into fit summary file!\n\n\n'
		print s_for_file

	elif len(sys.argv) == 2 and sys.argv[1] == '--run':
		fit_position_corrections(lFiles, fit_full_run=True, use_mcmc=True)

	else:
		print 'Either pass no argument to look at a single cathode sweep or pas "--run" to examine an entire run IF YOU KNOW THERE IS NO TIME DEPENDENCE IN THE CORRECTION.'
		sys.exit()


