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
from math import floor

import cuda_pmt_mc
from pycuda.compiler import SourceModule
import pycuda.driver as drv
import pycuda.tools
import pycuda.gpuarray
import pycuda.autoinit

gpu_pmt_mc = SourceModule(cuda_pmt_mc.cuda_pmt_mc, no_extern_c=True).get_function('gpu_pmt_mc')
setup_kernel = SourceModule(cuda_pmt_mc.cuda_pmt_mc, no_extern_c=True).get_function('setup_kernel')


def reduce_method(m):
    return (getattr, (m.__self__, m.__func__.__name__))



def poisson_binned_likelihood(a_model, a_data):
    #print a_data*np.log(a_model) - a_model
    return np.sum(a_data*np.log(a_model) - a_model)



def bkg_only_model(a_x, omega, bkg_mean, bkg_std, alpha):
    term1 = (1-omega)/(2*np.pi*bkg_std**2)**0.5*np.exp(-0.5*(a_x-bkg_mean)**2/bkg_std**2)
    
    # need to be careful of overflow in term 2
    # if heaviside gives zero do not compute exponential
    heaviside = (0.5 * (np.sign(a_x-bkg_mean) + 1))
    term2 = omega*heaviside*alpha*np.exp(-alpha*a_x*heaviside)
    #print alpha
    #term2 = omega*(0.5 * (np.sign(a_x-bkg_mean) + 1))
    #for i, item in enumerate(term2):
    #    if item != 0.:
    #        item *= np.exp(-alpha*a_x[i])
    
    return term1 + term2



def full_model(a_x, omega, bkg_mean, bkg_std, alpha, avg_num_pe, spe_mean, spe_std, max_num_pe):
    # max num pe caps the sum to reasonable contributions
    
    a_results = np.zeros(len(a_x))

    for num_pe in xrange(max_num_pe):
        poisson_term = poisson.pmf(num_pe, avg_num_pe)

        adjusted_mean = bkg_mean + num_pe*spe_mean
        adjusted_std = (bkg_std**2 + num_pe*spe_std**2)**0.5

        term1 = (1-omega)/(2*np.pi*adjusted_std**2)**0.5*np.exp(-0.5*(a_x-adjusted_mean)**2/adjusted_std**2)

        term2 = omega * alpha/2. * np.exp(-alpha*(a_x-adjusted_mean-alpha*adjusted_std**2/2.)) * ( erf(np.absolute(bkg_mean-adjusted_mean-adjusted_std**2*alpha)/(2**0.5*adjusted_std)) + np.sign(a_x-adjusted_mean-adjusted_std**2*alpha)*erf(np.absolute(a_x-adjusted_mean-adjusted_std**2*alpha)/(2**0.5*adjusted_std)))

        a_results += poisson_term * (term1 + term2)

    return a_results



def mc_model(a_mc, mean_num_pe, prob_hit_first, mean_e_from_dynode, bkg_mean, bkg_std):
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






