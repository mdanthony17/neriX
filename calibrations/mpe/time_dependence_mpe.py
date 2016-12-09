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


num_steps_to_include = 600
num_walkers = 16

l_percentiles = [16., 50., 84.]
l_par_names = ['mpe_gain_800V', 'gain_power'] + ['pol_par0', 'pol_par1', 'pol_par2']
l_colors = ['b', 'r', 'magenta', 'g', 'orange', 'black']

num_dim = len(l_par_names)

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
d_run_avg_fits = {}

l_time = []
l_datetime = []



# relative gains for 530V and 610V
d_rel_gains = {}
d_rel_gains['l_rel_gain_530V_mean'] = []
d_rel_gains['l_rel_gain_530V_std'] = []
d_rel_gains['l_rel_gain_610V_mean'] = []
d_rel_gains['l_rel_gain_610V_std'] = []

def produce_rel_gains_530V_610V(a_samples):
    num_pts = a_samples.shape[0]
    
    a_rel_gain_530V = np.zeros(num_pts)
    a_rel_gain_610V = np.zeros(num_pts)
    for i in xrange(num_pts):
        eff_530V = max(min(a_samples[i,2] + a_samples[i,3]*530. + a_samples[i,4]*530.**2., 1), 0)
        a_rel_gain_530V[i] = (530./800.)**a_samples[i,1] * eff_530V

        eff_610V = max(min(a_samples[i,2] + a_samples[i,3]*610. + a_samples[i,4]*610.**2., 1), 0)
        a_rel_gain_610V[i] = (610./800.)**a_samples[i,1] * eff_610V

    return np.mean(a_rel_gain_530V), np.std(a_rel_gain_530V), np.mean(a_rel_gain_610V), np.std(a_rel_gain_610V)


for i, par_name in enumerate(l_par_names):
    d_run_avg_fits[par_name] = {}
    d_run_avg_fits[par_name]['l_median'] = []
    d_run_avg_fits[par_name]['l_median_minus_one_sigma'] = []
    d_run_avg_fits[par_name]['l_median_plus_one_sigma'] = []
    d_run_avg_fits[par_name]['color'] = l_colors[i]

d_run_avg_fits['mpe_gain_800V']['title'] = r'$g(800V)$'
d_run_avg_fits['mpe_gain_800V']['units'] = r'$[e^-]$'
d_run_avg_fits['mpe_gain_800V']['axis_title'] = d_run_avg_fits['mpe_gain_800V']['title'] + ' ' + d_run_avg_fits['mpe_gain_800V']['units']
d_run_avg_fits['mpe_gain_800V']['guess'] = 1.5e9

d_run_avg_fits['gain_power']['title'] = r'Power Law'
d_run_avg_fits['gain_power']['units'] = r''
d_run_avg_fits['gain_power']['axis_title'] = d_run_avg_fits['gain_power']['title']
d_run_avg_fits['gain_power']['guess'] = 9.56

d_run_avg_fits['pol_par0']['title'] = r'Efficiency Polynomial Par 0'
d_run_avg_fits['pol_par0']['units'] = r''
d_run_avg_fits['pol_par0']['axis_title'] = d_run_avg_fits['pol_par0']['title']
d_run_avg_fits['pol_par0']['guess'] = 7.4e-1

d_run_avg_fits['pol_par1']['title'] = r'Efficiency Polynomial Par 1'
d_run_avg_fits['pol_par1']['units'] = r'$[\frac{1}{V}]$'
d_run_avg_fits['pol_par1']['axis_title'] = d_run_avg_fits['pol_par1']['title'] + ' ' + d_run_avg_fits['pol_par1']['units']
d_run_avg_fits['pol_par1']['guess'] = -2e-3

d_run_avg_fits['pol_par2']['title'] = r'Efficiency Polynomial Par 2'
d_run_avg_fits['pol_par2']['units'] = r'$[\frac{1}{V^2}]$'
d_run_avg_fits['pol_par2']['axis_title'] = d_run_avg_fits['pol_par2']['title'] + ' ' + d_run_avg_fits['pol_par2']['units']
d_run_avg_fits['pol_par2']['guess'] = 2.94e-6




