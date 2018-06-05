#!/usr/bin/python

import sys
import neriX_analysis
import os, re

sys.path.insert(0, '..')
sys.path.insert(0, '../..')
import neriX_simulation_config, pff_test

import pandas as pd

import numpy as np

import matplotlib
matplotlib.use('QT4Agg')
import matplotlib.style
matplotlib.style.use('classic')

matplotlib.rcParams['mathtext.fontset'] = 'stix'
matplotlib.rcParams['font.family'] = 'STIXGeneral'
matplotlib.rcParams['font.size'] = 14

import matplotlib.pyplot as plt
import matplotlib.dates as mdates
import matplotlib.patches as mpatches
from matplotlib.colors import LogNorm
import matplotlib.gridspec as gridspec
from matplotlib.ticker import LinearLocator
from mpl_toolkits.axes_grid1 import make_axes_locatable
from matplotlib.ticker import MaxNLocator

import tqdm

from rootpy import stl
from rootpy.io import File
from rootpy.tree import Tree, TreeModel, TreeChain
from rootpy.plotting import Hist, Hist2D, Canvas, Graph, func


from math import floor

from sklearn import neighbors
from sklearn import grid_search
from sklearn import preprocessing


import scipy.optimize as op
import cPickle as pickle

from scipy.special import gammaln
from scipy.interpolate import interp1d
from scipy.stats import ks_2samp

from matplotlib.offsetbox import AnchoredText

import labellines

num_s1_cuts = 3 # 4 edges, 3 slices

max_s2 = 2800

d_s1_cuts = {}
d_s1_cuts[-4] = [40, 70, 100, 150]
d_s1_cuts[3000] = [0, 7, 15, 40]
d_s1_cuts[3500] = [0, 7, 15, 40]
d_s1_cuts[4500] = [0, 10, 20, 40]
d_s1_cuts[5300] = [0, 10, 20, 40]

transparency = 0.2
l_quantiles_for_2d = [50]

# num_steps goes into how many random draws you will take from kept sampler
num_steps = 2
mc_bin_number_multiplier = 5
data_bin_multiplier = 2
# num_steps_to_include is how large of sampler you keep
num_steps_to_include = 1000
num_mc_events = int(2e5)

if(len(sys.argv) != 3):
	print 'Usage is python compare_data_fit_single_energy.py <cathode setting> <degree setting>'
	sys.exit(1)

cathode_setting = float(sys.argv[1])
degree_setting = int(sys.argv[2])

d_degree_setting_to_energy_name = {2300:3,
                                   3000:5,
                                   3500:7,
                                   4500:10,
                                   5300:15,
                                   6200:20}

d_cathode_voltages_to_field = {0.345:0.19,
                               1.054:0.49,
                               2.356:1.02} # in kV:V/cm


dir_specifier_name = 'multiple_energies_lindhard_model_with_ti_bq_fixed'


l_degree_settings_in_use = [-4, 3000, 3500, 4500, 5300]
s_degree_settings = ''
for name_degree_setting in l_degree_settings_in_use:
    s_degree_settings += '%s,' % (name_degree_setting)
s_degree_settings = s_degree_settings[:-1]


l_cathode_settings_in_use = [0.345, 1.054, 2.356]
s_cathode_settings = ''
for name_cathode_setting in l_cathode_settings_in_use:
    s_cathode_settings += '%.3f,' % (name_cathode_setting)
s_cathode_settings = s_cathode_settings[:-1]


name_of_results_directory = neriX_simulation_config.results_directory_name
l_load = ['plots', dir_specifier_name, '%s_kV_%s_deg' % (s_cathode_settings, s_degree_settings), 'preliminary']
l_plots = ['plots', dir_specifier_name, '%s_kV_%s_deg' % (s_cathode_settings, s_degree_settings)]

s_path_for_load = './'
for directory in l_load:
    s_path_for_load += directory + '/'

s_path_for_save = './'
for directory in l_plots:
    s_path_for_save += directory + '/'

if not os.path.exists(s_path_for_load):
    os.makedirs(s_path_for_load)

if not os.path.exists(s_path_for_save):
    os.makedirs(s_path_for_save)
    

