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


num_walkers = 64
num_steps_to_include = 500
num_dim = 11
num_events = num_walkers*num_steps_to_include

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
l_filenames = []
for dir_name in l_dir_names:
    if dir_name[:3] == 'ner':
        l_filenames.append(dir_name)

d_gas_gain = {}

l_cathode_settings = [0.345, 1.054, 2.356]
for cathode_setting in l_cathode_settings:
    d_gas_gain[cathode_setting] = {}
    
    d_gas_gain[cathode_setting]['l_gas_gain_mean_median'] = []
    d_gas_gain[cathode_setting]['l_gas_gain_mean_plus_one_sigma'] = []
    d_gas_gain[cathode_setting]['l_gas_gain_mean_minus_one_sigma'] = []
    d_gas_gain[cathode_setting]['l_gas_gain_mean_mad'] = []
    
    d_gas_gain[cathode_setting]['l_gas_gain_width_median'] = []
    d_gas_gain[cathode_setting]['l_gas_gain_width_plus_one_sigma'] = []
    d_gas_gain[cathode_setting]['l_gas_gain_width_minus_one_sigma'] = []
    d_gas_gain[cathode_setting]['l_gas_gain_width_mad'] = []
    
    d_gas_gain[cathode_setting]['l_time'] = []


d_gas_gain[0.345]['color'] = 'blue'
d_gas_gain[1.054]['color'] = 'green'
d_gas_gain[2.356]['color'] = 'red'



l_time = []

for filename in l_filenames:
    s_path_to_chain = './results/%s/sampler_dictionary.p' % (filename)
    cathode_setting = neriX_analysis.check_for_file_info(filename)[1]

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
    
    a_sampler = a_sampler[:, :, :num_dim].reshape((-1, num_dim))


    # check that chain is long enough
    if a_sampler.shape[0] < num_events:
        print '\nChain not long enough for %s\n\n' % (filename)
        continue

    bot_mean_minus_one_sigma, bot_mean_median, bot_mean_plus_one_sigma = np.percentile(a_sampler[:, index_bot_mean], l_percentiles)
    d_gas_gain[cathode_setting]['l_gas_gain_mean_median'].append(bot_mean_median)
    d_gas_gain[cathode_setting]['l_gas_gain_mean_minus_one_sigma'].append(bot_mean_minus_one_sigma)
    d_gas_gain[cathode_setting]['l_gas_gain_mean_plus_one_sigma'].append(bot_mean_plus_one_sigma)
    d_gas_gain[cathode_setting]['l_gas_gain_mean_mad'].append(neriX_analysis.mad(a_sampler[:, index_bot_mean]))

    bot_width_minus_one_sigma, bot_width_median, bot_width_plus_one_sigma = np.percentile(a_sampler[:, index_bot_width], l_percentiles)
    d_gas_gain[cathode_setting]['l_gas_gain_width_median'].append(bot_width_median)
    d_gas_gain[cathode_setting]['l_gas_gain_width_minus_one_sigma'].append(bot_width_minus_one_sigma)
    d_gas_gain[cathode_setting]['l_gas_gain_width_plus_one_sigma'].append(bot_width_plus_one_sigma)
    d_gas_gain[cathode_setting]['l_gas_gain_width_mad'].append(neriX_analysis.mad(a_sampler[:, index_bot_width]))

    d_gas_gain[cathode_setting]['l_time'].append(neriX_analysis.convert_name_to_unix_time(filename))

