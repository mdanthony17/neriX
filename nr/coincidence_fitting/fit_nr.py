
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





def reduce_method(m):
    return (getattr, (m.__self__, m.__func__.__name__))


d_cathode_voltage_to_field = {0.345:210,
                              1.054:490,
                              2.356:1000}



class gpu_pool:
    def __init__(self, num_gpus, grid_dim, block_dim, num_dim_gpu_call, d_gpu_single_copy_arrays, function_name):
        self.num_gpus = num_gpus
        self.grid_dim = grid_dim
        self.block_dim = block_dim
        self.num_dim_gpu_call = num_dim_gpu_call
        self.d_gpu_single_copy_arrays = d_gpu_single_copy_arrays
        self.function_name = function_name

        self.alive = True
        self.q_gpu = Queue()
        for i in xrange(self.num_gpus):
            self.q_gpu.put(i)

        self.q_in = Queue()
        self.q_out = Queue()
        self.l_dispatcher_threads = []
        self.dispatcher_dead_time = 0.5

        self.q_dead = Queue()


        for i in xrange(self.num_gpus):
            if self.q_gpu.empty():
                break
            print 'starting worker'
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




        seed = int(time.time()*1000)


        # source code
        local_gpu_setup_kernel = pycuda.compiler.SourceModule(cuda_full_observables_production.cuda_full_observables_production_code, no_extern_c=True).get_function('setup_kernel')

        local_rng_states = drv.mem_alloc(np.int32(self.block_dim*self.grid_dim)*pycuda.characterize.sizeof('curandStateXORWOW', '#include <curand_kernel.h>'))
        local_gpu_setup_kernel(np.int32(int(self.block_dim*self.grid_dim)), local_rng_states, np.uint64(0), np.uint64(0), grid=(int(self.grid_dim), 1), block=(int(self.block_dim), 1, 1))

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
        

        print 'Putting bin edges on GPU...'
        gpu_bin_edges_s1 = pycuda.gpuarray.to_gpu(self.d_gpu_single_copy_arrays['bin_edges_s1'])


        print 'Putting bin edges on GPU...'
        gpu_bin_edges_log = pycuda.gpuarray.to_gpu(self.d_gpu_single_copy_arrays['bin_edges_log'])


        d_gpu_local_info = {'function_to_call':gpu_observables_func,
                            'rng_states':local_rng_states,
                            'd_gpu_energy':d_gpu_energy_arrays,
                            'gpu_energy_bkg':gpu_energy_bkg,
                            'gpu_bin_edges_s1':gpu_bin_edges_s1,
                            'gpu_bin_edges_log':gpu_bin_edges_log}

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
    def __init__(self, fit_type, d_coincidence_data, d_band_data=None, free_exciton_to_ion_ratio=False, num_mc_events=5e6, name_notes=None, num_gpus=1, num_loops=1):
        
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
        # 5. Band with Lindhard model: 'ml'
        #    - for fitting the band using the
        #       Lindhard model to reduce number of free pars

        # d_coincidence_data should be in the following form:
        # d_coincidence_data['degree_settings'] = [2300, 3000, ...]
        # d_coincidence_data['cathode_settings'] = [0.345, 1.054, ...]
        # d_coincidence_data['cathode_settings'] = [4.5]
        # same convention for band data: deg=-4
        
        # total MC events = num_mc_events*num_loops
        
        self.num_loops = num_loops
        self.fit_type = fit_type
        
        if fit_type == 's':
            assert len(d_coincidence_data['degree_settings']) == 1, ('If looking at a single energy can only give one degree setting in d_coincidence_data.')
        elif fit_type == 'm':
            assert len(d_coincidence_data['degree_settings']) >= 1
        elif fit_type == 'ml':
            assert len(d_coincidence_data['degree_settings']) == 1
        elif fit_type == 'bl':
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

        self.l_energy_settings = [300, 0, 30]



        # ------------------------------------------------
        # ------------------------------------------------
        # Get information for data
        # ------------------------------------------------
        # ------------------------------------------------


        self.d_coincidence_data_information = {}

        # get all degrees and cathode settings and sort
        self.l_degree_settings_in_use = sorted(d_coincidence_data['degree_settings'])
        self.l_cathode_settings_in_use = sorted(d_coincidence_data['cathode_settings'])
        
        if fit_type == 'bl':
            num_bins = 60
        else:
            num_bins = 20
        
        self.l_s1_settings = [num_bins, 0, 40]
        self.l_s2_settings = [num_bins, 0, 2000]
        self.l_log_settings = [num_bins, 1, 3.5]

        self.l_quantiles = [20, 80]
        
        # define bin edges for use in MC
        self.a_s1_bin_edges = np.linspace(self.l_s1_settings[1], self.l_s1_settings[2], num=self.l_s1_settings[0]+1, dtype=np.float32)
        self.a_s2_bin_edges = np.linspace(self.l_s2_settings[1], self.l_s2_settings[2], num=self.l_s2_settings[0]+1, dtype=np.float32)
        self.a_log_bin_edges = np.linspace(self.l_log_settings[1], self.l_log_settings[2], num=self.l_log_settings[0]+1, dtype=np.float32)
        
        

        for cathode_setting in self.l_cathode_settings_in_use:
            self.d_coincidence_data_information[cathode_setting] = {}
            for degree_setting in self.l_degree_settings_in_use:
                self.d_coincidence_data_information[cathode_setting][degree_setting] = {}

                # add basic information to dictionary
                #self.d_coincidence_data_information[cathode_setting][degree_setting]['file'] = File('%s%.3fkV_%ddeg/s1_s2.pkl' % (self.path_to_coincidence_data, cathode_setting, degree_setting), 'read')
                self.d_coincidence_data_information[cathode_setting][degree_setting]['d_s1_s2'] = pickle.load(open('%s%.3fkV_%ddeg/s1_s2.pkl' % (self.path_to_coincidence_data, cathode_setting, degree_setting), 'r'))
                
                
                self.d_coincidence_data_information[cathode_setting][degree_setting]['s1_hist'] = Hist(*self.l_s1_settings, name='hS1')
                self.d_coincidence_data_information[cathode_setting][degree_setting]['s1_hist'].fill_array(self.d_coincidence_data_information[cathode_setting][degree_setting]['d_s1_s2']['s1'])
                
                self.d_coincidence_data_information[cathode_setting][degree_setting]['s2_hist'] = Hist(*self.l_s2_settings, name='hS2')
                self.d_coincidence_data_information[cathode_setting][degree_setting]['s2_hist'].fill_array(self.d_coincidence_data_information[cathode_setting][degree_setting]['d_s1_s2']['s2'])
                
                self.d_coincidence_data_information[cathode_setting][degree_setting]['s1_s2_hist'] = Hist2D(*(self.l_s1_settings+self.l_s2_settings), name='hS1S2')
                self.d_coincidence_data_information[cathode_setting][degree_setting]['s1_s2_hist'].fill_array(np.asarray([self.d_coincidence_data_information[cathode_setting][degree_setting]['d_s1_s2']['s1'], self.d_coincidence_data_information[cathode_setting][degree_setting]['d_s1_s2']['s2']]).T)
                
                self.d_coincidence_data_information[cathode_setting][degree_setting]['log_s2_s1_hist'] = Hist2D(*(self.l_s1_settings+self.l_log_settings), name='hLogS2S1')
                self.d_coincidence_data_information[cathode_setting][degree_setting]['log_s2_s1_hist'].fill_array(np.asarray([self.d_coincidence_data_information[cathode_setting][degree_setting]['d_s1_s2']['s1'], np.log10(self.d_coincidence_data_information[cathode_setting][degree_setting]['d_s1_s2']['s2']/self.d_coincidence_data_information[cathode_setting][degree_setting]['d_s1_s2']['s1'])]).T)
                
                self.d_coincidence_data_information[cathode_setting][degree_setting]['num_data_pts'] = len(self.d_coincidence_data_information[cathode_setting][degree_setting]['d_s1_s2']['s1'])
                
                self.d_coincidence_data_information[cathode_setting][degree_setting]['a_log_s2_s1'] = neriX_analysis.convert_2D_hist_to_matrix(self.d_coincidence_data_information[cathode_setting][degree_setting]['log_s2_s1_hist'], dtype=np.float32)
                
                
                self.d_coincidence_data_information[cathode_setting][degree_setting]['a_log_s2_over_s1'] = np.log10(self.d_coincidence_data_information[cathode_setting][degree_setting]['d_s1_s2']['s2']/self.d_coincidence_data_information[cathode_setting][degree_setting]['d_s1_s2']['s1'])



                # get quantiles
                # get values for bounds
                lb_s1, ub_s1 = np.percentile(self.d_coincidence_data_information[cathode_setting][degree_setting]['d_s1_s2']['s1'], self.l_quantiles)
                lb_logs2s1, ub_logs2s1 = np.percentile(self.d_coincidence_data_information[cathode_setting][degree_setting]['a_log_s2_over_s1'], self.l_quantiles)
                
                lower_bin_s1 = -1
                upper_bin_s1 = -1
                for bin_index, bin_edge in enumerate(self.a_s1_bin_edges):
                    # get lower index
                    if self.a_s1_bin_edges[bin_index] < lb_s1 and self.a_s1_bin_edges[bin_index+1] > lb_s1:
                        lower_bin_s1 = bin_index
                        
                    if self.a_s1_bin_edges[bin_index] < ub_s1 and self.a_s1_bin_edges[bin_index+1] > ub_s1:
                        upper_bin_s1 = bin_index + 1
                        
                    if lower_bin_s1 != -1 and upper_bin_s1 != -1:
                        break

                # repeat for logs2s1
                lower_bin_logs2s1 = -1
                upper_bin_logs2s1 = -1
                for bin_index, bin_edge in enumerate(self.a_log_bin_edges):
                    # get lower index
                    #print lb_logs2s1, ub_logs2s1
                    if self.a_log_bin_edges[bin_index] < lb_logs2s1 and self.a_log_bin_edges[bin_index+1] > lb_logs2s1:
                        lower_bin_logs2s1 = bin_index
                        
                    if self.a_log_bin_edges[bin_index] < ub_logs2s1 and self.a_log_bin_edges[bin_index+1] > ub_logs2s1:
                        upper_bin_logs2s1 = bin_index + 1
                        
                    if lower_bin_logs2s1 != -1 and upper_bin_logs2s1 != -1:
                        break
                    
            
                self.d_coincidence_data_information[cathode_setting][degree_setting]['low_bin_s1'] = lower_bin_s1
                self.d_coincidence_data_information[cathode_setting][degree_setting]['high_bin_s1'] = upper_bin_s1
                self.d_coincidence_data_information[cathode_setting][degree_setting]['low_bin_log'] = lower_bin_logs2s1
                self.d_coincidence_data_information[cathode_setting][degree_setting]['high_bin_log'] = upper_bin_logs2s1
                
                #print self.d_coincidence_data_information[cathode_setting][degree_setting]['low_bin_s1'], self.d_coincidence_data_information[cathode_setting][degree_setting]['high_bin_s1'], self.d_coincidence_data_information[cathode_setting][degree_setting]['low_bin_log'], self.d_coincidence_data_information[cathode_setting][degree_setting]['high_bin_log']
                





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

        self.g1_value = 0.117
        self.g1_uncertainty = 0.002
        
        self.w_value = 13.7
        self.w_value_uncertainty = 0.08
        
        self.extraction_efficiency_value = 0.895
        self.extraction_efficiency_uncertainty = 0.002

        self.gas_gain_value = 21.2
        self.gas_gain_uncertainty = 0.04

        self.gas_gain_width = 8.01
        self.gas_gain_width_uncertainty = 0.29

        self.spe_res_value = 0.59
        self.spe_res_uncertainty = 0.0056

        self.l_means_pf_stdev_pars = [0.014673497158526188, 0.5284272187436192, 4.3229124008196411]
        self.l_cov_matrix_pf_stdev_pars = [[9.4927705182690691e-10, 2.352933249729148e-08, -2.4920029049639913e-07], [2.352933249729148e-08, 4.8381636138100317e-06, -1.7993529191388666e-05], [-2.4920029049639913e-07, -1.7993529191388666e-05, 0.00013121519103705991]]

        self.l_means_pf_eff_pars = [3.09977598,  0.7398706] #[1.96178522,  0.46718076]
        self.l_cov_matrix_pf_eff_pars = [[1.88474706e-04, 4.67178803e-06], [4.67178803e-06, 7.54638566e-05]]

        # only for producing initial distribution
        # NOT FOR LIKELIHOOD
        #self.l_means_s1_eff_pars = [0.1, 0.1]
        #self.l_cov_matrix_s1_eff_pars = [[.3**2, 0], [0, .1**2]]

        self.l_means_s2_eff_pars = [110.02657201, 375.86306321]
        self.l_cov_matrix_s2_eff_pars = [[464.38711103, 151.68557669], [151.68557669, 535.59339378]]

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
            self.num_dimensions = 21
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
                self.num_dimensions = 14 + 9
                self.directory_name = 'multiple_energies_lindhard_model'
                self.gpu_function_name = 'gpu_full_observables_production_with_log_hist_lindhard_model'
            
            elif fit_type == 'mb':
                self.ln_likelihood_function = self.ln_likelihood_full_matching_single_energy_and_band
                self.ln_likelihood_function_wrapper = self.wrapper_ln_likelihood_full_matching_single_energy_and_band
                self.num_dimensions = 19
                self.directory_name = 'multiple_energies_with_band'
                self.gpu_function_name = 'gpu_full_observables_production_with_log_hist_spline'
            
            elif fit_type == 'bl':
                self.ln_likelihood_function = self.ln_likelihood_full_matching_band
                self.ln_likelihood_function_wrapper = self.wrapper_ln_likelihood_full_matching_band
                self.num_dimensions = 19 + len(self.l_cathode_settings_in_use) # 19 + scales
                self.directory_name = 'band_with_lindhard_model'
                self.gpu_function_name = 'gpu_full_observables_production_with_log_hist_lindhard'

            
            
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
                                                'bin_edges_s1':self.a_s1_bin_edges,
                                                'bin_edges_log':self.a_log_bin_edges
                                                }
        self.gpu_pool = gpu_pool(num_gpus=num_gpus, grid_dim=numBlocks, block_dim=block_dim, num_dim_gpu_call=self.num_dimensions, d_gpu_single_copy_arrays=d_gpu_single_copy_array_dictionaries, function_name=self.gpu_function_name)

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
                self.d_energy_arrays[degree_setting]['a_energy'][i] = self.d_energy_arrays[degree_setting]['h_energy'].GetRandom()
                    
            if view_energy_spectrum:
                cEnergySpec = Canvas()
                hEnergySpec = Hist(100, 0, 40)
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
                current_path_to_reduced_energy_spectra = '%snerixsim-%dkeV.root' % (self.path_to_reduced_energy_spectra, self.d_degree_setting_to_energy_name[degree_setting])
            else:
                current_path_to_reduced_energy_spectra = '%snerixsim-pure_nr_spec.root' % (self.path_to_reduced_energy_spectra)
                coincidence_degree_setting = False
            
            
            
            if coincidence_degree_setting and not os.path.exists(current_path_to_reduced_energy_spectra):
                # if the file doesn't exist we need to create it!
            
                # load the raw data file
                current_path_to_energy_spectra = '%snerixsim-%dkeV.root' % (self.path_to_energy_spectra, self.d_degree_setting_to_energy_name[degree_setting])
                f_simulation = File(current_path_to_energy_spectra, 'read')

                if sFileType == 'MC':

                    # set cuts
                    xRadius = '(sqrt(xpos[0]**2+ypos[0]**2) < 18)'
                    xZ = '(zpos[0]>-20 && zpos[0]<-4)'
                    xSingleScatter = '(nsteps_target==1)'
                    xLiqSciHeight = '(etotliqsci>700)'
                    xLXeEnergy = '(etot_target>0)'
                    #xLXeEnergy = '(etot_target>0)'
                    xLXeEnergy = '(Alt$(ed_nr_target[1],-1)==-1 && ed_nr_target[0] > 0)'

                    #print 'No TOF cut!\n'
                    neriX_analysis.warning_message('Need to finalize how tof is chosen in MC data!')
                    
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
                    
                    
                    xTOF = '(timeliqsci-tpos[0]> %d && timeliqsci-tpos[0] < %d)' % (low_time_tof, high_time_tof)

                    xAll = '%s && %s && %s && %s && %s && %s' % (xRadius, xZ, xSingleScatter, xLiqSciHeight, xLXeEnergy, xTOF)
                
                elif sFileType == 'MC_accidentals':
                    # set cuts
                    xRadius = '(sqrt(xpos[0]**2+ypos[0]**2) < 18)'
                    xZ = '(zpos[0]>-20 && zpos[0]<-4)'
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



    def get_prior_log_likelihood_scale(self, scale):
        if isinstance(scale, dict):
            for id in scale:
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
        if beta < 0:
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



    def get_prior_log_likelihood_probability(self, prob):
        if prob < 0 or prob > 1:
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





    def ln_likelihood_full_matching_single_energy(self, py, qy, intrinsic_res_s1, intrinsic_res_s2, g1_value, spe_res_value, extraction_efficiency_value, gas_gain_mean_value, gas_gain_width_value, pf_eff_par0, pf_eff_par1, s2_eff_par0, s2_eff_par1, pf_stdev_par0, pf_stdev_par1, pf_stdev_par2, exciton_to_ion_par0_rv, exciton_to_ion_par1_rv, exciton_to_ion_par2_rv, prob_bkg_event, scale_par, d_gpu_local_info, draw_fit=False):



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
        prior_ln_likelihood += self.get_prior_log_likelihood_resolution(intrinsic_res_s2)
        
        prior_ln_likelihood += self.get_prior_log_likelihood_scale(scale_par)


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

        current_ln_likelihood, pf_stdev_par0, pf_stdev_par1, pf_stdev_par2 = self.get_pf_stdev_default(pf_stdev_par0, pf_stdev_par1, pf_stdev_par2)
        prior_ln_likelihood += self.get_prior_log_likelihood_nuissance(current_ln_likelihood)
        
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

        a_pf_stdev = np.asarray([pf_stdev_par0, pf_stdev_par1, pf_stdev_par2], dtype=np.float32)
        
        prob_bkg_event = np.asarray(prob_bkg_event, dtype=np.float32)

        # for histogram binning
        num_bins_s1 = np.asarray(self.l_s1_settings[0], dtype=np.int32)
        num_bins_log = np.asarray(self.l_log_settings[0], dtype=np.int32)
        a_hist_2d = np.zeros(self.l_s1_settings[0]*self.l_log_settings[0], dtype=np.float32)
        
        num_loops = np.asarray(self.num_loops, dtype=np.int32)


        tArgs = (d_gpu_local_info['rng_states'], drv.In(num_trials), drv.In(mean_field), d_gpu_local_info['d_gpu_energy'][self.l_degree_settings_in_use[0]], d_gpu_local_info['gpu_energy_bkg'], drv.In(py), drv.In(qy), drv.In(g1_value), drv.In(extraction_efficiency), drv.In(gas_gain_value), drv.In(gas_gain_width), drv.In(spe_res), drv.In(intrinsic_res_s1), drv.In(intrinsic_res_s2), drv.In(a_pf_stdev), drv.In(exciton_to_ion_par0_rv), drv.In(exciton_to_ion_par1_rv), drv.In(exciton_to_ion_par2_rv), drv.In(pf_eff_par0), drv.In(pf_eff_par1), drv.In(s2_eff_par0), drv.In(s2_eff_par1), drv.In(prob_bkg_event), drv.In(num_bins_s1), d_gpu_local_info['gpu_bin_edges_s1'], drv.In(num_bins_log), d_gpu_local_info['gpu_bin_edges_log'], drv.InOut(a_hist_2d), drv.In(num_loops))

        d_gpu_local_info['function_to_call'](*tArgs, **self.d_gpu_scale)

        a_s1_s2_mc = np.reshape(a_hist_2d, (self.l_log_settings[0], self.l_s1_settings[0])).T

        sum_mc = np.sum(a_s1_s2_mc, dtype=np.float32)
        if sum_mc == 0:
            return -np.inf

        #a_s1_s2_mc = np.multiply(a_s1_s2_mc, np.sum(self.a_s1_s2, dtype=np.float32) / sum_mc)

        # if making PDF rather than scaling for rate
        scale_par *= float(self.num_mc_events*self.num_loops) / sum_mc

        a_s1_s2_mc = np.multiply(a_s1_s2_mc, float(scale_par)*self.d_coincidence_data_information[self.l_cathode_settings_in_use[0]][self.l_degree_settings_in_use[0]]['num_data_pts']/float(self.num_mc_events*self.num_loops))

        # likelihood for single energy
        if draw_fit:

            f, (ax1, ax2) = plt.subplots(2, sharex=True, sharey=True)

            s1_s2_data_plot = np.rot90(self.d_coincidence_data_information[self.l_cathode_settings_in_use[0]][self.l_degree_settings_in_use[0]]['a_log_s2_s1'])
            s1_s2_data_plot = np.flipud(s1_s2_data_plot)
            ax1.pcolormesh(self.a_s1_bin_edges, self.a_log_bin_edges, s1_s2_data_plot)

            s1_s2_mc_plot = np.rot90(a_s1_s2_mc)
            s1_s2_mc_plot = np.flipud(s1_s2_mc_plot)
            #print self.l_s1_settings
            #print self.l_log_settings
            #print self.d_coincidence_data_information[self.l_cathode_settings_in_use[0]][self.l_degree_settings_in_use[0]]['a_log_s2_s1'].shape
            #print a_s1_s2_mc.shape
            #print s1_s2_data_plot.shape
            #print s1_s2_mc_plot.shape
            ax2.pcolormesh(self.a_s1_bin_edges, self.a_log_bin_edges, s1_s2_mc_plot)
            #plt.colorbar()


            c1 = Canvas(1400, 400)
            c1.Divide(2)

            h_s1_data = Hist(*self.l_s1_settings, name='hS1_draw_data')
            root_numpy.array2hist(self.d_coincidence_data_information[self.l_cathode_settings_in_use[0]][self.l_degree_settings_in_use[0]]['a_log_s2_s1'].sum(axis=1), h_s1_data)

            hS1MC = Hist(*self.l_s1_settings, name='hS1_draw_mc')
            root_numpy.array2hist(a_s1_s2_mc.sum(axis=1), hS1MC)

            s1_scale_factor = h_s1_data.Integral() / float(hS1MC.Integral())

            g_s1_data = neriX_analysis.convert_hist_to_graph_with_poisson_errors(h_s1_data)
            g_s1_mc = neriX_analysis.convert_hist_to_graph_with_poisson_errors(hS1MC, scale=s1_scale_factor)

            g_s1_mc.SetFillColor(root.kBlue)
            g_s1_mc.SetMarkerColor(root.kBlue)
            g_s1_mc.SetLineColor(root.kBlue)
            g_s1_mc.SetFillStyle(3005)

            g_s1_data.SetTitle('S1 Comparison')
            g_s1_data.GetXaxis().SetTitle('S1 [PE]')
            g_s1_data.GetYaxis().SetTitle('Counts')

            g_s1_data.SetLineColor(root.kRed)
            g_s1_data.SetMarkerSize(0)
            g_s1_data.GetXaxis().SetRangeUser(self.l_s1_settings[1], self.l_s1_settings[2])
            g_s1_data.GetYaxis().SetRangeUser(0, 1.2*max(h_s1_data.GetMaximum(), hS1MC.GetMaximum()))

            c1.cd(1)
            g_s1_data.Draw('ap')
            g_s1_mc.Draw('same')
            g_s1_mc_band = g_s1_mc.Clone()
            g_s1_mc_band.Draw('3 same')

            h_s2_data = Hist(*self.l_log_settings, name='h_s2_draw_data')
            root_numpy.array2hist(self.d_coincidence_data_information[self.l_cathode_settings_in_use[0]][self.l_degree_settings_in_use[0]]['a_log_s2_s1'].sum(axis=0), h_s2_data)

            h_s2_mc = Hist(*self.l_log_settings, name='h_s2_draw_mc')
            root_numpy.array2hist(a_s1_s2_mc.sum(axis=0), h_s2_mc)

            s2_scale_factor = h_s2_data.Integral() / float(h_s2_mc.Integral())

            g_s2_data = neriX_analysis.convert_hist_to_graph_with_poisson_errors(h_s2_data)
            g_s2_mc = neriX_analysis.convert_hist_to_graph_with_poisson_errors(h_s2_mc, scale=s2_scale_factor)

            g_s2_mc.SetFillColor(root.kBlue)
            g_s2_mc.SetMarkerColor(root.kBlue)
            g_s2_mc.SetLineColor(root.kBlue)
            g_s2_mc.SetFillStyle(3005)

            g_s2_data.SetTitle('Log(S2/S1) Comparison')
            g_s2_data.GetXaxis().SetTitle('Log(S2/S1)')
            g_s2_data.GetYaxis().SetTitle('Counts')

            g_s2_data.SetLineColor(root.kRed)
            g_s2_data.SetMarkerSize(0)
            g_s2_data.GetXaxis().SetRangeUser(self.l_log_settings[1], self.l_log_settings[2])
            g_s2_data.GetYaxis().SetRangeUser(0, 1.2*max(h_s2_data.GetMaximum(), h_s2_mc.GetMaximum()))

            c1.cd(2)
            g_s2_data.Draw('ap')
            g_s2_mc.Draw('same')
            g_s2_mc_band = g_s2_mc.Clone()
            g_s2_mc_band.Draw('3 same')

            c1.Update()

            neriX_analysis.save_plot(['temp_results'], c1, '1d_hists', batch_mode=True)
            f.savefig('./temp_results/2d_hist.png')

            #plt.show()

        #flat_s1_log_data = np.asarray(self.d_coincidence_data_information[self.l_cathode_settings_in_use[0]][self.l_degree_settings_in_use[0]]['a_log_s2_s1'].flatten(), dtype=np.float32)
        #flat_s1_log_mc = np.asarray(a_s1_s2_mc.flatten(), dtype=np.float32)
        
        flat_s1_log_data = np.asarray(self.d_coincidence_data_information[self.l_cathode_settings_in_use[0]][self.l_degree_settings_in_use[0]]['a_log_s2_s1'][self.d_coincidence_data_information[self.l_cathode_settings_in_use[0]][self.l_degree_settings_in_use[0]]['low_bin_s1']:self.d_coincidence_data_information[self.l_cathode_settings_in_use[0]][self.l_degree_settings_in_use[0]]['high_bin_s1'],self.d_coincidence_data_information[self.l_cathode_settings_in_use[0]][self.l_degree_settings_in_use[0]]['low_bin_log']:self.d_coincidence_data_information[self.l_cathode_settings_in_use[0]][self.l_degree_settings_in_use[0]]['high_bin_log']].flatten(), dtype=np.float32)
        flat_s1_log_mc = np.asarray(a_s1_s2_mc[self.d_coincidence_data_information[self.l_cathode_settings_in_use[0]][self.l_degree_settings_in_use[0]]['low_bin_s1']:self.d_coincidence_data_information[self.l_cathode_settings_in_use[0]][self.l_degree_settings_in_use[0]]['high_bin_s1'],self.d_coincidence_data_information[self.l_cathode_settings_in_use[0]][self.l_degree_settings_in_use[0]]['low_bin_log']:self.d_coincidence_data_information[self.l_cathode_settings_in_use[0]][self.l_degree_settings_in_use[0]]['high_bin_log']].flatten(), dtype=np.float32)


        flat_s1_log_data = np.asarray(self.d_coincidence_data_information[self.l_cathode_settings_in_use[0]][self.l_degree_settings_in_use[0]]['a_log_s2_s1'][self.d_coincidence_data_information[self.l_cathode_settings_in_use[0]][self.l_degree_settings_in_use[0]]['low_bin_s1']:self.d_coincidence_data_information[self.l_cathode_settings_in_use[0]][self.l_degree_settings_in_use[0]]['high_bin_s1'],self.d_coincidence_data_information[self.l_cathode_settings_in_use[0]][self.l_degree_settings_in_use[0]]['low_bin_log']:self.d_coincidence_data_information[self.l_cathode_settings_in_use[0]][self.l_degree_settings_in_use[0]]['high_bin_log']].flatten(), dtype=np.float32)
        flat_s1_log_mc = np.asarray(a_s1_s2_mc[self.d_coincidence_data_information[self.l_cathode_settings_in_use[0]][self.l_degree_settings_in_use[0]]['low_bin_s1']:self.d_coincidence_data_information[self.l_cathode_settings_in_use[0]][self.l_degree_settings_in_use[0]]['high_bin_s1'],self.d_coincidence_data_information[self.l_cathode_settings_in_use[0]][self.l_degree_settings_in_use[0]]['low_bin_log']:self.d_coincidence_data_information[self.l_cathode_settings_in_use[0]][self.l_degree_settings_in_use[0]]['high_bin_log']].flatten(), dtype=np.float32)
        logLikelihoodMatching = c_log_likelihood(flat_s1_log_data, flat_s1_log_mc, len(flat_s1_log_data), int(self.d_coincidence_data_information[self.l_cathode_settings_in_use[0]][self.l_degree_settings_in_use[0]]['num_data_pts']), 0.95)
        
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
    
    
    # band matching
    def ln_likelihood_full_matching_band(self, w_value, alpha, zeta, beta, gamma, delta, kappa, intrinsic_res_s1, intrinsic_res_s2, g1_value, spe_res_value, extraction_efficiency_value, gas_gain_mean_value, gas_gain_width_value, pf_eff_par0, pf_eff_par1, s2_eff_par0, s2_eff_par1, pf_stdev_par0, pf_stdev_par1, pf_stdev_par2, d_scale_pars, d_gpu_local_info, draw_fit=False):



        # -----------------------------------------------
        # -----------------------------------------------
        # determine prior likelihood and variables
        # -----------------------------------------------
        # -----------------------------------------------


        prior_ln_likelihood = 0
        matching_ln_likelihood = 0


        current_likelihood, w_value = self.get_w_value_default(w_value)
        prior_ln_likelihood += self.get_prior_log_likelihood_nuissance(current_likelihood)
        
        prior_ln_likelihood += self.get_prior_log_likelihood_alpha(alpha)
        prior_ln_likelihood += self.get_prior_log_likelihood_zeta(zeta)
        prior_ln_likelihood += self.get_prior_log_likelihood_beta(beta)
        prior_ln_likelihood += self.get_prior_log_likelihood_gamma(gamma)
        prior_ln_likelihood += self.get_prior_log_likelihood_delta(delta)
        prior_ln_likelihood += self.get_prior_log_likelihood_kappa(kappa)


        # priors of resolutions
        prior_ln_likelihood += self.get_prior_log_likelihood_resolution(intrinsic_res_s1)
        prior_ln_likelihood += self.get_prior_log_likelihood_resolution(intrinsic_res_s2)
        
        prior_ln_likelihood += self.get_prior_log_likelihood_scale(d_scale_pars)



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

        w_value = np.asarray(w_value, dtype=np.float32)
        alpha = np.asarray(alpha, dtype=np.float32)
        zeta = np.asarray(zeta, dtype=np.float32)
        beta = np.asarray(beta, dtype=np.float32)
        gamma = np.asarray(gamma, dtype=np.float32)
        delta = np.asarray(delta, dtype=np.float32)
        kappa = np.asarray(kappa, dtype=np.float32)

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
        
        # for histogram binning
        num_bins_s1 = np.asarray(self.l_s1_settings[0], dtype=np.int32)
        num_bins_log = np.asarray(self.l_log_settings[0], dtype=np.int32)
        a_hist_2d = np.zeros(self.l_s1_settings[0]*self.l_log_settings[0], dtype=np.float32)
        
        num_loops = np.asarray(self.num_loops, dtype=np.int32)



        for cathode_setting in self.l_cathode_settings_in_use:
        
            tArgs = (d_gpu_local_info['rng_states'], drv.In(num_trials), drv.In(mean_field), d_gpu_local_info['d_gpu_energy'][self.l_degree_settings_in_use[0]], drv.In(w_value), drv.In(alpha), drv.In(zeta), drv.In(beta), drv.In(gamma), drv.In(delta), drv.In(kappa), drv.In(g1_value), drv.In(extraction_efficiency), drv.In(gas_gain_value), drv.In(gas_gain_width), drv.In(spe_res), drv.In(intrinsic_res_s1), drv.In(intrinsic_res_s2), drv.In(a_pf_stdev), drv.In(pf_eff_par0), drv.In(pf_eff_par1), drv.In(s2_eff_par0), drv.In(s2_eff_par1), drv.In(num_bins_s1), d_gpu_local_info['gpu_bin_edges_s1'], drv.In(num_bins_log), d_gpu_local_info['gpu_bin_edges_log'], drv.InOut(a_hist_2d), drv.In(num_loops))

            d_gpu_local_info['function_to_call'](*tArgs, **self.d_gpu_scale)

            a_s1_s2_mc = np.reshape(a_hist_2d, (self.l_log_settings[0], self.l_s1_settings[0])).T

            sum_mc = np.sum(a_s1_s2_mc, dtype=np.float32)
            if sum_mc == 0:
                return -np.inf

            #a_s1_s2_mc = np.multiply(a_s1_s2_mc, np.sum(self.a_s1_s2, dtype=np.float32) / sum_mc)

            # if making PDF rather than scaling for rate
            scale_par = d_scale_pars[cathode_setting]
            scale_par *= float(self.num_mc_events*self.num_loops) / sum_mc

            a_s1_s2_mc = np.multiply(a_s1_s2_mc, float(scale_par)*self.d_coincidence_data_information[self.l_cathode_settings_in_use[0]][self.l_degree_settings_in_use[0]]['num_data_pts']/float(self.num_mc_events*self.num_loops))

            # likelihood for single energy
            if draw_fit:

                f, (ax1, ax2) = plt.subplots(2, sharex=True, sharey=True)

                s1_s2_data_plot = np.rot90(self.d_coincidence_data_information[self.l_cathode_settings_in_use[0]][self.l_degree_settings_in_use[0]]['a_log_s2_s1'])
                s1_s2_data_plot = np.flipud(s1_s2_data_plot)
                ax1.pcolormesh(self.a_s1_bin_edges, self.a_log_bin_edges, s1_s2_data_plot)

                s1_s2_mc_plot = np.rot90(a_s1_s2_mc)
                s1_s2_mc_plot = np.flipud(s1_s2_mc_plot)
                #print self.l_s1_settings
                #print self.l_log_settings
                #print self.d_coincidence_data_information[self.l_cathode_settings_in_use[0]][self.l_degree_settings_in_use[0]]['a_log_s2_s1'].shape
                #print a_s1_s2_mc.shape
                #print s1_s2_data_plot.shape
                #print s1_s2_mc_plot.shape
                ax2.pcolormesh(self.a_s1_bin_edges, self.a_log_bin_edges, s1_s2_mc_plot)
                #plt.colorbar()


                c1 = Canvas(1400, 400)
                c1.Divide(2)

                h_s1_data = Hist(*self.l_s1_settings, name='hS1_draw_data')
                root_numpy.array2hist(self.d_coincidence_data_information[self.l_cathode_settings_in_use[0]][self.l_degree_settings_in_use[0]]['a_log_s2_s1'].sum(axis=1), h_s1_data)

                hS1MC = Hist(*self.l_s1_settings, name='hS1_draw_mc')
                root_numpy.array2hist(a_s1_s2_mc.sum(axis=1), hS1MC)

                s1_scale_factor = h_s1_data.Integral() / float(hS1MC.Integral())

                g_s1_data = neriX_analysis.convert_hist_to_graph_with_poisson_errors(h_s1_data)
                g_s1_mc = neriX_analysis.convert_hist_to_graph_with_poisson_errors(hS1MC, scale=s1_scale_factor)

                g_s1_mc.SetFillColor(root.kBlue)
                g_s1_mc.SetMarkerColor(root.kBlue)
                g_s1_mc.SetLineColor(root.kBlue)
                g_s1_mc.SetFillStyle(3005)

                g_s1_data.SetTitle('S1 Comparison')
                g_s1_data.GetXaxis().SetTitle('S1 [PE]')
                g_s1_data.GetYaxis().SetTitle('Counts')

                g_s1_data.SetLineColor(root.kRed)
                g_s1_data.SetMarkerSize(0)
                g_s1_data.GetXaxis().SetRangeUser(self.l_s1_settings[1], self.l_s1_settings[2])
                g_s1_data.GetYaxis().SetRangeUser(0, 1.2*max(h_s1_data.GetMaximum(), hS1MC.GetMaximum()))

                c1.cd(1)
                g_s1_data.Draw('ap')
                g_s1_mc.Draw('same')
                g_s1_mc_band = g_s1_mc.Clone()
                g_s1_mc_band.Draw('3 same')

                h_s2_data = Hist(*self.l_log_settings, name='h_s2_draw_data')
                root_numpy.array2hist(self.d_coincidence_data_information[self.l_cathode_settings_in_use[0]][self.l_degree_settings_in_use[0]]['a_log_s2_s1'].sum(axis=0), h_s2_data)

                h_s2_mc = Hist(*self.l_log_settings, name='h_s2_draw_mc')
                root_numpy.array2hist(a_s1_s2_mc.sum(axis=0), h_s2_mc)

                s2_scale_factor = h_s2_data.Integral() / float(h_s2_mc.Integral())

                g_s2_data = neriX_analysis.convert_hist_to_graph_with_poisson_errors(h_s2_data)
                g_s2_mc = neriX_analysis.convert_hist_to_graph_with_poisson_errors(h_s2_mc, scale=s2_scale_factor)

                g_s2_mc.SetFillColor(root.kBlue)
                g_s2_mc.SetMarkerColor(root.kBlue)
                g_s2_mc.SetLineColor(root.kBlue)
                g_s2_mc.SetFillStyle(3005)

                g_s2_data.SetTitle('Log(S2/S1) Comparison')
                g_s2_data.GetXaxis().SetTitle('Log(S2/S1)')
                g_s2_data.GetYaxis().SetTitle('Counts')

                g_s2_data.SetLineColor(root.kRed)
                g_s2_data.SetMarkerSize(0)
                g_s2_data.GetXaxis().SetRangeUser(self.l_log_settings[1], self.l_log_settings[2])
                g_s2_data.GetYaxis().SetRangeUser(0, 1.2*max(h_s2_data.GetMaximum(), h_s2_mc.GetMaximum()))

                c1.cd(2)
                g_s2_data.Draw('ap')
                g_s2_mc.Draw('same')
                g_s2_mc_band = g_s2_mc.Clone()
                g_s2_mc_band.Draw('3 same')

                c1.Update()

                neriX_analysis.save_plot(['temp_results'], c1, '1d_hists', batch_mode=True)
                f.savefig('./temp_results/2d_hist.png')

                #plt.show()

            #flat_s1_log_data = np.asarray(self.d_coincidence_data_information[self.l_cathode_settings_in_use[0]][self.l_degree_settings_in_use[0]]['a_log_s2_s1'].flatten(), dtype=np.float32)
            #flat_s1_log_mc = np.asarray(a_s1_s2_mc.flatten(), dtype=np.float32)
            
            flat_s1_log_data = np.asarray(self.d_coincidence_data_information[self.l_cathode_settings_in_use[0]][self.l_degree_settings_in_use[0]]['a_log_s2_s1'][self.d_coincidence_data_information[self.l_cathode_settings_in_use[0]][self.l_degree_settings_in_use[0]]['low_bin_s1']:self.d_coincidence_data_information[self.l_cathode_settings_in_use[0]][self.l_degree_settings_in_use[0]]['high_bin_s1'],self.d_coincidence_data_information[self.l_cathode_settings_in_use[0]][self.l_degree_settings_in_use[0]]['low_bin_log']:self.d_coincidence_data_information[self.l_cathode_settings_in_use[0]][self.l_degree_settings_in_use[0]]['high_bin_log']].flatten(), dtype=np.float32)
            flat_s1_log_mc = np.asarray(a_s1_s2_mc[self.d_coincidence_data_information[self.l_cathode_settings_in_use[0]][self.l_degree_settings_in_use[0]]['low_bin_s1']:self.d_coincidence_data_information[self.l_cathode_settings_in_use[0]][self.l_degree_settings_in_use[0]]['high_bin_s1'],self.d_coincidence_data_information[self.l_cathode_settings_in_use[0]][self.l_degree_settings_in_use[0]]['low_bin_log']:self.d_coincidence_data_information[self.l_cathode_settings_in_use[0]][self.l_degree_settings_in_use[0]]['high_bin_log']].flatten(), dtype=np.float32)


            flat_s1_log_data = np.asarray(self.d_coincidence_data_information[self.l_cathode_settings_in_use[0]][self.l_degree_settings_in_use[0]]['a_log_s2_s1'][self.d_coincidence_data_information[self.l_cathode_settings_in_use[0]][self.l_degree_settings_in_use[0]]['low_bin_s1']:self.d_coincidence_data_information[self.l_cathode_settings_in_use[0]][self.l_degree_settings_in_use[0]]['high_bin_s1'],self.d_coincidence_data_information[self.l_cathode_settings_in_use[0]][self.l_degree_settings_in_use[0]]['low_bin_log']:self.d_coincidence_data_information[self.l_cathode_settings_in_use[0]][self.l_degree_settings_in_use[0]]['high_bin_log']].flatten(), dtype=np.float32)
            flat_s1_log_mc = np.asarray(a_s1_s2_mc[self.d_coincidence_data_information[self.l_cathode_settings_in_use[0]][self.l_degree_settings_in_use[0]]['low_bin_s1']:self.d_coincidence_data_information[self.l_cathode_settings_in_use[0]][self.l_degree_settings_in_use[0]]['high_bin_s1'],self.d_coincidence_data_information[self.l_cathode_settings_in_use[0]][self.l_degree_settings_in_use[0]]['low_bin_log']:self.d_coincidence_data_information[self.l_cathode_settings_in_use[0]][self.l_degree_settings_in_use[0]]['high_bin_log']].flatten(), dtype=np.float32)
            logLikelihoodMatching = c_log_likelihood(flat_s1_log_data, flat_s1_log_mc, len(flat_s1_log_data), int(self.d_coincidence_data_information[self.l_cathode_settings_in_use[0]][self.l_degree_settings_in_use[0]]['num_data_pts']), 0.95)
            
            matching_ln_likelihood += logLikelihoodMatching
        
        total_ln_likelihood = matching_ln_likelihood + prior_ln_likelihood

        if self.b_suppress_likelihood:
            total_ln_likelihood /= self.ll_suppression_factor

        #print logLikelihoodMatching, prior_ln_likelihood
        #print 'Total time: %f' % (time.time() - start_time_tot_ll)
        #print total_ln_likelihood

        if np.isnan(total_ln_likelihood):
            return -np.inf
        else:
            return total_ln_likelihood


        
    def wrapper_ln_likelihood_full_matching_band(self, a_parameters, kwargs={}):
        #print a_parameters
        #print '\n\n\n'
        d_scale_pars = {}
        for i, cathode_setting in enumerate(self.l_cathode_settings_in_use):
            d_scale_pars[cathode_setting] = a_parameters[-4+i]
        
        return self.ln_likelihood_full_matching_band(*(list(a_parameters[:-4]) + [d_scale_pars, a_parameters[-1]]), **kwargs)





    def ln_likelihood_full_matching_multiple_energies(self, a_py, a_qy, intrinsic_res_s1, intrinsic_res_s2, g1_value, spe_res_rv, g2_value, gas_gain_rv, gas_gain_width_rv, pf_eff_par0, pf_eff_par1, s2_eff_par0, s2_eff_par1, pf_stdev_par0, pf_stdev_par1, pf_stdev_par2, exciton_to_ion_par0_rv, exciton_to_ion_par1_rv, exciton_to_ion_par2_rv, d_gpu_local_info, draw_fit=False):



        # -----------------------------------------------
        # -----------------------------------------------
        # determine prior likelihood and variables
        # -----------------------------------------------
        # -----------------------------------------------


        prior_ln_likelihood = 0
        matching_ln_likelihood = 0


        # priors of yields
        # can do them all together since uniform within
        # the same range
        prior_ln_likelihood += self.get_prior_log_likelihood_light_yields(a_py)
        prior_ln_likelihood += self.get_prior_log_likelihood_charge_yields(a_qy)


        # priors of resolutions
        prior_ln_likelihood += self.get_prior_log_likelihood_resolution(intrinsic_res_s1)
        prior_ln_likelihood += self.get_prior_log_likelihood_resolution(intrinsic_res_s2)


        # get exciton to ion prior
        current_likelihood = self.get_likelihood_exciton_to_ion(exciton_to_ion_par0_rv, exciton_to_ion_par1_rv, exciton_to_ion_par2_rv)
        prior_ln_likelihood += self.get_prior_log_likelihood_nuissance(current_likelihood)


        # priors of detector variables
        current_likelihood, g1_value, g2_value = self.get_g1_g2_default(g1_value, g2_value)
        prior_ln_likelihood += self.get_prior_log_likelihood_nuissance(current_likelihood)

        current_likelihood, spe_res = self.get_spe_res_default(spe_res_rv)
        prior_ln_likelihood += self.get_prior_log_likelihood_nuissance(current_likelihood)

        current_likelihood, gas_gain_value = self.get_gas_gain_default(gas_gain_rv)
        prior_ln_likelihood += self.get_prior_log_likelihood_nuissance(current_likelihood)

        current_likelihood, gas_gain_width = self.get_gas_gain_width_default(gas_gain_width_rv)
        prior_ln_likelihood += self.get_prior_log_likelihood_nuissance(current_likelihood)


        # priors of efficiencies
        current_likelihood, pf_eff_par0, pf_eff_par1 = self.get_pf_eff_default(pf_eff_par0, pf_eff_par1)
        prior_ln_likelihood += self.get_prior_log_likelihood_nuissance(current_likelihood)

        current_likelihood, s2_eff_par0, s2_eff_par1 = self.get_s2_eff_default(s2_eff_par0, s2_eff_par1)
        prior_ln_likelihood += self.get_prior_log_likelihood_nuissance(current_likelihood)

        current_ln_likelihood, pf_stdev_par0, pf_stdev_par1, pf_stdev_par2 = self.get_pf_stdev_default(pf_stdev_par0, pf_stdev_par1, pf_stdev_par2)
        prior_ln_likelihood += self.get_prior_log_likelihood_nuissance(current_ln_likelihood)

        extraction_efficiency = g2_value / float(gas_gain_value)


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
        a_spline_energies = np.asarray(self.a_spline_energies, dtype=np.float32)
        num_spline_pts = np.asarray(len(self.a_spline_energies), dtype=np.int32)

        a_py = np.asarray(a_py, dtype=np.float32)
        a_qy = np.asarray(a_qy, dtype=np.float32)

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

        a_pf_stdev = np.asarray([pf_stdev_par0, pf_stdev_par1, pf_stdev_par2], dtype=np.float32)
        
        a_nr_band_cut = np.asarray([-1000000000, 0, 0, 1, 1000000000000], dtype=np.float32)

        # for histogram binning
        num_bins_s1 = np.asarray(self.l_s1_settings[0], dtype=np.int32)
        num_bins_log = np.asarray(self.l_log_settings[0], dtype=np.int32)

        for degree_setting in self.l_degree_settings_in_use:
        
            a_hist_2d = np.zeros(self.l_s1_settings[0]*self.l_log_settings[0], dtype=np.int32)
            
            #print d_gpu_local_info['d_gpu_energy'][degree_setting]
            
            tArgs = (d_gpu_local_info['rng_states'], drv.In(num_trials), drv.In(mean_field), d_gpu_local_info['d_gpu_energy'][degree_setting], drv.In(num_spline_pts), drv.In(a_spline_energies), drv.In(a_py), drv.In(a_qy), drv.In(g1_value), drv.In(extraction_efficiency), drv.In(gas_gain_value), drv.In(gas_gain_width), drv.In(spe_res), drv.In(intrinsic_res_s1), drv.In(intrinsic_res_s2), drv.In(a_pf_stdev), drv.In(exciton_to_ion_par0_rv), drv.In(exciton_to_ion_par1_rv), drv.In(exciton_to_ion_par2_rv), drv.In(pf_eff_par0), drv.In(pf_eff_par1), drv.In(s2_eff_par0), drv.In(s2_eff_par1), drv.In(a_nr_band_cut), drv.In(num_bins_s1), d_gpu_local_info['gpu_bin_edges_s1'], drv.In(num_bins_log), d_gpu_local_info['gpu_bin_edges_log'], drv.InOut(a_hist_2d))

            d_gpu_local_info['function_to_call'](*tArgs, **self.d_gpu_scale)
            

            a_s1_s2_mc = np.reshape(a_hist_2d, (self.l_log_settings[0], self.l_s1_settings[0])).T
            
            #print a_s1_s2_mc

            sum_mc = np.sum(a_s1_s2_mc, dtype=np.float32)
            if sum_mc == 0:
                #print 'sum mc == 0'
                return -np.inf

            scale_par = float(self.num_mc_events*self.num_loops) / sum_mc

            a_s1_s2_mc = np.multiply(a_s1_s2_mc, float(scale_par)*self.d_coincidence_data_information[self.l_cathode_settings_in_use[0]][degree_setting]['num_data_pts']/float(self.num_mc_events*self.num_loops))

            if draw_fit:

                f, (ax1, ax2) = plt.subplots(2, sharex=True, sharey=True)

                s1_s2_data_plot = np.rot90(self.d_coincidence_data_information[self.l_cathode_settings_in_use[0]][degree_setting]['a_log_s2_s1'])
                s1_s2_data_plot = np.flipud(s1_s2_data_plot)
                ax1.pcolormesh(self.a_s1_bin_edges, self.a_log_bin_edges, s1_s2_data_plot)

                s1_s2_mc_plot = np.rot90(a_s1_s2_mc)
                s1_s2_mc_plot = np.flipud(s1_s2_mc_plot)
                #print self.l_s1_settings
                #print self.l_log_settings
                #print self.d_coincidence_data_information[self.l_cathode_settings_in_use[0]][self.l_degree_settings_in_use[0]]['a_log_s2_s1'].shape
                #print a_s1_s2_mc.shape
                #print s1_s2_data_plot.shape
                #print s1_s2_mc_plot.shape
                ax2.pcolormesh(self.a_s1_bin_edges, self.a_log_bin_edges, s1_s2_mc_plot)
                #plt.colorbar()


                c1 = Canvas(1400, 400)
                c1.Divide(2)

                h_s1_data = Hist(*self.l_s1_settings, name='hS1_draw_data')
                root_numpy.array2hist(self.d_coincidence_data_information[self.l_cathode_settings_in_use[0]][degree_setting]['a_log_s2_s1'].sum(axis=1), h_s1_data)

                hS1MC = Hist(*self.l_s1_settings, name='hS1_draw_mc')
                root_numpy.array2hist(a_s1_s2_mc.sum(axis=1), hS1MC)

                s1_scale_factor = h_s1_data.Integral() / float(hS1MC.Integral())

                g_s1_data = neriX_analysis.convert_hist_to_graph_with_poisson_errors(h_s1_data)
                g_s1_mc = neriX_analysis.convert_hist_to_graph_with_poisson_errors(hS1MC, scale=s1_scale_factor)

                g_s1_mc.SetFillColor(root.kBlue)
                g_s1_mc.SetMarkerColor(root.kBlue)
                g_s1_mc.SetLineColor(root.kBlue)
                g_s1_mc.SetFillStyle(3005)

                g_s1_data.SetTitle('S1 Comparison')
                g_s1_data.GetXaxis().SetTitle('S1 [PE]')
                g_s1_data.GetYaxis().SetTitle('Counts')

                g_s1_data.SetLineColor(root.kRed)
                g_s1_data.SetMarkerSize(0)
                g_s1_data.GetXaxis().SetRangeUser(self.l_s1_settings[1], self.l_s1_settings[2])
                g_s1_data.GetYaxis().SetRangeUser(0, 1.2*max(h_s1_data.GetMaximum(), hS1MC.GetMaximum()))

                c1.cd(1)
                g_s1_data.Draw('ap')
                g_s1_mc.Draw('same')
                g_s1_mc_band = g_s1_mc.Clone()
                g_s1_mc_band.Draw('3 same')

                h_s2_data = Hist(*self.l_log_settings, name='h_s2_draw_data')
                root_numpy.array2hist(self.d_coincidence_data_information[self.l_cathode_settings_in_use[0]][degree_setting]['a_log_s2_s1'].sum(axis=0), h_s2_data)

                h_s2_mc = Hist(*self.l_log_settings, name='h_s2_draw_mc')
                root_numpy.array2hist(a_s1_s2_mc.sum(axis=0), h_s2_mc)

                s2_scale_factor = h_s2_data.Integral() / float(h_s2_mc.Integral())

                g_s2_data = neriX_analysis.convert_hist_to_graph_with_poisson_errors(h_s2_data)
                g_s2_mc = neriX_analysis.convert_hist_to_graph_with_poisson_errors(h_s2_mc, scale=s2_scale_factor)

                g_s2_mc.SetFillColor(root.kBlue)
                g_s2_mc.SetMarkerColor(root.kBlue)
                g_s2_mc.SetLineColor(root.kBlue)
                g_s2_mc.SetFillStyle(3005)

                g_s2_data.SetTitle('S2 Comparison')
                g_s2_data.GetXaxis().SetTitle('S2 [PE]')
                g_s2_data.GetYaxis().SetTitle('Counts')

                g_s2_data.SetLineColor(root.kRed)
                g_s2_data.SetMarkerSize(0)
                g_s2_data.GetXaxis().SetRangeUser(self.l_log_settings[1], self.l_log_settings[2])
                g_s2_data.GetYaxis().SetRangeUser(0, 1.2*max(h_s2_data.GetMaximum(), h_s2_mc.GetMaximum()))

                c1.cd(2)
                g_s2_data.Draw('ap')
                g_s2_mc.Draw('same')
                g_s2_mc_band = g_s2_mc.Clone()
                g_s2_mc_band.Draw('3 same')

                c1.Update()

                neriX_analysis.save_plot(['temp_results'], c1, '%d_deg_1d_hists' % (degree_setting), batch_mode=True)
                f.savefig('./temp_results/%d_deg_2d_hist.png' % (degree_setting))

            flat_s1_log_data = np.asarray(self.d_coincidence_data_information[self.l_cathode_settings_in_use[0]][degree_setting]['a_log_s2_s1'].flatten(), dtype=np.float32)
            flat_s1_log_mc = np.asarray(a_s1_s2_mc.flatten(), dtype=np.float32)
            logLikelihoodMatching = c_log_likelihood(flat_s1_log_data, flat_s1_log_mc, len(flat_s1_log_data), int(self.num_mc_events*self.num_loops), scale_par, int(self.d_coincidence_data_information[self.l_cathode_settings_in_use[0]][degree_setting]['num_data_pts']), 0.95)
            
            if np.isnan(logLikelihoodMatching):
                #print '\nbad matching:'
                #print logLikelihoodMatching
                #print flat_s1_log_data
                #print flat_s1_log_mc
                return -np.inf
            else:
                matching_ln_likelihood += logLikelihoodMatching
            
            #print degree_setting
            #print matching_ln_likelihood
                
        total_ln_likelihood = prior_ln_likelihood + matching_ln_likelihood
        #print total_ln_likelihood
                
        if np.isnan(total_ln_likelihood):
            return -np.inf
        else:
            return total_ln_likelihood
            
            
            
    def wrapper_ln_likelihood_full_matching_multiple_energies(self, a_parameters, kwargs={}):
        num_non_yield_pars = 17 + 1
        l_non_yield_parameters = a_parameters[-num_non_yield_pars:]
        l_pars = [list(a_parameters[0:(len(a_parameters)-num_non_yield_pars)/2]), list(a_parameters[(len(a_parameters)-num_non_yield_pars)/2:len(a_parameters)-num_non_yield_pars])]
        for par in l_non_yield_parameters:
            l_pars.append(par)
        #print l_pars
        #print 'call'
        return self.ln_likelihood_full_matching_multiple_energies(*l_pars, **kwargs)
        


    def ln_likelihood_full_matching_multiple_energies_lindhard_model(self, w_value_rv, alpha, zeta, beta, gamma, delta, kappa, eta, lamb, intrinsic_res_s1, intrinsic_res_s2, g1_value, spe_res_rv, g2_value, gas_gain_rv, gas_gain_width_rv, pf_eff_par0, pf_eff_par1, s2_eff_par0, s2_eff_par1, pf_stdev_par0, pf_stdev_par1, pf_stdev_par2, d_gpu_local_info, draw_fit=False):



        # -----------------------------------------------
        # -----------------------------------------------
        # determine prior likelihood and variables
        # -----------------------------------------------
        # -----------------------------------------------


        prior_ln_likelihood = 0
        matching_ln_likelihood = 0

        # get w-value and prior lieklihood
        current_likelihood, w_value = self.get_w_value_default(w_value_rv)
        prior_ln_likelihood += self.get_prior_log_likelihood_nuissance(current_likelihood)


        # priors of resolutions
        prior_ln_likelihood += self.get_prior_log_likelihood_resolution(intrinsic_res_s1)
        prior_ln_likelihood += self.get_prior_log_likelihood_resolution(intrinsic_res_s2)



        # priors of detector variables
        current_likelihood, g1_value, g2_value = self.get_g1_g2_default(g1_value, g2_value)
        prior_ln_likelihood += self.get_prior_log_likelihood_nuissance(current_likelihood)

        current_likelihood, spe_res = self.get_spe_res_default(spe_res_rv)
        prior_ln_likelihood += self.get_prior_log_likelihood_nuissance(current_likelihood)

        current_likelihood, gas_gain_value = self.get_gas_gain_default(gas_gain_rv)
        prior_ln_likelihood += self.get_prior_log_likelihood_nuissance(current_likelihood)

        current_likelihood, gas_gain_width = self.get_gas_gain_width_default(gas_gain_width_rv)
        prior_ln_likelihood += self.get_prior_log_likelihood_nuissance(current_likelihood)


        # priors of efficiencies
        current_likelihood, pf_eff_par0, pf_eff_par1 = self.get_pf_eff_default(pf_eff_par0, pf_eff_par1)
        prior_ln_likelihood += self.get_prior_log_likelihood_nuissance(current_likelihood)

        current_likelihood, s2_eff_par0, s2_eff_par1 = self.get_s2_eff_default(s2_eff_par0, s2_eff_par1)
        prior_ln_likelihood += self.get_prior_log_likelihood_nuissance(current_likelihood)

        current_ln_likelihood, pf_stdev_par0, pf_stdev_par1, pf_stdev_par2 = self.get_pf_stdev_default(pf_stdev_par0, pf_stdev_par1, pf_stdev_par2)
        prior_ln_likelihood += self.get_prior_log_likelihood_nuissance(current_ln_likelihood)

        extraction_efficiency = g2_value / float(gas_gain_value)
        
        
        # get priors from lindhard parameters
        prior_ln_likelihood += self.get_prior_log_likelihood_alpha(alpha)
        prior_ln_likelihood += self.get_prior_log_likelihood_zeta(zeta)
        prior_ln_likelihood += self.get_prior_log_likelihood_beta(beta)
        prior_ln_likelihood += self.get_prior_log_likelihood_gamma(gamma)
        prior_ln_likelihood += self.get_prior_log_likelihood_delta(delta)
        prior_ln_likelihood += self.get_prior_log_likelihood_kappa(kappa)
        prior_ln_likelihood += self.get_prior_log_likelihood_eta(eta)
        prior_ln_likelihood += self.get_prior_log_likelihood_lamb(lamb)


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

        w_value = np.asarray(w_value, dtype=np.float32)
        alpha = np.asarray(alpha, dtype=np.float32)
        zeta = np.asarray(zeta, dtype=np.float32)
        beta = np.asarray(beta, dtype=np.float32)
        gamma = np.asarray(gamma, dtype=np.float32)
        delta = np.asarray(delta, dtype=np.float32)
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

        pf_eff_par0 = np.asarray(pf_eff_par0, dtype=np.float32)
        pf_eff_par1 = np.asarray(pf_eff_par1, dtype=np.float32)

        s2_eff_par0 = np.asarray(s2_eff_par0, dtype=np.float32)
        s2_eff_par1 = np.asarray(s2_eff_par1, dtype=np.float32)

        a_pf_stdev = np.asarray([pf_stdev_par0, pf_stdev_par1, pf_stdev_par2], dtype=np.float32)
        
        a_nr_band_cut = np.asarray([-1000000000, 0, 0, 1, 1000000000000], dtype=np.float32)

        # for histogram binning
        num_bins_s1 = np.asarray(self.l_s1_settings[0], dtype=np.int32)
        num_bins_log = np.asarray(self.l_log_settings[0], dtype=np.int32)

        for degree_setting in self.l_degree_settings_in_use:
        
            a_hist_2d = np.zeros(self.l_s1_settings[0]*self.l_log_settings[0], dtype=np.int32)
            
            #print d_gpu_local_info['d_gpu_energy'][degree_setting]
            
            tArgs = (d_gpu_local_info['rng_states'], drv.In(num_trials), drv.In(mean_field), d_gpu_local_info['d_gpu_energy'][degree_setting], drv.In(w_value), drv.In(alpha), drv.In(zeta), drv.In(beta), drv.In(gamma), drv.In(delta), drv.In(kappa), drv.In(eta), drv.In(lamb), drv.In(g1_value), drv.In(extraction_efficiency), drv.In(gas_gain_value), drv.In(gas_gain_width), drv.In(spe_res), drv.In(intrinsic_res_s1), drv.In(intrinsic_res_s2), drv.In(a_pf_stdev), drv.In(pf_eff_par0), drv.In(pf_eff_par1), drv.In(s2_eff_par0), drv.In(s2_eff_par1), drv.In(a_nr_band_cut), drv.In(num_bins_s1), d_gpu_local_info['gpu_bin_edges_s1'], drv.In(num_bins_log), d_gpu_local_info['gpu_bin_edges_log'], drv.InOut(a_hist_2d))

            d_gpu_local_info['function_to_call'](*tArgs, **self.d_gpu_scale)
            

            a_s1_s2_mc = np.reshape(a_hist_2d, (self.l_log_settings[0], self.l_s1_settings[0])).T
            
            #print a_s1_s2_mc

            sum_mc = np.sum(a_s1_s2_mc, dtype=np.float32)
            if sum_mc == 0:
                #print 'sum mc == 0'
                return -np.inf

            scale_par = float(self.num_mc_events*self.num_loops) / sum_mc
            #print degree_setting
            #print prior_ln_likelihood
            #print 'scale par: %f' % (scale_par)
            #print 'normalization: %f\n' % (float(scale_par)*self.d_coincidence_data_information[self.l_cathode_settings_in_use[0]][degree_setting]['num_data_pts']/float(self.num_mc_events))

            a_s1_s2_mc = np.multiply(a_s1_s2_mc, float(scale_par)*self.d_coincidence_data_information[self.l_cathode_settings_in_use[0]][degree_setting]['num_data_pts']/float(self.num_mc_events*self.num_loops))

            #'ml'
            if not draw_fit:

                f, (ax1, ax2) = plt.subplots(2, sharex=True, sharey=True)
                
                ax1.set_xlabel('S1 [PE]')
                ax1.set_ylabel('log(S2/S1)')
                ax2.set_xlabel('S1 [PE]')
                ax2.set_ylabel('log(S2/S1)')

                s1_s2_data_plot = np.rot90(self.d_coincidence_data_information[self.l_cathode_settings_in_use[0]][degree_setting]['a_log_s2_s1'])
                s1_s2_data_plot = np.flipud(s1_s2_data_plot)
                ax1.pcolormesh(self.a_s1_bin_edges, self.a_log_bin_edges, s1_s2_data_plot)

                s1_s2_mc_plot = np.rot90(a_s1_s2_mc)
                s1_s2_mc_plot = np.flipud(s1_s2_mc_plot)
                #print self.l_s1_settings
                #print self.l_log_settings
                #print self.d_coincidence_data_information[self.l_cathode_settings_in_use[0]][self.l_degree_settings_in_use[0]]['a_log_s2_s1'].shape
                #print a_s1_s2_mc.shape
                #print s1_s2_data_plot.shape
                #print s1_s2_mc_plot.shape
                ax2.pcolormesh(self.a_s1_bin_edges, self.a_log_bin_edges, s1_s2_mc_plot)
                #plt.colorbar()


                c1 = Canvas(1400, 400)
                c1.Divide(2)

                h_s1_data = Hist(*self.l_s1_settings, name='hS1_draw_data')
                root_numpy.array2hist(self.d_coincidence_data_information[self.l_cathode_settings_in_use[0]][degree_setting]['a_log_s2_s1'].sum(axis=1), h_s1_data)

                hS1MC = Hist(*self.l_s1_settings, name='hS1_draw_mc')
                root_numpy.array2hist(a_s1_s2_mc.sum(axis=1), hS1MC)

                s1_scale_factor = h_s1_data.Integral() / float(hS1MC.Integral())

                g_s1_data = neriX_analysis.convert_hist_to_graph_with_poisson_errors(h_s1_data)
                g_s1_mc = neriX_analysis.convert_hist_to_graph_with_poisson_errors(hS1MC, scale=s1_scale_factor)

                g_s1_mc.SetFillColor(root.kBlue)
                g_s1_mc.SetMarkerColor(root.kBlue)
                g_s1_mc.SetLineColor(root.kBlue)
                g_s1_mc.SetFillStyle(3005)

                g_s1_data.SetTitle('S1 Comparison')
                g_s1_data.GetXaxis().SetTitle('S1 [PE]')
                g_s1_data.GetYaxis().SetTitle('Counts')

                g_s1_data.SetLineColor(root.kRed)
                g_s1_data.SetMarkerSize(0)
                g_s1_data.GetXaxis().SetRangeUser(self.l_s1_settings[1], self.l_s1_settings[2])
                g_s1_data.GetYaxis().SetRangeUser(0, 1.2*max(h_s1_data.GetMaximum(), hS1MC.GetMaximum()))

                c1.cd(1)
                g_s1_data.Draw('ap')
                g_s1_mc.Draw('same')
                g_s1_mc_band = g_s1_mc.Clone()
                g_s1_mc_band.Draw('3 same')

                h_s2_data = Hist(*self.l_log_settings, name='h_s2_draw_data')
                root_numpy.array2hist(self.d_coincidence_data_information[self.l_cathode_settings_in_use[0]][degree_setting]['a_log_s2_s1'].sum(axis=0), h_s2_data)

                h_s2_mc = Hist(*self.l_log_settings, name='h_s2_draw_mc')
                root_numpy.array2hist(a_s1_s2_mc.sum(axis=0), h_s2_mc)

                s2_scale_factor = h_s2_data.Integral() / float(h_s2_mc.Integral())

                g_s2_data = neriX_analysis.convert_hist_to_graph_with_poisson_errors(h_s2_data)
                g_s2_mc = neriX_analysis.convert_hist_to_graph_with_poisson_errors(h_s2_mc, scale=s2_scale_factor)

                g_s2_mc.SetFillColor(root.kBlue)
                g_s2_mc.SetMarkerColor(root.kBlue)
                g_s2_mc.SetLineColor(root.kBlue)
                g_s2_mc.SetFillStyle(3005)

                g_s2_data.SetTitle('Log(S2/S1) Comparison')
                g_s2_data.GetXaxis().SetTitle('Log(S2/S1)')
                g_s2_data.GetYaxis().SetTitle('Counts')

                g_s2_data.SetLineColor(root.kRed)
                g_s2_data.SetMarkerSize(0)
                g_s2_data.GetXaxis().SetRangeUser(self.l_log_settings[1], self.l_log_settings[2])
                g_s2_data.GetYaxis().SetRangeUser(0, 1.2*max(h_s2_data.GetMaximum(), h_s2_mc.GetMaximum()))

                c1.cd(2)
                g_s2_data.Draw('ap')
                g_s2_mc.Draw('same')
                g_s2_mc_band = g_s2_mc.Clone()
                g_s2_mc_band.Draw('3 same')

                c1.Update()

                neriX_analysis.save_plot(['temp_results'], c1, '%d_deg_1d_hists' % (degree_setting), batch_mode=True)
                f.savefig('./temp_results/%d_deg_2d_hist.png' % (degree_setting))

            flat_s1_log_data = np.asarray(self.d_coincidence_data_information[self.l_cathode_settings_in_use[0]][degree_setting]['a_log_s2_s1'].flatten(), dtype=np.float32)
            flat_s1_log_mc = np.asarray(a_s1_s2_mc.flatten(), dtype=np.float32)
            logLikelihoodMatching = c_log_likelihood(flat_s1_log_data, flat_s1_log_mc, len(flat_s1_log_data), int(self.num_mc_events*self.num_loops), scale_par, int(self.d_coincidence_data_information[self.l_cathode_settings_in_use[0]][degree_setting]['num_data_pts']), 0.95)
            
            #print logLikelihoodMatching
            
            if np.isnan(logLikelihoodMatching):
                #print '\nbad matching:'
                #print logLikelihoodMatching
                #print flat_s1_log_data
                #print flat_s1_log_mc
                return -np.inf
            else:
                matching_ln_likelihood += logLikelihoodMatching
        
            # in "ml"
            #print degree_setting
            #print matching_ln_likelihood
                
        total_ln_likelihood = prior_ln_likelihood + matching_ln_likelihood
        print total_ln_likelihood
        
        if self.b_suppress_likelihood:
            total_ln_likelihood /= self.ll_suppression_factor
                
        if np.isnan(total_ln_likelihood):
            return -np.inf
        else:
            return total_ln_likelihood
            
            
            
    def wrapper_ln_likelihood_full_matching_multiple_energies_lindhard_model(self, a_parameters, kwargs={}):
        return self.ln_likelihood_full_matching_multiple_energies_lindhard_model(*a_parameters, **kwargs)





    def initial_positions_for_ensemble(self, a_free_parameters, num_walkers):
    
        if self.fit_type == 's':
            l_par_names = ['py', 'qy', 'intrinsic_res_s1', 'intrinsic_res_s2', 'g1_value', 'spe_res_value', 'extraction_efficiency_value', 'gas_gain_mean_value', 'gas_gain_width_value', 'pf_eff_par0', 'pf_eff_par1', 's2_eff_par0', 's2_eff_par1', 'pf_stdev_par0', 'pf_stdev_par1', 'pf_stdev_par2', 'exciton_to_ion_par0_rv', 'exciton_to_ion_par1_rv', 'exciton_to_ion_par2_rv', 'prob_bkg', 'scale']
        elif self.fit_type == 'm':
            l_par_names = ['a_py', 'a_qy', 'intrinsic_res_s1', 'intrinsic_res_s2', 'g1_value', 'spe_res_rv', 'g2_value', 'gas_gain_rv', 'gas_gain_width_rv', 'pf_eff_par0', 'pf_eff_par1', 's2_eff_par0', 's2_eff_par1', 'pf_stdev_par0', 'pf_stdev_par1', 'pf_stdev_par2', 'exciton_to_ion_par0_rv', 'exciton_to_ion_par1_rv', 'exciton_to_ion_par2_rv']
        elif self.fit_type == 'ml':
            l_par_names = ['w_value_rv', 'alpha', 'zeta', 'beta', 'gamma', 'delta', 'kappa', 'eta', 'lambda', 'intrinsic_res_s1', 'intrinsic_res_s2', 'g1_value', 'spe_res_rv', 'g2_value', 'gas_gain_rv', 'gas_gain_width_rv', 'pf_eff_par0', 'pf_eff_par1', 's2_eff_par0', 's2_eff_par1', 'pf_stdev_par0', 'pf_stdev_par1', 'pf_stdev_par2']
        elif self.fit_type == 'mb':
            l_par_names = ['a_py', 'a_qy', 'intrinsic_res_s1', 'intrinsic_res_s2', 'g1_value', 'spe_res_rv', 'g2_value', 'gas_gain_rv', 'gas_gain_width_rv', 'pf_eff_par0', 'pf_eff_par1', 's1_eff_par0', 's1_eff_par1', 's2_eff_par0', 's2_eff_par1', 'pf_stdev_par0', 'pf_stdev_par1', 'pf_stdev_par2', 'exciton_to_ion_par0_rv', 'exciton_to_ion_par1_rv', 'exciton_to_ion_par2_rv']

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

        for par_name in l_par_names:
            # handle photon and charge yield initial positions
            if par_name == 'a_py':
                d_variable_arrays[par_name] = np.asarray([np.random.normal(a_free_parameters[i], 0.3*np.asarray(a_free_parameters[i]), size=num_walkers) for i in xrange(num_yields)])

            elif par_name == 'a_qy':
                d_variable_arrays[par_name] = np.asarray([np.random.normal(a_free_parameters[i], 0.3*np.asarray(a_free_parameters[i]), size=num_walkers) for i in xrange(num_yields, 2*num_yields)])
        
            elif par_name == 'py':
                d_variable_arrays[par_name] = np.asarray([np.random.normal(a_free_parameters[i], 0.05*np.asarray(a_free_parameters[i]), size=num_walkers) for i in xrange(num_yields)])

            elif par_name == 'qy':
                d_variable_arrays[par_name] = np.asarray([np.random.normal(a_free_parameters[i], 0.05*np.asarray(a_free_parameters[i]), size=num_walkers) for i in xrange(num_yields, 2*num_yields)])

            elif par_name == 'w_value_rv':
                # special case since w_value is an RV
                d_variable_arrays[par_name] = np.random.normal(a_free_parameters[0], 1, size=num_walkers)

            elif par_name == 'alpha':
                d_variable_arrays[par_name] = np.random.normal(a_free_parameters[1], a_free_parameters[1]*0.3, size=num_walkers)

            elif par_name == 'zeta':
                d_variable_arrays[par_name] = np.random.normal(a_free_parameters[2], a_free_parameters[2]*0.3, size=num_walkers)

            elif par_name == 'beta':
                d_variable_arrays[par_name] = np.random.normal(a_free_parameters[3], a_free_parameters[3]*0.3, size=num_walkers)

            elif par_name == 'gamma':
                d_variable_arrays[par_name] = np.random.normal(a_free_parameters[4], a_free_parameters[4]*0.3, size=num_walkers)

            elif par_name == 'delta':
                d_variable_arrays[par_name] = np.random.normal(a_free_parameters[5], a_free_parameters[5]*0.3, size=num_walkers)

            elif par_name == 'kappa':
                d_variable_arrays[par_name] = np.random.normal(a_free_parameters[6], a_free_parameters[6]*0.3, size=num_walkers)

            elif par_name == 'eta':
                d_variable_arrays[par_name] = np.random.normal(a_free_parameters[7], a_free_parameters[7]*0.3, size=num_walkers)

            elif par_name == 'lambda':
                d_variable_arrays[par_name] = np.random.normal(a_free_parameters[8], a_free_parameters[8]*0.3, size=num_walkers)

            elif par_name == 'intrinsic_res_s1':
                d_variable_arrays[par_name] = np.random.normal(a_free_parameters[2*num_yields+0], .04, size=num_walkers)

            elif par_name == 'intrinsic_res_s2':
                d_variable_arrays[par_name] = np.random.normal(a_free_parameters[2*num_yields+1], .04, size=num_walkers)


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


            elif par_name == 'pf_stdev_par0':
                d_variable_arrays['pf_stdev_par0'], d_variable_arrays['pf_stdev_par1'], d_variable_arrays['pf_stdev_par2'] = np.random.multivariate_normal(self.l_means_pf_stdev_pars, self.l_cov_matrix_pf_stdev_pars, size=num_walkers).T
                
            
            elif par_name == 'prob_bkg':
                d_variable_arrays[par_name] = np.random.normal(a_free_parameters[-2], .02, size=num_walkers)
                

            elif par_name == 'scale':
                d_variable_arrays[par_name] = np.random.normal(a_free_parameters[-1], .02, size=num_walkers)



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

            if self.fit_type == 's' and self.num_dimensions == 21:
                degree_setting = self.l_degree_settings_in_use[0]
                cathode_setting = self.l_cathode_settings_in_use[0]
                
                if degree_setting == 2300:
                    if cathode_setting == 1.054:
                        a_free_parameter_guesses = [7.24, 6.10, 0.07, 0.31, 0.25, 0.97]
                        # [ 7.24279474,  6.10423574,  0.06494089,  0.30873822, 0.25022568,  0.99936306] # 2300
                    elif cathode_setting == 0.345:
                        a_free_parameter_guesses = [7.04, 5.68, 0.23, 0.12, 0.29, 0.97]
                        # [ 7.04498957,  5.6815345,  0.23072485,  0.11705023, 0.28815432,  0.97119736] # 2300
                        
                    elif cathode_setting == 2.356:
                        a_free_parameter_guesses = [7.79, 6.27, 0.19, 0.18, 0.16, 0.97]
                        # [ 8.21997358,  6.06919105,  0.06841404 ,  0.46676469,  0.14950445, 0.98737901] # 2300
                        
                elif degree_setting == 3000:
                    if cathode_setting == 1.054:
                        a_free_parameter_guesses = [7.56, 5.76, 0.07, 0.26, 0.38, 0.97]
                        # [ 7.56098549,  5.76996929,  0.07485624 ,  0.25600894, 0.38586383,  0.98883613] # 3000
                    
                    elif cathode_setting == 0.345:
                        a_free_parameter_guesses = [8.58, 6.40, 0.06, 0.32, 0.28, 0.97]
                        # [ 8.58012424,  6.4082583,  0.059447,  0.32507151, 0.28044492,  0.97627979] # 3500
                        
                    elif cathode_setting == 2.356:
                        a_free_parameter_guesses = [8.20, 7.40, 0.07, 0.22, 0.28, 0.92]
                        # [ 8.20653994,  7.45641684,  0.06750863 ,  0.21867223,  0.28004732, 0.92374844] # 3000
                        
                elif degree_setting == 3500:
                    if cathode_setting == 1.054:
                        a_free_parameter_guesses = [7.71, 6.03, 0.04, 0.06, 0.32, 0.92]
                        # [ 7.71422274,  6.03296573,  0.03810478,  0.05889366, 0.31801688,  0.91945319] # 3500
                        
                    elif cathode_setting == 0.345:
                        a_free_parameter_guesses = [7.76, 6.17, 0.21, 0.26, 0.05, 0.93]
                        # [ 7.76790283,  6.17368043,  0.21353381,  0.26405303, 0.05305485,  0.92298929] # 3500
                        
                    elif cathode_setting == 2.356:
                        a_free_parameter_guesses = [7.79, 6.27, 0.19, 0.18, 0.16, 0.97]
                        # [ 7.78806321,  6.27176934,  0.19965031 ,  0.17596064,  0.164257, 0.99944881] # 3500
                        
                elif degree_setting == 4500:
                    if cathode_setting == 1.054:
                        a_free_parameter_guesses = [7.80, 5.83, 0.18, 0.23, 0.05, 0.97]
                        # [ 7.78910186,  5.832333,  0.17891269,  0.22816425, 0.02462351,  0.99851682] # 4500
                    
                    elif cathode_setting == 0.345:
                        a_free_parameter_guesses = [8.82, 5.96, 0.19, 0.04, 0.48, 0.97]
                        # [ 8.82078054,  5.96911495,  0.1908735 ,  0.02383425,  0.48874606, 0.99311512] # 4500
                        
                    elif cathode_setting == 2.356:
                        a_free_parameter_guesses = [7.65, 5.96, 0.22, 0.05, 0.32, 0.97]
                        # [ 7.65321071,  5.96063235,  0.2200952 ,  0.0500525,  0.31784079, 0.98759496] # 4500
                    
                elif degree_setting == 5300:
                    if cathode_setting == 1.054:
                        a_free_parameter_guesses = [9.78, 6.12, 0.27, 0.04, 0.17, 0.97]
                        # [ 9.7874405,  6.12108864,  0.27462468,  0.01749649, 0.16826173,  0.99297773] # 5300
                        
                    elif cathode_setting == 0.345:
                        a_free_parameter_guesses = [9.82, 6.18, 0.21, 0.02, 0.13, 0.96]
                        # [ 9.82589491,  6.18360509,  0.20806021,  0.01750014,  0.13195983,  0.96117175] # 5300
                        
                    elif cathode_setting == 2.356:
                        a_free_parameter_guesses = [9.82, 6.18, 0.21, 0.02, 0.13, 0.96]
                        # [ 9.760886,  5.96287371,  0.2028719,  0.03330193,  0.22689327,  0.99626046] # 5300


                else:
                    a_free_parameter_guesses = [8., 6., 0.25, 0.25, 0.95]

            elif self.fit_type == 'm':
                a_free_parameter_guesses = [7 for i in xrange(len(self.a_spline_energies))] + [6 for i in xrange(len(self.a_spline_energies))] + [0.25, 0.05]
                
            elif self.fit_type == 'ml':
                a_free_parameter_guesses = [0, 1.240, 0.0472, 239, 0.01385, 0.0620, 0.1394, 3.3, 1.14] + [0.25, 0.05]

            else:
                print '\nPlease run differential evolution minimizer for this setup and implement results in source code.\n'
                sys.exit()


            starting_pos = self.initial_positions_for_ensemble(a_free_parameter_guesses, num_walkers=num_walkers)

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
        if self.fit_type == 's' or self.fit_type == 'ml':
            num_dim = self.num_dimensions
        elif self.fit_type == 'm':
            num_dim = self.num_dimensions - 2 + 2*len(self.a_spline_energies)
        sampler = emcee.DESampler(num_walkers, num_dim, self.ln_likelihood_function_wrapper, threads=num_threads, autoscale_gamma=True, pool=self.gpu_pool, kwargs={})



        print '\n\nBeginning MCMC sampler\n\n'

        print '\nNumber of walkers * number of steps = %d * %d = %d function calls\n' % (num_walkers, num_steps, num_walkers*num_steps)

        start_time_mcmc = time.time()

        with click.progressbar(sampler.sample(p0=starting_pos, iterations=num_steps, rstate0=random_state, thin=thin), length=num_steps) as mcmc_sampler:
            for i, l_iterator_values in enumerate(mcmc_sampler):
                pass
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



    def differential_evolution_minimizer_free_pars(self, a_bounds, maxiter=250, tol=0.05, popsize=15):
        def neg_log_likelihood_diff_ev(a_guesses):
            if self.fit_type == 's':
                a_guesses = list(a_guesses[:-2]) + [test.g1_value, test.spe_res_value, test.extraction_efficiency_value, test.gas_gain_value, test.gas_gain_width, test.l_means_pf_eff_pars[0], test.l_means_pf_eff_pars[1], test.l_means_s2_eff_pars[0], test.l_means_s2_eff_pars[1], test.l_means_pf_stdev_pars[0], test.l_means_pf_stdev_pars[1], test.l_means_pf_stdev_pars[2], 0, 0, 0] + list(a_guesses[-2:])
                return -self.gpu_pool.map(self.wrapper_ln_likelihood_full_matching_single_energy, [a_guesses])[0]
            else:
                a_guesses = [13.7] + list(a_guesses[:8]) + [test.g1_value, test.spe_res_value, test.extraction_efficiency_value, test.gas_gain_value, test.gas_gain_width, test.l_means_pf_eff_pars[0], test.l_means_pf_eff_pars[1], test.l_means_s2_eff_pars[0], test.l_means_s2_eff_pars[1], test.l_means_pf_stdev_pars[0], test.l_means_pf_stdev_pars[1], test.l_means_pf_stdev_pars[2]] + list(a_guesses[-3:])
                return -self.gpu_pool.map(self.wrapper_ln_likelihood_full_matching_band, [a_guesses])[0]
            
        print '\n\nStarting differential evolution minimizer...\n\n'
        result = optimize.differential_evolution(neg_log_likelihood_diff_ev, a_bounds, disp=True, maxiter=maxiter, tol=tol, popsize=popsize)
        print result



    def suppress_likelihood(self, iterations=200, a_free_par_guesses=None):
    
        # reset variables in case this is not the first time run
        self.b_suppress_likelihood = False
        self.ll_suppression_factor = 1.

        if self.fit_type == 's' and a_free_par_guesses == None:
        
            degree_setting = self.l_degree_settings_in_use[0]
            cathode_setting = self.l_cathode_settings_in_use[0]
            
            
            if degree_setting == 2300 and cathode_setting == 1.054:
                l_free_pars = [7.24, 6.10, 0.07, 0.31, 0.25, 0.99]
            elif degree_setting == 2300 and cathode_setting == 0.345:
                l_free_pars = [7.04, 5.68, 0.23, 0.12, 0.29, 0.97]
            elif degree_setting == 2300 and cathode_setting == 2.356:
                l_free_pars = [7.79, 6.27, 0.19, 0.18, 0.16, 0.97]
            
            elif degree_setting == 3000 and cathode_setting == 1.054:
                l_free_pars = [7.56, 5.76, 0.07, 0.26, 0.38, 0.99]
            elif degree_setting == 3000 and cathode_setting == 0.345:
                l_free_pars = [8.58, 6.40, 0.06, 0.32, 0.28, 0.97]
            elif degree_setting == 3000 and cathode_setting == 2.356:
                l_free_pars = [8.20, 7.40, 0.07, 0.22, 0.28, 0.92]
            
            elif degree_setting == 3500 and cathode_setting == 1.054:
                l_free_pars = [7.71, 6.03, 0.04, 0.06, 0.32, 0.92]
            elif degree_setting == 3500 and cathode_setting == 0.345:
                l_free_pars = [7.76, 6.17, 0.21, 0.26, 0.05, 0.93]
            elif degree_setting == 3500 and cathode_setting == 2.356:
                l_free_pars = [7.79, 6.27, 0.19, 0.18, 0.16, 0.97]
            
            elif degree_setting == 4500 and cathode_setting == 1.054:
                l_free_pars = [7.80, 5.83, 0.18, 0.23, 0.024, 0.99]
            elif degree_setting == 4500 and cathode_setting == 0.345:
                l_free_pars = [8.82, 5.96, 0.19, 0.04, 0.48, 0.97]
            elif degree_setting == 4500 and cathode_setting == 2.356:
                l_free_pars = [7.65, 5.96, 0.22, 0.05, 0.32, 0.97]
            
            elif degree_setting == 5300 and cathode_setting == 1.054:
                l_free_pars = [9.78, 6.12, 0.27, 0.04, 0.17, 0.97]
            elif degree_setting == 5300 and cathode_setting == 0.345:
                l_free_pars = [9.82, 6.18, 0.21, 0.02, 0.13, 0.96]
            elif degree_setting == 5300 and cathode_setting == 2.356:
                l_free_pars = [9.76, 5.96, 0.20, 0.03, 0.22, 0.99]
            
            else:
                print 'Not set up for %d at %.3f yet!' % (degree_setting, cathode_setting)
                sys.exit()
            
        
        
            a_free_par_guesses = l_free_pars[:4] + [test.g1_value, test.spe_res_value, test.extraction_efficiency_value, test.gas_gain_value, test.gas_gain_width, test.l_means_pf_eff_pars[0], test.l_means_pf_eff_pars[1], test.l_means_s2_eff_pars[0], test.l_means_s2_eff_pars[1], test.l_means_pf_stdev_pars[0], test.l_means_pf_stdev_pars[1], test.l_means_pf_stdev_pars[2], 0, 0, 0] + l_free_pars[-2:]
        
        
        
        elif self.fit_type == 'm' and a_free_par_guesses == None:
            a_free_par_guesses = [3, 4, 5, 6, 7, 8, 9, 7, 6.5, 6, 6, 5.5, 5.5, 5., 0.1, 0.1, self.l_means_g1_g2[0], 0., 20.89, 0.5, 0., self.l_means_pf_eff_pars[0], self.l_means_pf_eff_pars[1], self.l_means_s2_eff_pars[0], self.l_means_s2_eff_pars[1], self.l_means_pf_stdev_pars[0], self.l_means_pf_stdev_pars[1], self.l_means_pf_stdev_pars[2], 0, 0, 0]
            #a_free_par_guesses = list(self.a_nest_photon_yields) + list(self.a_nest_charge_yields) + [0.1, 0.1, self.l_means_g1_g2[0], 0., 20.89, 0.5, 0., self.l_means_pf_eff_pars[0], self.l_means_pf_eff_pars[1], self.l_means_s2_eff_pars[0], self.l_means_s2_eff_pars[1], self.l_means_pf_stdev_pars[0], self.l_means_pf_stdev_pars[1], self.l_means_pf_stdev_pars[2], 0, 0, 0]
        elif self.fit_type == 'ml' and a_free_par_guesses == None:
            a_free_par_guesses = [0, 1.240, 0.0472, 239, 0.01385, 0.0620, 0.1394, 3.3, 1.14] + [0.25, 0.05] + [self.l_means_g1_g2[0], 0., 20.89, 0.5, 0., self.l_means_pf_eff_pars[0], self.l_means_pf_eff_pars[1], self.l_means_s2_eff_pars[0], self.l_means_s2_eff_pars[1], self.l_means_pf_stdev_pars[0], self.l_means_pf_stdev_pars[1], self.l_means_pf_stdev_pars[2]]
        
        #print a_free_par_guesses
        l_parameters = [a_free_par_guesses for i in xrange(iterations)]
        l_log_likelihoods = self.gpu_pool.map(self.ln_likelihood_function_wrapper, l_parameters)
        #print l_log_likelihoods

        std_ll = np.std(l_log_likelihoods)

        print 'Standard deviation for %.3e MC iterations is %f' % (self.num_mc_events, std_ll)
        print 'Will scale LL such that stdev is 0.1'

        if std_ll < 0.1:
            self.b_suppress_likelihood = True
            self.ll_suppression_factor = 1.
            print 'Standard deviation already small so not supressing\n\n'
        else:
            self.b_suppress_likelihood = True
            self.ll_suppression_factor = std_ll / 0.1
            print 'LL suppression factor: %f\n' % self.ll_suppression_factor







