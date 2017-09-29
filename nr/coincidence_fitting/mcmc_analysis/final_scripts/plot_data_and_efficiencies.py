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

matplotlib.rcParams['mathtext.fontset'] = 'stix'
matplotlib.rcParams['font.family'] = 'STIXGeneral'
matplotlib.rcParams['font.size'] = 16


import matplotlib.pyplot as plt
import matplotlib.dates as mdates
import matplotlib.patches as mpatches
from matplotlib.colors import LogNorm, ColorConverter
import matplotlib.gridspec as gridspec
from matplotlib.ticker import LinearLocator

import tqdm

from rootpy import stl
from rootpy.io import File
from rootpy.tree import Tree, TreeModel, TreeChain
from rootpy.plotting import Hist, Hist2D, Canvas, Graph, func


from math import floor

from sklearn import neighbors
from sklearn import grid_search
from sklearn import preprocessing
from sklearn.neighbors import LocalOutlierFactor



import scipy.optimize as op
from scipy import interpolate
import cPickle as pickle

from scipy.special import gammaln

import pandas as pd

from sklearn.neighbors import KernelDensity

def kde_2D(x, y, bandwidth, xbins=100j, ybins=100j, **kwargs):
    # Build 2D kernel density estimate (KDE).

    # create grid of sample locations (default: 100x100)
    xx, yy = np.mgrid[x.min():x.max():xbins, 
                      y.min():y.max():ybins]

    xy_sample = np.vstack([yy.ravel(), xx.ravel()]).T
    xy_train  = np.vstack([y, x]).T

    scaler = preprocessing.StandardScaler().fit(xy_train)
    kde_skl = neighbors.KernelDensity(bandwidth=bandwidth, **kwargs)
    kde_skl.fit(scaler.transform(xy_train))

    # score_samples() returns the log-likelihood of the samples
    z = np.exp(kde_skl.score_samples(scaler.transform(xy_sample)))
    return xx, yy, np.reshape(z, xx.shape)



if(len(sys.argv) != 2):
	print 'Usage is python compare_data_fit_single_energy.py <cathode setting>'
	sys.exit(1)

cathode_setting = float(sys.argv[1])

d_degree_setting_to_energy_name = {2300:3,
                                   3000:5,
                                   3500:7,
                                   4500:10,
                                   5300:15,
                                   6200:20}

d_cathode_voltages_to_field = {0.345:190,
                               1.054:490,
                               2.356:1020} # in kV:V/cm


dir_specifier_name = 'multiple_energies_lindhard_model_with_ti'


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
l_plots = ['plots']


s_path_for_save = './'
for directory in l_plots:
    s_path_for_save += directory + '/'

if not os.path.exists(s_path_for_save):
    os.makedirs(s_path_for_save)





l_s1_binning = [500, 1, 40]
l_s2_binning = [500, 2.3, 3.6]
l_s1_binning_band = [500, 40, 180]
l_s2_binning_band = [500, 3.25, 3.85]

num_mc_events = int(1e4)
l_quantiles = [16, 50, 84]
transparency = 0.2



d_data = {}
for current_cathode_setting in l_cathode_settings_in_use:
    d_data[current_cathode_setting] = {}
    for current_degree_setting in l_degree_settings_in_use:
        s_path_to_data = '../../../coincidence_analysis/results/%.3fkV_%ddeg/s1_s2.pkl' % (current_cathode_setting, current_degree_setting)
        d_data[current_cathode_setting][current_degree_setting] = pd.DataFrame.from_dict(pickle.load(open(s_path_to_data, 'r')))
        # cut to ensure that only keep points in range (for KDE)
        #print d_data[current_cathode_setting][current_degree_setting]
        
        if current_degree_setting != -4:
            d_data[current_cathode_setting][current_degree_setting] = d_data[current_cathode_setting][current_degree_setting][(d_data[current_cathode_setting][current_degree_setting]['s1'] > l_s1_binning[1]) & (d_data[current_cathode_setting][current_degree_setting]['s1'] < l_s1_binning[2]) & (np.log10(d_data[current_cathode_setting][current_degree_setting]['s2']) > l_s2_binning[1]) & (np.log10(d_data[current_cathode_setting][current_degree_setting]['s2']) < l_s2_binning[2])]
        else:
            # different binning and er cut
            d_data[current_cathode_setting][current_degree_setting] = d_data[current_cathode_setting][current_degree_setting][(d_data[current_cathode_setting][current_degree_setting]['s1'] > l_s1_binning_band[1]) & (d_data[current_cathode_setting][current_degree_setting]['s1'] < l_s1_binning_band[2]) & (np.log10(d_data[current_cathode_setting][current_degree_setting]['s2']) > l_s2_binning_band[1]) & (np.log10(d_data[current_cathode_setting][current_degree_setting]['s2']) < l_s2_binning_band[2]) & (np.log10(d_data[current_cathode_setting][current_degree_setting]['s2']/d_data[current_cathode_setting][current_degree_setting]['s1']) < (2.196 - 3.596e-3*d_data[current_cathode_setting][current_degree_setting]['s1']))]


