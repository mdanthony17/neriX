#!/usr/bin/python
#import pickle
#print pickle.Pickler.dispatch
import dill
#print pickle.Pickler.dispatch

import ROOT as root
import sys, os

import matplotlib
matplotlib.use('QT4Agg')
import matplotlib.pyplot as plt

import emcee, corner, click
import neriX_analysis, neriX_datasets, neriX_config
from rootpy.plotting import Hist2D, Hist, Legend, Canvas
from rootpy.tree import Tree
from rootpy.io import File
from rootpy import stl
import numpy as np
import tqdm, time, copy_reg, types, pickle
from root_numpy import tree2array, array2tree
import scipy.optimize as op
import scipy.special
from scipy.stats import norm, poisson
from scipy.special import erf



def mc_pmt_response(a_mc, mean_num_pe, prob_hit_first, mean_e_from_dynode, bkg_mean, bkg_std):
    a_binom = np.random.binomial(1, prob_hit_first, len(a_mc))
    for event_num in xrange(len(a_mc)):
    
        # decrement the dynode number if "missed"
        num_dynodes = 12
        if a_binom[event_num] == 0:
            num_dynodes -= 1

        tot_num_electrons = np.random.poisson(lam=mean_num_pe)
        if tot_num_electrons != 0:
            for i in xrange(num_dynodes):
                tot_num_electrons = np.random.poisson(lam=tot_num_electrons*mean_e_from_dynode)

        tot_num_electrons += np.random.normal(loc=bkg_mean, scale=bkg_std)
        a_mc[event_num] = tot_num_electrons






if __name__ == '__main__':
    a_mc = np.zeros(1e5)
    mc_pmt_response(a_mc, 0.1, 0.95, 3.2, 1e4, 2e5)
    #print a_mc

    l_charge_settings_spe = [150, -5e5, 2e6]
    a_spe_bin_edges = np.linspace(l_charge_settings_spe[1], l_charge_settings_spe[2], l_charge_settings_spe[0]+1) # need +1 for bin edges
    bin_width = a_spe_bin_edges[1] - a_spe_bin_edges[0]
    a_spe_bin_centers = np.linspace(l_charge_settings_spe[1]+bin_width/2., l_charge_settings_spe[2]-bin_width/2., l_charge_settings_spe[0])

    a_hist, dummy = np.histogram(a_mc, bins=a_spe_bin_edges)

    f1, (ax1) = plt.subplots(1)
    ax1.plot(a_spe_bin_centers, a_hist, 'b.')
    #ax1.set_yscale('log', nonposx='clip')

    plt.show()






