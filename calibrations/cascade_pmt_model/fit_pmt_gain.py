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

import astroML.density_estimation

import cuda_pmt_mc
from pycuda.compiler import SourceModule
import pycuda.driver as drv
import pycuda.tools
import pycuda.gpuarray
import pycuda.autoinit

gpu_cascade_model = SourceModule(cuda_pmt_mc.cuda_pmt_mc, no_extern_c=True).get_function('cascade_pmt_model')
gpu_pure_cascade_spectrum = SourceModule(cuda_pmt_mc.cuda_pmt_mc, no_extern_c=True).get_function('pure_cascade_spectrum')
gpu_fixed_pe_cascade_spectrum = SourceModule(cuda_pmt_mc.cuda_pmt_mc, no_extern_c=True).get_function('fixed_pe_cascade_spectrum')
setup_kernel = SourceModule(cuda_pmt_mc.cuda_pmt_mc, no_extern_c=True).get_function('setup_kernel')



def weighted_avg_and_std(values, weights):
    """
    Return the weighted average and standard deviation.

    values, weights -- Numpy ndarrays with the same shape.
    """
    average = np.average(values, weights=weights)
    variance = np.average((values-average)**2, weights=weights)  # Fast and numerically precise
    return (average, (variance)**0.5)


def reduce_method(m):
    return (getattr, (m.__self__, m.__func__.__name__))



def poisson_binned_likelihood(a_model, a_data):
    #print a_data*np.log(a_model) - a_model
    return np.sum(a_data*np.log(a_model) - a_model - a_data*np.log(a_data) + a_data)



