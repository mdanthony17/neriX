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

from sklearn import neighbors
from sklearn import grid_search
from sklearn import preprocessing

num_walkers = 64
num_steps_to_include = 2500
num_steps = 3000
num_events = num_walkers*num_steps_to_include

l_percentiles = [16., 50., 84.]
l_par_names = ['g1', 'eta', 'gas_gain', 'w', 'res_cs137', 'res_na22']
l_colors = ['b', 'r', 'magenta', 'g', 'orange', 'black']
num_dim = 6



# get list of everything in directory
l_dir_names = os.listdir('./results/run_16/')
l_dir_names.sort()

# select only names that start with nerix
l_filenames = []
for dir_name in l_dir_names:
    if dir_name[:2] == 'ne':
        l_filenames.append(dir_name)

d_anticorrelation = {}

d_run_avg_fits = {}
for par in l_par_names:
    d_run_avg_fits[par] = {}
l_sym_par_names = ['g1', 'w', 'res_cs137', 'res_na22']
l_asym_par_names = ['eta', 'gas_gain']

for i, par_name in enumerate(l_sym_par_names):
    d_run_avg_fits[par_name]['l_mean'] = []
    d_run_avg_fits[par_name]['l_std'] = []
    d_run_avg_fits[par_name]['color'] = l_colors[i]
d_run_avg_fits['g1']['title'] = r'$g_1$'
d_run_avg_fits['g1']['units'] = r'$[\frac{\mathrm{PE}}{\mathrm{Photons}}]$'
d_run_avg_fits['g1']['axis_title'] = d_run_avg_fits['g1']['title'] + ' ' + d_run_avg_fits['g1']['units']
d_run_avg_fits['g1']['guess_mean'] = 0.13
d_run_avg_fits['g1']['guess_std'] = 0.05

d_run_avg_fits['w']['title'] = r'$\mathrm{w-value}$'
d_run_avg_fits['w']['units'] = r'$[\frac{\mathrm{keV}}{\mathrm{Quanta}}]$'
d_run_avg_fits['w']['axis_title'] = d_run_avg_fits['w']['title'] + ' ' + d_run_avg_fits['w']['units']
d_run_avg_fits['w']['guess_mean'] = 13.7
d_run_avg_fits['w']['guess_std'] = 0.2

d_run_avg_fits['res_cs137']['title'] = r'$\sigma_{Cs137}$'
d_run_avg_fits['res_cs137']['units'] = r'$[\mathrm{keV}]$'
d_run_avg_fits['res_cs137']['axis_title'] = d_run_avg_fits['res_cs137']['title'] + ' ' + d_run_avg_fits['res_cs137']['units']
d_run_avg_fits['res_cs137']['guess_mean'] = 30.
d_run_avg_fits['res_cs137']['guess_std'] = 3.

d_run_avg_fits['res_na22']['title'] = r'$\sigma_{Na22}$'
d_run_avg_fits['res_na22']['units'] = r'$[\mathrm{keV}]$'
d_run_avg_fits['res_na22']['axis_title'] = d_run_avg_fits['res_na22']['title'] + ' ' + d_run_avg_fits['res_na22']['units']
d_run_avg_fits['res_na22']['guess_mean'] = 23.
d_run_avg_fits['res_na22']['guess_std'] = 3.

for i, par_name in enumerate(l_asym_par_names):
    d_run_avg_fits[par_name]['l_minus_one_sig'] = []
    d_run_avg_fits[par_name]['l_plus_one_sig'] = []
    d_run_avg_fits[par_name]['l_median'] = []
    d_run_avg_fits[par_name]['color'] = l_colors[i+4]

d_run_avg_fits['eta']['title'] = r'$\eta$'
d_run_avg_fits['eta']['units'] = r''
d_run_avg_fits['eta']['axis_title'] = d_run_avg_fits['eta']['title']

d_run_avg_fits['gas_gain']['title'] = r'$\mathrm{Gas\ Gain}$'
d_run_avg_fits['gas_gain']['units'] = r'$[\mathrm{PE}]$'
d_run_avg_fits['gas_gain']['axis_title'] = d_run_avg_fits['gas_gain']['title'] + ' ' + d_run_avg_fits['gas_gain']['units']


