#!/usr/bin/python
import matplotlib
matplotlib.use('QT4Agg')
import ROOT as root
from ROOT import gROOT
import sys, os, root_numpy, threading, random, emcee, corner, signal, re
import neriX_analysis
from rootpy import stl
from rootpy.io import File
from rootpy.tree import Tree, TreeModel, TreeChain
from rootpy.plotting import Hist, Hist2D, Canvas, Graph, func
import numpy as np
from math import exp, factorial, erf, ceil, log, pow, floor
from scipy import optimize, misc, stats
from scipy.stats import norm
import copy_reg, types, pickle, click, time
from subprocess import call
from tqdm import tqdm

root.TVirtualFitter.SetMaxIterations(10000)

"""
with open('./sim_inputs.txt', 'r') as f_sim_input:
	s_sim_input = f_sim_input.read()

#print s_sim_input

# find all matches to the reg exp in file
r_exp = re.compile('S1\s\d+.\d+')
l_matches_found = re.findall(r_exp, s_sim_input)
#print l_matches_found


# output is in the form of "S1\t%f"
# where %f is a float
# can extract float by starting at 3rd index
a_sim_inputs = np.zeros(len(l_matches_found))
for i in xrange(len(l_matches_found)):
	a_sim_inputs[i] = float(l_matches_found[i][3:])
#print a_sim_inputs
"""

f_inputs = File('./wfsim_pre_processing.root')
t_inputs = f_inputs.tree_out
a_sim_inputs = root_numpy.tree2array(tree=t_inputs, branches='S1')
print a_sim_inputs

# fill histograms as appropriate
s1_settings_small_window = [400, 0, 15]
s1_settings_large_window = [800, 0, 40]
rel_diff_settings = [100, -0.5, 0.5]


gain_adjustment_term = 1. #1.4e6/8.85e5
s1_branch = 'S1sTotBottom[0]*%f' % gain_adjustment_term

# adjust S1 inputs for appropriate gain
a_sim_inputs *= gain_adjustment_term

h_sim_inputs = Hist(*s1_settings_small_window, name='h_sim_inputs')
h_sim_inputs.fill_array(a_sim_inputs)

#h_processed_outputs = Hist(*s1_settings, name='h_processed_outputs')
#h_processed_outputs.SetColor(root.kRed)

h_pf_found = Hist(*s1_settings_small_window, name='h_pf_found')
h_pf_found.SetColor(root.kBlue)

h_pf_smearing = Hist2D(*(s1_settings_large_window+rel_diff_settings), name='h_pf_found', title='Peak Finder Smearing')
h_pf_smearing.SetStats(0)
h_pf_smearing.GetXaxis().SetTitle('Simulated PE [PE]')
h_pf_smearing.GetYaxis().SetTitle('#frac{Simulated PE - Reconstructed PE}{Simulated PE}')

f_processed_outputs = File('./nerix_111111_1112.root', 'r')
t1 = f_processed_outputs.T1
t2 = f_processed_outputs.T2
t2.AddFriend(t1)

#t2.Scan('EventId:S1sTotBottom[0]:S1sPeak[0]')

a_processed_outputs = root_numpy.tree2array(tree=t2, branches=['EventId', 'S1sTotBottom', 'S1sPeak'])

# inputs and outputs should have the same length
# although some output "events" should be empty
assert len(a_processed_outputs) == len(a_sim_inputs)

a_input_for_rel_distance = np.zeros(len(a_processed_outputs))
a_rel_distance_from_input = np.zeros(len(a_processed_outputs))
count_filled_events = 0

for i in tqdm(xrange(len(a_sim_inputs))):
	input_value = a_sim_inputs[i]
	
	l_current_output = a_processed_outputs[i]
	a_s1 = l_current_output[1]*gain_adjustment_term
	s1_peak = l_current_output[2]
	#print input_value,
	if len(a_s1) == 0 or (s1_peak < 898 or s1_peak > 902):
		continue
	else:
		h_pf_found.Fill(input_value)
		h_pf_smearing.Fill(input_value, (input_value-a_s1[0])/input_value)

		a_input_for_rel_distance[count_filled_events] = input_value
		a_rel_distance_from_input[count_filled_events] = (input_value-a_s1[0])/input_value
		count_filled_events += 1


a_input_for_rel_distance = a_input_for_rel_distance[:count_filled_events]
a_rel_distance_from_input = a_rel_distance_from_input[:count_filled_events]

#t2.Draw(s1_branch, hist=h_processed_outputs, selection='%s > 0.005' % (s1_branch))