"""
d_for_save = {}
# want to save down pickle files
# S1 cuts used
d_for_save['s1_cuts'] = d_s1_cuts

# 2D histograms
d_for_save['s1_bin_edges'] = d_bin_edges['s1'][cathode_setting][degree_setting]
d_for_save['log_bin_edges'] = d_bin_edges['log'][cathode_setting][degree_setting]
d_for_save['2d_hist_data'] = hist_s1_log_data[3]
d_for_save['2d_hist_mc'] = hist_s1_log_mc

# median lines
d_for_save['median_line_s1'] = a_median_line_s1
d_for_save['median_line_data_log'] = a_median_line_data_log
d_for_save['median_line_mc_log'] = a_median_line_mc_log

# S1 projections with band and best fit
d_for_save['s1_projection_x_values'] = a_s1_x_values
d_for_save['s1_projection_y_values'] = a_s1_y_values
d_for_save['s1_projection_x_err_low'] = a_s1_x_err_low
d_for_save['s1_projection_x_err_high'] = a_s1_x_err_high
d_for_save['s1_projection_y_err_low'] = a_s1_y_err_low
d_for_save['s1_projection_y_err_high'] = a_s1_y_err_high
d_for_save['s1_projection_bin_centers_mc'] = s1_bin_centers_mc
d_for_save['s1_projection_best_fit_line'] = d_quantiles['s1'][l_quantiles[1]]
d_for_save['s1_projection_unc_band_low'] = d_quantiles['s1'][l_quantiles[0]]
d_for_save['s1_projection_unc_band_high'] = d_quantiles['s1'][l_quantiles[2]]


# S2 matches
# remove axis
#for i, s1_cut_num in enumerate(xrange(num_s1_cuts)):
#    del d_s2_plots[current_set_s1_cuts]['axis']

d_for_save['d_s2_plots'] = d_s2_plots
"""


# pickle file
with open('%s%.3f_kV_%d_deg_comps.p' % (s_path_for_load, cathode_setting, degree_setting), 'rb') as f:
    d_plotting = pickle.load(f)
with open('%s%.3f_kV_%d_deg_comps_high_stats.p' % (s_path_for_load, cathode_setting, degree_setting), 'rb') as f:
    d_plotting_high_stats = pickle.load(f)


if degree_setting == -4:
    l_s1_binning = [40, 150]
else:
    l_s1_binning = [d_plotting['s1_bin_edges'][0], d_plotting['s1_bin_edges'][-1]]


figure_sizes_s1 = (8, 5.5)
figure_sizes_s2 = (8, 2)

figure_sizes_do = (8, 11)

fig_s1, (ax_s1_log_data, ax_s1_log_mc, ax_s1_projection) = plt.subplots(3, sharex=True, sharey=False, figsize=figure_sizes_s1)
fig_s2, ax_s2_projections = plt.subplots(1, sharex=True, sharey=False, figsize=figure_sizes_s2)

fig_s1.subplots_adjust(hspace=0)



fig_do, (ax_s1_log_do, ax_s1_projection_do, ax_s2_projections_do) = plt.subplots(3, sharex=False, sharey=False, figsize=figure_sizes_do)
fig_do.subplots_adjust(hspace=0.3)


fig_s1_thesis, (ax_s1_log_data_thesis, ax_s1_log_mc_thesis, ax_s1_projection_thesis) = plt.subplots(3, sharex=True, sharey=False, figsize=figure_sizes_s1)
fig_s2_thesis, ax_s2_projections_thesis = plt.subplots(1, sharex=True, sharey=False, figsize=figure_sizes_s2)

fig_s1_thesis.subplots_adjust(hspace=0)


fig_energy, ax_energy = plt.subplots(figsize=figure_sizes_s2)


#fig.tight_layout()


#print d_plotting

pcm_data = ax_s1_log_data.pcolor(d_plotting['s1_bin_edges'], d_plotting['log_bin_edges'], d_plotting['2d_hist_data'].T, cmap='Blues', linewidth=0,rasterized=True)
pcm_do = ax_s1_log_do.pcolor(d_plotting['s1_bin_edges'], d_plotting['log_bin_edges'], d_plotting['2d_hist_data'].T, cmap='Blues', linewidth=0,rasterized=True)
pcm_data_thesis = ax_s1_log_data_thesis.pcolor(d_plotting['s1_bin_edges'], d_plotting['log_bin_edges'], d_plotting['2d_hist_data'].T, cmap='Blues', linewidth=0,rasterized=True)
#pcm_data.set_edgecolor('face')

#print d_plotting['2d_hist_mc_fine'].shape
pcm_mc = ax_s1_log_mc.pcolor(d_plotting_high_stats['s1_bin_edges_fine'], d_plotting_high_stats['log_bin_edges_fine'], d_plotting_high_stats['2d_hist_mc_fine'].T, cmap='Blues', linewidth=0, rasterized=True)
pcm_mc_thesis = ax_s1_log_mc_thesis.pcolor(d_plotting_high_stats['s1_bin_edges_fine'], d_plotting_high_stats['log_bin_edges_fine'], d_plotting_high_stats['2d_hist_mc_fine'].T, cmap='Blues', linewidth=0, rasterized=True)
#pcm_mc.set_edgecolor('face')

if degree_setting == -4:
    l_log_bounds = [1.4, 2.2]
else:
    l_log_bounds = [1.4, 2.9]

ax_s1_log_data.set_xlim(*l_s1_binning)
ax_s1_log_data.set_ylim(*l_log_bounds)

ax_s1_log_data_thesis.set_xlim(*l_s1_binning)
ax_s1_log_data_thesis.set_ylim(*l_log_bounds)

ax_s1_log_do.set_xlim(*l_s1_binning)
ax_s1_log_do.set_ylim(*l_log_bounds)

