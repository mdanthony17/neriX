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
import matplotlib.dates as mdates
import matplotlib.patches as mpatches

from rootpy.plotting import root2matplotlib as rplt

import emcee, corner, click
import neriX_analysis, neriX_datasets
from rootpy.plotting import Hist2D, Hist, Legend, Canvas
from rootpy.tree import Tree
import numpy as np
import tqdm, time, copy_reg, types, pickle, datetime
from root_numpy import tree2array, array2tree
import scipy.optimize as op
import scipy.special
import scipy.misc


num_steps_to_include = 500
num_dim = 2
num_walkers = 16

index_bot_mean = 0
index_bot_width = 1
l_percentiles = [16., 50., 84.]

s_time_dependence_plots_dir = './time_dependence/'
if not os.path.exists(s_time_dependence_plots_dir):
    os.makedirs(s_time_dependence_plots_dir)

# get list of everything in directory
l_dir_names = os.listdir('./results/')
l_dir_names.sort()

# select only names that start with nerix
l_timestamps = []
for dir_name in l_dir_names:
    if dir_name[:2] == '16':
        l_timestamps.append(dir_name)


d_mpe = {}
d_mpe['l_gain_800V_value'] = []
d_mpe['l_gain_800V_uncertainty'] = []
d_mpe['l_power_value'] = []
d_mpe['l_power_uncertainty'] = []

l_time = []
l_datetime = []



for timestamp in l_timestamps:
    s_path_to_chain = './results/%s/sampler_dictionary.p' % (timestamp)

    if os.path.exists(s_path_to_chain):
        d_sampler = pickle.load(open(s_path_to_chain, 'r'))
        l_chains = []
        for sampler in d_sampler[num_walkers]:
            l_chains.append(sampler['_chain'])

        if len(l_chains) == 0:
            continue

        if len(l_chains) > 1:
            a_sampler = np.concatenate(l_chains, axis=1)
        else:
            a_sampler = l_chains[0]

        print 'Successfully loaded sampler!'
    else:
        print sPathToFile
        print 'Could not find file!'
        sys.exit()
    
    a_sampler = a_sampler[:, -num_steps_to_include:, :num_dim].reshape((-1, num_dim))


    # check that chain is long enough
    if a_sampler.shape[0] < num_steps_to_include:
        print '\nChain not long enough for %s\n\n' % (filename)
        continue

    d_mpe['l_gain_800V_value'].append(np.mean(a_sampler[:,0]))
    d_mpe['l_gain_800V_uncertainty'].append(np.var(a_sampler[:,0])**0.5)
    d_mpe['l_power_value'].append(np.mean(a_sampler[:,1]))
    d_mpe['l_power_uncertainty'].append(np.var(a_sampler[:,1])**0.5)


    l_time.append(neriX_analysis.convert_name_to_unix_time('nerix_%s_1200' % (timestamp)))
    l_datetime.append(datetime.datetime.fromtimestamp(l_time[-1]))



d_mpe['l_gain_800V_value'] = np.asarray(d_mpe['l_gain_800V_value'])
d_mpe['l_gain_800V_uncertainty'] = np.asarray(d_mpe['l_gain_800V_uncertainty'])
d_mpe['l_power_value'] = np.asarray(d_mpe['l_power_value'])
d_mpe['l_power_uncertainty'] = np.asarray(d_mpe['l_power_uncertainty'])
l_time = np.asarray(l_time)



# likelihood function should be the integral
# of the integral of two gaussians sharing one variable
# the first gaussian should be the trial gaussian
# the second gaussian should be the point with statistical uncertainty

def ln_likelihood_gaussian_uncertainty_on_pts(a_parameters, a_means, a_stds):
    fit_mean, fit_std = a_parameters
    
    if fit_std < 0:
        return -np.inf


    exponential_term = -(a_means-fit_mean)**2 / (a_stds**2 + fit_std**2) / 2.
    return np.sum(exponential_term - np.log(a_stds) - np.log(fit_std) - 1./2.*np.log(1./a_stds**2+1./fit_std**2) - 1./2.*np.log(2*np.pi))




"""
def neg_log_likelihood_means_diff_ev(a_guesses):
    return -ln_likelihood_gaussian_uncertainty_on_pts(l_gas_gain_mean_median, l_gas_gain_mean_mad, *a_guesses)

def neg_log_likelihood_widths_diff_ev(a_guesses):
    return -ln_likelihood_gaussian_uncertainty_on_pts(l_gas_gain_width_median, l_gas_gain_width_mad, *a_guesses)

a_bounds = [(10, 20), (1e-3, 5)]
result_mean = op.differential_evolution(neg_log_likelihood_means_diff_ev, a_bounds, disp=True, maxiter=100, tol=0.01, polish=True)
print result_mean
"""


num_steps = 2000
steps_to_keep = 1000

l_guesses_mean = [1.5e9, 1.3e7]
l_stds_mean = [3e8, 3e6]
l_labels_mean = ['mean_of_gain_800V', 'std_of_gain_800V']
a_starting_pos_mean = emcee.utils.sample_ball(l_guesses_mean, l_stds_mean, size=num_walkers)

l_guesses_width = [11.45, 5.5e-2]
l_stds_width = [0.3, 1e-2]
l_labels_width = ['mean_of_power', 'std_of_power']
a_starting_pos_width = emcee.utils.sample_ball(l_guesses_width, l_stds_width, size=num_walkers)


d_mpe['sampler_gain_800V'] = emcee.DESampler(num_walkers, 2, ln_likelihood_gaussian_uncertainty_on_pts, threads=1, autoscale_gamma=True, args=[d_mpe['l_gain_800V_value'], d_mpe['l_gain_800V_uncertainty']])