c1 = Canvas()
#h_pf_found.Draw()
#h_sim_inputs.Draw('same')

h_pf_efficiency = root.TEfficiency(h_pf_found, h_sim_inputs)
h_pf_efficiency.SetTitle('S1 Peak Finder Efficiency; Simulated PE [PE]; Percentage Found by Peak Finder');

g_pf_efficiency = h_pf_efficiency.CreateGraph()
g_pf_efficiency.GetXaxis().SetRangeUser(s1_settings_small_window[1], s1_settings_small_window[2])
g_pf_efficiency.Draw('AP')

#f_eff = root.TF1('f_eff', '(1. - exp(-(x-[0])/[1]))', 0, 15)
#f_eff.SetParameters(2., 2.5)

f_eff = root.TF1('f_eff', '1./((1. + exp(-(x-[0])/[1])))', 0, 15)
f_eff.SetParameters(2., 2.5)

#f_eff = root.TF1('f_eff', '1./((1. + exp(-(x-[0])/[1]))) * 1./((1. + exp(-(x-[2])/[3])))', s1_settings[1], s1_settings[2])
#f_eff.SetParameters(3., 2.5, 5, 2.5)

# exp of exp is called GOMPERTZ FUNCTIOn
# parameters must be positive
#f_eff = root.TF1('f_eff', 'exp(-[0]*exp(-x*[1]))', s1_settings[1], s1_settings[2])
#f_eff.SetParameters(8., 0.5)

frp_eff = g_pf_efficiency.Fit('f_eff', 'SNRLL')
f_eff.Draw('same')

#a_fit_pars = np.asarray([f_eff.GetParameter(0), f_eff.GetParameter(1), f_eff.GetParameter(2), f_eff.GetParameter(3)])
a_fit_pars = np.asarray([f_eff.GetParameter(0), f_eff.GetParameter(1)])
a_cov_matrix = np.asarray(root_numpy.matrix(frp_eff.GetCovarianceMatrix()))
print a_fit_pars
print a_cov_matrix

"""
def pyfunc_eff(x, center, shape):
	if x < center:
		return 0
	else:
		return 1. - exp(-(x-center)/shape)
"""

def pyfunc_eff(x, center, shape, center_2, shape_2):
	return 1./(1. + exp(-(x-center)/shape)) * 1./(1. + exp(-(x-center_2)/shape_2))

"""
def pyfunc_eff(x, center, shape):
	return np.exp(-center*np.exp(-shape*x))

g_eff_conf_band = neriX_analysis.create_1d_fit_confidence_band(pyfunc_eff, a_fit_pars, a_cov_matrix, s1_settings[1], s1_settings[2], confidence_level=0.68)
g_eff_conf_band.Draw('3 same')
"""


tpt_pf_efficiency = root.TPaveText(.45,.1,.85,.45,'blNDC')
#tpt_pf_efficiency.AddText('#varepsilon_{S1 PF} = e^{-#alpha e^{-#beta S1}}')
tpt_pf_efficiency.AddText('#varepsilon_{S1 PF} = #frac{1}{1 + e^{#frac{-(S1-#alpha)}{#beta}}}')
tpt_pf_efficiency.AddText('#alpha = %.2e +/- %.2e [PE]' % (f_eff.GetParameter(0), f_eff.GetParError(0)))
tpt_pf_efficiency.AddText('#beta = %.2e +/- %.2e [PE]' % (f_eff.GetParameter(1), f_eff.GetParError(1)))
tpt_pf_efficiency.SetTextColor(root.kBlack)
tpt_pf_efficiency.SetFillStyle(0)
tpt_pf_efficiency.SetBorderSize(0)
tpt_pf_efficiency.Draw('same')




c1.Update()


c2 = Canvas()
h_pf_smearing.Draw('colz')
c2.Update()


def ln_likelihood_bias_and_res(a_parameters, a_input_for_rel_distance, a_rel_distance_from_input):

	# a_parameters should consist of 4 parameters
	# 0: bias (mean of gaussian)
	# 1,2,3: parameters for stdev of gaussian
	
	assert len(a_input_for_rel_distance) == len(a_rel_distance_from_input)
	
	if abs(a_parameters[0]) > 0.2 or a_parameters[3] <= 0:
		return -np.inf
	
	a_model_rel_distance_from_input = np.full(len(a_input_for_rel_distance), a_parameters[0])
	a_stdev_of_rel_distance = a_parameters[1] + a_parameters[2]*np.exp(-a_input_for_rel_distance/a_parameters[3])
	
	return np.sum(np.log(1./(2*np.pi)**0.5/np.abs(a_stdev_of_rel_distance)) - 0.5*(a_rel_distance_from_input-a_model_rel_distance_from_input)**2/a_stdev_of_rel_distance**2)



