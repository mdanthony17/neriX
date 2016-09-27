#!/usr/bin/python
import pickle
#print pickle.Pickler.dispatch
import dill
#print pickle.Pickler.dispatch

import ROOT as root
import sys, os

import matplotlib
matplotlib.use('QT4Agg')
import matplotlib.pyplot as plt

from rootpy.plotting import root2matplotlib as rplt

import emcee, corner, click
import neriX_analysis, neriX_datasets
from rootpy.plotting import Hist2D, Hist, Legend, Canvas
from rootpy.tree import Tree
import numpy as np
import tqdm, time, copy_reg, types, pickle
from root_numpy import tree2array, array2tree
import scipy.optimize as op
import scipy.special
import scipy.misc


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

class fit_gas_gain:
    def __init__(self, filename, radial_cut=(0, 0.85), tot_threshold_high=80):

        self.filename = filename
        self.tot_threshold_high = tot_threshold_high
        
        # make class methods pickleable for multithreading process
        copy_reg.pickle(types.MethodType, reduce_method)
        
        current_analysis = neriX_analysis.neriX_analysis(self.filename)
        

        hard_coded_correction_factor = 1.0 # 1.48e6 / 1.18e4 * (530./800.)**11.41
        neriX_analysis.warning_message('Using hard coded correction: %.2e' % hard_coded_correction_factor)

        self.bot_branch = 'S2sTotBottom[S2Order[0]]'#'ctS2sTotBottom[S2Order[0]]'
        self.top_branch = 'S2sTotTop[S2Order[0]]'
        asym_branch = '(ctS2sTotBottom[S2Order[0]] - S2sTotTop[S2Order[0]])/(ctS2sTotBottom[S2Order[0]] + S2sTotTop[S2Order[0]])'
        width_branch = 'S2sRightEdge[S2Order[0]] - S2sLeftEdge[S2Order[0]]'
        peak_branch = 'S2sPeak[S2Order[0]]'
        left_edge_branch = 'S2sLeftEdge[S2Order[0]]'
        right_edge_branch = 'S2sRightEdge[S2Order[0]]'
        
        self.l_bot_binning = [80, 0, 60]
        self.l_top_binning = [80, 0, 60]
        
        
        # --------------------------------------------
        # --------------------------------------------
        # Add cuts for analysis
        # --------------------------------------------
        # --------------------------------------------
        
        photoionization_window_gate = [100,200] # run 16
        
        iS2Wdith = 300
        iSamplesBetweenS2 = 200
        iSamplesAfterS2 = 1500


        # bot min cut
        current_analysis.add_cut('%s > %f' % (self.bot_branch, self.l_bot_binning[1]))

        # top min cut
        current_analysis.add_cut('%s > %f' % (self.top_branch, self.l_top_binning[1]))

        # S2 bottom high cut
        current_analysis.add_cut('%s < %f' % (self.bot_branch, self.l_bot_binning[2]))

        # S2 top high cut
        current_analysis.add_cut('%s < %f' % (self.top_branch, self.l_top_binning[2]))

        # noisy waveform cut
        current_analysis.add_cut('log10((S1Tot + S2Tot)/(AreaTot - S1Tot - S2Tot)) > 0.')

        # width cut
        current_analysis.add_cut('%s - %s < %d' % (right_edge_branch, left_edge_branch, iS2Wdith))

        # radial cut
        current_analysis.add_radius_cut(*radial_cut)
        
        # time cut for gate
        current_analysis.add_cut('%s - S1sPeak[0] > %d && %s - S1sPeak[0] < %d' % (peak_branch, photoionization_window_gate[0], peak_branch, photoionization_window_gate[1]))

        # make sure that next S2 (typically main one) isn't too close
        current_analysis.add_cut('Alt$(S2sLeftEdge[S2Order[1]],100000) - S2sRightEdge[S2Order[0]] > %i' % (iSamplesBetweenS2))
        
        
        current_analysis.set_event_list()


        # --------------------------------------------
        # --------------------------------------------
        #  Fill histogram
        # --------------------------------------------
        # --------------------------------------------

        
        self.run_number = current_analysis.get_run()
        self.h_bot_top = Hist2D(*(self.l_bot_binning+self.l_top_binning), name='h_bot_top_%s' % (self.filename), title='Gas Gain %s - V_{c} %.3f kV' % (self.filename, current_analysis.get_cathode_setting()))
        self.h_bot_top.SetStats(0)
        current_analysis.Draw('%s:%s' % (self.bot_branch, self.top_branch), hist=self.h_bot_top, selection=current_analysis.get_cuts())
        
        # tot max cut
        current_analysis.add_cut('%s + %s < %f' % (self.bot_branch, self.top_branch, tot_threshold_high))
        
        
        # --------------------------------------------
        # --------------------------------------------
        #  Fill array
        # --------------------------------------------
        # --------------------------------------------
        
        
        self.a_bot_top = tree2array(current_analysis.get_T1(), [self.bot_branch, self.top_branch], selection=current_analysis.get_cuts())
        
        self.s_directory_save_name = './results/%s/' % (self.filename)
        self.s_base_save_name = 'gas_gain_%s_' % (self.filename)
        self.dict_filename = 'sampler_dictionary.p'
        self.acceptance_filename = 'acceptance_fraction.p'
        
        
        self.a_free_par_guesses = [13.5, 4.9, 15.5, 5., 0., 5., 5., 5., 3., 0.26, 0.05]
        self.a_free_par_std_guesses = [1.5, 0.5, 1.5, 0.5, 0.25, 1.5, 1.5, 1.5, 1.5, 0.05, 0.015]
        self.a_free_par_names = ['bot_mean', 'bot_width', 'top_mean', 'top_width', 'correlation', 'fd_center_tot', 'fd_shape_tot', 'fd_center_top', 'fd_shape_top', 'rel_prob_2e', 'rel_prob_3e']
        
        



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



    def prior_correlation(self, parameter_to_examine):
        if parameter_to_examine > -1 and parameter_to_examine < 1:
            return 0
        else:
            return -np.inf


    def gas_gain_ln_likelihood(self, a_parameters):
        bot_mean, bot_width, top_mean, top_width, correlation, fd_center_tot, fd_shape_tot, fd_center_top, fd_shape_top, p_mult_two, p_mult_three = a_parameters

        ln_prior = 0
        ln_likelihood = 0

        ln_prior += self.prior_greater_than_0(bot_mean)
        ln_prior += self.prior_greater_than_0(bot_width)
        ln_prior += self.prior_greater_than_0(top_mean)
        ln_prior += self.prior_greater_than_0(top_width)
        ln_prior += self.prior_correlation(correlation)
        ln_prior += self.prior_greater_than_0(fd_center_tot)
        ln_prior += self.prior_greater_than_0(fd_shape_tot)
        ln_prior += self.prior_greater_than_0(fd_center_top)
        ln_prior += self.prior_greater_than_0(fd_shape_top)
        ln_prior += self.prior_between_0_and_1(p_mult_two)
        ln_prior += self.prior_between_0_and_1(p_mult_three)
        
        if not np.isfinite(ln_prior):
            return -np.inf

        x = self.a_bot_top[self.bot_branch]
        y = self.a_bot_top[self.top_branch]

        # weights for sum of exponentials
        # weights must have same shape as exponents
        l_multiplers = np.asarray([1., p_mult_two, p_mult_three])
        l_multiplers /= sum(l_multiplers)
        l_weights = np.asarray([np.asarray([l_multiplers[i]/(2*np.pi*(i+1)*bot_width*top_width*(1-correlation**2)**0.5) for i in xrange(3)]) for j in xrange(len(x))]).T
        # width expansion term is in prefactor
        
        l_exponents = np.asarray([-1./(2*(i+1)*(1-correlation**2)) * ( (x-(i+1)*bot_mean)**2/bot_width**2 + (y-(i+1)*top_mean)**2/top_width**2 - 2*correlation*(x-(i+1)*bot_mean)*(y-(i+1)*top_mean)/bot_width/top_width ) for i in xrange(3)])

        ln_fd = -np.log1p(np.exp((fd_center_tot-x-y)/fd_shape_tot)) - np.log1p(np.exp((fd_center_top-x-y)/fd_shape_top))

        # first is an array of the unbinned likelihoods

        #print l_weights.shape
        #print l_exponents.shape
        #print scipy.misc.logsumexp(l_exponents, axis=0, b=l_weights)
        ln_likelihood = ln_fd + scipy.misc.logsumexp(l_exponents, axis=0, b=l_weights)
        #print ln_likelihood
        ln_likelihood = np.sum(ln_likelihood)
        #print ln_likelihood


        return ln_likelihood + ln_prior
        
        
        
        
    def run_gas_gain_mcmc(self, num_walkers=32, num_steps=2000, threads=1):
        
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
        sampler = emcee.DESampler(num_walkers, num_dim, self.gas_gain_ln_likelihood, threads=threads, autoscale_gamma=True)
        
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
        



    def gas_gain_likelihood_function(self, x, y, a_parameters):
        bot_mean, bot_width, top_mean, top_width, correlation, fd_center_tot, fd_shape_tot, fd_center_top, fd_shape_top, p_mult_two, p_mult_three = a_parameters
        
        x, y = np.asarray(x), np.asarray(y)
    
        fd_tot = 1./(1. + np.exp((fd_center_tot-x-y)/fd_shape_tot))
        fd_top = 1./(1. + np.exp((fd_center_top-x-y)/fd_shape_top))
    
        l_multiplers = np.asarray([1., p_mult_two, p_mult_three])
        l_multiplers /= sum(l_multiplers)
        l_weights = np.asarray([np.asarray([l_multiplers[i]/(2*np.pi*(i+1)*bot_width*top_width*(1-correlation**2)**0.5) for i in xrange(3)]) for j in xrange(len(x))]).T
        # width expansion term is in prefactor
        
        l_exponents = np.asarray([-1./(2*(i+1)*(1-correlation**2)) * ( (x-(i+1)*bot_mean)**2/bot_width**2 + (y-(i+1)*top_mean)**2/top_width**2 - 2*correlation*(x-(i+1)*bot_mean)*(y-(i+1)*top_mean)/bot_width/top_width ) for i in xrange(3)])
        
    
        likelihood = 1.
        likelihood *= fd_tot*fd_top
        exponential_terms = 0.
        for i in xrange(3):
            exponential_terms += l_weights[i,:]*np.exp(l_exponents[i,:])
        
        likelihood *= exponential_terms

        return likelihood
    
    
    
    

    def draw_gas_gain_corner_plot(self, num_walkers, num_steps_to_include):
        
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
        
        a_medians = np.percentile(a_sampler, 50., axis=0)
        #print a_medians
        
        f1, (ax1) = plt.subplots(1)
        
        rplt.hist2d(self.h_bot_top, axes=ax1)
        
        x = np.linspace(self.l_bot_binning[1], self.l_bot_binning[2], self.l_bot_binning[0])
        y = np.linspace(self.l_top_binning[1], self.l_top_binning[2], self.l_top_binning[0])
        
        x_line = x
        y_line = self.tot_threshold_high - x_line
        
        ax1.plot(x_line, y_line, color='magenta', linestyle='--')
        
        x, y = np.meshgrid(x, y)
        z = self.gas_gain_likelihood_function(x, y, a_medians)
    
        l_levels = np.array([0.01, 0.05, 0.1, 0.2, 0.3, 0.5, 0.9])*np.max(z)
    
        ax1.contour(x, y, z, cmap=plt.get_cmap('Greys'), levels=l_levels)
        #ax1.set_yscale('log', nonposx='clip')
        ax1.set_title('Gas Gain - %s' % (self.filename))
        ax1.set_xlabel(r'$S2_{bot} [PE]$')
        ax1.set_ylabel(r'$S2_{top} [PE]$')
    
        #plt.show()
        f1.savefig(self.s_directory_save_name + self.s_base_save_name + '_hist.png')
    



    def differential_evolution_minimizer(self, a_bounds, maxiter=250, tol=0.05, popsize=15, polish=False):
        def neg_log_likelihood_diff_ev(a_guesses):
            return -self.gas_gain_ln_likelihood(a_guesses)
        print '\n\nStarting differential evolution minimizer...\n\n'
        result = op.differential_evolution(neg_log_likelihood_diff_ev, a_bounds, disp=True, maxiter=maxiter, tol=tol, popsize=popsize, polish=polish)
        print result





if __name__ == '__main__':
    #test = fit_gas_gain('nerix_160404_1059')
    test = fit_gas_gain('nerix_160407_1357')
    #test = fit_gas_gain('nerix_160412_0919')
    #test = fit_gas_gain('nerix_160414_1215')
    #test = fit_gas_gain('nerix_160418_0919')
    #test = fit_gas_gain('nerix_160620_1048')
    #test.gas_gain_ln_likelihood(test.a_free_par_guesses)

    #a_bounds = [(10, 20), (1, 9), (10, 20), (1, 9), (-1, 1), (1, 40), (0.1, 10), (1, 13), (0.1, 10), (0, 0.5), (0, 0.2)]
    #test.differential_evolution_minimizer(a_bounds, maxiter=20, tol=0.01, popsize=15)

    test.run_gas_gain_mcmc(num_walkers=64, num_steps=500, threads=7)
    test.draw_gas_gain_corner_plot(num_walkers=64, num_steps_to_include=200)
    test.draw_best_fit_on_hist(num_walkers=64, num_steps_to_include=100)