# remove band data
l_degree_settings_in_use.remove(-4)



d_scatter_pars = {}
"""
d_scatter_pars['marker'] = {3000:'s',
                            3500:'^',
                            4500:'v',
                            5300:'o'}
d_scatter_pars['color'] = {0.345:'b',
                           1.054:'darkgreen',
                           2.356:'r'}
"""

d_scatter_pars['color'] = {3000:'#0082c8', # blue
                           3500:'#3cb44b', # green
                           4500:'#e6194b', # red
                           5300:'#808080'} # gray

figure_size = (16, 6)

l_grid_dim = [3, 4] # smaller number means larger upper row

fig = plt.figure(figsize=figure_size)
gs = gridspec.GridSpec(*l_grid_dim) # 3 rows, 4 columns

ax_data = fig.add_subplot(111)
ax_data.set_position(gs[1:,:(l_grid_dim[1]-1)].get_position(fig))
ax_data.set_subplotspec(gs[1:,:(l_grid_dim[1]-1)])

#fig, ax_data = plt.subplots(1, sharex=True, sharey=False, figsize=figure_size)

l_legend_handles_coincidence = []

for current_cathode_setting in l_cathode_settings_in_use:
    if not current_cathode_setting == cathode_setting:
        continue
    for current_degree_setting in l_degree_settings_in_use:
        l_legend_handles_coincidence.append(ax_data.scatter(d_data[current_cathode_setting][current_degree_setting]['s1'], np.log10(d_data[current_cathode_setting][current_degree_setting]['s2']), marker='o', c=d_scatter_pars['color'][current_degree_setting], s=3, linewidths=0, alpha=1, label='$%d^{\circ}$' % (current_degree_setting/100)))

        # works well but no integral
        """
        xx, yy, zz = kde_2D(np.asarray(d_data[current_cathode_setting][current_degree_setting]['s1']), np.asarray(np.log10(d_data[current_cathode_setting][current_degree_setting]['s2'])), 0.4)
        #print xx
        ax_data.contour(xx, yy, zz, colors=d_scatter_pars['color'][current_degree_setting], levels=[0.75*np.max(zz)], linewidths=2)
        """


        xx, yy, zz = kde_2D(np.asarray(d_data[current_cathode_setting][current_degree_setting]['s1']), np.asarray(np.log10(d_data[current_cathode_setting][current_degree_setting]['s2'])), 0.25)
        zz = zz / np.sum(zz) # normalize s.t. integral is 1
        t = np.linspace(0, np.max(zz), 1000)
        integral = ((zz >= t[:, None, None]) * zz).sum(axis=(1,2))
        f = interpolate.interp1d(integral, t)
        ax_data.contour(xx, yy, zz, colors=d_scatter_pars['color'][current_degree_setting], levels=[f(0.5), f(0.25)], linewidths=2, linestyles=['dashed', 'solid'])
        

        """
        a_x = np.asarray(d_data[current_cathode_setting][current_degree_setting]['s1'])
        a_y = np.asarray(np.log10(d_data[current_cathode_setting][current_degree_setting]['s2']))
        clf = LocalOutlierFactor(n_neighbors=50)
        xy_train  = np.vstack([a_x, a_y]).T
        clf.fit(xy_train)

        xx, yy = np.mgrid[a_x.min():a_x.max():100j, a_y.min():a_y.max():100j]
        zz = clf._decision_function(np.c_[xx.ravel(), yy.ravel()])
        zz = -1./zz.reshape(xx.shape)
        zz /= zz.max()
        ax_data.contour(xx, yy, zz, levels=[0.55, 0.75])
        break
        """