for cathode_setting in l_cathode_settings:
    d_gas_gain[cathode_setting]['l_gas_gain_mean_median'] = np.asarray(d_gas_gain[cathode_setting]['l_gas_gain_mean_median'])
    d_gas_gain[cathode_setting]['l_gas_gain_mean_minus_one_sigma'] = np.asarray(d_gas_gain[cathode_setting]['l_gas_gain_mean_minus_one_sigma'])
    d_gas_gain[cathode_setting]['l_gas_gain_mean_minus_one_sigma'] = d_gas_gain[cathode_setting]['l_gas_gain_mean_median'] - d_gas_gain[cathode_setting]['l_gas_gain_mean_minus_one_sigma']
    d_gas_gain[cathode_setting]['l_gas_gain_mean_plus_one_sigma'] = np.asarray(d_gas_gain[cathode_setting]['l_gas_gain_mean_plus_one_sigma'])
    d_gas_gain[cathode_setting]['l_gas_gain_mean_plus_one_sigma'] = d_gas_gain[cathode_setting]['l_gas_gain_mean_plus_one_sigma'] - d_gas_gain[cathode_setting]['l_gas_gain_mean_median']
    d_gas_gain[cathode_setting]['l_gas_gain_mean_mad'] = np.asarray(d_gas_gain[cathode_setting]['l_gas_gain_mean_mad'])


    d_gas_gain[cathode_setting]['l_gas_gain_width_median'] = np.asarray(d_gas_gain[cathode_setting]['l_gas_gain_width_median'])
    d_gas_gain[cathode_setting]['l_gas_gain_width_minus_one_sigma'] = np.asarray(d_gas_gain[cathode_setting]['l_gas_gain_width_minus_one_sigma'])
    d_gas_gain[cathode_setting]['l_gas_gain_width_minus_one_sigma'] = d_gas_gain[cathode_setting]['l_gas_gain_width_median'] - d_gas_gain[cathode_setting]['l_gas_gain_width_minus_one_sigma']
    d_gas_gain[cathode_setting]['l_gas_gain_width_plus_one_sigma'] = np.asarray(d_gas_gain[cathode_setting]['l_gas_gain_width_plus_one_sigma'])
    d_gas_gain[cathode_setting]['l_gas_gain_width_plus_one_sigma'] = d_gas_gain[cathode_setting]['l_gas_gain_width_plus_one_sigma'] - d_gas_gain[cathode_setting]['l_gas_gain_width_median']
    d_gas_gain[cathode_setting]['l_gas_gain_width_mad'] = np.asarray(d_gas_gain[cathode_setting]['l_gas_gain_width_mad'])


    #d_gas_gain[cathode_setting]['l_time'] = np.asarray(d_gas_gain[cathode_setting]['l_time'])
    for i, unixtime in enumerate(d_gas_gain[cathode_setting]['l_time']):
        d_gas_gain[cathode_setting]['l_time'][i] = datetime.datetime.fromtimestamp(unixtime)


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
steps_to_keep = 1000*num_walkers

l_guesses_mean = [13.5, 0.3]
l_stds_mean = [0.2, 0.05]
l_labels_mean = ['mean_of_gg_mean', 'std_of_gg_mean']
a_starting_pos_mean = emcee.utils.sample_ball(l_guesses_mean, l_stds_mean, size=num_walkers)

l_guesses_width = [5., 0.3]
l_stds_width = [0.2, 0.05]
l_labels_width = ['mean_of_gg_width', 'std_of_gg_width']
a_starting_pos_width = emcee.utils.sample_ball(l_guesses_width, l_stds_width, size=num_walkers)

