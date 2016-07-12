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

path_to_data = '/Users/Matt/Desktop/Xenon/neriX/data/run_11'

hpge_branch = 'GeHeight[0]'
width_from_trial_fit = 0.7
percentage_from_guess_for_trial = 0.02
s_fit_pars = 'NQLR'


d_hpge = {}

# Cs137
d_hpge['cs137'] = {}
d_hpge['cs137']['filename'] = 'nerix_150428_1239'
d_hpge['cs137']['file'] = File('%s/%s.root' % (path_to_data, d_hpge['cs137']['filename']), 'read')
d_hpge['cs137']['tree'] = d_hpge['cs137']['file'].T1
d_hpge['cs137']['peak_energy'] = 661.657
d_hpge['cs137']['guess_voltage'] = 0.951
d_hpge['cs137']['binning_voltage'] = [200, 0.94, 0.96]

# Na22
# CANNOT USE NA22 FOR RESOLUTION BC OF DOPPLER BROADENING
"""
d_hpge['na22_511keV'] = {}
d_hpge['na22_511keV']['filename'] = 'nerix_150428_1248'
d_hpge['na22_511keV']['file'] = File('%s/%s.root' % (path_to_data, d_hpge['na22_511keV']['filename']), 'read')
d_hpge['na22_511keV']['tree'] = d_hpge['na22_511keV']['file'].T1
d_hpge['na22_511keV']['peak_energy'] = 511.
d_hpge['na22_511keV']['guess_voltage'] = 0.735
d_hpge['na22_511keV']['binning_voltage'] = [200, 0.72, 0.75]
"""

d_hpge['na22_1270keV'] = {}
d_hpge['na22_1270keV']['filename'] = 'nerix_150428_1248'
d_hpge['na22_1270keV']['file'] = File('%s/%s.root' % (path_to_data, d_hpge['na22_1270keV']['filename']), 'read')
d_hpge['na22_1270keV']['tree'] = d_hpge['na22_1270keV']['file'].T1
d_hpge['na22_1270keV']['peak_energy'] = 1274.537
d_hpge['na22_1270keV']['guess_voltage'] = 1.836
d_hpge['na22_1270keV']['binning_voltage'] = [200, 1.82, 1.85]

# Co57
d_hpge['co57_122keV'] = {}
d_hpge['co57_122keV']['filename'] = 'nerix_150428_1402'
d_hpge['co57_122keV']['file'] = File('%s/%s.root' % (path_to_data, d_hpge['co57_122keV']['filename']), 'read')
d_hpge['co57_122keV']['tree'] = d_hpge['co57_122keV']['file'].T1
d_hpge['co57_122keV']['peak_energy'] = 122.06
d_hpge['co57_122keV']['guess_voltage'] = 0.1735
d_hpge['co57_122keV']['binning_voltage'] = [200, 0.167, 0.178]

d_hpge['co57_137keV'] = {}
d_hpge['co57_137keV']['filename'] = d_hpge['co57_122keV']['filename']
d_hpge['co57_137keV']['file'] = d_hpge['co57_122keV']['file']
d_hpge['co57_137keV']['tree'] = d_hpge['co57_122keV']['tree']
d_hpge['co57_137keV']['peak_energy'] = 137.
d_hpge['co57_137keV']['guess_voltage'] = 0.194
d_hpge['co57_137keV']['binning_voltage'] = [200, 0.188, 0.198]



# set up canvas for appropriate number of peaks
c_width = 600
c_height = 500


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
g_energy_cal.Fit('f_energy_cal', 'N')
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

cal_slope = f_energy_cal.GetParameter(1)

# convert means and widths from volts to energy
# uncertainty on slope is ~5 orders of magnitude smaller
# than value so approximate
a_width *= cal_slope
a_width_err *= cal_slope

c_sigma = Canvas(c_width, c_height)

g_width = root.TGraphErrors(len(d_hpge), a_energy, a_width, a_energy_err, a_width_err)
g_width.SetTitle('HPGe Width')
g_width.GetXaxis().SetTitle('Energy [keV]')
g_width.GetYaxis().SetTitle('Width [keV]')
g_width.SetMarkerStyle(32)
g_width.Draw('ap')

f_width = root.TF1('f_width', '[0] + [1]*(x)^0.5', 0, 1500)
f_width.SetParameters(0, 0.01)
#f_width = root.TF1('f_width', '[0] + [1]*x + (0.08*2.96/1000.*x)^0.5', 0, 1500)
#f_width.SetParameters(0, 0.00037)
f_width.SetLineColor(root.kBlue)
f_width.SetLineStyle(2)
g_width.Fit('f_width', 'NMELL')
f_width.Draw('same')



s_width_eqn = '#sigma = #alpha + #beta#sqrt{E}'
s_width_par0 = '#alpha = %.2e #pm %.2e keV' % (f_width.GetParameter(0), f_width.GetParError(0))
s_width_par1 = '#beta = %.2e #pm %.2e keV^{#frac{1}{2}}' % (f_width.GetParameter(1), f_width.GetParError(1))
tpt_width = root.TPaveText(.2,.65,.55,.8,'blNDC')
tpt_width.AddText(s_width_eqn)
tpt_width.AddText(s_width_par0)
tpt_width.AddText(s_width_par1)
tpt_width.SetTextColor(root.kBlack)
tpt_width.SetFillStyle(0)
tpt_width.SetBorderSize(0)
tpt_width.Draw('same')


# draw confidence interval
g_conf_interval = root.TGraphAsymmErrors(*neriX_analysis.create_graph_with_confidence_interval_for_fit(g_width, root.TVirtualFitter.GetFitter()))
g_conf_interval.SetLineColor(root.kBlue)
g_conf_interval.SetFillColor(root.kBlue)
g_conf_interval.SetFillStyle(3005)
g_conf_interval.Draw('3 same')


c_sigma.Update()


neriX_analysis.save_plot(['results'], c_energy_cal, 'hpge_energy_cal_function')
neriX_analysis.save_plot(['results'], c_sigma, 'hpge_width_function')

for source in d_hpge:
	neriX_analysis.save_plot(['results'], d_hpge[source]['canvas'], 'hpge_spec_%s' % (source))

raw_input('Press enter to continue')



