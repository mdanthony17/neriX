#!/usr/bin/python
import matplotlib
matplotlib.use('QT4Agg')
import ROOT as root
from ROOT import gROOT
import sys, os, root_numpy, threading, random, emcee, corner, signal, re
from rootpy import stl
from rootpy.io import File
from rootpy.tree import Tree, TreeModel, TreeChain
from rootpy.plotting import Hist, Hist2D, Canvas, Graph, func
import numpy as np
import neriX_analysis
from math import exp, factorial, erf, ceil, log, pow, floor
from scipy import optimize, misc, stats
from scipy.stats import norm
import copy_reg, types, pickle, click, time
from subprocess import call
from tqdm import tqdm

import hpge_fit_results

d_run_avg = {10:{}, 11:{}}
for run in d_run_avg:
	d_run_avg[run]['sum_inv_cov_matrix'] = 0
	d_run_avg[run]['sum_inv_cov_matrix_dot_mean_vector'] = 0

d_fit_results = hpge_fit_results.d_fit_results

for file in d_fit_results:
	current_run = d_fit_results[file]['run']
	current_mean_vector = np.asarray(d_fit_results[file]['l_best_fit_pars'])
	current_cov_matrix = np.asarray(d_fit_results[file]['l_cov_matrix'])

	current_inv_cov_matrix = np.linalg.pinv(current_cov_matrix)
	current_inv_cov_matrix_dot_mean_vector = np.dot(current_inv_cov_matrix, current_mean_vector)

	if d_run_avg[current_run]['sum_inv_cov_matrix'] is int:
		d_run_avg[current_run]['sum_inv_cov_matrix'] = current_inv_cov_matrix
		d_run_avg[current_run]['sum_inv_cov_matrix_dot_mean_vector'] = current_inv_cov_matrix_dot_mean_vector
	else:
		d_run_avg[current_run]['sum_inv_cov_matrix'] += current_inv_cov_matrix
		d_run_avg[current_run]['sum_inv_cov_matrix_dot_mean_vector'] += current_inv_cov_matrix_dot_mean_vector


for run in d_run_avg:
	d_run_avg[run]['combined_cov_matrix'] = np.linalg.pinv(d_run_avg[run]['sum_inv_cov_matrix'])
	d_run_avg[run]['combined_mean_vector'] = np.dot(d_run_avg[run]['combined_cov_matrix'], d_run_avg[run]['sum_inv_cov_matrix_dot_mean_vector'])

	print '\n\nRun %d HPGe calibration: ' % run
	print d_run_avg[run]['combined_mean_vector']
	print d_run_avg[run]['combined_cov_matrix']
	print 'Intercept: %.4f +/- %.4f' % (d_run_avg[run]['combined_mean_vector'][0], d_run_avg[run]['combined_cov_matrix'][0, 0]**0.5)
	print 'Slope: %.4f +/- %.4f' % (d_run_avg[run]['combined_mean_vector'][1], d_run_avg[run]['combined_cov_matrix'][1, 1]**0.5)
	print 'Correlation: %.2f' % (d_run_avg[run]['combined_cov_matrix'][1, 0]/(d_run_avg[run]['combined_cov_matrix'][0, 0]**0.5*d_run_avg[run]['combined_cov_matrix'][1, 1]**0.5))