for timestamp in l_timestamps:
    d_mpe[timestamp] = {}

    s_path_to_chain = './results/%s/sampler_dictionary.p' % (timestamp)
    s_path_to_kde = './results/%s/kde/' % (timestamp)
    
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

        print 'Successfully loaded sampler!'
    else:
        print sPathToFile
        print 'Could not find file!'
        sys.exit()
    
    a_sampler = a_sampler[:, -num_steps_to_include:, :num_dim].reshape((-1, num_dim))


    # check if we need to make the kde or just load it
    # only make KDE for eta and gas gain
    if not os.path.exists('%skde.p' % (s_path_to_kde)):

        # have to scale first since dimensions span three
        # orders of magnitude!
        scaler = preprocessing.StandardScaler()
        scaler.fit(a_sampler)
        a_scaled_samples = scaler.transform(a_sampler)
        
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
        fig.savefig('%scorner_kde.png' % (s_path_to_kde))

        pickle.dump(kde, open('%skde.p' % (s_path_to_kde), 'w'))
        pickle.dump(scaler, open('%sscaler.p' % (s_path_to_kde), 'w'))


    d_mpe[timestamp]['kde'] = pickle.load(open('%skde.p' % (s_path_to_kde), 'r'))
    d_mpe[timestamp]['scaler'] = pickle.load(open('%sscaler.p' % (s_path_to_kde), 'r'))


    # run though a_sampler to get rel gains at 530V and 610V
    current_530V_mean, current_530V_std, current_610V_mean, current_610V_std = produce_rel_gains_530V_610V(a_sampler)
    #print current_530V_mean, current_530V_std, current_610V_mean, current_610V_std
    d_rel_gains['l_rel_gain_530V_mean'].append(current_530V_mean)
    d_rel_gains['l_rel_gain_530V_std'].append(current_530V_std)
    d_rel_gains['l_rel_gain_610V_mean'].append(current_610V_mean)
    d_rel_gains['l_rel_gain_610V_std'].append(current_610V_std)


    for i, par_name in enumerate(l_par_names):
        current_low, current_median, current_high = np.percentile(a_sampler[:, i], l_percentiles)
        d_run_avg_fits[par_name]['l_median'].append(current_median)
        d_run_avg_fits[par_name]['l_median_minus_one_sigma'].append(current_low)
        d_run_avg_fits[par_name]['l_median_plus_one_sigma'].append(current_high)


    l_time.append(neriX_analysis.convert_name_to_unix_time('nerix_%s_1200' % (timestamp)))
    l_datetime.append(datetime.datetime.fromtimestamp(l_time[-1]))


# convert lists to numpy arrays
for par_name in l_par_names:
    d_run_avg_fits[par_name]['l_median'] = np.asarray(d_run_avg_fits[par_name]['l_median'])
    d_run_avg_fits[par_name]['l_median_minus_one_sigma'] = np.asarray(d_run_avg_fits[par_name]['l_median_minus_one_sigma'])
    d_run_avg_fits[par_name]['l_median_plus_one_sigma'] = np.asarray(d_run_avg_fits[par_name]['l_median_plus_one_sigma'])



d_rel_gains['l_rel_gain_530V_mean'] = np.asarray(d_rel_gains['l_rel_gain_530V_mean'])
d_rel_gains['l_rel_gain_530V_std'] = np.asarray(d_rel_gains['l_rel_gain_530V_std'])
d_rel_gains['l_rel_gain_610V_mean'] = np.asarray(d_rel_gains['l_rel_gain_610V_mean'])
d_rel_gains['l_rel_gain_610V_std'] = np.asarray(d_rel_gains['l_rel_gain_610V_std'])




start_time = np.min(l_datetime) - datetime.timedelta(days=5)
stop_time = np.max(l_datetime) + datetime.timedelta(days=5)
start_time = mdates.date2num(start_time)
stop_time = mdates.date2num(stop_time)
points_for_lines = 50



def ln_likelihood_kde_posterior(a_parameters, d_mpe):
    tot_ln_likelihood = 0
    
    for filename in d_mpe:
        a_parameters_scaled = d_mpe[filename]['scaler'].transform(a_parameters)
        #print a_parameters_scaled
        tot_ln_likelihood += d_mpe[filename]['kde'].score(a_parameters_scaled)

    if not np.isfinite(tot_ln_likelihood):
        return -np.inf
    else:
        return tot_ln_likelihood



