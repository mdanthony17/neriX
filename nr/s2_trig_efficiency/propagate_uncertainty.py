#!/usr/bin/python

import ROOT as root
import sys, array, math
import neriX_analysis
from rootpy.plotting import Hist, Hist2D, Canvas, Legend
from rootpy.io import root_open, File
import numpy as np
from tqdm import tqdm
import root_numpy
from math import exp

from scipy.stats import multivariate_normal

import matplotlib
matplotlib.use('QT4Agg')
import matplotlib.pyplot as plt
import matplotlib.dates as mdates
import matplotlib.patches as mpatches
import matplotlib.mlab as mlab
import matplotlib.lines as mlines




l_means = []
l_means.append(np.asarray([118.71406758, 428.12577677]).T)
l_means.append(np.asarray([105.48908001, 371.76309815]).T)

l_covariance_matrices = []
l_covariance_matrices.append(np.asarray([[1964.21611655, -1912.58582097], [-1912.58582097, 4209.08207196]]))
l_covariance_matrices.append(np.asarray([[1370.72669531, -1063.78007348], [-1063.78007348, 1611.49007206]]))

l_inverse_covariance_matrices = []
for matrix in l_covariance_matrices:
    l_inverse_covariance_matrices.append(np.linalg.pinv(matrix))
    #print matrix
    #print np.linalg.pinv(matrix)
    #print np.dot(matrix, np.linalg.pinv(matrix))

a_inv_sum = np.zeros(l_covariance_matrices[0].shape)
a_inv_times_mean_sum = np.zeros(np.dot(l_inverse_covariance_matrices[0], l_means[0]).shape)

for i, matrix in enumerate(l_inverse_covariance_matrices):
    a_inv_sum += matrix
    a_inv_times_mean_sum += np.dot(matrix, l_means[i])

a_weighted_covariance_matrix = np.linalg.pinv(a_inv_sum)
a_weighted_mean = np.dot(a_weighted_covariance_matrix, a_inv_times_mean_sum)

print a_weighted_mean
print a_weighted_covariance_matrix

num_pts = 50
x_range = [50, 150]
y_range = [300, 500]

x = np.linspace(x_range[0], x_range[1], num_pts)
y = np.linspace(y_range[0], y_range[1], num_pts)

X, Y = np.meshgrid(x, y)
x_flat = X.flatten()
y_flat = Y.flatten()

mvn_one = multivariate_normal(mean=l_means[0], cov=l_covariance_matrices[0])
mvn_two = multivariate_normal(mean=l_means[1], cov=l_covariance_matrices[1])
mvn_combined = multivariate_normal(mean=a_weighted_mean, cov=a_weighted_covariance_matrix)

f1, ax1 = plt.subplots(1)


#Z1 = mlab.bivariate_normal(X, Y, sigmax=l_covariance_matrices[0][0, 0], sigmay=l_covariance_matrices[0][1, 1, mux=])

z_one = mvn_one.pdf(np.asarray([x_flat, y_flat]).T)
z_one = z_one.reshape(num_pts, num_pts)

z_two = mvn_two.pdf(np.asarray([x_flat, y_flat]).T)
z_two = z_two.reshape(num_pts, num_pts)

z_combined = mvn_combined.pdf(np.asarray([x_flat, y_flat]).T)
z_combined = z_combined.reshape(num_pts, num_pts)

c_true = ax1.contour(X, Y, z_one, cmap='Blues')
c_random = ax1.contour(X, Y, z_two, cmap='Greens')
c_combined = ax1.contour(X, Y, z_combined, cmap='Reds')

ax1.set_title('Method Fit Comparison S2 Trigger Efficiency')
ax1.set_xlabel('Center Parameter [PE]')
ax1.set_ylabel('Shape Parameter [PE]')

line_true = mlines.Line2D([], [], color='blue', label='True Coincidences')
line_random = mlines.Line2D([], [], color='green', label='Random Coincidences')
line_combined = mlines.Line2D([], [], color='red', label='Combined Fits')

ax1.legend(handles=[line_true, line_random, line_combined])


#plt.show()

neriX_analysis.save_figure(['results'], f1, 'trig_efficiency_parameter_contours')