ax_data.set_ylabel(r'$log_{10}\left(S2\right)$')
ax_data.set_xlabel('S1 [PE]')
ax_data.text(20, 2.5, r'$E = 1.02 \, \frac{kV}{cm}$')
ax_data.text(1.5, 3.3, '(d)')

# S1 efficiency

ax_s1_eff = fig.add_subplot(211, sharex=ax_data)
ax_s1_eff.set_position(gs[0,:(l_grid_dim[1]-1)].get_position(fig))
ax_s1_eff.set_subplotspec(gs[0,:(l_grid_dim[1]-1)])

ax_s1_eff.text(1.5, 0.8, '(a)')


l_s1_bin_edges = np.linspace(l_s1_binning[1], l_s1_binning[2], l_s1_binning[0]+1)
l_s1_eff_means = [3.16064649,  0.75476424]
l_s1_eff_covariance = [[1.98043618e-04, 5.02423109e-06], [5.02423109e-06, 7.88784819e-05]]

l_s1_eff_lb = [-1. for i in xrange(len(l_s1_bin_edges))]
l_s1_eff_median = [-1. for i in xrange(len(l_s1_bin_edges))]
l_s1_eff_ub = [-1. for i in xrange(len(l_s1_bin_edges))]

a_output = np.random.multivariate_normal(l_s1_eff_means, l_s1_eff_covariance, size=num_mc_events)
a_1 = a_output[:,0]
a_2 = a_output[:,1]

for i, x_pos in enumerate(l_s1_bin_edges):
    l_s1_eff_lb[i], l_s1_eff_median[i], l_s1_eff_ub[i] = np.percentile(1./(1.+np.exp(-(x_pos-a_1) / a_2)), l_quantiles)




ax_s1_eff.plot(l_s1_bin_edges, l_s1_eff_median, color='k', linestyle='-')
ax_s1_eff.fill_between(l_s1_bin_edges, l_s1_eff_lb, l_s1_eff_ub, color='k', alpha=transparency)
ax_s1_eff.axhline(1., color='k', linestyle='--')

ax_s1_eff.get_xaxis().tick_top()
#ax_s1_eff.get_xaxis().set_visible(False)
ax_s1_eff.set_ylim(0, 1.05)
ax_s1_eff.set_ylabel('Efficiency')
ax_s1_eff.set_yticks([0.2, 0.4, 0.6, 0.8, 1])


# S2 efficiency

ax_s2_eff = fig.add_subplot(311, sharey=ax_data)
ax_s2_eff.set_position(gs[1:,(l_grid_dim[1]-1)].get_position(fig))
ax_s2_eff.set_subplotspec(gs[1:,(l_grid_dim[1]-1)])


l_s2_bin_edges = np.linspace(l_s2_binning[1], l_s2_binning[2], l_s2_binning[0]+1)


l_s2_bin_edges = np.linspace(l_s2_binning[1], l_s2_binning[2], l_s2_binning[0]+1)
l_s2_eff_means = [114.36224212, 381.80770505]
l_s2_eff_covariance = [[799.28476818, -659.93322804], [-659.93322804, 1133.62883639]]

l_s2_eff_lb = [-1. for i in xrange(len(l_s2_bin_edges))]
l_s2_eff_median = [-1. for i in xrange(len(l_s2_bin_edges))]
l_s2_eff_ub = [-1. for i in xrange(len(l_s2_bin_edges))]

a_output = np.random.multivariate_normal(l_s2_eff_means, l_s2_eff_covariance, size=num_mc_events)
a_1 = a_output[:,0]
a_2 = a_output[:,1]

for i, x_pos in enumerate(l_s2_bin_edges):
    # need factor of 10 since binned in log space
    l_s2_eff_lb[i], l_s2_eff_median[i], l_s2_eff_ub[i] = np.percentile(1. - np.exp(-(10**(x_pos)-a_1) / a_2), l_quantiles)