def ln_likelihood_gaussian_uncertainty_on_pts(a_parameters, a_means, a_stds):
    fit_mean, fit_std = a_parameters
    
    if fit_std < 0:
        return -np.inf


    exponential_term = -(a_means-fit_mean)**2 / (a_stds**2 + fit_std**2) / 2.
    return np.sum(exponential_term - np.log(a_stds) - np.log(fit_std) - 1./2.*np.log(1./a_stds**2+1./fit_std**2) - 1./2.*np.log(2*np.pi))



# ------------------------------------------------
# ------------------------------------------------
#  Fit individual parameters
# ------------------------------------------------
# ------------------------------------------------


# common parameters and objects

num_steps = 2000
steps_to_keep = 1000

points_for_lines = 50

start_time = min(l_datetime) - datetime.timedelta(days=1)
stop_time = max(l_datetime) + datetime.timedelta(days=1)
start_time = mdates.date2num(start_time)
stop_time = mdates.date2num(stop_time)

x_line = np.linspace(start_time, stop_time, points_for_lines)
x_line = mdates.num2date(x_line)






"""



l_guesses_mean = []
l_guesses_std = []
for par_name in l_par_names:
    l_guesses_mean.append(d_run_avg_fits[par_name]['guess'])
    l_guesses_std.append(d_run_avg_fits[par_name]['guess']*0.05)

a_starting_pos = emcee.utils.sample_ball(l_guesses_mean, l_guesses_std, size=num_walkers)


run_avg_sampler = emcee.DESampler(num_walkers, num_dim, ln_likelihood_kde_posterior, threads=1, autoscale_gamma=True, args=[d_mpe])

with click.progressbar(run_avg_sampler.sample(a_starting_pos, iterations=num_steps, ), length=num_steps) as mcmc_sampler:
    for pos, lnprob, state in mcmc_sampler:
        pass

run_avg_samples = run_avg_sampler.chain[:, -steps_to_keep:, :num_dim].reshape((-1, num_dim))

fig_corner = corner.corner(run_avg_samples, labels=l_par_names, quantiles=[0.16, 0.5, 0.84], show_titles=True, title_fmt='.3e', title_kwargs={"fontsize": 8})
fig_corner.savefig('%scorner_time_dependence_run_avg.png' % (s_time_dependence_plots_dir))


for i, par_name in enumerate(l_par_names):
    d_run_avg_fits[par_name]['run_avg_median_minus_one_sigma'], d_run_avg_fits[par_name]['run_avg_median'], d_run_avg_fits[par_name]['run_avg_median_plus_one_sigma'] = np.percentile(run_avg_samples[:,i], l_percentiles)
    #print d_run_avg_fits[par_name]['run_avg_median_minus_one_sigma'], d_run_avg_fits[par_name]['run_avg_median'], d_run_avg_fits[par_name]['run_avg_median_plus_one_sigma']




for i, par_name in enumerate(l_par_names):

    d_run_avg_fits[par_name]['fig'], d_run_avg_fits[par_name]['ax'] = plt.subplots(1)

    y_line_median = np.asarray([d_run_avg_fits[par_name]['run_avg_median'] for j in xrange(points_for_lines)])
    y_line_median_minus_one_sigma = np.asarray([d_run_avg_fits[par_name]['run_avg_median_minus_one_sigma'] for j in xrange(points_for_lines)])
    y_line_median_plus_one_sigma = np.asarray([d_run_avg_fits[par_name]['run_avg_median_plus_one_sigma'] for j in xrange(points_for_lines)])

    d_run_avg_fits[par_name]['ax'].errorbar(l_datetime, d_run_avg_fits[par_name]['l_median'], yerr=[d_run_avg_fits[par_name]['l_median'] - d_run_avg_fits[par_name]['l_median_minus_one_sigma'], d_run_avg_fits[par_name]['l_median_plus_one_sigma'] - d_run_avg_fits[par_name]['l_median']], color=d_run_avg_fits[par_name]['color'], linestyle='')
    
    d_run_avg_fits[par_name]['ax'].plot(x_line, y_line_median, color=d_run_avg_fits[par_name]['color'], linestyle='--')
    
    #print x_line, y_line_median_minus_one_sigma, y_line_median_plus_one_sigma
    d_run_avg_fits[par_name]['ax'].fill_between(x_line, y_line_median_minus_one_sigma, y_line_median_plus_one_sigma, color=d_run_avg_fits[par_name]['color'], alpha=0.2)
    
    s_fit = d_run_avg_fits[par_name]['title']
    s_fit += ' = $%.3e^{+%.3e}_{-%.3e}$ ' % (d_run_avg_fits[par_name]['run_avg_median'], abs(d_run_avg_fits[par_name]['run_avg_median']-d_run_avg_fits[par_name]['run_avg_median_plus_one_sigma']), abs(d_run_avg_fits[par_name]['run_avg_median']-d_run_avg_fits[par_name]['run_avg_median_minus_one_sigma']))
    s_fit += d_run_avg_fits[par_name]['units']
    #print s_fit
    
    d_run_avg_fits[par_name]['ax'].text(0.5, 0.9, s_fit, ha='center', va='center', transform=d_run_avg_fits[par_name]['ax'].transAxes)

    d_run_avg_fits[par_name]['fig'].savefig('%ssummary_%s.png' % (s_time_dependence_plots_dir, par_name))

"""
# ------------------------------------------------
# ------------------------------------------------
#  Fit relative gains
# ------------------------------------------------
# ------------------------------------------------

