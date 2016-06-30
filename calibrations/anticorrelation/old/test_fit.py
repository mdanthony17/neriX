#!/usr/bin/python

import ROOT as root
import sys

import matplotlib
matplotlib.use('QT4Agg')
import matplotlib.pyplot as plt

import neriX_analysis, neriX_datasets
from rootpy.plotting import Hist2D, Hist, Legend, Canvas
from rootpy.tree import Tree
import numpy as np
from math import exp, log
import tqdm
from root_numpy import tree2array, array2tree
import scipy.optimize as op
import pymc3 as pm

"""
def abLikelihood(value):
    return tensor.switch(any(value <= 0), -np.Inf, power * np.log(value[0]+value[1])

ab = pymc3.DensityDist('mylike', computeLike, shape=2, testval = [1,1])

a = ab[0]
b = ab[1]



def computeLike(value):
  x = value[:n]
  y = value[n:]
  return #computed likelihood

xy = pymc3.DensityDist('mylike', computeLike, observed=my2d_data)

x = xy[:n]

y = xy[n:]

"""


def fit_position_corrections(lFiles, batch_mode=False, fit_full_run=False, use_mcmc=False):

	#--------------- Start Parameters to Change ----------------


	s1NumBins = 40
	s1Min = 1200
	s1Max = 4500


	s2NumBins = 40
	s2Min = 3e5
	s2Max = 10e5

	zNumBins = 23
	zMin = -24
	zMax = -1

	rNumBins = 10
	rMin = 0
	rMax = 1

	dtNumBins = 40
	dtMin = 0
	dtMax = 20

	numBinsCES = 50
	upperBoundCES = 1200.
	
	s1_par = 'ctS1sTotBottom[0]'
	s2_par = 'ctS2sTotBottom[0]'

	relNumBins = 40
	relMin = 0.65
	relMax = 2.0

	normalizedS2NumBins = 20
	normalizedS2Min = 0.3
	normalizedS2Max = 1.7
	
	numBinsProfile = 200


	#--------------- End Parameters to Change ----------------


	column_names = ('Z', 'R2', 'dt', 'S1', 'S2')

	a_combined_tree = -1

	column_dtype = -1

	l_columns = [np.empty(0) for i in xrange(len(column_names))]
	
	l_settings = [[-1,0.345,4.5], [-1,0.7,4.5], [-1,1.054,4.5], [-1,1.500,4.5], [-1,2.356,4.5]]
	
	
	# transpose list of files such that
	# files of the same cathode voltage are together
	if fit_full_run:
		lFiles = map(list, zip(*lFiles))
	

	for cur_index, file in enumerate(lFiles):
		# if fitting a full run, "file" is actually a list of
		# files for a given cathode setting
		# name is from older version
		
		current_analysis = neriX_analysis.neriX_analysis(file, *l_settings[cur_index])
		
		current_analysis.add_z_cut()
		current_analysis.add_single_scatter_cut()
		current_analysis.add_radius_cut(0, 0.85)
		
		run_number = current_analysis.get_run()
		
		current_analysis.set_event_list()
		
		for j in xrange(len(list(file))):
			current_tree = tree2array(current_analysis.get_T1(j), [current_analysis.get_T1().GetAlias('Z'), current_analysis.get_T1().GetAlias('R') + '^2', current_analysis.get_T1().GetAlias('dt'), 'ctS1sTotBottom[0]', 'ctS2sTotBottom[0]'], selection=current_analysis.get_cuts())
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
	g1 = 0.12
	g2 = 28. #26.2 #24.
	sigmaCES = 2.0

	cCES = Canvas(width=900, height=700, name='cCES')
	cCES.SetGridx()
	cCES.SetGridy()

	hCES = Hist(numBinsCES, 0, upperBoundCES, name='hCES', title='hCES', drawstyle='hist')
	combined_tree.Draw('0.0137*(S1/%f + S2/%f)>>hCES' % (g1, g2))

	hCES.Draw()
	cCES.Update()

	lbCES = float(raw_input('\n\nPlease enter lower bound on combined energy scale: '))
	ubCES = float(raw_input('\n\nPlease enter upper bound on combined energy scale: '))

	#print '\n\nDEBUG MODE SKIPPING LIMIT PICKS\n\n'
	#lbCES = 500
	#ubCES = 900

	#fGausCES = root.TF1('fGausCES', 'gaus', lbCES, ubCES)
	fGausCES = root.TF1('fGausCES', '[0]*exp(-[1]*x) + [2]*exp(-0.5*pow((x-[3])/[4], 2))', lbCES, ubCES)
	fGausCES.SetParameters(500, 0.01, 500, 650, 50)
	fGausCES.SetParLimits(3, lbCES, ubCES)
	fGausCES.SetParLimits(0, 1, 1e8)
	fGausCES.SetParLimits(2, 1, 1e8)
	hCES.Fit(fGausCES, 'LR')
	fGausCES.Draw('same')
	cCES.Update()

	lbCES = fGausCES.GetParameter(3) - sigmaCES*fGausCES.GetParameter(4)
	ubCES = fGausCES.GetParameter(3) + sigmaCES*fGausCES.GetParameter(4)
	raw_input('Fit okay?')


	#sCES_arr = '(0.0137*(S1/%f + S2/%f) > %f && 0.0137*(S1/%f + S2/%f) < %f)' % (g1, g2, lbCES, g1, g2, ubCES)
	sCES_tree = '(0.0137*(%s/%f + %s/%f) > %f && 0.0137*(%s/%f + %s/%f) < %f)' % (s1_par, g1, s2_par, g2, lbCES, s1_par, g1, s2_par, g2, ubCES)




	percent_from_median = 34.




	cMeans = Canvas(1800, 700)
	cMeans.Divide(2)
	cS1 = Canvas(1800, 700)
	cS1.Divide(2)
	cS2 = Canvas(1800, 700)
	cS2.Divide(2)


	# -----------------------------------------
	# -----------------------------------------
	# Loop through files and fit individually
	# -----------------------------------------
	# -----------------------------------------

	l_first_draw = [True, True, True, True, True, True]

	d_cathode_data = {}

	for iteration_num, file in enumerate(lFiles):

		current_analysis = neriX_analysis.neriX_analysis(file, *l_settings[iteration_num])
		cathode_setting = current_analysis.get_cathode_setting()
		
		
		
		

		
		
		
		d_cathode_data[cathode_setting] = {}
		d_fits[cathode_setting] = {}

		if cathode_setting == 0.345:
			color = root.kMagenta
			movement = 0.0
			fill_style = 3004
			save_name = current_analysis.get_filename_no_ext()
		elif cathode_setting == 0.700:
			color = root.kBlue
			movement = 0.05
			fill_style = 3005
		elif cathode_setting == 1.054:
			color = root.kGreen+1
			movement = 0.1
			fill_style = 3006
		elif cathode_setting == 1.500:
			color = root.kOrange+1
			movement = 0.15
			fill_style = 3007
		elif cathode_setting == 2.356:
			color = root.kRed
			movement = 0.2
			fill_style = 3008
		
		current_analysis.add_z_cut()
		current_analysis.add_single_scatter_cut()
		current_analysis.add_radius_cut(0, 0.85)
		current_analysis.add_cut(sCES_tree)
		

		# -----------------------------------------
		# -----------------------------------------
		# Simple S1 and S2 for means AT CENTER
		# -----------------------------------------
		# -----------------------------------------
		
		
		s_center_cut = ' && ((Z > -16 && Z < -9))'

		cMeans.cd(1)

		d_cathode_data[cathode_setting]['h_s1'] = Hist(s1NumBins, s1Min, s1Max, name='h_s1_%f' % (cathode_setting), title='S1')
		current_analysis.Draw(s1_par, hist=d_cathode_data[cathode_setting]['h_s1'], selection=current_analysis.get_cuts() + s_center_cut)

		d_cathode_data[cathode_setting]['h_s1'].SetStats(0)
		d_cathode_data[cathode_setting]['h_s1'].SetMarkerSize(0)
		d_cathode_data[cathode_setting]['h_s1'].GetXaxis().SetTitle('S1 [PE]')
		d_cathode_data[cathode_setting]['h_s1'].GetYaxis().SetTitle('Counts')
		d_cathode_data[cathode_setting]['h_s1'].SetLineColor(color)
		d_cathode_data[cathode_setting]['h_s1'].GetYaxis().SetRangeUser(0, 2.5*d_cathode_data[cathode_setting]['h_s1'].GetMaximum())

		if l_first_draw[0]:
			d_cathode_data[cathode_setting]['h_s1'].Draw('')
			l_first_draw[0] = False
		else:
			d_cathode_data[cathode_setting]['h_s1'].Draw('same')

		d_cathode_data[cathode_setting]['f_s1_trial'] = root.TF1('f_s1_trial_%f' % (cathode_setting), 'gaus', s1Min, s1Max)
		d_cathode_data[cathode_setting]['h_s1'].Fit('f_s1_trial_%f' % (cathode_setting), 'QLLEMN')
		s1MinTrial = d_cathode_data[cathode_setting]['f_s1_trial'].GetParameter(1) - 1.5*d_cathode_data[cathode_setting]['f_s1_trial'].GetParameter(2)
		s1MaxTrial = d_cathode_data[cathode_setting]['f_s1_trial'].GetParameter(1) + 0.5*d_cathode_data[cathode_setting]['f_s1_trial'].GetParameter(2)


		d_cathode_data[cathode_setting]['f_s1'] = root.TF1('f_s1_%f' % (cathode_setting), 'gaus', s1MinTrial, s1MaxTrial)
		d_cathode_data[cathode_setting]['h_s1'].Fit('f_s1_%f' % (cathode_setting), 'QLLEMNR')
		d_cathode_data[cathode_setting]['f_s1'].SetLineColor(color)
		d_cathode_data[cathode_setting]['f_s1'].Draw('same')

		meanS1 = d_cathode_data[cathode_setting]['f_s1'].GetParameter(1)
		widthS1 = d_cathode_data[cathode_setting]['f_s1'].GetParameter(2)

		d_cathode_data[cathode_setting]['s1_mean'] = d_cathode_data[cathode_setting]['f_s1'].GetParameter(1)
		d_cathode_data[cathode_setting]['s1_mean_err'] = d_cathode_data[cathode_setting]['f_s1'].GetParError(1)
		d_cathode_data[cathode_setting]['s1_width'] = d_cathode_data[cathode_setting]['f_s1'].GetParameter(2)
		d_cathode_data[cathode_setting]['s1_width_err'] = d_cathode_data[cathode_setting]['f_s1'].GetParError(2)
		
		d_fits[cathode_setting]['s1_mean'] = d_cathode_data[cathode_setting]['f_s1'].GetParameter(1)
		d_fits[cathode_setting]['s1_mean_err'] = d_cathode_data[cathode_setting]['f_s1'].GetParError(1)
		d_fits[cathode_setting]['s1_width'] = d_cathode_data[cathode_setting]['f_s1'].GetParameter(2)
		d_fits[cathode_setting]['s1_width_err'] = d_cathode_data[cathode_setting]['f_s1'].GetParError(2)

		d_cathode_data[cathode_setting]['tpt_s1_%f' % (cathode_setting)] = root.TPaveText(.15,.85-movement,.7,.9-movement,'blNDC')
		d_cathode_data[cathode_setting]['tpt_s1_%f' % (cathode_setting)].SetTextColor(color)
		d_cathode_data[cathode_setting]['tpt_s1_%f' % (cathode_setting)].SetFillStyle(0)
		d_cathode_data[cathode_setting]['tpt_s1_%f' % (cathode_setting)].SetBorderSize(0)
		d_cathode_data[cathode_setting]['tpt_s1_%f' % (cathode_setting)].AddText('V_{c} = %skV: mean = %.2e PE, width = %.2e PE' % (cathode_setting, d_cathode_data[cathode_setting]['s1_mean'], d_cathode_data[cathode_setting]['s1_width']))
		d_cathode_data[cathode_setting]['tpt_s1_%f' % (cathode_setting)].Draw('same')



		cMeans.cd(2)

		d_cathode_data[cathode_setting]['h_s2'] = Hist(s2NumBins, s2Min, s2Max, name='h_s2_%f' % (cathode_setting), title='S2')
		current_analysis.Draw(s2_par, hist=d_cathode_data[cathode_setting]['h_s2'], selection=current_analysis.get_cuts() + s_center_cut)

		d_cathode_data[cathode_setting]['h_s2'].SetStats(0)
		d_cathode_data[cathode_setting]['h_s2'].SetMarkerSize(0)
		d_cathode_data[cathode_setting]['h_s2'].GetXaxis().SetTitle('S2 [PE]')
		d_cathode_data[cathode_setting]['h_s2'].GetYaxis().SetTitle('Counts')
		d_cathode_data[cathode_setting]['h_s2'].SetLineColor(color)
		d_cathode_data[cathode_setting]['h_s2'].GetYaxis().SetRangeUser(0, 2.5*d_cathode_data[cathode_setting]['h_s2'].GetMaximum())

		if l_first_draw[1]:
			d_cathode_data[cathode_setting]['h_s2'].Draw('')
			l_first_draw[1] = False
		else:
			d_cathode_data[cathode_setting]['h_s2'].Draw('same')

		d_cathode_data[cathode_setting]['f_s2_trial'] = root.TF1('f_s2_trial_%f' % (cathode_setting), 'gaus', s2Min, s2Max)
		d_cathode_data[cathode_setting]['h_s2'].Fit('f_s2_trial_%f' % (cathode_setting), 'QLLEMN')
		s1MinTrial = d_cathode_data[cathode_setting]['f_s2_trial'].GetParameter(1) - 0.5*d_cathode_data[cathode_setting]['f_s2_trial'].GetParameter(2)
		s1MaxTrial = d_cathode_data[cathode_setting]['f_s2_trial'].GetParameter(1) + 1.5*d_cathode_data[cathode_setting]['f_s2_trial'].GetParameter(2)


		d_cathode_data[cathode_setting]['f_s2'] = root.TF1('f_s2_%f' % (cathode_setting), 'gaus', s1MinTrial, s1MaxTrial)
		d_cathode_data[cathode_setting]['h_s2'].Fit('f_s2_%f' % (cathode_setting), 'QLLEMNR')
		d_cathode_data[cathode_setting]['f_s2'].SetLineColor(color)
		d_cathode_data[cathode_setting]['f_s2'].Draw('same')

		meanS2 = d_cathode_data[cathode_setting]['f_s2'].GetParameter(1)
		widthS2 = d_cathode_data[cathode_setting]['f_s2'].GetParameter(2)

		d_cathode_data[cathode_setting]['s2_mean'] = d_cathode_data[cathode_setting]['f_s2'].GetParameter(1)
		d_cathode_data[cathode_setting]['s2_mean_err'] = d_cathode_data[cathode_setting]['f_s2'].GetParError(1)
		d_cathode_data[cathode_setting]['s2_width'] = d_cathode_data[cathode_setting]['f_s2'].GetParameter(2)
		d_cathode_data[cathode_setting]['s2_width_err'] = d_cathode_data[cathode_setting]['f_s2'].GetParError(2)
		
		d_fits[cathode_setting]['s2_mean'] = d_cathode_data[cathode_setting]['f_s2'].GetParameter(1)
		d_fits[cathode_setting]['s2_mean_err'] = d_cathode_data[cathode_setting]['f_s2'].GetParError(1)
		d_fits[cathode_setting]['s2_width'] = d_cathode_data[cathode_setting]['f_s2'].GetParameter(2)
		d_fits[cathode_setting]['s2_width_err'] = d_cathode_data[cathode_setting]['f_s2'].GetParError(2)

		d_cathode_data[cathode_setting]['tpt_s2_%f' % (cathode_setting)] = root.TPaveText(.15,.85-movement,.7,.9-movement,'blNDC')
		d_cathode_data[cathode_setting]['tpt_s2_%f' % (cathode_setting)].SetTextColor(color)
		d_cathode_data[cathode_setting]['tpt_s2_%f' % (cathode_setting)].SetFillStyle(0)
		d_cathode_data[cathode_setting]['tpt_s2_%f' % (cathode_setting)].SetBorderSize(0)
		d_cathode_data[cathode_setting]['tpt_s2_%f' % (cathode_setting)].AddText('V_{c} = %skV: mean = %.2e PE, width = %.2e PE' % (cathode_setting, d_cathode_data[cathode_setting]['s2_mean'], d_cathode_data[cathode_setting]['s2_width']))
		d_cathode_data[cathode_setting]['tpt_s2_%f' % (cathode_setting)].Draw('same')



		cMeans.Update()
		
		
		# add these cuts to narrow down good events
		distance_from_mean = 2.0
		current_analysis.add_cut('(%s > %f) && (%s < %f)' % (s1_par, d_fits[cathode_setting]['s1_mean'] - distance_from_mean*d_fits[cathode_setting]['s1_width'], s1_par, d_fits[cathode_setting]['s1_mean'] + distance_from_mean*d_fits[cathode_setting]['s1_width']))
		current_analysis.add_cut('(%s > %f) && (%s < %f)' % (s2_par, d_fits[cathode_setting]['s2_mean'] - distance_from_mean*d_fits[cathode_setting]['s2_width'], s2_par, d_fits[cathode_setting]['s2_mean'] + distance_from_mean*d_fits[cathode_setting]['s2_width']))
		current_analysis.set_event_list()


		# fill array if using mcmc
		if use_mcmc:
			l_columns = [np.empty(0) for i in xrange(len(column_names))]
		
			for j in xrange(len(list(file))):
				current_tree = tree2array(current_analysis.get_T1(j), [current_analysis.get_T1().GetAlias('Z'), current_analysis.get_T1().GetAlias('R') + '^2', current_analysis.get_T1().GetAlias('dt'), 'ctS1sTotBottom[0]', 'ctS2sTotBottom[0]'], selection=current_analysis.get_cuts())
				current_tree.dtype.names = column_names
				column_dtype = current_tree.dtype
			for i, column_name in enumerate(current_tree.dtype.names):
				l_columns[i] = np.append(l_columns[i], current_tree[column_name])


			a_combined_tree = np.empty(len(l_columns[0]), dtype=column_dtype)
			for i, column_name in enumerate(column_names):
				a_combined_tree[column_name] = l_columns[i]


		# -----------------------------------------
		# -----------------------------------------
		# S1 vs Z and R^2
		# -----------------------------------------
		# -----------------------------------------


		cS1.cd(1)

		d_cathode_data[cathode_setting]['h_s1_z'] = Hist2D(zNumBins, zMin, zMax, numBinsProfile, relMin, relMax, name='h_s1_z_%f' % (cathode_setting), title='S1 vs. Z')
		current_analysis.Draw('Z:(%s/%f)' % (s1_par, meanS1), hist=d_cathode_data[cathode_setting]['h_s1_z'])


		d_cathode_data[cathode_setting]['tuple'] = neriX_analysis.profile_y_median(d_cathode_data[cathode_setting]['h_s1_z'], percent_from_median=percent_from_median)
		d_cathode_data[cathode_setting]['g_s1_z'] = root.TGraphAsymmErrors(*d_cathode_data[cathode_setting]['tuple'])
		d_cathode_data[cathode_setting]['g_s1_z'].SetTitle('Relative S1 Signal vs Z')
		d_cathode_data[cathode_setting]['g_s1_z'].SetMarkerSize(0)
		d_cathode_data[cathode_setting]['g_s1_z'].GetYaxis().SetRangeUser(relMin, relMax)
		d_cathode_data[cathode_setting]['g_s1_z'].GetXaxis().SetTitle('Z [mm]')
		d_cathode_data[cathode_setting]['g_s1_z'].GetYaxis().SetTitle('Relative S1 Signal')
		d_cathode_data[cathode_setting]['g_s1_z'].SetLineColor(color)
		if l_first_draw[2]:
			d_cathode_data[cathode_setting]['g_s1_z'].Draw('ap')
			l_first_draw[2] = False
		else:
			d_cathode_data[cathode_setting]['g_s1_z'].Draw('same p')

		d_cathode_data[cathode_setting]['f_s1_z'] = root.TF1('f_s1_z_%f' % (cathode_setting), 'pol1', zMin, zMax)
		d_cathode_data[cathode_setting]['f_s1_z'].SetLineColor(color)

		if not use_mcmc:
			
			d_cathode_data[cathode_setting]['g_s1_z'].Fit('f_s1_z_%f' % (cathode_setting), 'QLLEMN')

			d_cathode_data[cathode_setting]['s1_z_intercept'] = d_cathode_data[cathode_setting]['f_s1_z'].GetParameter(0)
			d_cathode_data[cathode_setting]['s1_z_intercept_err'] = d_cathode_data[cathode_setting]['f_s1_z'].GetParError(0)
			d_cathode_data[cathode_setting]['s1_z_slope'] = d_cathode_data[cathode_setting]['f_s1_z'].GetParameter(1)
			d_cathode_data[cathode_setting]['s1_z_slope_err'] = d_cathode_data[cathode_setting]['f_s1_z'].GetParError(1)
		else:
			trace = None
			y = a_combined_tree['S1']/meanS1
			with pm.Model() as model:
				alpha = pm.Uniform('alpha', lower=-1, upper=1)
				beta = pm.Uniform('beta', lower=-1, upper=1)
				sigma = pm.Uniform('sigma', lower=0, upper=1)
				
				y_est = alpha + beta*a_combined_tree['Z']
				
				likelihood = pm.Normal('y', mu=y_est, sd=sigma, observed=y)
				
				start = pm.find_MAP()
				step = pm.NUTS(state=start)
				trace = pm.sample(3000, step, start=start, progressbar=True, njobs=4)
				
				#pm.summary(trace)
				
				f, ax = plt.subplots(3, 2)
				pm.traceplot(trace, ax=ax)
				plt.show()
				
				# use median and median absolute deviation for
				# representative value and uncertainty
				
				d_cathode_data[cathode_setting]['s1_z_intercept'] = np.median(trace.alpha)
				d_cathode_data[cathode_setting]['s1_z_intercept_err'] = neriX_analysis.mad(trace.alpha)
				d_cathode_data[cathode_setting]['s1_z_slope'] = np.median(trace.beta)
				d_cathode_data[cathode_setting]['s1_z_slope_err'] = neriX_analysis.mad(trace.beta)

				d_cathode_data[cathode_setting]['f_s1_z'].SetParameters(d_cathode_data[cathode_setting]['s1_z_intercept'], d_cathode_data[cathode_setting]['s1_z_slope'])
				

		d_cathode_data[cathode_setting]['f_s1_z'].Draw('same')

		d_fits[cathode_setting]['s1_z_intercept'] = d_cathode_data[cathode_setting]['s1_z_intercept'] = np.median(trace.alpha)
		d_fits[cathode_setting]['s1_z_intercept_err'] = d_cathode_data[cathode_setting]['s1_z_intercept_err']
		d_fits[cathode_setting]['s1_z_slope'] = d_cathode_data[cathode_setting]['s1_z_slope']
		d_fits[cathode_setting]['s1_z_slope_err'] = d_cathode_data[cathode_setting]['s1_z_slope_err']

		d_cathode_data[cathode_setting]['tpt_s1_z_%f' % (cathode_setting)] = root.TPaveText(.15,.85-movement,.7,.9-movement,'blNDC')
		d_cathode_data[cathode_setting]['tpt_s1_z_%f' % (cathode_setting)].SetTextColor(color)
		d_cathode_data[cathode_setting]['tpt_s1_z_%f' % (cathode_setting)].SetFillStyle(0)
		d_cathode_data[cathode_setting]['tpt_s1_z_%f' % (cathode_setting)].SetBorderSize(0)
		d_cathode_data[cathode_setting]['tpt_s1_z_%f' % (cathode_setting)].AddText('V_{c} = %skV: intercept = %.2e +/- %.2e, slope = %.2e +/- %.2e' % (cathode_setting, d_cathode_data[cathode_setting]['s1_z_intercept'], d_cathode_data[cathode_setting]['s1_z_intercept_err'], d_cathode_data[cathode_setting]['s1_z_slope'], d_cathode_data[cathode_setting]['s1_z_slope_err']))
		d_cathode_data[cathode_setting]['tpt_s1_z_%f' % (cathode_setting)].Draw('same')



		cS1.cd(2)

		d_cathode_data[cathode_setting]['h_s1_r'] = Hist2D(rNumBins, rMin, rMax, numBinsProfile, relMin, relMax, name='h_s1_r_%f' % (cathode_setting), title='S1 vs. R^{2}')
		current_analysis.Draw('R*R:(%s/%f)' % (s1_par, meanS1), hist=d_cathode_data[cathode_setting]['h_s1_r'])


		d_cathode_data[cathode_setting]['tuple'] = neriX_analysis.profile_y_median(d_cathode_data[cathode_setting]['h_s1_r'], percent_from_median=percent_from_median)
		d_cathode_data[cathode_setting]['g_s1_r'] = root.TGraphAsymmErrors(*d_cathode_data[cathode_setting]['tuple'])
		d_cathode_data[cathode_setting]['g_s1_r'].SetTitle('Relative S1 Signal vs R^{2}')
		d_cathode_data[cathode_setting]['g_s1_r'].SetMarkerSize(0)
		d_cathode_data[cathode_setting]['g_s1_r'].GetYaxis().SetRangeUser(relMin, relMax)
		d_cathode_data[cathode_setting]['g_s1_r'].GetXaxis().SetTitle('R^{2}')
		d_cathode_data[cathode_setting]['g_s1_r'].GetYaxis().SetTitle('Relative S1 Signal')
		d_cathode_data[cathode_setting]['g_s1_r'].SetLineColor(color)
		if l_first_draw[3]:
			d_cathode_data[cathode_setting]['g_s1_r'].Draw('ap')
			l_first_draw[3] = False
		else:
			d_cathode_data[cathode_setting]['g_s1_r'].Draw('same p')

		d_cathode_data[cathode_setting]['f_s1_r'] = root.TF1('f_s1_r_%f' % (cathode_setting), 'pol1', rMin, rMax)
		d_cathode_data[cathode_setting]['f_s1_r'].SetLineColor(color)

		if not use_mcmc:
			d_cathode_data[cathode_setting]['g_s1_r'].Fit('f_s1_r_%f' % (cathode_setting), 'QLLEMN')
			
			d_cathode_data[cathode_setting]['s1_r_intercept'] = d_cathode_data[cathode_setting]['f_s1_r'].GetParameter(0)
			d_cathode_data[cathode_setting]['s1_r_intercept_err'] = d_cathode_data[cathode_setting]['f_s1_r'].GetParError(0)
			d_cathode_data[cathode_setting]['s1_r_slope'] = d_cathode_data[cathode_setting]['f_s1_r'].GetParameter(1)
			d_cathode_data[cathode_setting]['s1_r_slope_err'] = d_cathode_data[cathode_setting]['f_s1_r'].GetParError(1)
		else:
			trace = None
			y = a_combined_tree['S1']/meanS1
			with pm.Model() as model:
				alpha = pm.Uniform('alpha', lower=-1, upper=1)
				beta = pm.Uniform('beta', lower=-1, upper=1)
				sigma = pm.Uniform('sigma', lower=0, upper=1)
				
				y_est = alpha + beta*a_combined_tree['R2']
				
				likelihood = pm.Normal('y', mu=y_est, sd=sigma, observed=y)
				
				start = pm.find_MAP()
				step = pm.NUTS(state=start)
				trace = pm.sample(3000, step, start=start, progressbar=True, njobs=4)
				
				#pm.summary(trace)
				
				f, ax = plt.subplots(3, 2)
				pm.traceplot(trace, ax=ax)
				plt.show()
				
				# use median and median absolute deviation for
				# representative value and uncertainty
				
				d_cathode_data[cathode_setting]['s1_r_intercept'] = np.median(trace.alpha)
				d_cathode_data[cathode_setting]['s1_r_intercept_err'] = neriX_analysis.mad(trace.alpha)
				d_cathode_data[cathode_setting]['s1_r_slope'] = np.median(trace.beta)
				d_cathode_data[cathode_setting]['s1_r_slope_err'] = neriX_analysis.mad(trace.beta)

				d_cathode_data[cathode_setting]['f_s1_r'].SetParameters(d_cathode_data[cathode_setting]['s1_r_intercept'], d_cathode_data[cathode_setting]['s1_r_slope'])


		d_cathode_data[cathode_setting]['f_s1_r'].Draw('same')


		d_fits[cathode_setting]['s1_r_intercept'] = d_cathode_data[cathode_setting]['s1_r_intercept']
		d_fits[cathode_setting]['s1_r_intercept_err'] = d_cathode_data[cathode_setting]['s1_r_intercept_err']
		d_fits[cathode_setting]['s1_r_slope'] = d_cathode_data[cathode_setting]['s1_r_slope']
		d_fits[cathode_setting]['s1_r_slope_err'] = d_cathode_data[cathode_setting]['s1_r_slope_err']

		d_cathode_data[cathode_setting]['tpt_s1_r_%f' % (cathode_setting)] = root.TPaveText(.15,.85-movement,.7,.9-movement,'blNDC')
		d_cathode_data[cathode_setting]['tpt_s1_r_%f' % (cathode_setting)].SetTextColor(color)
		d_cathode_data[cathode_setting]['tpt_s1_r_%f' % (cathode_setting)].SetFillStyle(0)
		d_cathode_data[cathode_setting]['tpt_s1_r_%f' % (cathode_setting)].SetBorderSize(0)
		d_cathode_data[cathode_setting]['tpt_s1_r_%f' % (cathode_setting)].AddText('V_{c} = %skV: intercept = %.2e +/- %.2e, slope = %.2e +/- %.2e' % (cathode_setting, d_cathode_data[cathode_setting]['s1_r_intercept'], d_cathode_data[cathode_setting]['s1_r_intercept_err'], d_cathode_data[cathode_setting]['s1_r_slope'], d_cathode_data[cathode_setting]['s1_r_slope_err']))
		d_cathode_data[cathode_setting]['tpt_s1_r_%f' % (cathode_setting)].Draw('same')


		cS1.Update()
		



		cS2.cd(1)

		d_cathode_data[cathode_setting]['h_s2_dt_nn'] = Hist2D(dtNumBins, dtMin, dtMax, numBinsProfile, s2Min, s2Max, name='h_s2_dt_nn_%f' % (cathode_setting), title='S2 vs. dt')
		current_analysis.Draw('(dt-%f):%s' % (neriX_datasets.d_dt_offset_gate[run_number], s2_par), hist=d_cathode_data[cathode_setting]['h_s2_dt_nn'])
		d_cathode_data[cathode_setting]['h_s2_dt_nn'].Sumw2()

		# first fit unnormalized S2 spectrum

		d_cathode_data[cathode_setting]['tuple_nn'] = neriX_analysis.profile_y_median(d_cathode_data[cathode_setting]['h_s2_dt_nn'], percent_from_median=percent_from_median)
		d_cathode_data[cathode_setting]['g_s2_dt_nn'] = root.TGraphAsymmErrors(*d_cathode_data[cathode_setting]['tuple_nn'])
		d_cathode_data[cathode_setting]['g_s2_dt_nn'].SetTitle('S2 vs. dt (adjusted for gate time)')
		d_cathode_data[cathode_setting]['g_s2_dt_nn'].SetMarkerSize(0)
		d_cathode_data[cathode_setting]['g_s2_dt_nn'].GetYaxis().SetRangeUser(s2Min, s2Max)
		d_cathode_data[cathode_setting]['g_s2_dt_nn'].GetXaxis().SetTitle('dt [us after gate]')
		d_cathode_data[cathode_setting]['g_s2_dt_nn'].GetYaxis().SetTitle('S2 [PE]')
		d_cathode_data[cathode_setting]['g_s2_dt_nn'].SetLineColor(color)


		d_cathode_data[cathode_setting]['f_s2_dt_nn'] = root.TF1('f_s2_dt_nn_%f' % (cathode_setting), '[0]*exp(-x/[1])', dtMin, 11.)
		d_cathode_data[cathode_setting]['f_s2_dt_nn'].SetLineColor(color)

		if not use_mcmc:
			d_cathode_data[cathode_setting]['f_s2_dt_nn'].SetParameters(meanS2*1.1, 100.)
			d_cathode_data[cathode_setting]['f_s2_dt_nn'].SetParLimits(0, meanS2, 2*meanS2)
			d_cathode_data[cathode_setting]['f_s2_dt_nn'].SetParLimits(1, 10., 5000.)
			d_cathode_data[cathode_setting]['g_s2_dt_nn'].Fit('f_s2_dt_nn_%f' % (cathode_setting), 'QLEMN')

			d_cathode_data[cathode_setting]['e_lifetime_exp_scale'] = d_cathode_data[cathode_setting]['f_s2_dt_nn'].GetParameter(0)
			d_cathode_data[cathode_setting]['e_lifetime_exp_scale_err'] = d_cathode_data[cathode_setting]['f_s2_dt_nn'].GetParError(0)
			d_cathode_data[cathode_setting]['e_lifetime'] = d_cathode_data[cathode_setting]['f_s2_dt_nn'].GetParameter(1)
			d_cathode_data[cathode_setting]['e_lifetime_err'] = d_cathode_data[cathode_setting]['f_s2_dt_nn'].GetParError(1)


		else:
			trace = None
			y = a_combined_tree['S2']
			with pm.Model() as model:
				alpha = pm.Uniform('alpha', lower=3e5, upper=1e6)
				beta = pm.Uniform('beta', lower=10, upper=800)
				sigma = pm.Uniform('sigma', lower=0, upper=3e5)
				
				# exponential with offset for gate time
				y_est = alpha * np.exp(-(a_combined_tree['dt']-neriX_datasets.d_dt_offset_gate[run_number]) / beta)
				
				likelihood = pm.Normal('y', mu=y_est, sd=sigma, observed=y)
				
				start = pm.find_MAP()
				step = pm.NUTS(state=start)
				trace = pm.sample(3000, step, start=start, progressbar=True, njobs=4)
				
				#pm.summary(trace)
				
				f, ax = plt.subplots(3, 2)
				pm.traceplot(trace, ax=ax)
				plt.show()
				
				# use median and median absolute deviation for
				# representative value and uncertainty
				
				d_cathode_data[cathode_setting]['e_lifetime_exp_scale'] = np.median(trace.alpha)
				d_cathode_data[cathode_setting]['e_lifetime_exp_scale_err'] = neriX_analysis.mad(trace.alpha)
				d_cathode_data[cathode_setting]['e_lifetime'] = np.median(trace.beta)
				d_cathode_data[cathode_setting]['e_lifetime_err'] = neriX_analysis.mad(trace.beta)

				d_cathode_data[cathode_setting]['f_s2_dt_nn'].SetParameters(d_cathode_data[cathode_setting]['e_lifetime_exp_scale'], d_cathode_data[cathode_setting]['e_lifetime'])



		#d_cathode_data[cathode_setting]['f_s2_dt_nn'].Draw('same')


		d_fits[cathode_setting]['e_lifetime_exp_scale'] = d_cathode_data[cathode_setting]['e_lifetime_exp_scale']
		d_fits[cathode_setting]['e_lifetime_exp_scale_err'] = d_cathode_data[cathode_setting]['e_lifetime_exp_scale_err']
		d_fits[cathode_setting]['e_lifetime'] = d_cathode_data[cathode_setting]['e_lifetime']
		d_fits[cathode_setting]['e_lifetime_err'] = d_cathode_data[cathode_setting]['e_lifetime_err']

		# now that histogram is scaled, draw for easy comparison

		d_cathode_data[cathode_setting]['h_s2_dt'] = Hist2D(dtNumBins, dtMin, dtMax, numBinsProfile, normalizedS2Min, normalizedS2Max, name='h_s2_dt_%f' % (cathode_setting), title='S2 vs. dt')
		current_analysis.Draw('(dt-%f):(%s/%f)' % (neriX_datasets.d_dt_offset_gate[run_number], s2_par, d_cathode_data[cathode_setting]['e_lifetime_exp_scale']), hist=d_cathode_data[cathode_setting]['h_s2_dt'])

		d_cathode_data[cathode_setting]['tuple'] = neriX_analysis.profile_y_median(d_cathode_data[cathode_setting]['h_s2_dt'], percent_from_median=percent_from_median)
		d_cathode_data[cathode_setting]['g_s2_dt'] = root.TGraphAsymmErrors(*d_cathode_data[cathode_setting]['tuple'])
		d_cathode_data[cathode_setting]['g_s2_dt'].SetTitle('S2 vs. dt (adjusted for gate time)')
		d_cathode_data[cathode_setting]['g_s2_dt'].SetMarkerSize(0)
		d_cathode_data[cathode_setting]['g_s2_dt'].GetYaxis().SetRangeUser(normalizedS2Min, normalizedS2Max)
		d_cathode_data[cathode_setting]['g_s2_dt'].GetXaxis().SetTitle('dt [us after gate]')
		d_cathode_data[cathode_setting]['g_s2_dt'].GetYaxis().SetTitle('S2 Normalized to Max')
		d_cathode_data[cathode_setting]['g_s2_dt'].SetLineColor(color)

		if l_first_draw[4]:
			d_cathode_data[cathode_setting]['g_s2_dt'].Draw('ap')
			l_first_draw[4] = False
		else:
			d_cathode_data[cathode_setting]['g_s2_dt'].Draw('same p')


		d_cathode_data[cathode_setting]['f_s2_dt'] = root.TF1('f_s2_dt_%f' % (cathode_setting), '[0]*exp(-x/[1])', dtMin, 11.)
		d_cathode_data[cathode_setting]['f_s2_dt'].SetLineColor(color)
		d_cathode_data[cathode_setting]['f_s2_dt'].SetParameters(1., d_cathode_data[cathode_setting]['e_lifetime'])
		#d_cathode_data[cathode_setting]['g_s2_dt'].Fit('f_s2_dt_%f' % (cathode_setting), 'QLEMN')
		d_cathode_data[cathode_setting]['f_s2_dt'].Draw('same')

		d_cathode_data[cathode_setting]['tpt_s2_dt_%f' % (cathode_setting)] = root.TPaveText(.15,.85-movement,.7,.9-movement,'blNDC')
		d_cathode_data[cathode_setting]['tpt_s2_dt_%f' % (cathode_setting)].SetTextColor(color)
		d_cathode_data[cathode_setting]['tpt_s2_dt_%f' % (cathode_setting)].SetFillStyle(0)
		d_cathode_data[cathode_setting]['tpt_s2_dt_%f' % (cathode_setting)].SetBorderSize(0)
		d_cathode_data[cathode_setting]['tpt_s2_dt_%f' % (cathode_setting)].AddText('V_{c} = %skV: electron lifetime = %.2e +/- %.2e us' % (cathode_setting, d_cathode_data[cathode_setting]['e_lifetime'], d_cathode_data[cathode_setting]['e_lifetime_err']))
		d_cathode_data[cathode_setting]['tpt_s2_dt_%f' % (cathode_setting)].Draw('same')




		cS2.cd(2)

		d_cathode_data[cathode_setting]['h_s2_r'] = Hist2D(rNumBins, rMin, rMax, numBinsProfile, relMin, relMax, name='h_s2_r_%f' % (cathode_setting), title='S2 vs. R^{2}')
		current_analysis.Draw('R*R:(%s/%f)' % (s2_par, meanS2), hist=d_cathode_data[cathode_setting]['h_s2_r'])


		d_cathode_data[cathode_setting]['tuple'] = neriX_analysis.profile_y_median(d_cathode_data[cathode_setting]['h_s2_r'], percent_from_median=percent_from_median)
		d_cathode_data[cathode_setting]['g_s2_r'] = root.TGraphAsymmErrors(*d_cathode_data[cathode_setting]['tuple'])
		d_cathode_data[cathode_setting]['g_s2_r'].SetTitle('Relative S2 Signal vs R^{2}')
		d_cathode_data[cathode_setting]['g_s2_r'].SetMarkerSize(0)
		d_cathode_data[cathode_setting]['g_s2_r'].GetYaxis().SetRangeUser(relMin, relMax)
		d_cathode_data[cathode_setting]['g_s2_r'].GetXaxis().SetTitle('R^{2}')
		d_cathode_data[cathode_setting]['g_s2_r'].GetYaxis().SetTitle('Relative S2 Signal')
		d_cathode_data[cathode_setting]['g_s2_r'].SetLineColor(color)
		if l_first_draw[5]:
			d_cathode_data[cathode_setting]['g_s2_r'].Draw('ap')
			l_first_draw[5] = False
		else:
			d_cathode_data[cathode_setting]['g_s2_r'].Draw('same p')

		d_cathode_data[cathode_setting]['f_s2_r'] = root.TF1('f_s2_r_%f' % (cathode_setting), 'pol1', rMin, rMax)
		d_cathode_data[cathode_setting]['f_s2_r'].SetLineColor(color)

		if not use_mcmc:
		
			d_cathode_data[cathode_setting]['g_s2_r'].Fit('f_s2_r_%f' % (cathode_setting), 'QLLEMN')

			d_cathode_data[cathode_setting]['s2_r_intercept'] = d_cathode_data[cathode_setting]['f_s2_r'].GetParameter(0)
			d_cathode_data[cathode_setting]['s2_r_intercept_err'] = d_cathode_data[cathode_setting]['f_s2_r'].GetParError(0)
			d_cathode_data[cathode_setting]['s2_r_slope'] = d_cathode_data[cathode_setting]['f_s2_r'].GetParameter(1)
			d_cathode_data[cathode_setting]['s2_r_slope_err'] = d_cathode_data[cathode_setting]['f_s2_r'].GetParError(1)

		else:
			trace = None
			y = a_combined_tree['S2']/meanS2
			with pm.Model() as model:
				alpha = pm.Uniform('alpha', lower=-1, upper=1)
				beta = pm.Uniform('beta', lower=-1, upper=1)
				sigma = pm.Uniform('sigma', lower=0, upper=1)
				
				y_est = alpha + beta*a_combined_tree['R2']
				
				likelihood = pm.Normal('y', mu=y_est, sd=sigma, observed=y)
				
				start = pm.find_MAP()
				step = pm.NUTS(state=start)
				trace = pm.sample(3000, step, start=start, progressbar=True, njobs=4)
				
				#pm.summary(trace)
				
				f, ax = plt.subplots(3, 2)
				pm.traceplot(trace, ax=ax)
				plt.show()
				
				# use median and median absolute deviation for
				# representative value and uncertainty
				
				d_cathode_data[cathode_setting]['s2_r_intercept'] = np.median(trace.alpha)
				d_cathode_data[cathode_setting]['s2_r_intercept_err'] = neriX_analysis.mad(trace.alpha)
				d_cathode_data[cathode_setting]['s2_r_slope'] = np.median(trace.beta)
				d_cathode_data[cathode_setting]['s2_r_slope_err'] = neriX_analysis.mad(trace.beta)

				d_cathode_data[cathode_setting]['f_s2_r'].SetParameters(d_cathode_data[cathode_setting]['s2_r_intercept'], d_cathode_data[cathode_setting]['s2_r_slope'])



		d_cathode_data[cathode_setting]['f_s2_r'].Draw('same')

		d_fits[cathode_setting]['s2_r_intercept'] = d_cathode_data[cathode_setting]['s2_r_intercept']
		d_fits[cathode_setting]['s2_r_intercept_err'] = d_cathode_data[cathode_setting]['s2_r_intercept_err']
		d_fits[cathode_setting]['s2_r_slope'] = d_cathode_data[cathode_setting]['s2_r_slope']
		d_fits[cathode_setting]['s2_r_slope_err'] = d_cathode_data[cathode_setting]['s2_r_slope_err']

		d_cathode_data[cathode_setting]['tpt_s2_r_%f' % (cathode_setting)] = root.TPaveText(.15,.85-movement,.7,.9-movement,'blNDC')
		d_cathode_data[cathode_setting]['tpt_s2_r_%f' % (cathode_setting)].SetTextColor(color)
		d_cathode_data[cathode_setting]['tpt_s2_r_%f' % (cathode_setting)].SetFillStyle(0)
		d_cathode_data[cathode_setting]['tpt_s2_r_%f' % (cathode_setting)].SetBorderSize(0)
		d_cathode_data[cathode_setting]['tpt_s2_r_%f' % (cathode_setting)].AddText('V_{c} = %skV: intercept = %.2e +/- %.2e, slope = %.2e +/- %.2e' % (cathode_setting, d_cathode_data[cathode_setting]['s2_r_intercept'], d_cathode_data[cathode_setting]['s2_r_intercept_err'], d_cathode_data[cathode_setting]['s2_r_slope'], d_cathode_data[cathode_setting]['s2_r_slope_err']))
		d_cathode_data[cathode_setting]['tpt_s2_r_%f' % (cathode_setting)].Draw('same')



		cS2.Update()


	if fit_full_run:
		save_name = 'fit_full_run'

	s_to_return = ''

	s_to_return += '# %s\n' % (save_name)
	if fit_full_run:
		s_to_return += 'd_fit_full_run[%d] = %s' % (run_number, d_fits)
	else:
		s_to_return += 'd_pos_correction_fits[\'' + str(save_name) + '\'] = ' + str(d_fits)
	s_to_return += '\n\n'


	neriX_analysis.save_plot(['results', 'run_%d' % run_number, save_name], cMeans, 's1_s2_%s' % save_name, batch_mode=batch_mode)
	neriX_analysis.save_plot(['results', 'run_%d' % run_number, save_name], cS1, 's1_pos_variation_%s' % save_name, batch_mode=batch_mode)
	neriX_analysis.save_plot(['results', 'run_%d' % run_number, save_name], cS2, 's2_pos_variation_%s' % save_name, batch_mode=batch_mode)

	return s_to_return




if __name__ == '__main__':

	lFiles = []

	lFiles.append(['nerix_160404_1204', 'nerix_160404_1232', 'nerix_160404_1259', 'nerix_160404_1325', 'nerix_160404_1350'])
	lFiles.append(['nerix_160411_0612', 'nerix_160411_0644', 'nerix_160411_0712', 'nerix_160411_0739', 'nerix_160411_0925'])
	"""
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