ax_s2_eff.plot(l_s2_eff_median, l_s2_bin_edges, color='k', linestyle='-')
ax_s2_eff.fill_betweenx(l_s2_bin_edges, l_s2_eff_lb, l_s2_eff_ub, color='k', alpha=transparency)
ax_s2_eff.axvline(1., color='k', linestyle='--')


ax_s2_eff.get_yaxis().tick_right()
#ax_s2_eff.get_yaxis().set_visible(False)
ax_s2_eff.set_xlim(0, 1.05)
ax_s2_eff.set_xlabel('Efficiency')
ax_s2_eff.set_xticks([0.2, 0.4, 0.6, 0.8, 1])

ax_s2_eff.text(0.2, 3.4, '(e)')





# EJ plot

ax_ej = fig.add_subplot(411)
ax_ej.set_position(gs[0,(l_grid_dim[1]-1)].get_position(fig))
ax_ej.set_subplotspec(gs[0,(l_grid_dim[1]-1)])

d_ej_data = pickle.load(open('../../../liq_scint_discrimination/results/2.356kV_2300deg/ej_data.p', 'rb'))
# only using EJ3
d_ej_data = d_ej_data[3]

df_ej_all = pd.DataFrame.from_dict({'height':d_ej_data['height'], 'psd':d_ej_data['psd']})
df_ej_neutrons = df_ej_all[(df_ej_all['height'] > d_ej_data['min_height']) & (df_ej_all['height'] < d_ej_data['max_height']) & (df_ej_all['psd'] > d_ej_data['psd_cut'])]
df_ej_gammas = df_ej_all[(df_ej_all['height'] > d_ej_data['min_height']) & (df_ej_all['height'] < d_ej_data['max_height']) & (df_ej_all['psd'] < d_ej_data['psd_cut'])]


l_legend_handles_ej = []

l_legend_handles_ej.append(ax_ej.scatter(df_ej_neutrons['height'], df_ej_neutrons['psd'], marker='o', c='magenta', s=3, linewidths=0, alpha=1, label='n'))
l_legend_handles_ej.append(ax_ej.scatter(df_ej_gammas['height'], df_ej_gammas['psd'], marker='o', c='orange', s=3, linewidths=0, alpha=1, label='$\gamma$'))

ax_ej.axhline(0.228, color='red', linestyle='--')

ax_ej.xaxis.tick_top()
ax_ej.yaxis.tick_right()

ax_ej.set_xlim(d_ej_data['min_height'], d_ej_data['max_height'])
ax_ej.set_ylim(0.1, 0.38)
ax_ej.set_yticks([0.15, 0.25, 0.35])
ax_ej.set_xticks([0.6, 0.8, 1.0, 1.2, 1.4])
ax_ej.get_xaxis().set_label_position('top')
ax_ej.get_yaxis().set_label_position('right')
ax_ej.set_xlabel('Pulse Height [V]')
ax_ej.set_ylabel('PSD Parameter', rotation=270, labelpad=16)

ax_ej.text(1.28, 0.34, '(c)')


# inlay plot of band

from mpl_toolkits.axes_grid.inset_locator import inset_axes
#iax_band = inset_axes(ax_s1_eff, width='45%', height='60%', loc=7)
iax_band = fig.add_axes([0.38, 0.69, 0.32, 0.16])
iax_band.scatter(d_data[cathode_setting][-4]['s1'], np.log10(d_data[cathode_setting][-4]['s2']), marker='o', c='#008080', s=3, linewidths=0, alpha=1, label='NR Band')

iax_band.set_xlim(l_s1_binning_band[1], l_s1_binning_band[2])
iax_band.set_ylim(l_s2_binning_band[1], l_s2_binning_band[2])
#iax_band.set_xscale('log', nonposx='clip')

iax_band.set_xlabel('S1 [PE]', fontsize=12, labelpad=-0.3)
iax_band.set_ylabel(r'$log_{10}\left(S2\right)$', fontsize=12, labelpad=-0.3)
iax_band.tick_params(labelsize=10)

iax_band.text(170, 3.72, '(b)')


