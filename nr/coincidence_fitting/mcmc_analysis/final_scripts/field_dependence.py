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
import matplotlib.pyplot as plt
import matplotlib.dates as mdates
import matplotlib.patches as mpatches
from matplotlib.colors import LogNorm

import tqdm

from rootpy import stl
from rootpy.io import File
from rootpy.tree import Tree, TreeModel, TreeChain
from rootpy.plotting import Hist, Hist2D, Canvas, Graph, func


from math import floor

from scipy.stats import norm, chi2
import scipy.optimize as op
import cPickle as pickle

from scipy.special import gammaln


l_cathode_settings = [0.345, 1.054, 2.356]

d_parameters = {}

d_parameters['sigma'] = {}
#d_parameters['sigma'][0.345] = {'med':8.49e-03, 'lb':-7.00e-04, 'ub':6.43e-04, 'f':0.19}
#d_parameters['sigma'][1.054] = {'med':8.67e-03, 'lb':-6.41e-04, 'ub':6.49e-04, 'f':0.49}
#d_parameters['sigma'][2.356] = {'med':9.04e-03, 'lb':-6.73e-04, 'ub':7.05e-04, 'f':1.02}
d_parameters['sigma'][0.345] = {'med':8.37e-03, 'lb':-4.08e-04, 'ub':4.14e-04, 'f':0.19}
d_parameters['sigma'][1.054] = {'med':8.64e-03, 'lb':-4.10e-04, 'ub':4.69e-04, 'f':0.49}
d_parameters['sigma'][2.356] = {'med':9.04e-03, 'lb':-4.00e-04, 'ub':4.60e-04, 'f':1.02}

d_parameters['alpha'] = {}
#d_parameters['alpha'][0.345] = {'med':1.06e+00, 'lb':-6.48e-02, 'ub':7.81e-02, 'f':0.19}
#d_parameters['alpha'][1.054] = {'med':1.06e+00, 'lb':-6.65e-02, 'ub':7.83e-02, 'f':0.49}
#d_parameters['alpha'][2.356] = {'med':9.93e-01, 'lb':-6.29e-02, 'ub':6.59e-02, 'f':1.02}
d_parameters['alpha'][0.345] = {'med':1.10e+00, 'lb':-5.59e-02, 'ub':5.62e-02, 'f':0.19}
d_parameters['alpha'][1.054] = {'med':1.07e+00, 'lb':-5.48e-02, 'ub':6.60e-02, 'f':0.49}
d_parameters['alpha'][2.356] = {'med':9.94e-01, 'lb':-5.21e-02, 'ub':5.48e-02, 'f':1.02}




def neg_ll_constant(a_par, b_alpha):
    par0, = a_par

    val1 = par0
    val2 = par0
    val3 = par0
    
    if b_alpha:
        key = 'alpha'
    else:
        key = 'sigma'

    if val1 < d_parameters[key][0.345]['med']:
        std1 = np.abs(d_parameters[key][0.345]['lb'])
    else:
        std1 = np.abs(d_parameters[key][0.345]['ub'])


    if val2 < d_parameters[key][1.054]['med']:
        std2 = np.abs(d_parameters[key][1.054]['lb'])
    else:
        std2 = np.abs(d_parameters[key][1.054]['ub'])


    if val3 < d_parameters[key][2.356]['med']:
        std3 = np.abs(d_parameters[key][2.356]['lb'])
    else:
        std3 = np.abs(d_parameters[key][2.356]['ub'])

    #print (norm.logpdf(val1, d_parameters[key][0.345]['med'], std1) + norm.logpdf(val2, d_parameters[key][1.054]['med'], std2) + norm.logpdf(val3, d_parameters[key][2.356]['med'], std3))
    return -(norm.logpdf(val1, d_parameters[key][0.345]['med'], std1) + norm.logpdf(val2, d_parameters[key][1.054]['med'], std2) + norm.logpdf(val3, d_parameters[key][2.356]['med'], std3))