num_dim = 2

#print d_rel_gains['l_rel_gain_530V_mean']
#print d_rel_gains['l_rel_gain_610V_mean']




# BEGIN WITH 530 V

rel_gain_530V_mean_guess = 9.4e-3
rel_gain_530V_std_guess = 1e-4


a_starting_pos_rel_gain_530V = emcee.utils.sample_ball([rel_gain_530V_mean_guess, rel_gain_530V_std_guess], [rel_gain_530V_mean_guess*0.1, rel_gain_530V_std_guess*0.1], size=num_walkers)

rel_gain_530V_sampler = emcee.DESampler(num_walkers, num_dim, ln_likelihood_gaussian_uncertainty_on_pts, threads=1, autoscale_gamma=True, args=[d_rel_gains['l_rel_gain_530V_mean'], d_rel_gains['l_rel_gain_530V_std']])

with click.progressbar(rel_gain_530V_sampler.sample(a_starting_pos_rel_gain_530V, iterations=num_steps, ), length=num_steps) as mcmc_sampler:
    for pos, lnprob, state in mcmc_sampler:
        pass

rel_gain_530V_samples = rel_gain_530V_sampler.chain[:, -steps_to_keep:, :num_dim].reshape((-1, num_dim))

fig_corner_rel_gain_530V = corner.corner(rel_gain_530V_samples, labels=['Relative Gain at 530V Mean', 'Relative Gain at 530V Std'], quantiles=[0.16, 0.5, 0.84], show_titles=True, title_fmt='.3e', title_kwargs={"fontsize": 8})
fig_corner_rel_gain_530V.savefig('%scorner_time_dependence_rel_gain_530V.png' % (s_time_dependence_plots_dir))

run_avg_rel_gain_530V_mean, run_avg_rel_gain_530V_std = np.percentile(rel_gain_530V_samples[:,0], 50), np.percentile(rel_gain_530V_samples[:,1], 50)


fig_530V, ax_530V = plt.subplots(1)

y_line_rel_gain_530V_mean = np.asarray([run_avg_rel_gain_530V_mean for j in xrange(points_for_lines)])
y_line_rel_gain_530V_mean_minus_one_sigma = np.asarray([run_avg_rel_gain_530V_mean - run_avg_rel_gain_530V_std for j in xrange(points_for_lines)])
y_line_rel_gain_530V_mean_plus_one_sigma = np.asarray([run_avg_rel_gain_530V_mean + run_avg_rel_gain_530V_std for j in xrange(points_for_lines)])

ax_530V.errorbar(l_datetime, d_rel_gains['l_rel_gain_530V_mean'], yerr=d_rel_gains['l_rel_gain_530V_std'], color='b', linestyle='', marker='.')

ax_530V.plot(x_line, y_line_rel_gain_530V_mean, color='b', linestyle='--')
ax_530V.fill_between(x_line, y_line_rel_gain_530V_mean_minus_one_sigma, y_line_rel_gain_530V_mean_plus_one_sigma, color='b', alpha=0.2)