class fit_pmt_gain(object):
    def __init__(self, d_fit_filenames, run=16, channel_number=17, num_mc_events=1e6):
    
        # d_fit_files should be of the form
        # d_fit_files['bkg'] = <bkg file name>
        # d_fit_files['spe'] = <spe file name>

        self.run = run

        # make class methods pickleable for multithreading process
        copy_reg.pickle(types.MethodType, reduce_method)

        self.channel_number = channel_number
        self.parameter_to_examine = 'SingleIntegral[%d]' % (self.channel_number - 1)
        
        
        self.num_mc_events = int(num_mc_events)
        self.d_gpu_scale = {}
        block_dim = 1024
        self.d_gpu_scale['block'] = (block_dim,1,1)
        numBlocks = floor(num_mc_events / float(block_dim))
        self.d_gpu_scale['grid'] = (int(numBlocks), 1)
        self.num_mc_events = int(numBlocks*block_dim)
        
        
        seed = int(time.time())
        self.rng_states = drv.mem_alloc(self.num_mc_events*pycuda.characterize.sizeof('curandStateXORWOW', '#include <curand_kernel.h>'))
        setup_kernel(np.int32(self.num_mc_events), self.rng_states, np.uint64(seed), np.uint64(0), **self.d_gpu_scale)
        print 'Cuda random states setup...\n'
        

        self.d_fit_filenames = d_fit_filenames
        self.d_fit_files = {}
        self.d_fit_files['bkg'] = {}
        self.d_fit_files['spe'] = {}
        self.d_fit_files['mpe1'] = {}
        self.d_fit_files['bkg']['file'] = File(neriX_config.pathToData + 'run_16/' + self.d_fit_filenames['bkg'] + '.root')
        self.d_fit_files['spe']['file'] = File(neriX_config.pathToData + 'run_16/' + self.d_fit_filenames['spe'] + '.root')
        self.d_fit_files['mpe1']['file'] = File(neriX_config.pathToData + 'run_16/' + self.d_fit_filenames['mpe1'] + '.root')

        self.d_fit_files['bkg']['tree'] = self.d_fit_files['bkg']['file'].T0
        self.d_fit_files['spe']['tree'] = self.d_fit_files['spe']['file'].T0
        self.d_fit_files['mpe1']['tree'] = self.d_fit_files['mpe1']['file'].T0

        self.l_charge_settings_spe = [50, -2e5, 2e6]
        self.l_charge_settings_mpe = [50, -2e5, 5e6]
        self.max_num_pe_spe = 3
        self.max_num_pe_mpe = 12
        self.a_spe_bin_edges = np.linspace(self.l_charge_settings_spe[1], self.l_charge_settings_spe[2], self.l_charge_settings_spe[0]+1) # need +1 for bin edges
        bin_width = self.a_spe_bin_edges[1] - self.a_spe_bin_edges[0]
        self.a_spe_bin_centers = np.linspace(self.l_charge_settings_spe[1]+bin_width/2., self.l_charge_settings_spe[2]-bin_width/2., self.l_charge_settings_spe[0])
        
        self.a_mpe_bin_edges = np.linspace(self.l_charge_settings_spe[1], self.l_charge_settings_mpe[2], self.l_charge_settings_mpe[0]+1) # need +1 for bin edges
        bin_width = self.a_mpe_bin_edges[1] - self.a_mpe_bin_edges[0]
        self.a_mpe_bin_centers = np.linspace(self.l_charge_settings_mpe[1]+bin_width/2., self.l_charge_settings_mpe[2]-bin_width/2., self.l_charge_settings_mpe[0])

        self.d_fit_files['bkg']['array'] = tree2array(self.d_fit_files['bkg']['tree'], [self.parameter_to_examine])
        self.d_fit_files['bkg']['array'] = np.array(filter(lambda x: x < self.l_charge_settings_spe[2] and x > self.l_charge_settings_spe[1], self.d_fit_files['bkg']['array'][self.parameter_to_examine]))

        self.d_fit_files['spe']['array'] = tree2array(self.d_fit_files['spe']['tree'], [self.parameter_to_examine])
        self.d_fit_files['spe']['array'] = np.array(filter(lambda x: x < self.l_charge_settings_spe[2] and x > self.l_charge_settings_spe[1], self.d_fit_files['spe']['array'][self.parameter_to_examine]))
        
        self.d_fit_files['mpe1']['array'] = tree2array(self.d_fit_files['mpe1']['tree'], [self.parameter_to_examine])
        self.d_fit_files['mpe1']['array'] = np.array(filter(lambda x: x < self.l_charge_settings_mpe[2] and x > self.l_charge_settings_mpe[1], self.d_fit_files['mpe1']['array'][self.parameter_to_examine]))
        
        
        self.d_fit_files['bkg']['hist'], dummy = np.histogram(self.d_fit_files['bkg']['array'], bins=self.a_spe_bin_edges)
        
        self.d_fit_files['spe']['hist'], dummy = np.histogram(self.d_fit_files['spe']['array'], bins=self.a_spe_bin_edges)
        
        self.d_fit_files['mpe1']['hist'], dummy = np.histogram(self.d_fit_files['mpe1']['array'], bins=self.a_mpe_bin_edges)
    
    
        self.b_suppress_likelihood = False
    
    
        #print self.d_fit_files['spe']['hist']
        
        
        
    def omega_prior(self, omega):
        if 0 < omega < 1:
            return 0
        else:
            return -np.inf



    def alpha_prior(self, alpha):
        if 0 < alpha < 1:
            return 0
        else:
            return -np.inf



    def std_prior(self, std):
        if std > 0:
            return 0
        else:
            return -np.inf



    def bkg_ln_likelihood(self, a_parameters):
        omega, bkg_mean, bkg_std, ln_alpha = a_parameters
        alpha = np.exp(ln_alpha)

        ln_prior = 0
        ln_likelihood = 0

        ln_prior += self.omega_prior(omega)
        ln_prior += self.alpha_prior(alpha)
        ln_prior += self.std_prior(bkg_std)

        if not np.isfinite(ln_prior):
            return -np.inf

        # unbinned had overflow issues
        #a_ln_likelihood = (np.log(1-omega) - 0.5*np.log(2*np.pi*bkg_std**2) - 0.5*(self.d_fit_files['bkg']['array']-bkg_mean)**2/bkg_std**2) + np.log1p( (omega/(1-omega))*(0.5*(np.sign(self.d_fit_files['bkg']['array']-bkg_mean)+1))*alpha*(2*np.pi*bkg_std**2)**0.5*np.exp(-alpha*self.d_fit_files['bkg']['array'] + 0.5*(self.d_fit_files['bkg']['array']-bkg_mean)**2/bkg_std**2) )
        #return np.sum(a_ln_likelihood)
        
        a_model = bkg_only_model(self.a_spe_bin_centers, omega, bkg_mean, bkg_std, alpha)
        a_model *= np.sum(self.d_fit_files['bkg']['hist']) / np.sum(a_model)
        
        ln_likelihood += poisson_binned_likelihood(a_model, self.d_fit_files['bkg']['hist'])

        if np.isnan(ln_likelihood):
            return -np.inf
        else:
            return ln_prior + ln_likelihood



    def run_bkg_only_mcmc(self, num_walkers=128, num_steps=2000, threads=1, burn_in=1000):

        if num_steps < burn_in:
            'Number of steps is less than the burn-in period, please increase the number of steps or reduce your burn-in period.\n'
            sys.exit()
        
        l_value_guesses = [0.01, 1e4, 2e5, -14]
        l_std_guesses = [0.01, 5e3, 5e4, 2]
        l_par_names = ['omega', 'bkg_mean', 'bkg_std', 'ln_alpha']

        s_base_save_name = 'bkg_only_fit'
        s_directory_save_name = 'results/%s/' % (self.d_fit_filenames['bkg'])
        if not os.path.exists(s_directory_save_name):
            os.makedirs(s_directory_save_name)
        num_dim = len(l_value_guesses)

        a_starting_pos = emcee.utils.sample_ball(l_value_guesses, l_std_guesses, size=num_walkers)

        #sampler = emcee.EnsembleSampler(num_walkers, num_dim, self.bkg_ln_likelihood, threads=threads)
        sampler = emcee.DESampler(num_walkers, num_dim, self.bkg_ln_likelihood, threads=threads, autoscale_gamma=True)

        with click.progressbar(sampler.sample(a_starting_pos, iterations=num_steps, ), length=num_steps) as mcmc_sampler:
            for pos, lnprob, state in mcmc_sampler:
                pass


        samples = sampler.chain[:, burn_in:, :].reshape((-1, num_dim))
        fig = corner.corner(samples, labels=l_par_names, quantiles=[0.16, 0.5, 0.84], show_titles=True, title_fmt='.3e', title_kwargs={'fontsize': 12})
        
        try:
            print emcee.autocorr.integrated_time(np.mean(sampler.chain[:, burn_in:, :], axis=0), axis=0, low=10, high=None, step=1, c=2, fast=False)
        except:
            print '\nCould not get autocorrelation - try longer chain...\n'

        print sampler.acceptance_fraction


        if not os.path.exists(s_directory_save_name):
            os.makedirs(s_directory_save_name)
        fig.savefig(s_directory_save_name + '/' + s_base_save_name + '_corner.png')
        


    def draw_bkg_only_fit(self, a_parameters):
        omega, bkg_mean, bkg_std, ln_alpha = a_parameters
        alpha = np.exp(ln_alpha)
    
        a_model = bkg_only_model(self.a_spe_bin_centers, omega, bkg_mean, bkg_std, alpha)
        #print a_model
        a_model *= np.sum(self.d_fit_files['bkg']['hist']) / np.sum(a_model)
    
        f, (ax1) = plt.subplots(1)
        ax1.set_yscale('log', nonposx='clip')
    
        ax1.plot(self.a_spe_bin_centers, self.d_fit_files['bkg']['hist'], 'b.')
        ax1.plot(self.a_spe_bin_centers, a_model, 'g-')
    
        plt.show()
    
    
    
    def full_model_ln_likelihood(self, a_parameters):
        omega, bkg_mean, bkg_std, ln_alpha, avg_num_pe_spe, spe_mean, spe_std = a_parameters
        #omega, bkg_mean, bkg_std, ln_alpha, avg_num_pe_spe, spe_mean, spe_std, avg_num_pe_mpe1 = a_parameters
        alpha = np.exp(ln_alpha)

        ln_prior = 0
        ln_likelihood = 0

        ln_prior += self.omega_prior(omega)
        ln_prior += self.alpha_prior(alpha)
        ln_prior += self.std_prior(bkg_std)
        ln_prior += self.std_prior(avg_num_pe_spe) # just require > 0
        #ln_prior += self.std_prior(avg_num_pe_mpe1) # just require > 0
        ln_prior += self.std_prior(spe_std)

        if not np.isfinite(ln_prior):
            return -np.inf

        a_model_bkg = bkg_only_model(self.a_spe_bin_centers, omega, bkg_mean, bkg_std, alpha)
        a_model_bkg *= np.sum(self.d_fit_files['bkg']['hist']) / np.sum(a_model_bkg)
        ln_likelihood += poisson_binned_likelihood(a_model_bkg, self.d_fit_files['bkg']['hist'])

        a_model_spe = full_model(self.a_spe_bin_centers, omega, bkg_mean, bkg_std, alpha, avg_num_pe_spe, spe_mean, spe_std, self.max_num_pe_spe)
        a_model_spe *= np.sum(self.d_fit_files['spe']['hist']) / np.sum(a_model_spe)
        ln_likelihood += poisson_binned_likelihood(a_model_spe, self.d_fit_files['spe']['hist'])
        
        #a_model_mpe1 = full_model(self.a_mpe_bin_centers, omega, bkg_mean, bkg_std, alpha, avg_num_pe_mpe1, spe_mean, spe_std, self.max_num_pe_mpe)
        #a_model_mpe1 *= np.sum(self.d_fit_files['mpe1']['hist']) / np.sum(a_model_mpe1)
        #ln_likelihood += poisson_binned_likelihood(a_model_mpe1, self.d_fit_files['mpe1']['hist'])

        if np.isnan(ln_likelihood):
            return -np.inf
        else:
            return ln_prior + ln_likelihood



    def run_full_model_mcmc(self, num_walkers=128, num_steps=2000, threads=1, burn_in=1000):

        if num_steps < burn_in:
            'Number of steps is less than the burn-in period, please increase the number of steps or reduce your burn-in period.\n'
            sys.exit()
        
        l_value_guesses = [0.005, 1.19e4, 2.e5, -14, 0.05, 8e5, 6e5]
        l_std_guesses = [0.001, 3e3, 4e4, 2, 0.02, 1e5, 1e5]
        l_par_names = ['omega', 'bkg_mean', 'bkg_std', 'ln_alpha', 'avg_num_pe_spe', 'spe_mean', 'spe_std']

        #l_value_guesses = [0.005, 1.19e4, 2.e5, -14, 0.05, 8e5, 6e5, 1.4]
        #l_std_guesses = [0.001, 3e3, 4e4, 2, 0.02, 1e5, 1e5, 0.3]
        #l_par_names = ['omega', 'bkg_mean', 'bkg_std', 'ln_alpha', 'avg_num_pe_spe', 'spe_mean', 'spe_std', 'avg_num_pe_mpe']
        
        s_base_save_name = 'full_model_fit'
        s_directory_save_name = 'results/%s/' % (self.d_fit_filenames['bkg'])
        if not os.path.exists(s_directory_save_name):
            os.makedirs(s_directory_save_name)
        num_dim = len(l_value_guesses)

        a_starting_pos = emcee.utils.sample_ball(l_value_guesses, l_std_guesses, size=num_walkers)

        #sampler = emcee.EnsembleSampler(num_walkers, num_dim, self.full_model_ln_likelihood, threads=threads)
        sampler = emcee.DESampler(num_walkers, num_dim, self.full_model_ln_likelihood, threads=threads, autoscale_gamma=True)

        with click.progressbar(sampler.sample(a_starting_pos, iterations=num_steps, ), length=num_steps) as mcmc_sampler:
            for pos, lnprob, state in mcmc_sampler:
                pass


        samples = sampler.chain[:, burn_in:, :].reshape((-1, num_dim))
        fig = corner.corner(samples, labels=l_par_names, quantiles=[0.16, 0.5, 0.84], show_titles=True, title_fmt='.3e', title_kwargs={'fontsize': 12})
        
        try:
            print emcee.autocorr.integrated_time(np.mean(sampler.chain[:, burn_in:, :], axis=0), axis=0, low=10, high=None, step=1, c=2, fast=False)
        except:
            print '\nCould not get autocorrelation - try longer chain...\n'

        print sampler.acceptance_fraction


        if not os.path.exists(s_directory_save_name):
            os.makedirs(s_directory_save_name)
        fig.savefig(s_directory_save_name + '/' + s_base_save_name + '_corner.png')
    
    
    
    def draw_full_model_fit(self, a_parameters):
        omega, bkg_mean, bkg_std, ln_alpha, avg_num_pe_spe, spe_mean, spe_std = a_parameters
        #omega, bkg_mean, bkg_std, ln_alpha, avg_num_pe_spe, spe_mean, spe_std, avg_num_pe_mpe1 = a_parameters
        alpha = np.exp(ln_alpha)
    
        a_model_spe = full_model(self.a_spe_bin_centers, omega, bkg_mean, bkg_std, alpha, avg_num_pe_spe, spe_mean, spe_std, self.max_num_pe_spe)
        a_model_spe *= np.sum(self.d_fit_files['spe']['hist']) / np.sum(a_model_spe)
        
        #a_model_mpe1 = full_model(self.a_mpe_bin_centers, omega, bkg_mean, bkg_std, alpha, avg_num_pe_mpe1, spe_mean, spe_std, self.max_num_pe_mpe)
        #a_model_mpe1 *= np.sum(self.d_fit_files['mpe1']['hist']) / np.sum(a_model_mpe1)
    
        f1, (ax1) = plt.subplots(1)
        #ax1.set_yscale('log', nonposx='clip')
    
        ax1.plot(self.a_spe_bin_centers, self.d_fit_files['spe']['hist'], 'b.')
        ax1.plot(self.a_spe_bin_centers, a_model_spe, 'g-')
        
        #f2, (ax2) = plt.subplots(1)
        #ax2.set_yscale('log', nonposx='clip')
    
        #ax2.plot(self.a_mpe_bin_centers, self.d_fit_files['mpe1']['hist'], 'b.')
        #ax2.plot(self.a_mpe_bin_centers, a_model_mpe1, 'g-')
    
        plt.show()



    def cpu_mc_model_ln_likelihood(self, a_parameters):
        mean_num_pe_spe, prob_hit_first, mean_e_from_dynode, bkg_mean, bkg_std, mean_num_pe_mpe1 = a_parameters

        ln_prior = 0
        ln_likelihood = 0

        ln_prior += self.std_prior(mean_num_pe_spe) # just require > 0
        ln_prior += self.omega_prior(prob_hit_first)
        ln_prior += self.std_prior(mean_e_from_dynode)
        ln_prior += self.std_prior(bkg_std)
        ln_prior += self.std_prior(mean_num_pe_mpe1) # just require > 0

        if not np.isfinite(ln_prior):
            return -np.inf

        #a_model_bkg = np.zeros(self.num_mc_events)
        #mc_model(a_model_bkg, 0, prob_hit_first, mean_e_from_dynode, bkg_mean, bkg_std)
        #a_model_bkg, dummy = np.histogram(a_model_bkg, bins=self.a_bkg_bin_edges)
        #a_model_bkg = np.asarray(a_model_bkg, dtype=np.float64)*np.sum(self.d_fit_files['bkg']['hist'])/np.sum(a_model_bkg)
        
        a_model_spe = np.zeros(self.num_mc_events)
        start_time_spe = time.time()
        mc_model(a_model_spe, mean_num_pe_spe, prob_hit_first, mean_e_from_dynode, bkg_mean, bkg_std)
        print 'SPE MC Time: %f s' % (time.time() - start_time_spe)
        a_model_spe, dummy = np.histogram(a_model_spe, bins=self.a_spe_bin_edges)
        a_model_spe = np.asarray(a_model_spe, dtype=np.float64)*np.sum(self.d_fit_files['spe']['hist'])/np.sum(a_model_spe)
        
        a_model_mpe1 = np.zeros(self.num_mc_events)
        start_time_mpe1 = time.time()
        mc_model(a_model_mpe1, mean_num_pe_mpe1, prob_hit_first, mean_e_from_dynode, bkg_mean, bkg_std)
        print 'MPE1 MC Time: %f s' % (time.time() - start_time_mpe1)
        a_model_mpe1, dummy = np.histogram(a_model_mpe1, bins=self.a_mpe_bin_edges)
        a_model_mpe1 = np.asarray(a_model_mpe1, dtype=np.float64)*np.sum(self.d_fit_files['mpe1']['hist'])/np.sum(a_model_mpe1)
        
        
        #ln_likelihood += poisson_binned_likelihood(a_model_bkg, self.d_fit_files['bkg']['hist'])
        ln_likelihood += poisson_binned_likelihood(a_model_spe, self.d_fit_files['spe']['hist'])
        ln_likelihood += poisson_binned_likelihood(a_model_mpe1, self.d_fit_files['mpe1']['hist'])


        if np.isnan(ln_likelihood):
            return -np.inf
        else:
            return ln_prior + ln_likelihood
            
            
            
    def gpu_mc_model_ln_likelihood(self, a_parameters):
        mean_num_pe_spe, prob_hit_first, mean_e_from_dynode, bkg_mean, bkg_std, mean_num_pe_mpe1 = a_parameters

        ln_prior = 0
        ln_likelihood = 0

        ln_prior += self.std_prior(mean_num_pe_spe) # just require > 0
        ln_prior += self.omega_prior(prob_hit_first)
        ln_prior += self.std_prior(mean_e_from_dynode)
        ln_prior += self.std_prior(bkg_std)
        ln_prior += self.std_prior(mean_num_pe_mpe1) # just require > 0

        if not np.isfinite(ln_prior):
            return -np.inf

        a_hist_spe = np.zeros(self.l_charge_settings_spe[0], dtype=np.float32)
        a_hist_mpe1 = np.zeros(self.l_charge_settings_mpe[0], dtype=np.float32)
        
        mean_num_pe_spe = np.asarray(mean_num_pe_spe, dtype=np.float32)
        mean_num_pe_mpe1 = np.asarray(mean_num_pe_mpe1, dtype=np.float32)
        
        num_trials = np.asarray(self.num_mc_events, dtype=np.int32)
        prob_hit_first = np.asarray(prob_hit_first, dtype=np.float32)
        mean_e_from_dynode = np.asarray(mean_e_from_dynode, dtype=np.float32)
        bkg_mean = np.asarray(bkg_mean, dtype=np.float32)
        bkg_std = np.asarray(bkg_std, dtype=np.float32)
        
        num_bins = np.asarray(self.l_charge_settings_mpe[0], dtype=np.int32)
        spe_bin_edges = np.asarray(self.a_spe_bin_edges, dtype=np.float32)
        mpe_bin_edges = np.asarray(self.a_mpe_bin_edges, dtype=np.float32)
        
        
        l_spe_args_gpu = [self.rng_states, drv.In(num_trials), drv.InOut(a_hist_spe), drv.In(mean_num_pe_spe), drv.In(prob_hit_first), drv.In(mean_e_from_dynode), drv.In(bkg_mean), drv.In(bkg_std), drv.In(num_bins), drv.In(spe_bin_edges)]
        l_mpe1_args_gpu = [self.rng_states, drv.In(num_trials), drv.InOut(a_hist_mpe1), drv.In(mean_num_pe_mpe1), drv.In(prob_hit_first), drv.In(mean_e_from_dynode), drv.In(bkg_mean), drv.In(bkg_std), drv.In(num_bins), drv.In(mpe_bin_edges)]
    
        #start_time_spe = time.time()
        gpu_pmt_mc(*l_spe_args_gpu, **self.d_gpu_scale)
        #print 'Time for SPE call: %f s' % (time.time() - start_time_spe)
        a_model_spe = np.asarray(a_hist_spe, dtype=np.float32)*np.sum(self.d_fit_files['spe']['hist'])/np.sum(a_hist_spe)
        
        #start_time_mpe1 = time.time()
        gpu_pmt_mc(*l_mpe1_args_gpu, **self.d_gpu_scale)
        #print 'Time for MPE1 call: %f s' % (time.time() - start_time_spe)
        a_model_mpe1 = np.asarray(a_hist_mpe1, dtype=np.float32)*np.sum(self.d_fit_files['mpe1']['hist'])/np.sum(a_hist_mpe1)
        
        
        #ln_likelihood += poisson_binned_likelihood(a_model_bkg, self.d_fit_files['bkg']['hist'])
        ln_likelihood += poisson_binned_likelihood(a_model_spe, self.d_fit_files['spe']['hist'])
        ln_likelihood += poisson_binned_likelihood(a_model_mpe1, self.d_fit_files['mpe1']['hist'])

        total_ln_likelihood = ln_prior + ln_likelihood

        if np.isnan(total_ln_likelihood):
            return -np.inf
            
        if self.b_suppress_likelihood:
            total_ln_likelihood /= self.ll_suppression_factor
        
        return total_ln_likelihood
    
    
    
    def draw_mc_model_fit(self, a_parameters):
        mean_num_pe_spe, prob_hit_first, mean_e_from_dynode, bkg_mean, bkg_std, mean_num_pe_mpe1 = a_parameters
        
        
        a_hist_spe = np.zeros(self.l_charge_settings_spe[0], dtype=np.float32)
        a_hist_mpe1 = np.zeros(self.l_charge_settings_mpe[0], dtype=np.float32)
        
        mean_num_pe_spe = np.asarray(mean_num_pe_spe, dtype=np.float32)
        mean_num_pe_mpe1 = np.asarray(mean_num_pe_mpe1, dtype=np.float32)
        
        num_trials = np.asarray(self.num_mc_events, dtype=np.int32)
        prob_hit_first = np.asarray(prob_hit_first, dtype=np.float32)
        mean_e_from_dynode = np.asarray(mean_e_from_dynode, dtype=np.float32)
        bkg_mean = np.asarray(bkg_mean, dtype=np.float32)
        bkg_std = np.asarray(bkg_std, dtype=np.float32)
        
        num_bins = np.asarray(self.l_charge_settings_mpe[0], dtype=np.int32)
        spe_bin_edges = np.asarray(self.a_spe_bin_edges, dtype=np.float32)
        mpe_bin_edges = np.asarray(self.a_mpe_bin_edges, dtype=np.float32)
        
        
        l_spe_args_gpu = [self.rng_states, drv.In(num_trials), drv.InOut(a_hist_spe), drv.In(mean_num_pe_spe), drv.In(prob_hit_first), drv.In(mean_e_from_dynode), drv.In(bkg_mean), drv.In(bkg_std), drv.In(num_bins), drv.In(spe_bin_edges)]
        l_mpe1_args_gpu = [self.rng_states, drv.In(num_trials), drv.InOut(a_hist_mpe1), drv.In(mean_num_pe_mpe1), drv.In(prob_hit_first), drv.In(mean_e_from_dynode), drv.In(bkg_mean), drv.In(bkg_std), drv.In(num_bins), drv.In(mpe_bin_edges)]
    
        #start_time_spe = time.time()
        gpu_pmt_mc(*l_spe_args_gpu, **self.d_gpu_scale)
        #print 'Time for SPE call: %f s' % (time.time() - start_time_spe)
        a_model_spe = np.asarray(a_hist_spe, dtype=np.float32)*np.sum(self.d_fit_files['spe']['hist'])/np.sum(a_hist_spe)
        
        #start_time_mpe1 = time.time()
        gpu_pmt_mc(*l_mpe1_args_gpu, **self.d_gpu_scale)
        #print 'Time for MPE1 call: %f s' % (time.time() - start_time_spe)
        a_model_mpe1 = np.asarray(a_hist_mpe1, dtype=np.float32)*np.sum(self.d_fit_files['mpe1']['hist'])/np.sum(a_hist_mpe1)
    
        f1, (ax1) = plt.subplots(1)
        ax1.set_yscale('log', nonposx='clip')
    
        ax1.plot(self.a_spe_bin_centers, self.d_fit_files['spe']['hist'], 'b.')
        ax1.plot(self.a_spe_bin_centers, a_model_spe, 'g-')
        
        f2, (ax2) = plt.subplots(1)
        #ax2.set_yscale('log', nonposx='clip')
    
        ax2.plot(self.a_mpe_bin_centers, self.d_fit_files['mpe1']['hist'], 'b.')
        ax2.plot(self.a_mpe_bin_centers, a_model_mpe1, 'g-')
    
        plt.show()
    
    
    
    



    def differential_evolution_minimizer(self, a_bounds, maxiter=250, tol=0.05, popsize=15, polish=False):
        def neg_log_likelihood_diff_ev(a_guesses):
            return -self.gpu_mc_model_ln_likelihood(a_guesses)
        print '\n\nStarting differential evolution minimizer...\n\n'
        result = op.differential_evolution(neg_log_likelihood_diff_ev, a_bounds, disp=True, maxiter=maxiter, tol=tol, popsize=popsize, polish=polish)
        print result



    def suppress_likelihood(self, iterations=200):

        a_free_par_guesses = [0.05, 0.95, 3.15, 1e4, 2e5, 1.2]
        
        l_parameters = [a_free_par_guesses for i in xrange(iterations)]
        l_log_likelihoods = [0. for i in xrange(iterations)]
        for i in tqdm.tqdm(xrange(iterations)):
            l_log_likelihoods[i] = self.gpu_mc_model_ln_likelihood(a_free_par_guesses)

        var_ll = np.std(l_log_likelihoods)

        print 'Standard deviation for %.3e MC iterations is %f' % (self.num_mc_events, var_ll)
        print 'Will scale LL such that variance is 0.1'

        self.b_suppress_likelihood = True
        self.ll_suppression_factor = var_ll / 0.1

        print 'LL suppression factor: %f\n' % self.ll_suppression_factor




