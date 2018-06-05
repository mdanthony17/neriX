#!/usr/bin/python
import pickle
#print pickle.Pickler.dispatch
import dill
#print pickle.Pickler.dispatch

import ROOT as root
import sys, os

import matplotlib
matplotlib.use('QT4Agg')

matplotlib.rcParams['mathtext.fontset'] = 'stix'
matplotlib.rcParams['font.family'] = 'STIXGeneral'
matplotlib.rcParams['font.size'] = 16

import matplotlib.pyplot as plt

from rootpy.plotting import root2matplotlib as rplt

import emcee, corner, click
import neriX_analysis, neriX_datasets, neriX_config
from rootpy.plotting import Hist2D, Hist, Legend, Canvas
from rootpy.tree import Tree
from rootpy.io import File
import numpy as np
import tqdm, time, copy_reg, types, pickle
from root_numpy import tree2array, array2tree
import scipy.optimize as op
import scipy.special
import scipy.misc
import scipy.stats

import pandas as pd

#print pickle.Pickler.dispatch

"""
def _pickle_method(method):
    print method.__dict__
    func_name = method.im_func.__name__
    obj = method.im_self
    cls = method.im_class
    return _unpickle_method, (func_name, obj, cls)

def _unpickle_method(func_name, obj, cls):
    for cls in cls.mro():
        try:
            func = cls.__dict__[func_name]
        except KeyError:
            pass
        else:
            break
    return func.__get__(obj, cls)

copy_reg.pickle(types.FunctionType, _pickle_method, _unpickle_method)
"""

def reduce_method(m):
    return (getattr, (m.__self__, m.__func__.__name__))

#print pickle.Pickler.dispatch

def save_function(self, obj):
    # Save functions by value if they are defined interactively
    if obj.__module__ == '__main__' or obj.func_name == '<lambda>':
        args = (obj.func_code, obj.func_globals, obj.func_name, obj.func_defaults, obj.func_closure)
        self.save_reduce(types.FunctionType, args, obj=obj)
    else:
        pickle.Pickler.save_global(self, obj)
pickle.Pickler.dispatch[types.FunctionType] = save_function



#print pickle.Pickler.dispatch