ax_s1_log_mc.set_xlim(d_plotting['s1_bin_edges'][0], d_plotting['s1_bin_edges'][-1])
ax_s1_log_mc.set_ylim(*l_log_bounds)

ax_s1_log_mc_thesis.set_xlim(*l_s1_binning)
ax_s1_log_mc_thesis.set_ylim(*l_log_bounds)

if not degree_setting == -4:
    l_log_tick_marks = [1.5, 1.75, 2, 2.25, 2.5, 2.75]
    ax_s1_log_data.set_yticks(l_log_tick_marks)
    ax_s1_log_do.set_yticks(l_log_tick_marks)
    ax_s1_log_mc.set_yticks(l_log_tick_marks)
    ax_s1_log_data_thesis.set_yticks(l_log_tick_marks)
    ax_s1_log_mc_thesis.set_yticks(l_log_tick_marks)

ax_s1_log_data.set_ylabel(r'$\mathrm{log}_{10}(\frac{\mathrm{S2}}{\mathrm{S1}})$')
ax_s1_log_do.set_ylabel(r'$\mathrm{log}_{10}(\frac{\mathrm{S2}}{\mathrm{S1}})$')
ax_s1_log_mc.set_ylabel(r'$\mathrm{log}_{10}(\frac{\mathrm{S2}}{\mathrm{S1}})$')

ax_s1_log_data_thesis.set_ylabel(r'$\mathrm{log}_{10}(\frac{\mathrm{S2}}{\mathrm{S1}})$')
ax_s1_log_mc_thesis.set_ylabel(r'$\mathrm{log}_{10}(\frac{\mathrm{S2}}{\mathrm{S1}})$')

ax_s1_log_do.set_xlabel('S1 [PE]')

# make label lines

gas_gain = 22.9
eta = 0.904
g2 = eta*gas_gain
w_value = 13.7 / 1000.
g1_value = 0.126
lindhard_factor = 0.189
eta_prop = 1.84
line_color = 'gray'
label_color = 'dimgray'

a_ll_s1 = np.linspace(d_plotting['s1_bin_edges'][0], d_plotting['s1_bin_edges'][-1], 200)

def energy_to_epsilon(ll_energy):
    return 11.5*ll_energy*54**(-7./3.)

def lindhard_quenching(ll_energy):
    epsilon = 11.5*ll_energy*54**(-7./3.)
    g = 3*epsilon**0.15 + 0.7*epsilon**0.6 + epsilon
    return lindhard_factor*g / (1 + lindhard_factor*g)

def get_s1_from_log(ll_energy, log_value):
    #return ll_energy*lindhard_quenching(ll_energy)/(w_value * (10**(log_value)/(eta*gas_gain) + 1./(g1_value*(1./(1. + eta_prop*(11.5*ll_energy*54**(-7./3.))**0.5)))))
    f = 1. / (1. + eta_prop*energy_to_epsilon(ll_energy)**0.5)
    return ll_energy*lindhard_quenching(ll_energy)*g2 / w_value / (10**log_value + g2/(f*g1_value))


if degree_setting == 5300:
    l_ll_energies = [5, 7, 10, 15, 20]
elif degree_setting == -4:
    l_ll_energies = [40, 55, 75]
else:
    l_ll_energies = [5, 7, 10, 15]


d_ll = {}

# put keV label in
if not degree_setting == -4:
    ax_s1_log_data.text(16, 2.62, '$\mathrm{keV}$', size='medium', color=label_color)
    ax_s1_log_do.text(16, 2.62, '$\mathrm{keV}$', size='medium', color=label_color)
    ax_s1_log_mc.text(16, 2.62, '$\mathrm{keV}$', size='medium', color=label_color)
    ax_s1_log_data_thesis.text(16, 2.62, '$\mathrm{keV}$', size='medium', color=label_color)
    ax_s1_log_mc_thesis.text(16, 2.62, '$\mathrm{keV}$', size='medium', color=label_color)
else:
    ax_s1_log_data.text(103, 2.0, '$\mathrm{keV}$', size='medium', color=label_color)
    ax_s1_log_do.text(103, 2.0, '$\mathrm{keV}$', size='medium', color=label_color)
    ax_s1_log_mc.text(103, 2.0, '$\mathrm{keV}$', size='medium', color=label_color)
    ax_s1_log_data_thesis.text(103, 2.0, '$\mathrm{keV}$', size='medium', color=label_color)
    ax_s1_log_mc_thesis.text(103, 2.0, '$\mathrm{keV}$', size='medium', color=label_color)

