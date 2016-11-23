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


num_walkers = 32
num_steps_to_include = 4000
num_events = num_walkers*num_steps_to_include

index_s1_z_intercept = 0
index_s1_z_slope = 1
index_e_lifetime = 1
l_percentiles = [16., 50., 84.]

s_time_dependence_plots_dir = './time_dependence/'
if not os.path.exists(s_time_dependence_plots_dir):
    os.makedirs(s_time_dependence_plots_dir)

# get list of everything in directory
l_dir_names = os.listdir('./results/run_16/')
l_dir_names.sort()

# select only names that start with nerix
l_filenames = []
for dir_name in l_dir_names:
    if dir_name[:2] == '16':
        l_filenames.append(dir_name)

d_anticorrelation = {}




l_time = []

for filename in tqdm.tqdm(l_filenames):

    s_path_to_chain = './results/%s/%s/sampler_dictionary.p' % ('run_16', filename)

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

        #print 'Successfully loaded sampler!'
    else:
        print filename, cathode_setting
        print s_path_to_chain
        print 'Could not find file!'
        break
    
    num_dim = 6
    
    a_sampler = a_sampler[:, :, :num_dim].reshape((-1, num_dim))
    a_sampler = a_sampler[-num_steps_to_include:, :]


    # check that chain is long enough
    if a_sampler.shape[0] < num_events:
        print '\nChain not long enough for %s\n\n' % (filename)
        continue

    if analysis_type == 's1_z':
        s1_z_intercept_minus_one_sigma, s1_z_intercept_median, s1_z_intercept_plus_one_sigma = np.percentile(a_sampler[:, index_s1_z_intercept], l_percentiles)
        d_anticorrelation[cathode_setting]['l_s1_z_intercept_median'].append(s1_z_intercept_median)
        d_anticorrelation[cathode_setting]['l_s1_z_intercept_mad'].append(neriX_analysis.mad(a_sampler[:, index_s1_z_intercept]))

        s1_z_slope_minus_one_sigma, s1_z_slope_median, s1_z_slope_plus_one_sigma = np.percentile(a_sampler[:, index_s1_z_slope], l_percentiles)
        d_anticorrelation[cathode_setting]['l_s1_z_slope_median'].append(s1_z_slope_median)
        d_anticorrelation[cathode_setting]['l_s1_z_slope_mad'].append(neriX_analysis.mad(a_sampler[:, index_s1_z_slope]))

    elif analysis_type == 's2_dt':
        e_lifetime_minus_one_sigma, e_lifetime_median, e_lifetime_plus_one_sigma = np.percentile(a_sampler[:, index_e_lifetime], l_percentiles)
        
        # care about difference so correct plus/minus one sigma
        e_lifetime_minus_one_sigma = e_lifetime_median - e_lifetime_minus_one_sigma
        e_lifetime_plus_one_sigma = e_lifetime_plus_one_sigma - e_lifetime_median
        
        d_anticorrelation[cathode_setting]['l_e_lifetime_median'].append(e_lifetime_median)
        d_anticorrelation[cathode_setting]['l_e_lifetime_minus_one_sigma'].append(e_lifetime_minus_one_sigma)
        d_anticorrelation[cathode_setting]['l_e_lifetime_plus_one_sigma'].append(e_lifetime_plus_one_sigma)
        d_anticorrelation[cathode_setting]['l_e_lifetime_mad'].append(neriX_analysis.mad(a_sampler[:, index_e_lifetime]))

        # don't do for both types or will be twice as long
        # as it should be
        d_anticorrelation[cathode_setting]['l_time'].append(neriX_analysis.convert_name_to_unix_time('nerix_%s_1200' % (filename)))


for cathode_setting in l_cathode_settings:
    for i, unixtime in enumerate(d_anticorrelation[cathode_setting]['l_time']):
        d_anticorrelation[cathode_setting]['l_time'][i] = datetime.datetime.fromtimestamp(unixtime)


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

l_intercept_means = []
l_intercept_stdevs = []
l_slope_means = []
l_slope_stdevs = []

for cathode_setting in l_cathode_settings:
    for i in xrange(len(d_anticorrelation[cathode_setting]['l_e_lifetime_mad'])):
    
        l_intercept_means.append(d_anticorrelation[cathode_setting]['l_s1_z_intercept_median'][i])
        l_intercept_stdevs.append(d_anticorrelation[cathode_setting]['l_s1_z_intercept_mad'][i])

        l_slope_means.append(d_anticorrelation[cathode_setting]['l_s1_z_slope_median'][i])
        l_slope_stdevs.append(d_anticorrelation[cathode_setting]['l_s1_z_slope_mad'][i])