s_path_to_time_dependence = './time_dependence/'
    
if not os.path.exists(s_path_to_time_dependence):
    os.makedirs(s_path_to_time_dependence)

l_time = []
l_filenames.sort()

for filename in tqdm.tqdm(l_filenames):

    d_anticorrelation[filename] = {}

    s_path_to_chain = './results/%s/%s/sampler_dictionary.p' % ('run_16', filename)
    s_path_to_kde = './results/%s/%s/kde/' % ('run_16', filename)
    
    if not os.path.exists(s_path_to_kde):
        os.makedirs(s_path_to_kde)

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
    
    
    a_sampler = a_sampler[:, :, :num_dim].reshape((-1, num_dim))
    a_sampler = a_sampler[-num_steps_to_include*num_walkers:, :]

    # check if we need to make the kde or just load it
    # only make KDE for eta and gas gain
    if not os.path.exists('%skde.p' % (s_path_to_kde)):

        # have to scale first since dimensions span three
        # orders of magnitude!
        scaler = preprocessing.StandardScaler()
        scaler.fit(a_sampler[:,1:3])
        a_scaled_samples = scaler.transform(a_sampler[:,1:3])
        
        #print a_sampler[:,1:3]
        #print a_scaled_samples

        # find the best fit bandwith since this allows us
        # to play with bias vs variance
        grid = grid_search.GridSearchCV(neighbors.KernelDensity(), {'bandwidth':np.linspace(0.01, 2., 20)}, cv=4, verbose=1, n_jobs=4)
        print '\nDetermining best bandwidth...\n'
        grid.fit(a_scaled_samples)

        kde = neighbors.KernelDensity(**grid.best_params_)
        kde.fit(a_scaled_samples)
        a_test = kde.sample(3000)
        a_test_transformed = scaler.inverse_transform(a_test)
        
        # make corner plot for comparison
        fig = corner.corner(a_test_transformed, labels=l_par_names, quantiles=[0.16, 0.5, 0.84], show_titles=True, title_fmt='.3e', title_kwargs={"fontsize": 8})
        fig.savefig('%scorner_g2_kde.png' % (s_path_to_kde))

        pickle.dump(kde, open('%skde.p' % (s_path_to_kde), 'w'))
        pickle.dump(scaler, open('%sscaler.p' % (s_path_to_kde), 'w'))


    d_anticorrelation[filename]['kde_g2'] = pickle.load(open('%skde.p' % (s_path_to_kde), 'r'))
    d_anticorrelation[filename]['scaler_g2'] = pickle.load(open('%sscaler.p' % (s_path_to_kde), 'r'))


    d_anticorrelation[filename]['eta_minus_one_sig'], d_anticorrelation[filename]['eta_median'], d_anticorrelation[filename]['eta_plus_one_sig'] = np.percentile(a_sampler[:, 1], l_percentiles)
    d_anticorrelation[filename]['gas_gain_minus_one_sig'], d_anticorrelation[filename]['gas_gain_median'], d_anticorrelation[filename]['gas_gain_plus_one_sig'] = np.percentile(a_sampler[:, 2], l_percentiles)

    d_anticorrelation[filename]['mean_g1'] = np.mean(a_sampler[:,0])
    d_anticorrelation[filename]['mean_w'] = np.mean(a_sampler[:,3])
    d_anticorrelation[filename]['mean_res_cs137'] = np.mean(a_sampler[:,4])
    d_anticorrelation[filename]['mean_res_na22'] = np.mean(a_sampler[:,5])

    d_anticorrelation[filename]['std_g1'] = np.std(a_sampler[:,0], ddof=1)
    d_anticorrelation[filename]['std_w'] = np.std(a_sampler[:,3], ddof=1)
    d_anticorrelation[filename]['std_res_cs137'] = np.std(a_sampler[:,4], ddof=1)
    d_anticorrelation[filename]['std_res_na22'] = np.std(a_sampler[:,5], ddof=1)

    l_time.append(datetime.datetime.fromtimestamp(neriX_analysis.convert_name_to_unix_time('%s_1200' % (filename))))


    # append to full lists for plots
    for par_name in l_sym_par_names:
        d_run_avg_fits[par_name]['l_mean'].append(d_anticorrelation[filename]['mean_%s' % (par_name)])
        d_run_avg_fits[par_name]['l_std'].append(d_anticorrelation[filename]['std_%s' % (par_name)])

    for par_name in l_asym_par_names:
        d_run_avg_fits[par_name]['l_median'].append(d_anticorrelation[filename]['%s_median' % (par_name)])
        d_run_avg_fits[par_name]['l_minus_one_sig'].append(d_anticorrelation[filename]['%s_median' % (par_name)] - d_anticorrelation[filename]['%s_minus_one_sig' % (par_name)])
        d_run_avg_fits[par_name]['l_plus_one_sig'].append(d_anticorrelation[filename]['%s_plus_one_sig' % (par_name)] - d_anticorrelation[filename]['%s_median' % (par_name)])