for cathode_setting in l_cathode_settings:

    # mean of gas gain
    d_gas_gain[cathode_setting]['sampler_gas_gain_mean'] = emcee.DESampler(num_walkers, 2, ln_likelihood_gaussian_uncertainty_on_pts, threads=1, autoscale_gamma=True, args=[d_gas_gain[cathode_setting]['l_gas_gain_mean_median'], d_gas_gain[cathode_setting]['l_gas_gain_mean_mad']])

    with click.progressbar(d_gas_gain[cathode_setting]['sampler_gas_gain_mean'].sample(a_starting_pos_mean, iterations=num_steps, ), length=num_steps) as mcmc_sampler:
        for pos, lnprob, state in mcmc_sampler:
            pass

    d_gas_gain[cathode_setting]['a_samples_gas_gain_mean'] = d_gas_gain[cathode_setting]['sampler_gas_gain_mean'].chain[:, :, :2].reshape((-1, 2))
    d_gas_gain[cathode_setting]['a_samples_gas_gain_mean'] = d_gas_gain[cathode_setting]['a_samples_gas_gain_mean'][-steps_to_keep:, :]

    d_gas_gain[cathode_setting]['mean_for_gg_mean'], d_gas_gain[cathode_setting]['std_for_gg_mean'] = np.percentile(d_gas_gain[cathode_setting]['a_samples_gas_gain_mean'], 50., axis=0)


    # width of gas gain
    d_gas_gain[cathode_setting]['sampler_gas_gain_width'] = emcee.DESampler(num_walkers, 2, ln_likelihood_gaussian_uncertainty_on_pts, threads=1, autoscale_gamma=True, args=[d_gas_gain[cathode_setting]['l_gas_gain_width_median'], d_gas_gain[cathode_setting]['l_gas_gain_width_mad']])

    with click.progressbar(d_gas_gain[cathode_setting]['sampler_gas_gain_width'].sample(a_starting_pos_width, iterations=num_steps, ), length=num_steps) as mcmc_sampler:
        for pos, lnprob, state in mcmc_sampler:
            pass

    d_gas_gain[cathode_setting]['a_samples_gas_gain_width'] = d_gas_gain[cathode_setting]['sampler_gas_gain_width'].chain[:, :, :2].reshape((-1, 2))
    d_gas_gain[cathode_setting]['a_samples_gas_gain_width'] = d_gas_gain[cathode_setting]['a_samples_gas_gain_width'][-steps_to_keep:, :]

    d_gas_gain[cathode_setting]['mean_for_gg_width'], d_gas_gain[cathode_setting]['std_for_gg_width'] = np.percentile(d_gas_gain[cathode_setting]['a_samples_gas_gain_width'], 50., axis=0)


    # make corner plots for each fit
    d_gas_gain[cathode_setting]['fig_for_gg_mean'] = corner.corner(d_gas_gain[cathode_setting]['a_samples_gas_gain_mean'], labels=l_labels_mean, quantiles=[0.16, 0.5, 0.84], show_titles=True, title_fmt='.3e', title_kwargs={"fontsize": 12})
    d_gas_gain[cathode_setting]['fig_for_gg_mean'].savefig('%sposterior_gg_mean_%.3f.png' % (s_time_dependence_plots_dir, cathode_setting))

    d_gas_gain[cathode_setting]['fig_for_gg_width'] = corner.corner(d_gas_gain[cathode_setting]['a_samples_gas_gain_width'], labels=l_labels_width, quantiles=[0.16, 0.5, 0.84], show_titles=True, title_fmt='.3e', title_kwargs={"fontsize": 12})
    d_gas_gain[cathode_setting]['fig_for_gg_width'].savefig('%sposterior_gg_width_%.3f.png' % (s_time_dependence_plots_dir, cathode_setting))


fig1, ax1 = plt.subplots(1)
fig2, ax2 = plt.subplots(1)
points_for_lines = 50

start_time = min(np.min(d_gas_gain[0.345]['l_time']), np.min(d_gas_gain[1.054]['l_time']), np.min(d_gas_gain[2.356]['l_time'])) - datetime.timedelta(days=1)
stop_time = max(np.max(d_gas_gain[0.345]['l_time']), np.max(d_gas_gain[1.054]['l_time']), np.max(d_gas_gain[2.356]['l_time'])) + datetime.timedelta(days=1)
start_time = mdates.date2num(start_time)
stop_time = mdates.date2num(stop_time)

l_legend_handles = []

