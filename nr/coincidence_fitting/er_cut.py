#!/usr/bin/python

import ROOT as root
import sys, tqdm
import neriX_analysis
from rootpy.plotting import Hist2D, Hist, Legend
from rootpy.io import root_open, File
import root_numpy
import os, re
import neriX_simulation_config

import matplotlib
matplotlib.use('QT4Agg')
import matplotlib.style
matplotlib.style.use('classic')

matplotlib.rcParams['mathtext.fontset'] = 'stix'
matplotlib.rcParams['font.family'] = 'STIXGeneral'
matplotlib.rcParams['font.size'] = 16

import matplotlib as plt
import matplotlib.pyplot as plt

import numpy as np
import cPickle as pickle

import pandas as pd

s1Min = 0
s1Max = 40

s2Min = 0.
s2Max = 4000

num_pts_to_use = 8000

if(len(sys.argv) != 2):
	print 'Usage is python coincidence_quick_check.py <cathode>'
	sys.exit(1)

cathodeSetting = float(sys.argv[1])
degreeSetting = -4

s_main_save_directory = '../coincidence_analysis/results/%.3fkV_%ddeg/' % (cathodeSetting, degreeSetting)
s_plot_directory = './er_cut_plots/'


d_s1_s2 = pickle.load(open('%ss1_s2.pkl' % (s_main_save_directory), 'r'))
df_s1_s2 = pd.DataFrame(d_s1_s2)

percentile_used = 99

l_s1_binning = neriX_simulation_config.d_binning['s1'][cathodeSetting][degreeSetting]
l_log_binning = neriX_simulation_config.d_binning['logs2s1'][cathodeSetting][degreeSetting]

# pre-cut the dataframe to save time

if l_s1_binning[0] == 'lin':
    bin_func = np.linspace
else:
    bin_func = np.logspace

s1_bin_edges = bin_func(l_s1_binning[2], l_s1_binning[3], l_s1_binning[1])


if l_log_binning[0] == 'lin':
    bin_func = np.linspace
else:
    bin_func = np.logspace

log_bin_edges = bin_func(l_log_binning[2], l_log_binning[3], l_log_binning[1])


num_bins = len(s1_bin_edges)-1
bin_limit_for_fit = int(0.9*num_bins)

df_s1_s2 = df_s1_s2[(df_s1_s2['s1'] > s1_bin_edges[0]) & (df_s1_s2['s1'] < s1_bin_edges[-1])]
df_s1_s2['log_s2_s1'] = np.log10(df_s1_s2['s2']/df_s1_s2['s1'])
df_s1_s2_no_prelim_cut = df_s1_s2

df_s1_s2 = df_s1_s2[(df_s1_s2['log_s2_s1'] > log_bin_edges[0]) & (df_s1_s2['log_s2_s1'] < log_bin_edges[-1])]

# prelim cut
# (15, 2.3), (150, 1.6)
prelim_slope = (1.6 - 2.1) / (175 - 40)
df_s1_s2 = df_s1_s2[(df_s1_s2['log_s2_s1'] < prelim_slope*(df_s1_s2['s1']-40.) + 2.1)]



a_x = np.zeros(bin_limit_for_fit)
a_x_all = np.zeros(num_bins)
a_y = np.zeros(bin_limit_for_fit)

for i in tqdm.tqdm(xrange(num_bins)):
    current_df = df_s1_s2[(df_s1_s2['s1'] > s1_bin_edges[i]) & (df_s1_s2['s1'] < s1_bin_edges[i+1])]
    
    a_x_all[i] = np.mean(current_df['s1'])
    
    if i < bin_limit_for_fit:
        a_x[i] = np.mean(current_df['s1'])
        a_y[i] = np.percentile(np.log10(current_df['s2']/current_df['s1']), [percentile_used])


a_data, _, _ = np.histogram2d(df_s1_s2_no_prelim_cut['s1'], df_s1_s2_no_prelim_cut['log_s2_s1'], bins=[s1_bin_edges, log_bin_edges])
l_pol = np.polyfit(a_x, a_y, 1)

a_y_fit = l_pol[1] + l_pol[0]*a_x_all# + l_pol[0]*a_x_all**2

fig_data, ax_data = plt.subplots(1)
ax_data.pcolormesh(s1_bin_edges, log_bin_edges, a_data.T, cmap='Blues')
ax_data.plot(a_x_all, a_y_fit, 'r-')

ax_data.set_xlabel('S1 [PE]')
ax_data.set_ylabel(r'$\mathrm{Log}_{10}(\frac{\mathrm{S2}}{\mathrm{S1}})$')

ax_data.set_xlim(s1_bin_edges[0], 160)
ax_data.set_ylim(log_bin_edges[0], log_bin_edges[-1])

print '\n\nText of cut removed!!\n\n'
#ax_data.text(0.35, 0.85, r'$ \mathrm{Log}_{10}(\frac{\mathrm{S2}}{\mathrm{S1}}) < %.2f - %.4f \cdot \mathrm{S1} $' % (l_pol[1], abs(l_pol[0])), transform=ax_data.transAxes, fontsize=12, verticalalignment='top')


df_s1_s2 = df_s1_s2[df_s1_s2['z'] < -3.5]

df_s1_s2['r2'] = df_s1_s2['x']**2 + df_s1_s2['y']**2
r2_min = np.min(df_s1_s2['r2'])
r2_max = np.max(df_s1_s2['r2'])
r2_num_bins = 20
z_min = np.min(df_s1_s2['z'])
z_max = -3.5 #np.max(df_s1_s2['z'])
z_num_bins = 20

tpc_rad = 21.5011
tpc_height = 23.39

r2_bins = np.linspace(r2_min, r2_max, r2_num_bins)
z_bins = np.linspace(z_min, z_max, z_num_bins)

fix_pos, (ax_x_y, ax_r2_z) = plt.subplots(1, 2, figsize=(10,5))
ax_r2_z.scatter(df_s1_s2['r2']*tpc_rad**2, df_s1_s2['z'], marker='o', c='#0082c8', s=4, linewidth=0, alpha=0.7)
#ax_r2_z.scatter(df_s1_s2['r2'], df_s1_s2['z'])
ax_x_y.scatter(df_s1_s2['x']*tpc_rad, df_s1_s2['y']*tpc_rad, marker='o', c='#0082c8', s=4, linewidth=0, alpha=0.7)


ax_x_y.set_xlabel('X [mm]')
ax_x_y.set_ylabel('Y [mm]')
ax_x_y.set_xlim(-tpc_rad, tpc_rad)
ax_x_y.set_ylim(-tpc_rad, tpc_rad)


ax_r2_z.set_xlim(0, tpc_rad**2)
ax_r2_z.set_ylim(-24.5, -3)
ax_r2_z.set_xlabel(r'$\mathrm{R}^2 \, \, [\mathrm{mm}^2]$')
ax_r2_z.set_ylabel('Z [mm]')

fix_pos.tight_layout()


if not os.path.exists(s_plot_directory):
    os.makedirs(s_plot_directory)

fig_data.savefig('%ser_cut%.3f_kV.png' % (s_plot_directory, cathodeSetting))
fix_pos.savefig('%spos_rec%.3f_kV.png' % (s_plot_directory, cathodeSetting))

#plt.show()