l_intercept_means = np.asarray(l_intercept_means)
l_intercept_stdevs = np.asarray(l_intercept_stdevs)
l_slope_means = np.asarray(l_slope_means)
l_slope_stdevs = np.asarray(l_slope_stdevs)


num_steps = 10000
steps_to_keep = 6000*num_walkers

l_guesses_slope = [-0.01, 0.003]
l_stds_slope = [0.005, 0.0005]
l_labels_slope = ['mean_of_s1_z_mean', 'std_of_s1_z_mean']
a_starting_pos_slope = emcee.utils.sample_ball(l_guesses_slope, l_stds_slope, size=num_walkers)

l_guesses_intercept = [0.85, 0.1]
l_stds_intercept = [0.1, 0.03]
l_labels_intercept = ['mean_of_s1_z_intercept', 'std_of_s1_z_intercept']
a_starting_pos_intercept = emcee.utils.sample_ball(l_guesses_intercept, l_stds_intercept, size=num_walkers)



# mean of gas gain
sampler_intercept = emcee.DESampler(num_walkers, 2, ln_likelihood_gaussian_uncertainty_on_pts, threads=1, autoscale_gamma=True, args=[l_intercept_means, l_intercept_stdevs])

with click.progressbar(sampler_intercept.sample(a_starting_pos_intercept, iterations=num_steps, ), length=num_steps) as mcmc_sampler:
    for pos, lnprob, state in mcmc_sampler:
        pass

a_samples_intercept = sampler_intercept.chain[:, :, :2].reshape((-1, 2))
a_samples_intercept = a_samples_intercept[-steps_to_keep:, :]

intercept_mean, intercept_stdev = np.mean(a_samples_intercept, axis=0)


sampler_slope = emcee.DESampler(num_walkers, 2, ln_likelihood_gaussian_uncertainty_on_pts, threads=1, autoscale_gamma=True, args=[l_slope_means, l_slope_stdevs])

with click.progressbar(sampler_slope.sample(a_starting_pos_slope, iterations=num_steps, ), length=num_steps) as mcmc_sampler:
    for pos, lnprob, state in mcmc_sampler:
        pass

a_samples_slope = sampler_slope.chain[:, :, :2].reshape((-1, 2))
a_samples_slope = a_samples_slope[-steps_to_keep:, :]

slope_mean, slope_stdev = np.mean(a_samples_slope, axis=0)


# make corner plots for each fit
fig_intercept = corner.corner(a_samples_intercept, labels=l_labels_intercept, quantiles=[0.16, 0.5, 0.84], show_titles=True, title_fmt='.3e', title_kwargs={"fontsize": 8})
fig_intercept.savefig('%sposterior_s1_z_intercept.png' % (s_time_dependence_plots_dir))

fig_slope = corner.corner(a_samples_slope, labels=l_labels_slope, quantiles=[0.16, 0.5, 0.84], show_titles=True, title_fmt='.3e', title_kwargs={"fontsize": 8})
fig_slope.savefig('%sposterior_s1_z_slope.png' % (s_time_dependence_plots_dir))



fig1, ax1 = plt.subplots(1)
fig2, ax2 = plt.subplots(1)
fig3, ax3 = plt.subplots(1)
fig4, ax4 = plt.subplots(1)
points_for_lines = 50

start_time = np.min(d_anticorrelation[0.345]['l_time']) - datetime.timedelta(days=5)
stop_time = np.max(d_anticorrelation[0.345]['l_time']) + datetime.timedelta(days=5)
start_time = mdates.date2num(start_time)
stop_time = mdates.date2num(stop_time)

l_legend_handles = []