if __name__ == '__main__':
    copy_reg.pickle(types.MethodType, reduce_method)

    # d_coincidence_data should be in the following form:
    # d_coincidence_data['degree_settings'] = [2300, 3000, ...]
    # d_coincidence_data['cathode_settings'] = [0.345, 1.054, ...]
    # d_coincidence_data['cathode_settings'] = [4.5]

    """
    d_coincidence_data = {}
    d_coincidence_data['degree_settings'] = [4500]
    d_coincidence_data['cathode_settings'] = [0.345]

    test = fit_nr('s', d_coincidence_data, num_mc_events=2.5e6, num_gpus=3, num_loops=4)
    test.suppress_likelihood()
    
    #ln_likelihood_full_matching_single_energy(self, py, qy, intrinsic_res_s1, intrinsic_res_s2, g1_value, spe_res_value, extraction_efficiency_value, gas_gain_mean_value, gas_gain_width_value, pf_eff_par0, pf_eff_par1, s2_eff_par0, s2_eff_par1, pf_stdev_par0, pf_stdev_par1, pf_stdev_par2, exciton_to_ion_par0_rv, exciton_to_ion_par1_rv, exciton_to_ion_par2_rv, d_gpu_local_info, draw_fit=False):
    
    # 2300, 1054: [7.24, 6.10, 0.07, 0.31, 0.25, 0.99]
    # 3000, 1054: [7.95, 5.84, 0.05, 0.34, 0.32, 0.99]
    # 3500, 1054: [7.71, 6.03, 0.04, 0.06, 0.32, 0.92]
    # 4500, 1054: [7.80, 5.83, 0.18, 0.23, 0.024, 0.99]
    # 5300, 1054: [9.69, 6.04, 0.23, 0.05, 0.17, 0.98]
    #l_test_parameters_single_energy = [9.69, 6.04, 0.23, 0.05, test.g1_value, test.spe_res_value, test.extraction_efficiency_value, test.gas_gain_value, test.gas_gain_width, test.l_means_pf_eff_pars[0], test.l_means_pf_eff_pars[1], test.l_means_s2_eff_pars[0], test.l_means_s2_eff_pars[1], test.l_means_pf_stdev_pars[0], test.l_means_pf_stdev_pars[1], test.l_means_pf_stdev_pars[2], 0, 0, 0, 0.17, 0.98]
    #test.gpu_pool.map(test.wrapper_ln_likelihood_full_matching_single_energy, [l_test_parameters_single_energy])

    #a_free_par_bounds = [(3., 15.), (3.5, 15.), (0.01, 0.5), (0.01, 0.5), (0., 1.), (0, 1.2)]
    #test.differential_evolution_minimizer_free_pars(a_free_par_bounds, maxiter=50, popsize=15, tol=0.05)
    
    test.run_mcmc(num_steps=500, num_walkers=256)
    """
    
    
    d_coincidence_data = {}
    d_coincidence_data['degree_settings'] = [-4]
    d_coincidence_data['cathode_settings'] = [0.345, 1.054, 2.356]
    
    test = fit_nr('bl', d_coincidence_data, num_mc_events=5e5, num_gpus=3, num_loops=4)
    #test.suppress_likelihood()
    
    # ln_likelihood_full_matching_band(self, w_value, alpha, zeta, beta, gamma, delta, kappa, intrinsic_res_s1, intrinsic_res_s2, g1_value, spe_res_value, extraction_efficiency_value, gas_gain_mean_value, gas_gain_width_value, pf_eff_par0, pf_eff_par1, s2_eff_par0, s2_eff_par1, pf_stdev_par0, pf_stdev_par1, pf_stdev_par2, d_scale_par, d_gpu_local_info, draw_fit=False)
    
    #l_test_parameters_single_energy = [13.7, 1.24, 0.047, 239, 0.01385, 0.0620, 0.1394, 0.1, 0.2, test.g1_value, test.spe_res_value, test.extraction_efficiency_value, test.gas_gain_value, test.gas_gain_width, test.l_means_pf_eff_pars[0], test.l_means_pf_eff_pars[1], test.l_means_s2_eff_pars[0], test.l_means_s2_eff_pars[1], test.l_means_pf_stdev_pars[0], test.l_means_pf_stdev_pars[1], test.l_means_pf_stdev_pars[2], 0.99, 0.99, 0.99]
    #test.gpu_pool.map(test.wrapper_ln_likelihood_full_matching_band, [l_test_parameters_single_energy])

    a_free_par_bounds = [(0., 5.), (0., 0.15), (0., 800), (0., 0.03), (0., 0.15), (0, 0.4), (0, 0.5), (0, 0.5), (0, 1), (0, 1), (0, 1)]
    test.differential_evolution_minimizer_free_pars(a_free_par_bounds, maxiter=50, popsize=15, tol=0.05)
    
    #test.run_mcmc(num_steps=500, num_walkers=256)
    
    
   
    
    
    test.close_workers()



