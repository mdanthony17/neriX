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

num_walkers = 16
num_steps = 10000
num_steps_to_include = 5000

l_percentiles = [16., 50., 84.]
l_cathode_settings = [0.345, 1.054, 2.356]
d_source_energies = {'cs137':661.7, 'na22':511.}
l_sources = d_source_energies.keys()
l_yields = ['ly', 'qy']


l_colors = ['b', 'r', 'magenta', 'g', 'orange', 'black']


# define run avgs
g1_value = 0.117
g1_unc = 0.002
g2_value = 0.895*21.2
g2_unc = g2_value * ((0.002/0.895)**2 + (0.04/21.2)**2)**0.5



# get list of everything in directory
l_dir_names = os.listdir('./results/run_16/')
l_dir_names.sort()

# select only names that start with nerix
l_filenames = []
for dir_name in l_dir_names:
    if dir_name[:2] == 'ne':
        l_filenames.append(dir_name)


d_run_avg_fits = {}
counter = 0
for source in l_sources:
    d_run_avg_fits[source] = {}
    for cathode_setting in l_cathode_settings:
        d_run_avg_fits[source][cathode_setting] = {}

        for yield_type in l_yields:
            d_run_avg_fits[source][cathode_setting][yield_type] = {}

            d_run_avg_fits[source][cathode_setting][yield_type]['color'] = l_colors[counter]
            d_run_avg_fits[source][cathode_setting][yield_type]['l_means'] = []
            d_run_avg_fits[source][cathode_setting][yield_type]['l_stds'] = []

            if source == 'cs137':
                s_source = 'Cs137'
            elif source == 'na22':
                s_source = 'Na22'
    
            if yield_type == 'ly':
                s_units = r'$[\frac{\mathrm{Photons}}{\mathrm{keV}}]$'
                s_yield = r'$L_y$'
            else:
                s_units = r'$[\frac{\mathrm{Electrons}}{\mathrm{keV}}]$'
                s_yield = r'$Q_y$'
                
                
            d_run_avg_fits[source][cathode_setting][yield_type]['title'] = s_yield + ' ' + s_source + ' at %.3f kV' % (cathode_setting)
            d_run_avg_fits[source][cathode_setting][yield_type]['units'] = s_units
            d_run_avg_fits[source][cathode_setting][yield_type]['axis_title'] = d_run_avg_fits[source][cathode_setting][yield_type]['title'] + ' ' + d_run_avg_fits[source][cathode_setting][yield_type]['units']
            d_run_avg_fits[source][cathode_setting][yield_type]['guess_mean'] = 35
            d_run_avg_fits[source][cathode_setting][yield_type]['guess_std'] = 1


        counter += 1

#print d_run_avg_fits

s_path_to_time_dependence = './time_dependence/'
    
if not os.path.exists(s_path_to_time_dependence):
    os.makedirs(s_path_to_time_dependence)

l_time = []
l_filenames.sort()

for filename in tqdm.tqdm(l_filenames):

    s_path_to_yields = './results/%s/%s/yield_info.pkl' % ('run_16', filename)
    

    if os.path.exists(s_path_to_yields):
        d_yield_info = pickle.load(open(s_path_to_yields, 'r'))
    else:
        print filename
        print s_path_to_yields
        print 'Could not find file!'
        break
    
    for source in l_sources:
        for cathode_setting in l_cathode_settings:
            s1_mean_value = d_yield_info[source][cathode_setting]['s1_mean_value']
            s1_mean_unc = d_yield_info[source][cathode_setting]['s1_mean_unc']
            s2_mean_value = d_yield_info[source][cathode_setting]['s2_mean_value']
            s2_mean_unc = d_yield_info[source][cathode_setting]['s2_mean_unc']
            
            for yield_type in l_yields:
                
                if yield_type == 'ly':
                    current_yield_value = s1_mean_value / g1_value / d_source_energies[source]
                    current_yield_unc = current_yield_value * ((s1_mean_unc/s1_mean_value)**2 + (g1_unc/g1_value)**2)**0.5
                else:
                    current_yield_value = s2_mean_value / g2_value / d_source_energies[source]
                    current_yield_unc = current_yield_value * ((s2_mean_unc/s2_mean_value)**2 + (g2_unc/g2_value)**2)**0.5
                
                #print '\n\n'
                #print source, cathode_setting, yield_type
                #print current_yield_value, current_yield_unc
                
            
                d_run_avg_fits[source][cathode_setting][yield_type]['l_means'].append(current_yield_value)
                d_run_avg_fits[source][cathode_setting][yield_type]['l_stds'].append(current_yield_unc)


    l_time.append(datetime.datetime.fromtimestamp(neriX_analysis.convert_name_to_unix_time('%s_1200' % (filename))))




def ln_likelihood_gaussian_uncertainty_on_pts(a_parameters, a_means, a_stds):
    fit_mean, fit_std = a_parameters
    
    if fit_std < 0:
        return -np.inf


    exponential_term = -(a_means-fit_mean)**2 / (a_stds**2 + fit_std**2) / 2.
    return np.sum(exponential_term - np.log(a_stds) - np.log(fit_std) - 1./2.*np.log(1./a_stds**2+1./fit_std**2) - 1./2.*np.log(2*np.pi))


start_time = np.min(l_time) - datetime.timedelta(days=5)
stop_time = np.max(l_time) + datetime.timedelta(days=5)
start_time = mdates.date2num(start_time)
stop_time = mdates.date2num(stop_time)
points_for_lines = 50


