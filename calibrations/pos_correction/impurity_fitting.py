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
l_dir_names = os.listdir('./results/')
l_dir_names.sort()

# select only names that start with nerix
l_filenames = []
for dir_name in l_dir_names:
    if dir_name[:2] == '16':
        l_filenames.append(dir_name)

d_pos_correction = {}

l_cathode_settings = [0.345, 0.700, 1.054]
l_analysis_types = ['s2_dt']

for cathode_setting in l_cathode_settings:
    d_pos_correction[cathode_setting] = {}
    
    d_pos_correction[cathode_setting]['l_e_lifetime_median'] = []
    d_pos_correction[cathode_setting]['l_e_lifetime_plus_one_sigma'] = []
    d_pos_correction[cathode_setting]['l_e_lifetime_minus_one_sigma'] = []
    d_pos_correction[cathode_setting]['l_e_lifetime_mad'] = []
    
    d_pos_correction[cathode_setting]['l_e_lifetime_mean'] = []
    d_pos_correction[cathode_setting]['l_e_lifetime_var'] = []
    d_pos_correction[cathode_setting]['l_e_lifetime_stdev'] = []
    
    d_pos_correction[cathode_setting]['l_e_lifetime_stdev_approx'] = []
    
    d_pos_correction[cathode_setting]['l_time'] = []


d_pos_correction[0.345]['color'] = 'blue'
d_pos_correction[0.7]['color'] = 'green'
d_pos_correction[1.054]['color'] = 'y'


#print '\n\n\nDebugging so only looking at first three files\n\n\n'
#l_filenames = l_filenames[:3]

l_time = []


for filename in tqdm.tqdm(l_filenames):
    for cathode_setting in l_cathode_settings:
        for analysis_type in l_analysis_types:
    
            s_path_to_chain = './results/%s/%s/%s_sampler_dictionary.p' % (filename, str(cathode_setting), analysis_type)

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
            
            if analysis_type == 's1_z':
                num_dim = 2
            elif analysis_type == 's2_dt':
                num_dim = 3
            
            a_sampler = a_sampler[:, :, :num_dim].reshape((-1, num_dim))


            # check that chain is long enough
            if a_sampler.shape[0] < num_events:
                print '\nChain not long enough for %s\n\n' % (filename)
                continue


            e_lifetime_minus_one_sigma, e_lifetime_median, e_lifetime_plus_one_sigma = np.percentile(a_sampler[:, index_e_lifetime], l_percentiles)
            
            # care about difference so correct plus/minus one sigma
            e_lifetime_minus_one_sigma = e_lifetime_median - e_lifetime_minus_one_sigma
            e_lifetime_plus_one_sigma = e_lifetime_plus_one_sigma - e_lifetime_median
            
            d_pos_correction[cathode_setting]['l_e_lifetime_median'].append(e_lifetime_median)
            d_pos_correction[cathode_setting]['l_e_lifetime_minus_one_sigma'].append(e_lifetime_minus_one_sigma)
            d_pos_correction[cathode_setting]['l_e_lifetime_plus_one_sigma'].append(e_lifetime_plus_one_sigma)
            d_pos_correction[cathode_setting]['l_e_lifetime_mad'].append(neriX_analysis.mad(a_sampler[:, index_e_lifetime]))
            
            d_pos_correction[cathode_setting]['l_e_lifetime_mean'].append(np.mean(a_sampler[:, index_e_lifetime]))
            d_pos_correction[cathode_setting]['l_e_lifetime_var'].append(np.var(a_sampler[:, index_e_lifetime]))
            d_pos_correction[cathode_setting]['l_e_lifetime_stdev'].append(d_pos_correction[cathode_setting]['l_e_lifetime_var'][-1]**0.5)
            
            # both plus_one and minus_one are distances already!
            d_pos_correction[cathode_setting]['l_e_lifetime_stdev_approx'].append((d_pos_correction[cathode_setting]['l_e_lifetime_plus_one_sigma'][-1] + d_pos_correction[cathode_setting]['l_e_lifetime_minus_one_sigma'][-1])/2.)
            
            
            
            # don't do for both types or will be twice as long
            # as it should be
            d_pos_correction[cathode_setting]['l_time'].append(neriX_analysis.convert_name_to_unix_time('nerix_%s_1200' % (filename)))