# likelihood function should be the integral
# of the integral of two gaussians sharing one variable
# the first gaussian should be the trial gaussian
# the second gaussian should be the point with statistical uncertainty


def ln_likelihood_g2_posterior(a_parameters, d_anticorrelation):
    tot_ln_likelihood = 0
    
    for filename in d_anticorrelation:
        a_parameters_scaled = d_anticorrelation[filename]['scaler_g2'].transform(a_parameters)
        #print a_parameters_scaled
        tot_ln_likelihood += d_anticorrelation[filename]['kde_g2'].score(a_parameters_scaled)

    if not np.isfinite(tot_ln_likelihood):
        return -np.inf
    else:
        return tot_ln_likelihood



start_time = np.min(l_time) - datetime.timedelta(days=5)
stop_time = np.max(l_time) + datetime.timedelta(days=5)
start_time = mdates.date2num(start_time)
stop_time = mdates.date2num(stop_time)
points_for_lines = 50


l_value_guesses_g2 = [0.96, 21.5]
l_std_guesses_g2 = [0.02, 0.5]
num_dim_g2 = len(l_value_guesses_g2)


a_starting_pos_g2 = emcee.utils.sample_ball(l_value_guesses_g2, l_std_guesses_g2, size=num_walkers)

sampler_g2 = emcee.DESampler(num_walkers, num_dim_g2, ln_likelihood_g2_posterior, autoscale_gamma=True, args=[d_anticorrelation], threads=4)

with click.progressbar(sampler_g2.sample(a_starting_pos_g2, iterations=num_steps, ), length=num_steps) as mcmc_sampler:
    for pos, lnprob, state in mcmc_sampler:
        pass

a_samples_g2 = sampler_g2.chain[:, :, :].reshape((-1, num_dim_g2))
a_samples_g2 = a_samples_g2[-num_steps_to_include*num_walkers:, :]

fig_time_dependence_g2 = corner.corner(a_samples_g2, labels=['eta', 'gas_gain'], quantiles=[0.16, 0.5, 0.84], show_titles=True, title_fmt='.3e', title_kwargs={"fontsize": 8})
fig_time_dependence_g2.savefig('%scorner_run_avg_g2.png' % (s_path_to_time_dependence))


d_run_avg_fits['eta']['run_avg_minus_one_sig'], d_run_avg_fits['eta']['run_avg_median'], d_run_avg_fits['eta']['run_avg_plus_one_sig'] = np.percentile(a_samples_g2[:, 0], l_percentiles)
d_run_avg_fits['gas_gain']['run_avg_minus_one_sig'], d_run_avg_fits['gas_gain']['run_avg_median'], d_run_avg_fits['gas_gain']['run_avg_plus_one_sig'] = np.percentile(a_samples_g2[:, 1], l_percentiles)