for ll_energy in l_ll_energies:
    #a_ll_log = np.log10(eta*gas_gain * (ll_energy*lindhard_quenching(ll_energy)/(w_value*a_ll_s1) - 1./g1_value))
    f = 1. / (1. + eta_prop*energy_to_epsilon(ll_energy)**0.5)
    a_ll_log = np.log10(g2 * (ll_energy*lindhard_quenching(ll_energy)/(a_ll_s1*w_value) - 1./(g1_value*f)))
    
    
    d_ll[ll_energy],  = ax_s1_log_data.plot(a_ll_s1, a_ll_log, color=line_color, linestyle='-', label=ll_energy)
    ax_s1_log_data.text(get_s1_from_log(ll_energy, 1.5), 1.5, ll_energy, ha='left', size='small', color=label_color)
    
    ax_s1_log_do.plot(a_ll_s1, a_ll_log, color=line_color, linestyle='-', label=ll_energy)
    ax_s1_log_do.text(get_s1_from_log(ll_energy, 1.5), 1.5, ll_energy, ha='left', size='small', color=label_color)
    
    #labellines.labelLine(d_ll[ll_energy], get_s1_from_log(ll_energy, 1.5), align=False, backgroundcolor='none', horizontalalignment='right')

    d_ll[ll_energy],  = ax_s1_log_mc.plot(a_ll_s1, a_ll_log, color=line_color, linestyle='-', label=ll_energy)
    ax_s1_log_mc.text(get_s1_from_log(ll_energy, 1.5), 1.5, ll_energy, ha='left', size='small', color=label_color)

    ax_s1_log_data_thesis.plot(a_ll_s1, a_ll_log, color=line_color, linestyle='-', label=ll_energy)
    ax_s1_log_data_thesis.text(get_s1_from_log(ll_energy, 1.5), 1.5, ll_energy, ha='left', size='small', color=label_color)

    ax_s1_log_mc_thesis.plot(a_ll_s1, a_ll_log, color=line_color, linestyle='-', label=ll_energy)
    ax_s1_log_mc_thesis.text(get_s1_from_log(ll_energy, 1.5), 1.5, ll_energy, ha='left', size='small', color=label_color)





# median lines
#line_handle_data, = ax_s1_log_data.plot(d_plotting['median_line_s1'], d_plotting['median_line_data_log'], color='r', label='Data Median', linestyle='--')
line_handle_mc, = ax_s1_log_data.plot(d_plotting['median_line_s1'], d_plotting['median_line_mc_log'], color='magenta', label='MC Median', linestyle='--')
#ax_s1_log_mc.plot(d_plotting['median_line_s1'], d_plotting['median_line_data_log'], color='r', label='Data Median', linestyle='--')
ax_s1_log_mc.plot(d_plotting['median_line_s1'], d_plotting['median_line_mc_log'], color='magenta', label='MC Median', linestyle='--')
#ax_s1_log_data.legend(handles=[line_handle_data, line_handle_mc], loc='best', prop={'size': 20})
ax_s1_log_data.legend(handles=[line_handle_mc], loc='best', prop={'size': 16}, frameon=False)
ax_s1_log_mc.legend(handles=[line_handle_mc], loc='best', prop={'size': 16}, frameon=False)


line_handle_mc_thesis, = ax_s1_log_data_thesis.plot(d_plotting['median_line_s1'], d_plotting['median_line_mc_log'], color='magenta', label='MC Median', linestyle='--')
ax_s1_log_mc_thesis.plot(d_plotting['median_line_s1'], d_plotting['median_line_mc_log'], color='magenta', label='MC Median', linestyle='--')
ax_s1_log_data_thesis.legend(handles=[line_handle_mc_thesis], loc='best', prop={'size': 16}, frameon=False)
ax_s1_log_mc_thesis.legend(handles=[line_handle_mc_thesis], loc='best', prop={'size': 16}, frameon=False)



ax_s1_log_data.annotate('(a) Data', xy=(0.9, 0.55), xycoords='axes fraction', ha='center', va='center')
ax_s1_log_mc.annotate('(b) MC', xy=(0.9, 0.55), xycoords='axes fraction', ha='center', va='center')
ax_s1_log_data_thesis.annotate('Data', xy=(0.9, 0.65), xycoords='axes fraction', ha='center', va='center')
ax_s1_log_mc_thesis.annotate('MC', xy=(0.9, 0.65), xycoords='axes fraction', ha='center', va='center')

#ax_s1_log_data.text(27, 2.1, 'Data', ha='center', va='center')
#ax_s1_log_mc.text(27, 2.1, 'MC', ha='center', va='center')


ax_s1_projection.errorbar(d_plotting['s1_projection_x_values'], d_plotting['s1_projection_y_values'], xerr=[d_plotting['s1_projection_x_err_low'], d_plotting['s1_projection_x_err_high']], yerr=[d_plotting['s1_projection_y_err_low'], d_plotting['s1_projection_y_err_high']], linestyle='', color='black')
ax_s1_projection.plot(d_plotting['s1_projection_bin_centers_mc'], d_plotting['s1_projection_best_fit_line'], color='b', linestyle='--')
ax_s1_projection.fill_between(d_plotting['s1_projection_bin_centers_mc'], d_plotting['s1_projection_unc_band_low'], d_plotting['s1_projection_unc_band_high'], color='b', alpha=transparency)