s_fit = r'$\frac{g(530V)}{g(800V)} = %.3e \pm %.3e$' % (run_avg_rel_gain_530V_mean, run_avg_rel_gain_530V_std)

ax_530V.text(0.5, 0.9, s_fit, ha='center', va='center', transform=ax_530V.transAxes)

ax_530V.set_title('Relative Gain at 530V - Run 16 Average')
ax_530V.set_ylabel(r'$\frac{g(530V)}{g(800V)}$', labelpad=-3)
fig_530V.autofmt_xdate()

fig_530V.savefig('%ssummary_rel_gain_530V.png' % (s_time_dependence_plots_dir))






# CONTINUE WITH 610V

rel_gain_610V_mean_guess = 4.44e-3
rel_gain_610V_std_guess = 4e-5

a_starting_pos_rel_gain_610V = emcee.utils.sample_ball([rel_gain_610V_mean_guess, rel_gain_610V_std_guess], [rel_gain_610V_mean_guess*0.1, rel_gain_610V_std_guess*0.1], size=num_walkers)

rel_gain_610V_sampler = emcee.DESampler(num_walkers, num_dim, ln_likelihood_gaussian_uncertainty_on_pts, threads=1, autoscale_gamma=True, args=[d_rel_gains['l_rel_gain_610V_mean'], d_rel_gains['l_rel_gain_610V_std']])

with click.progressbar(rel_gain_610V_sampler.sample(a_starting_pos_rel_gain_610V, iterations=num_steps, ), length=num_steps) as mcmc_sampler:
    for pos, lnprob, state in mcmc_sampler:
        pass

rel_gain_610V_samples = rel_gain_610V_sampler.chain[:, -steps_to_keep:, :num_dim].reshape((-1, num_dim))

fig_corner_rel_gain_610V = corner.corner(rel_gain_610V_samples, labels=['Relative Gain at 610V Mean', 'Relative Gain at 610V Std'], quantiles=[0.16, 0.5, 0.84], show_titles=True, title_fmt='.3e', title_kwargs={"fontsize": 8})
fig_corner_rel_gain_610V.savefig('%scorner_time_dependence_rel_gain_610V.png' % (s_time_dependence_plots_dir))

run_avg_rel_gain_610V_mean, run_avg_rel_gain_610V_std = np.percentile(rel_gain_610V_samples[:,0], 50), np.percentile(rel_gain_610V_samples[:,1], 50)


fig_610V, ax_610V = plt.subplots(1)

y_line_rel_gain_610V_mean = np.asarray([run_avg_rel_gain_610V_mean for j in xrange(points_for_lines)])
y_line_rel_gain_610V_mean_minus_one_sigma = np.asarray([run_avg_rel_gain_610V_mean - run_avg_rel_gain_610V_std for j in xrange(points_for_lines)])
y_line_rel_gain_610V_mean_plus_one_sigma = np.asarray([run_avg_rel_gain_610V_mean + run_avg_rel_gain_610V_std for j in xrange(points_for_lines)])

ax_610V.errorbar(l_datetime, d_rel_gains['l_rel_gain_610V_mean'], yerr=d_rel_gains['l_rel_gain_610V_std'], color='b', linestyle='', marker='.')

ax_610V.plot(x_line, y_line_rel_gain_610V_mean, color='b', linestyle='--')
ax_610V.fill_between(x_line, y_line_rel_gain_610V_mean_minus_one_sigma, y_line_rel_gain_610V_mean_plus_one_sigma, color='b', alpha=0.2)

s_fit = r'$\frac{g(610V)}{g(800V)} = %.3e \pm %.3e$' % (run_avg_rel_gain_610V_mean, run_avg_rel_gain_610V_std)

ax_610V.text(0.5, 0.9, s_fit, ha='center', va='center', transform=ax_610V.transAxes)

ax_610V.set_title('Relative Gain at 610V - Run 16 Average')
ax_610V.set_ylabel(r'$\frac{g(610V)}{g(800V)}$', labelpad=-3)
fig_610V.autofmt_xdate()

fig_610V.savefig('%ssummary_rel_gain_610V.png' % (s_time_dependence_plots_dir))