l_time = d_pos_correction[0.345]['l_time']

for cathode_setting in l_cathode_settings:
    d_pos_correction[cathode_setting]['l_e_lifetime_median'] = np.asarray(d_pos_correction[cathode_setting]['l_e_lifetime_median'])
    d_pos_correction[cathode_setting]['l_e_lifetime_minus_one_sigma'] = np.asarray(d_pos_correction[cathode_setting]['l_e_lifetime_minus_one_sigma'])
    d_pos_correction[cathode_setting]['l_e_lifetime_plus_one_sigma'] = np.asarray(d_pos_correction[cathode_setting]['l_e_lifetime_plus_one_sigma'])
    d_pos_correction[cathode_setting]['l_e_lifetime_mad'] = np.asarray(d_pos_correction[cathode_setting]['l_e_lifetime_mad'])
    
    d_pos_correction[cathode_setting]['l_e_lifetime_mean'] = np.asarray(d_pos_correction[cathode_setting]['l_e_lifetime_mean'])
    d_pos_correction[cathode_setting]['l_e_lifetime_var'] = np.asarray(d_pos_correction[cathode_setting]['l_e_lifetime_var'])
    d_pos_correction[cathode_setting]['l_e_lifetime_stdev'] = np.asarray(d_pos_correction[cathode_setting]['l_e_lifetime_stdev'])
    
    d_pos_correction[cathode_setting]['l_e_lifetime_stdev_approx'] = np.asarray(d_pos_correction[cathode_setting]['l_e_lifetime_stdev_approx'])
    
    #print d_pos_correction[cathode_setting]['l_e_lifetime_mean']
    #print d_pos_correction[cathode_setting]['l_e_lifetime_stdev']

    for i, unixtime in enumerate(d_pos_correction[cathode_setting]['l_time']):
        d_pos_correction[cathode_setting]['l_time'][i] = datetime.datetime.fromtimestamp(unixtime)


# likelihood function should be the integral
# of the integral of two gaussians sharing one variable
# the first gaussian should be the trial gaussian
# the second gaussian should be the point with statistical uncertainty


def ln_likelihood_gaussian_uncertainty_imputirites(a_parameters, d_pos_correction):
    # l_a_means is the list of means where
    # l_a_means[0] = a_e_lifetime for 345 V
    # ...
    # l_a_stds is the same idea
    
    a_345V_mean, a_345V_stdev, a_700V_mean, a_700V_stdev, a_1054V_mean, a_1054V_stdev = d_pos_correction[0.345]['l_e_lifetime_median'], d_pos_correction[0.345]['l_e_lifetime_stdev_approx'], d_pos_correction[0.700]['l_e_lifetime_median'], d_pos_correction[0.700]['l_e_lifetime_stdev_approx'], d_pos_correction[1.054]['l_e_lifetime_median'], d_pos_correction[1.054]['l_e_lifetime_stdev_approx']
    
    k1_over_k2, k1_over_k3 = a_parameters
    
    if k1_over_k2 <= 0 or k1_over_k3 <= 0:
        return -np.inf
    
    # correct 700V and 1054V using parameters
    a_700V_mean = a_700V_mean/k1_over_k2
    a_700V_stdev = a_700V_stdev/k1_over_k2
    a_1054V_mean = a_1054V_mean/k1_over_k3
    a_1054V_stdev = a_1054V_stdev/k1_over_k3
    

    a_ln_likelihood = -(a_700V_mean-a_345V_mean)**2 / (a_700V_stdev**2 + a_345V_stdev**2) / 2.
    a_ln_likelihood += -np.log(a_700V_stdev) - np.log(a_345V_stdev) - 1./2.*np.log(1./a_700V_stdev**2+1./a_345V_stdev**2) - 1./2.*np.log(2*np.pi)
    a_ln_likelihood += -(a_1054V_mean-a_345V_mean)**2 / (a_1054V_stdev**2 + a_345V_stdev**2) / 2.
    a_ln_likelihood += -np.log(a_1054V_stdev) - np.log(a_345V_stdev) - 1./2.*np.log(1./a_1054V_stdev**2+1./a_345V_stdev**2) - 1./2.*np.log(2*np.pi)
    
    sum_ln_likelihood = np.sum(a_ln_likelihood)
    
    if not np.isfinite(sum_ln_likelihood):
        return -np.inf
    else:
        return np.sum(a_ln_likelihood)