class fit_mpe:
    def __init__(self, d_files, pmt_channel=17):

        self.d_files = d_files
        self.l_voltage_settings = [800, 750, 700, 650, 600, 550, 500]
        self.l_percentiles = [5, 95]
        
        self.channel_number = pmt_channel
        self.parameter_to_examine = 'SingleIntegral[%d]' % (self.channel_number - 1)
        
        self.spe_gain_value = 9.35e5
        self.spe_gain_uncertainty = 6.83e5
        
        self.d_fit_files = {}
        for voltage_setting in self.l_voltage_settings:
            self.d_fit_files[voltage_setting] = {}
            self.d_fit_files[voltage_setting]['name'] = self.d_files[voltage_setting]
            self.d_fit_files[voltage_setting]['file'] = File(neriX_config.pathToData + 'run_16/' + self.d_fit_files[voltage_setting]['name'] + '.root')
            self.d_fit_files[voltage_setting]['tree'] = self.d_fit_files[voltage_setting]['file'].T0
        
            self.d_fit_files[voltage_setting]['a_sig'] = tree2array(self.d_fit_files[voltage_setting]['tree'], branches=self.parameter_to_examine)
            
            # put arrays into dataframe
            self.d_fit_files[voltage_setting]['df'] = pd.DataFrame({'sig':self.d_fit_files[voltage_setting]['a_sig']})
            
            # make cut at 20th and 80th percentile
            self.d_fit_files[voltage_setting]['l_percentiles'] = np.percentile(self.d_fit_files[voltage_setting]['a_sig'], self.l_percentiles)
            self.d_fit_files[voltage_setting]['df'] = self.d_fit_files[voltage_setting]['df'][(self.d_fit_files[voltage_setting]['df']['sig'] > self.d_fit_files[voltage_setting]['l_percentiles'][0]) & (self.d_fit_files[voltage_setting]['df']['sig'] < self.d_fit_files[voltage_setting]['l_percentiles'][1])]
            
            self.d_fit_files[voltage_setting]['sig_mean_value'] = np.mean(self.d_fit_files[voltage_setting]['df']['sig'])
            self.d_fit_files[voltage_setting]['sig_mean_uncertainty'] = np.var(self.d_fit_files[voltage_setting]['df']['sig'])**0.5/float(len(self.d_fit_files[voltage_setting]['df']['sig']))**0.5
            
        
            #print self.d_fit_files[voltage_setting]['df'].head()
            #print len(self.d_fit_files[voltage_setting]['df'])
            #print self.d_fit_files[voltage_setting]['sig_mean_value'], self.d_fit_files[voltage_setting]['sig_mean_uncertainty']
            #print self.d_fit_files[voltage_setting]['a_sig']
        

        self.timestamp = self.d_fit_files[800]['name'][6:12]
        self.s_base_save_name = 'mpe_fit'
        self.dict_filename = 'sampler_dictionary.p'
        self.s_directory_save_name = 'results/%s/' % (self.timestamp)
        self.a_free_par_guesses = [self.d_fit_files[800]['sig_mean_value'], 10.24] + [1.98e-1, 4.15e-4, 7.25e-7]
        self.a_free_par_std_guesses = [self.d_fit_files[800]['sig_mean_uncertainty'], 0.05] + [1.98e-2, 4.15e-5, 7.25e-8]
        self.a_free_par_names = ['mpe_gain_800V', 'gain_power'] + ['pol_par0', 'pol_par1', 'pol_par2']
        

        
        



    def prior_between_0_and_1(self, parameter_to_examine):
        if 0 < parameter_to_examine < 1:
            return 0
        else:
            return -np.inf



    def prior_between_minus_1_and_1(self, parameter_to_examine):
        if -1 < parameter_to_examine < 1:
            return 0
        else:
            return -np.inf



    def prior_greater_than_0(self, parameter_to_examine):
        if parameter_to_examine > 0:
            return 0
        else:
            return -np.inf



    def prior_mpe_gain_800(self, gain):
        return scipy.stats.norm.logpdf(gain, self.d_fit_files[800]['sig_mean_value'], self.d_fit_files[800]['sig_mean_uncertainty'])



    def prior_mpe_power(self, power):
        if 8 < power < 13:
            return 0
        else:
            return -np.inf


    def mpe_ln_likelihood(self, a_parameters):
        mpe_gain_800, mpe_power, pol_par0, pol_par1, pol_par2 = a_parameters
        #print a_parameters

        ln_prior = 0
        ln_likelihood = 0

        ln_prior += self.prior_mpe_power(mpe_power)
        ln_prior += self.prior_mpe_gain_800(mpe_gain_800)
        ln_prior += self.prior_between_minus_1_and_1(pol_par0)
        ln_prior += self.prior_between_minus_1_and_1(pol_par1)
        ln_prior += self.prior_between_minus_1_and_1(pol_par2)
        
        
        
        if not np.isfinite(ln_prior):
            return -np.inf

        for voltage_setting in self.l_voltage_settings:
            efficiency = max(min(pol_par0 + pol_par1*float(voltage_setting) + pol_par2*float(voltage_setting)**2, 1), 0)
            expected_gain = mpe_gain_800*np.power(float(voltage_setting)/800., mpe_power)
            #print voltage_setting, float(voltage_setting)/800.
            #print expected_gain, efficiency, self.d_fit_files[voltage_setting]['sig_mean_value']
            
            #print voltage_setting, pol_par0 + pol_par1*float(voltage_setting) + pol_par2*float(voltage_setting)**2, expected_gain, self.d_fit_files[voltage_setting]['sig_mean_value']
            #print max(min(pol_par0 + pol_par1*float(voltage_setting) + pol_par2*float(voltage_setting)**2, 1.), 0.)
            expected_gain *= efficiency
            ln_likelihood += scipy.stats.norm.logpdf(expected_gain, self.d_fit_files[voltage_setting]['sig_mean_value'], self.d_fit_files[voltage_setting]['sig_mean_uncertainty'])


        return ln_likelihood + ln_prior
        
        
        
        
    def run_mpe_mcmc(self, num_walkers=16, num_steps=1000, threads=1):
        
        l_value_guesses = self.a_free_par_guesses
        l_std_guesses = self.a_free_par_std_guesses
        l_par_names = self.a_free_par_names

        
        
        
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
        
        



        #sampler = emcee.EnsembleSampler(num_walkers, num_dim, self.gas_gain_ln_likelihood, threads=threads)
        sampler = emcee.DESampler(num_walkers, num_dim, self.mpe_ln_likelihood, threads=threads, autoscale_gamma=True)
        
        print '\n\nBeginning MCMC sampler\n\n'
        print '\nNumber of walkers * number of steps = %d * %d = %d function calls\n' % (num_walkers, num_steps, num_walkers*num_steps)
        start_time_mcmc = time.time()

        with click.progressbar(sampler.sample(a_starting_pos, iterations=num_steps, ), length=num_steps) as mcmc_sampler:
            for pos, lnprob, state in mcmc_sampler:
                pass

        total_time_mcmc = (time.time() - start_time_mcmc) / 60.
        print '\n\n%d function calls took %.2f minutes.\n\n' % (num_walkers*num_steps, total_time_mcmc)


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



    def draw_mpe_corner_plot(self, num_walkers, num_steps_to_include):
        
        l_labels_for_corner_plot = self.a_free_par_names
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
        
        if not os.path.exists(self.s_directory_save_name):
            os.makedirs(self.s_directory_save_name)

        fig.savefig(self.s_directory_save_name + self.s_base_save_name + '_corner.png')
        
        try:
            print emcee.autocorr.integrated_time(np.mean(a_sampler, axis=0), axis=0,
                                        low=10, high=None, step=1, c=2,
                                        fast=False)
        except:
            print 'Chain too short to find autocorrelation time!'




    def draw_best_fit_on_hist(self, num_walkers, num_steps_to_include):
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
        
        num_dim = len(self.a_free_par_guesses)
        a_sampler = a_sampler[:, -num_steps_to_include:, :num_dim].reshape((-1, num_dim))
        
        a_means = np.mean(a_sampler, axis=0)
        a_cov = np.cov(a_sampler.T)
        
        #print a_means
        #print a_cov
        
        
        a_medians = np.percentile(a_sampler, 50., axis=0)
        gain_800V_minus_one_sigma, gain_800V_median, gain_800V_plus_one_sigma = np.percentile(a_sampler[:,0], [16., 50, 84.])
        power_minus_one_sigma, power_median, power_plus_one_sigma = np.percentile(a_sampler[:,1], [16., 50, 84.])
        pol_par0_minus_one_sigma, pol_par0_median, pol_par0_plus_one_sigma = np.percentile(a_sampler[:,2], [16., 50, 84.])
        pol_par1_minus_one_sigma, pol_par1_median, pol_par1_plus_one_sigma = np.percentile(a_sampler[:,3], [16., 50, 84.])
        pol_par2_minus_one_sigma, pol_par2_median, pol_par2_plus_one_sigma = np.percentile(a_sampler[:,4], [16., 50, 84.])
        
        #print a_medians
        
        f1, (ax1) = plt.subplots(1)
        f2, (ax2) = plt.subplots(1)
        
        l_gains = [0. for i in xrange(len(self.l_voltage_settings))]
        l_gain_uncertainties = [0. for i in xrange(len(self.l_voltage_settings))]
        for i, voltage_setting in enumerate(self.l_voltage_settings):
            l_gains[i] = self.d_fit_files[voltage_setting]['sig_mean_value']
            l_gain_uncertainties[i] = self.d_fit_files[voltage_setting]['sig_mean_uncertainty']
        
        x_lb = min(self.l_voltage_settings)*0.95
        x_ub = max(self.l_voltage_settings)*1.05
        
        
        # draw confidence band
        def pyfunc_power_law(voltage, gain_800V, power, pol_par0, pol_par1, pol_par2):
            return gain_800V*np.power((voltage/800.), power) * max(min(pol_par0 + pol_par1*voltage + pol_par2*voltage**2, 1), 0)
        
        a_band_x_values, a_band_y_values, a_band_y_err_low, a_band_y_err_high = neriX_analysis.create_1d_fit_confidence_band(pyfunc_power_law, a_means, a_cov, x_lb, x_ub)
        
        x_line = np.linspace(x_lb, x_ub, 50)
        y_line = a_means[0]*np.power(x_line/800., a_means[1])*np.maximum(np.minimum(a_means[2] + a_means[3]*x_line + a_means[4]*x_line**2, [1 for i in xrange(len(x_line))]), [0 for i in xrange(len(x_line))])
        
        
        def pyfunc_eff(voltage, pol_par0, pol_par1, pol_par2):
            return max(min(pol_par0 + pol_par1*voltage + pol_par2*voltage**2, 1), 0)
        
        a_band_x_eff_values, a_band_y_eff_values, a_band_y_eff_err_low, a_band_y_eff_err_high = neriX_analysis.create_1d_fit_confidence_band(pyfunc_eff, a_means[2:], a_cov[2:,2:], x_lb, x_ub)
        y_eff_line = np.maximum(np.minimum(a_means[2] + a_means[3]*x_line + a_means[4]*x_line**2, [1 for i in xrange(len(x_line))]), [0 for i in xrange(len(x_line))])
        
        
        
    
        ax1.errorbar(self.l_voltage_settings, l_gains, yerr=l_gain_uncertainties, color='b', marker='.', linestyle='')
        ax1.plot(x_line, y_line, color='purple', linestyle='--')
        
        ax1.fill_between(a_band_x_values, a_band_y_values-a_band_y_err_low, a_band_y_values+a_band_y_err_high, facecolor='purple', alpha=0.2)
        
        #ax1.set_yscale('log', nonposx='clip')
        #ax1.set_title('PMT Power Law - %s' % (self.timestamp))
        ax1.set_xlabel(r'PMT Voltage [V]')
        ax1.set_ylabel(r'PMT Signal [$e^-$]')
        
        ax1.set_yscale('log', nonposy='clip')
        ax1.set_xlim(x_lb, x_ub)
        
        
        
        
        ax2.plot(x_line, y_eff_line, color='red', linestyle='--')
        
        ax2.fill_between(a_band_x_eff_values, a_band_y_eff_values-a_band_y_eff_err_low, a_band_y_eff_values+a_band_y_eff_err_high, facecolor='red', alpha=0.2)
        
        #ax2.set_yscale('log', nonposx='clip')
        #ax2.set_title('First Dynode Collection Efficiency - %s' % (self.timestamp))
        ax2.set_xlabel(r'PMT Voltage [V]')
        ax2.set_ylabel(r'Efficiency')
        
        #ax2.set_yscale('log', nonposy='clip')
        ax2.set_xlim(x_lb, x_ub)
        ax2.set_ylim(0, 1.05)
        
        
        
        
        
        
        s_formula = r'$g(V) = g(800 \, V)(\frac{V}{800 \, V})^{\beta} \cdot \epsilon(V)$'
        s_eff = '$\epsilon(V) = \gamma_0 + \gamma_1V + \gamma_2V^2$'
        s_eff_values = '$\gamma_0=%.2e^{+%.2e}_{-%.2e} [e^-]$\n' % (pol_par0_median, pol_par0_plus_one_sigma, pol_par0_minus_one_sigma)
        s_eff_values += '$\gamma_1=%.2e^{+%.2e}_{-%.2e} [e^-]$\n' % (pol_par1_median, pol_par1_plus_one_sigma, pol_par1_minus_one_sigma)
        s_eff_values += '$\gamma_2=%.2e^{+%.2e}_{-%.2e} [e^-]$' % (pol_par2_median, pol_par2_plus_one_sigma, pol_par2_minus_one_sigma)
        s_gain = r'$g(800 \, V)=%.2e^{+%.2e}_{-%.2e} [e^-]$' % (gain_800V_median, gain_800V_plus_one_sigma-gain_800V_median, gain_800V_median-gain_800V_minus_one_sigma)
        s_power = r'$\beta=%.2f^{+%.2e}_{-%.2e}$' % (power_median, power_plus_one_sigma-power_median, power_median-power_minus_one_sigma)
        ax1.text(0.3, 0.75, s_formula + '\n' + s_eff + '\n' + s_gain + '\n' + s_power + '\n' + s_eff_values, ha='center', va='center', transform=ax1.transAxes, fontsize=12)
        
        f1.tight_layout()
        f2.tight_layout()
    
        #plt.show()
        f1.savefig(self.s_directory_save_name + self.s_base_save_name + '_power_law_thesis.png')
        f2.savefig(self.s_directory_save_name + self.s_base_save_name + '_efficiency_thesis.png')
    



    def differential_evolution_minimizer(self, a_bounds, maxiter=250, tol=0.05, popsize=15, polish=False):
        def neg_log_likelihood_diff_ev(a_guesses):
            return -self.mpe_ln_likelihood(a_guesses)
        print '\n\nStarting differential evolution minimizer...\n\n'
        result = op.differential_evolution(neg_log_likelihood_diff_ev, a_bounds, disp=True, maxiter=maxiter, tol=tol, popsize=popsize, polish=polish)
        print result





if __name__ == '__main__':
    """
    d_files = {800:'nerix_160404_1014',
               750:'nerix_160404_1015',
               700:'nerix_160404_1016',
               650:'nerix_160404_1017',
               600:'nerix_160404_1018',
               550:'nerix_160404_1019',
               500:'nerix_160404_1020'}
    """
    
    
    d_files = {800:'nerix_160411_1205',
               750:'nerix_160411_1206',
               700:'nerix_160411_1207',
               650:'nerix_160411_1208',
               600:'nerix_160411_1209',
               550:'nerix_160411_1210',
               500:'nerix_160411_1211'}
    
    

    test = fit_mpe(d_files)

    #test.mpe_ln_likelihood(test.a_free_par_guesses)

    """
    a_bounds = [(1.47e9, 1.49e9), (9.5, 11.49), (0.08, 0.2), (4e-4, 5.9e-4), (4.5e-7, 1e-6)]
    test.differential_evolution_minimizer(a_bounds)
    """
    
    
    test.run_mpe_mcmc(num_walkers=16, num_steps=100, threads=1)
    test.draw_mpe_corner_plot(num_walkers=16, num_steps_to_include=2000)
    test.draw_best_fit_on_hist(num_walkers=16, num_steps_to_include=2000)