num_walkers = 128
num_steps = 400
num_steps_to_keep = 150
num_dim = 4
starting_values = [0.03, 0.01, 0.25, 5]
par_names = ['bias', 'stdev_const', 'stdev_exp_scale', 'stdev_exp_shape']

sampler = emcee.EnsembleSampler(num_walkers, num_dim, ln_likelihood_bias_and_res, threads=1, kwargs={'a_input_for_rel_distance':a_input_for_rel_distance, 'a_rel_distance_from_input':a_rel_distance_from_input})

starting_pos = emcee.utils.sample_ball(starting_values, [0.1*starting_values[i] for i in xrange(len(starting_values))], size=num_walkers)

with click.progressbar(sampler.sample(starting_pos, iterations=num_steps, ), length=num_steps) as mcmc_sampler:
	for pos, lnprob, state in mcmc_sampler:
		pass

samples = sampler.chain[:, -num_steps_to_keep:, :].reshape((-1, num_dim))


# find best fit values and put them on the resolution plot

distance_from_median = 34.

bias_low, bias_median, bias_high = np.percentile(samples[:, 0], [50.-distance_from_median, 50., 50.+distance_from_median])
bias_low = bias_median - bias_low
bias_high = bias_high - bias_median

stdev_const_low, stdev_const_median, stdev_const_high = np.percentile(samples[:, 1], [50.-distance_from_median, 50., 50.+distance_from_median])
stdev_const_low = stdev_const_median - stdev_const_low
stdev_const_high = stdev_const_high - stdev_const_median

stdev_exp_scale_low, stdev_exp_scale_median, stdev_exp_scale_high = np.percentile(samples[:, 2], [50.-distance_from_median, 50., 50.+distance_from_median])
stdev_exp_scale_low = stdev_exp_scale_median - stdev_exp_scale_low
stdev_exp_scale_high = stdev_exp_scale_high - stdev_exp_scale_median

stdev_exp_shape_low, stdev_exp_shape_median, stdev_exp_shape_high = np.percentile(samples[:, 3], [50.-distance_from_median, 50., 50.+distance_from_median])
stdev_exp_shape_low = stdev_exp_shape_median - stdev_exp_shape_low
stdev_exp_shape_high = stdev_exp_shape_high - stdev_exp_shape_median


#print bias_median, stdev_const_median, stdev_exp_scale_median, stdev_exp_shape_median
f_upper_std_curve = root.TF1('f_upper_std_curve', '[0] + [1] + [2]*exp(-x/[3])', s1_settings_large_window[1], s1_settings_large_window[2])
f_upper_std_curve.SetParameters(bias_median, stdev_const_median, stdev_exp_scale_median, stdev_exp_shape_median)
f_upper_std_curve.SetLineStyle(3)
f_upper_std_curve.Draw('same')


f_lower_std_curve = root.TF1('f_lower_std_curve', '[0] - [1] - [2]*exp(-x/[3])', s1_settings_large_window[1], s1_settings_large_window[2])
f_lower_std_curve.SetParameters(bias_median, stdev_const_median, stdev_exp_scale_median, stdev_exp_shape_median)
f_lower_std_curve.SetLineStyle(3)
f_lower_std_curve.Draw('same')


f_bias = root.TF1('f_bias', '[0]', s1_settings_large_window[1], s1_settings_large_window[2])
f_bias.SetParameter(0, bias_median)
f_bias.SetLineStyle(2)
f_bias.Draw('same')


a_means = np.mean(samples, axis=0)[:4]
a_cov_matrix = np.cov(samples.T)[:4,:4]

a_means = list(a_means)
a_cov_matrix = list(a_cov_matrix)
for i in xrange(4):
	a_cov_matrix[i] = list(a_cov_matrix[i])

print a_means
print a_cov_matrix


c2.Update()



fig = corner.corner(samples, labels=par_names, quantiles=[0.16, 0.5, 0.84], show_titles=True, title_kwargs={'fontsize': 12})
fig.savefig('./results/resolution_fit_corner.png')

neriX_analysis.save_plot(['results'], c1, 's1_pf_efficiency')
neriX_analysis.save_plot(['results'], c2, 's1_pf_resolution')



raw_input('Press enter to continue...')

f_processed_outputs.Close()