for par_name in l_asym_par_names:
    d_run_avg_fits[par_name]['fig'], d_run_avg_fits[par_name]['ax'] = plt.subplots(1)
    
    #d_run_avg_fits[par_name]['run_avg_minus_one_sig'] = d_run_avg_fits[par_name]['run_avg_median'] - d_run_avg_fits[par_name]['run_avg_minus_one_sig']
    #d_run_avg_fits[par_name]['run_avg_plus_one_sig'] = d_run_avg_fits[par_name]['run_avg_plus_one_sig'] - d_run_avg_fits[par_name]['run_avg_median']

    d_run_avg_fits[par_name]['ax'].errorbar(l_time, d_run_avg_fits[par_name]['l_median'], yerr=[d_run_avg_fits[par_name]['l_minus_one_sig'], d_run_avg_fits[par_name]['l_plus_one_sig']], color=d_run_avg_fits[par_name]['color'], marker='o', linestyle='')
    
    # plot shaded region for run avg fit
    x_line = np.linspace(start_time, stop_time, points_for_lines)
    x_line = mdates.num2date(x_line)
    y_mean = np.asarray([d_run_avg_fits[par_name]['run_avg_median'] for i in xrange(points_for_lines)])
    y_mean_plus_one_sigma = np.asarray([d_run_avg_fits[par_name]['run_avg_plus_one_sig'] for i in xrange(points_for_lines)])
    y_mean_minus_one_sigma = np.asarray([d_run_avg_fits[par_name]['run_avg_minus_one_sig'] for i in xrange(points_for_lines)])
    
    d_run_avg_fits[par_name]['ax'].plot(x_line, y_mean, color=d_run_avg_fits[par_name]['color'], linestyle='--')
    d_run_avg_fits[par_name]['ax'].fill_between(x_line, y_mean_minus_one_sigma, y_mean_plus_one_sigma, color=d_run_avg_fits[par_name]['color'], alpha=0.1)

    d_run_avg_fits[par_name]['fig'].autofmt_xdate()
    d_run_avg_fits[par_name]['fig'].savefig('%srun_avg_%s.png' % (s_path_to_time_dependence, par_name))


    d_run_avg_fits[par_name]['ax'].set_title('$\mathrm{Run\ Average\ -\ }$' + d_run_avg_fits[par_name]['title'])
    d_run_avg_fits[par_name]['ax'].set_ylabel(d_run_avg_fits[par_name]['axis_title'])

    s_run_avg = d_run_avg_fits[par_name]['title']
    s_run_avg += r'$ = %.2e^{%.2e}_{%.2e}$' % (d_run_avg_fits[par_name]['run_avg_median'], d_run_avg_fits[par_name]['run_avg_plus_one_sig'] - d_run_avg_fits[par_name]['run_avg_median'], d_run_avg_fits[par_name]['run_avg_minus_one_sig'] - d_run_avg_fits[par_name]['run_avg_median'])
    s_run_avg += ' ' + d_run_avg_fits[par_name]['units']
    d_run_avg_fits[par_name]['ax'].text(0.6, 0.95, s_run_avg, ha='center', va='center', transform=d_run_avg_fits[par_name]['ax'].transAxes)

    d_run_avg_fits[par_name]['fig'].autofmt_xdate()
    d_run_avg_fits[par_name]['fig'].savefig('%srun_avg_%s.png' % (s_path_to_time_dependence, par_name))




def ln_likelihood_gaussian_uncertainty_on_pts(a_parameters, a_means, a_stds):
    fit_mean, fit_std = a_parameters
    
    if fit_std < 0:
        return -np.inf


    exponential_term = -(a_means-fit_mean)**2 / (a_stds**2 + fit_std**2) / 2.
    return np.sum(exponential_term - np.log(a_stds) - np.log(fit_std) - 1./2.*np.log(1./a_stds**2+1./fit_std**2) - 1./2.*np.log(2*np.pi))