class fit_pmt_gain(object):
    def __init__(self, d_fit_filenames, run=16, channel_number=17, num_mc_events=1e6, b_making_comparison_plots=False):
    
        # d_fit_files should be of the form
        # d_fit_files['mpe1'] = <bkg file name>
        # d_fit_files['mpe2'] = <spe file name>

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
        
        
        self.b_making_comparison_plots = b_making_comparison_plots
        
        
        seed = int(time.time())
        self.rng_states = drv.mem_alloc(self.num_mc_events*pycuda.characterize.sizeof('curandStateXORWOW', '#include <curand_kernel.h>'))
        setup_kernel(np.int32(self.num_mc_events), self.rng_states, np.uint64(seed), np.uint64(0), **self.d_gpu_scale)
        print 'Cuda random states setup...\n'
        

        self.d_fit_filenames = d_fit_filenames
        self.d_fit_files = {}
        self.d_fit_files['mpe1'] = {}
        self.d_fit_files['mpe2'] = {}
        self.d_fit_files['mpe1']['file'] = File(neriX_config.pathToData + 'run_16/' + self.d_fit_filenames['mpe1'] + '.root')
        self.d_fit_files['mpe2']['file'] = File(neriX_config.pathToData + 'run_16/' + self.d_fit_filenames['mpe2'] + '.root')

        self.d_fit_files['mpe1']['tree'] = self.d_fit_files['mpe1']['file'].T0
        self.d_fit_files['mpe2']['tree'] = self.d_fit_files['mpe2']['file'].T0

        self.d_fit_files['mpe1']['settings'] = [50, -6e5, 3e6]
        self.d_fit_files['mpe2']['settings'] = [50, -6e5, 4e6]
        
        self.d_fit_files['mpe1']['bin_edges'] = np.linspace(self.d_fit_files['mpe1']['settings'][1], self.d_fit_files['mpe1']['settings'][2], self.d_fit_files['mpe1']['settings'][0]+1) # need +1 for bin edges
        self.d_fit_files['mpe1']['bin_width'] = self.d_fit_files['mpe1']['bin_edges'][1] - self.d_fit_files['mpe1']['bin_edges'][0]
        self.d_fit_files['mpe1']['bin_centers'] = np.linspace(self.d_fit_files['mpe1']['settings'][1]+self.d_fit_files['mpe1']['bin_width']/2., self.d_fit_files['mpe1']['settings'][2]-self.d_fit_files['mpe1']['bin_width']/2., self.d_fit_files['mpe1']['settings'][0])
        if self.b_making_comparison_plots:
            num_bins_plots = 5*self.d_fit_files['mpe1']['settings'][0]
            self.d_fit_files['mpe1']['bin_edges_plots'] = np.linspace(self.d_fit_files['mpe1']['settings'][1], self.d_fit_files['mpe1']['settings'][2], num_bins_plots+1) # need +1 for bin edges
            self.d_fit_files['mpe1']['bin_width_plots'] = self.d_fit_files['mpe1']['bin_edges_plots'][1] - self.d_fit_files['mpe1']['bin_edges_plots'][0]
            self.d_fit_files['mpe1']['bin_centers_plots'] = np.linspace(self.d_fit_files['mpe1']['settings'][1]+self.d_fit_files['mpe1']['bin_width_plots']/2., self.d_fit_files['mpe1']['settings'][2]-self.d_fit_files['mpe1']['bin_width_plots']/2., num_bins_plots)
        
        
        self.d_fit_files['mpe2']['bin_edges'] = np.linspace(self.d_fit_files['mpe2']['settings'][1], self.d_fit_files['mpe2']['settings'][2], self.d_fit_files['mpe2']['settings'][0]+1) # need +1 for bin edges
        self.d_fit_files['mpe2']['bin_width'] = self.d_fit_files['mpe2']['bin_edges'][1] - self.d_fit_files['mpe2']['bin_edges'][0]
        self.d_fit_files['mpe2']['bin_centers'] = np.linspace(self.d_fit_files['mpe2']['settings'][1]+self.d_fit_files['mpe2']['bin_width']/2., self.d_fit_files['mpe2']['settings'][2]-self.d_fit_files['mpe2']['bin_width']/2., self.d_fit_files['mpe2']['settings'][0])
        if self.b_making_comparison_plots:
            num_bins_plots = 5*self.d_fit_files['mpe2']['settings'][0]
            self.d_fit_files['mpe2']['bin_edges_plots'] = np.linspace(self.d_fit_files['mpe2']['settings'][1], self.d_fit_files['mpe2']['settings'][2], num_bins_plots+1) # need +1 for bin edges
            self.d_fit_files['mpe2']['bin_width_plots'] = self.d_fit_files['mpe2']['bin_edges_plots'][1] - self.d_fit_files['mpe2']['bin_edges_plots'][0]
            self.d_fit_files['mpe2']['bin_centers_plots'] = np.linspace(self.d_fit_files['mpe2']['settings'][1]+self.d_fit_files['mpe2']['bin_width_plots']/2., self.d_fit_files['mpe2']['settings'][2]-self.d_fit_files['mpe2']['bin_width_plots']/2., num_bins_plots)
        
        
        
        
        self.d_fit_files['mpe1']['array'] = tree2array(self.d_fit_files['mpe1']['tree'], [self.parameter_to_examine])
        self.d_fit_files['mpe1']['array'] = np.array(filter(lambda x: x < self.d_fit_files['mpe1']['settings'][2] and x > self.d_fit_files['mpe1']['settings'][1], self.d_fit_files['mpe1']['array'][self.parameter_to_examine]))
        
        self.d_fit_files['mpe2']['array'] = tree2array(self.d_fit_files['mpe2']['tree'], [self.parameter_to_examine])
        self.d_fit_files['mpe2']['array'] = np.array(filter(lambda x: x < self.d_fit_files['mpe2']['settings'][2] and x > self.d_fit_files['mpe2']['settings'][1], self.d_fit_files['mpe2']['array'][self.parameter_to_examine]))
        
       
        
        #self.d_fit_files['mpe1']['bin_edges'] = astroML.density_estimation.bayesian_blocks(self.d_fit_files['mpe1']['array'])
        self.d_fit_files['mpe1']['hist'], dummy = np.histogram(self.d_fit_files['mpe1']['array'], bins=self.d_fit_files['mpe1']['bin_edges'])
       
        
        #self.d_fit_files['mpe2']['bin_edges'] = astroML.density_estimation.bayesian_blocks(self.d_fit_files['mpe2']['array'])
        self.d_fit_files['mpe2']['hist'], dummy = np.histogram(self.d_fit_files['mpe2']['array'], bins=self.d_fit_files['mpe2']['bin_edges'])
        
        
        
        # set save and load paths
        self.timestamp = self.d_fit_filenames['mpe1'][6:12]
        self.s_base_save_name = 'cascade_model_fit'
        self.dict_filename = 'sampler_dictionary.p'
        self.acceptance_filename = 'acceptance_fraction.p'
        self.s_directory_save_name = 'results/%s/' % (self.timestamp)
        self.s_directory_save_plots_name = 'plots/%s/' % (self.timestamp)
        
        
        self.a_free_par_guesses = [0.8881, 25.106, 0.1251, 4.55e4, 2.129e5, 9.24e5, 0.074, 1.04, 2.104]
    
    
        self.b_suppress_likelihood = False
    
    
        #print self.d_fit_files['spe']['hist']
        
        
        
    def prior_between_0_and_1(self, parameter_to_examine):
        if 0 < parameter_to_examine < 1:
            return 0
        else:
            return -np.inf



    def prior_greater_than_0(self, parameter_to_examine):
        if parameter_to_examine > 0:
            return 0
        else:
            return -np.inf



            
    def cascade_model_ln_likelihood(self, a_parameters):
        prob_hit_first, mean_e_from_dynode, probability_electron_ionized, bkg_mean, bkg_std, bkg_exp, prob_exp_bkg, mean_num_pe_mpe1, mean_num_pe_mpe2 = a_parameters

        ln_prior = 0
        ln_likelihood = 0

        ln_prior += self.prior_between_0_and_1(prob_hit_first)
        ln_prior += self.prior_greater_than_0(mean_e_from_dynode)
        ln_prior += self.prior_greater_than_0(bkg_std)
        ln_prior += self.prior_greater_than_0(bkg_exp)
        ln_prior += self.prior_between_0_and_1(prob_exp_bkg)
        ln_prior += self.prior_greater_than_0(mean_num_pe_mpe1) # just require > 0
        ln_prior += self.prior_greater_than_0(mean_num_pe_mpe2) # just require > 0

        if not np.isfinite(ln_prior):
            return -np.inf

        a_hist_mpe1 = np.zeros(len(self.d_fit_files['mpe1']['hist']), dtype=np.float32)
        a_hist_mpe2 = np.zeros(len(self.d_fit_files['mpe2']['hist']), dtype=np.float32)
        
        mean_num_pe_mpe1 = np.asarray(mean_num_pe_mpe1, dtype=np.float32)
        mean_num_pe_mpe2 = np.asarray(mean_num_pe_mpe2, dtype=np.float32)
        
        num_trials = np.asarray(self.num_mc_events, dtype=np.int32)
        prob_hit_first = np.asarray(prob_hit_first, dtype=np.float32)
        mean_e_from_dynode = np.asarray(mean_e_from_dynode, dtype=np.float32)
        probability_electron_ionized = np.asarray(probability_electron_ionized, dtype=np.float32)
        bkg_mean = np.asarray(bkg_mean, dtype=np.float32)
        bkg_std = np.asarray(bkg_std, dtype=np.float32)
        bkg_exp = np.asarray(bkg_exp, dtype=np.float32)
        prob_exp_bkg = np.asarray(prob_exp_bkg, dtype=np.float32)
        
        num_bins_mpe1 = np.asarray(len(self.d_fit_files['mpe1']['hist']), dtype=np.int32)
        mpe1_bin_edges = np.asarray(self.d_fit_files['mpe1']['bin_edges'], dtype=np.float32)
        num_bins_mpe2 = np.asarray(len(self.d_fit_files['mpe2']['hist']), dtype=np.int32)
        mpe2_bin_edges = np.asarray(self.d_fit_files['mpe2']['bin_edges'], dtype=np.float32)
        
        
        l_mpe1_args_gpu = [self.rng_states, drv.In(num_trials), drv.InOut(a_hist_mpe1), drv.In(mean_num_pe_mpe1), drv.In(prob_hit_first), drv.In(mean_e_from_dynode), drv.In(probability_electron_ionized), drv.In(bkg_mean), drv.In(bkg_std), drv.In(bkg_exp), drv.In(prob_exp_bkg), drv.In(num_bins_mpe1), drv.In(mpe1_bin_edges)]
        l_mpe2_args_gpu = [self.rng_states, drv.In(num_trials), drv.InOut(a_hist_mpe2), drv.In(mean_num_pe_mpe2), drv.In(prob_hit_first), drv.In(mean_e_from_dynode), drv.In(probability_electron_ionized), drv.In(bkg_mean), drv.In(bkg_std), drv.In(bkg_exp), drv.In(prob_exp_bkg), drv.In(num_bins_mpe2), drv.In(mpe2_bin_edges)]
    
    
        #start_time_mpe1 = time.time()
        gpu_cascade_model(*l_mpe1_args_gpu, **self.d_gpu_scale)
        #print 'Time for MPE1 call: %f s' % (time.time() - start_time_spe)
        a_model_mpe1 = np.asarray(a_hist_mpe1, dtype=np.float32)*np.sum(self.d_fit_files['mpe1']['hist'])/np.sum(a_hist_mpe1)
        
        #start_time_mpe2 = time.time()
        gpu_cascade_model(*l_mpe2_args_gpu, **self.d_gpu_scale)
        #print 'Time for mpe2 call: %f s' % (time.time() - start_time_spe)
        a_model_mpe2 = np.asarray(a_hist_mpe2, dtype=np.float32)*np.sum(self.d_fit_files['mpe2']['hist'])/np.sum(a_hist_mpe2)
        
        
        
        ln_likelihood += poisson_binned_likelihood(a_model_mpe1, self.d_fit_files['mpe1']['hist'])
        ln_likelihood += poisson_binned_likelihood(a_model_mpe2, self.d_fit_files['mpe2']['hist'])

        total_ln_likelihood = ln_prior + ln_likelihood

        if np.isnan(total_ln_likelihood):
            return -np.inf
            
        if self.b_suppress_likelihood:
            total_ln_likelihood /= self.ll_suppression_factor
        
        return total_ln_likelihood
    
    
    
    def run_cascade_model_mcmc(self, num_walkers=32, num_steps=2000, threads=1):
        
        l_value_guesses = self.a_free_par_guesses
        l_std_guesses = [0.005, 0.5, 0.01, 1e4, 5e4, 8e4, 0.04, 0.03, 0.06]
        l_par_names = ['p_hit_first_dynode', 'electrons_per_dynode', 'p_e_freed', 'bkg_mean', 'bkg_std', 'bkg_std', 'bkg_exp', 'mean_num_pe_mpe1', 'mean_num_pe_mpe2']

        
        
        
        if not os.path.exists(self.s_directory_save_name):
            os.makedirs(self.s_directory_save_name)
        num_dim = len(l_value_guesses)
        
        
        loaded_prev_sampler = False
        try:
            # two things will fail potentially
            # 1. open if file doesn't exist
            # 2. posWalkers load since initialized to None

            with open(self.s_directory_save_name + self.dict_filename, 'r') as f_prev_sampler:

                d_sampler = pickle.load(f_prev_sampler)
                prevSampler = d_sampler[num_walkers][-1]


                # need to load in weird way bc can't pickle
                # ensembler object
                a_starting_pos = prevSampler['_chain'][:,-1,:]
                random_state = prevSampler['_random']
            loaded_prev_sampler = True
            print '\nSuccessfully loaded previous chain!\n'
        except:
            print '\nCould not load previous sampler or none existed - starting new sampler.\n'

        if not loaded_prev_sampler:

            a_starting_pos = emcee.utils.sample_ball(l_value_guesses, l_std_guesses, size=num_walkers)

            random_state = None

            # create file if it doesn't exist
            if not os.path.exists(self.s_directory_save_name + self.dict_filename):
                with open(self.s_directory_save_name + self.dict_filename, 'w') as f_prev_sampler:
                    d_sampler = {}

                    d_sampler[num_walkers] = []

                    pickle.dump(d_sampler, f_prev_sampler)
            else:
                with open(self.s_directory_save_name + self.dict_filename, 'r') as f_prev_sampler:
                    d_sampler = pickle.load(f_prev_sampler)
                with open(self.s_directory_save_name + self.dict_filename, 'w') as f_prev_sampler:

                    d_sampler[num_walkers] = []

                    pickle.dump(d_sampler, f_prev_sampler)
        
        



        #sampler = emcee.EnsembleSampler(num_walkers, num_dim, self.cascade_model_ln_likelihood, threads=threads)
        sampler = emcee.DESampler(num_walkers, num_dim, self.cascade_model_ln_likelihood, threads=threads, autoscale_gamma=True)
        
        print '\n\nBeginning MCMC sampler\n\n'
        print '\nNumber of walkers * number of steps = %d * %d = %d function calls\n' % (num_walkers, num_steps, num_walkers*num_steps)
        start_time_mcmc = time.time()

        with click.progressbar(sampler.sample(a_starting_pos, iterations=num_steps, ), length=num_steps) as mcmc_sampler:
            for pos, lnprob, state in mcmc_sampler:
                pass

        total_time_mcmc = (time.time() - start_time_mcmc) / 3600.
        print '\n\n%d function calls took %.2f hours.\n\n' % (num_walkers*num_steps, total_time_mcmc)


        dictionary_for_sampler = sampler.__dict__
        if 'lnprobfn' in dictionary_for_sampler:
            del dictionary_for_sampler['lnprobfn']
        if 'pool' in dictionary_for_sampler:
            del dictionary_for_sampler['pool']

        with open(self.s_directory_save_name + self.dict_filename, 'r') as f_prev_sampler:
            d_sampler = pickle.load(f_prev_sampler)
        #f_prev_sampler.close()

        f_prev_sampler = open(self.s_directory_save_name + self.dict_filename, 'w')

        d_sampler[num_walkers].append(sampler.__dict__)

        pickle.dump(d_sampler, f_prev_sampler)
        f_prev_sampler.close()



        #sampler.run_mcmc(posWalkers, numSteps) # shortcut of above method
        pickle.dump(sampler.acceptance_fraction, open(self.s_directory_save_name + self.acceptance_filename, 'w'))
    
    
    
    def draw_mc_model_fit(self, a_parameters):
        prob_hit_first, mean_e_from_dynode, probability_electron_ionized, bkg_mean, bkg_std, bkg_exp, prob_exp_bkg, mean_num_pe_mpe1, mean_num_pe_mpe2 = a_parameters
        
        
        a_hist_mpe1 = np.zeros(len(self.d_fit_files['mpe1']['hist']), dtype=np.float32)
        a_hist_mpe2 = np.zeros(len(self.d_fit_files['mpe2']['hist']), dtype=np.float32)
        
        mean_num_pe_mpe1 = np.asarray(mean_num_pe_mpe1, dtype=np.float32)
        mean_num_pe_mpe2 = np.asarray(mean_num_pe_mpe2, dtype=np.float32)
        
        num_trials = np.asarray(self.num_mc_events, dtype=np.int32)
        prob_hit_first = np.asarray(prob_hit_first, dtype=np.float32)
        mean_e_from_dynode = np.asarray(mean_e_from_dynode, dtype=np.float32)
        probability_electron_ionized = np.asarray(probability_electron_ionized, dtype=np.float32)
        bkg_mean = np.asarray(bkg_mean, dtype=np.float32)
        bkg_std = np.asarray(bkg_std, dtype=np.float32)
        bkg_exp = np.asarray(bkg_exp, dtype=np.float32)
        prob_exp_bkg = np.asarray(prob_exp_bkg, dtype=np.float32)
        
        num_bins_mpe1 = np.asarray(len(self.d_fit_files['mpe1']['hist']), dtype=np.int32)
        mpe1_bin_edges = np.asarray(self.d_fit_files['mpe1']['bin_edges'], dtype=np.float32)
        num_bins_mpe2 = np.asarray(len(self.d_fit_files['mpe2']['hist']), dtype=np.int32)
        mpe2_bin_edges = np.asarray(self.d_fit_files['mpe2']['bin_edges'], dtype=np.float32)
        
        
        l_mpe1_args_gpu = [self.rng_states, drv.In(num_trials), drv.InOut(a_hist_mpe1), drv.In(mean_num_pe_mpe1), drv.In(prob_hit_first), drv.In(mean_e_from_dynode), drv.In(probability_electron_ionized), drv.In(bkg_mean), drv.In(bkg_std), drv.In(bkg_exp), drv.In(prob_exp_bkg), drv.In(num_bins_mpe1), drv.In(mpe1_bin_edges)]
        l_mpe2_args_gpu = [self.rng_states, drv.In(num_trials), drv.InOut(a_hist_mpe2), drv.In(mean_num_pe_mpe2), drv.In(prob_hit_first), drv.In(mean_e_from_dynode), drv.In(probability_electron_ionized), drv.In(bkg_mean), drv.In(bkg_std), drv.In(bkg_exp), drv.In(prob_exp_bkg), drv.In(num_bins_mpe2), drv.In(mpe2_bin_edges)]
    
    
        #start_time_mpe1 = time.time()
        gpu_cascade_model(*l_mpe1_args_gpu, **self.d_gpu_scale)
        #print 'Time for MPE1 call: %f s' % (time.time() - start_time_spe)
        a_model_mpe1 = np.asarray(a_hist_mpe1, dtype=np.float32)*np.sum(self.d_fit_files['mpe1']['hist'])/np.sum(a_hist_mpe1)
        
        #start_time_mpe2 = time.time()
        gpu_cascade_model(*l_mpe2_args_gpu, **self.d_gpu_scale)
        #print 'Time for mpe2 call: %f s' % (time.time() - start_time_spe)
        a_model_mpe2 = np.asarray(a_hist_mpe2, dtype=np.float32)*np.sum(self.d_fit_files['mpe2']['hist'])/np.sum(a_hist_mpe2)
        
    
        f1, (ax1) = plt.subplots(1)
        #ax1.set_yscale('log', nonposx='clip')
    
        a_x_values, a_y_values, a_x_err_low, a_x_err_high, a_y_err_low, a_y_err_high = neriX_analysis.prepare_hist_arrays_for_plotting(self.d_fit_files['mpe1']['hist'], self.d_fit_files['mpe1']['bin_edges'])
        ax1.errorbar(a_x_values, a_y_values, xerr=[a_x_err_low, a_x_err_high], yerr=[a_y_err_low, a_y_err_high], color='b', fmt='.')
        a_x_values, a_y_values, a_x_err_low, a_x_err_high, a_y_err_low, a_y_err_high = neriX_analysis.prepare_hist_arrays_for_plotting(a_model_mpe1, self.d_fit_files['mpe1']['bin_edges'])
        ax1.errorbar(a_x_values, a_y_values, xerr=[a_x_err_low, a_x_err_high], yerr=[a_y_err_low, a_y_err_high], color='r', fmt='.')
        
        
        f2, (ax2) = plt.subplots(1)
        #ax2.set_yscale('log', nonposx='clip')
    
        a_x_values, a_y_values, a_x_err_low, a_x_err_high, a_y_err_low, a_y_err_high = neriX_analysis.prepare_hist_arrays_for_plotting(self.d_fit_files['mpe2']['hist'], self.d_fit_files['mpe2']['bin_edges'])
        ax2.errorbar(a_x_values, a_y_values, xerr=[a_x_err_low, a_x_err_high], yerr=[a_y_err_low, a_y_err_high], color='b', fmt='.')
        a_x_values, a_y_values, a_x_err_low, a_x_err_high, a_y_err_low, a_y_err_high = neriX_analysis.prepare_hist_arrays_for_plotting(a_model_mpe2, self.d_fit_files['mpe2']['bin_edges'])
        ax2.errorbar(a_x_values, a_y_values, xerr=[a_x_err_low, a_x_err_high], yerr=[a_y_err_low, a_y_err_high], color='r', fmt='.')
    
        plt.show()
    
    
    
    def draw_cascade_model_with_error_bands(self, num_walkers, num_steps_to_include=20):
        if not self.b_making_comparison_plots:
            print 'Must instantiate class such that it is ready for making comparison plots\n.'
            sys.exit()
        
        print '\n\nAdded single PE spectrum with mean and variance output in this function too\n\n'
        
        num_dim = len(self.a_free_par_guesses)
        
        sPathToFile = self.s_directory_save_name + self.dict_filename
        
        if os.path.exists(sPathToFile):
            dSampler = pickle.load(open(sPathToFile, 'r'))
            l_chains = []
            for sampler in dSampler[num_walkers]:
                l_chains.append(sampler['_chain'])

            a_sampler = np.concatenate(l_chains, axis=1)

            print 'Successfully loaded sampler!'
        else:
            print sPathToFile
            print 'Could not find file!'
            sys.exit()
        
        a_sampler = a_sampler[:, -num_steps_to_include:, :num_dim].reshape((-1, num_dim))
        
        num_independent_samples = num_walkers*num_steps_to_include
        
        num_bins_plots_mpe1 = len(self.d_fit_files['mpe1']['bin_centers_plots'])
        num_bins_plots_mpe2 = len(self.d_fit_files['mpe2']['bin_centers_plots'])
        a_histograms_mpe1 = np.zeros((num_independent_samples, num_bins_plots_mpe1), dtype=np.float32)
        a_histograms_mpe2 = np.zeros((num_independent_samples, num_bins_plots_mpe2), dtype=np.float32)
        a_histograms_pure = np.zeros((num_independent_samples, num_bins_plots_mpe1), dtype=np.float32)
        
        a_means = np.zeros(num_independent_samples)
        a_stds = np.zeros(num_independent_samples)
        
        
        print 'Generating spectra from draws from the posterior'
        for i in tqdm.tqdm(xrange(num_independent_samples)):
            prob_hit_first = a_sampler[i][0]
            mean_e_from_dynode = a_sampler[i][1]
            probability_electron_ionized = a_sampler[i][2]
            bkg_mean = a_sampler[i][3]
            bkg_std = a_sampler[i][4]
            bkg_exp = a_sampler[i][5]
            prob_exp_bkg = a_sampler[i][6]
            mean_num_pe_mpe1 = a_sampler[i][7]
            mean_num_pe_mpe2 = a_sampler[i][8]
        
        
            a_hist_mpe1 = np.zeros(num_bins_plots_mpe1, dtype=np.float32)
            a_hist_mpe2 = np.zeros(num_bins_plots_mpe2, dtype=np.float32)
            
            a_hist_pure = np.zeros(num_bins_plots_mpe2, dtype=np.float32)
            
            mean_num_pe_mpe1 = np.asarray(mean_num_pe_mpe1, dtype=np.float32)
            mean_num_pe_mpe2 = np.asarray(mean_num_pe_mpe2, dtype=np.float32)
            
            num_trials = np.asarray(self.num_mc_events, dtype=np.int32)
            prob_hit_first = np.asarray(prob_hit_first, dtype=np.float32)
            mean_e_from_dynode = np.asarray(mean_e_from_dynode, dtype=np.float32)
            probability_electron_ionized = np.asarray(probability_electron_ionized, dtype=np.float32)
            bkg_mean = np.asarray(bkg_mean, dtype=np.float32)
            bkg_std = np.asarray(bkg_std, dtype=np.float32)
            bkg_exp = np.asarray(bkg_exp, dtype=np.float32)
            prob_exp_bkg = np.asarray(prob_exp_bkg, dtype=np.float32)
            
            num_bins_mpe1 = np.asarray(num_bins_plots_mpe1, dtype=np.int32)
            mpe1_bin_edges = np.asarray(self.d_fit_files['mpe1']['bin_edges_plots'], dtype=np.float32)
            num_bins_mpe2 = np.asarray(num_bins_plots_mpe2, dtype=np.int32)
            mpe2_bin_edges = np.asarray(self.d_fit_files['mpe2']['bin_edges_plots'], dtype=np.float32)
            
            
            l_mpe1_args_gpu = [self.rng_states, drv.In(num_trials), drv.InOut(a_hist_mpe1), drv.In(mean_num_pe_mpe1), drv.In(prob_hit_first), drv.In(mean_e_from_dynode), drv.In(probability_electron_ionized), drv.In(bkg_mean), drv.In(bkg_std), drv.In(bkg_exp), drv.In(prob_exp_bkg), drv.In(num_bins_mpe1), drv.In(mpe1_bin_edges)]
            l_mpe2_args_gpu = [self.rng_states, drv.In(num_trials), drv.InOut(a_hist_mpe2), drv.In(mean_num_pe_mpe2), drv.In(prob_hit_first), drv.In(mean_e_from_dynode), drv.In(probability_electron_ionized), drv.In(bkg_mean), drv.In(bkg_std), drv.In(bkg_exp), drv.In(prob_exp_bkg), drv.In(num_bins_mpe2), drv.In(mpe2_bin_edges)]
        
        
            #start_time_mpe1 = time.time()
            gpu_cascade_model(*l_mpe1_args_gpu, **self.d_gpu_scale)
            #print 'Time for MPE1 call: %f s' % (time.time() - start_time_spe)
            a_model_mpe1 = np.asarray(a_hist_mpe1, dtype=np.float32)*np.sum(self.d_fit_files['mpe1']['hist'])/np.sum(a_hist_mpe1)*self.d_fit_files['mpe1']['bin_width']/self.d_fit_files['mpe1']['bin_width_plots']
            
            #start_time_mpe2 = time.time()
            gpu_cascade_model(*l_mpe2_args_gpu, **self.d_gpu_scale)
            #print 'Time for mpe2 call: %f s' % (time.time() - start_time_spe)
            a_model_mpe2 = np.asarray(a_hist_mpe2, dtype=np.float32)*np.sum(self.d_fit_files['mpe2']['hist'])/np.sum(a_hist_mpe2)*self.d_fit_files['mpe2']['bin_width']/self.d_fit_files['mpe2']['bin_width_plots']
            
            a_histograms_mpe1[i] = a_model_mpe1
            a_histograms_mpe2[i] = a_model_mpe2
        
        
        
            # gather inputs for pure spec
            l_pure_spec = [self.rng_states, drv.In(num_trials), drv.InOut(a_hist_pure), drv.In(np.asarray(1, dtype=np.int32)), drv.In(mean_e_from_dynode), drv.In(probability_electron_ionized), drv.In(num_bins_mpe1), drv.In(mpe1_bin_edges)]
            
            gpu_pure_cascade_spectrum(*l_pure_spec, **self.d_gpu_scale)
            
            sum_pure_spec = np.sum(a_hist_pure)
            
            if sum_pure_spec != 0:
                a_means[i], a_stds[i] = weighted_avg_and_std(self.d_fit_files['mpe1']['bin_centers_plots'], a_hist_pure)
                a_pure_single_spec = np.asarray(a_hist_pure, dtype=np.float32)/np.sum(a_hist_pure)
                a_histograms_pure[i] = a_pure_single_spec
            else:
                print 'Bad pure spectrum giving, removing sample'
                a_means[i], a_stds[i] = -1, -1
                # remove the -1's later
        
        
        
        # remove all bad pure spectras
        a_means = filter(lambda a: a!=-1, a_means)
        a_stds = filter(lambda a: a!=-1, a_stds)
        
        
        a_one_sigma_below_mpe1 = np.zeros(num_bins_plots_mpe1, dtype=np.float32)
        a_one_sigma_above_mpe1 = np.zeros(num_bins_plots_mpe1, dtype=np.float32)
        for bin in xrange(num_bins_mpe1):
            a_one_sigma_below_mpe1[bin], a_one_sigma_above_mpe1[bin] = np.percentile(a_histograms_mpe1[:, bin], [16, 84])
        
        a_one_sigma_below_mpe2 = np.zeros(num_bins_plots_mpe2, dtype=np.float32)
        a_one_sigma_above_mpe2 = np.zeros(num_bins_plots_mpe2, dtype=np.float32)
        for bin in xrange(num_bins_mpe2):
            a_one_sigma_below_mpe2[bin], a_one_sigma_above_mpe2[bin] = np.percentile(a_histograms_mpe2[:, bin], [16, 84])
            
        a_one_sigma_below_pure = np.zeros(num_bins_plots_mpe1, dtype=np.float32)
        a_one_sigma_above_pure = np.zeros(num_bins_plots_mpe1, dtype=np.float32)
        for bin in xrange(num_bins_mpe1):
            a_one_sigma_below_pure[bin], a_one_sigma_above_pure[bin] = np.percentile(a_histograms_pure[:, bin], [16, 84])
            
            
            
        # get the median from a_sampler for each parameter
        
        
    
        f1, (ax1) = plt.subplots(1)
        #ax1.set_yscale('log', nonposx='clip')
    
        a_x_values, a_y_values, a_x_err_low, a_x_err_high, a_y_err_low, a_y_err_high = neriX_analysis.prepare_hist_arrays_for_plotting(self.d_fit_files['mpe1']['hist'], self.d_fit_files['mpe1']['bin_edges'])
        ax1.errorbar(a_x_values, a_y_values, xerr=[a_x_err_low, a_x_err_high], yerr=[a_y_err_low, a_y_err_high], color='b', fmt='.')
        ax1.fill_between(self.d_fit_files['mpe1']['bin_centers_plots'], a_one_sigma_below_mpe1, a_one_sigma_above_mpe1, facecolor='red', alpha=0.1, interpolate=True)
        ax1.set_title('Integrated Charge Spectrum - %s' % (self.d_fit_filenames['mpe1']))
        ax1.set_xlabel(r'Integrated Charge [$e^{-}$]')
        ax1.set_ylabel('Counts')
        
        
        
        f2, (ax2) = plt.subplots(1)
        #ax2.set_yscale('log', nonposx='clip')
    
        a_x_values, a_y_values, a_x_err_low, a_x_err_high, a_y_err_low, a_y_err_high = neriX_analysis.prepare_hist_arrays_for_plotting(self.d_fit_files['mpe2']['hist'], self.d_fit_files['mpe2']['bin_edges'])
        ax2.errorbar(a_x_values, a_y_values, xerr=[a_x_err_low, a_x_err_high], yerr=[a_y_err_low, a_y_err_high], color='b', fmt='.')
        ax2.fill_between(self.d_fit_files['mpe2']['bin_centers_plots'], a_one_sigma_below_mpe2, a_one_sigma_above_mpe2, facecolor='red', alpha=0.1, interpolate=True)
        ax2.set_title('Integrated Charge Spectrum - %s' % (self.d_fit_filenames['mpe2']))
        ax2.set_xlabel(r'Integrated Charge [$e^{-}$]')
        ax2.set_ylabel('Counts')
        
        
        s_mean_gain = 'Mean = %.2e +/- %.2e' % (np.average(a_means), np.std(a_means))
        s_rms_gain = 'RMS = %.2e +/- %.2e' % (np.average(a_stds), np.std(a_stds))
        
        f3, (ax3) = plt.subplots(1)
        ax3.fill_between(self.d_fit_files['mpe1']['bin_centers_plots'], a_one_sigma_below_pure, a_one_sigma_above_pure, facecolor='blue', alpha=0.3, interpolate=True)
        ax3.set_title('Single PE Spectrum - Best Fit')
        ax3.set_xlabel(r'Integrated Charge [$e^{-}$]')
        ax3.set_ylabel('Normalized Counts')


        ax1.text(0.7, 0.9, '%s\n%s' % (s_mean_gain, s_rms_gain), ha='center', va='center', transform=ax1.transAxes)
        ax2.text(0.7, 0.9, '%s\n%s' % (s_mean_gain, s_rms_gain), ha='center', va='center', transform=ax2.transAxes)
        ax3.text(0.7, 0.8, '%s\n%s' % (s_mean_gain, s_rms_gain), ha='center', va='center', transform=ax3.transAxes)
        
        if not os.path.exists(self.s_directory_save_plots_name):
            os.makedirs(self.s_directory_save_plots_name)

        f1.savefig(self.s_directory_save_plots_name + self.s_base_save_name + '_mpe1.png')
        f2.savefig(self.s_directory_save_plots_name + self.s_base_save_name + '_mpe2.png')
        f3.savefig(self.s_directory_save_plots_name + self.s_base_save_name + '_pure.png')

    
        #plt.show()



    def draw_cascade_model_corner_plot(self, num_walkers, num_steps_to_include):
        
        l_labels_for_corner_plot = ['p_hit_first_dynode', 'electrons_per_dynode', 'p_e_freed', 'bkg_mean', 'bkg_std', 'bkg_exp', 'p_exp_bkg', 'mean_num_pe_mpe1', 'mean_num_pe_mpe2']
        num_dim = len(l_labels_for_corner_plot)
        
        sPathToFile = self.s_directory_save_name + self.dict_filename
        
        if os.path.exists(sPathToFile):
            dSampler = pickle.load(open(sPathToFile, 'r'))
            l_chains = []
            for sampler in dSampler[num_walkers]:
                l_chains.append(sampler['_chain'])

            a_sampler = np.concatenate(l_chains, axis=1)

            print 'Successfully loaded sampler!'
        else:
            print sPathToFile
            print 'Could not find file!'
            sys.exit()
        
        a_sampler = a_sampler[:, -num_steps_to_include:, :num_dim].reshape((-1, num_dim))
        
        print 'Starting corner plot...\n'
        start_time = time.time()
        fig = corner.corner(a_sampler, labels=l_labels_for_corner_plot, quantiles=[0.16, 0.5, 0.84], show_titles=True, title_fmt='.3e', title_kwargs={"fontsize": 12})
        print 'Corner plot took %.3f minutes.\n\n' % ((time.time()-start_time)/60.)
        
        if not os.path.exists(self.s_directory_save_plots_name):
            os.makedirs(self.s_directory_save_plots_name)

        fig.savefig(self.s_directory_save_plots_name + self.s_base_save_name + '_corner.png')
        
        try:
            print emcee.autocorr.integrated_time(np.mean(a_sampler, axis=0), axis=0,
                                        low=10, high=None, step=1, c=2,
                                        fast=False)
        except:
            print 'Chain too short to find autocorrelation time!'




    def draw_fit_with_peaks(self, num_walkers, num_steps_to_include):
        
        num_dim =len(self.a_free_par_guesses)
        
        sPathToFile = self.s_directory_save_name + self.dict_filename
        
        if os.path.exists(sPathToFile):
            dSampler = pickle.load(open(sPathToFile, 'r'))
            l_chains = []
            for sampler in dSampler[num_walkers]:
                l_chains.append(sampler['_chain'])

            a_sampler = np.concatenate(l_chains, axis=1)

            print 'Successfully loaded sampler!'
        else:
            print sPathToFile
            print 'Could not find file!'
            sys.exit()
        
        a_sampler = a_sampler[:, -num_steps_to_include:, :num_dim].reshape((-1, num_dim))
        a_medians = np.median(a_sampler, axis=0)


        l_num_pe = [0, 1, 2, 3, 4, 5, 6]
        l_colors = ['r', 'b', 'g', 'c', 'y', 'saddlebrown', 'orchid']
        prob_hit_first, mean_e_from_dynode, probability_electron_ionized, bkg_mean, bkg_std, bkg_exp, prob_exp_bkg, mean_num_pe_mpe1, mean_num_pe_mpe2 = a_medians

        d_hists = {}
        d_hists['mpe1'] = [np.zeros(len(self.d_fit_files['mpe1']['bin_centers_plots']), dtype=np.float32) for i in xrange(len(l_num_pe))]
        d_hists['mpe2'] = [np.zeros(len(self.d_fit_files['mpe2']['bin_centers_plots']), dtype=np.float32) for i in xrange(len(l_num_pe))]

        d_sum_hists = {}
        d_sum_hists['mpe1'] = np.zeros(len(self.d_fit_files['mpe1']['bin_centers_plots']), dtype=np.float32)
        d_sum_hists['mpe2'] = np.zeros(len(self.d_fit_files['mpe2']['bin_centers_plots']), dtype=np.float32)
        #sum_hist = np.zeros(len(self.d_fit_files['bin_centers_plots']), dtype=np.float32)
        
        mean_num_pe_mpe1 = np.asarray(mean_num_pe_mpe1, dtype=np.float32)
        mean_num_pe_mpe2 = np.asarray(mean_num_pe_mpe2, dtype=np.float32)
        
        num_trials = np.asarray(self.num_mc_events, dtype=np.int32)



        prob_hit_first = np.asarray(prob_hit_first, dtype=np.float32)
        mean_e_from_dynode = np.asarray(mean_e_from_dynode, dtype=np.float32)
        probability_electron_ionized = np.asarray(probability_electron_ionized, dtype=np.float32)
        bkg_mean = np.asarray(bkg_mean, dtype=np.float32)
        bkg_std = np.asarray(bkg_std, dtype=np.float32)
        bkg_exp = np.asarray(bkg_exp, dtype=np.float32)
        prob_exp_bkg = np.asarray(prob_exp_bkg, dtype=np.float32)
        
        bin_edges_mpe1 = np.asarray(self.d_fit_files['mpe1']['bin_edges_plots'], dtype=np.float32)
        bin_edges_mpe2 = np.asarray(self.d_fit_files['mpe2']['bin_edges_plots'], dtype=np.float32)
        
        num_bins_mpe1 = np.asarray(len(bin_edges_mpe1)-1, dtype=np.int32)
        num_bins_mpe2 = np.asarray(len(bin_edges_mpe2)-1, dtype=np.int32)
        
        sum_of_hists_mpe1 = 0
        sum_of_hists_mpe2 = 0

        for i, num_pe in enumerate(l_num_pe):
            current_hist_mpe1 = d_hists['mpe1'][i]
            num_trials_mpe1 = np.asarray(int(self.num_mc_events*scipy.stats.poisson.pmf(num_pe, mean_num_pe_mpe1)), dtype=np.int32)
            
            current_hist_mpe2 = d_hists['mpe2'][i]
            num_trials_mpe2 = np.asarray(int(self.num_mc_events*scipy.stats.poisson.pmf(num_pe, mean_num_pe_mpe2)), dtype=np.int32)
            
            num_pe = np.asarray(num_pe, dtype=np.int32)
        
        
            l_args_gpu_mpe1 = [self.rng_states, drv.In(num_trials_mpe1), drv.InOut(current_hist_mpe1), drv.In(num_pe), drv.In(prob_hit_first), drv.In(mean_e_from_dynode), drv.In(probability_electron_ionized), drv.In(bkg_mean), drv.In(bkg_std), drv.In(bkg_exp), drv.In(prob_exp_bkg), drv.In(num_bins_mpe1), drv.In(bin_edges_mpe1)]
            l_args_gpu_mpe2 = [self.rng_states, drv.In(num_trials_mpe2), drv.InOut(current_hist_mpe2), drv.In(num_pe), drv.In(prob_hit_first), drv.In(mean_e_from_dynode), drv.In(probability_electron_ionized), drv.In(bkg_mean), drv.In(bkg_std), drv.In(bkg_exp), drv.In(prob_exp_bkg), drv.In(num_bins_mpe2), drv.In(bin_edges_mpe2)]
            
            gpu_fixed_pe_cascade_spectrum(*l_args_gpu_mpe1, **self.d_gpu_scale)
            sum_of_hists_mpe1 += np.sum(current_hist_mpe1)
            
            gpu_fixed_pe_cascade_spectrum(*l_args_gpu_mpe2, **self.d_gpu_scale)
            sum_of_hists_mpe2 += np.sum(current_hist_mpe2)
            
            d_hists['mpe1'][i] = current_hist_mpe1
            d_hists['mpe2'][i] = current_hist_mpe2





        for i, num_pe in enumerate(l_num_pe):
            current_hist_mpe1 = d_hists['mpe1'][i]
            current_hist_mpe2 = d_hists['mpe2'][i]
            
            current_hist_mpe1 = np.asarray(current_hist_mpe1, dtype=np.float32)*np.sum(self.d_fit_files['mpe1']['hist'])/sum_of_hists_mpe1*self.d_fit_files['mpe1']['bin_width']/self.d_fit_files['mpe1']['bin_width_plots']
            current_hist_mpe2 = np.asarray(current_hist_mpe2, dtype=np.float32)*np.sum(self.d_fit_files['mpe2']['hist'])/sum_of_hists_mpe2*self.d_fit_files['mpe2']['bin_width']/self.d_fit_files['mpe2']['bin_width_plots']
            
            d_sum_hists['mpe1'] += current_hist_mpe1
            d_sum_hists['mpe2'] += current_hist_mpe2
            
            d_hists['mpe1'][i] = current_hist_mpe1
            d_hists['mpe2'][i] = current_hist_mpe2
        




        f1, (ax1) = plt.subplots(1)
        ax1.set_yscale('log', nonposx='clip')
        
        f2, (ax2) = plt.subplots(1)
        ax2.set_yscale('log', nonposx='clip')
    
        a_x_values, a_y_values, a_x_err_low, a_x_err_high, a_y_err_low, a_y_err_high = neriX_analysis.prepare_hist_arrays_for_plotting(self.d_fit_files['mpe1']['hist'], self.d_fit_files['mpe1']['bin_edges'])
        ax1.errorbar(a_x_values, a_y_values, xerr=[a_x_err_low, a_x_err_high], yerr=[a_y_err_low, a_y_err_high], color='k', fmt='.')
        ax1.plot(self.d_fit_files['mpe1']['bin_centers_plots'], d_sum_hists['mpe1'], color='darkorange', linestyle='-')
        
        a_x_values, a_y_values, a_x_err_low, a_x_err_high, a_y_err_low, a_y_err_high = neriX_analysis.prepare_hist_arrays_for_plotting(self.d_fit_files['mpe2']['hist'], self.d_fit_files['mpe2']['bin_edges'])
        ax2.errorbar(a_x_values, a_y_values, xerr=[a_x_err_low, a_x_err_high], yerr=[a_y_err_low, a_y_err_high], color='k', fmt='.')
        ax2.plot(self.d_fit_files['mpe2']['bin_centers_plots'], d_sum_hists['mpe2'], color='darkorange', linestyle='-')
        

        for i in xrange(len(l_num_pe)):
            ax1.plot(self.d_fit_files['mpe1']['bin_centers_plots'], d_hists['mpe1'][i], color=l_colors[i])
            ax2.plot(self.d_fit_files['mpe2']['bin_centers_plots'], d_hists['mpe2'][i], color=l_colors[i])
        
        
        ax1.set_title('Integrated Charge Spectrum - %s' % (self.d_fit_filenames['mpe1']))
        ax1.set_xlabel(r'Integrated Charge [$e^{-}$]')
        ax1.set_ylabel('Counts')

        ax2.set_title('Integrated Charge Spectrum - %s' % (self.d_fit_filenames['mpe2']))
        ax2.set_xlabel(r'Integrated Charge [$e^{-}$]')
        ax2.set_ylabel('Counts')
        
        f1.savefig('%s%s_pe_specs_%s.png' % (self.s_directory_save_plots_name, self.s_base_save_name, self.d_fit_filenames['mpe1']))
        f2.savefig('%s%s_pe_specs_%s.png' % (self.s_directory_save_plots_name, self.s_base_save_name, self.d_fit_filenames['mpe2']))


    



    def differential_evolution_minimizer(self, a_bounds, maxiter=250, tol=0.05, popsize=15, polish=False):
        def neg_log_likelihood_diff_ev(a_guesses):
            return -self.cascade_model_ln_likelihood(a_guesses)
        print '\n\nStarting differential evolution minimizer...\n\n'
        result = op.differential_evolution(neg_log_likelihood_diff_ev, a_bounds, disp=True, maxiter=maxiter, tol=tol, popsize=popsize, polish=polish)
        print result



    def suppress_likelihood(self, iterations=200):

        #a_free_par_guesses = [0.9807, 3.1225, 4.5875e4, 2.1723e5, 1.104, 2.1]
        a_free_par_guesses = self.a_free_par_guesses
        
        l_parameters = [a_free_par_guesses for i in xrange(iterations)]
        l_log_likelihoods = [0. for i in xrange(iterations)]
        for i in tqdm.tqdm(xrange(iterations)):
            l_log_likelihoods[i] = self.cascade_model_ln_likelihood(a_free_par_guesses)

        #print l_log_likelihoods
        var_ll = np.std(l_log_likelihoods)

        print 'Standard deviation for %.3e MC iterations is %f' % (self.num_mc_events, var_ll)
        print 'Will scale LL such that variance is 0.1'

        self.b_suppress_likelihood = True
        self.ll_suppression_factor = var_ll / 0.1

        print 'LL suppression factor: %f\n' % self.ll_suppression_factor