ax_s1_projection_thesis.errorbar(d_plotting['s1_projection_x_values'], d_plotting['s1_projection_y_values'], xerr=[d_plotting['s1_projection_x_err_low'], d_plotting['s1_projection_x_err_high']], yerr=[d_plotting['s1_projection_y_err_low'], d_plotting['s1_projection_y_err_high']], linestyle='', color='black')
ax_s1_projection_thesis.plot(d_plotting['s1_projection_bin_centers_mc'], d_plotting['s1_projection_best_fit_line'], color='b', linestyle='--')
ax_s1_projection_thesis.fill_between(d_plotting['s1_projection_bin_centers_mc'], d_plotting['s1_projection_unc_band_low'], d_plotting['s1_projection_unc_band_high'], color='b', alpha=transparency)



ax_s1_projection_do.errorbar(d_plotting['s1_projection_x_values'], d_plotting['s1_projection_y_values'], xerr=[d_plotting['s1_projection_x_err_low'], d_plotting['s1_projection_x_err_high']], yerr=[d_plotting['s1_projection_y_err_low'], d_plotting['s1_projection_y_err_high']], linestyle='', color='black')

#ax_s1_projection.set_xlim(d_plotting['s1_bin_edges'][0], d_plotting['s1_bin_edges'][-1])
ax_s1_projection.set_xlim(*l_s1_binning)
ax_s1_projection_thesis.set_xlim(*l_s1_binning)


ax_s1_projection_do.set_xlim(*l_s1_binning)
#ax_s1_projection.set_ylim(0, max(1.2*d_plotting['s1_projection_best_fit_line']))

#labels = ax_s1_projection.get_yticklabels()
#labels[-1] = ''
#ax_s1_projection.set_yticklabels(labels)

ax_s1_projection.set_yticks(ax_s1_projection.get_yticks()[:-1])
ax_s1_projection.set_xlabel('S1 [PE]')
ax_s1_projection.set_ylabel('Counts')

ax_s1_projection_thesis.set_yticks(ax_s1_projection.get_yticks()[:-1])
ax_s1_projection_thesis.set_xlabel('S1 [PE]')
ax_s1_projection_thesis.set_ylabel('Counts')

#ax_s1_projection_do.set_yticks(ax_s1_projection.get_yticks()[:-1])
ax_s1_projection_do.set_xlabel('S1 [PE]')
ax_s1_projection_do.set_ylabel('Counts')

ax_s1_projection.annotate('(c)', xy=(0.45, 0.85), xycoords='axes fraction', ha='center', va='center')

# grab energy spectrum for inset axis
# inside S1 projection

if degree_setting == -4:
    s_path_to_energy_file = '../../reduced_simulation_data/nerixsim-pure_nr_spec.root'
else:
    s_path_to_energy_file = '../../reduced_simulation_data/nerixsim-%dkeV.root' % (d_degree_setting_to_energy_name[degree_setting])
f_reduced_sim = File(s_path_to_energy_file, 'read')
h_mc = f_reduced_sim.h_mc

num_mc_events = int(4e5)
a_energy_spec = np.zeros(num_mc_events, dtype=np.float32)
for i in tqdm.tqdm(xrange(num_mc_events)):
    a_energy_spec[i] = h_mc.GetRandom()

from mpl_toolkits.axes_grid.inset_locator import inset_axes
iax_energy = inset_axes(ax_s1_projection, width='45%', height='45%', loc=1)

if (degree_setting == 3000) or (degree_setting == 3500):
    l_energy_binning = [100, 1, 15]
    bandwidth = 0.6
elif (degree_setting == 4500) or (degree_setting == 5300):
    l_energy_binning = [100, 1, 25]
    bandwidth = 1.0
else:
    l_energy_binning = [100, 1, 80]
    bandwidth = 1.0

a_energy_axis = np.linspace(l_energy_binning[1], l_energy_binning[2], l_energy_binning[0])

# use KDE to smoothen out
kde = neighbors.KernelDensity(kernel='gaussian', bandwidth=bandwidth).fit(a_energy_spec[:, np.newaxis])
a_log_density = kde.score_samples(a_energy_axis[:, np.newaxis])
iax_energy.plot(a_energy_axis, np.exp(a_log_density))
iax_energy.set_xlim(l_energy_binning[1], l_energy_binning[2])
iax_energy.set_ylim(0, np.max(np.exp(a_log_density))*1.2)
iax_energy.get_yaxis().set_visible(False)
iax_energy.set_xlabel('Energy [keV]')

ax_energy.plot(a_energy_axis, np.exp(a_log_density))
ax_energy.set_xlim(l_energy_binning[1], l_energy_binning[2])
ax_energy.set_ylim(0, np.max(np.exp(a_log_density))*1.2)
ax_energy.get_yaxis().set_visible(False)
ax_energy.set_xlabel('Energy [keV]')

iax_energy.annotate('(d)', xy=(0.7, 0.805), xycoords='axes fraction', ha='center', va='center')

# or use histogram
#iax_energy.hist(a_energy_spec)

d_s2_plots = d_plotting['d_s2_plots']
d_s1_cuts = d_plotting['s1_cuts']
num_s1_cuts = len(d_s1_cuts[degree_setting])-1

