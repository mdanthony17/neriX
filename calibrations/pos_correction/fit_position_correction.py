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

import pandas
import root_pandas

from rootpy.plotting import root2matplotlib as rplt

import emcee, corner, click
import neriX_analysis, neriX_datasets
from rootpy.plotting import Hist2D, Hist, Legend, Canvas
from rootpy.tree import Tree
import numpy as np
import tqdm, time, copy_reg, types, pickle
from root_numpy import tree2array, array2tree
import scipy.optimize as op
import scipy.stats
import scipy.special
import scipy.misc



def neg_ln_likelihood_2d_gaussian(a_parameters, x):
    mean_x, mean_y, var_x, var_y, cov_xy = a_parameters
    return -np.sum(scipy.stats.multivariate_normal.logpdf(x.T, mean=[mean_x, mean_y], cov=[[var_x, cov_xy], [cov_xy, var_y]]))

def neg_ln_likelihood_1d_gaussian(a_parameters, x):
    mean_x, stdev_x = a_parameters
    #print a_parameters
    #print -np.sum(scipy.stats.norm.logpdf(x, loc=mean_x, scale=stdev_x))
    return -np.sum(scipy.stats.norm.logpdf(x, loc=mean_x, scale=stdev_x))


class fit_position_corrections:
    def __init__(self, d_files):
        self.l_s1_settings = [40, 1200, 4500]
        self.l_s2_settings = [40, 3e5, 10e5]
        self.l_dt_settings = [25, 0, 26]
        self.l_z_settings = [20, -24.3, -1.0]
        self.l_ces_settings = [50, 0, 900]
        
        self.d_files = d_files
        
        self.dt_gate_offset = neriX_datasets.d_dt_offset_gate[16]
        self.max_dt = 18
        self.l_z_bounds = [-24.3, -1]
        self.l_center_of_detector = [-12.65, -10.65]
        self.z_cut_electron_lifetime = -12.5

        self.l_radial_cut = [0, 0.85]
        
        self.s1_branch = 'S1sTotBottom[0]'
        self.s2_branch = 'S2sTotBottom[0]'
        
        self.l_cathode_settings = self.d_files.keys()
        self.l_cathode_settings.sort()
        
        self.save_name = self.d_files[self.l_cathode_settings[0]][6:12]

        self.s_directory_save_name = './results/%s/' % (self.save_name)
        self.dict_filename = 'sampler_dictionary.p'
        self.acceptance_filename = 'acceptance_fraction.p'

        l_column_names = ['s1', 's2', 'dt', 'z']
        l_columns = [np.empty(0) for i in xrange(len(l_column_names))]
        
        self.d_info = {}

        for cathode_setting in self.l_cathode_settings:
            
            current_analysis = neriX_analysis.neriX_analysis(d_files[cathode_setting])
            
            current_analysis.add_z_cut()
            current_analysis.add_single_scatter_cut()
            current_analysis.add_radius_cut(0, 0.85)
            
            run_number = current_analysis.get_run()
            
            current_analysis.set_event_list()
            
            current_tree = tree2array(current_analysis.get_T1(), [self.s1_branch, self.s2_branch, current_analysis.get_T1().GetAlias('dt'), current_analysis.get_T1().GetAlias('Z')], selection=current_analysis.get_cuts())
            current_tree.dtype.names = l_column_names
            column_dtype = current_tree.dtype
            
            
            for i, column_name in enumerate(current_tree.dtype.names):
                l_columns[i] = np.append(l_columns[i], current_tree[column_name])

            del current_analysis

        a_combined_tree = np.empty(len(l_columns[0]), dtype=column_dtype)
        for i, column_name in enumerate(l_column_names):
            a_combined_tree[column_name] = l_columns[i]

        self.d_info['combined_array'] = a_combined_tree
        self.d_info['combined_tree'] = array2tree(a_combined_tree, name='combined_tree')

        # ---------- MAKE CES CUT ----------

        # no time correction: 0.08, 14.
        correction_factor = 1.48e6 / 9.5e5
        print 'Correction factor: %.3e' % (correction_factor)
        g1 = 0.13/correction_factor
        g2 = 21./correction_factor
        sigma_ces = 2.0

        c_ces = Canvas()
        c_ces.SetGridx()
        c_ces.SetGridy()

        h_ces = Hist(*self.l_ces_settings, name='h_ces', title='h_ces', drawstyle='hist')
        self.d_info['combined_tree'].Draw('0.0137*(s1/%f + s2/%f)>>h_ces' % (g1, g2))

        h_ces.Draw()
        c_ces.Update()

        #lb_ces = float(raw_input('\n\nPlease enter lower bound on combined energy scale: '))
        #ub_ces = float(raw_input('\n\nPlease enter upper bound on combined energy scale: '))

        #print '\n\nDEBUG MODE SKIPPING LIMIT PICKS\n\n'
        lb_ces = 500
        ub_ces = 900

        #f_ces = root.TF1('f_ces', 'gaus', lb_ces, ub_ces)
        f_ces = root.TF1('f_ces', '[0]*exp(-[1]*x) + [2]*exp(-0.5*pow((x-[3])/[4], 2))', lb_ces, ub_ces)
        f_ces.SetParameters(500, 0.01, 500, 650, 50)
        f_ces.SetParLimits(3, lb_ces, ub_ces)
        f_ces.SetParLimits(0, 1, 1e8)
        f_ces.SetParLimits(2, 1, 1e8)
        h_ces.Fit(f_ces, 'MELSR')
        f_ces.Draw('same')
        c_ces.Update()

        lb_ces = f_ces.GetParameter(3) - sigma_ces*f_ces.GetParameter(4)
        ub_ces = f_ces.GetParameter(3) + sigma_ces*f_ces.GetParameter(4)
        #raw_input('Fit okay?')


        s_ces_cut = '(0.0137*(%s/%f + %s/%f) > %f && 0.0137*(%s/%f + %s/%f) < %f)' % (self.s1_branch, g1, self.s2_branch, g2, lb_ces, self.s1_branch, g1, self.s2_branch, g2, ub_ces)


        # now add individual arrays to dictionary
        for cathode_setting in self.l_cathode_settings:
            self.d_info[cathode_setting] = {}
            
            current_analysis = neriX_analysis.neriX_analysis(d_files[cathode_setting])
            
            current_analysis.add_z_cut()
            current_analysis.add_single_scatter_cut()
            current_analysis.add_radius_cut(0, 0.85)
            current_analysis.add_cut(s_ces_cut)
            
            run_number = current_analysis.get_run()
            
            current_analysis.set_event_list()
            
            self.d_info[cathode_setting]['a_tree'] = tree2array(current_analysis.get_T1(), [self.s1_branch, self.s2_branch, current_analysis.get_T1().GetAlias('dt'), current_analysis.get_T1().GetAlias('Z')], selection=current_analysis.get_cuts())
            self.d_info[cathode_setting]['a_tree'].dtype.names = l_column_names
            
            # create datafram
            self.d_info[cathode_setting]['df_tree'] = root_pandas.readwrite.convert_to_dataframe(self.d_info[cathode_setting]['a_tree'])
            self.d_info[cathode_setting]['df_tree'] = self.d_info[cathode_setting]['df_tree'][ (self.d_info[cathode_setting]['df_tree'].s1 > 0) & (self.d_info[cathode_setting]['df_tree'].s2 > 0) & (self.d_info[cathode_setting]['df_tree'].s1 < np.inf) & (self.d_info[cathode_setting]['df_tree'].s2 < np.inf)]
            
            # fit 2d gaussian and eliminate points that are clearly from dead scatters
            # first estimate means and covariance
            
            #estimated_mean_s1 = np.median(self.d_info[cathode_setting]['a_tree']['s1'])
            estimated_mean_s2 = np.median(self.d_info[cathode_setting]['a_tree']['s2'])
            #estimated_cov_matrix = np.cov([self.d_info[cathode_setting]['a_tree']['s1'], self.d_info[cathode_setting]['a_tree']['s2']])
            #estimated_var_s1 = estimated_cov_matrix[0, 0]
            estimated_var_s2 = np.var(self.d_info[cathode_setting]['a_tree']['s2']) #estimated_cov_matrix[1, 1]
            #estimated_cov_s1_s2 = estimated_cov_matrix[0, 1]
            
            #a_2d_gaus_guesses = [estimated_mean_s1, estimated_mean_s2, estimated_var_s1, estimated_var_s2, estimated_cov_s1_s2]
            #data_array = np.asarray([self.d_info[cathode_setting]['a_tree']['s1'], self.d_info[cathode_setting]['a_tree']['s2']])
            #result_2d_guas_fit = op.minimize(neg_ln_likelihood_2d_gaussian, a_2d_gaus_guesses, args=np.asarray(data_array))
            #gaus_mean_s1, gaus_mean_s2, gaus_var_s1, gaus_var_s2, gaus_cov_s1_s2 =  result_2d_guas_fit.x
            """
            a_1d_gaus_guesses = [estimated_mean_s2, estimated_var_s2]
            print a_1d_gaus_guesses
            data_array = np.asarray(self.d_info[cathode_setting]['df_tree'][ (self.d_info[cathode_setting]['df_tree'].s2 > estimated_mean_s2) ]['s2'])
            result_1d_guas_fit = op.minimize(neg_ln_likelihood_1d_gaussian, a_1d_gaus_guesses, args=np.asarray(data_array))
            gaus_mean_s2, gaus_var_s2 =  result_1d_guas_fit.x
            
            # add cuts to dataframe elimintating points far outside gaussian
            ln_likelihood_difference = 1
            
            length_before_gaus_cut = len(self.d_info[cathode_setting]['df_tree'])
            #self.d_info[cathode_setting]['df_tree'] = self.d_info[cathode_setting]['df_tree'][scipy.stats.multivariate_normal.logpdf([gaus_mean_s1, gaus_mean_s2], mean=[gaus_mean_s1, gaus_mean_s2], cov=[[gaus_var_s1, gaus_cov_s1_s2], [gaus_cov_s1_s2, gaus_var_s2]]) - scipy.stats.multivariate_normal.logpdf(data_array.T, mean=[gaus_mean_s1, gaus_mean_s2], cov=[[gaus_var_s1, gaus_cov_s1_s2], [gaus_cov_s1_s2, gaus_var_s2]]) < ln_likelihood_difference]
            print scipy.stats.norm.logpdf(gaus_mean_s2, loc=gaus_mean_s2, scale=gaus_var_s2**0.5) - scipy.stats.norm.logpdf(self.d_info[cathode_setting]['df_tree']['s2'], loc=gaus_mean_s2, scale=gaus_var_s2**0.5)
            self.d_info[cathode_setting]['df_tree'] = self.d_info[cathode_setting]['df_tree'][scipy.stats.norm.logpdf(gaus_mean_s2, loc=gaus_mean_s2, scale=gaus_var_s2**0.5) - scipy.stats.norm.logpdf(self.d_info[cathode_setting]['df_tree']['s2'], loc=gaus_mean_s2, scale=gaus_var_s2**0.5) < ln_likelihood_difference]
            length_after_gaus_cut = len(self.d_info[cathode_setting]['df_tree'])
            print '\n\nEvents removed by 2D gaussian cut: %d\n\n' % (length_before_gaus_cut - length_after_gaus_cut)
            """
            
            
            # draw figure with contours
            
            if not os.path.exists(self.s_directory_save_name + str(cathode_setting)):
                os.makedirs(self.s_directory_save_name + str(cathode_setting))
            
            l_percentiles = [35, 55, 95]
            estimated_lower_bound, estimated_mean, estimated_upper_bound = np.percentile(-self.d_info[cathode_setting]['df_tree']['s1']/g2 + self.d_info[cathode_setting]['df_tree']['s2']/g1, l_percentiles)
            print estimated_lower_bound, estimated_mean
            estimated_stdev = 5e5
            width_cut = 3.
            a_1d_gaus_guesses = np.asarray([estimated_mean, estimated_stdev])
            
            # make dataframe with rough upper bound in discrimination space
            reduced_df = self.d_info[cathode_setting]['df_tree'][((-self.d_info[cathode_setting]['df_tree']['s1']/g2 + self.d_info[cathode_setting]['df_tree']['s2']/g1) > estimated_lower_bound) & ((-self.d_info[cathode_setting]['df_tree']['s1']/g2 + self.d_info[cathode_setting]['df_tree']['s2']/g1) < estimated_upper_bound)]
            
            # fit reduced data with gaussian
            data_array = np.asarray(-reduced_df['s1']/g2 + reduced_df['s2']/g1)
            data_array = data_array[np.isfinite(data_array)]
            #result_1d_guas_fit = op.minimize(neg_ln_likelihood_1d_gaussian, a_1d_gaus_guesses, args=np.asarray(data_array))
            result_1d_guas_fit = op.differential_evolution(neg_ln_likelihood_1d_gaussian, bounds=[(3e6, 7e6), (1e5, 1.5e6)], args=[data_array], polish=True)
            
            gaus_mean, gaus_stdev =  result_1d_guas_fit.x
            
            

            f1, (ax1) = plt.subplots(1)
            #dummy1, x_edges, y_edges, dummy2 = ax1.hist2d(self.d_info[cathode_setting]['df_tree']['s2'], np.log(self.d_info[cathode_setting]['df_tree']['s2']/self.d_info[cathode_setting]['df_tree']['s1']), bins=40)
            dummy1, x_edges, dummy2 = ax1.hist(-self.d_info[cathode_setting]['df_tree']['s1']/g2 + self.d_info[cathode_setting]['df_tree']['s2']/g1, bins=60, normed=True)
            
            #x_contour, y_contour = np.meshgrid(x_edges, y_edges)
            #z_contour = scipy.stats.multivariate_normal.pdf(np.asarray([x_contour, y_contour]).T, mean=[gaus_mean_s1, gaus_mean_s2], cov=[[gaus_var_s1, gaus_cov_s1_s2], [gaus_cov_s1_s2, gaus_var_s2]])
            #l_levels = np.array([0.01, 0.05, 0.1, 0.2, 0.3, 0.5, 0.9])*np.max(z_contour)
            #ax1.contour(x_contour, y_contour, z_contour, cmap=plt.get_cmap('Greys'), levels=l_levels)
            
            x_gaus = np.linspace(x_edges[0], x_edges[-1], 100)
            y_gaus = scipy.stats.norm.pdf(x_gaus, loc=gaus_mean, scale=gaus_stdev)
            ax1.plot(x_gaus, y_gaus, color='magenta', linestyle='--')
            
            ax1.set_title('Double Scatter Removal - %.3f kV' % (cathode_setting))
            ax1.set_xlabel(r'$-\frac{S1}{g_{2}} + \frac{S2}{g1}$')
            ax1.set_ylabel(r'Normalized Counts')
            
            
            f1.savefig(self.s_directory_save_name + '%s/' % (cathode_setting) + 'double_scatters_%d,%d.png' % (l_percentiles[0], l_percentiles[2]))
            
            
            # add cut to dataframe
            self.d_info[cathode_setting]['df_tree'] = self.d_info[cathode_setting]['df_tree'][ ( -self.d_info[cathode_setting]['df_tree']['s1']/g2 + self.d_info[cathode_setting]['df_tree']['s2']/g1 > (gaus_mean - width_cut*gaus_stdev) ) & ( -self.d_info[cathode_setting]['df_tree']['s1']/g2 + self.d_info[cathode_setting]['df_tree']['s2']/g1 < (gaus_mean + width_cut*gaus_stdev) ) ]
            

            df_current = self.d_info[cathode_setting]['df_tree']
            
            self.d_info[cathode_setting]['df_tree_top_half'] = self.d_info[cathode_setting]['df_tree'][self.d_info[cathode_setting]['df_tree'].z > self.z_cut_electron_lifetime]
            
            current_s1_mean_center = np.mean(df_current[(df_current.z > self.l_center_of_detector[0]) & (df_current.z < self.l_center_of_detector[1])]['s1'])
            current_s2_mean_center = np.mean(df_current[(df_current.z > self.l_center_of_detector[0]) & (df_current.z < self.l_center_of_detector[1])]['s2'])
            self.d_info[cathode_setting]['a_scaled_s1'] = self.d_info[cathode_setting]['df_tree']['s1'] / current_s1_mean_center
            self.d_info[cathode_setting]['s2_scale_guess'] = current_s2_mean_center


        
        self.d_free_par_guesses = {}
        self.d_free_par_guesses['s1_z'] = {}
        self.d_free_par_guesses['s2_dt'] = {}
        
        self.d_free_par_std_guesses = {}
        self.d_free_par_std_guesses['s1_z'] = {}
        self.d_free_par_std_guesses['s2_dt'] = {}
        
        self.d_par_names = {}
        self.d_par_names['s1_z'] = ['intercept', 'slope']
        self.d_par_names['s2_dt'] = ['scale', 'electron_lifetime', 'intrinsic_width', 'diffusion_const']
        
        for cathode_setting in self.l_cathode_settings:
            self.d_free_par_guesses['s1_z'][cathode_setting] = [0.88, -0.01]
            self.d_free_par_guesses['s2_dt'][cathode_setting] = [self.d_info[cathode_setting]['s2_scale_guess']*1.05, 400, self.d_info[cathode_setting]['s2_scale_guess']*0.1, 2e4]
        
            self.d_free_par_std_guesses['s1_z'][cathode_setting] = [0.1, -0.001]
            self.d_free_par_std_guesses['s2_dt'][cathode_setting] = [self.d_info[cathode_setting]['s2_scale_guess']*0.2, 100, self.d_info[cathode_setting]['s2_scale_guess']*0.02, 5e3]




        
        

        
        


    def ln_likelihood_s1_z(self, a_parameters, cathode_setting):
        intercept, slope = a_parameters
        

        ln_likelihood = -np.sum(( self.d_info[cathode_setting]['a_scaled_s1'] - (intercept + slope*self.d_info[cathode_setting]['df_tree']['z']) )**2)

        if not np.isfinite(ln_likelihood):
            return -np.inf
        else:
            return ln_likelihood
        
        
        
    def ln_likelihood_s2_dt(self, a_parameters, cathode_setting):
        scale, e_lifetime, intrinsic_width, diffusion_const = a_parameters
        
        if e_lifetime <= 0. or scale <= 0. or e_lifetime > 1000 or intrinsic_width <= 0 or diffusion_const <= 0:
            return -np.inf
        
        s2_width = intrinsic_width + diffusion_const*(self.d_info[cathode_setting]['df_tree']['dt']-self.dt_gate_offset)**0.5
        ln_likelihood = np.sum(-( self.d_info[cathode_setting]['df_tree']['s2'] - (scale*np.exp( -(self.d_info[cathode_setting]['df_tree']['dt']-self.dt_gate_offset)/e_lifetime )) )**2/s2_width**2/2. - np.log(s2_width))
        
        if not np.isfinite(ln_likelihood):
            return -np.inf
        else:
            return ln_likelihood



    def run_mcmc(self, analysis_name, cathode_setting, num_walkers, num_steps, threads=1):
        if analysis_name == 's1_z':
            ln_likelihood_func = self.ln_likelihood_s1_z
        elif analysis_name == 's2_dt':
            ln_likelihood_func = self.ln_likelihood_s2_dt
        else:
            print '\nAnalysis should either be "s1_z" or "s2_dt"\n\n'
            return

        self.s_base_save_name = '%s/%s_' % (cathode_setting, analysis_name)

        l_value_guesses = self.d_free_par_guesses[analysis_name][cathode_setting]
        l_std_guesses = self.d_free_par_std_guesses[analysis_name][cathode_setting]
        l_par_names = self.d_par_names[analysis_name]

        if not os.path.exists(self.s_directory_save_name + str(cathode_setting)):
            os.makedirs(self.s_directory_save_name + str(cathode_setting))
        num_dim = len(l_value_guesses)
        
        
        loaded_prev_sampler = False
        try:
            # two things will fail potentially
            # 1. open if file doesn't exist
            # 2. posWalkers load since initialized to None

            with open(self.s_directory_save_name + self.s_base_save_name + self.dict_filename, 'r') as f_prev_sampler:

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
            if not os.path.exists(self.s_directory_save_name + self.s_base_save_name + self.dict_filename):
                with open(self.s_directory_save_name + self.s_base_save_name + self.dict_filename, 'w') as f_prev_sampler:
                    d_sampler = {}

                    d_sampler[num_walkers] = []

                    pickle.dump(d_sampler, f_prev_sampler)
            else:
                with open(self.s_directory_save_name + self.s_base_save_name + self.dict_filename, 'r') as f_prev_sampler:
                    d_sampler = pickle.load(f_prev_sampler)
                with open(self.s_directory_save_name + self.s_base_save_name + self.dict_filename, 'w') as f_prev_sampler:

                    d_sampler[num_walkers] = []

                    pickle.dump(d_sampler, f_prev_sampler)
        
        

        #print a_starting_pos

        #sampler = emcee.EnsembleSampler(num_walkers, num_dim, ln_likelihood_func, threads=threads)
        sampler = emcee.DESampler(num_walkers, num_dim, ln_likelihood_func, threads=threads, autoscale_gamma=True, args=[cathode_setting])
        
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

        with open(self.s_directory_save_name + self.s_base_save_name + self.dict_filename, 'r') as f_prev_sampler:
            d_sampler = pickle.load(f_prev_sampler)
        #f_prev_sampler.close()

        f_prev_sampler = open(self.s_directory_save_name + self.s_base_save_name + self.dict_filename, 'w')

        d_sampler[num_walkers].append(sampler.__dict__)

        pickle.dump(d_sampler, f_prev_sampler)
        f_prev_sampler.close()



        #sampler.run_mcmc(posWalkers, numSteps) # shortcut of above method
        pickle.dump(sampler.acceptance_fraction, open(self.s_directory_save_name + self.s_base_save_name + self.acceptance_filename, 'w'))



    def draw_pos_correction_plots(self, analysis_name, cathode_setting, num_walkers, num_steps_to_include):
        if not (analysis_name == 's1_z' or analysis_name == 's2_dt'):
            print '\nAnalysis should either be "s1_z" or "s2_dt"\n\n'
            return
        
        l_labels_for_corner_plot = self.d_par_names[analysis_name]
        num_dim = len(l_labels_for_corner_plot)
        
        self.s_base_save_name = '%s/%s_' % (cathode_setting, analysis_name)
        
        sPathToFile = self.s_directory_save_name + self.s_base_save_name + self.dict_filename
        
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
        fig = corner.corner(a_sampler, labels=l_labels_for_corner_plot, quantiles=[0.16, 0.5, 0.84], show_titles=True, title_fmt='.3e', title_kwargs={"fontsize": 8})
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


        a_medians = np.median(a_sampler, axis=0)

        f1, (ax1) = plt.subplots(1)
        if analysis_name == 's1_z':
            ax1.plot(self.d_info[cathode_setting]['df_tree']['z'], self.d_info[cathode_setting]['a_scaled_s1'], color='b', linestyle='', marker='.')
            
            x_line = np.linspace(self.l_z_bounds[0], self.l_z_bounds[1], 50)
            y_line = a_medians[0] + a_medians[1]*x_line
            ax1.plot(x_line, y_line, color='magenta', linestyle='--')
            
            ax1.set_title('Scaled S1 vs. Z - %.3f kV' % (cathode_setting))
            ax1.set_xlabel(r'$Z [mm]$')
            ax1.set_ylabel(r'$Scaled S1$')
        
        if analysis_name == 's2_dt':
            ax1.plot(self.d_info[cathode_setting]['df_tree']['dt'], self.d_info[cathode_setting]['df_tree']['s2'], color='b', linestyle='', marker='.')
            

            x_line = np.linspace(self.dt_gate_offset, self.max_dt, 50)
            y_line = a_medians[0]*np.exp(-(x_line-self.dt_gate_offset)/a_medians[1])
            ax1.plot(x_line, y_line, color='magenta', linestyle='--')
        
            ax1.set_title('Electron Lifetime - %.3f kV' % (cathode_setting))
            ax1.set_xlabel(r'$dt [us]$')
            ax1.set_ylabel(r'$S2 [PE]$')

        f1.savefig(self.s_directory_save_name + self.s_base_save_name + '_%s.png' % (analysis_name))






if __name__ == '__main__':
    d_files = {}
    d_files[0.345] = 'nerix_160411_0612'
    d_files[0.700] = 'nerix_160411_0644'
    d_files[1.054] = 'nerix_160411_0712'
    d_files[1.500] = 'nerix_160411_0739'
    d_files[2.356] = 'nerix_160411_0925'

    test = fit_position_corrections(d_files)

    for cathode_setting in d_files:
        test.run_mcmc('s1_z', cathode_setting, 32, 1000, threads=1)
        test.draw_pos_correction_plots('s1_z', cathode_setting, 32, 500)
        test.run_mcmc('s2_dt', cathode_setting, 32, 1000, threads=1)
        test.draw_pos_correction_plots('s2_dt', cathode_setting, 32, 500)