with click.progressbar(d_mpe['sampler_gain_800V'].sample(a_starting_pos_mean, iterations=num_steps, ), length=num_steps) as mcmc_sampler:
    for pos, lnprob, state in mcmc_sampler:
        pass

d_mpe['a_samples_gain_800V'] = d_mpe['sampler_gain_800V'].chain[:, -steps_to_keep:, :2].reshape((-1, 2))

d_mpe['mean_for_gain_800V'], d_mpe['std_for_gain_800V'] = np.percentile(d_mpe['a_samples_gain_800V'], 50., axis=0)




d_mpe['sampler_power'] = emcee.DESampler(num_walkers, 2, ln_likelihood_gaussian_uncertainty_on_pts, threads=1, autoscale_gamma=True, args=[d_mpe['l_power_value'], d_mpe['l_power_uncertainty']])

with click.progressbar(d_mpe['sampler_power'].sample(a_starting_pos_width, iterations=num_steps, ), length=num_steps) as mcmc_sampler:
    for pos, lnprob, state in mcmc_sampler:
        pass

d_mpe['a_samples_power'] = d_mpe['sampler_power'].chain[:, -steps_to_keep:, :2].reshape((-1, 2))

d_mpe['mean_for_power'], d_mpe['std_for_power'] = np.percentile(d_mpe['a_samples_power'], 50., axis=0)


# make corner plots for each fit
d_mpe['fig_for_gain_800V'] = corner.corner(d_mpe['a_samples_gain_800V'], labels=l_labels_mean, quantiles=[0.16, 0.5, 0.84], show_titles=True, title_fmt='.3e', title_kwargs={"fontsize": 8})
d_mpe['fig_for_gain_800V'].savefig('%sposterior_gain_800V.png' % (s_time_dependence_plots_dir))

d_mpe['fig_for_power'] = corner.corner(d_mpe['a_samples_power'], labels=l_labels_width, quantiles=[0.16, 0.5, 0.84], show_titles=True, title_fmt='.3e', title_kwargs={"fontsize":8})
d_mpe['fig_for_power'].savefig('%sposterior_power.png' % (s_time_dependence_plots_dir))



fig1, ax1 = plt.subplots(1)
fig2, ax2 = plt.subplots(1)
points_for_lines = 50

start_time = min(l_datetime) - datetime.timedelta(days=1)
stop_time = max(l_datetime) + datetime.timedelta(days=1)
start_time = mdates.date2num(start_time)
stop_time = mdates.date2num(stop_time)

l_legend_handles = []

x_line = np.linspace(start_time, stop_time, points_for_lines)
x_line = mdates.num2date(x_line)

y_line_gain_800V_mean = np.asarray([d_mpe['mean_for_gain_800V'] for i in xrange(points_for_lines)])
y_line_gain_800V_lb = np.asarray([d_mpe['mean_for_gain_800V']-d_mpe['std_for_gain_800V'] for i in xrange(points_for_lines)])
y_line_gain_800V_ub = np.asarray([d_mpe['mean_for_gain_800V']+d_mpe['std_for_gain_800V'] for i in xrange(points_for_lines)])

y_line_power_mean = np.asarray([d_mpe['mean_for_power'] for i in xrange(points_for_lines)])
y_line_power_lb = np.asarray([d_mpe['mean_for_power']-d_mpe['std_for_power'] for i in xrange(points_for_lines)])
y_line_power_ub = np.asarray([d_mpe['mean_for_power']+d_mpe['std_for_power'] for i in xrange(points_for_lines)])


ax1.errorbar(l_datetime, d_mpe['l_gain_800V_value'], yerr=d_mpe['l_gain_800V_uncertainty'], color='b', marker='o', linestyle='')
ax1.plot(x_line, y_line_gain_800V_mean, color='b', linestyle='--')
ax1.fill_between(x_line, y_line_gain_800V_lb, y_line_gain_800V_ub, color='b', alpha=0.2)

ax2.errorbar(l_datetime, d_mpe['l_power_value'], yerr=d_mpe['l_power_uncertainty'], color='b', marker='o', linestyle='')
ax2.plot(x_line, y_line_power_mean, color='b', linestyle='--')
ax2.fill_between(x_line, y_line_power_lb, y_line_power_ub, color='b', alpha=0.2)


s_gain_800V = r'PMT Gain at 800 V = %.2e +/- %.2e [$e^{-}$]' % (d_mpe['mean_for_gain_800V'], d_mpe['std_for_gain_800V'])
s_power = r'Power = %.2e +/- %.2e' % (d_mpe['mean_for_power'],  d_mpe['std_for_power'])

ax1.text(0.5, 0.9, s_gain_800V, ha='center', va='center', transform=ax1.transAxes)
ax2.text(0.5, 0.9, s_power, ha='center', va='center', transform=ax2.transAxes)


#ax1.legend(handles=l_legend_handles, title='Cathode Voltages')
ax1.set_title('PMT Gain at 800 V - Run 16 Average')
ax1.set_ylabel(r'PMT Gain at 800 V [$e^-$]')
#ax1.set_ylim(12.8, 15)
fig1.autofmt_xdate()

#ax2.legend(handles=l_legend_handles, title='Cathode Voltages')
ax2.set_title('Power in PMT Power Law - Run 16 Average')
ax2.set_ylabel('Power')
ax2.set_ylim(11.38, 11.50)
fig2.autofmt_xdate()


fig1.savefig('%ssummary_gain_800V.png' % (s_time_dependence_plots_dir))
fig2.savefig('%ssummary_power.png' % (s_time_dependence_plots_dir))
#plt.show()