for par_name in l_sym_par_names:
    d_run_avg_fits[par_name]['fig'], d_run_avg_fits[par_name]['ax'] = plt.subplots(1)

    l_value_guesses = [d_run_avg_fits[par_name]['guess_mean'], d_run_avg_fits[par_name]['guess_std']]
    l_std_guesses = [d_run_avg_fits[par_name]['guess_mean']*0.2, d_run_avg_fits[par_name]['guess_std']*0.2]

    a_starting_pos = emcee.utils.sample_ball(l_value_guesses, l_std_guesses, size=num_walkers)

    sampler = emcee.DESampler(num_walkers, 2, ln_likelihood_gaussian_uncertainty_on_pts, autoscale_gamma=True, args=[np.asarray(d_run_avg_fits[par_name]['l_mean']), np.asarray(d_run_avg_fits[par_name]['l_std'])], threads=1)

    with click.progressbar(sampler.sample(a_starting_pos, iterations=num_steps, ), length=num_steps) as mcmc_sampler:
        for pos, lnprob, state in mcmc_sampler:
            pass

    a_samples = sampler.chain[:, :, :].reshape((-1, 2))
    a_samples = a_samples[-num_steps_to_include*num_walkers:, :]

    d_run_avg_fits[par_name]['fig_corner'] = corner.corner(a_samples, labels=['%s_mean' % (par_name), '%s_std' % (par_name)], quantiles=[0.16, 0.5, 0.84], show_titles=True, title_fmt='.3e', title_kwargs={"fontsize": 8})
    d_run_avg_fits[par_name]['fig_corner'].savefig('%scorner_run_avg_%s.png' % (s_path_to_time_dependence, par_name))

    d_run_avg_fits[par_name]['run_avg_mean'] = np.median(a_samples[:, 0])
    d_run_avg_fits[par_name]['run_avg_std'] = np.median(a_samples[:, 1])


    d_run_avg_fits[par_name]['ax'].errorbar(l_time, d_run_avg_fits[par_name]['l_mean'], yerr=d_run_avg_fits[par_name]['l_std'], color=d_run_avg_fits[par_name]['color'], marker='o', linestyle='')
    
    # plot shaded region for run avg fit
    x_line = np.linspace(start_time, stop_time, points_for_lines)
    x_line = mdates.num2date(x_line)
    y_mean = np.asarray([d_run_avg_fits[par_name]['run_avg_mean'] for i in xrange(points_for_lines)])
    y_mean_plus_one_sigma = np.asarray([d_run_avg_fits[par_name]['run_avg_mean'] + d_run_avg_fits[par_name]['run_avg_std'] for i in xrange(points_for_lines)])
    y_mean_minus_one_sigma = np.asarray([d_run_avg_fits[par_name]['run_avg_mean'] - d_run_avg_fits[par_name]['run_avg_std'] for i in xrange(points_for_lines)])
    
    d_run_avg_fits[par_name]['ax'].plot(x_line, y_mean, color=d_run_avg_fits[par_name]['color'], linestyle='--')
    d_run_avg_fits[par_name]['ax'].fill_between(x_line, y_mean_minus_one_sigma, y_mean_plus_one_sigma, color=d_run_avg_fits[par_name]['color'], alpha=0.1)

    d_run_avg_fits[par_name]['ax'].set_title('$\mathrm{Run\ Average\ -\ }$' + d_run_avg_fits[par_name]['title'])
    d_run_avg_fits[par_name]['ax'].set_ylabel(d_run_avg_fits[par_name]['axis_title'])

    s_run_avg = d_run_avg_fits[par_name]['title']
    s_run_avg += r'$ = %.2e \pm %.2e$' % (d_run_avg_fits[par_name]['run_avg_mean'], d_run_avg_fits[par_name]['run_avg_std'])
    s_run_avg += ' ' + d_run_avg_fits[par_name]['units']
    d_run_avg_fits[par_name]['ax'].text(0.6, 0.95, s_run_avg, ha='center', va='center', transform=d_run_avg_fits[par_name]['ax'].transAxes)

    d_run_avg_fits[par_name]['fig'].autofmt_xdate()
    d_run_avg_fits[par_name]['fig'].savefig('%srun_avg_%s.png' % (s_path_to_time_dependence, par_name))