# make label lines
gas_gain = 23.1
eta = 0.892
w_value = 13.7 / 1000.
g1_value = 0.125
lindhard_factor = 0.166
line_color = 'gray'
label_color = 'dimgray'

a_ll_s1 = np.linspace(l_s1_binning_band[1], l_s1_binning_band[2], 200)


def lindhard_quenching(ll_energy):
    epsilon = 11.5*ll_energy*54**(-7./3.)
    g = 3*epsilon**0.15 + 0.7*epsilon**0.6 + epsilon
    return lindhard_factor*g / (1 + lindhard_factor*g)

def get_s1_from_log(ll_energy, log_value):
    return g1_value*(ll_energy*lindhard_quenching(ll_energy)/w_value - 10**log_value/(eta*gas_gain))

ll_energy = 74
#print lindhard_quenching(ll_energy)

a_ll_log = np.log10(eta*gas_gain * (ll_energy*lindhard_quenching(ll_energy)/(w_value) - a_ll_s1/g1_value))
iax_band.plot(a_ll_s1, a_ll_log, color=line_color, linestyle='-', label=ll_energy)
iax_band.text(get_s1_from_log(ll_energy, 3.3)-2.5, 3.27, '%d keV NR Band Kinematic Maximum Energy' % ll_energy, ha='right', size='x-small', color=label_color)





ax_data.set_xscale('log', nonposx='clip')
ax_data.set_xlim(l_s1_binning[1], l_s1_binning[2])
ax_data.set_ylim(l_s2_binning[1], l_s2_binning[2])




fig.subplots_adjust(hspace=0, wspace=0)
#fig.tight_layout()


# put legends in place
legend_coincidence = ax_s2_eff.legend(handles=l_legend_handles_coincidence, scatterpoints=1, loc='center left', ncol=2, prop={'size': 16}, frameon=False, handletextpad=0.1, columnspacing=0.3)
legend_ej = ax_s2_eff.legend(handles=l_legend_handles_ej, loc='upper center', prop={'size': 16}, frameon=False, handletextpad=0.1)
ax_s2_eff.add_artist(legend_coincidence)


# make custom legend for contours
#p_lower_percentile = matplotlib.patches.Patch(color='k', linestyle='-', label=r'$25^{\mathrm{th}}$ Percentile')
#p_higher_percentile = matplotlib.patches.Patch(color='k', linestyle='--', label=r'$50^{\mathrm{th}}$ Percentile')
p_lower_percentile = plt.Line2D((-1000, -1000), (-1000, -1000), color='k', linestyle='-', label=r'$25^{\mathrm{th}}$ Percentile')
p_higher_percentile = plt.Line2D((-1000, -1000), (-1000, -1000), color='k', linestyle='--', label=r'$50^{\mathrm{th}}$ Percentile')
ax_data.legend(handles=[p_lower_percentile, p_higher_percentile], frameon=False, ncol=2, prop={'size': 16}, columnspacing=1.4, loc='center', bbox_to_anchor=(0.23, 0.9), handletextpad=0.1)




c = ColorConverter()

for i in xrange(len(legend_coincidence.legendHandles)):
    #print legend_coincidence.legendHandles[i].__dict__
    legend_coincidence.legendHandles[i]._sizes = [30]
    legend_coincidence.legendHandles[i]._alpha = 0.99
    #legend_coincidence.legendHandles[i]._facecolors = c.to_rgb(legend_coincidence.legendHandles[i]._facecolors_original)

for i in xrange(len(legend_ej.legendHandles)):
    legend_ej.legendHandles[i]._sizes = [30]
    legend_ej.legendHandles[i]._alpha = 1



ax_data.set_xticks([2, 6, 10, 30])
ax_data.get_xaxis().set_major_formatter(matplotlib.ticker.ScalarFormatter())
ax_data.set_yticks(ax_data.get_yticks()[1:-2])

iax_band.set_xticks([40, 70, 100, 130, 160])
iax_band.set_yticks([3.3, 3.5, 3.7])
iax_band.get_xaxis().set_major_formatter(matplotlib.ticker.ScalarFormatter())

fig.savefig('%s/%.3f_kV_data_efficiencies_psd.pdf' % (s_path_for_save, cathode_setting), bbox_inches='tight')

plt.show()


