
import matplotlib
matplotlib.use('QT4Agg')

import ROOT as root
from ROOT import gROOT

import sys, os, root_numpy, threading, random, emcee, corner, signal, tqdm

from rootpy import stl
from rootpy.io import File
from rootpy.tree import Tree, TreeModel, TreeChain
from rootpy.plotting import Hist, Hist2D, Canvas, Graph, func

import neriX_simulation_config, neriX_analysis, neriX_datasets

import numpy as np
from math import exp, factorial, erf, ceil, log, pow, floor, lgamma

from scipy import optimize, misc, stats
from scipy.stats import norm, multivariate_normal, binom

import copy_reg, types, pickle, click, time
from subprocess import call
from pprint import pprint

from produce_nest_yields import nest_nr_mean_yields

import cuda_full_observables_production
from pycuda.compiler import SourceModule
import pycuda.driver as drv
import pycuda.tools
import pycuda.gpuarray
from Queue import Queue
import threading

import pandas as pd



def reduce_method(m):
    return (getattr, (m.__self__, m.__func__.__name__))


d_cathode_voltage_to_field = {0.345:210,
                              1.054:490,
                              2.356:1000}



class gpu_pool:
    def __init__(self, l_gpus, grid_dim, block_dim, num_dim_gpu_call, d_gpu_single_copy_arrays, function_name):
        self.num_gpus = len(l_gpus)
        self.grid_dim = grid_dim
        self.block_dim = block_dim
        self.num_dim_gpu_call = num_dim_gpu_call
        self.d_gpu_single_copy_arrays = d_gpu_single_copy_arrays
        self.function_name = function_name

        self.alive = True
        self.q_gpu = Queue()
        for i in l_gpus:
            self.q_gpu.put(i)

        self.q_in = Queue()
        self.q_out = Queue()
        self.l_dispatcher_threads = []
        self.dispatcher_dead_time = 0.5

        self.q_dead = Queue()


        for i in l_gpus:
            if self.q_gpu.empty():
                break
            print 'Starting Worker...'
            self.l_dispatcher_threads.append(threading.Thread(target=self.dispatcher, args=[self.q_gpu.get()]))
            self.l_dispatcher_threads[-1].start()


    def dispatcher(self, device_num):

        try:
            drv.init()
        except:
            self.q_dead.put(device_num)
            sys.exit()

        try:
            dev = drv.Device(device_num)
        except:
            self.q_dead.put(device_num)
            sys.exit()

        #print device_num
        #print drv.Device.count()
        if drv.Device.count() - 1 < device_num:
            self.q_dead.put(device_num)
            sys.exit()

        ctx = dev.make_context()
        print dev.name()




        seed = int(time.time()*1000*device_num)


        # source code
        local_gpu_setup_kernel = pycuda.compiler.SourceModule(cuda_full_observables_production.cuda_full_observables_production_code, no_extern_c=True).get_function('setup_kernel')

        local_rng_states = drv.mem_alloc(np.int32(self.block_dim*self.grid_dim)*pycuda.characterize.sizeof('curandStateXORWOW', '#include <curand_kernel.h>'))
        local_gpu_setup_kernel(np.int32(int(self.block_dim*self.grid_dim)), local_rng_states, np.uint64(seed), np.uint64(0), grid=(int(self.grid_dim), 1), block=(int(self.block_dim), 1, 1))

        print '\nSetup Kernel run!\n'
        sys.stdout.flush()

        gpu_observables_func = SourceModule(cuda_full_observables_production.cuda_full_observables_production_code, no_extern_c=True).get_function(self.function_name)

        print 'Putting energy array on GPU...'
        d_energy_arrays = self.d_gpu_single_copy_arrays['energy']
        d_gpu_energy_arrays = {}
        for degree_setting in d_energy_arrays:
            d_gpu_energy_arrays[degree_setting] = pycuda.gpuarray.to_gpu(d_energy_arrays[degree_setting]['a_energy'])


        print 'Putting bkg energy array on GPU...'
        gpu_energy_bkg = pycuda.gpuarray.to_gpu(self.d_gpu_single_copy_arrays['energy_bkg'])
        
        d_gpu_bin_edges = {}
        d_gpu_bin_edges['s1'] = {}
        d_gpu_bin_edges['log'] = {}
        
        #d_bin_edges
        
        for cathode_setting in self.d_gpu_single_copy_arrays['d_bin_edges']['s1']:
            d_gpu_bin_edges['s1'][cathode_setting] = {}
            d_gpu_bin_edges['log'][cathode_setting] = {}
            for degree_setting in self.d_gpu_single_copy_arrays['d_bin_edges']['s1'][cathode_setting]:
                d_gpu_bin_edges['s1'][cathode_setting][degree_setting] = pycuda.gpuarray.to_gpu(self.d_gpu_single_copy_arrays['d_bin_edges']['s1'][cathode_setting][degree_setting])
                d_gpu_bin_edges['log'][cathode_setting][degree_setting] = pycuda.gpuarray.to_gpu(self.d_gpu_single_copy_arrays['d_bin_edges']['log'][cathode_setting][degree_setting])
        

        d_gpu_local_info = {'function_to_call':gpu_observables_func,
                            'rng_states':local_rng_states,
                            'd_gpu_energy':d_gpu_energy_arrays,
                            'gpu_energy_bkg':gpu_energy_bkg,
                            'd_gpu_bin_edges':d_gpu_bin_edges
                            }

        # wrap up function
        # modeled off of pycuda's autoinit

        def _finish_up(ctx):
            print '\n\nWrapping up thread %d...\n\n' % (device_num)
            sys.stdout.flush()
            ctx.pop()

            from pycuda.tools import clear_context_caches
            clear_context_caches()

            # put something in output queue to satisfy
            # parent's map
            self.q_out.put([0, device_num])
            sys.exit()

        #atexit.register(_finish_up, [ctx])
        #atexit.register(ctx.pop)



        while self.alive:
            if not self.q_in.empty():
                task, args, id_num = self.q_in.get()

                #print '\nTask ID: %d\n' % id_num
                #print args
                sys.stdout.flush()

                if task == 'exit':
                    _finish_up(ctx)
                else:
                    #print len(args), self.num_dim_gpu_call
                    if True:# or len(args) == self.num_dim_gpu_call or len(args) == 21:
                        args = np.append(args, [d_gpu_local_info])
                    #print args
                    return_value = task(args)

                #print '\nFinished Task ID: %d\n' % id_num
                #print id_num, return_value

                self.q_out.put((id_num, return_value))
            else:
                time.sleep(self.dispatcher_dead_time)


    def map(self, func, l_args):
        start_time = time.time()

        #print '\n\nMap called\n\n'
        sys.stdout.flush()

        len_input = len(l_args)

        for id_num, args in enumerate(l_args):
            self.q_in.put((func, args, id_num))

        #while not self.q_in.empty():
        while len(self.q_out.queue) != len_input:
            time.sleep(0.1)

        #print 'Time calling function: %.3e' % (time.time() - start_time)
        sys.stdout.flush()

        l_q = list(self.q_out.queue)
        self.q_out.queue.clear()
        #print len(l_q)
        l_q = sorted(l_q, key=lambda x: x[0])
        a_return_values = np.asarray([s_pair[1] for s_pair in l_q])
        #print a_return_values
        return a_return_values


    def close(self):
        self.map('exit', [[0] for i in xrange(self.num_gpus - len(self.q_dead.queue))])
        print 'Closed children'
        time.sleep(2)



import matplotlib.pyplot as plt
import matplotlib.mlab as mlab
import rootpy.compiled as C
from numpy.random import normal, binomial, seed, poisson
from rootpy.plotting import root2matplotlib as rplt

path_to_this_module = neriX_simulation_config.path_to_this_module
C.register_file('../../../python_modules/mc_code/c_log_likelihood.C', ['smart_log_likelihood'])
c_log_likelihood = C.smart_log_likelihood

#import gc
#gc.disable()

"""
import warnings
#warnings.filterwarnings('error')
# this turns runtime warnings into errors to
# catch unexpected behavior
warnings.simplefilter('error', RuntimeWarning)
# this turns off a deprecation warning in general but aimed
# at corners plot creation (should try to only put there)
# and move back to all warnings as errors)
warnings.simplefilter('ignore', DeprecationWarning)
warnings.simplefilter('ignore', FutureWarning)
"""

# MUST INCLUDE TYPES TO AVOID SEG FAULT
stl.vector(stl.vector('float'))
stl.vector(stl.vector('double'))
stl.vector(stl.vector('int'))


# create dot product in c++ for speed
# and thread safety

C.register_file('../../../python_modules/mc_code/c_safe_dot.C', ['safe_dot'])



def reduce_method(m):
    return (getattr, (m.__self__, m.__func__.__name__))



