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

d_degree_setting_to_energy_name = {2300:3,
                                   3000:5,
                                   3500:7,
                                   4500:10,
                                   5300:15,
                                   6200:20}

l_degree_settings = [3000, 3500, 4500, 5300, -4]


d_energy_spec = {}
d_energy_spec[3000] = {'color':'#e6194b'}
d_energy_spec[3500] = {'color':'#0082c8'}
d_energy_spec[4500] = {'color':'#3cb44b'}
d_energy_spec[5300] = {'color':'#f58231'}
d_energy_spec[-4] = {'color':'#0082c8'}

line_width = 5
figure_size = (8, 3)

fig_coin, ax_coin = plt.subplots(1, figsize=figure_size)
fig_band, ax_band = plt.subplots(1, figsize=figure_size)

max_y = 0

l_line_handlers = []

for degree_setting in l_degree_settings:

    if not degree_setting == -4:
        s_path_to_energy_file = '../../reduced_simulation_data/nerixsim-%dkeV.root' % (d_degree_setting_to_energy_name[degree_setting])
    else:
        s_path_to_energy_file = '../../reduced_simulation_data/nerixsim-pure_nr_spec.root'
    f_reduced_sim = File(s_path_to_energy_file, 'read')
    h_mc = f_reduced_sim.h_mc
    
    if degree_setting == -4:
        ax_energy = ax_band
    else:
        ax_energy = ax_coin

    num_mc_events = int(4e5)
    a_energy_spec = np.zeros(num_mc_events, dtype=np.float32)
    for i in tqdm.tqdm(xrange(num_mc_events)):
        a_energy_spec[i] = h_mc.GetRandom()


    if (degree_setting == 3000) or (degree_setting == 3500):
        l_energy_binning = [100, 2, 25]
        bandwidth = 0.6
    elif (degree_setting == 4500) or (degree_setting == 5300):
        l_energy_binning = [100, 2, 25]
        bandwidth = 1.0
    else:
        l_energy_binning = [100, 3, 80]
        bandwidth = 1.0

    a_energy_axis = np.linspace(l_energy_binning[1], l_energy_binning[2], l_energy_binning[0])

    # use KDE to smoothen out
    kde = neighbors.KernelDensity(kernel='gaussian', bandwidth=bandwidth).fit(a_energy_spec[:, np.newaxis])
    a_log_density = kde.score_samples(a_energy_axis[:, np.newaxis])
    a_density = np.exp(a_log_density)
    a_density /= np.sum(a_density)

    current_max_y = np.max(a_density)
    if current_max_y > max_y:
        max_y = current_max_y

    s_label = '$%d^{\circ}$' % (degree_setting/100)

    if not degree_setting == -4:
        l_line_handlers.append(ax_energy.plot(a_energy_axis, a_density, color=d_energy_spec[degree_setting]['color'], label=s_label, linewidth=line_width)[0])
        ax_energy.set_ylim(0, max_y*1.2)
    else:
        ax_energy.plot(a_energy_axis, a_density, color=d_energy_spec[degree_setting]['color'], linewidth=line_width)
        #print a_log_density
        #print np.max(a_log_density)
        ax_energy.set_ylim(0, np.max(a_density)*1.2)
    ax_energy.set_xlim(l_energy_binning[1], l_energy_binning[2])
    #ax_energy.get_yaxis().set_visible(False)
    ax_energy.set_xlabel('Energy [keV]')

    if degree_setting == -4:
        ax_energy.set_ylabel(r'$p(E)$')
    else:
        ax_energy.set_ylabel(r'$p(E)$')


ax_coin.legend(handles=l_line_handlers, loc='upper right', ncol=2, prop={'size': 16}, frameon=False, handletextpad=0.5, columnspacing=1, numpoints=1)

band_text = AnchoredText('Nuclear Recoil Spectrum', loc=1, frameon=False)
ax_band.add_artist(band_text)


s_path_for_save = './plots'

#fig_energy.savefig('%s/%d_deg_energy_spectrum.png' % (s_path_for_save, degree_setting), bbox_inches='tight')
fig_band.savefig('%s/nerix_energy_spectrum_band.png' % (s_path_for_save), bbox_inches='tight')
fig_coin.savefig('%s/nerix_energy_spectrum_coin.png' % (s_path_for_save), bbox_inches='tight')



plt.show()