for cathode_setting in l_cathode_settings:

    #plot gas gain means
    ax1.errorbar(d_gas_gain[cathode_setting]['l_time'], d_gas_gain[cathode_setting]['l_gas_gain_mean_median'], yerr=d_gas_gain[cathode_setting]['l_gas_gain_width_mad'], color=d_gas_gain[cathode_setting]['color'], marker='o', linestyle='')

    x_line = np.linspace(start_time, stop_time, points_for_lines)
    x_line = mdates.num2date(x_line)
    y_mean_for_gg_mean_line = np.asarray([d_gas_gain[cathode_setting]['mean_for_gg_mean'] for i in xrange(points_for_lines)])
    y_mean_plus_one_sigma_for_gg_mean_line = np.asarray([d_gas_gain[cathode_setting]['mean_for_gg_mean']+d_gas_gain[cathode_setting]['std_for_gg_mean'] for i in xrange(points_for_lines)])
    y_mean_minus_one_sigma_for_gg_mean_line = np.asarray([d_gas_gain[cathode_setting]['mean_for_gg_mean']-d_gas_gain[cathode_setting]['std_for_gg_mean'] for i in xrange(points_for_lines)])
    
    ax1.plot(x_line, y_mean_for_gg_mean_line, color=d_gas_gain[cathode_setting]['color'], linestyle='--')
    ax1.fill_between(x_line, y_mean_minus_one_sigma_for_gg_mean_line, y_mean_plus_one_sigma_for_gg_mean_line, color=d_gas_gain[cathode_setting]['color'], alpha=0.2)


    # plot gas gain widths
    ax2.errorbar(d_gas_gain[cathode_setting]['l_time'], d_gas_gain[cathode_setting]['l_gas_gain_width_median'], yerr=d_gas_gain[cathode_setting]['l_gas_gain_width_mad'], color=d_gas_gain[cathode_setting]['color'], marker='o', linestyle='')

    x_line = np.linspace(start_time, stop_time, points_for_lines)
    x_line = mdates.num2date(x_line)
    y_mean_for_gg_width_line = np.asarray([d_gas_gain[cathode_setting]['mean_for_gg_width'] for i in xrange(points_for_lines)])
    y_mean_plus_one_sigma_for_gg_width_line = np.asarray([d_gas_gain[cathode_setting]['mean_for_gg_width']+d_gas_gain[cathode_setting]['std_for_gg_width'] for i in xrange(points_for_lines)])
    y_mean_minus_one_sigma_for_gg_width_line = np.asarray([d_gas_gain[cathode_setting]['mean_for_gg_width']-d_gas_gain[cathode_setting]['std_for_gg_width'] for i in xrange(points_for_lines)])
    
    ax2.plot(x_line, y_mean_for_gg_width_line, color=d_gas_gain[cathode_setting]['color'], linestyle='--')
    ax2.fill_between(x_line, y_mean_minus_one_sigma_for_gg_width_line, y_mean_plus_one_sigma_for_gg_width_line, color=d_gas_gain[cathode_setting]['color'], alpha=0.2)

    # add labels for legends
    d_gas_gain[cathode_setting]['legend_entry'] = mpatches.Patch(color=d_gas_gain[cathode_setting]['color'], label=r'%.3f kV' % (cathode_setting))
    l_legend_handles.append(d_gas_gain[cathode_setting]['legend_entry'])

ax1.legend(handles=l_legend_handles, title='Cathode Voltages')
ax1.set_title('Gas Gain Mean - Run 16 Average')
ax1.set_ylabel('Gas Gain Mean [PE]')
fig1.autofmt_xdate()

ax2.legend(handles=l_legend_handles, title='Cathode Voltages')
ax2.set_title('Gas Gain Width - Run 16 Average')
ax2.set_ylabel('Gas Gain Width [PE]')
fig2.autofmt_xdate()


fig1.savefig('%ssummary_gg_mean.png' % (s_time_dependence_plots_dir))
fig2.savefig('%ssummary_gg_width.png' % (s_time_dependence_plots_dir))
#plt.show()