if __name__ == '__main__':
    d_fit_filenames = {}
    d_fit_filenames['bkg'] = 'nerix_160407_1517'
    d_fit_filenames['spe'] = 'nerix_160407_1525'
    d_fit_filenames['mpe1'] = 'nerix_160407_1533'
    #d_fit_filenames['bkg'] = 'nerix_160414_0947'
    #d_fit_filenames['spe'] = 'nerix_160414_0956'

    test = fit_pmt_gain(d_fit_filenames, num_mc_events=5e5)
    
    #test.bkg_ln_likelihood([9.42947869e-01, 7.56473634e+04, 1.12592575e+05, 4.61294214e-03])
    #print test.full_model_ln_likelihood([0.005, 1.19e4, 2.e5, -14, 0.05, 8e5, 6e5])
    #for i in xrange(10):
    #    print test.gpu_mc_model_ln_likelihood([0.05, 0.95, 3.15, 1e4, 2e5, 1.2])

    #test.draw_bkg_only_fit([0.005, 3.02e3, 1.95e5, -14])
    #test.draw_full_model_fit([0.012, 1.17e4, 2.06e5, -13.7, 0.0442, 7.68e5, 5.83e5])
    #test.draw_mc_model_fit([0.069, 0.89, 3.08, 1.77e4, 2e5, 1.38])

    test.suppress_likelihood()
    a_bounds = [(0.01, 0.1), (0.5, 1), (2.9, 3.4), (1e3, 1e5), (5e4, 5e5), (0.6, 2)]
    test.differential_evolution_minimizer(a_bounds, maxiter=50, tol=0.05, popsize=10, polish=True)

    #test.run_bkg_only_mcmc(num_walkers=128, num_steps=1000, threads=7, burn_in=500)
    #test.run_full_model_mcmc(num_walkers=128, num_steps=1000, threads=7, burn_in=500)