for cathode_setting in l_cathode_settings:

    #plot s1 z intercept
    ax1.errorbar(d_anticorrelation[cathode_setting]['l_time'], d_anticorrelation[cathode_setting]['l_s1_z_intercept_median'], yerr=d_anticorrelation[cathode_setting]['l_s1_z_intercept_mad'], color=d_anticorrelation[cathode_setting]['color'], marker='o', linestyle='')
    
    
    # plot shaded region for run avg fit
    x_line = np.linspace(start_time, stop_time, points_for_lines)
    x_line = mdates.num2date(x_line)
    y_mean_for_intercept_line = np.asarray([intercept_mean for i in xrange(points_for_lines)])
    y_mean_plus_one_sigma_for_intercept_line = np.asarray([intercept_mean+intercept_stdev for i in xrange(points_for_lines)])
    y_mean_minus_one_sigma_for_intercept_line = np.asarray([intercept_mean-intercept_stdev for i in xrange(points_for_lines)])
    
    ax1.plot(x_line, y_mean_for_intercept_line, color='magenta', linestyle='--')
    ax1.fill_between(x_line, y_mean_minus_one_sigma_for_intercept_line, y_mean_plus_one_sigma_for_intercept_line, color='magenta', alpha=0.1)
    
    
    
    ax2.errorbar(d_anticorrelation[cathode_setting]['l_time'], d_anticorrelation[cathode_setting]['l_s1_z_slope_median'], yerr=d_anticorrelation[cathode_setting]['l_s1_z_slope_mad'], color=d_anticorrelation[cathode_setting]['color'], marker='o', linestyle='')
    
    # plot shaded region for run avg fit
    x_line = np.linspace(start_time, stop_time, points_for_lines)
    x_line = mdates.num2date(x_line)
    y_mean_for_slope_line = np.asarray([slope_mean for i in xrange(points_for_lines)])
    y_mean_plus_one_sigma_for_slope_line = np.asarray([slope_mean+slope_stdev for i in xrange(points_for_lines)])
    y_mean_minus_one_sigma_for_slope_line = np.asarray([slope_mean-slope_stdev for i in xrange(points_for_lines)])
    
    ax2.plot(x_line, y_mean_for_slope_line, color='magenta', linestyle='--')
    ax2.fill_between(x_line, y_mean_minus_one_sigma_for_slope_line, y_mean_plus_one_sigma_for_slope_line, color='magenta', alpha=0.1)
    
    


    # plot electron lifetime
    ax3.errorbar(d_anticorrelation[cathode_setting]['l_time'], d_anticorrelation[cathode_setting]['l_e_lifetime_median'], yerr=[d_anticorrelation[cathode_setting]['l_e_lifetime_minus_one_sigma'], d_anticorrelation[cathode_setting]['l_e_lifetime_plus_one_sigma']], color=d_anticorrelation[cathode_setting]['color'], marker='o', linestyle='')

    if cathode_setting in [0.345, 0.7, 1.054]:
        ax4.errorbar(d_anticorrelation[cathode_setting]['l_time'], d_anticorrelation[cathode_setting]['l_e_lifetime_median'], yerr=[d_anticorrelation[cathode_setting]['l_e_lifetime_minus_one_sigma'], d_anticorrelation[cathode_setting]['l_e_lifetime_plus_one_sigma']], color=d_anticorrelation[cathode_setting]['color'], marker='o', linestyle='')
    


    # add labels for legends
    d_anticorrelation[cathode_setting]['legend_entry'] = mpatches.Patch(color=d_anticorrelation[cathode_setting]['color'], label=r'%.3f kV' % (cathode_setting))
    l_legend_handles.append(d_anticorrelation[cathode_setting]['legend_entry'])

ax1.legend(handles=l_legend_handles, title='Cathode Voltages', loc='upper center', ncol=3)
ax1.set_title('S1 vs. Z Intercept - Run 16')
ax1.set_ylabel('Intercept')
ax1.set_ylim(0.6, 1.12)
fig1.autofmt_xdate()
s_intercept = r'$\mathrm{Intercept} = %.2e \pm %.2e$' % (intercept_mean, intercept_stdev)
ax1.text(0.7, 0.1, s_intercept, ha='center', va='center', transform=ax1.transAxes)

ax2.legend(handles=l_legend_handles, title='Cathode Voltages', loc='upper center', ncol=3)
ax2.set_title('S1 vs. Z Slope - Run 16')
ax2.set_ylabel('Slope [1/mm]')
ax2.set_ylim(-0.035, 0.008)
fig2.autofmt_xdate()
s_slope = r'$\mathrm{Slope} = %.2e \pm %.2e \frac{1}{\mathrm{mm}}$' % (slope_mean, slope_stdev)
ax2.text(0.7, 0.1, s_slope, ha='center', va='center', transform=ax2.transAxes)

ax3.legend(handles=l_legend_handles, title='Cathode Voltages', loc='upper center', ncol=3)
ax3.set_title('Electron Lifetime - Run 16')
ax3.set_ylabel('Electron Lifetime [us]')
ax3.set_ylim(10, 1e5)
ax3.set_yscale('log', nonposy='clip')
fig3.autofmt_xdate()

ax4.legend(handles=l_legend_handles[:3], title='Cathode Voltages', loc='upper center', ncol=3)
ax4.set_title('Electron Lifetime - Run 16')
ax4.set_ylabel('Electron Lifetime [us]')
ax4.set_ylim(10, 300)
fig4.autofmt_xdate()


fig1.savefig('%ssummary_s1_z_intercept.png' % (s_time_dependence_plots_dir))
fig2.savefig('%ssummary_s1_z_slope.png' % (s_time_dependence_plots_dir))
fig3.savefig('%ssummary_e_lifetime.png' % (s_time_dependence_plots_dir))
fig4.savefig('%ssummary_e_lifetime_zoomed.png' % (s_time_dependence_plots_dir))
#plt.show()