class fit_nr(object):
    def __init__(self, fit_type, d_coincidence_data, d_band_data=None, free_exciton_to_ion_ratio=False, num_mc_events=5e6, name_notes=None, l_gpus=[0], num_loops=1, test_switch='', small_number=0.001):
        
        # three modes:
        # 1. Single Energy: 's'
        #    - for fitting a single light and charge yield
        #      that is assumed to be constant across all energies
        #      in the Geant4 spectrum
        # 2. Multiple Energies: 'm'
        #    - for fitting multiple yields using a linear spline
        #      to interpolate the light and charge yield
        # 3. Multple Energies + Band: 'mb'
        #    - same as above but fit NR band at the same time
        # 4. Multiple Energies with Lindhard model: 'ml'
        #    - for fitting multiple yields using the
        #       Lindhard model to reduce number of free pars
        # 5. Band with Lindhard model and Gompertz Recombination: 'ml'
        #    - for fitting the band using the
        #       Lindhard model to reduce number of free pars
        # 6. Multiple Energies with Lindhard model and Thomas-Imel Recombination: 'mlti'
        #    - for fitting multiple yields using the
        #       Lindhard model to reduce number of free pars

        # d_coincidence_data should be in the following form:
        # d_coincidence_data['degree_settings'] = [2300, 3000, ...]
        # d_coincidence_data['cathode_settings'] = [0.345, 1.054, ...]
        # d_coincidence_data['cathode_settings'] = [4.5]
        # same convention for band data: deg=-4
        
        # total MC events = num_mc_events*num_loops
        
        self.num_loops = num_loops
        self.fit_type = fit_type
        self.small_number = small_number
        
        # test switch is used to have multiple test versions
        # available to run accross multiple GPUs (since minimizer
        # cannot multithread)
        self.test_switch = test_switch
        
        if fit_type == 's':
            assert len(d_coincidence_data['degree_settings']) == 1, ('If looking at a single energy can only give one degree setting in d_coincidence_data.')
        elif fit_type == 'm':
            assert len(d_coincidence_data['degree_settings']) >= 1
        elif fit_type == 'ml':
            assert len(d_coincidence_data['degree_settings']) >= 1
        elif fit_type == 'mlti':
            assert len(d_coincidence_data['degree_settings']) >= 1
        elif fit_type == 'mlti_moved_pos':
            assert len(d_coincidence_data['degree_settings']) >= 1
        elif fit_type == 'bl':
            assert len(d_coincidence_data['degree_settings']) >= 1
        elif fit_type == 'test':
            assert len(d_coincidence_data['degree_settings']) >= 1
        elif fit_type == 'mb':
            print 'Multiple energies with NR band not setup yet!\n'
            sys.exit()
        else:
            print '\nDo not recognize fit type: %s.\n' % (fit_type)
            sys.exit()

        copy_reg.pickle(types.MethodType, reduce_method)


        # set number of mc events
        self.d_gpu_scale = {}
        block_dim = 1024
        self.d_gpu_scale['block'] = (block_dim,1,1)
        numBlocks = floor(num_mc_events / float(block_dim))
        self.d_gpu_scale['grid'] = (int(numBlocks), 1)
        self.num_mc_events = int(numBlocks*block_dim)

        #assert len(d_coincidence_data['cathode_settings']) == 1, ('Only setup to handle one cathode setting at a time currently')

        # ------------------------------------------------
        # ------------------------------------------------
        # Set paths and allowed values
        # ------------------------------------------------
        # ------------------------------------------------


        l_allowed_degree_settings = [2300, 3000, 3500, 4500, 5300, 6200, -4]
        l_allowed_cathode_settings = [0.345, 1.054, 2.356]
        l_allowed_anode_settings = [4.5]

        self.d_cathode_voltages_to_field = {0.345:190,
                                       1.054:490,
                                       2.356:1020} # in kV:V/cm
        self.d_degree_setting_to_energy_name = {2300:3,
                                           3000:5,
                                           3500:7,
                                           4500:10,
                                           5300:15,
                                           6200:20} # colloquial names of each
        self.d_degree_setting_to_mean_energy = {2300:2.96,
                                           3000:4.93,
                                           3500:6.61,
                                           4500:9.76,
                                           5300:13.88,
                                           6200:17.50} # actual energies that degree corresponds to

        self.name_notes = name_notes
        self.path_to_coincidence_data = '%s/../coincidence_analysis/results/' % (neriX_simulation_config.path_to_this_module)
        self.path_to_energy_spectra = '%s/simulated_data/' % (neriX_simulation_config.path_to_this_module)
        self.path_to_reduced_energy_spectra = '%s/reduced_simulation_data/' % (neriX_simulation_config.path_to_this_module)

        self.l_energy_settings = neriX_simulation_config.l_energy_settings



        # ------------------------------------------------
        # ------------------------------------------------
        # Get information for data
        # ------------------------------------------------
        # ------------------------------------------------


        self.d_coincidence_data_information = {}

        # get all degrees and cathode settings and sort
        self.l_degree_settings_in_use = sorted(d_coincidence_data['degree_settings'])
        self.l_cathode_settings_in_use = sorted(d_coincidence_data['cathode_settings'])
        
        
        self.d_bin_edges = {}
        self.d_bin_edges['s1'] = {}
        self.d_bin_edges['log'] = {}
        
        for cathode_setting in self.l_cathode_settings_in_use:
            self.d_coincidence_data_information[cathode_setting] = {}
            self.d_bin_edges['s1'][cathode_setting] = {}
            self.d_bin_edges['log'][cathode_setting] = {}
            
            for degree_setting in self.l_degree_settings_in_use:
                self.d_coincidence_data_information[cathode_setting][degree_setting] = {}
                

                # add basic information to dictionary
                self.d_coincidence_data_information[cathode_setting][degree_setting]['d_s1_s2'] = pd.DataFrame(pickle.load(open('%s%.3fkV_%ddeg/s1_s2.pkl' % (self.path_to_coincidence_data, cathode_setting, degree_setting), 'r')))
                
                current_s1 = self.d_coincidence_data_information[cathode_setting][degree_setting]['d_s1_s2']['s1']
                current_log = np.log10(self.d_coincidence_data_information[cathode_setting][degree_setting]['d_s1_s2']['s2']/self.d_coincidence_data_information[cathode_setting][degree_setting]['d_s1_s2']['s1'])
                
                
                l_s1_binning = neriX_simulation_config.d_binning['s1'][cathode_setting][degree_setting]
                l_logs2s1_binning = neriX_simulation_config.d_binning['logs2s1'][cathode_setting][degree_setting]
                
                if l_s1_binning[0] == 'lin':
                    self.d_coincidence_data_information[cathode_setting][degree_setting]['bin_edges_s1'] = np.linspace(l_s1_binning[2], l_s1_binning[3], l_s1_binning[1]+1)
                else:
                    self.d_coincidence_data_information[cathode_setting][degree_setting]['bin_edges_s1'] = np.logspace(l_s1_binning[2], l_s1_binning[3], l_s1_binning[1]+1)
                
                if l_logs2s1_binning[0] == 'lin':
                    self.d_coincidence_data_information[cathode_setting][degree_setting]['bin_edges_log'] = np.linspace(l_logs2s1_binning[2], l_logs2s1_binning[3], l_logs2s1_binning[1]+1)
                else:
                    self.d_coincidence_data_information[cathode_setting][degree_setting]['bin_edges_log'] = np.logspace(l_logs2s1_binning[2], l_logs2s1_binning[3], l_logs2s1_binning[1]+1)
                
                
                self.d_bin_edges['s1'][cathode_setting][degree_setting] = np.asarray(self.d_coincidence_data_information[cathode_setting][degree_setting]['bin_edges_s1'], dtype=np.float32)
                self.d_bin_edges['log'][cathode_setting][degree_setting] = np.asarray(self.d_coincidence_data_information[cathode_setting][degree_setting]['bin_edges_log'], dtype=np.float32)
                
                # make cut on ER noise
                neriX_analysis.warning_message('Hard coded values in fit_nr, CUDA code, and compare_data_mc code for ER cut - must be certain that they are the same!')
                er_cut_lb = 40.
                if cathode_setting == 0.345:
                    intercept = 2.229
                    slope = -3.838e-3
                elif cathode_setting == 1.054:
                    intercept = 2.237
                    slope = -4.109e-3
                elif cathode_setting == 2.356:
                    intercept = 2.196
                    slope = -3.596e-3
                
                
                self.d_coincidence_data_information[cathode_setting][degree_setting]['d_s1_s2'] = self.d_coincidence_data_information[cathode_setting][degree_setting]['d_s1_s2'][(current_s1 < er_cut_lb) | ((current_log < (intercept + slope*current_s1)) & (current_s1 > er_cut_lb))]
                
                
                
                
                
                # histogram that will be used for matching
                self.d_coincidence_data_information[cathode_setting][degree_setting]['a_log_s2_s1'], _, _ = np.histogram2d(self.d_coincidence_data_information[cathode_setting][degree_setting]['d_s1_s2']['s1'], np.log10(self.d_coincidence_data_information[cathode_setting][degree_setting]['d_s1_s2']['s2']/self.d_coincidence_data_information[cathode_setting][degree_setting]['d_s1_s2']['s1']), bins=[self.d_coincidence_data_information[cathode_setting][degree_setting]['bin_edges_s1'], self.d_coincidence_data_information[cathode_setting][degree_setting]['bin_edges_log']])
                self.d_coincidence_data_information[cathode_setting][degree_setting]['num_data_pts'] = np.sum(self.d_coincidence_data_information[cathode_setting][degree_setting]['a_log_s2_s1'])
                
                
                


        #print self.l_s1_settings, self.l_s2_settings, self.l_log_settings

        # ------------------------------------------------
        # ------------------------------------------------
        # Check that given settings are appropriate
        # ------------------------------------------------
        # ------------------------------------------------


        for cathode_setting in self.l_cathode_settings_in_use:
            assert cathode_setting in l_allowed_cathode_settings, '%.3f kV is not an allowed cathode setting: %s' % (cathode_setting, str(self.l_allowed_cathode_settings))

        for degree_setting in self.l_degree_settings_in_use:
            assert degree_setting in l_allowed_degree_settings, '%d deg is not an allowed degree setting: %s' % (degree_setting, str(self.l_allowed_degree_settings))



        # set whether or not we can fit electron to ion ratio
        """
        if len(self.l_cathode_settings_in_use) == 1 and not free_exciton_to_ion_ratio:
            print '\nCannot fit exciton to ion ratio with only one field - setting as a nuissance parameter.\n'
            self.free_exciton_to_ion_ratio = False
        else:
            print '\nCurrently not setup to handle free exciton to ion ratio so setting to nuissance parameter.\n'
            self.free_exciton_to_ion_ratio = False
        """



        # ------------------------------------------------
        # ------------------------------------------------
        # Load MC files or create reduced file if not present
        # and load into easy_graph
        # ------------------------------------------------
        # ------------------------------------------------

        self.load_mc_data('MC')

        # fill energy array
        self.fill_energy_array(view_energy_spectrum=False)


        #print self.d_coincidence_data_information[self.l_cathode_settings_in_use[0]][self.l_degree_settings_in_use[0]]



        # ------------------------------------------------
        # ------------------------------------------------
        # Set default parameter values for use in MC
        # ------------------------------------------------
        # ------------------------------------------------


        neriX_analysis.warning_message('Detector parameters hard-coded')

        self.g1_value = 0.125#0.117
        self.g1_uncertainty = 0.003#0.002
        
        self.w_value = 13.7
        self.w_value_uncertainty = 0.1#0.08
        
        self.extraction_efficiency_value = 0.892#0.895
        self.extraction_efficiency_uncertainty = 0.007#0.002

        self.gas_gain_value = 23.1#21.2
        self.gas_gain_uncertainty = 0.2#0.04

        self.gas_gain_width = 8.62#8.01
        self.gas_gain_width_uncertainty = 0.31#0.29

        self.spe_res_value = 0.644#0.59
        self.spe_res_uncertainty = 0.006#0.0056

        self.l_means_pf_stdev_pars = [0.014675238121911261, 0.5283368131669588, 6.9731568575515102]
        self.l_cov_matrix_pf_stdev_pars = [[9.7679768145661727e-10, 2.163163938286531e-08, -3.8945450018151318e-07], [2.163163938286531e-08, 4.8569294796130197e-06, -2.9829801702396074e-05], [-3.8945450018151318e-07, -2.9829801702396074e-05, 0.000347538069496915]]

        self.l_means_pf_eff_pars = [3.16064649,  0.75476424] #[1.96178522,  0.46718076]
        self.l_cov_matrix_pf_eff_pars = [[1.98043618e-04, 5.02423109e-06], [5.02423109e-06, 7.88784819e-05]]

        self.l_means_s2_eff_pars = [114.36224212, 381.80770505]
        self.l_cov_matrix_s2_eff_pars = [[799.28476818, -659.93322804], [-659.93322804, 1133.62883639]]

        # below are the NEST values for the Lindhard model
        self.nest_lindhard_model = {}
        self.nest_lindhard_model['values'] = {'alpha':1.240,
                                                'zeta':0.0472,
                                                'beta':239,
                                                'gamma':0.01385,
                                                'delta':0.0620,
                                                'kappa':0.1394,
                                                'eta':3.3,
                                                'lambda':1.14}

        # below are the spreads for walker initialization only
        # these are NOT uncertainties
        self.nest_lindhard_model['spreads'] = {'alpha':0.008,
                                                'zeta':0.008,
                                                'beta':12,
                                                'gamma':0.00065,
                                                'delta':0.006,
                                                'kappa':0.003,
                                                'eta':2,
                                                'lambda':0.25}



        if fit_type == 's':
            self.ln_likelihood_function = self.ln_likelihood_full_matching_single_energy
            self.ln_likelihood_function_wrapper = self.wrapper_ln_likelihood_full_matching_single_energy
            self.num_dimensions = 20
            self.gpu_function_name = 'gpu_full_observables_production_with_log_hist_single_energy_with_bkg'
            self.directory_name = 'single_energy'

        else:
            
            if fit_type == 'm':
                self.ln_likelihood_function = self.ln_likelihood_full_matching_multiple_energies
                self.ln_likelihood_function_wrapper = self.wrapper_ln_likelihood_full_matching_multiple_energies
                self.num_dimensions = 19
                self.directory_name = 'multiple_energies'
                self.gpu_function_name = 'gpu_full_observables_production_with_log_hist_spline'
            if fit_type == 'ml':
                self.ln_likelihood_function = self.ln_likelihood_full_matching_multiple_energies_lindhard_model
                self.ln_likelihood_function_wrapper = self.wrapper_ln_likelihood_full_matching_multiple_energies_lindhard_model
                self.num_dimensions = 17 + len(self.l_cathode_settings_in_use)*len(self.l_degree_settings_in_use)*2 + len(self.l_cathode_settings_in_use)*(1+4) # 17 + scales + bkg + recombination + exciton-to-ion ratio
                self.directory_name = 'multiple_energies_lindhard_model'
                self.gpu_function_name = 'gpu_full_observables_production_with_hist_gompertz_wth_bkg'
                


            elif fit_type == 'mlti':
                self.ln_likelihood_function = self.ln_likelihood_full_matching_multiple_energies_lindhard_model_with_ti_recombination
                self.ln_likelihood_function_wrapper = self.wrapper_ln_likelihood_full_matching_multiple_energies_lindhard_model_with_ti_recombination
                self.num_dimensions = 21 + len(self.l_cathode_settings_in_use)*len(self.l_degree_settings_in_use)*2 + len(self.l_cathode_settings_in_use)*(1+1) - len(self.l_cathode_settings_in_use) # 17 + scales + bkg + recombination + exciton-to-ion ratio
                self.directory_name = 'multiple_energies_lindhard_model_with_ti'
                self.gpu_function_name = 'gpu_full_observables_production_with_hist_ti_wth_bkg'


            elif fit_type == 'mlti_moved_pos':
                self.ln_likelihood_function = self.ln_likelihood_full_matching_multiple_energies_lindhard_model_with_ti_recombination
                self.ln_likelihood_function_wrapper = self.wrapper_ln_likelihood_full_matching_multiple_energies_lindhard_model_with_ti_recombination
                self.num_dimensions = 21 + len(self.l_cathode_settings_in_use)*len(self.l_degree_settings_in_use)*2 + len(self.l_cathode_settings_in_use)*(1+1) - len(self.l_cathode_settings_in_use) # 17 + scales + bkg + recombination + exciton-to-ion ratio
                self.directory_name = 'multiple_energies_lindhard_model_with_ti_moved_pos'
                self.gpu_function_name = 'gpu_full_observables_production_with_hist_ti_wth_bkg'

            
            elif fit_type == 'mb':
                self.ln_likelihood_function = self.ln_likelihood_full_matching_single_energy_and_band
                self.ln_likelihood_function_wrapper = self.wrapper_ln_likelihood_full_matching_single_energy_and_band
                self.num_dimensions = 19
                self.directory_name = 'multiple_energies_with_band'
                self.gpu_function_name = 'gpu_full_observables_production_with_log_hist_spline'
            
            elif fit_type == 'bl':
                self.ln_likelihood_function = self.ln_likelihood_full_matching_band
                self.ln_likelihood_function_wrapper = self.wrapper_ln_likelihood_full_matching_band
                self.num_dimensions = 17 + len(self.l_cathode_settings_in_use)*(1+4+1) # 17 + scales + recombination + exciton-to-ion ratio
                self.directory_name = 'band_with_lindhard_model'
                self.gpu_function_name = 'gpu_full_observables_production_with_hist_gompertz'

            elif fit_type == 'test':
                self.ln_likelihood_function = self.ln_likelihood_test
                self.ln_likelihood_function_wrapper = self.wrapper_ln_likelihood_test
                self.num_dimensions = 20 + len(self.l_cathode_settings_in_use)*len(self.l_degree_settings_in_use)*2 + len(self.l_cathode_settings_in_use)*(1+1) - len(self.l_cathode_settings_in_use)
                self.directory_name = 'likelihood_ratio_test'
                if self.test_switch == 'ti':
                    self.gpu_function_name = 'gpu_full_observables_production_test_ti'
                elif self.test_switch == 'pol1':
                    self.gpu_function_name = 'gpu_full_observables_production_test_pol1'
                elif self.test_switch == 'pol2':
                    self.gpu_function_name = 'gpu_full_observables_production_test_pol2'
                elif self.test_switch == 'pol3':
                    self.gpu_function_name = 'gpu_full_observables_production_test_pol3'
                elif self.test_switch == 'pol4':
                    self.gpu_function_name = 'gpu_full_observables_production_test_pol4'
                elif self.test_switch == 'pol5':
                    self.gpu_function_name = 'gpu_full_observables_production_test_pol5'
                elif self.test_switch == 'mod_gom':
                    self.gpu_function_name = 'gpu_full_observables_production_test_mod_gom'

            
            
            # -----------------------------------------------
            # -----------------------------------------------
            # set spline points in energy
            # -----------------------------------------------
            # -----------------------------------------------
            
            """
            # will fill spline points dynamically since depends on
            # energies given
            l_spline_energies = [0.5]
            for degree_setting in self.l_degree_settings_in_use:
                l_spline_energies.append(self.d_degree_setting_to_mean_energy[degree_setting])
            l_spline_energies.append(35.)
            
            self.a_spline_energies = np.asarray(l_spline_energies, dtype=np.float32)

            #self.num_dimensions += 2*len(self.a_spline_energies)
            
            
            print '\n\nEnergy Spline Points:\n'
            pprint(list(self.a_spline_energies))
            print '\n\n'
            
        
            self.a_nest_photon_yields = np.zeros(len(self.a_spline_energies), dtype=np.float32)
            self.a_nest_charge_yields = np.zeros(len(self.a_spline_energies), dtype=np.float32)
            for i in xrange(len(self.a_spline_energies)):
                l_yields = nest_nr_mean_yields(self.a_spline_energies[i], self.d_cathode_voltages_to_field[cathode_setting])
                self.a_nest_photon_yields[i] = l_yields[0]
                self.a_nest_charge_yields[i] = l_yields[1]
            """


        d_gpu_single_copy_array_dictionaries = {'energy':self.d_energy_arrays,
                                                'energy_bkg':self.a_energy_bkg,
                                                'd_bin_edges':self.d_bin_edges,
                                                }
        self.gpu_pool = gpu_pool(l_gpus=l_gpus, grid_dim=numBlocks, block_dim=block_dim, num_dim_gpu_call=self.num_dimensions, d_gpu_single_copy_arrays=d_gpu_single_copy_array_dictionaries, function_name=self.gpu_function_name)

        self.b_suppress_likelihood = False


    def close_workers(self):
        self.gpu_pool.close()



    def read_g1_from_file(self, g1_root_file):
        return (0, 0)



    def read_g2_from_file(self, g2_root_file):
        return (0, 0)



    def read_gas_gain_from_file(self, gas_gain_root_file):
        return (0, 0)



    def fill_energy_array(self, view_energy_spectrum=False):

        random.seed()
    
        for degree_setting in self.l_degree_settings_in_use:
    
            self.d_energy_arrays[degree_setting]['a_energy'] = np.zeros(self.num_mc_events, dtype=np.float32)
            
            for i in tqdm.tqdm(xrange(self.num_mc_events)):
                random_energy = self.d_energy_arrays[degree_setting]['h_energy'].GetRandom()
            
                # special condition for band
                if degree_setting == -4:
                    while random_energy < (self.d_bin_edges['s1'][0.345][-4][0]*0.025/0.11/7.):
                        random_energy = self.d_energy_arrays[degree_setting]['h_energy'].GetRandom()
                
            
                self.d_energy_arrays[degree_setting]['a_energy'][i] = random_energy
                    
            if view_energy_spectrum:
                cEnergySpec = Canvas()
                hEnergySpec = Hist(*self.l_energy_settings)
                hEnergySpec.fill_array(self.d_energy_arrays[degree_setting]['a_energy'])
                hEnergySpec.Draw()
                cEnergySpec.Update()
                raw_input('Press enter to continue...')
                
                del hEnergySpec
                del cEnergySpec

        # fill bkg energy array
        # same for all degrees
        self.a_energy_bkg = np.zeros(self.num_mc_events, dtype=np.float32)
        for i in tqdm.tqdm(xrange(self.num_mc_events)):
            self.a_energy_bkg[i] = self.h_energy_bkg.GetRandom()





    def load_mc_data(self, sFileType):

        self.d_energy_arrays = {}

        for degree_setting in self.l_degree_settings_in_use:
            self.d_energy_arrays[degree_setting] = {}

            # check if reduced file exists
            coincidence_degree_setting = True
            if degree_setting > 0:
                if not self.identifier == 'mlti_moved_pos':
                    current_path_to_reduced_energy_spectra = '%snerixsim-%dkeV.root' % (self.path_to_reduced_energy_spectra, self.d_degree_setting_to_energy_name[degree_setting])
                else:
                    current_path_to_reduced_energy_spectra = '%snerixsim-%dkeV_moved_pos.root' % (self.path_to_reduced_energy_spectra, self.d_degree_setting_to_energy_name[degree_setting])
            else:
                current_path_to_reduced_energy_spectra = '%snerixsim-pure_nr_spec.root' % (self.path_to_reduced_energy_spectra)
                coincidence_degree_setting = False
            
            
            
            if not os.path.exists(current_path_to_reduced_energy_spectra):
            
                # if the file doesn't exist we need to create it!
            
                # load the raw data file
                if coincidence_degree_setting:
                    if not self.identifier == 'mlti_moved_pos':
                        current_path_to_energy_spectra = '%snerixsim-%dkeV.root' % (self.path_to_energy_spectra, self.d_degree_setting_to_energy_name[degree_setting])
                    else:
                        current_path_to_energy_spectra = '%snerixsim-%dkeV_moved_pos.root' % (self.path_to_energy_spectra, self.d_degree_setting_to_energy_name[degree_setting])
                else:
                    current_path_to_energy_spectra = '%snerixsim-pure_nr_spec.root' % (self.path_to_energy_spectra)
                
                f_simulation = File(current_path_to_energy_spectra, 'read')
                

                if sFileType == 'MC' and coincidence_degree_setting:

                    # set cuts
                    #xRadius = '(sqrt(xpos[0]**2+ypos[0]**2) < 18)'
                    xRadius = '(sqrt(xpos[0]**2+ypos[0]**2) < 21.5*0.85)'
                    #xZ = '(zpos[0]>-20 && zpos[0]<-4)'
                    xZ = '(zpos[0]>-24.3 && zpos[0]<-1.0)'
                    xSingleScatter = '(nsteps_target==1)'
                    xLiqSciHeight = '(etotliqsci>700)'
                    xLXeEnergy = '(etot_target>0)'
                    #xLXeEnergy = '(etot_target>0)'
                    xLXeEnergy = '(Alt$(ed_nr_target[1],-1)==-1 && ed_nr_target[0] > 0)'

                    #print 'No TOF cut!\n'
                    #neriX_analysis.warning_message('Need to finalize how tof is chosen in MC data!')
                    
                    low_time_tof = -1
                    high_time_tof = -1
                    if degree_setting == 2300:
                        low_time_tof = 24
                        high_time_tof = 34
                    elif degree_setting == 3000:
                        low_time_tof = 31
                        high_time_tof = 41
                    elif degree_setting == 3500:
                        low_time_tof = 15
                        high_time_tof = 25
                    elif degree_setting == 4500:
                        low_time_tof = 22
                        high_time_tof = 34
                    elif degree_setting == 5300:
                        low_time_tof = 14
                        high_time_tof = 24
                    elif degree_setting == 6200:
                        low_time_tof = 8
                        high_time_tof = 25
                    
                    
                    #xTOF = '(timeliqsci-tpos[0]> %d && timeliqsci-tpos[0] < %d)' % (low_time_tof, high_time_tof)
                    xTOF = 'etotliqsci > 1.'

                    xAll = '%s && %s && %s && %s && %s && %s' % (xRadius, xZ, xSingleScatter, xLiqSciHeight, xLXeEnergy, xTOF)
                
                elif sFileType == 'MC' and not coincidence_degree_setting:
                    # set cuts
                    #xRadius = '(sqrt(xpos[0]**2+ypos[0]**2) < 18)'
                    xRadius = '(sqrt(xpos[0]**2+ypos[0]**2) < 21.5*0.85)'
                    #xZ = '(zpos[0]>-20 && zpos[0]<-4)'
                    xZ = '(zpos[0]>-24.3 && zpos[0]<-1.0)'
                    xSingleScatter = '(nsteps_target==1)'
                    #xLXeEnergy = '(etot_target>0)' # this includes ER
                    xSingleScatter_2 = '(Alt$(ed_nr_target[1],-1)==-1)'
                    xLXeEnergy = '(ed_nr_target[0] > 0.)'

                    xAll = '%s && %s && %s && %s && %s' % (xRadius, xZ, xSingleScatter, xLXeEnergy, xSingleScatter_2)


                # create and fill histogram
                t_mc = f_simulation.t2
                h_mc = Hist(*self.l_energy_settings, name='h_mc', title='Simulated Spectrum', drawstyle='hist')
                #t_mc.Draw('etot_target', hist=hMC, selection=xAll)
                t_mc.Draw('ed_nr_target[0]', hist=h_mc, selection=xAll)
                h_mc.Sumw2()

                # draw histogram to new file for quick retrieval
                f_reduced = File(current_path_to_reduced_energy_spectra, 'recreate')
                h_mc.Write()
                f_reduced.Close()
            
                neriX_analysis.success_message('Created new reduced MC file!')
            
            

            self.f_reduced = File(current_path_to_reduced_energy_spectra, 'read')
            h_mc = self.f_reduced.h_mc
            #g_mc = neriX_analysis.convert_hist_to_graph_with_poisson_errors(h_mc)
            
            # now get accidental bkg
            current_path_to_bkg_spectra = '%snerixsim-pure_nr_spec.root' % (self.path_to_reduced_energy_spectra)
            self.f_reduced_bkg = File(current_path_to_bkg_spectra, 'read')
            h_mc_bkg = self.f_reduced_bkg.h_mc

            self.d_energy_arrays[degree_setting]['h_energy'] = h_mc
            self.h_energy_bkg = h_mc_bkg



    def get_g1_default(self, g1_value):
        return norm.pdf(g1_value, self.g1_value, self.g1_uncertainty), g1_value
    
    
    def get_extraction_efficiency_default(self, extraction_efficiency_value):
        return norm.pdf(extraction_efficiency_value, self.extraction_efficiency_value, self.extraction_efficiency_uncertainty), extraction_efficiency_value


    # get likelihood and gas gain given random variable (nuissance parameter)
    # gasGainRV should be normally distributed
    def get_gas_gain_default(self, gas_gain_mean):
        return norm.pdf(gas_gain_mean, self.gas_gain_value, self.gas_gain_uncertainty), gas_gain_mean


    # get likelihood and gas gain width given random variable (nuissance parameter)
    # gasGainWidthRV should be normally distributed
    def get_gas_gain_width_default(self, gas_gain_width_value):
        return norm.pdf(gas_gain_width_value, self.gas_gain_width, self.gas_gain_width_uncertainty), gas_gain_width_value


    # get likelihood and spe res width given random variable (nuissance parameter)
    # gasGainWidthRV should be normally distributed
    def get_spe_res_default(self, spe_res):
        return norm.pdf(spe_res, self.spe_res_value, self.spe_res_uncertainty), spe_res


    def get_pf_eff_default(self, pf_eff_par0, pf_eff_par1):
        return multivariate_normal.pdf([pf_eff_par0, pf_eff_par1], self.l_means_pf_eff_pars, self.l_cov_matrix_pf_eff_pars), pf_eff_par0, pf_eff_par1



    def get_pf_stdev_default(self, pf_stdev_par0, pf_stdev_par1, pf_stdev_par2):
        return multivariate_normal.pdf([pf_stdev_par0, pf_stdev_par1, pf_stdev_par2], self.l_means_pf_stdev_pars, self.l_cov_matrix_pf_stdev_pars), pf_stdev_par0, pf_stdev_par1, pf_stdev_par2



    def get_s2_eff_default(self, s2_eff_par0, s2_eff_par1):
        return multivariate_normal.pdf([s2_eff_par0, s2_eff_par1], self.l_means_s2_eff_pars, self.l_cov_matrix_s2_eff_pars), s2_eff_par0, s2_eff_par1



    def get_exciton_ion_ratio(self, lParsRV):
        likelihood = 1.
        lParValues = [0. for i in xrange(len(lParsRV))]
        for parNumber, parRV in enumerate(lParsRV):
            likelihood *= norm.pdf(parRV)
            if parNumber == 0:
                # alpha
                meanFromNEST = 1.240
                sigmaLowFromNEST = 0.073
                sigmaHighFromNEST = 0.079
            elif parNumber == 1:
                # zeta
                meanFromNEST = 0.0472
                sigmaLowFromNEST = 0.0073
                sigmaHighFromNEST = 0.0088
            elif parNumber == 2:
                # beta
                meanFromNEST = 239.0
                sigmaLowFromNEST = 8.8
                sigmaHighFromNEST = 28.0
            else:
                print 'FAILURE: Incorrect number of parameters given for exciton to ion ratio.'
                sys.exit()

            if parRV >= 0:
                valueForPar = meanFromNEST + parRV*sigmaHighFromNEST
            else:
                valueForPar = meanFromNEST - parRV*sigmaLowFromNEST

            lParValues[parNumber] = valueForPar

        return likelihood, lParValues[0]*(self.meanField**(-lParValues[1])) * ( 1 - exp(-lParValues[2] * 11.5*self.meanEnergy*54**(-7./3.)) )



    def get_likelihood_exciton_to_ion(self, par0, par1, par2):
        return norm.pdf(par0)*norm.pdf(par1)*norm.pdf(par2)
    
    
    
    # get likelihood and w-value given random variable (nuissance parameter)
    # w_value_rv should be normally distributed
    def get_w_value_default(self, w_value):
        return norm.pdf(w_value, self.w_value, self.w_value_uncertainty), w_value



    def get_prior_log_likelihood_nuissance(self, likelihoodNuissance):
        if likelihoodNuissance > 1e-550:
            return np.log(likelihoodNuissance)
        else:
            return -np.inf



    def get_prior_log_likelihood_dpe_prob(self, dpe_prob):
        if 0.17 < dpe_prob < 0.24:
            return 0
        else:
            return -np.inf



    def get_log_likelihood_s1_eff(self, l_s1_eff_pars):
        # first par is center, second is shape
        if -10 < l_s1_eff_pars[0] < 10 and 0.01 < l_s1_eff_pars[1] < 10:
            return 0.
        else:
            return -np.inf



    def get_log_likelihood_s2_eff(self, l_s2_eff_pars):
        # first par is center, second is shape
        if -100 < l_s2_eff_pars[0] < 1000 and 1 < l_s2_eff_pars[1] < 1000:
            return 0.
        else:
            return -np.inf



    def get_prior_log_likelihood_probabilities(self, scale):
        if isinstance(scale, dict):
            for id in scale:
                # check if 2d dict
                if isinstance(scale[id], dict):
                    for id_2 in scale[id]:
                        if scale[id][id_2] < 0. or scale[id][id_2] > 1.:
                            return -np.inf
                        else:
                            return 0.
            
                else:
                    if scale[id] < 0. or scale[id] > 1.:
                        return -np.inf
                    else:
                        return 0.
        
        else:
            if scale < 0. or scale > 1.:
                return -np.inf
            else:
                return 0



    def get_prior_log_likelihood_resolution(self, intrinsicResolution):
        if intrinsicResolution < 0 or intrinsicResolution > 1.5:
            return -np.inf
        else:
            return 0



    def get_prior_log_likelihood_scale_par(self, scale_par):
        if scale_par < 0:
            return -np.inf
        else:
            return 0



    def get_prior_log_likelihood_light_yields(self, l_yields):
        for value in l_yields:
            if value < 3. or value > 15.:
                return -np.inf
        return 0.



    def get_prior_log_likelihood_charge_yields(self, l_yields):
        for value in l_yields:
            if value < 3.5 or value > 15.:
                return -np.inf
        return 0.



    def get_prior_log_likelihood_gompertz_pars(self, l_pars):
        assert len(l_pars) == 4
        if not (0 < l_pars[0] < 1):
            return -np.inf
        if not (0 < l_pars[1] < 1):
            return -np.inf
        if not (0 < (l_pars[0] + l_pars[1]) < 1):
            return -np.inf
        if not (l_pars[2] > 0):
            return -np.inf
        if not (1e-7 < l_pars[3] < 1):
            return -np.inf

        # otherwise
        return 0.


    def get_prior_log_likelihood_ti_par(self, ti_par):
        if not (ti_par > 0):
            return -np.inf
        else:
            return 0.



    def get_prior_log_likelihood_exciton_to_ion_ratio(self, exciton_to_ion_ratio):
        if exciton_to_ion_ratio < 0:
            return -np.inf
        else:
            return 0.



    def get_prior_log_likelihood_probability(self, value):
        if not (0. <= value <= 1.):
            return -np.inf
        else:
            return 0.



    def get_prior_log_likelihood_greater_than_zero(self, value):
        if not (0. <= value):
            return -np.inf
        else:
            return 0.

    
    
    def get_prior_log_likelihood_alpha(self, alpha):
        if alpha < 0:
            return -np.inf
        else:
            return 0.



    def get_prior_log_likelihood_zeta(self, zeta):
        if zeta < 0:
            return -np.inf
        else:
            return 0.



    def get_prior_log_likelihood_beta(self, beta):
        # beta represents an exponential suppression
        # so above certain limit it has no effect (~5000)
        if beta < 0 or beta > 5000.:
            return -np.inf
        else:
            return 0.



    def get_prior_log_likelihood_gamma(self, gamma):
        if gamma < 0:
            return -np.inf
        else:
            return 0.



    def get_prior_log_likelihood_delta(self, delta):
        if delta < 0:
            return -np.inf
        else:
            return 0.



    def get_prior_log_likelihood_kappa(self, kappa):
        if kappa < 0:
            return -np.inf
        else:
            return 0.



    def get_prior_log_likelihood_eta(self, eta):
        if eta < 0:
            return -np.inf
        else:
            return 0.



    def get_prior_log_likelihood_lamb(self, lamb):
        if lamb < 0:
            return -np.inf
        else:
            return 0.



    def get_indices_for_given_quantile(self, aBinCenters, aCountsInBins, leftPercentile, rightPercentile):
        aX = aBinCenters.flatten()
        aY = aCountsInBins.flatten()

        totalCounts = int(np.rint(np.sum(aCountsInBins)))
        aForQuantiles = np.zeros(totalCounts)

        # build array for quantile analysis
        # essentially add another index for each count in array
        currentIndex = 0

        for i in xrange(len(aX)):
            for j in xrange(int(np.rint(aY[i]))):
                aForQuantiles[currentIndex] = aX[i]
                currentIndex += 1

        valueLeft = np.percentile(aForQuantiles, leftPercentile*100)
        valueRight = np.percentile(aForQuantiles, rightPercentile*100)

        indexLeft = np.argmin(np.abs(aBinCenters - valueLeft))
        indexRight = np.argmin(np.abs(aBinCenters - valueRight))

        # add 1 to right index to keep it inclusive
        return (indexLeft, indexRight + 1)





    def ln_likelihood_full_matching_single_energy(self, py, qy, intrinsic_res_s1, intrinsic_res_s2, g1_value, spe_res_value, extraction_efficiency_value, gas_gain_mean_value, gas_gain_width_value, pf_eff_par0, pf_eff_par1, s2_eff_par0, s2_eff_par1, pf_res_s1, pf_res_s2, exciton_to_ion_par0_rv, exciton_to_ion_par1_rv, exciton_to_ion_par2_rv, prob_bkg_event, scale_par, d_gpu_local_info, draw_fit=False):



        # -----------------------------------------------
        # -----------------------------------------------
        # determine prior likelihood and variables
        # -----------------------------------------------
        # -----------------------------------------------


        prior_ln_likelihood = 0


        # priors of yields
        # can do them all together since uniform within
        # the same range
        prior_ln_likelihood += self.get_prior_log_likelihood_light_yields([py])
        prior_ln_likelihood += self.get_prior_log_likelihood_charge_yields([qy])


        # priors of resolutions
        prior_ln_likelihood += self.get_prior_log_likelihood_resolution(intrinsic_res_s1)
        prior_ln_likelihood += self.get_prior_log_likelihood_resolution(pf_res_s1)
        prior_ln_likelihood += self.get_prior_log_likelihood_resolution(pf_res_s2)
        prior_ln_likelihood += self.get_prior_log_likelihood_resolution(intrinsic_res_s2)
        
        prior_ln_likelihood += self.get_prior_log_likelihood_greater_than_zero(scale_par)


        # get exciton to ion prior
        current_likelihood = self.get_likelihood_exciton_to_ion(exciton_to_ion_par0_rv, exciton_to_ion_par1_rv, exciton_to_ion_par2_rv)
        prior_ln_likelihood += self.get_prior_log_likelihood_nuissance(current_likelihood)


        # priors of detector variables
        current_likelihood, g1_value = self.get_g1_default(g1_value)
        prior_ln_likelihood += self.get_prior_log_likelihood_nuissance(current_likelihood)
        
        current_likelihood, extraction_efficiency = self.get_extraction_efficiency_default(extraction_efficiency_value)
        prior_ln_likelihood += self.get_prior_log_likelihood_nuissance(current_likelihood)

        current_likelihood, spe_res = self.get_spe_res_default(spe_res_value)
        prior_ln_likelihood += self.get_prior_log_likelihood_nuissance(current_likelihood)

        current_likelihood, gas_gain_value = self.get_gas_gain_default(gas_gain_mean_value)
        prior_ln_likelihood += self.get_prior_log_likelihood_nuissance(current_likelihood)

        current_likelihood, gas_gain_width = self.get_gas_gain_width_default(gas_gain_width_value)
        prior_ln_likelihood += self.get_prior_log_likelihood_nuissance(current_likelihood)


        # priors of efficiencies
        current_likelihood, pf_eff_par0, pf_eff_par1 = self.get_pf_eff_default(pf_eff_par0, pf_eff_par1)
        prior_ln_likelihood += self.get_prior_log_likelihood_nuissance(current_likelihood)

        current_likelihood, s2_eff_par0, s2_eff_par1 = self.get_s2_eff_default(s2_eff_par0, s2_eff_par1)
        prior_ln_likelihood += self.get_prior_log_likelihood_nuissance(current_likelihood)

        prior_ln_likelihood += self.get_prior_log_likelihood_probability(prob_bkg_event)


        # if prior is -inf then don't bother with MC
        #print 'removed prior infinity catch temporarily'
        if not np.isfinite(prior_ln_likelihood) and not draw_fit:
            return -np.inf



        # -----------------------------------------------
        # -----------------------------------------------
        # run MC
        # -----------------------------------------------
        # -----------------------------------------------
        

        num_trials = np.asarray(self.num_mc_events, dtype=np.int32)
        mean_field = np.asarray(self.d_cathode_voltages_to_field[self.l_cathode_settings_in_use[0]], dtype=np.float32)

        py = np.asarray(py, dtype=np.float32)
        qy = np.asarray(qy, dtype=np.float32)

        g1_value = np.asarray(g1_value, dtype=np.float32)
        extraction_efficiency = np.asarray(extraction_efficiency, dtype=np.float32)
        spe_res = np.asarray(spe_res, dtype=np.float32)
        gas_gain_value = np.asarray(gas_gain_value, dtype=np.float32)
        gas_gain_width = np.asarray(gas_gain_width, dtype=np.float32)

        intrinsic_res_s1 = np.asarray(intrinsic_res_s1, dtype=np.float32)
        intrinsic_res_s2 = np.asarray(intrinsic_res_s2, dtype=np.float32)

        exciton_to_ion_par0_rv = np.asarray(exciton_to_ion_par0_rv, dtype=np.float32)
        exciton_to_ion_par1_rv = np.asarray(exciton_to_ion_par1_rv, dtype=np.float32)
        exciton_to_ion_par2_rv = np.asarray(exciton_to_ion_par2_rv, dtype=np.float32)

        pf_eff_par0 = np.asarray(pf_eff_par0, dtype=np.float32)
        pf_eff_par1 = np.asarray(pf_eff_par1, dtype=np.float32)

        s2_eff_par0 = np.asarray(s2_eff_par0, dtype=np.float32)
        s2_eff_par1 = np.asarray(s2_eff_par1, dtype=np.float32)

        a_pf_res_s1 = np.asarray(pf_res_s1, dtype=np.float32)
        a_pf_res_s2 = np.asarray(pf_res_s2, dtype=np.float32)
        
        prob_bkg_event = np.asarray(prob_bkg_event, dtype=np.float32)
        
        cathode_setting = self.l_cathode_settings_in_use[0]
        degree_setting = self.l_degree_settings_in_use[0]

        # for histogram binning
        num_bins_s1 = np.asarray(len(d_gpu_local_info['d_gpu_bin_edges']['s1'][cathode_setting][degree_setting])-1, dtype=np.int32)
        num_bins_log = np.asarray(len(d_gpu_local_info['d_gpu_bin_edges']['log'][cathode_setting][degree_setting])-1, dtype=np.int32)
        
        a_hist_2d = np.zeros(num_bins_s1*num_bins_log, dtype=np.float32)
        
        num_loops = np.asarray(self.num_loops, dtype=np.int32)


        tArgs = (d_gpu_local_info['rng_states'], drv.In(num_trials), drv.In(mean_field), d_gpu_local_info['d_gpu_energy'][self.l_degree_settings_in_use[0]], d_gpu_local_info['gpu_energy_bkg'], drv.In(py), drv.In(qy), drv.In(g1_value), drv.In(extraction_efficiency), drv.In(gas_gain_value), drv.In(gas_gain_width), drv.In(spe_res), drv.In(intrinsic_res_s1), drv.In(intrinsic_res_s2), drv.In(a_pf_res_s1), drv.In(a_pf_res_s2), drv.In(exciton_to_ion_par0_rv), drv.In(exciton_to_ion_par1_rv), drv.In(exciton_to_ion_par2_rv), drv.In(pf_eff_par0), drv.In(pf_eff_par1), drv.In(s2_eff_par0), drv.In(s2_eff_par1), drv.In(prob_bkg_event), drv.In(num_bins_s1), d_gpu_local_info['d_gpu_bin_edges']['s1'][cathode_setting][degree_setting], drv.In(num_bins_log), d_gpu_local_info['d_gpu_bin_edges']['log'][cathode_setting][degree_setting], drv.InOut(a_hist_2d), drv.In(num_loops))

        d_gpu_local_info['function_to_call'](*tArgs, **self.d_gpu_scale)

        a_s1_s2_mc = np.reshape(a_hist_2d, (num_bins_log, num_bins_s1)).T

        sum_mc = np.sum(a_s1_s2_mc, dtype=np.float32)
        if sum_mc == 0:
            return -np.inf

        a_s1_s2_mc = np.multiply(a_s1_s2_mc, float(scale_par)*self.d_coincidence_data_information[cathode_setting][degree_setting]['num_data_pts']/float(self.num_mc_events*self.num_loops))

        # likelihood for single energy
        if draw_fit:

            f, (ax1, ax2) = plt.subplots(2, sharex=True, sharey=True)
                    
            a_s1_bin_edges = self.d_bin_edges['s1'][cathode_setting][degree_setting]
            a_log_bin_edges = self.d_bin_edges['log'][cathode_setting][degree_setting]

            s1_s2_data_plot = np.rot90(self.d_coincidence_data_information[cathode_setting][degree_setting]['a_log_s2_s1'])
            s1_s2_data_plot = np.flipud(s1_s2_data_plot)
            ax1.pcolormesh(a_s1_bin_edges, a_log_bin_edges, s1_s2_data_plot)

            s1_s2_mc_plot = np.rot90(a_s1_s2_mc)
            s1_s2_mc_plot = np.flipud(s1_s2_mc_plot)
            ax2.pcolormesh(a_s1_bin_edges, a_log_bin_edges, s1_s2_mc_plot)


            f_flat, (ax_s1, ax_log) = plt.subplots(2)
            
            flat_s1_data = np.sum(s1_s2_data_plot, axis=0)
            flat_s1_mc = np.sum(s1_s2_mc_plot, axis=0)
            ax_s1.errorbar((a_s1_bin_edges[1:]+a_s1_bin_edges[:-1])/2., flat_s1_data, yerr=flat_s1_data**0.5, fmt='bo')
            ax_s1.errorbar((a_s1_bin_edges[1:]+a_s1_bin_edges[:-1])/2., flat_s1_mc, yerr=flat_s1_mc**0.5, fmt='ro')
            
            flat_log_data = np.sum(s1_s2_data_plot, axis=1)
            flat_log_mc = np.sum(s1_s2_mc_plot, axis=1)
            ax_log.errorbar((a_log_bin_edges[1:]+a_log_bin_edges[:-1])/2., flat_log_data, yerr=flat_log_data**0.5, fmt='bo')
            ax_log.errorbar((a_log_bin_edges[1:]+a_log_bin_edges[:-1])/2., flat_log_mc, yerr=flat_log_mc**0.5, fmt='ro')

            f.savefig('./temp_results/2d_hist_%.3f_kV_%d_deg.png' % (cathode_setting, degree_setting))
            f_flat.savefig('./temp_results/1d_hists_%.3f_kV_%d_deg.png' % (cathode_setting, degree_setting))

            #plt.show()


        flat_s1_log_data = np.asarray(self.d_coincidence_data_information[cathode_setting][degree_setting]['a_log_s2_s1'].flatten(), dtype=np.float32)
        flat_s1_log_mc = np.asarray(a_s1_s2_mc.flatten(), dtype=np.float32)
        logLikelihoodMatching = c_log_likelihood(flat_s1_log_data, flat_s1_log_mc, len(flat_s1_log_data), self.small_number)
        
        total_ln_likelihood = logLikelihoodMatching + prior_ln_likelihood

        if self.b_suppress_likelihood:
            total_ln_likelihood /= self.ll_suppression_factor

        #print logLikelihoodMatching, prior_ln_likelihood
        #print 'Total time: %f' % (time.time() - start_time_tot_ll)
        #print total_ln_likelihood

        if np.isnan(total_ln_likelihood):
            return -np.inf
        else:
            return total_ln_likelihood


        
    def wrapper_ln_likelihood_full_matching_single_energy(self, a_parameters, kwargs={}):
        return self.ln_likelihood_full_matching_single_energy(*a_parameters, **kwargs)
    


    # -----------------------------------------------
    # -----------------------------------------------
    # Likelihood function for Lindhard with T-I
    # -----------------------------------------------
    # -----------------------------------------------


    def ln_likelihood_full_matching_multiple_energies_lindhard_model_with_ti_recombination(self, w_value, beta, kappa, eta, lamb, g1_value, spe_res_value, extraction_efficiency_value, gas_gain_mean_value, gas_gain_width_value, pf_eff_par0, pf_eff_par1, s2_eff_par0, s2_eff_par1, pf_stdev_s1_par0, pf_stdev_s1_par1, pf_stdev_s1_par2, pf_stdev_s2_par0, pf_stdev_s2_par1, pf_stdev_s2_par2, dpe_prob, d_field_degree_pars, d_gpu_local_info, draw_fit=False):



        # -----------------------------------------------
        # -----------------------------------------------
        # determine prior likelihood and variables
        # -----------------------------------------------
        # -----------------------------------------------

        #start_time_full = time.time()

        prior_ln_likelihood = 0
        matching_ln_likelihood = 0


        current_likelihood, w_value = self.get_w_value_default(w_value)
        prior_ln_likelihood += self.get_prior_log_likelihood_nuissance(current_likelihood)
        
        prior_ln_likelihood += self.get_prior_log_likelihood_beta(beta)
        prior_ln_likelihood += self.get_prior_log_likelihood_kappa(kappa)
        prior_ln_likelihood += self.get_prior_log_likelihood_eta(eta)
        prior_ln_likelihood += self.get_prior_log_likelihood_lamb(lamb)
        
        
        for cathode_setting in self.l_cathode_settings_in_use:
            for degree_setting in self.l_degree_settings_in_use:
                # recombination prior
                prior_ln_likelihood += self.get_prior_log_likelihood_ti_par(d_field_degree_pars[cathode_setting][degree_setting]['ti_par'])
                
                # exciton to ion ratio prior
                prior_ln_likelihood += self.get_prior_log_likelihood_exciton_to_ion_ratio(d_field_degree_pars[cathode_setting][degree_setting]['exciton_to_ion_ratio'])
            
    
                # scale prior
                prior_ln_likelihood += self.get_prior_log_likelihood_greater_than_zero(d_field_degree_pars[cathode_setting][degree_setting]['scale'])
    
    
                prior_ln_likelihood += self.get_prior_log_likelihood_probability(d_field_degree_pars[cathode_setting][degree_setting]['prob_bkg'])
                


        # priors of detector variables
        current_likelihood, g1_value = self.get_g1_default(g1_value)
        prior_ln_likelihood += self.get_prior_log_likelihood_nuissance(current_likelihood)
        
        current_likelihood, extraction_efficiency = self.get_extraction_efficiency_default(extraction_efficiency_value)
        prior_ln_likelihood += self.get_prior_log_likelihood_nuissance(current_likelihood)

        current_likelihood, spe_res = self.get_spe_res_default(spe_res_value)
        prior_ln_likelihood += self.get_prior_log_likelihood_nuissance(current_likelihood)

        current_likelihood, gas_gain_value = self.get_gas_gain_default(gas_gain_mean_value)
        prior_ln_likelihood += self.get_prior_log_likelihood_nuissance(current_likelihood)

        current_likelihood, gas_gain_width = self.get_gas_gain_width_default(gas_gain_width_value)
        prior_ln_likelihood += self.get_prior_log_likelihood_nuissance(current_likelihood)


        # priors of efficiencies
        current_likelihood, pf_eff_par0, pf_eff_par1 = self.get_pf_eff_default(pf_eff_par0, pf_eff_par1)
        prior_ln_likelihood += self.get_prior_log_likelihood_nuissance(current_likelihood)

        current_likelihood, s2_eff_par0, s2_eff_par1 = self.get_s2_eff_default(s2_eff_par0, s2_eff_par1)
        prior_ln_likelihood += self.get_prior_log_likelihood_nuissance(current_likelihood)

        #current_ln_likelihood, pf_stdev_par0, pf_stdev_par1, pf_stdev_par2 = self.get_pf_stdev_default(pf_stdev_par0, pf_stdev_par1, pf_stdev_par2)
        #prior_ln_likelihood += self.get_prior_log_likelihood_nuissance(current_ln_likelihood)
        
        prior_ln_likelihood += self .get_prior_log_likelihood_greater_than_zero(pf_stdev_s1_par0)
        prior_ln_likelihood += self .get_prior_log_likelihood_greater_than_zero(pf_stdev_s1_par1)
        prior_ln_likelihood += self .get_prior_log_likelihood_greater_than_zero(pf_stdev_s1_par2)
        
        prior_ln_likelihood += self .get_prior_log_likelihood_greater_than_zero(pf_stdev_s2_par0)
        prior_ln_likelihood += self .get_prior_log_likelihood_greater_than_zero(pf_stdev_s2_par1)
        prior_ln_likelihood += self .get_prior_log_likelihood_greater_than_zero(pf_stdev_s2_par2)
        
        prior_ln_likelihood += self.get_prior_log_likelihood_dpe_prob(dpe_prob)
        

        # if prior is -inf then don't bother with MC
        #print 'removed prior infinity catch temporarily'
        if not np.isfinite(prior_ln_likelihood) and not draw_fit:
            return -np.inf



        # -----------------------------------------------
        # -----------------------------------------------
        # run MC
        # -----------------------------------------------
        # -----------------------------------------------
        

        num_trials = np.asarray(self.num_mc_events, dtype=np.int32)

        w_value = np.asarray(w_value, dtype=np.float32)
        beta = np.asarray(beta, dtype=np.float32)
        kappa = np.asarray(kappa, dtype=np.float32)
        eta = np.asarray(eta, dtype=np.float32)
        lamb = np.asarray(lamb, dtype=np.float32)

        g1_value = np.asarray(g1_value, dtype=np.float32)
        extraction_efficiency = np.asarray(extraction_efficiency, dtype=np.float32)
        spe_res = np.asarray(spe_res, dtype=np.float32)
        gas_gain_value = np.asarray(gas_gain_value, dtype=np.float32)
        gas_gain_width = np.asarray(gas_gain_width, dtype=np.float32)



        #print 'hard coding pf eff'
        #pf_eff_par0 = np.asarray(0.01, dtype=np.float32)
        #pf_eff_par1 = np.asarray(0.01, dtype=np.float32)
        pf_eff_par0 = np.asarray(pf_eff_par0, dtype=np.float32)
        pf_eff_par1 = np.asarray(pf_eff_par1, dtype=np.float32)

        s2_eff_par0 = np.asarray(s2_eff_par0, dtype=np.float32)
        s2_eff_par1 = np.asarray(s2_eff_par1, dtype=np.float32)

        a_pf_stdev_s1 = np.asarray([pf_stdev_s1_par0, pf_stdev_s1_par1, pf_stdev_s1_par2], dtype=np.float32)
        a_pf_stdev_s2 = np.asarray([pf_stdev_s2_par0, pf_stdev_s2_par1, pf_stdev_s2_par2], dtype=np.float32)

        dpe_prob = np.asarray(dpe_prob, dtype=np.float32)
        
        
        # for histogram binning
        
        num_loops = np.asarray(self.num_loops, dtype=np.int32)



        for cathode_setting in self.l_cathode_settings_in_use:
            for degree_setting in self.l_degree_settings_in_use:
            
                # get mean field
                mean_field = np.asarray(self.d_cathode_voltages_to_field[cathode_setting], dtype=np.float32)
                
                prob_bkg = np.asarray(d_field_degree_pars[cathode_setting][degree_setting]['prob_bkg'], dtype=np.float32)
                
                #print '%.3f kV %d deg' % (cathode_setting, degree_setting)
                #print 'prob bkg: %f\n' % (prob_bkg)
                
                
                # get recombination parameters
                ti_par = np.asarray(d_field_degree_pars[cathode_setting][degree_setting]['ti_par'], dtype=np.float32)
                
                
                # get exciton to ion ratio
                exciton_to_ion_ratio = np.asarray(d_field_degree_pars[cathode_setting][degree_setting]['exciton_to_ion_ratio'], dtype=np.float32)
                
                # binning
                num_bins_s1 = np.asarray(len(d_gpu_local_info['d_gpu_bin_edges']['s1'][cathode_setting][degree_setting])-1, dtype=np.int32)
                num_bins_log = np.asarray(len(d_gpu_local_info['d_gpu_bin_edges']['log'][cathode_setting][degree_setting])-1, dtype=np.int32)
                
                #print d_gpu_local_info['d_gpu_bin_edges']['s1'][cathode_setting][degree_setting]
                
                a_hist_2d = np.zeros(int(num_bins_s1)*int(num_bins_log), dtype=np.float32)
                
                #print cathode_setting, degree_setting
                #print mean_field
                
                #start_time_mc = time.time()
                
            
                tArgs = (d_gpu_local_info['rng_states'], drv.In(num_trials), drv.In(mean_field), d_gpu_local_info['d_gpu_energy'][degree_setting], d_gpu_local_info['gpu_energy_bkg'], drv.In(w_value), drv.In(exciton_to_ion_ratio), drv.In(ti_par), drv.In(beta), drv.In(kappa), drv.In(eta), drv.In(lamb), drv.In(g1_value), drv.In(extraction_efficiency), drv.In(gas_gain_value), drv.In(gas_gain_width), drv.In(spe_res), drv.In(a_pf_stdev_s1), drv.In(a_pf_stdev_s2), drv.In(pf_eff_par0), drv.In(pf_eff_par1), drv.In(s2_eff_par0), drv.In(s2_eff_par1), drv.In(dpe_prob), drv.In(prob_bkg), drv.In(num_bins_s1), d_gpu_local_info['d_gpu_bin_edges']['s1'][cathode_setting][degree_setting], drv.In(num_bins_log), d_gpu_local_info['d_gpu_bin_edges']['log'][cathode_setting][degree_setting], drv.InOut(a_hist_2d), drv.In(num_loops))

                d_gpu_local_info['function_to_call'](*tArgs, **self.d_gpu_scale)
                
                #print 'MC time: %f' % (time.time() - start_time_mc)
                #start_time_tot_ll = time.time()

                a_s1_s2_mc = np.reshape(a_hist_2d, (int(num_bins_log), int(num_bins_s1))).T
                
                sum_mc = np.sum(a_s1_s2_mc, dtype=np.float32)
                if sum_mc == 0:
                    return -np.inf

                #a_s1_s2_mc = np.multiply(a_s1_s2_mc, np.sum(self.a_s1_s2, dtype=np.float32) / sum_mc)

                # if making PDF rather than scaling for rate
                scale_par = d_field_degree_pars[cathode_setting][degree_setting]['scale']
                #scale_par *= float(self.num_mc_events*self.num_loops) / sum_mc
                #integral_mc = np.sum(((a_s1_s2_mc*np.diff(self.d_bin_edges['log'][cathode_setting][degree_setting])).T)*np.diff(self.d_bin_edges['s1'][cathode_setting][degree_setting]))
                #integral_data = np.sum(((self.d_coincidence_data_information[cathode_setting][degree_setting]['a_log_s2_s1']*np.diff(self.d_bin_edges['log'][cathode_setting][degree_setting])).T)*np.diff(self.d_bin_edges['s1'][cathode_setting][degree_setting]))
                #a_s1_s2_mc = np.multiply(a_s1_s2_mc, float(scale_par)*integral_data/float(integral_mc))
                
                #a_s1_s2_mc = np.multiply(a_s1_s2_mc, float(scale_par)*self.d_coincidence_data_information[cathode_setting][degree_setting]['num_data_pts']/float(sum_mc))
                
                #print cathode_setting, degree_setting, scale_par, self.d_coincidence_data_information[cathode_setting][degree_setting]['num_data_pts'], self.num_mc_events*self.num_loops
                #print np.sum(a_s1_s2_mc)
                a_s1_s2_mc = np.multiply(a_s1_s2_mc, float(scale_par)*self.d_coincidence_data_information[cathode_setting][degree_setting]['num_data_pts']/float(self.num_mc_events*self.num_loops))
                #print np.sum(a_s1_s2_mc)

                # likelihood for mlti
                if draw_fit:

                    f, (ax1, ax2) = plt.subplots(2, sharex=True, sharey=True)
                    
                    a_s1_bin_edges = self.d_bin_edges['s1'][cathode_setting][degree_setting]
                    a_log_bin_edges = self.d_bin_edges['log'][cathode_setting][degree_setting]

                    s1_s2_data_plot = np.rot90(self.d_coincidence_data_information[cathode_setting][degree_setting]['a_log_s2_s1'])
                    s1_s2_data_plot = np.flipud(s1_s2_data_plot)
                    ax1.pcolormesh(a_s1_bin_edges, a_log_bin_edges, s1_s2_data_plot)

                    s1_s2_mc_plot = np.rot90(a_s1_s2_mc)
                    s1_s2_mc_plot = np.flipud(s1_s2_mc_plot)
                    ax2.pcolormesh(a_s1_bin_edges, a_log_bin_edges, s1_s2_mc_plot)


                    f_flat, (ax_s1, ax_log) = plt.subplots(2)
                    
                    flat_s1_data = np.sum(s1_s2_data_plot, axis=0)
                    flat_s1_mc = np.sum(s1_s2_mc_plot, axis=0)
                    ax_s1.errorbar((a_s1_bin_edges[1:]+a_s1_bin_edges[:-1])/2., flat_s1_data, yerr=flat_s1_data**0.5, fmt='bo')
                    ax_s1.errorbar((a_s1_bin_edges[1:]+a_s1_bin_edges[:-1])/2., flat_s1_mc, yerr=flat_s1_mc**0.5, fmt='ro')
                    
                    flat_log_data = np.sum(s1_s2_data_plot, axis=1)
                    flat_log_mc = np.sum(s1_s2_mc_plot, axis=1)
                    ax_log.errorbar((a_log_bin_edges[1:]+a_log_bin_edges[:-1])/2., flat_log_data, yerr=flat_log_data**0.5, fmt='bo')
                    ax_log.errorbar((a_log_bin_edges[1:]+a_log_bin_edges[:-1])/2., flat_log_mc, yerr=flat_log_mc**0.5, fmt='ro')

                    f.savefig('./temp_results/2d_hist_%.3f_kV_%d_deg.png' % (cathode_setting, degree_setting))
                    f_flat.savefig('./temp_results/1d_hists_%.3f_kV_%d_deg.png' % (cathode_setting, degree_setting))

                    #plt.show()

                flat_s1_log_data = np.asarray(self.d_coincidence_data_information[cathode_setting][degree_setting]['a_log_s2_s1'].flatten(), dtype=np.float32)
                flat_s1_log_mc = np.asarray(a_s1_s2_mc.flatten(), dtype=np.float32)
                
                logLikelihoodMatching = c_log_likelihood(flat_s1_log_data, flat_s1_log_mc, len(flat_s1_log_data), self.small_number)
                
                matching_ln_likelihood += logLikelihoodMatching

                #print cathode_setting, degree_setting, logLikelihoodMatching

        #print 'full function time: %f' % (time.time() - start_time_full)
        total_ln_likelihood = matching_ln_likelihood + prior_ln_likelihood
        #print total_ln_likelihood

        if self.b_suppress_likelihood:
            total_ln_likelihood /= self.ll_suppression_factor



        if np.isnan(total_ln_likelihood):
            return -np.inf
        else:
            return total_ln_likelihood


        
    def wrapper_ln_likelihood_full_matching_multiple_energies_lindhard_model_with_ti_recombination(self, a_parameters, kwargs={}):
        #print a_parameters
        #print '\n\n\n'
        d_field_degree_pars = {}
        counter = 0
        num_field_degree_pars = len(self.l_cathode_settings_in_use)*(2) + len(self.l_degree_settings_in_use)*len(self.l_cathode_settings_in_use)*2
        # 5 field parameters and scales/bkgs
        
        if -4 in self.l_degree_settings_in_use:
            b_band_used = True
            # have to subtract out bkg parameters
            num_field_degree_pars -= len(self.l_cathode_settings_in_use)
        else:
            b_band_used = False
        
        
        for cathode_setting in self.l_cathode_settings_in_use:
            d_field_degree_pars[cathode_setting] = {}
            count_degree_pars = 0
            for degree_setting in self.l_degree_settings_in_use:
                d_field_degree_pars[cathode_setting][degree_setting] = {}
                
                # recombination
                d_field_degree_pars[cathode_setting][degree_setting]['ti_par'] = a_parameters[-(num_field_degree_pars+1)+counter+0]
                
                # exciton to ion ratio
                d_field_degree_pars[cathode_setting][degree_setting]['exciton_to_ion_ratio'] = a_parameters[-(num_field_degree_pars+1)+counter+1]
                
                if not b_band_used:
                    d_field_degree_pars[cathode_setting][degree_setting]['prob_bkg'] = a_parameters[-(num_field_degree_pars+1)+counter+2+count_degree_pars]
                    count_degree_pars += 1
                    d_field_degree_pars[cathode_setting][degree_setting]['scale'] = a_parameters[-(num_field_degree_pars+1)+counter+2+count_degree_pars]
                    count_degree_pars += 1
        
                else:
                    if degree_setting == -4:
                        d_field_degree_pars[cathode_setting][degree_setting]['prob_bkg'] = 0.
                        d_field_degree_pars[cathode_setting][degree_setting]['scale'] = a_parameters[-(num_field_degree_pars+1)+counter+2+count_degree_pars]
                        count_degree_pars += 1
                    
                    
                    else:
                        d_field_degree_pars[cathode_setting][degree_setting]['prob_bkg'] = a_parameters[-(num_field_degree_pars+1)+counter+2+count_degree_pars]
                        count_degree_pars += 1
                        d_field_degree_pars[cathode_setting][degree_setting]['scale'] = a_parameters[-(num_field_degree_pars+1)+counter+2+count_degree_pars]
                        count_degree_pars += 1
        
                
                
    
            # move counter
            counter += 1 + 1 + count_degree_pars
            
        
        
        return self.ln_likelihood_full_matching_multiple_energies_lindhard_model_with_ti_recombination(*(list(a_parameters[:-(num_field_degree_pars+1)]) + [d_field_degree_pars, a_parameters[-1]]), **kwargs)





    # -----------------------------------------------
    # -----------------------------------------------
    # Likelihood function for testing models
    # -----------------------------------------------
    # -----------------------------------------------



    # mod gompertz
    # (self, w_value, alpha, gom_par0, gom_par1, gom_par2, gom_par3, kappa, intrinsic_res_s1, intrinsic_res_s2, g1_value, spe_res_value, extraction_efficiency_value, gas_gain_mean_value, gas_gain_width_value, pf_eff_par0, pf_eff_par1, s2_eff_par0, s2_eff_par1, pf_stdev_par0, pf_stdev_par1, pf_stdev_par2, dpe_prob, d_scale_pars, d_gpu_local_info, draw_fit=False)

    # mod gompertz with beta
    # (self, w_value, alpha, beta, gom_par0, gom_par1, gom_par2, gom_par3, kappa, intrinsic_res_s1, intrinsic_res_s2, g1_value, spe_res_value, extraction_efficiency_value, gas_gain_mean_value, gas_gain_width_value, pf_eff_par0, pf_eff_par1, s2_eff_par0, s2_eff_par1, pf_stdev_par0, pf_stdev_par1, pf_stdev_par2, dpe_prob, d_scale_pars, d_gpu_local_info, draw_fit=False)


    def ln_likelihood_test(self, w_value, beta, kappa, eta, lamb, intrinsic_res_s1, intrinsic_res_s2, g1_value, spe_res_value, extraction_efficiency_value, gas_gain_mean_value, gas_gain_width_value, pf_eff_par0, pf_eff_par1, s2_eff_par0, s2_eff_par1, pf_stdev_par0, pf_stdev_par1, pf_stdev_par2, dpe_prob, d_field_degree_pars, d_gpu_local_info, draw_fit=False):



        # -----------------------------------------------
        # -----------------------------------------------
        # determine prior likelihood and variables
        # -----------------------------------------------
        # -----------------------------------------------

        #start_time_full = time.time()

        prior_ln_likelihood = 0
        matching_ln_likelihood = 0


        current_likelihood, w_value = self.get_w_value_default(w_value)
        prior_ln_likelihood += self.get_prior_log_likelihood_nuissance(current_likelihood)
        
        prior_ln_likelihood += self.get_prior_log_likelihood_beta(beta)
        prior_ln_likelihood += self.get_prior_log_likelihood_kappa(kappa)
        prior_ln_likelihood += self.get_prior_log_likelihood_eta(eta)
        prior_ln_likelihood += self.get_prior_log_likelihood_lamb(lamb)
        
        
        for cathode_setting in self.l_cathode_settings_in_use:
            for degree_setting in self.l_degree_settings_in_use:
                
                # exciton to ion ratio prior
                prior_ln_likelihood += self.get_prior_log_likelihood_exciton_to_ion_ratio(d_field_degree_pars[cathode_setting][degree_setting]['exciton_to_ion_ratio'])
            
    
                # scale prior
                prior_ln_likelihood += self.get_prior_log_likelihood_probability(d_field_degree_pars[cathode_setting][degree_setting]['scale'])
    
    
                prior_ln_likelihood += self.get_prior_log_likelihood_probability(d_field_degree_pars[cathode_setting][degree_setting]['prob_bkg'])
                

        # priors of resolutions
        prior_ln_likelihood += self.get_prior_log_likelihood_resolution(intrinsic_res_s1)
        prior_ln_likelihood += self.get_prior_log_likelihood_resolution(intrinsic_res_s2)
        

        # priors of detector variables
        current_likelihood, g1_value = self.get_g1_default(g1_value)
        prior_ln_likelihood += self.get_prior_log_likelihood_nuissance(current_likelihood)
        
        current_likelihood, extraction_efficiency = self.get_extraction_efficiency_default(extraction_efficiency_value)
        prior_ln_likelihood += self.get_prior_log_likelihood_nuissance(current_likelihood)

        current_likelihood, spe_res = self.get_spe_res_default(spe_res_value)
        prior_ln_likelihood += self.get_prior_log_likelihood_nuissance(current_likelihood)

        current_likelihood, gas_gain_value = self.get_gas_gain_default(gas_gain_mean_value)
        prior_ln_likelihood += self.get_prior_log_likelihood_nuissance(current_likelihood)

        current_likelihood, gas_gain_width = self.get_gas_gain_width_default(gas_gain_width_value)
        prior_ln_likelihood += self.get_prior_log_likelihood_nuissance(current_likelihood)


        # priors of efficiencies
        current_likelihood, pf_eff_par0, pf_eff_par1 = self.get_pf_eff_default(pf_eff_par0, pf_eff_par1)
        prior_ln_likelihood += self.get_prior_log_likelihood_nuissance(current_likelihood)

        current_likelihood, s2_eff_par0, s2_eff_par1 = self.get_s2_eff_default(s2_eff_par0, s2_eff_par1)
        prior_ln_likelihood += self.get_prior_log_likelihood_nuissance(current_likelihood)

        current_ln_likelihood, pf_stdev_par0, pf_stdev_par1, pf_stdev_par2 = self.get_pf_stdev_default(pf_stdev_par0, pf_stdev_par1, pf_stdev_par2)
        prior_ln_likelihood += self.get_prior_log_likelihood_nuissance(current_ln_likelihood)
        
        prior_ln_likelihood += self.get_prior_log_likelihood_dpe_prob(dpe_prob)
        

        # if prior is -inf then don't bother with MC
        #print 'removed prior infinity catch temporarily'
        if not np.isfinite(prior_ln_likelihood) and not draw_fit:
            return -np.inf



        # -----------------------------------------------
        # -----------------------------------------------
        # run MC
        # -----------------------------------------------
        # -----------------------------------------------
        

        num_trials = np.asarray(self.num_mc_events, dtype=np.int32)

        w_value = np.asarray(w_value, dtype=np.float32)
        beta = np.asarray(beta, dtype=np.float32)
        kappa = np.asarray(kappa, dtype=np.float32)
        eta = np.asarray(eta, dtype=np.float32)
        lamb = np.asarray(lamb, dtype=np.float32)

        g1_value = np.asarray(g1_value, dtype=np.float32)
        extraction_efficiency = np.asarray(extraction_efficiency, dtype=np.float32)
        spe_res = np.asarray(spe_res, dtype=np.float32)
        gas_gain_value = np.asarray(gas_gain_value, dtype=np.float32)
        gas_gain_width = np.asarray(gas_gain_width, dtype=np.float32)

        intrinsic_res_s1 = np.asarray(intrinsic_res_s1, dtype=np.float32)
        intrinsic_res_s2 = np.asarray(intrinsic_res_s2, dtype=np.float32)

        #print 'hard coding pf eff'
        #pf_eff_par0 = np.asarray(0.01, dtype=np.float32)
        #pf_eff_par1 = np.asarray(0.01, dtype=np.float32)
        pf_eff_par0 = np.asarray(pf_eff_par0, dtype=np.float32)
        pf_eff_par1 = np.asarray(pf_eff_par1, dtype=np.float32)

        s2_eff_par0 = np.asarray(s2_eff_par0, dtype=np.float32)
        s2_eff_par1 = np.asarray(s2_eff_par1, dtype=np.float32)

        a_pf_stdev = np.asarray([pf_stdev_par0, pf_stdev_par1, pf_stdev_par2], dtype=np.float32)

        dpe_prob = np.asarray(dpe_prob, dtype=np.float32)
        
        
        # for histogram binning
        
        num_loops = np.asarray(self.num_loops, dtype=np.int32)



        for cathode_setting in self.l_cathode_settings_in_use:
            for degree_setting in self.l_degree_settings_in_use:
            
                # get mean field
                mean_field = np.asarray(self.d_cathode_voltages_to_field[cathode_setting], dtype=np.float32)
                
                prob_bkg = np.asarray(d_field_degree_pars[cathode_setting][degree_setting]['prob_bkg'], dtype=np.float32)
                
                # get recombination parameters
                a_recombination_pars = np.asarray(d_field_degree_pars[cathode_setting][degree_setting]['l_recombination'], dtype=np.float32)
                
                
                # get exciton to ion ratio
                exciton_to_ion_ratio = np.asarray(d_field_degree_pars[cathode_setting][degree_setting]['exciton_to_ion_ratio'], dtype=np.float32)
                
                # binning
                num_bins_s1 = np.asarray(len(d_gpu_local_info['d_gpu_bin_edges']['s1'][cathode_setting][degree_setting])-1, dtype=np.int32)
                num_bins_log = np.asarray(len(d_gpu_local_info['d_gpu_bin_edges']['log'][cathode_setting][degree_setting])-1, dtype=np.int32)
                
                
                a_hist_2d = np.zeros(int(num_bins_s1)*int(num_bins_log), dtype=np.float32)
                
                
                tArgs = (d_gpu_local_info['rng_states'], drv.In(num_trials), drv.In(mean_field), d_gpu_local_info['d_gpu_energy'][degree_setting], d_gpu_local_info['gpu_energy_bkg'], drv.In(w_value), drv.In(exciton_to_ion_ratio), drv.In(a_recombination_pars), drv.In(beta), drv.In(kappa), drv.In(eta), drv.In(lamb), drv.In(g1_value), drv.In(extraction_efficiency), drv.In(gas_gain_value), drv.In(gas_gain_width), drv.In(spe_res), drv.In(intrinsic_res_s1), drv.In(intrinsic_res_s2), drv.In(a_pf_stdev), drv.In(pf_eff_par0), drv.In(pf_eff_par1), drv.In(s2_eff_par0), drv.In(s2_eff_par1), drv.In(dpe_prob), drv.In(prob_bkg), drv.In(num_bins_s1), d_gpu_local_info['d_gpu_bin_edges']['s1'][cathode_setting][degree_setting], drv.In(num_bins_log), d_gpu_local_info['d_gpu_bin_edges']['log'][cathode_setting][degree_setting], drv.InOut(a_hist_2d), drv.In(num_loops))

                d_gpu_local_info['function_to_call'](*tArgs, **self.d_gpu_scale)
                
                #print 'MC time: %f' % (time.time() - start_time_mc)
                #start_time_tot_ll = time.time()

                a_s1_s2_mc = np.reshape(a_hist_2d, (int(num_bins_log), int(num_bins_s1))).T
                
                sum_mc = np.sum(a_s1_s2_mc, dtype=np.float32)
                if sum_mc == 0:
                    return -np.inf

                #a_s1_s2_mc = np.multiply(a_s1_s2_mc, np.sum(self.a_s1_s2, dtype=np.float32) / sum_mc)

                # if making PDF rather than scaling for rate
                scale_par = d_field_degree_pars[cathode_setting][degree_setting]['scale']
                #scale_par *= float(self.num_mc_events*self.num_loops) / sum_mc
                integral_mc = np.sum(((a_s1_s2_mc*np.diff(self.d_bin_edges['log'][cathode_setting][degree_setting])).T)*np.diff(self.d_bin_edges['s1'][cathode_setting][degree_setting]))
                integral_data = np.sum(((self.d_coincidence_data_information[cathode_setting][degree_setting]['a_log_s2_s1']*np.diff(self.d_bin_edges['log'][cathode_setting][degree_setting])).T)*np.diff(self.d_bin_edges['s1'][cathode_setting][degree_setting]))
                #a_s1_s2_mc = np.multiply(a_s1_s2_mc, float(scale_par)*self.d_coincidence_data_information[cathode_setting][degree_setting]['num_data_pts']/float(self.num_mc_events*self.num_loops))
                a_s1_s2_mc = np.multiply(a_s1_s2_mc, float(scale_par)*integral_data/float(integral_mc))

                # likelihood for mlti
                if draw_fit:

                    f, (ax1, ax2) = plt.subplots(2, sharex=True, sharey=True)
                    
                    a_s1_bin_edges = self.d_bin_edges['s1'][cathode_setting][degree_setting]
                    a_log_bin_edges = self.d_bin_edges['log'][cathode_setting][degree_setting]

                    s1_s2_data_plot = np.rot90(self.d_coincidence_data_information[cathode_setting][degree_setting]['a_log_s2_s1'])
                    s1_s2_data_plot = np.flipud(s1_s2_data_plot)
                    ax1.pcolormesh(a_s1_bin_edges, a_log_bin_edges, s1_s2_data_plot)

                    s1_s2_mc_plot = np.rot90(a_s1_s2_mc)
                    s1_s2_mc_plot = np.flipud(s1_s2_mc_plot)
                    ax2.pcolormesh(a_s1_bin_edges, a_log_bin_edges, s1_s2_mc_plot)


                    f_flat, (ax_s1, ax_log) = plt.subplots(2)
                    
                    flat_s1_data = np.sum(s1_s2_data_plot, axis=0)
                    flat_s1_mc = np.sum(s1_s2_mc_plot, axis=0)
                    ax_s1.errorbar((a_s1_bin_edges[1:]+a_s1_bin_edges[:-1])/2., flat_s1_data, yerr=flat_s1_data**0.5, fmt='bo')
                    ax_s1.errorbar((a_s1_bin_edges[1:]+a_s1_bin_edges[:-1])/2., flat_s1_mc, yerr=flat_s1_mc**0.5, fmt='ro')
                    
                    flat_log_data = np.sum(s1_s2_data_plot, axis=1)
                    flat_log_mc = np.sum(s1_s2_mc_plot, axis=1)
                    ax_log.errorbar((a_log_bin_edges[1:]+a_log_bin_edges[:-1])/2., flat_log_data, yerr=flat_log_data**0.5, fmt='bo')
                    ax_log.errorbar((a_log_bin_edges[1:]+a_log_bin_edges[:-1])/2., flat_log_mc, yerr=flat_log_mc**0.5, fmt='ro')

                    f.savefig('./temp_results/2d_hist_%.3f_kV_%d_deg.png' % (cathode_setting, degree_setting))
                    f_flat.savefig('./temp_results/1d_hists_%.3f_kV_%d_deg.png' % (cathode_setting, degree_setting))

                    #plt.show()

                flat_s1_log_data = np.asarray(self.d_coincidence_data_information[cathode_setting][degree_setting]['a_log_s2_s1'].flatten(), dtype=np.float32)
                flat_s1_log_mc = np.asarray(a_s1_s2_mc.flatten(), dtype=np.float32)
                
                logLikelihoodMatching = c_log_likelihood(flat_s1_log_data, flat_s1_log_mc, len(flat_s1_log_data), int(self.d_coincidence_data_information[cathode_setting][degree_setting]['num_data_pts']), 0.95)
                
                matching_ln_likelihood += logLikelihoodMatching

                #print logLikelihoodMatching, prior_ln_likelihood
                #print 'likelihood calculation time: %f' % (time.time() - start_time_tot_ll)
                #print logLikelihoodMatching

        #print 'full function time: %f' % (time.time() - start_time_full)
        total_ln_likelihood = matching_ln_likelihood + prior_ln_likelihood
        #print total_ln_likelihood

        if self.b_suppress_likelihood:
            total_ln_likelihood /= self.ll_suppression_factor



        if np.isnan(total_ln_likelihood):
            return -np.inf
        else:
            return total_ln_likelihood

        
    def wrapper_ln_likelihood_test(self, a_parameters, kwargs={}):
        #print a_parameters
        #print '\n\n\n'
        d_field_degree_pars = {}
        counter = 0
        
        if self.test_switch == 'ti':
            num_recombination_pars = 1
        elif self.test_switch == 'pol1':
            num_recombination_pars = 2
        elif self.test_switch == 'pol2':
            num_recombination_pars = 3
        elif self.test_switch == 'pol3':
            num_recombination_pars = 4
        elif self.test_switch == 'pol4':
            num_recombination_pars = 5
        elif self.test_switch == 'pol5':
            num_recombination_pars = 6
        elif self.test_switch == 'mod_gom':
            num_recombination_pars = 4
        
        num_field_degree_pars = len(self.l_cathode_settings_in_use)*(1+num_recombination_pars) + len(self.l_degree_settings_in_use)*len(self.l_cathode_settings_in_use)*2
        # 5 field parameters and scales/bkgs
        
        if -4 in self.l_degree_settings_in_use:
            b_band_used = True
            # have to subtract out bkg parameters
            num_field_degree_pars -= len(self.l_cathode_settings_in_use)
        else:
            b_band_used = False
        
        
        for cathode_setting in self.l_cathode_settings_in_use:
            d_field_degree_pars[cathode_setting] = {}
            count_degree_pars = 0
            for degree_setting in self.l_degree_settings_in_use:
                d_field_degree_pars[cathode_setting][degree_setting] = {}
                
                # recombination
                d_field_degree_pars[cathode_setting][degree_setting]['l_recombination'] = a_parameters[(-(num_field_degree_pars+1)+counter+0):(-(num_field_degree_pars+1)+counter+num_recombination_pars)]
                
                # exciton to ion ratio
                d_field_degree_pars[cathode_setting][degree_setting]['exciton_to_ion_ratio'] = a_parameters[-(num_field_degree_pars+(num_recombination_pars))+counter+1]
                
                if not b_band_used:
                    d_field_degree_pars[cathode_setting][degree_setting]['prob_bkg'] = a_parameters[-(num_field_degree_pars+1)+counter+(num_recombination_pars+1)+count_degree_pars]
                    count_degree_pars += 1
                    d_field_degree_pars[cathode_setting][degree_setting]['scale'] = a_parameters[-(num_field_degree_pars+1)+counter+(num_recombination_pars+1)+count_degree_pars]
                    count_degree_pars += 1
        
                else:
                    if degree_setting == -4:
                        d_field_degree_pars[cathode_setting][degree_setting]['prob_bkg'] = 0.
                        d_field_degree_pars[cathode_setting][degree_setting]['scale'] = a_parameters[-(num_field_degree_pars+1)+counter+(num_recombination_pars+1)+count_degree_pars]
                        count_degree_pars += 1
                    
                    
                    else:
                        d_field_degree_pars[cathode_setting][degree_setting]['prob_bkg'] = a_parameters[-(num_field_degree_pars+1)+counter+(num_recombination_pars+1)+count_degree_pars]
                        count_degree_pars += 1
                        d_field_degree_pars[cathode_setting][degree_setting]['scale'] = a_parameters[-(num_field_degree_pars+1)+counter+(num_recombination_pars+1)+count_degree_pars]
                        count_degree_pars += 1
        
                
                
    
            # move counter
            counter += num_recombination_pars + 1 + count_degree_pars
        
        
        return self.ln_likelihood_test(*(list(a_parameters[:-(num_field_degree_pars+1)]) + [d_field_degree_pars, a_parameters[-1]]), **kwargs)





    def initial_positions_for_ensemble(self, a_free_parameters, num_walkers):
    
        count_free_pars = 0
    
        if self.fit_type == 's':
            l_par_names = ['py', 'qy', 'intrinsic_res_s1', 'intrinsic_res_s2', 'g1_value', 'spe_res_value', 'extraction_efficiency_value', 'gas_gain_mean_value', 'gas_gain_width_value', 'pf_eff_par0', 'pf_eff_par1', 's2_eff_par0', 's2_eff_par1', 'pf_res_s1', 'pf_res_s2', 'exciton_to_ion_par0_rv', 'exciton_to_ion_par1_rv', 'exciton_to_ion_par2_rv', 'prob_bkg', 'scale']
        
        elif self.fit_type == 'm':
            l_par_names = ['a_py', 'a_qy', 'intrinsic_res_s1', 'intrinsic_res_s2', 'g1_value', 'spe_res_rv', 'g2_value', 'gas_gain_rv', 'gas_gain_width_rv', 'pf_eff_par0', 'pf_eff_par1', 's2_eff_par0', 's2_eff_par1', 'pf_stdev_par0', 'pf_stdev_par1', 'pf_stdev_par2', 'exciton_to_ion_par0_rv', 'exciton_to_ion_par1_rv', 'exciton_to_ion_par2_rv']
        
        
        elif self.fit_type == 'mb':
            l_par_names = ['a_py', 'a_qy', 'intrinsic_res_s1', 'intrinsic_res_s2', 'g1_value', 'spe_res_rv', 'g2_value', 'gas_gain_rv', 'gas_gain_width_rv', 'pf_eff_par0', 'pf_eff_par1', 's1_eff_par0', 's1_eff_par1', 's2_eff_par0', 's2_eff_par1', 'pf_stdev_par0', 'pf_stdev_par1', 'pf_stdev_par2', 'exciton_to_ion_par0_rv', 'exciton_to_ion_par1_rv', 'exciton_to_ion_par2_rv']
            
            
        elif self.fit_type == 'bl':
            count_free_pars = 0
            l_field_free_pars = []
            for cathode_setting in self.l_cathode_settings_in_use:
                l_field_free_pars += ['gom_par0_%.3f_kV' % (cathode_setting), 'gom_par1_%.3f_kV' % (cathode_setting), 'gom_par2_%.3f_kV' % (cathode_setting), 'gom_par3_%.3f_kV' % (cathode_setting), 'exciton_to_ion_ratio_%.3f_kV' % (cathode_setting), 'scale_%.3f_kV' % (cathode_setting)]
            
            free_par_per_field = 6
            
            l_par_names = ['w_value', 'kappa', 'intrinsic_res_s1', 'intrinsic_res_s2', 'g1_value', 'spe_res_value', 'extraction_efficiency_value', 'gas_gain_mean_value', 'gas_gain_width_value', 'pf_eff_par0', 'pf_eff_par1', 's2_eff_par0', 's2_eff_par1', 'pf_stdev_par0', 'pf_stdev_par1', 'pf_stdev_par2', 'dpe_prob'] + l_field_free_pars


        elif self.fit_type == 'ml':
            
            l_field_free_pars = []
            for cathode_setting in self.l_cathode_settings_in_use:
                l_field_free_pars += ['gom_par0_%.3f_kV' % (cathode_setting), 'gom_par1_%.3f_kV' % (cathode_setting), 'gom_par2_%.3f_kV' % (cathode_setting), 'gom_par3_%.3f_kV' % (cathode_setting), 'exciton_to_ion_ratio_%.3f_kV' % (cathode_setting)]
                
                for degree_setting in self.l_degree_settings_in_use:
                    l_field_free_pars.append('prob_bkg_%.3f_kV_%d_deg' % (cathode_setting, degree_setting))
                    l_field_free_pars.append('scale_%.3f_kV_%d_deg' % (cathode_setting, degree_setting))
            
            free_par_per_field = 5
            free_par_per_degree_field = 2
            
            l_par_names = ['w_value', 'kappa', 'intrinsic_res_s1', 'intrinsic_res_s2', 'g1_value', 'spe_res_value', 'extraction_efficiency_value', 'gas_gain_mean_value', 'gas_gain_width_value', 'pf_eff_par0', 'pf_eff_par1', 's2_eff_par0', 's2_eff_par1', 'pf_stdev_par0', 'pf_stdev_par1', 'pf_stdev_par2', 'dpe_prob'] + l_field_free_pars
            
            
        elif self.fit_type[:4] == 'mlti':
            
            l_field_free_pars = []
            for cathode_setting in self.l_cathode_settings_in_use:
                l_field_free_pars += ['ti_par_%.3f_kV' % (cathode_setting), 'exciton_to_ion_ratio_%.3f_kV' % (cathode_setting)]
                
                for degree_setting in self.l_degree_settings_in_use:
                    if degree_setting != -4:
                        l_field_free_pars.append('prob_bkg_%.3f_kV_%d_deg' % (cathode_setting, degree_setting))
                    
                    l_field_free_pars.append('scale_%.3f_kV_%d_deg' % (cathode_setting, degree_setting))
            
            free_par_per_field = 2
            free_par_per_degree_field = 2
            
            l_par_names = ['w_value', 'beta', 'kappa', 'eta', 'lambda', 'g1_value', 'spe_res_value', 'extraction_efficiency_value', 'gas_gain_mean_value', 'gas_gain_width_value', 'pf_eff_par0', 'pf_eff_par1', 's2_eff_par0', 's2_eff_par1', 'pf_stdev_s1_par0', 'pf_stdev_s1_par1', 'pf_stdev_s1_par2', 'pf_stdev_s2_par0', 'pf_stdev_s2_par1', 'pf_stdev_s2_par2', 'dpe_prob'] + l_field_free_pars
            

        d_variable_arrays = {}
        d_stdevs = {}
        if self.fit_type == 'm' or self.fit_type == 'mb':
            num_yields = 2*len(self.a_spline_energies)
        elif self.fit_type == 's':
            num_yields = 1
        elif self.fit_type == 'ml':
            num_yields = 9


        # position array should be (num_walkers, num_dim)

        """
        g1_value, g2_value = self.l_means_g1_g2
        gas_gain_rv = 0
        gas_gain_width_rv = 0
        spe_res_rv = 0
        pf_eff_par0, pf_eff_par1 = self.l_means_pf_eff_pars
        pf_stdev_par0, pf_stdev_par1, pf_stdev_par2 = self.l_means_pf_stdev_pars
        exciton_to_ion_par0_rv, exciton_to_ion_par1_rv, exciton_to_ion_par2_rv = 0, 0, 0
        """

        count_bkg_pars = 0
        count_scale_pars = 0
        count_etoi_pars = 0
        count_gompertz_pars = 0
        count_gompertz_fields = 0

        for par_name in l_par_names:
            # handle photon and charge yield initial positions
            if par_name == 'a_py':
                d_variable_arrays[par_name] = np.asarray([np.random.normal(a_free_parameters[i], 0.3*np.asarray(a_free_parameters[i]), size=num_walkers) for i in xrange(num_yields)])

            elif par_name == 'a_qy':
                d_variable_arrays[par_name] = np.asarray([np.random.normal(a_free_parameters[i], 0.3*np.asarray(a_free_parameters[i]), size=num_walkers) for i in xrange(num_yields, 2*num_yields)])
        
            elif par_name == 'py':
                d_variable_arrays[par_name] = np.asarray([np.random.normal(a_free_parameters[count_free_pars], 0.05*np.asarray(a_free_parameters[count_free_pars]), size=num_walkers) for i in xrange(num_yields)])
                count_free_pars += 1

            elif par_name == 'qy':
                d_variable_arrays[par_name] = np.asarray([np.random.normal(a_free_parameters[count_free_pars], 0.05*np.asarray(a_free_parameters[count_free_pars]), size=num_walkers) for i in xrange(num_yields, 2*num_yields)])
                count_free_pars += 1

            elif par_name == 'w_value_rv':
                # special case since w_value is an RV
                d_variable_arrays[par_name] = np.random.normal(a_free_parameters[0], 1, size=num_walkers)
            
            elif par_name == 'w_value':
                d_variable_arrays[par_name] = np.random.normal(self.w_value, self.w_value_uncertainty, size=num_walkers)

            elif par_name == 'alpha':
                d_variable_arrays[par_name] = np.random.normal(a_free_parameters[1], a_free_parameters[1]*0.3, size=num_walkers)

            elif par_name == 'zeta':
                d_variable_arrays[par_name] = np.random.normal(a_free_parameters[2], a_free_parameters[2]*0.3, size=num_walkers)

            elif par_name == 'beta':
                if self.fit_type[:4] == 'mlti':
                    d_variable_arrays[par_name] = np.random.normal(a_free_parameters[count_free_pars], a_free_parameters[count_free_pars]*0.3, size=num_walkers)
                    count_free_pars += 1
                else:
                    d_variable_arrays[par_name] = np.random.normal(a_free_parameters[3], a_free_parameters[3]*0.3, size=num_walkers)

            elif par_name == 'gamma':
                d_variable_arrays[par_name] = np.random.normal(a_free_parameters[4], a_free_parameters[4]*0.3, size=num_walkers)

            elif par_name == 'delta':
                d_variable_arrays[par_name] = np.random.normal(a_free_parameters[5], a_free_parameters[5]*0.3, size=num_walkers)

            elif par_name == 'kappa':
                if self.fit_type[:4] == 'mlti':
                    d_variable_arrays[par_name] = np.random.normal(a_free_parameters[count_free_pars], a_free_parameters[count_free_pars]*0.3, size=num_walkers)
                    count_free_pars += 1
                
                elif self.fit_type == 'bl' or self.fit_type[:2] == 'ml':
                    d_variable_arrays[par_name] = np.random.normal(a_free_parameters[0], a_free_parameters[0]*0.3, size=num_walkers)
                    count_free_pars += 1
                else:
                    d_variable_arrays[par_name] = np.random.normal(a_free_parameters[6], a_free_parameters[6]*0.3, size=num_walkers)


            elif par_name == 'eta':
                if self.fit_type[:4] == 'mlti':
                    d_variable_arrays[par_name] = np.random.normal(a_free_parameters[count_free_pars], a_free_parameters[count_free_pars]*0.3, size=num_walkers)
                    count_free_pars += 1
                
                else:
                    d_variable_arrays[par_name] = np.random.normal(a_free_parameters[7], a_free_parameters[7]*0.3, size=num_walkers)

            elif par_name == 'lambda':
                if self.fit_type[:4] == 'mlti':
                    d_variable_arrays[par_name] = np.random.normal(a_free_parameters[count_free_pars], a_free_parameters[count_free_pars]*0.3, size=num_walkers)
                    count_free_pars += 1
            
                else:
                    d_variable_arrays[par_name] = np.random.normal(a_free_parameters[8], a_free_parameters[8]*0.3, size=num_walkers)

            elif par_name == 'intrinsic_res_s1' or par_name == 'band_res_s1':
                if self.fit_type[:4] == 'mlti':
                    d_variable_arrays[par_name] = np.random.normal(a_free_parameters[count_free_pars], 0.04, size=num_walkers)
                    count_free_pars += 1
                elif self.fit_type == 's':
                    d_variable_arrays[par_name] = np.random.normal(a_free_parameters[count_free_pars], .04, size=num_walkers)
                    count_free_pars += 1
                elif self.fit_type == 'bl' or self.fit_type[:2] == 'ml':
                    d_variable_arrays[par_name] = np.random.normal(a_free_parameters[1], .04, size=num_walkers)
                    count_free_pars += 1
                    
            elif par_name[0:6] == 'pf_res':
                d_variable_arrays[par_name] = np.random.normal(a_free_parameters[count_free_pars], .04, size=num_walkers)
                count_free_pars += 1
            

            elif par_name == 'intrinsic_res_s2' or par_name == 'band_res_s2':
                if self.fit_type[:4] == 'mlti':
                    d_variable_arrays[par_name] = np.random.normal(a_free_parameters[count_free_pars], 0.04, size=num_walkers)
                    count_free_pars += 1
                elif self.fit_type == 's':
                    d_variable_arrays[par_name] = np.random.normal(a_free_parameters[count_free_pars], .04, size=num_walkers)
                    count_free_pars += 1
                elif self.fit_type == 'bl' or self.fit_type[:2] == 'ml':
                    d_variable_arrays[par_name] = np.random.normal(a_free_parameters[2], .04, size=num_walkers)
                    count_free_pars += 1


            elif par_name == 'g1_value':
                d_variable_arrays['g1_value'] = np.random.multivariate_normal([self.g1_value], [[self.g1_uncertainty**2 * 0.2]], size=num_walkers).T
                
                
            elif par_name == 'extraction_efficiency_value':
                d_variable_arrays['extraction_efficiency_value'] = np.random.multivariate_normal([self.extraction_efficiency_value], [[self.extraction_efficiency_uncertainty**2 * 0.2]], size=num_walkers).T
            
            
            elif par_name == 'spe_res_value':
                d_variable_arrays['spe_res_value'] = np.random.multivariate_normal([self.spe_res_value], [[self.spe_res_uncertainty**2 * 0.2]], size=num_walkers).T
                
            
            elif par_name == 'gas_gain_mean_value':
                d_variable_arrays['gas_gain_mean_value'] = np.random.multivariate_normal([self.gas_gain_value], [[self.gas_gain_uncertainty**2 * 0.2]], size=num_walkers).T
            
            
            elif par_name == 'gas_gain_width_value':
                d_variable_arrays['gas_gain_width_value'] = np.random.multivariate_normal([self.gas_gain_width], [[self.gas_gain_width_uncertainty**2 * 0.2]], size=num_walkers).T


            # catch both parameters of s1 efficiency prior
            elif par_name == 'pf_eff_par0':
                d_variable_arrays['pf_eff_par0'], d_variable_arrays['pf_eff_par1'] = np.random.multivariate_normal(self.l_means_pf_eff_pars, self.l_cov_matrix_pf_eff_pars, size=num_walkers).T


            elif par_name == 's1_eff_par0':
                d_variable_arrays['s1_eff_par0'], d_variable_arrays['s1_eff_par1'] = np.asarray([np.random.normal(a_free_parameters[i], 0.3*np.asarray(a_free_parameters[i]), size=num_walkers) for i in xrange(2*num_yields+2, 2*num_yields+4)])
                #d_variable_arrays[par_name] = np.random.normal(a_free_parameters[2*num_yields+2:2*num_yields+4], 0.3*np.asarray(a_free_parameters[2*num_yields+2:2*num_yields+4]), size=num_walkers)


            elif par_name == 's2_eff_par0':
                d_variable_arrays['s2_eff_par0'], d_variable_arrays['s2_eff_par1'] = np.random.multivariate_normal(self.l_means_s2_eff_pars, self.l_cov_matrix_s2_eff_pars, size=num_walkers).T


            elif par_name[:8] == 'pf_stdev':
                d_variable_arrays[par_name] = np.random.normal(a_free_parameters[count_free_pars], a_free_parameters[count_free_pars]*0.2, size=num_walkers)
                count_free_pars += 1


            elif par_name == 'dpe_prob':
                # values taken from DPE paper
                d_variable_arrays['dpe_prob'] = np.random.uniform(0.17, 0.24, size=num_walkers).T
                
            
            elif par_name[:8] == 'prob_bkg':
                if self.fit_type[:2] == 'ml':
                    d_variable_arrays[par_name] = np.random.normal(a_free_parameters[count_free_pars], .02, size=num_walkers)
                    count_free_pars += 1
                
                elif self.fit_type[:2] == 's':
                    d_variable_arrays[par_name] = np.random.normal(a_free_parameters[count_free_pars], .02, size=num_walkers)
                    count_free_pars += 1
                
                else:
                    d_variable_arrays[par_name] = np.random.normal(a_free_parameters[-2*(len(self.l_cathode_settings_in_use)*len(self.l_degree_settings_in_use)) + count_bkg_pars], 0.1, size=num_walkers)
                    count_bkg_pars += 1

                

            elif par_name[:5] == 'scale':
                # need to track scale parameters
                # always the last indices so move back by
                # number of degree settings multiplied by
                # number of cathode settings
                if self.fit_type[:4] == 'mlti':
                    d_variable_arrays[par_name] = np.random.normal(a_free_parameters[count_free_pars], 0.25, size=num_walkers)
                    count_free_pars += 1
                
                elif self.fit_type == 's':
                    d_variable_arrays[par_name] = np.random.normal(a_free_parameters[count_free_pars], 0.25, size=num_walkers)
                    count_free_pars += 1
                
                elif self.fit_type == 'ml':
                    d_variable_arrays[par_name] = np.random.normal(a_free_parameters[count_free_pars], .05, size=num_walkers)
                    count_free_pars += 1
                    
                    
                else:
                    d_variable_arrays[par_name] = np.random.normal(a_free_parameters[-(len(self.l_cathode_settings_in_use)*len(self.l_degree_settings_in_use)*free_par_per_field) + 5 + count_scale_pars*free_par_per_field], .05, size=num_walkers)
                
                    count_scale_pars += 1


            elif par_name[:7] == 'exciton':
                # need to track scale parameters
                # always the last indices so move back by
                # number of degree settings multiplied by
                # number of cathode settings
                if self.fit_type[:4] == 'mlti':
                    d_variable_arrays[par_name] = np.random.normal(a_free_parameters[count_free_pars], 0.1, size=num_walkers)
                    count_free_pars += 1
                
                elif self.fit_type == 'bl':
                    d_variable_arrays[par_name] = np.random.normal(a_free_parameters[-(len(self.l_cathode_settings_in_use)*len(self.l_degree_settings_in_use)*free_par_per_field) + 4 + count_etoi_pars*free_par_per_field], .1, size=num_walkers)
                    
                elif self.fit_type == 's':
                    d_variable_arrays[par_name] = np.random.normal(0, 1, size=num_walkers)

                elif self.fit_type[:2] == 'ml':
                    d_variable_arrays[par_name] = np.random.normal(a_free_parameters[count_free_pars], .1, size=num_walkers)
                    count_free_pars += 1
                
                count_etoi_pars += 1


            elif par_name[:7] == 'gom_par':
                # need to track scale parameters
                # always the last indices so move back by
                # number of degree settings multiplied by
                # number of cathode settings
                gompertz_par_number = int(par_name[7])
                
                if self.fit_type == 'bl':
                    temp_val = a_free_parameters[-(len(self.l_cathode_settings_in_use)*len(self.l_degree_settings_in_use)*free_par_per_field) + gompertz_par_number + count_gompertz_fields*free_par_per_field]
                    d_variable_arrays[par_name] = np.random.normal(temp_val, temp_val*0.3, size=num_walkers)
                    
                    count_gompertz_pars += 1
                    if count_gompertz_pars % 4 == 0:
                        count_gompertz_fields += 1

                elif self.fit_type[:2] == 'ml':
                    temp_val = a_free_parameters[count_free_pars]
                    d_variable_arrays[par_name] = np.random.normal(temp_val, temp_val*0.3, size=num_walkers)
                    count_free_pars += 1
        
        
            elif par_name[:6] == 'ti_par':
                # need to track scale parameters
                # always the last indices so move back by
                # number of degree settings multiplied by
                # number of cathode settings
                
                temp_val = a_free_parameters[count_free_pars]
                d_variable_arrays[par_name] = np.random.normal(temp_val, temp_val*0.3, size=num_walkers)
                count_free_pars += 1


            # catch all normally distributed RVs
            else:
                if par_name == 'g2_value' or par_name == 'pf_eff_par1' or par_name == 's1_eff_par1' or par_name == 's2_eff_par1'  or par_name == 'pf_stdev_par1' or par_name == 'pf_stdev_par2':
                    continue
                d_variable_arrays[par_name] = np.random.randn(num_walkers)


        l_parameters = []

        for par_name in l_par_names:
            if d_variable_arrays[par_name].shape[0] != num_walkers:
                for array in d_variable_arrays[par_name]:
                    l_parameters.append(array)
            else:
                l_parameters.append(d_variable_arrays[par_name])

        l_parameters = np.asarray(l_parameters).T

        return l_parameters




    def run_mcmc(self, num_steps, num_walkers, num_threads=1, thin=1):

        # get string for cathode voltages in use
        s_cathode_voltages = ''
        for cathode_setting in self.l_cathode_settings_in_use:
            s_cathode_voltages += '%s,' % (cathode_setting)
        s_cathode_voltages = s_cathode_voltages[:-1]

        # get string for degree settings in use
        s_degree_settings = ''
        for degree_setting in self.l_degree_settings_in_use:
            s_degree_settings += '%s,' % (degree_setting)
        s_degree_settings = s_degree_settings[:-1]

        # before emcee, setup save locations
        dir_specifier_name = '%s_kV_%s_deg' % (s_cathode_voltages, s_degree_settings)
        self.results_directory_name = neriX_simulation_config.results_directory_name
        self.path_for_save = '%s/%s/%s/' % (self.results_directory_name, self.directory_name, dir_specifier_name)


        if not os.path.isdir(self.path_for_save):
            os.makedirs(self.path_for_save)


        # chain dictionary will have the following format
        # d_sampler[walkers] = [sampler_000, sampler_001, ...]

        dict_filename = 'sampler_dictionary.p'
        acceptance_filename = 'acceptance_fraction.p'
        autocorrelation_filename = 'autocorrelation.p'


        loaded_prev_sampler = False
        try:
            # two things will fail potentially
            # 1. open if file doesn't exist
            # 2. posWalkers load since initialized to None

            with open(self.path_for_save + dict_filename, 'r') as f_prev_sampler:

                d_sampler = pickle.load(f_prev_sampler)
                prevSampler = d_sampler[num_walkers][-1]


                # need to load in weird way bc can't pickle
                # ensembler object
                starting_pos = prevSampler['_chain'][:,-1,:]
                random_state = prevSampler['_random']
            loaded_prev_sampler = True
            print '\nSuccessfully loaded previous chain!\n'
        except:
            print '\nCould not load previous sampler or none existed - starting new sampler.\n'

        if not loaded_prev_sampler:

            if self.fit_type == 's':
                degree_setting = self.l_degree_settings_in_use[0]
                cathode_setting = self.l_cathode_settings_in_use[0]
                
                # 3000, 345: [9.42934482, 6.80825196, 0.17586548, 0.19511261, 0.47934021, 0.22603184, 0.18254204, 2.35914599]
                # 3000, 1054: [7.25966214, 6.49163371, 0.04033762, 0.14759035, 0.64133758, 0.25714344, 0.35668657, 2.59558031]
                # 3000, 2356: [8.48228266, 6.45346487, 0.14723975, 0.33379526, 0.465922, 0.04439688, 0.20463928, 2.16469687]
                
                # 3500, 345: [8.61340613, 5.98094083, 0.04387717, 0.02617817, 0.24818277, 0.211507, 0.12784283, 1.78493792]
                # 3500, 1054: [8.23064289, 5.78370797, 0.03284778, 0.22921785, 0.28779962, 0.08552447, 0.0198607, 1.82238142]
                # 3500, 2356: [8.34192553, 5.93691378, 0.12957123, 0.02833529, 0.16640535, 0.16816157, 0.10335449, 1.75901753]
                
                # 4500, 345: [8.44539528, 5.72455525, 0.03460704, 0.10541354, 0.29660492, 0.03163016, 0.33665642, 1.72612264]
                # 4500, 1054: 8.54604476, 5.70928189, 0.28809291, 0.04366459, 0.04548912, 0.06554624, 0.00995299, 1.41115711
                # 4500, 2356: [8.52197558, 5.8022725, 0.03320323, 0.02104382, 0.20920254, 0.07357184, 0.13532101, 1.47980755]
                
                # 5300, 345: [10.65639391, 6.00171995, 0.21318309, 0.04482494, 0.15244351, 0.02067115, 0.02876731, 1.41288691]
                # 5300, 1054: [1.03030254e+01, 5.97495921e+00, 1.74354451e-01, 1.23981905e-02, 2.53512778e-01, 1.67677835e-02, 4.90220849e-03, 1.42171689e+00]
                # 5300, 2356: [10.04609711, 6.2544237, 0.31215843, 0.04122312, 0.01038293, 0.01665143, 0.02820888, 1.31779608]
            
                if degree_setting == 2300:
                    if cathode_setting == 0.345:
                        a_free_parameter_guesses = [ 7.24279474,  6.10423574,  0.06494089,  0.30873822, 0.25022568,  0.99936306]
                        # [ 7.24279474,  6.10423574,  0.06494089,  0.30873822, 0.25022568,  0.99936306] # 2300
                    elif cathode_setting == 1.054:
                        a_free_parameter_guesses = [7.04, 5.68, 0.23, 0.12, 0.29, 0.97]
                        # [ 7.04498957,  5.6815345,  0.23072485,  0.11705023, 0.28815432,  0.97119736] # 2300
                        
                    elif cathode_setting == 2.356:
                        a_free_parameter_guesses = [7.79, 6.27, 0.19, 0.18, 0.16, 0.97]
                        # [ 8.21997358,  6.06919105,  0.06841404 ,  0.46676469,  0.14950445, 0.98737901] # 2300
                        
                elif degree_setting == 3000:
                    if cathode_setting == 0.345:
                        a_free_parameter_guesses = [9.42934482, 6.80825196, 0.17586548, 0.19511261, 0.47934021, 0.22603184, 0.18254204, 2.35914599]
                
                    elif cathode_setting == 1.054:
                        a_free_parameter_guesses = [7.25966214, 6.49163371, 0.04033762, 0.14759035, 0.64133758, 0.25714344, 0.35668657, 2.59558031]
                        
                    elif cathode_setting == 2.356:
                        a_free_parameter_guesses = [8.48228266, 6.45346487, 0.14723975, 0.33379526, 0.465922, 0.04439688, 0.20463928, 2.16469687]
                        
                elif degree_setting == 3500:
                    if cathode_setting == 0.345:
                        a_free_parameter_guesses = [8.61340613, 5.98094083, 0.04387717, 0.02617817, 0.24818277, 0.211507, 0.12784283, 1.78493792]
                
                    elif cathode_setting == 1.054:
                        a_free_parameter_guesses = [8.23064289, 5.78370797, 0.03284778, 0.22921785, 0.28779962, 0.08552447, 0.0198607, 1.82238142]
                        
                    elif cathode_setting == 2.356:
                        a_free_parameter_guesses = [8.34192553, 5.93691378, 0.12957123, 0.02833529, 0.16640535, 0.16816157, 0.10335449, 1.75901753]
                        
                elif degree_setting == 4500:
                    if cathode_setting == 0.345:
                        a_free_parameter_guesses = [8.44539528, 5.72455525, 0.03460704, 0.10541354, 0.29660492, 0.03163016, 0.33665642, 1.72612264]
                
                    elif cathode_setting == 1.054:
                        a_free_parameter_guesses = [8.54604476, 5.70928189, 0.28809291, 0.04366459, 0.04548912, 0.06554624, 0.00995299, 1.41115711]
                        
                    elif cathode_setting == 2.356:
                        a_free_parameter_guesses = [8.52197558, 5.8022725, 0.03320323, 0.02104382, 0.20920254, 0.07357184, 0.13532101, 1.47980755]
                    
                elif degree_setting == 5300:
                    if cathode_setting == 0.345:
                        a_free_parameter_guesses = [10.65639391, 6.00171995, 0.21318309, 0.04482494, 0.15244351, 0.02067115, 0.02876731, 1.41288691]
                    
                    elif cathode_setting == 1.054:
                        a_free_parameter_guesses = [1.03030254e+01, 5.97495921e+00, 1.74354451e-01, 1.23981905e-02, 2.53512778e-01, 1.67677835e-02, 4.90220849e-03, 1.42171689e+00]
                    
                    elif cathode_setting == 2.356:
                        a_free_parameter_guesses = [10.04609711, 6.2544237, 0.31215843, 0.04122312, 0.01038293, 0.01665143, 0.02820888, 1.31779608]


                else:
                    a_free_parameter_guesses = [8., 6., 0.25, 0.25, 0.95]

            elif self.fit_type == 'bl':
                a_free_parameter_guesses = [0.09581742, 0.17142747, 0.19346854, 0.10765697, 0.16335862, 22.90365074, 0.0359913 , 1.13402335, 0.77952878, 0.11695468, 0.16992129, 11.77422521, 0.03032592, 1.04006065, 0.78162736, 0.09716007, 0.19966872, 29.70498045, 0.04494912, 1.06974509, 0.82369163]
                
            elif self.fit_type == 'ml':
                a_free_parameter_guesses = [1.50570624e-01, 1.73586063e-01, 1.20784480e-01, 8.28728080e-02, 2.50685523e-01, 3.47215755e+01, 2.97646444e-02, 1.17420176e+00, 3.29941916e-01, 8.60177855e-01, 1.37438771e-01, 8.81837797e-01, 2.82658988e-01, 7.59336611e-01, 3.93830317e-01, 9.75755515e-01, 4.09473409e-02, 1.83664545e-01, 3.48234408e+01, 3.78743406e-02, 1.29242185e+00, 1.18262763e-01, 7.30735165e-01, 2.50593149e-01, 7.81316055e-01, 3.63693355e-01, 9.32919132e-01, 3.13526304e-01, 8.52441265e-01, 5.72525297e-02, 2.44435244e-01, 3.42007248e+01, 2.61562638e-02, 1.27534975e+00, 1.26479405e-01, 8.01497954e-01, 2.27479810e-01, 8.70813328e-01, 3.94553333e-01, 9.02541013e-01, 3.48684210e-01, 9.43474342e-01]
            
            elif self.fit_type[:4] == 'mlti':
                a_free_parameter_guesses = [3.22182010e+03, 1.56037068e-01, 3.05109850e+00, 1.16001614e+00, 1.26899592e-01, 9.72338277e-01, 2.20896740e+00, 1.24605540e-01, 2.00780510e-01, 1.07290240e+03, 5.76968344e-03, 1.23491295e+00, 2.19176568e+00, 3.09352209e-01, 2.35900361e+00, 1.00254241e-01, 2.15905404e+00, 6.01790172e-03, 2.26503500e+00, 1.60397675e-01, 1.19741699e+00, 6.72672821e-03, 1.33180979e+00, 2.22957542e+00, 3.58497476e-01, 2.65224391e+00, 2.35506919e-01, 2.20839990e+00, 1.70133070e-01, 1.73809979e+00, 3.71251968e-01, 1.70512637e+00, 8.02725402e-03, 9.16496586e-01, 1.62461513e+00, 3.18310422e-01, 2.07738773e+00, 1.25221404e-01, 2.51620240e+00, 1.96360030e-01, 1.35731466e+00, 1.71761316e-01, 2.03880016e+00]
            
            else:
                print '\nPlease run differential evolution minimizer for this setup and implement results in source code.\n'
                sys.exit()


            starting_pos = self.initial_positions_for_ensemble(a_free_parameter_guesses, num_walkers=num_walkers)
            #print list(starting_pos[0])

            random_state = None

            # create file if it doesn't exist
            if not os.path.exists(self.path_for_save + dict_filename):
                with open(self.path_for_save + dict_filename, 'w') as f_prev_sampler:
                    d_sampler = {}

                    d_sampler[num_walkers] = []

                    pickle.dump(d_sampler, f_prev_sampler)
            else:
                with open(self.path_for_save + dict_filename, 'r') as f_prev_sampler:
                    d_sampler = pickle.load(f_prev_sampler)
                with open(self.path_for_save + dict_filename, 'w') as f_prev_sampler:

                    d_sampler[num_walkers] = []

                    pickle.dump(d_sampler, f_prev_sampler)




        #sampler = emcee.EnsembleSampler(num_walkers, self.num_dimensions, self.wrapper_ln_likelihood_coincidence_matching, a=proposal_scale, threads=num_threads, pool=self.gpu_pool, kwargs={})
        if self.fit_type == 's' or self.fit_type == 'bl' or self.fit_type[:2] == 'ml':
            num_dim = self.num_dimensions
        elif self.fit_type == 'm':
            num_dim = self.num_dimensions - 2 + 2*len(self.a_spline_energies)
        sampler = emcee.DESampler(num_walkers, num_dim, self.ln_likelihood_function_wrapper, threads=num_threads, autoscale_gamma=True, pool=self.gpu_pool, kwargs={})



        print '\n\nBeginning MCMC sampler\n\n'

        print '\nNumber of walkers * number of steps = %d * %d = %d function calls\n' % (num_walkers, num_steps, num_walkers*num_steps)

        start_time_mcmc = time.time()
        sleep_timer = time.time()
        sleep_time = 3600. # s

        with click.progressbar(sampler.sample(p0=starting_pos, iterations=num_steps, rstate0=random_state, thin=thin), length=num_steps) as mcmc_sampler:
            for i, l_iterator_values in enumerate(mcmc_sampler):
                # after "sleep_time" passes, rest for a minute
                # to let GPUs cool down
                if time.time() - sleep_timer > sleep_time:
                    sleep_timer = time.time()
                    time.sleep(60)
                    print '\nResting GPUs for a minute...\n'
                # should NOT refresh suppresion - screws up acceptance
                # since scale is constantly changing
                """
                if (i != 0 and (i % 25) == 0) or (i == 3):
                    index_max_flattened = np.argmax(sampler.lnprobability[:, :i].flatten())
                    flat_chain = sampler.chain[:, :i, :].reshape(-1, num_dim)
                    self.suppress_likelihood(iterations=200, a_free_par_guesses=flat_chain[index_max_flattened, :])
                """

        total_time_mcmc = (time.time() - start_time_mcmc) / 3600.

        print '\n\n%d function calls took %.2f hours.\n\n' % (num_walkers*num_steps, total_time_mcmc)

        #samples = sampler.chain[:, 10:, :].reshape((-1, num_dim))
        #print samples

        #fig = corner.corner(samples)
        #fig.savefig(self.path_for_save + 'corner_plot.png')


        # ------------------------------------------------
        # Prepare and save data and plots
        # ------------------------------------------------

        #print sampler.__dict__
        dictionary_for_sampler = sampler.__dict__
        if 'lnprobfn' in dictionary_for_sampler:
            del dictionary_for_sampler['lnprobfn']
        if 'pool' in dictionary_for_sampler:
            del dictionary_for_sampler['pool']

        with open(self.path_for_save + dict_filename, 'r') as f_prev_sampler:
            d_sampler = pickle.load(f_prev_sampler)
        #f_prev_sampler.close()

        f_prev_sampler = open(self.path_for_save + dict_filename, 'w')

        d_sampler[num_walkers].append(sampler.__dict__)

        pickle.dump(d_sampler, f_prev_sampler)
        f_prev_sampler.close()



        #sampler.run_mcmc(posWalkers, numSteps) # shortcut of above method
        pickle.dump(sampler.acceptance_fraction, open(self.path_for_save + acceptance_filename, 'w'))
        try:
            pickle.dump(sampler.acor, open(self.path_for_save + autocorrelation_filename, 'w'))
        except:
            print '\n\nWARNING: Not enough sample points to estimate the autocorrelation time - this likely means that the fit is bad since the burn-in time was not reached.\n\n'



    def differential_evolution_minimizer_free_pars(self, a_bounds, maxiter=250, tol=0.05, popsize=15, polish=False):
        def neg_log_likelihood_diff_ev(a_guesses):
            if self.fit_type == 's':
                a_guesses = list(a_guesses[:4]) + [test.g1_value, test.spe_res_value, test.extraction_efficiency_value, test.gas_gain_value, test.gas_gain_width, test.l_means_pf_eff_pars[0], test.l_means_pf_eff_pars[1], test.l_means_s2_eff_pars[0], test.l_means_s2_eff_pars[1], a_guesses[4], a_guesses[5], 0, 0, 0] + list(a_guesses[-2:])
                return -self.gpu_pool.map(self.wrapper_ln_likelihood_full_matching_single_energy, [a_guesses])[0]
        
        
            elif self.fit_type == 'bl':
                a_guesses = [13.7] + list(a_guesses[:3]) + [test.g1_value, test.spe_res_value, test.extraction_efficiency_value, test.gas_gain_value, test.gas_gain_width, test.l_means_pf_eff_pars[0], test.l_means_pf_eff_pars[1], test.l_means_s2_eff_pars[0], test.l_means_s2_eff_pars[1], test.l_means_pf_stdev_pars[0], test.l_means_pf_stdev_pars[1], test.l_means_pf_stdev_pars[2], 0.20] + list(a_guesses[-len(self.l_cathode_settings_in_use)*6:])
                return -self.gpu_pool.map(self.wrapper_ln_likelihood_full_matching_band, [a_guesses])[0]
                
            
            elif self.fit_type == 'ml':
                a_guesses = [13.7] + list(a_guesses[:3]) + [test.g1_value, test.spe_res_value, test.extraction_efficiency_value, test.gas_gain_value, test.gas_gain_width, test.l_means_pf_eff_pars[0], test.l_means_pf_eff_pars[1], test.l_means_s2_eff_pars[0], test.l_means_s2_eff_pars[1], test.l_means_pf_stdev_pars[0], test.l_means_pf_stdev_pars[1], test.l_means_pf_stdev_pars[2], 0.20] + list(a_guesses[-(len(self.l_cathode_settings_in_use)*5 + len(self.l_cathode_settings_in_use)*len(self.l_degree_settings_in_use)*2):])
                return -self.gpu_pool.map(self.wrapper_ln_likelihood_full_matching_multiple_energies_lindhard_model, [a_guesses])[0]
                
                
            elif self.fit_type[:4] == 'mlti':
                a_guesses = [13.7] + list(a_guesses[:4]) + [test.g1_value, test.spe_res_value, test.extraction_efficiency_value, test.gas_gain_value, test.gas_gain_width, test.l_means_pf_eff_pars[0], test.l_means_pf_eff_pars[1], test.l_means_s2_eff_pars[0], test.l_means_s2_eff_pars[1]] + list(a_guesses[4:10]) + [0.20] + list(a_guesses[-(len(self.l_cathode_settings_in_use)*2 + len(self.l_cathode_settings_in_use)*(len(self.l_degree_settings_in_use)*2-1)):])
                return -self.gpu_pool.map(self.wrapper_ln_likelihood_full_matching_multiple_energies_lindhard_model_with_ti_recombination, [a_guesses])[0]
            
                
                
            elif self.fit_type == 'test':
                
                # TI
                if self.test_switch == 'ti':
                    num_recombination_pars = 1
                elif self.test_switch == 'pol1':
                    num_recombination_pars = 2
                elif self.test_switch == 'pol2':
                    num_recombination_pars = 3
                elif self.test_switch == 'pol3':
                    num_recombination_pars = 4
                elif self.test_switch == 'pol4':
                    num_recombination_pars = 5
                elif self.test_switch == 'pol5':
                    num_recombination_pars = 6
                elif self.test_switch == 'mod_gom':
                    num_recombination_pars = 4
                
                a_guesses = [13.7] + list(a_guesses[:6]) + [test.g1_value, test.spe_res_value, test.extraction_efficiency_value, test.gas_gain_value, test.gas_gain_width, test.l_means_pf_eff_pars[0], test.l_means_pf_eff_pars[1], test.l_means_s2_eff_pars[0], test.l_means_s2_eff_pars[1], test.l_means_pf_stdev_pars[0], test.l_means_pf_stdev_pars[1], test.l_means_pf_stdev_pars[2], 0.20] + list(a_guesses[-(len(self.l_cathode_settings_in_use)*(1+num_recombination_pars) + len(self.l_cathode_settings_in_use)*(len(self.l_degree_settings_in_use)*2-1)):])
                return -self.gpu_pool.map(self.wrapper_ln_likelihood_test, [a_guesses])[0]
            
        print '\n\nStarting differential evolution minimizer...\n\n'
        result = optimize.differential_evolution(neg_log_likelihood_diff_ev, a_bounds, disp=True, maxiter=maxiter, tol=tol, popsize=popsize, polish=polish)
        print result
        
        




    def suppress_likelihood(self, iterations=200, a_free_par_guesses=None):
    
        # reset variables in case this is not the first time run
        self.b_suppress_likelihood = False
        self.ll_suppression_factor = 1.

        if self.fit_type == 's' and a_free_par_guesses == None:
        
            degree_setting = self.l_degree_settings_in_use[0]
            cathode_setting = self.l_cathode_settings_in_use[0]
            
            
            # 3000, 345: [9.42934482, 6.80825196, 0.17586548, 0.19511261, 0.47934021, 0.22603184, 0.18254204, 2.35914599]
            # 3000, 1054: [7.25966214, 6.49163371, 0.04033762, 0.14759035, 0.64133758, 0.25714344, 0.35668657, 2.59558031]
            # 3000, 2356: [8.48228266, 6.45346487, 0.14723975, 0.33379526, 0.465922, 0.04439688, 0.20463928, 2.16469687]
            
            # 3500, 345: [8.61340613, 5.98094083, 0.04387717, 0.02617817, 0.24818277, 0.211507, 0.12784283, 1.78493792]
            # 3500, 1054: [8.23064289, 5.78370797, 0.03284778, 0.22921785, 0.28779962, 0.08552447, 0.0198607, 1.82238142]
            # 3500, 2356: [8.34192553, 5.93691378, 0.12957123, 0.02833529, 0.16640535, 0.16816157, 0.10335449, 1.75901753]
            
            # 4500, 345: [8.44539528, 5.72455525, 0.03460704, 0.10541354, 0.29660492, 0.03163016, 0.33665642, 1.72612264]
            # 4500, 1054: 8.54604476, 5.70928189, 0.28809291, 0.04366459, 0.04548912, 0.06554624, 0.00995299, 1.41115711
            # 4500, 2356: [8.52197558, 5.8022725, 0.03320323, 0.02104382, 0.20920254, 0.07357184, 0.13532101, 1.47980755]
            
            # 5300, 345: [10.65639391, 6.00171995, 0.21318309, 0.04482494, 0.15244351, 0.02067115, 0.02876731, 1.41288691]
            # 5300, 1054: [1.03030254e+01, 5.97495921e+00, 1.74354451e-01, 1.23981905e-02, 2.53512778e-01, 1.67677835e-02, 4.90220849e-03, 1.42171689e+00]
            # 5300, 2356: [10.04609711, 6.2544237, 0.31215843, 0.04122312, 0.01038293, 0.01665143, 0.02820888, 1.31779608]
            
            if degree_setting == 2300 and cathode_setting == 0.345:
                l_free_pars = [7.24, 6.10, 0.07, 0.31, 0.25, 0.99]
            elif degree_setting == 2300 and cathode_setting == 1.054:
                l_free_pars = [7.04, 5.68, 0.23, 0.12, 0.29, 0.97]
            elif degree_setting == 2300 and cathode_setting == 2.356:
                l_free_pars = [7.79, 6.27, 0.19, 0.18, 0.16, 0.97]
            
            elif degree_setting == 3000 and cathode_setting == 0.345:
                l_free_pars = [9.42934482, 6.80825196, 0.17586548, 0.19511261, 0.47934021, 0.22603184, 0.18254204, 2.35914599]
            elif degree_setting == 3000 and cathode_setting == 1.054:
                l_free_pars = [7.25966214, 6.49163371, 0.04033762, 0.14759035, 0.64133758, 0.25714344, 0.35668657, 2.59558031]
            elif degree_setting == 3000 and cathode_setting == 2.356:
                l_free_pars = [8.48228266, 6.45346487, 0.14723975, 0.33379526, 0.465922, 0.04439688, 0.20463928, 2.16469687]
            
            elif degree_setting == 3500 and cathode_setting == 0.345:
                l_free_pars = [8.61340613, 5.98094083, 0.04387717, 0.02617817, 0.24818277, 0.211507, 0.12784283, 1.78493792]
            elif degree_setting == 3500 and cathode_setting == 1.054:
                l_free_pars = [8.23064289, 5.78370797, 0.03284778, 0.22921785, 0.28779962, 0.08552447, 0.0198607, 1.82238142]
            elif degree_setting == 3500 and cathode_setting == 2.356:
                l_free_pars = [8.34192553, 5.93691378, 0.12957123, 0.02833529, 0.16640535, 0.16816157, 0.10335449, 1.75901753]
            
            elif degree_setting == 4500 and cathode_setting == 0.345:
                l_free_pars = [8.44539528, 5.72455525, 0.03460704, 0.10541354, 0.29660492, 0.03163016, 0.33665642, 1.72612264]
            elif degree_setting == 4500 and cathode_setting == 1.054:
                l_free_pars = [8.54604476, 5.70928189, 0.28809291, 0.04366459, 0.04548912, 0.06554624, 0.00995299, 1.41115711]
            elif degree_setting == 4500 and cathode_setting == 2.356:
                l_free_pars = [8.52197558, 5.8022725, 0.03320323, 0.02104382, 0.20920254, 0.07357184, 0.13532101, 1.47980755]
            
            elif degree_setting == 5300 and cathode_setting == 0.345:
                l_free_pars = [10.65639391, 6.00171995, 0.21318309, 0.04482494, 0.15244351, 0.02067115, 0.02876731, 1.41288691]
            elif degree_setting == 5300 and cathode_setting == 1.054:
                l_free_pars = [1.03030254e+01, 5.97495921e+00, 1.74354451e-01, 1.23981905e-02, 2.53512778e-01, 1.67677835e-02, 4.90220849e-03, 1.42171689e+00]
            elif degree_setting == 5300 and cathode_setting == 2.356:
                l_free_pars = [10.04609711, 6.2544237, 0.31215843, 0.04122312, 0.01038293, 0.01665143, 0.02820888, 1.31779608]
            
            else:
                print 'Not set up for %d at %.3f yet!' % (degree_setting, cathode_setting)
                sys.exit()
            
        
        
            a_free_par_guesses = l_free_pars[:4] + [self.g1_value, self.spe_res_value, self.extraction_efficiency_value, self.gas_gain_value, self.gas_gain_width, self.l_means_pf_eff_pars[0], self.l_means_pf_eff_pars[1], self.l_means_s2_eff_pars[0], self.l_means_s2_eff_pars[1], l_free_pars[4], l_free_pars[5], 0, 0, 0] + l_free_pars[-2:]
        
        
        elif self.fit_type == 'bl' and a_free_par_guesses == None:
            
            """
            [0.09581742, 0.17142747, 0.19346854, 0.10765697, 0.16335862, 22.90365074, 0.0359913 , 1.13402335, 0.77952878, 0.11695468, 0.16992129, 11.77422521, 0.03032592, 1.04006065, 0.78162736, 0.09716007, 0.19966872, 29.70498045, 0.04494912, 1.06974509, 0.82369163]
            """
            
            a_free_par_guesses = [13.7] + [0.09581742, 0.17142747, 0.19346854, test.g1_value, test.spe_res_value, test.extraction_efficiency_value, test.gas_gain_value, test.gas_gain_width, test.l_means_pf_eff_pars[0], test.l_means_pf_eff_pars[1], test.l_means_s2_eff_pars[0], test.l_means_s2_eff_pars[1], test.l_means_pf_stdev_pars[0], test.l_means_pf_stdev_pars[1], test.l_means_pf_stdev_pars[2], 0.20, 0.10765697, 0.16335862, 22.90365074, 0.0359913, 1.13402335, 0.77952878, 0.11695468, 0.16992129, 11.77422521, 0.03032592, 1.04006065, 0.78162736, 0.09716007, 0.19966872, 29.70498045, 0.04494912, 1.06974509, 0.82369163]
        

        elif self.fit_type == 'ml' and a_free_par_guesses == None:
            
            """
            [1.50570624e-01,   1.73586063e-01,   1.20784480e-01,
            
             8.28728080e-02,   2.50685523e-01,   3.47215755e+01,
             2.97646444e-02,   1.17420176e+00,   3.29941916e-01,
             8.60177855e-01,   1.37438771e-01,   8.81837797e-01,
             2.82658988e-01,   7.59336611e-01,   3.93830317e-01,
             9.75755515e-01,
             
             4.09473409e-02,   1.83664545e-01,
             3.48234408e+01,   3.78743406e-02,   1.29242185e+00,
             1.18262763e-01,   7.30735165e-01,   2.50593149e-01,
             7.81316055e-01,   3.63693355e-01,   9.32919132e-01,
             3.13526304e-01,   8.52441265e-01,   5.72525297e-02,
             2.44435244e-01,   3.42007248e+01,   2.61562638e-02,
             1.27534975e+00,   1.26479405e-01,   8.01497954e-01,
             2.27479810e-01,   8.70813328e-01,   3.94553333e-01,
             9.02541013e-01,   3.48684210e-01,   9.43474342e-01]
            """
            
            a_free_par_guesses = [13.7] + [1.50570624e-01, 1.73586063e-01, 1.20784480e-01, test.g1_value, test.spe_res_value, test.extraction_efficiency_value, test.gas_gain_value, test.gas_gain_width, test.l_means_pf_eff_pars[0], test.l_means_pf_eff_pars[1], test.l_means_s2_eff_pars[0], test.l_means_s2_eff_pars[1], test.l_means_pf_stdev_pars[0], test.l_means_pf_stdev_pars[1], test.l_means_pf_stdev_pars[2], 0.20]
            a_free_par_guesses += [8.28728080e-02, 2.50685523e-01, 3.47215755e+01, 2.97646444e-02, 1.17420176e+00, 3.29941916e-01, 8.60177855e-01, 1.37438771e-01, 8.81837797e-01, 2.82658988e-01, 7.59336611e-01, 3.93830317e-01, 9.75755515e-01, 4.09473409e-02, 1.83664545e-01, 3.48234408e+01, 3.78743406e-02, 1.29242185e+00, 1.18262763e-01, 7.30735165e-01, 2.50593149e-01, 7.81316055e-01, 3.63693355e-01, 9.32919132e-01, 3.13526304e-01, 8.52441265e-01, 5.72525297e-02, 2.44435244e-01, 3.42007248e+01, 2.61562638e-02, 1.27534975e+00, 1.26479405e-01, 8.01497954e-01, 2.27479810e-01, 8.70813328e-01, 3.94553333e-01, 9.02541013e-01, 3.48684210e-01, 9.43474342e-01]
            
            
        elif self.fit_type[:4] == 'mlti' and a_free_par_guesses == None:
            # -7611.2 (11.03)
            #[2.54939588e+03, 1.57619946e-01, 5.41196862e+00, 1.75879471e+00, 1.99031263e-01, 2.53147460e-01, 6.53197967e-03, 9.49893129e-01, 2.54523355e+00, 4.57287900e-01, 1.14810646e+00, 2.50333894e-01, 2.75434024e+00, 2.39239144e-02, 2.07915289e+00, 2.14821200e-01, 1.58167711e+00, 5.27447143e-03, 1.31299744e+00, 2.68573145e+00, 3.76481584e-01, 1.72374777e+00, 2.06270516e-01, 1.66246384e+00, 2.81156302e-01, 2.10244886e+00, 2.98699789e-01, 1.49194186e+00, 6.24836144e-03, 1.06208467e+00, 2.18957740e+00, 1.88771843e-01, 2.24797129e+00, 1.11121062e-01, 1.64893837e+00, 7.57261406e-02, 2.57242159e+00, 4.70625362e-01, 2.11841929e+00, 2.10727420e-01, 1.59032232e-01]
            #a_free_par_guesses = [13.7] + [2.54939588e+03, 0.14, 5.41196862e+00, 1.75879471e+00, test.g1_value, test.spe_res_value, test.extraction_efficiency_value, test.gas_gain_value, test.gas_gain_width, test.l_means_pf_eff_pars[0], test.l_means_pf_eff_pars[1], test.l_means_s2_eff_pars[0], test.l_means_s2_eff_pars[1]]
            #a_free_par_guesses += [0.15, 0.6, 7, 0.05, 0.3, 800, 0.20]
            #a_free_par_guesses += [6.53197967e-03, 9.49893129e-01, 2., 0.2213, 2.735, 0.03, 1.803, 0.2, 2.26, 0.05, 1.43, 5.27447143e-03, 1.31299744e+00, 2., 0.345, 2.57, 0.08, 1.757, 0.13, 1.53, 0.05, 1.414, 6.24836144e-03, 1.06208467e+00, 2., 0.3227, 1.84, 0.02465, 1.657, 0.082, 1.458, 0.06, 1.39]

            #
            #a_free_par_guesses = [1.08709924e+03, 1.70254489e-01, 8.01974265e+00, 1.22726072e+00, 1.89359892e-01, 7.68846343e-01, 4.24334312e+00, 5.44814977e-02, 3.02645109e-01, 1.24590231e+03, 7.03652882e-03, 1.15264692e+00, 1.65709112e+00, 9.56960856e-02, 2.22106910e+00, 8.09572580e-02, 1.96258172e+00, 1.42793177e-02, 1.87825016e+00, 2.00295791e-01, 1.91625363e+00, 7.83532539e-03, 9.94827912e-01, 2.19609231e+00, 3.72114093e-01, 1.55361579e+00, 2.63458835e-01, 2.85696056e+00, 1.10304053e-01, 1.40789054e+00, 4.86195840e-01, 1.89213081e+00, 7.42901857e-03, 1.20864482e+00, 1.85946542e+00, 1.31883714e-01, 1.64934477e+00, 4.30770255e-01, 2.51437672e+00, 2.66576458e-01, 1.78342843e+00, 3.26682937e-01, 1.32609627e+00]
            a_free_par_guesses = [13.7] + [1.08709924e+03, 1.70254489e-01, 8.01974265e+00, 1.22726072e+00, test.g1_value, test.spe_res_value, test.extraction_efficiency_value, test.gas_gain_value, test.gas_gain_width, test.l_means_pf_eff_pars[0], test.l_means_pf_eff_pars[1], test.l_means_s2_eff_pars[0], test.l_means_s2_eff_pars[1]]
            a_free_par_guesses += [1.89359892e-01, 7.68846343e-01, 4.24334312e+00, 5.44814977e-02, 3.02645109e-01, 1.24590231e+03, 0.20]
            a_free_par_guesses += [7.03652882e-03, 1.15264692e+00, 1.65709112e+00, 9.56960856e-02, 2.22106910e+00, 8.09572580e-02, 1.96258172e+00, 1.42793177e-02, 1.87825016e+00, 2.00295791e-01, 1.91625363e+00, 7.83532539e-03, 9.94827912e-01, 2.19609231e+00, 3.72114093e-01, 1.55361579e+00, 2.63458835e-01, 2.85696056e+00, 1.10304053e-01, 1.40789054e+00, 4.86195840e-01, 1.89213081e+00, 7.42901857e-03, 1.20864482e+00, 1.85946542e+00, 1.31883714e-01, 1.64934477e+00, 4.30770255e-01, 2.51437672e+00, 2.66576458e-01, 1.78342843e+00, 3.26682937e-01, 1.32609627e+00]
            
            print 'Using marginalized mode...'
            a_free_par_guesses = [13.914009221136185, 4281.479735635744, 0.16546679196675235, 6.7610197183403518, 1.0551062796773112, 0.13350611083445305, 0.61575573532796068, 0.88982164137123598, 23.89000788593486, 7.6801510096048231, 3.2115687293802844, 0.77890357954768774, 107.54602888516612, 374.42796295691306, 0.17156032308612026, 3.6834392491735657, 0.80057626013852246, 0.071422634106864349, 0.28338549124786155, 1312.7676711910303, 0.22676954767731816, 0.0082611906604543074, 0.94991602321268953, 1.8244924880641056, 0.48243550189213524, 2.5816007168398172, 0.082465200981977307, 1.515418108635328, 0.1273199452676535, 1.7027009874815704, 0.19673497271467499, 1.4904720994584044, 0.0089083426902317776, 0.86706524988184286, 1.9087969065681276, 0.34728048935805578, 2.0127241327607996, 0.086090950575291397, 1.677609707659518, 0.31064858310847204, 1.5789756044724259, 0.073248603636165491, 1.4121197822759908, 0.0088082437075026904, 0.87576609616960521, 1.8194502227135598, 0.39577337640654064, 2.4493442738260267, 0.06191591254314633, 1.6925834308195318, 0.14304672420577361, 1.553981076516237, 0.15508256596425907, 1.5308692016096666]
            
            
        elif self.fit_type == 'test' and a_free_par_guesses == None:
            if self.test_switch == 'ti':
                a_free_par_guesses = [13.7] + [1.54475062e+03, 1.64379147e-01, 4.36104463e+00, 8.98996057e-01, 2.08472361e-01, 1.38411501e-01, test.g1_value, test.spe_res_value, test.extraction_efficiency_value, test.gas_gain_value, test.gas_gain_width, test.l_means_pf_eff_pars[0], test.l_means_pf_eff_pars[1], test.l_means_s2_eff_pars[0], test.l_means_s2_eff_pars[1], test.l_means_pf_stdev_pars[0], test.l_means_pf_stdev_pars[1], test.l_means_pf_stdev_pars[2], 0.20]
                a_free_par_guesses += [7.82492440e-03, 9.23119912e-01, 9.16746943e-01, 1.83772833e-01, 8.72860632e-01, 1.47541249e-01, 8.55168969e-01, 3.45080996e-01, 8.94896574e-01, 4.69564070e-01, 9.77457418e-01, 7.52168829e-03, 9.46251728e-01, 9.46406213e-01, 1.73989599e-01, 8.42582117e-01, 8.07942982e-03, 9.58538157e-01, 3.88891740e-01, 9.76036064e-01, 1.77750998e-01, 9.96065813e-01, 7.92346661e-03, 1.01050877e+00, 9.40182314e-01, 1.78074617e-01, 8.62478972e-01, 1.04958438e-01, 8.61875976e-01, 2.11984882e-01, 8.81830883e-01, 8.11339649e-02, 8.47709662e-01]
            elif self.test_switch == 'pol1':
                sys.exit()
            elif self.test_switch == 'pol2':
                sys.exit()
            elif self.test_switch == 'pol3':
                sys.exit()
            elif self.test_switch == 'pol4':
                sys.exit()
            elif self.test_switch == 'pol5':
                sys.exit()
            elif self.test_switch == 'mod_gom':
                sys.exit()
        

            
          

        
        #print a_free_par_guesses
        l_parameters = [a_free_par_guesses for i in xrange(iterations)]
        l_log_likelihoods = self.gpu_pool.map(self.ln_likelihood_function_wrapper, l_parameters)
        
        #plt.hist(l_log_likelihoods, bins=20)
        #plt.show()
        
        l_log_likelihoods = np.asarray(l_log_likelihoods)
        l_quantiles = np.percentile(l_log_likelihoods, [2.5, 97.5])
        l_log_likelihoods = l_log_likelihoods[(l_log_likelihoods > l_quantiles[0]) & (l_log_likelihoods < l_quantiles[1])]
        #print l_log_likelihoods

        std_ll = np.std(l_log_likelihoods)

        std_scale = 1.0

        print 'Mean for %.3e MC iterations is %f' % (self.num_mc_events, np.mean(l_log_likelihoods))
        print 'Standard deviation for %.3e MC iterations is %f' % (self.num_mc_events*self.num_loops, std_ll)
        print 'Will scale LL such that stdev is %.1f' % (std_scale)

        if std_ll < std_scale:
            self.b_suppress_likelihood = True
            self.ll_suppression_factor = 1.
            print 'Standard deviation already small so not supressing\n\n'
        else:
            self.b_suppress_likelihood = True
            self.ll_suppression_factor = std_ll / std_scale
            print 'LL suppression factor: %f\n' % self.ll_suppression_factor







