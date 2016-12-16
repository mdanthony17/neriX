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


s1_nb = 40
s1_min = 1.5
s1_max = 40

s2_nb = 40
s2_min = 180
s2_max = 4000

kde_nb = 100



if(len(sys.argv) != 3):
	print 'Usage is python plot_s1_s2.py <cathode> <degree>'
	sys.exit(1)

cathode_setting = float(sys.argv[1])
degree_setting = int(sys.argv[2])

s_main_save_directory = './results/%.3fkV_%ddeg/' % (cathode_setting, degree_setting)

d_s1_s2 = pickle.load(open('%ss1_s2.pkl' % (s_main_save_directory), 'r'))

#print d_s1_s2

fig_s1_s2, ax_s1_s2 = plt.subplots(1)
ax_s1_s2.hist2d(d_s1_s2['s1'], d_s1_s2['s2'], bins=[s1_nb, s2_nb], range=[[s1_min, s1_max], [s2_min, s2_max]])



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




# fit KDE
a_data = np.asarray([d_s1_s2['s1'], d_s1_s2['s2']]).T

scaler = preprocessing.StandardScaler()
scaler.fit(a_data)
a_scaled_samples = scaler.transform(a_data)

#print a_sampler[:,1:3]
#print a_scaled_samples

# find the best fit bandwith since this allows us
# to play with bias vs variance
grid = grid_search.GridSearchCV(neighbors.KernelDensity(), {'bandwidth':np.linspace(0.01, 2., 20)}, cv=4, verbose=1, n_jobs=4)
print '\nDetermining best bandwidth...\n'
grid.fit(a_scaled_samples)

kde = neighbors.KernelDensity(**grid.best_params_)
kde.fit(a_scaled_samples)


# get test samples
a_test = kde.sample(300000)
a_test_transformed = scaler.inverse_transform(a_test)

a_weights = 1. / ( 1. + np.exp(-(a_test_transformed[:,0]-1.96)/4.67e-1) )
a_weights *= (1. - np.exp(-(a_test_transformed[:,1]-110.)/375.86))
a_weights = np.maximum(0.001, a_weights)
a_weights = np.divide(1., a_weights)



fig_kde, ax_kde = plt.subplots(1)
a_counts, kde_s1_x_edges, kde_s2_edges, dummy3 = ax_kde.hist2d(a_test_transformed[:,0], a_test_transformed[:,1], bins=[kde_nb, kde_nb], range=[[s1_min, s1_max], [s2_min, s2_max]], weights=a_weights)



a_counts.shape

a_kde_s1_bin_edges = np.linspace(s1_min, s1_max, kde_nb+1)
a_kde_s1_y_values = np.sum(a_counts, axis=1)

a_kde_s1_x_values, a_kde_s1_y_values, a_kde_s1_x_err_low, a_kde_s1_x_err_high, a_kde_s1_y_err_low, a_kde_s1_y_err_high = neriX_analysis.prepare_hist_arrays_for_plotting(a_kde_s1_y_values, a_kde_s1_bin_edges)

ax_s1.errorbar(a_kde_s1_x_values, a_kde_s1_y_values, xerr=[a_kde_s1_x_err_low, a_kde_s1_x_err_high], yerr=[a_kde_s1_y_err_low, a_kde_s1_y_err_high], marker='.', color='b', linestyle='')




a_kde_s2_bin_edges = np.linspace(s2_min, s2_max, kde_nb+1)
a_kde_s2_y_values = np.sum(a_counts, axis=0)

a_kde_s2_x_values, a_kde_s2_y_values, a_kde_s2_x_err_low, a_kde_s2_x_err_high, a_kde_s2_y_err_low, a_kde_s2_y_err_high = neriX_analysis.prepare_hist_arrays_for_plotting(a_kde_s2_y_values, a_kde_s2_bin_edges)

ax_s2.errorbar(a_kde_s2_x_values, a_kde_s2_y_values, xerr=[a_kde_s2_x_err_low, a_kde_s2_x_err_high], yerr=[a_kde_s2_y_err_low, a_kde_s2_y_err_high], marker='.', color='b', linestyle='')




plt.show()