def neg_ll_pol1(a_par, b_alpha):
    par0, par1 = a_par
    #print par0, par1
    
    if b_alpha:
        key = 'alpha'
    else:
        key = 'sigma'

    val1 = par0 + d_parameters[key][0.345]['f']*par1
    val2 = par0 + d_parameters[key][1.054]['f']*par1
    val3 = par0 + d_parameters[key][2.356]['f']*par1

    if val1 < d_parameters[key][0.345]['med']:
        std1 = np.abs(d_parameters[key][0.345]['lb'])
    else:
        std1 = np.abs(d_parameters[key][0.345]['ub'])


    if val2 < d_parameters[key][1.054]['med']:
        std2 = np.abs(d_parameters[key][1.054]['lb'])
    else:
        std2 = np.abs(d_parameters[key][1.054]['ub'])


    if val3 < d_parameters[key][2.356]['med']:
        std3 = np.abs(d_parameters[key][2.356]['lb'])
    else:
        std3 = np.abs(d_parameters[key][2.356]['ub'])

    #print (norm.logpdf(val1, d_parameters[key][0.345]['med'], std1) + norm.logpdf(val2, d_parameters[key][1.054]['med'], std2) + norm.logpdf(val3, d_parameters[key][2.356]['med'], std3))
    return -(norm.logpdf(val1, d_parameters[key][0.345]['med'], std1) + norm.logpdf(val2, d_parameters[key][1.054]['med'], std2) + norm.logpdf(val3, d_parameters[key][2.356]['med'], std3))





def neg_ll_pow(a_par, b_alpha):
    par0, par1 = a_par
    #print par0, par1
    
    if b_alpha:
        key = 'alpha'
    else:
        key = 'sigma'

    val1 = par0 * d_parameters[key][0.345]['f']**par1
    val2 = par0 * d_parameters[key][1.054]['f']**par1
    val3 = par0 * d_parameters[key][2.356]['f']**par1

    if val1 < d_parameters[key][0.345]['med']:
        std1 = np.abs(d_parameters[key][0.345]['lb'])
    else:
        std1 = np.abs(d_parameters[key][0.345]['ub'])


    if val2 < d_parameters[key][1.054]['med']:
        std2 = np.abs(d_parameters[key][1.054]['lb'])
    else:
        std2 = np.abs(d_parameters[key][1.054]['ub'])


    if val3 < d_parameters[key][2.356]['med']:
        std3 = np.abs(d_parameters[key][2.356]['lb'])
    else:
        std3 = np.abs(d_parameters[key][2.356]['ub'])

    #print (norm.logpdf(val1, d_parameters[key][0.345]['med'], std1) + norm.logpdf(val2, d_parameters[key][1.054]['med'], std2) + norm.logpdf(val3, d_parameters[key][2.356]['med'], std3))
    return -(norm.logpdf(val1, d_parameters[key][0.345]['med'], std1) + norm.logpdf(val2, d_parameters[key][1.054]['med'], std2) + norm.logpdf(val3, d_parameters[key][2.356]['med'], std3))




# use result.fun[0] to get neg_ll and result.x to get value

fit_results_const_alpha = op.differential_evolution(neg_ll_constant, [(0.00001, 2)], args=[True])
fit_results_pol1_alpha = op.differential_evolution(neg_ll_pol1, [(0.00001, 2), (-2, 2)], args=[True])
fit_results_pow_alpha = op.differential_evolution(neg_ll_pow, [(0.00001, 2), (-2, 2)], args=[True])


d_stat_alpha_pol1 = -2*(fit_results_pol1_alpha.fun - fit_results_const_alpha.fun)
print 'p const vs pol1, alpha: %.5f' % (1 - chi2.cdf(d_stat_alpha_pol1, 1))
print fit_results_const_alpha.fun
print fit_results_pol1_alpha.fun
print '\n\n'

d_stat_alpha_pow = -2*(fit_results_pow_alpha.fun - fit_results_const_alpha.fun)
print 'p const vs pow, alpha: %.5f' % (1 - chi2.cdf(d_stat_alpha_pow, 1))
print fit_results_const_alpha.fun
print fit_results_pow_alpha.fun
print '\n\n'




fit_results_const_sigma = op.differential_evolution(neg_ll_constant, [(0.00001, 2)], args=[False])
fit_results_pol1_sigma = op.differential_evolution(neg_ll_pol1, [(0.00001, 2), (-2, 2)], args=[False])
fit_results_pow_sigma = op.differential_evolution(neg_ll_pow, [(0.00001, 2), (-2, 2)], args=[False])


d_stat_sigma_pol1 = -2*(fit_results_pol1_sigma.fun - fit_results_const_sigma.fun)
print 'p const vs pol1, sigma: %.5f' % (1 - chi2.cdf(d_stat_sigma_pol1, 1))
print fit_results_const_sigma.fun
print fit_results_pol1_sigma.fun
print fit_results_const_sigma.x
print fit_results_pol1_sigma.x
print '\n\n'

d_stat_sigma_pow = -2*(fit_results_pow_sigma.fun - fit_results_const_sigma.fun)
print 'p const vs pow, sigma: %.5f' % (1 - chi2.cdf(d_stat_sigma_pow, 1))
print fit_results_const_sigma.fun
print fit_results_pow_sigma.fun
print fit_results_pow_sigma.x
print '\n\n'