l_line_handles = []
l_line_handles_do = []
l_line_handles_thesis = []

l_colors = ['teal', 'magenta', 'k']


text_x0 = 400
text_y = 100
text_dx = 300
#ax_s2_projections.text(text_x0, text_y, 'p:')
text_x0 += 150

max_s2_counts = 0

for i, s1_cut_num in enumerate(xrange(num_s1_cuts)):
    current_set_s1_cuts = (d_s1_cuts[degree_setting][s1_cut_num], d_s1_cuts[degree_setting][s1_cut_num+1])

    d_s2_plots[current_set_s1_cuts]['axis'] = ax_s2_projections

    d_s2_plots[current_set_s1_cuts]['axis'].errorbar(d_s2_plots[current_set_s1_cuts]['x_values'], d_s2_plots[current_set_s1_cuts]['y_values'], xerr=[d_s2_plots[current_set_s1_cuts]['x_err_high'], d_s2_plots[current_set_s1_cuts]['x_err_high']], yerr=[d_s2_plots[current_set_s1_cuts]['y_err_low'], d_s2_plots[current_set_s1_cuts]['y_err_high']], linestyle='', color=l_colors[i])
    
    ax_s2_projections_thesis.errorbar(d_s2_plots[current_set_s1_cuts]['x_values'], d_s2_plots[current_set_s1_cuts]['y_values'], xerr=[d_s2_plots[current_set_s1_cuts]['x_err_high'], d_s2_plots[current_set_s1_cuts]['x_err_high']], yerr=[d_s2_plots[current_set_s1_cuts]['y_err_low'], d_s2_plots[current_set_s1_cuts]['y_err_high']], linestyle='', color=l_colors[i])
    
    ax_s2_projections_do.errorbar(d_s2_plots[current_set_s1_cuts]['x_values'], d_s2_plots[current_set_s1_cuts]['y_values'], xerr=[d_s2_plots[current_set_s1_cuts]['x_err_high'], d_s2_plots[current_set_s1_cuts]['x_err_high']], yerr=[d_s2_plots[current_set_s1_cuts]['y_err_low'], d_s2_plots[current_set_s1_cuts]['y_err_high']], linestyle='', color=l_colors[i])
    
    current_max_s2_counts = d_s2_plots[current_set_s1_cuts]['y_values'].max()
    if current_max_s2_counts > max_s2_counts:
        max_s2_counts = current_max_s2_counts
    


    d_s2_plots[current_set_s1_cuts]['axis'].plot(d_plotting['s2_bin_centers_mc'], d_plotting['s2_projection_best_fit_line'][current_set_s1_cuts], color=l_colors[i], linestyle='--')
    ax_s2_projections_thesis.plot(d_plotting['s2_bin_centers_mc'], d_plotting['s2_projection_best_fit_line'][current_set_s1_cuts], color=l_colors[i], linestyle='--')
    
    d_s2_plots[current_set_s1_cuts]['f_fit'] = interp1d(d_plotting['s2_bin_centers_mc'], d_plotting['s2_projection_best_fit_line'][current_set_s1_cuts], kind='cubic')
    
    f_cum_sum = interp1d(d_plotting['s2_bin_centers_mc'], np.cumsum(d_plotting['s2_projection_best_fit_line'][current_set_s1_cuts]), kind='cubic')
    #f_cum_sum = f_cum_sum / f_cum_sum(d_plotting['s2_bin_centers_mc'][-1])
    
    
    f_fit_ub = interp1d(d_plotting['s2_bin_centers_mc'], d_plotting['s2_projection_unc_band_high'][current_set_s1_cuts], kind='cubic')
    f_fit_lb = interp1d(d_plotting['s2_bin_centers_mc'], d_plotting['s2_projection_unc_band_low'][current_set_s1_cuts], kind='cubic')
    #a_avg_mc_uncertainty = np.asarray((d_plotting['s2_projection_unc_band_low'][current_set_s1_cuts] - d_plotting['s2_projection_unc_band_high'][current_set_s1_cuts]) / 2.)
    
    
    
    mask = (d_s2_plots[current_set_s1_cuts]['y_values'] > 1) & (d_s2_plots[current_set_s1_cuts]['x_values'] < max_s2)
    
    a_avg_mc_uncertainty = (f_fit_ub(d_s2_plots[current_set_s1_cuts]['x_values'][mask]) - f_fit_lb(d_s2_plots[current_set_s1_cuts]['x_values'][mask]))/2.
    
    print d_s2_plots[current_set_s1_cuts]['x_values']
    print a_avg_mc_uncertainty
    
    #print (d_s2_plots[current_set_s1_cuts]['y_values'][mask] - d_s2_plots[current_set_s1_cuts]['f_fit'](d_s2_plots[current_set_s1_cuts]['x_values'][mask]))**2/d_s2_plots[current_set_s1_cuts]['y_values'][mask]
    print np.sum((d_s2_plots[current_set_s1_cuts]['y_values'][mask] - d_s2_plots[current_set_s1_cuts]['f_fit'](d_s2_plots[current_set_s1_cuts]['x_values'][mask]))**2/(d_s2_plots[current_set_s1_cuts]['y_values'][mask] + a_avg_mc_uncertainty**2)) / len(d_s2_plots[current_set_s1_cuts]['x_values'][mask])
    d_s2_plots[current_set_s1_cuts]['p_value_ks'] = ks_2samp(d_s2_plots[current_set_s1_cuts]['y_values'], d_plotting['s2_projection_best_fit_line'][current_set_s1_cuts])[1]
    
    #ax_s2_projections.text(text_x0, text_y, '%.3f,' % (d_s2_plots[current_set_s1_cuts]['p_value_ks']), color=l_colors[i])
    text_x0 += text_dx
    
    l_line_handles.append(d_s2_plots[current_set_s1_cuts]['axis'].fill_between(d_plotting['s2_bin_centers_mc'], d_plotting['s2_projection_unc_band_low'][current_set_s1_cuts], d_plotting['s2_projection_unc_band_high'][current_set_s1_cuts], color=l_colors[i], alpha=transparency, label='$ %d < S1 < %d$' % (current_set_s1_cuts[0], current_set_s1_cuts[1])))
    #l_line_handles.append(d_s2_plots[current_set_s1_cuts]['axis'].fill_between(d_plotting['s2_bin_centers_mc'], d_plotting['s2_projection_unc_band_low'][current_set_s1_cuts], d_plotting['s2_projection_unc_band_high'][current_set_s1_cuts], color=l_colors[i], alpha=transparency, label='$ %d < S1 < %d: p=%.3f$' % (current_set_s1_cuts[0], current_set_s1_cuts[1], d_s2_plots[current_set_s1_cuts]['p_value_ks'])))
    
    
    l_line_handles_do.append(ax_s2_projections_do.fill_between([-1000, -999], [-1, -1], [1, 1], color=l_colors[i], alpha=transparency, label='$ %d < S1 < %d$' % (current_set_s1_cuts[0], current_set_s1_cuts[1])))

    l_line_handles_thesis.append(ax_s2_projections_thesis.fill_between(d_plotting['s2_bin_centers_mc'], d_plotting['s2_projection_unc_band_low'][current_set_s1_cuts], d_plotting['s2_projection_unc_band_high'][current_set_s1_cuts], color=l_colors[i], alpha=transparency, label='$ %d < S1 < %d$' % (current_set_s1_cuts[0], current_set_s1_cuts[1])))
    

    d_s2_plots[current_set_s1_cuts]['axis'].set_xlabel('S2 [PE]')
    d_s2_plots[current_set_s1_cuts]['axis'].set_ylabel('Counts')

    ax_s2_projections_thesis.set_xlabel('S2 [PE]')
    ax_s2_projections_thesis.set_ylabel('Counts')

    ax_s2_projections_do.set_xlabel('S2 [PE]')
    ax_s2_projections_do.set_ylabel('Counts')

    # create text box
    #d_s2_plots[current_set_s1_cuts]['axis'].text(0.05, 0.95, '$ %d < S1 < %d $' % current_set_s1_cuts, transform=d_s2_plots[current_set_s1_cuts]['axis'].transAxes, fontsize=10, verticalalignment='top')