num_steps = 5000
steps_to_keep = 3000*num_walkers

l_guesses = [2., 2.5]
l_stds = [0.4, 0.4]
l_labels = ['k1_over_k2', 'k1_over_k3']
a_starting_pos = emcee.utils.sample_ball(l_guesses, l_stds, size=num_walkers)


sampler = emcee.DESampler(num_walkers, 2, ln_likelihood_gaussian_uncertainty_imputirites, threads=1, autoscale_gamma=True, args=[d_pos_correction])

with click.progressbar(sampler.sample(a_starting_pos, iterations=num_steps, ), length=num_steps) as mcmc_sampler:
    for pos, lnprob, state in mcmc_sampler:
        pass

a_samples = sampler.chain[:, :, :2].reshape((-1, 2))
a_samples = a_samples[-steps_to_keep:, :]

fig_corner = corner.corner(a_samples, labels=l_labels, quantiles=[0.16, 0.5, 0.84], show_titles=True, title_fmt='.3e', title_kwargs={"fontsize": 12})
fig_corner.savefig('%sposterior_relative_attachment_rates.png' % (s_time_dependence_plots_dir))

k1_over_k2_mean, k1_over_k3_mean = np.mean(a_samples, axis=0)
k1_over_k2_var, k1_over_k3_var = np.var(a_samples, axis=0)


a_impurity_conc_mean = np.zeros(len(l_time))
a_impurity_conc_stdev = np.zeros(len(l_time))


for i in xrange(len(l_time)):
    unc_345 = d_pos_correction[0.345]['l_e_lifetime_stdev_approx'][i]
    unc_700 = d_pos_correction[0.700]['l_e_lifetime_median'][i]/k1_over_k2_mean * ( (d_pos_correction[0.700]['l_e_lifetime_stdev_approx'][i]/d_pos_correction[0.700]['l_e_lifetime_median'][i])**2 + k1_over_k2_var/k1_over_k2_mean**2 )**0.5
    unc_1054 = d_pos_correction[1.054]['l_e_lifetime_median'][i]/k1_over_k3_mean * ( (d_pos_correction[1.054]['l_e_lifetime_stdev_approx'][i]/d_pos_correction[1.054]['l_e_lifetime_median'][i])**2 + k1_over_k3_var/k1_over_k3_mean**2 )**0.5
    
    a_impurity_conc_mean[i], a_impurity_conc_stdev[i] = np.average([d_pos_correction[0.345]['l_e_lifetime_median'][i], d_pos_correction[0.700]['l_e_lifetime_median'][i]/k1_over_k2_mean, d_pos_correction[1.054]['l_e_lifetime_median'][i]/k1_over_k3_mean], weights=[unc_345**-2, unc_700**-2, unc_1054**-2], returned=True)
    a_impurity_conc_stdev[i] = a_impurity_conc_stdev[i]**(-1./2.)



# -----------------------------------------------------
# -----------------------------------------------------
#  Now fit for constant electron lifetime
# -----------------------------------------------------
# -----------------------------------------------------

def ln_likelihood_gaussian_uncertainty_on_pts(a_parameters, a_means, a_stds):
    fit_mean, fit_std = a_parameters
    
    if fit_std < 0:
        return -np.inf


    exponential_term = -(a_means-fit_mean)**2 / (a_stds**2 + fit_std**2) / 2.
    return np.sum(exponential_term - np.log(a_stds) - np.log(fit_std) - 1./2.*np.log(1./a_stds**2+1./fit_std**2) - 1./2.*np.log(2*np.pi))


num_steps = 5000
steps_to_keep = 3000*num_walkers

