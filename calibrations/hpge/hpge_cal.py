#!/usr/bin/python
import matplotlib
matplotlib.use('QT4Agg')
import ROOT as root
from ROOT import gROOT
import sys, os, root_numpy, threading, random, emcee, corner, signal, re
from rootpy import stl
from rootpy.io import File
from rootpy.tree import Tree, TreeModel, TreeChain
from rootpy.plotting import Hist, Hist2D, Canvas, Graph, func
import numpy as np
import neriX_analysis
from math import exp, factorial, erf, ceil, log, pow, floor
from scipy import optimize, misc, stats
from scipy.stats import norm
import copy_reg, types, pickle, click, time
from subprocess import call
from tqdm import tqdm



hpge_branch = 'GeHeight[0]'
width_from_trial_fit = 0.7
percentage_from_guess_for_trial = 0.02
s_fit_pars = 'NQLR'



# set up canvas for appropriate number of peaks
c_width = 600
c_height = 500




def fit_hpge(run, d_files, batch_mode=False):
	# d_files should have the format d_files[source] = filename_no_ext

	path_to_data = '/Users/Matt/Desktop/Xenon/neriX/data/run_%d' % run
	
	
	if batch_mode:
		gROOT.SetBatch(True)


	d_hpge = {}

	# Cs137
	d_hpge['cs137'] = {}
	d_hpge['cs137']['filename'] = d_files['cs137']
	d_hpge['cs137']['file'] = File('%s/%s.root' % (path_to_data, d_hpge['cs137']['filename']), 'read')
	d_hpge['cs137']['tree'] = d_hpge['cs137']['file'].T1
	d_hpge['cs137']['peak_energy'] = 661.657
	d_hpge['cs137']['guess_voltage'] = 0.951
	d_hpge['cs137']['binning_voltage'] = [200, 0.94, 0.96]


	# Na22
	# CANNOT USE NA22 FOR RESOLUTION BC OF DOPPLER BROADENING
	d_hpge['na22_511keV'] = {}
	d_hpge['na22_511keV']['filename'] = d_files['na22_511keV']
	d_hpge['na22_511keV']['file'] = File('%s/%s.root' % (path_to_data, d_hpge['na22_511keV']['filename']), 'read')
	d_hpge['na22_511keV']['tree'] = d_hpge['na22_511keV']['file'].T1
	d_hpge['na22_511keV']['peak_energy'] = 511.
	d_hpge['na22_511keV']['guess_voltage'] = 0.735
	d_hpge['na22_511keV']['binning_voltage'] = [200, 0.72, 0.75]


	d_hpge['na22_1270keV'] = {}
	d_hpge['na22_1270keV']['filename'] = d_files['na22_511keV']
	d_hpge['na22_1270keV']['file'] = d_hpge['na22_511keV']['file']
	d_hpge['na22_1270keV']['tree'] = d_hpge['na22_511keV']['tree']
	d_hpge['na22_1270keV']['peak_energy'] = 1274.537
	d_hpge['na22_1270keV']['guess_voltage'] = 1.836
	d_hpge['na22_1270keV']['binning_voltage'] = [200, 1.82, 1.85]





	# arrays for graphs
	a_energy = np.zeros(len(d_hpge))
	a_energy_err = np.zeros(len(d_hpge))
	a_mean = np.zeros(len(d_hpge))
	a_mean_err = np.zeros(len(d_hpge))
	a_width = np.zeros(len(d_hpge))
	a_width_err = np.zeros(len(d_hpge))

	# make sure energies are in order
	l_sources = []
	l_energies = []
	for source in d_hpge:
		l_sources.append(source)
		l_energies.append(d_hpge[source]['peak_energy'])
	l_energies, l_sources = zip(*sorted(zip(l_energies, l_sources)))


	for source_iteration, source in enumerate(l_sources):
		d_hpge[source]['canvas'] = Canvas(c_width, c_height)
		
		# create histogram
		d_hpge[source]['hist'] = Hist(*d_hpge[source]['binning_voltage'], name='h_hpge_height_%s' % (source), title='HPGe Height - %s' % (source))
		d_hpge[source]['hist'].SetMarkerSize(0)
		d_hpge[source]['hist'].SetStats(0)
		d_hpge[source]['hist'].GetXaxis().SetTitle('HPGe Voltage [V]')
		d_hpge[source]['hist'].GetYaxis().SetTitle('Counts')
		
		# cut such that only keep pts in bounds
		s_cut = '%s > %f && %s < %f' % (hpge_branch, d_hpge[source]['binning_voltage'][1], hpge_branch, d_hpge[source]['binning_voltage'][2])
		d_hpge[source]['tree'].Draw(hpge_branch, hist=d_hpge[source]['hist'], selection=s_cut)

		# preliminary fit
		d_hpge[source]['prelim_fit'] = root.TF1('f_prelim_%s' % (source), 'gaus', (1-percentage_from_guess_for_trial)*d_hpge[source]['guess_voltage'], (1+percentage_from_guess_for_trial)*d_hpge[source]['guess_voltage'])
		d_hpge[source]['hist'].Fit('f_prelim_%s' % (source), s_fit_pars)

		trial_mean = d_hpge[source]['prelim_fit'].GetParameter(1)
		trial_width = d_hpge[source]['prelim_fit'].GetParameter(2)

		lb_fit = trial_mean - width_from_trial_fit*trial_width
		ub_fit = trial_mean + width_from_trial_fit*trial_width

		# now with lb and ub fit the peak again for keeps
		d_hpge[source]['fit'] = root.TF1('f_%s' % (source), 'gaus', lb_fit, ub_fit)
		d_hpge[source]['fit'].SetLineColor(root.kBlue)
		d_hpge[source]['fit'].SetLineStyle(2)
		d_hpge[source]['hist'].Fit('f_%s' % (source), s_fit_pars)
		
		d_hpge[source]['hist'].Draw()
		d_hpge[source]['fit'].Draw('same')
		
		
		
		s_current_mean = '#mu = %.2e #pm %.2e keV' % (d_hpge[source]['fit'].GetParameter(1), d_hpge[source]['fit'].GetParError(1))
		s_current_width = '#sigma = %.2e #pm %.2e keV' % (d_hpge[source]['fit'].GetParameter(2), d_hpge[source]['fit'].GetParError(2))
		d_hpge[source]['tpt'] = root.TPaveText(.15,.65,.4,.75,'blNDC')
		d_hpge[source]['tpt'].AddText(s_current_mean)
		d_hpge[source]['tpt'].AddText(s_current_width)
		d_hpge[source]['tpt'].SetTextColor(root.kBlack)
		d_hpge[source]['tpt'].SetFillStyle(0)
		d_hpge[source]['tpt'].SetBorderSize(0)
		d_hpge[source]['tpt'].Draw('same')
		
		

		d_hpge[source]['canvas'].Update()

		d_hpge[source]['mean'] = d_hpge[source]['fit'].GetParameter(1)
		d_hpge[source]['width'] = d_hpge[source]['fit'].GetParameter(2)

		d_hpge[source]['mean_err'] = d_hpge[source]['fit'].GetParError(1)
		d_hpge[source]['width_err'] = d_hpge[source]['fit'].GetParError(2)

		a_energy[source_iteration] = d_hpge[source]['peak_energy']
		a_mean[source_iteration] = d_hpge[source]['mean']
		a_mean_err[source_iteration] = d_hpge[source]['mean_err']
		a_width[source_iteration] = d_hpge[source]['width']
		a_width_err[source_iteration] = d_hpge[source]['width_err']



	c_energy_cal = Canvas(c_width, c_height)

	g_energy_cal = root.TGraphErrors(len(d_hpge), a_mean, a_energy, a_mean_err, a_energy_err)
	g_energy_cal.SetTitle('HPGe Energy Calibration')
	g_energy_cal.GetXaxis().SetTitle('Peak Voltage [V]')
	g_energy_cal.GetYaxis().SetTitle('Energy [keV]')
	g_energy_cal.SetMarkerStyle(32)
	g_energy_cal.Draw('ap')

	f_energy_cal = root.TF1('f_energy_cal', 'pol1', 0, 2)
	f_energy_cal.SetParameters(-50, 600)
	f_energy_cal.SetLineColor(root.kBlue)
	f_energy_cal.SetLineStyle(2)
	frp_energy_cal = g_energy_cal.Fit('f_energy_cal', 'SN')
	f_energy_cal.Draw('same')

	s_energy_cal_par0 = 'Intercept = %.4e #pm %.4e keV' % (f_energy_cal.GetParameter(0), f_energy_cal.GetParError(0))
	s_energy_cal_par1 = 'Slope = %.4e #pm %.4e #frac{keV}{V}' % (f_energy_cal.GetParameter(1), f_energy_cal.GetParError(1))
	tpt_energy_cal = root.TPaveText(.2,.67,.55,.75,'blNDC')
	tpt_energy_cal.AddText(s_energy_cal_par0)
	tpt_energy_cal.AddText(s_energy_cal_par1)
	tpt_energy_cal.SetTextColor(root.kBlack)
	tpt_energy_cal.SetFillStyle(0)
	tpt_energy_cal.SetBorderSize(0)
	tpt_energy_cal.Draw('same')

	c_energy_cal.Update()

	filename_for_save = d_hpge['cs137']['filename']
	cal_intercept = f_energy_cal.GetParameter(0)
	cal_slope = f_energy_cal.GetParameter(1)
	measurement_time = neriX_analysis.convert_name_to_unix_time(filename_for_save)
	
	l_fit_pars = [f_energy_cal.GetParameter(0), f_energy_cal.GetParameter(1)]
	a_cov_matrix = np.asarray(root_numpy.matrix(frp_energy_cal.GetCovarianceMatrix()))
	
	# want list for dictionary so convert numpy array to list
	l_cov_matrix = list(a_cov_matrix)
	for i, sublist in enumerate(l_cov_matrix):
		l_cov_matrix[i] = list(sublist)
	#print str(l_cov_matrix)

	d_fit_results = {'start_time':measurement_time, 'run':run, 'Chi2':frp_energy_cal.Chi2(), 'NDF':frp_energy_cal.Ndf(), 'l_best_fit_pars':l_fit_pars, 'l_cov_matrix':l_cov_matrix}
	s_fit_results = '# %s\nd_fit_results[\'%s\'] = %s\n\n' % (filename_for_save, filename_for_save, str(d_fit_results))


	neriX_analysis.save_plot(['results', filename_for_save], c_energy_cal, 'hpge_energy_cal_function', batch_mode=batch_mode)

	for source in d_hpge:
		neriX_analysis.save_plot(['results', filename_for_save], d_hpge[source]['canvas'], 'hpge_spec_%s' % (source), batch_mode=batch_mode)

	#raw_input('Press enter to continue')

	return s_fit_results