for source in l_sources:
    for cathode_setting in l_cathode_settings:
        for yield_type in l_yields:


            d_run_avg_fits[source][cathode_setting][yield_type]['fig'], d_run_avg_fits[source][cathode_setting][yield_type]['ax'] = plt.subplots(1)

            l_value_guesses = [d_run_avg_fits[source][cathode_setting][yield_type]['guess_mean'], d_run_avg_fits[source][cathode_setting][yield_type]['guess_std']]
            l_std_guesses = [d_run_avg_fits[source][cathode_setting][yield_type]['guess_mean']*0.2, d_run_avg_fits[source][cathode_setting][yield_type]['guess_std']*0.2]

            a_starting_pos = emcee.utils.sample_ball(l_value_guesses, l_std_guesses, size=num_walkers)

            sampler = emcee.DESampler(num_walkers, 2, ln_likelihood_gaussian_uncertainty_on_pts, autoscale_gamma=True, args=[np.asarray(d_run_avg_fits[source][cathode_setting][yield_type]['l_means']), np.asarray(d_run_avg_fits[source][cathode_setting][yield_type]['l_stds'])], threads=1)

            with click.progressbar(sampler.sample(a_starting_pos, iterations=num_steps, ), length=num_steps) as mcmc_sampler:
                for pos, lnprob, state in mcmc_sampler:
                    pass

            a_samples = sampler.chain[:, :, :].reshape((-1, 2))
            a_samples = a_samples[-num_steps_to_include*num_walkers:, :]

            d_run_avg_fits[source][cathode_setting][yield_type]['fig_corner'] = corner.corner(a_samples, labels=['%s_%.3f_%s_mean' % (source, cathode_setting, yield_type), '%s_%.3f_%s_std' % (source, cathode_setting, yield_type)], quantiles=[0.16, 0.5, 0.84], show_titles=True, title_fmt='.3e', title_kwargs={"fontsize": 8})
            d_run_avg_fits[source][cathode_setting][yield_type]['fig_corner'].savefig('%scorner_run_avg_%s_%.3f_%s.png' % (s_path_to_time_dependence, source, cathode_setting, yield_type))

            d_run_avg_fits[source][cathode_setting][yield_type]['run_avg_mean'] = np.median(a_samples[:, 0])
            d_run_avg_fits[source][cathode_setting][yield_type]['run_avg_std'] = np.median(a_samples[:, 1])


            d_run_avg_fits[source][cathode_setting][yield_type]['ax'].errorbar(l_time, d_run_avg_fits[source][cathode_setting][yield_type]['l_means'], yerr=d_run_avg_fits[source][cathode_setting][yield_type]['l_stds'], color=d_run_avg_fits[source][cathode_setting][yield_type]['color'], marker='o', linestyle='')
            
            # plot shaded region for run avg fit
            x_line = np.linspace(start_time, stop_time, points_for_lines)
            x_line = mdates.num2date(x_line)
            y_mean = np.asarray([d_run_avg_fits[source][cathode_setting][yield_type]['run_avg_mean'] for i in xrange(points_for_lines)])
            y_mean_plus_one_sigma = np.asarray([d_run_avg_fits[source][cathode_setting][yield_type]['run_avg_mean'] + d_run_avg_fits[source][cathode_setting][yield_type]['run_avg_std'] for i in xrange(points_for_lines)])
            y_mean_minus_one_sigma = np.asarray([d_run_avg_fits[source][cathode_setting][yield_type]['run_avg_mean'] - d_run_avg_fits[source][cathode_setting][yield_type]['run_avg_std'] for i in xrange(points_for_lines)])
            
            d_run_avg_fits[source][cathode_setting][yield_type]['ax'].plot(x_line, y_mean, color=d_run_avg_fits[source][cathode_setting][yield_type]['color'], linestyle='--')
            d_run_avg_fits[source][cathode_setting][yield_type]['ax'].fill_between(x_line, y_mean_minus_one_sigma, y_mean_plus_one_sigma, color=d_run_avg_fits[source][cathode_setting][yield_type]['color'], alpha=0.1)

            d_run_avg_fits[source][cathode_setting][yield_type]['ax'].set_title('$\mathrm{Run\ Average\ -\ }$' + d_run_avg_fits[source][cathode_setting][yield_type]['title'])
            d_run_avg_fits[source][cathode_setting][yield_type]['ax'].set_ylabel(d_run_avg_fits[source][cathode_setting][yield_type]['axis_title'])

            s_run_avg = d_run_avg_fits[source][cathode_setting][yield_type]['title']
            s_run_avg += r'$ = %.2e \pm %.2e$' % (d_run_avg_fits[source][cathode_setting][yield_type]['run_avg_mean'], d_run_avg_fits[source][cathode_setting][yield_type]['run_avg_std'])
            s_run_avg += ' ' + d_run_avg_fits[source][cathode_setting][yield_type]['units']
            d_run_avg_fits[source][cathode_setting][yield_type]['ax'].text(0.6, 0.95, s_run_avg, ha='center', va='center', transform=d_run_avg_fits[source][cathode_setting][yield_type]['ax'].transAxes)

            d_run_avg_fits[source][cathode_setting][yield_type]['fig'].autofmt_xdate()
            d_run_avg_fits[source][cathode_setting][yield_type]['fig'].savefig('%srun_avg_%s_%.3f_%s.png' % (s_path_to_time_dependence, source, cathode_setting, yield_type))