l_guesses = [70, 13]
l_stds = [5, 3]
l_labels = ['mean_impurity_level', 'stdev_of_impurity_level']
a_starting_pos = emcee.utils.sample_ball(l_guesses, l_stds, size=num_walkers)

sampler = emcee.DESampler(num_walkers, 2, ln_likelihood_gaussian_uncertainty_on_pts, threads=1, autoscale_gamma=True, args=[a_impurity_conc_mean, a_impurity_conc_stdev])

with click.progressbar(sampler.sample(a_starting_pos, iterations=num_steps, ), length=num_steps) as mcmc_sampler:
    for pos, lnprob, state in mcmc_sampler:
        pass

a_samples = sampler.chain[:, :, :2].reshape((-1, 2))
a_samples = a_samples[-steps_to_keep:, :]

fig_corner = corner.corner(a_samples, labels=l_labels, quantiles=[0.16, 0.5, 0.84], show_titles=True, title_fmt='.3e', title_kwargs={"fontsize": 12})
fig_corner.savefig('%sposterior_run_avg_e_lifetime.png' % (s_time_dependence_plots_dir))

run_avg_impurity_mean, run_avg_impurity_stdev = np.mean(a_samples, axis=0)


fig1, ax1 = plt.subplots(1)

points_for_lines = 50

start_time = np.min(d_pos_correction[0.345]['l_time']) - datetime.timedelta(days=5)
stop_time = np.max(d_pos_correction[0.345]['l_time']) + datetime.timedelta(days=5)
start_time = mdates.date2num(start_time)
stop_time = mdates.date2num(stop_time)


for cathode_setting in l_cathode_settings:

    #plot gas gain means
    ax1.errorbar(l_time, a_impurity_conc_mean, yerr=a_impurity_conc_stdev, color='blue', marker='o', linestyle='')

    # plot shaded region for run avg fit
    x_line = np.linspace(start_time, stop_time, points_for_lines)
    x_line = mdates.num2date(x_line)
    y_mean_for_impurity_mean_line = np.asarray([run_avg_impurity_mean for i in xrange(points_for_lines)])
    y_mean_plus_one_sigma_for_impurity_mean_line = np.asarray([run_avg_impurity_mean+run_avg_impurity_stdev for i in xrange(points_for_lines)])
    y_mean_minus_one_sigma_for_impurity_mean_line = np.asarray([run_avg_impurity_mean-run_avg_impurity_stdev for i in xrange(points_for_lines)])
    
    ax1.plot(x_line, y_mean_for_impurity_mean_line, color='g', linestyle='--')
    ax1.fill_between(x_line, y_mean_minus_one_sigma_for_impurity_mean_line, y_mean_plus_one_sigma_for_impurity_mean_line, color='g', alpha=0.1)

s_impurity_mean = r'$\frac{1}{[I]^*}=%.2f \pm %.2f \mu s$' % (run_avg_impurity_mean, run_avg_impurity_stdev)
ax1.text(0.7, 0.9, s_impurity_mean, ha='center', va='center', transform=ax1.transAxes)

ax1.set_title('Impurity Concentrations - Run 16')
ax1.set_ylabel(r'$\frac{1}{[I]^*}=\tau_1 [\mu s]$')
ax1.set_xlim(start_time, stop_time)
fig1.autofmt_xdate()

fig1.savefig('%ssummary_impurities.png' % (s_time_dependence_plots_dir))
#plt.show()

def multiplicative_uncertainty_prop(a, a_unc, b, b_unc):
    value = a*b
    unc = value*(a_unc**2/a**2 + b_unc**2/b**2)**0.5
    return value, unc



print 'e_lifetime_345V = %.2f +/- %.2f' % (run_avg_impurity_mean, run_avg_impurity_stdev)
print 'e_lifetime_700V = %.2f +/- %.2f' % (multiplicative_uncertainty_prop(run_avg_impurity_mean, run_avg_impurity_stdev, k1_over_k2_mean, k1_over_k2_var**0.5))
print 'e_lifetime_1054V = %.2f +/- %.2f' % (multiplicative_uncertainty_prop(run_avg_impurity_mean, run_avg_impurity_stdev, k1_over_k3_mean, k1_over_k3_var**0.5))