if __name__ == '__main__':
    d_fit_filenames = {}
    d_fit_filenames['mpe1'] = 'nerix_160407_1533'
    d_fit_filenames['mpe2'] = 'nerix_160407_1550'
    #d_fit_filenames['bkg'] = 'nerix_160414_0947'
    #d_fit_filenames['spe'] = 'nerix_160414_0956'

    test = fit_pmt_gain(d_fit_filenames, num_mc_events=1e5, b_making_comparison_plots=True)
    
    #test.bkg_ln_likelihood([9.42947869e-01, 7.56473634e+04, 1.12592575e+05, 4.61294214e-03])
    #print test.full_model_ln_likelihood([0.005, 1.19e4, 2.e5, -14, 0.05, 8e5, 6e5])
    #for i in xrange(10):
    #    print test.gpu_mc_model_ln_likelihood([0.05, 0.95, 3.15, 1e4, 2e5, 1.2])


    test.draw_cascade_model_with_error_bands(num_walkers=128, num_steps_to_include=20)
    test.draw_cascade_model_corner_plot(num_walkers=128, num_steps_to_include=20)
    test.draw_fit_with_peaks(num_walkers=128, num_steps_to_include=150)

    #test.suppress_likelihood()
    
    #a_bounds = [(0.75, 1), (20, 40), (0, 0.3), (1e3, 1e5), (5e4, 5e5), (1e4, 1e6), (0, 1), (0.6, 2), (1.5, 2.5)]
    #test.differential_evolution_minimizer(a_bounds, maxiter=50, tol=0.01, popsize=10, polish=False)

    #test.run_cascade_model_mcmc(num_walkers=128, num_steps=50, threads=1)