if __name__ == '__main__':
	# only need cs137 and 511 keV peak of Na22 (will copy for 1270)
	
	# different voltage before 9/17/14
	l_files = []

	"""
	run = 10
	l_files.append({'cs137':'nerix_140917_1218', 'na22_511keV':'nerix_140917_1224'})
	l_files.append({'cs137':'nerix_140924_1352', 'na22_511keV':'nerix_140924_1403'})
	l_files.append({'cs137':'nerix_141001_1157', 'na22_511keV':'nerix_141001_1204'})
	#l_files.append({'cs137':'nerix_141008_1141', 'na22_511keV':'nerix_141008_1210'}) # no data
	l_files.append({'cs137':'nerix_141015_1400', 'na22_511keV':'nerix_141015_1427'})
	l_files.append({'cs137':'nerix_141022_1357', 'na22_511keV':'nerix_141022_1407'})
	l_files.append({'cs137':'nerix_141029_1251', 'na22_511keV':'nerix_141029_1258'})
	"""
	
	
	run = 11
	l_files.append({'cs137':'nerix_141119_1355', 'na22_511keV':'nerix_141119_1359'})
	l_files.append({'cs137':'nerix_141128_1303', 'na22_511keV':'nerix_141128_1320'})
	l_files.append({'cs137':'nerix_141203_1432', 'na22_511keV':'nerix_141203_1443'})
	l_files.append({'cs137':'nerix_141210_1403', 'na22_511keV':'nerix_141210_1411'})
	l_files.append({'cs137':'nerix_141217_1334', 'na22_511keV':'nerix_141217_1344'})
	l_files.append({'cs137':'nerix_141219_1506', 'na22_511keV':'nerix_141219_1517'})
	l_files.append({'cs137':'nerix_141219_1555', 'na22_511keV':'nerix_141219_1547'})
	l_files.append({'cs137':'nerix_141222_1412', 'na22_511keV':'nerix_141222_1420'})
	l_files.append({'cs137':'nerix_150107_1641', 'na22_511keV':'nerix_150107_1648'})
	l_files.append({'cs137':'nerix_150114_1728', 'na22_511keV':'nerix_150114_1738'})
	l_files.append({'cs137':'nerix_150121_1332', 'na22_511keV':'nerix_150121_1352'})
	l_files.append({'cs137':'nerix_150204_1420', 'na22_511keV':'nerix_150204_1428'})
	#l_files.append({'cs137':'nerix_150428_1239', 'na22_511keV':'nerix_150428_1248'}) # long afterwards
	
	
	
	s_all_fit_results = ''
	for d_files in l_files:
		s_all_fit_results += fit_hpge(run, d_files, batch_mode=True)

	print s_all_fit_results