if degree_setting == -4:
    ax_s2_projections.set_xlim(1000, 6000)
    ax_s2_projections_thesis.set_xlim(1000, 6000)
    ax_s2_projections_do.set_xlim(1000, 6000)
else:
    ax_s2_projections.set_xlim(0, max_s2)
    ax_s2_projections_thesis.set_xlim(0, max_s2)
    ax_s2_projections_do.set_xlim(0, max_s2)

ax_s2_projections.set_ylim(0, max_s2_counts*1.2)
ax_s2_projections_thesis.set_ylim(0, max_s2_counts*1.2)
#ax_s2_projections.set_xscale('log')
ax_s2_projections.legend(handles=l_line_handles, loc='best', prop={'size': 16}, frameon=False, ncol=1)
ax_s2_projections_thesis.legend(handles=l_line_handles_thesis, loc=2, prop={'size': 16}, frameon=False, ncol=1)
#ax_s2_projections.legend(handles=l_line_handles, loc='best', prop={'size': 16}, frameon=False, ncol=3)
ax_s2_projections.set_xlabel('S2 [PE]')
ax_s2_projections.set_ylabel('Counts')
ax_s2_projections_thesis.set_xlabel('S2 [PE]')
ax_s2_projections_thesis.set_ylabel('Counts')


ax_s2_projections_do.set_ylim(0, max_s2_counts*1.2)

if degree_setting == -4:
    leg_loc = 2
else:
    leg_loc = 0

ax_s2_projections_do.legend(handles=l_line_handles_do, loc=leg_loc, prop={'size': 16}, frameon=False, ncol=1)
ax_s2_projections_do.set_xlabel('S2 [PE]')
ax_s2_projections_do.set_ylabel('Counts')


