#!/usr/bin/python

import sys
import neriX_analysis
import os, re

import numpy as np

import matplotlib
matplotlib.use('QT4Agg')
import matplotlib.pyplot as plt
import matplotlib.dates as mdates
import matplotlib.patches as mpatches

from sklearn import neighbors
from sklearn import grid_search
from sklearn import preprocessing


import cPickle as pickle



if(len(sys.argv) != 3):
	print 'Usage is python plot_s1_s2.py <cathode> <degree>'
	sys.exit(1)

cathode_setting = float(sys.argv[1])
degree_setting = int(sys.argv[2])


if degree_setting < 0:
    num_bins = 60
else:
    num_bins = 20

s1_nb = num_bins
s1_min = 0
s1_max = 40

s2_nb = num_bins
s2_min = 0
s2_max = 4000

log_nb = num_bins
log_min = 1.
log_max = 3.5





s_main_save_directory = './results/%.3fkV_%ddeg/' % (cathode_setting, degree_setting)

d_s1_s2 = pickle.load(open('%ss1_s2.pkl' % (s_main_save_directory), 'r'))

#print d_s1_s2

fig_s1_s2, ax_s1_s2 = plt.subplots(1)
ax_s1_s2.hist2d(d_s1_s2['s1'], np.log10(d_s1_s2['s2']/d_s1_s2['s1']), bins=[s1_nb, log_nb], range=[[s1_min, s1_max], [log_min, log_max]])



# Plot S1 Hist
fig_s1, ax_s1 = plt.subplots(1)

a_s1_bin_edges = np.linspace(s1_min, s1_max, s1_nb+1)
a_s1_y_values, a_s1_bin_edges = np.histogram(d_s1_s2['s1'], bins=a_s1_bin_edges)

a_s1_x_values, a_s1_y_values, a_s1_x_err_low, a_s1_x_err_high, a_s1_y_err_low, a_s1_y_err_high = neriX_analysis.prepare_hist_arrays_for_plotting(a_s1_y_values, a_s1_bin_edges)

ax_s1.errorbar(a_s1_x_values, a_s1_y_values, xerr=[a_s1_x_err_low, a_s1_x_err_high], yerr=[a_s1_y_err_low, a_s1_y_err_high], marker='.', color='b', linestyle='')



# Plot S2 Hist
fig_s2, ax_s2 = plt.subplots(1)

a_s2_bin_edges = np.linspace(s2_min, s2_max, s2_nb+1)
a_s2_y_values, a_s2_bin_edges = np.histogram(d_s1_s2['s2'], bins=a_s2_bin_edges)

a_s2_x_values, a_s2_y_values, a_s2_x_err_low, a_s2_x_err_high, a_s2_y_err_low, a_s2_y_err_high = neriX_analysis.prepare_hist_arrays_for_plotting(a_s2_y_values, a_s2_bin_edges)

ax_s2.errorbar(a_s2_x_values, a_s2_y_values, xerr=[a_s2_x_err_low, a_s2_x_err_high], yerr=[a_s2_y_err_low, a_s2_y_err_high], marker='.', color='b', linestyle='')




plt.show()