if __name__ == '__main__':
    copy_reg.pickle(types.MethodType, reduce_method)

    # d_coincidence_data should be in the following form:
    # d_coincidence_data['degree_settings'] = [2300, 3000, ...]
    # d_coincidence_data['cathode_settings'] = [0.345, 1.054, ...]
    # d_coincidence_data['cathode_settings'] = [4.5]


    """
    d_coincidence_data = {}
    d_coincidence_data['degree_settings'] = [3000]
    d_coincidence_data['cathode_settings'] = [0.345]

    test = fit_nr('s', d_coincidence_data, num_mc_events=2e6, l_gpus=[0, 1, 2, 3, 4, 5], num_loops=4)
    #test.suppress_likelihood()
    
    #ln_likelihood_full_matching_single_energy(self, py, qy, intrinsic_res_s1, intrinsic_res_s2, g1_value, spe_res_value, extraction_efficiency_value, gas_gain_mean_value, gas_gain_width_value, pf_eff_par0, pf_eff_par1, s2_eff_par0, s2_eff_par1, pf_stdev_par0, pf_stdev_par1, pf_stdev_par2, exciton_to_ion_par0_rv, exciton_to_ion_par1_rv, exciton_to_ion_par2_rv, d_gpu_local_info, draw_fit=False):
    
    # 3000, 345: [9.42934482, 6.80825196, 0.17586548, 0.19511261, 0.47934021, 0.22603184, 0.18254204, 2.35914599]
    # 3000, 1054: [7.25966214, 6.49163371, 0.04033762, 0.14759035, 0.64133758, 0.25714344, 0.35668657, 2.59558031]
    # 3000, 2356: [8.48228266, 6.45346487, 0.14723975, 0.33379526, 0.465922, 0.04439688, 0.20463928, 2.16469687]
    
    # 3500, 345: [8.61340613, 5.98094083, 0.04387717, 0.02617817, 0.24818277, 0.211507, 0.12784283, 1.78493792]
    # 3500, 1054: [8.23064289, 5.78370797, 0.03284778, 0.22921785, 0.28779962, 0.08552447, 0.0198607, 1.82238142]
    # 3500, 2356: [8.34192553, 5.93691378, 0.12957123, 0.02833529, 0.16640535, 0.16816157, 0.10335449, 1.75901753]
    
    # 4500, 345: [8.44539528, 5.72455525, 0.03460704, 0.10541354, 0.29660492, 0.03163016, 0.33665642, 1.72612264]
    # 4500, 1054: 8.54604476, 5.70928189, 0.28809291, 0.04366459, 0.04548912, 0.06554624, 0.00995299, 1.41115711
    # 4500, 2356: [8.52197558, 5.8022725, 0.03320323, 0.02104382, 0.20920254, 0.07357184, 0.13532101, 1.47980755]
    
    # 5300, 345: [10.65639391, 6.00171995, 0.21318309, 0.04482494, 0.15244351, 0.02067115, 0.02876731, 1.41288691]
    # 5300, 1054: [1.03030254e+01, 5.97495921e+00, 1.74354451e-01, 1.23981905e-02, 2.53512778e-01, 1.67677835e-02, 4.90220849e-03, 1.42171689e+00]
    # 5300, 2356: [10.04609711, 6.2544237, 0.31215843, 0.04122312, 0.01038293, 0.01665143, 0.02820888, 1.31779608]
    
    l_test_parameters_single_energy = [9.43, 6.81, 0.18, 0.20, test.g1_value, test.spe_res_value, test.extraction_efficiency_value, test.gas_gain_value, test.gas_gain_width, test.l_means_pf_eff_pars[0], test.l_means_pf_eff_pars[1], test.l_means_s2_eff_pars[0], test.l_means_s2_eff_pars[1], 0.48, 0.23, 0, 0, 0, 0.18, 2.36]
    #test.gpu_pool.map(test.wrapper_ln_likelihood_full_matching_single_energy, [l_test_parameters_single_energy])

    #a_free_par_bounds = [(3., 15.), (3.5, 15.), (0.01, 1.0), (0.01, 0.9), (0.01, 1.0), (0.01, 0.9), (0., 1.), (0.5, 3.5)]
    #test.differential_evolution_minimizer_free_pars(a_free_par_bounds, maxiter=50, popsize=15, tol=0.05)
    
    test.suppress_likelihood()
    test.run_mcmc(num_steps=200, num_walkers=256)
    """
    
    
    # band fitting
    # (self, w_value, kappa, intrinsic_res_s1, intrinsic_res_s2, g1_value, spe_res_value, extraction_efficiency_value, gas_gain_mean_value, gas_gain_width_value, pf_eff_par0, pf_eff_par1, s2_eff_par0, s2_eff_par1, pf_stdev_par0, pf_stdev_par1, pf_stdev_par2, d_field_degree_pars, d_gpu_local_info, draw_fit=False)
    
    
    """
    d_coincidence_data = {}
    d_coincidence_data['degree_settings'] = [-4]
    d_coincidence_data['cathode_settings'] = [0.345, 1.054, 2.356]
    # for seeding walkers
    #d_coincidence_data['cathode_settings'] = [0.345]
    
    test = fit_nr('bl', d_coincidence_data, num_mc_events=5e6, l_gpus=[0, 1, 2], num_loops=4)
    
    
    # (self, w_value, kappa, intrinsic_res_s1, intrinsic_res_s2, g1_value, spe_res_value, extraction_efficiency_value, gas_gain_mean_value, gas_gain_width_value, pf_eff_par0, pf_eff_par1, s2_eff_par0, s2_eff_par1, pf_stdev_par0, pf_stdev_par1, pf_stdev_par2, d_field_degree_pars, d_gpu_local_info, draw_fit=False)
    
    # [0.0704274, 0.18719008, 0.12343091, 0.17159282, 0.22879805, 35.26485876, 0.07535348, 1.03668265, 0.72258757, 0.14327453, 0.16939731, 16.88048192, 0.11803932, 1.15755857, 0.69684433, 0.36502801, 0.2520376, 15.13040272, 0.09705286, 0.52839206, 0.72988628]
    #l_test_parameters_band = [13.7, 0.0704274, 0.18719008, 0.12343091, test.g1_value, test.spe_res_value, test.extraction_efficiency_value, test.gas_gain_value, test.gas_gain_width, test.l_means_pf_eff_pars[0], test.l_means_pf_eff_pars[1], test.l_means_s2_eff_pars[0], test.l_means_s2_eff_pars[1], test.l_means_pf_stdev_pars[0], test.l_means_pf_stdev_pars[1], test.l_means_pf_stdev_pars[2], 0.20, 0.17159282, 0.22879805, 35.26485876, 0.07535348, 1.03668265, 0.72258757, 0.14327453, 0.16939731, 16.88048192, 0.11803932, 1.15755857, 0.69684433, 0.36502801, 0.2520376, 15.13040272, 0.09705286, 0.52839206, 0.72988628]
    #test.gpu_pool.map(test.wrapper_ln_likelihood_full_matching_band, [l_test_parameters_band])
    

    a_free_par_bounds = [(0.05, 0.2), (0, 0.3), (0, 0.3)] + [(0, 0.15), (0.1, 0.3), (10, 35), (0.01, 0.045), (0.9, 1.5), (0.6, 1)] + [(0, 0.15), (0.1, 0.3), (10, 35), (0.01, 0.045), (0.9, 1.5), (0.6, 1)] + [(0, 0.15), (0.1, 0.3), (10, 35), (0.01, 0.045), (0.9, 1.5), (0.6, 1)]
    #a_free_par_bounds = [(0.1, 0.2), (0, 0.3), (0, 0.3)] + [(0, 0.15), (0.1, 0.3), (10, 35), (0.01, 0.045), (0.9, 1.5), (0.6, 1)]
    test.differential_evolution_minimizer_free_pars(a_free_par_bounds, maxiter=150, popsize=20, tol=0.05)
    
    #test.suppress_likelihood()
    #test.run_mcmc(num_steps=50, num_walkers=512)
    
    """
    
    """
    
    d_coincidence_data = {}
    d_coincidence_data['degree_settings'] = [-4, 3000, 3500, 4500, 5300]
    d_coincidence_data['cathode_settings'] = [0.345, 1.054, 2.356]
    
    # ti
    test_switch = 'ti'
    l_recombination_pars = [(0.001, 0.05)]
    gpu_num = 1
    
    # pol1
    #test_switch = 'pol1'
    #l_recombination_pars = [(0.001, 0.25), (0.0005, 0.002)]
    #gpu_num = 2
    
    # pol2
    #test_switch = 'pol2'
    #l_recombination_pars = [(0.001, 0.25), (-0.005, 0.005), (-7e-6, 7e-6)]
    #gpu_num = 3
    
    # pol3
    #test_switch = 'pol3'
    #l_recombination_pars = [(0.001, 0.25), (-0.005, 0.005), (-1e-5, 1e-5), (-7e-9, 7e-9)]
    #gpu_num = 4
    
    # pol4
    #test_switch = 'pol4'
    #l_recombination_pars = [(0.001, 0.25), (-0.005, 0.005), (-7e-6, 7e-6), (-1e-8, 1e-8), (-1e-11, 1e-11)]
    #gpu_num = 5
    
    # pol5
    #test_switch = 'pol5'
    #l_recombination_pars = [(0.001, 0.25), (-0.005, 0.005), (-7e-6, 7e-6), (-1e-8, 1e-8), (-2e-11, 2e-11), (-1e-13, 1e-13)]
    #gpu_num = 0
    
    # mod_gom
    #test_switch = 'mod_gom'
    #l_recombination_pars = [(0.001, 0.25), (0.05, 0.8), (0.1, 50), (0.003, 0.05)]
    #gpu_num = 0
    
    test = fit_nr('test', d_coincidence_data, num_mc_events=2e6, l_gpus=[gpu_num], num_loops=1, test_switch=test_switch)
    
    a_free_par_bounds = [(100, 4000), (0.1, 0.2), (1.5, 10), (0.8, 2), (0, 0.3), (0, 0.3)]
    a_free_par_bounds += (l_recombination_pars + [(0.9, 1.5), (0.8, 1), (0, 0.5), (0.8, 1), (0, 0.5), (0.8, 1), (0, 0.5), (0.8, 1), (0, 0.5), (0.8, 1)])*len(d_coincidence_data['cathode_settings'])
    
    test.differential_evolution_minimizer_free_pars(a_free_par_bounds, maxiter=150, popsize=5, tol=0.05)
    
    #test.suppress_likelihood()


    """
    
    d_coincidence_data = {}
    d_coincidence_data['degree_settings'] = [-4, 3000, 3500, 4500, 5300]
    d_coincidence_data['cathode_settings'] = [0.345, 1.054, 2.356]
    
    test = fit_nr('mlti', d_coincidence_data, num_mc_events=2e6, l_gpus=[0, 1, 2, 3, 4, 5], num_loops=4)
    
    

    a_free_par_guesses = [13.7] + [2.54939588e+03, 0.14, 5.41196862e+00, 1.75879471e+00, test.g1_value, test.spe_res_value, test.extraction_efficiency_value, test.gas_gain_value, test.gas_gain_width, test.l_means_pf_eff_pars[0], test.l_means_pf_eff_pars[1], test.l_means_s2_eff_pars[0], test.l_means_s2_eff_pars[1]]
    a_free_par_guesses += [0.15, 0.6, 7, 0.05, 0.3, 800]
    a_free_par_guesses += [0.20]
    a_free_par_guesses += [6.53197967e-03, 9.49893129e-01, 2., 0.2213, 2.735, 0.03, 1.803, 0.2, 2.26, 0.05, 1.43, 5.27447143e-03, 1.31299744e+00, 2., 0.345, 2.57, 0.08, 1.757, 0.13, 1.53, 0.05, 1.414, 6.24836144e-03, 1.06208467e+00, 2., 0.3227, 1.84, 0.02465, 1.657, 0.082, 1.458, 0.06, 1.39]
    #test.gpu_pool.map(test.wrapper_ln_likelihood_full_matching_multiple_energies_lindhard_model_with_ti_recombination, [a_free_par_guesses])
    
    #a_free_par_bounds = [(100, 4000), (0.1, 0.2), (1.5, 10), (0.8, 2)]
    #a_free_par_bounds += [(0.01, 0.35), (0.1, 2.), (1, 20), (0.01, 0.3), (0.1, 2), (100, 2000)]
    #a_free_par_bounds += [(0.001, 0.05), (0.9, 1.5), (0.8, 3.), (0, 0.5), (0.8, 3.), (0, 0.5), (0.8, 3.), (0, 0.5), (0.8, 3.), (0, 0.5), (0.8, 3.)] + [(0.001, 0.05), (0.9, 1.5), (0.8, 3.), (0, 0.5), (0.8, 3.), (0, 0.5), (0.8, 3.), (0, 0.5), (0.8, 3.), (0, 0.5), (0.8, 3.)] + [(0.001, 0.05), (0.9, 1.5), (0.8, 3.), (0, 0.5), (0.8, 3.), (0, 0.5), (0.8, 3.), (0, 0.5), (0.8, 3.), (0, 0.5), (0.8, 3.)]
    #test.differential_evolution_minimizer_free_pars(a_free_par_bounds, maxiter=150, popsize=5, tol=0.1)
    
    
    #test.suppress_likelihood(100)
    test.run_mcmc(num_steps=38*3, num_walkers=512)
    
    
     
    test.close_workers()