if degree_setting == -4:
    s_angle_field = r'Nuclear Recoil Band'
else:
    s_angle_field = r'$\theta = %d^{\circ}$' % (degree_setting/100)
s_angle_field += '\n'
s_angle_field += r'$\mid \vec{E} \mid = %.2f \, \frac{\mathrm{kV}}{\mathrm{cm}}$' % (d_cathode_voltages_to_field[cathode_setting])
ax_s2_projections.text(1600, 80, s_angle_field, ha='center', va='center')

ax_s2_projections.annotate('(e)', xy=(0.07, 0.85), xycoords='axes fraction', ha='center', va='center')

if (degree_setting == 3000) or (degree_setting == 3500):
    loc_degree_field = (0.55, 0.78)
elif degree_setting == 4500:
    loc_degree_field = (0.15, 0.78)
else:
    loc_degree_field = (0.85, 0.78)


#anchor_do = AnchoredText(s_angle_field, loc='best')
#ax_s2_projections_do.add_artist(anchor_do)
ax_s2_projections_do.annotate(s_angle_field, xy=loc_degree_field, xycoords='axes fraction', ha='center', va='center', fontsize='small')
ax_s2_projections_thesis.annotate(s_angle_field, xy=loc_degree_field, xycoords='axes fraction', ha='center', va='center', fontsize='small')




# add colorbar data
l_ticks_data = np.linspace(0, np.max(d_plotting['2d_hist_data']), 6, dtype=np.int32)
l_ticks_mc = np.linspace(0, np.max(d_plotting['2d_hist_mc_fine']), 3, dtype=np.int32)


box_data = ax_s1_log_data.get_position()
cax_data = fig_s1.add_axes([box_data.xmax, box_data.ymin, 0.01, box_data.height])
cbar_data = fig_s1.colorbar(pcm_data, cax=cax_data)
cbar_data.ax.tick_params(labelsize=10)
cbar_data.set_ticks(l_ticks_data)
cbar_data.set_ticklabels(l_ticks_data)

box_do = ax_s1_log_do.get_position()
cax_do = fig_do.add_axes([box_do.xmax, box_do.ymin, 0.01, box_do.height])
cbar_do = fig_do.colorbar(pcm_do, cax=cax_do)
cbar_do.ax.tick_params(labelsize=10)
#cbar_do.set_ticks(l_ticks_data)
#cbar_do.set_ticklabels(l_ticks_data)

box_mc = ax_s1_log_mc.get_position()
cax_mc = fig_s1.add_axes([box_mc.xmax, box_mc.ymin, 0.01, box_mc.height])
cbar_mc = fig_s1.colorbar(pcm_mc, cax=cax_mc)
cbar_mc.ax.tick_params(labelsize=10)
cbar_mc.set_ticks(l_ticks_mc)
cbar_mc.set_ticklabels(l_ticks_mc)

box_data_thesis = ax_s1_log_data_thesis.get_position()
cax_data_thesis = fig_s1.add_axes([box_data_thesis.xmax, box_data_thesis.ymin, 0.01, box_data_thesis.height])
cbar_data_thesis = fig_s1.colorbar(pcm_data_thesis, cax=cax_data_thesis)
cbar_data_thesis.ax.tick_params(labelsize=10)
#cbar_data_thesis.set_ticks(l_ticks_data)
#cbar_data_thesis.set_ticklabels(l_ticks_data)

box_mc_thesis = ax_s1_log_mc_thesis.get_position()
cax_mc_thesis = fig_s1.add_axes([box_mc_thesis.xmax, box_mc_thesis.ymin, 0.01, box_mc_thesis.height])
cbar_mc_thesis = fig_s1.colorbar(pcm_mc_thesis, cax=cax_mc_thesis)
cbar_mc_thesis.ax.tick_params(labelsize=10)
#cbar_mc_thesis.set_ticks(l_ticks_mc_thesis)
#cbar_mc_thesis.set_ticklabels(l_ticks_mc_thesis)



#fig_do.tight_layout()

fig_s1.savefig('%s/%.3f_kV_%d_deg_s1_2d.pdf' % (s_path_for_save, cathode_setting, degree_setting), bbox_inches='tight')
fig_s1_thesis.savefig('%s/%.3f_kV_%d_deg_s1_2d_thesis.png' % (s_path_for_save, cathode_setting, degree_setting), bbox_inches='tight')
fig_do.savefig('%s/%.3f_kV_%d_deg_data_only.png' % (s_path_for_save, cathode_setting, degree_setting), bbox_inches='tight')
fig_s2.savefig('%s/%.3f_kV_%d_deg_s2.pdf' % (s_path_for_save, cathode_setting, degree_setting), bbox_inches='tight')
fig_s2_thesis.savefig('%s/%.3f_kV_%d_deg_s2_thesis.png' % (s_path_for_save, cathode_setting, degree_setting), bbox_inches='tight')
#fig_energy.savefig('%s/%d_deg_energy_spectrum.png' % (s_path_for_save, degree_setting), bbox_inches='tight')




#plt.show()






