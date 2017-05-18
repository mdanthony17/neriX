#!/usr/bin/python

import sys
import neriX_analysis
import os, re

sys.path.insert(0, '..')
import neriX_simulation_config

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

from sklearn import neighbors
from sklearn import grid_search
from sklearn import preprocessing


import cPickle as pickle



l_s1_cuts = [0, 10, 20, 40]
transparency = 0.2
l_quantiles_for_2d = [50]

# num_steps goes into how many random draws you will take from kept sampler
num_steps = 5
# num_steps_to_include is how large of sampler you keep
num_steps_to_include = 1000
num_mc_events = int(1e5)

if(len(sys.argv) != 2):
	print 'Usage is python compare_data_fit_single_energy.py <num walkers>'
	sys.exit(1)

num_walkers = int(sys.argv[1])

d_degree_setting_to_energy_name = {2300:3,
                                   3000:5,
                                   3500:7,
                                   4500:10,
                                   5300:15,
                                   6200:20}

d_cathode_voltages_to_field = {0.345:190,
                               1.054:490,
                               2.356:1020} # in kV:V/cm


dir_specifier_name = 'band_with_lindhard_model'


l_degree_settings_in_use = [-4]
s_degree_settings = ''
for degree_setting in l_degree_settings_in_use:
    s_degree_settings += '%s,' % (degree_setting)
s_degree_settings = s_degree_settings[:-1]


l_cathode_settings_in_use = [0.345, 1.054, 2.356]
s_cathode_settings = ''
for cathode_setting in l_cathode_settings_in_use:
    s_cathode_settings += '%.3f,' % (cathode_setting)
s_cathode_settings = s_cathode_settings[:-1]

name_of_results_directory = neriX_simulation_config.results_directory_name
l_plots = ['plots', dir_specifier_name, '%s_kV' % s_cathode_settings]

s_path_to_file = './%s/%s/%s_kV_%s_deg/sampler_dictionary.p' % (name_of_results_directory, dir_specifier_name, s_cathode_settings, s_degree_settings)


if os.path.exists(s_path_to_file):
    dSampler = pickle.load(open(s_path_to_file, 'r'))
    l_chains = []
    for sampler in dSampler[num_walkers]:
        l_chains.append(sampler['_chain'])

    a_full_sampler = np.concatenate(l_chains, axis=1)

    print 'Successfully loaded sampler!'
else:
    print s_path_to_file
    print 'Could not find file!'
    sys.exit()

total_length_sampler = len(a_full_sampler)

# get block and grid size
d_gpu_scale = {}
block_dim = 1024
d_gpu_scale['block'] = (block_dim,1,1)
num_blocks = floor(num_mc_events / float(block_dim))
d_gpu_scale['grid'] = (int(num_blocks), 1)
num_mc_events = int(num_blocks*block_dim)


# get significant value sets to loop over later
a_samples = a_full_sampler[:, -num_steps_to_include:, :].reshape((-1, a_full_sampler.shape[2]))
a_percentile_values = np.percentile(a_samples, l_quantiles_for_2d, axis=0)


l_s1_settings = [40, 0, 40]
l_log_settings = [40, 1, 3.5]
l_s2_settings = [40, 0, 2000]

mc_bin_number_multiplier = 5
figure_sizes = (11, 13)
d_subplot_space = {'wspace':0.2, 'hspace':0.3}





# need to prepare GPU for MC simulations
import cuda_full_observables_production
from pycuda.compiler import SourceModule
import pycuda.driver as drv
import pycuda.tools
import pycuda.gpuarray

drv.init()
dev = drv.Device(0)
ctx = dev.make_context()
print 'Device Name: %s\n' % (dev.name())



d_plotting_information = {}
for cathode_setting in l_cathode_settings_in_use:
    d_plotting_information[cathode_setting] = {}
    
    for degree_setting in l_degree_settings_in_use:
        d_plotting_information[cathode_setting][degree_setting] = {}

        path_to_reduced_energy_spectra = '%s/reduced_simulation_data/' % (neriX_simulation_config.path_to_this_module)
        current_path_to_reduced_energy_spectra = '%snerixsim-pure_nr_spec.root' % (path_to_reduced_energy_spectra)
        f_reduced = File(current_path_to_reduced_energy_spectra, 'read')
        h_energy = f_reduced.h_mc


        a_energy = np.zeros(num_mc_events, dtype=np.float32)
        for i in tqdm.tqdm(xrange(num_mc_events)):
            a_energy[i] = h_energy.GetRandom()


        print 'Putting energy array on GPU...'
        d_plotting_information[cathode_setting][degree_setting]['gpu_energy'] = pycuda.gpuarray.to_gpu(a_energy)
        d_plotting_information[cathode_setting][degree_setting]['a_energy'] = a_energy





# get random seeds setup
local_gpu_setup_kernel = pycuda.compiler.SourceModule(cuda_full_observables_production.cuda_full_observables_production_code, no_extern_c=True).get_function('setup_kernel')
local_rng_states = drv.mem_alloc(np.int32(num_blocks*block_dim)*pycuda.characterize.sizeof('curandStateXORWOW', '#include <curand_kernel.h>'))
local_gpu_setup_kernel(np.int32(int(num_blocks*block_dim)), local_rng_states, np.uint64(0), np.uint64(0), grid=(int(num_blocks), 1), block=(int(block_dim), 1, 1))


# get observables function
gpu_observables_func = SourceModule(cuda_full_observables_production.cuda_full_observables_production_code, no_extern_c=True).get_function('gpu_full_observables_production_with_arrays_gompertz')



for cathode_setting in l_cathode_settings_in_use:
    for degree_setting in l_degree_settings_in_use:

        # get s1 and s2 data arrays
        d_plotting_information[cathode_setting][degree_setting]['data_save_directory'] = '../../coincidence_analysis/results/%.3fkV_%ddeg/' % (cathode_setting, degree_setting)
        d_plotting_information[cathode_setting][degree_setting]['d_s1_s2_data'] = pickle.load(open('%ss1_s2.pkl' % (d_plotting_information[cathode_setting][degree_setting]['data_save_directory']), 'r'))
        d_plotting_information[cathode_setting][degree_setting]['a_s1_data'] = d_plotting_information[cathode_setting][degree_setting]['d_s1_s2_data']['s1']
        d_plotting_information[cathode_setting][degree_setting]['a_s2_data'] = d_plotting_information[cathode_setting][degree_setting]['d_s1_s2_data']['s2']

        # get number of data pts
        d_plotting_information[cathode_setting][degree_setting]['num_data_pts'] = len(d_plotting_information[cathode_setting][degree_setting]['a_s1_data'])


        # create figure and give settings for space
        fig_s1_log, (ax_s1_projection, ax_s1_log_data, ax_s1_log_mc) = plt.subplots(3, sharex=False, sharey=False, figsize=figure_sizes)
        fig_s1_log.subplots_adjust(**d_subplot_space)


        # create 2d histogram of data
        hist_s1_log_data = ax_s1_log_data.hist2d(d_plotting_information[cathode_setting][degree_setting]['a_s1_data'], np.log10(d_plotting_information[cathode_setting][degree_setting]['a_s2_data']/d_plotting_information[cathode_setting][degree_setting]['a_s1_data']), bins=[l_s1_settings[0], l_log_settings[0]], range=[[l_s1_settings[1], l_s1_settings[2]], [l_log_settings[1], l_log_settings[2]]], cmap='Blues')
        fig_s1_log.colorbar(hist_s1_log_data[3], ax=ax_s1_log_data)





        # -----------------------------------------------
        # -----------------------------------------------
        # run MC
        # -----------------------------------------------
        # -----------------------------------------------


        for quantile_number, a_fit_parameters in enumerate(a_percentile_values):

            num_trials = np.asarray(num_mc_events, dtype=np.int32)
            mean_field = np.asarray(d_cathode_voltages_to_field[cathode_setting], dtype=np.float32)

            w_value = np.asarray(a_fit_parameters[0], dtype=np.float32)
            kappa = np.asarray(a_fit_parameters[1], dtype=np.float32)

            g1_value = np.asarray(a_fit_parameters[4], dtype=np.float32)
            spe_res = np.asarray(a_fit_parameters[5], dtype=np.float32)
            extraction_efficiency = np.asarray(a_fit_parameters[6], dtype=np.float32)
            gas_gain_value = np.asarray(a_fit_parameters[7], dtype=np.float32)
            gas_gain_width = np.asarray(a_fit_parameters[8], dtype=np.float32)

            intrinsic_res_s1 = np.asarray(a_fit_parameters[2], dtype=np.float32)
            intrinsic_res_s2 = np.asarray(a_fit_parameters[3], dtype=np.float32)

            pf_eff_par0 = np.asarray(a_fit_parameters[9], dtype=np.float32)
            pf_eff_par1 = np.asarray(a_fit_parameters[10], dtype=np.float32)

            s2_eff_par0 = np.asarray(a_fit_parameters[11], dtype=np.float32)
            s2_eff_par1 = np.asarray(a_fit_parameters[12], dtype=np.float32)

            a_pf_stdev = np.asarray([a_fit_parameters[13], a_fit_parameters[14], a_fit_parameters[15]], dtype=np.float32)
            
            dpe_prob = np.asarray(a_fit_parameters[16], dtype=np.float32)

            a_s1_mc = np.zeros(num_mc_events, dtype=np.float32)
            a_s2_mc = np.zeros(num_mc_events, dtype=np.float32)
            
            if cathode_setting == 0.345:
                mult_factor = 3
                gom_par0 = np.asarray(a_fit_parameters[-mult_factor*6 + 0], dtype=np.float32)
                gom_par1 = np.asarray(a_fit_parameters[-mult_factor*6 + 1], dtype=np.float32)
                gom_par2 = np.asarray(a_fit_parameters[-mult_factor*6 + 2], dtype=np.float32)
                gom_par3 = np.asarray(a_fit_parameters[-mult_factor*6 + 3], dtype=np.float32)
            
                exciton_to_ion_ratio = np.asarray(a_fit_parameters[-mult_factor*6 + 4], dtype=np.float32)
            
                scale_par = np.asarray(a_fit_parameters[-mult_factor*6 + 5], dtype=np.float32)
            
            elif cathode_setting == 1.054:
                mult_factor = 2
                gom_par0 = np.asarray(a_fit_parameters[-mult_factor*6 + 0], dtype=np.float32)
                gom_par1 = np.asarray(a_fit_parameters[-mult_factor*6 + 1], dtype=np.float32)
                gom_par2 = np.asarray(a_fit_parameters[-mult_factor*6 + 2], dtype=np.float32)
                gom_par3 = np.asarray(a_fit_parameters[-mult_factor*6 + 3], dtype=np.float32)
            
                exciton_to_ion_ratio = np.asarray(a_fit_parameters[-mult_factor*6 + 4], dtype=np.float32)
            
                scale_par = np.asarray(a_fit_parameters[-mult_factor*6 + 5], dtype=np.float32)
            
            elif cathode_setting == 2.356:
                mult_factor = 1
                gom_par0 = np.asarray(a_fit_parameters[-mult_factor*6 + 0], dtype=np.float32)
                gom_par1 = np.asarray(a_fit_parameters[-mult_factor*6 + 1], dtype=np.float32)
                gom_par2 = np.asarray(a_fit_parameters[-mult_factor*6 + 2], dtype=np.float32)
                gom_par3 = np.asarray(a_fit_parameters[-mult_factor*6 + 3], dtype=np.float32)
            
                exciton_to_ion_ratio = np.asarray(a_fit_parameters[-mult_factor*6 + 4], dtype=np.float32)
            
                scale_par = np.asarray(a_fit_parameters[-mult_factor*6 + 5], dtype=np.float32)
            


            tArgs = (local_rng_states, drv.In(num_trials), drv.In(mean_field), d_plotting_information[cathode_setting][degree_setting]['gpu_energy'], drv.In(w_value), drv.In(exciton_to_ion_ratio), drv.In(gom_par0), drv.In(gom_par1), drv.In(gom_par2), drv.In(gom_par3), drv.In(kappa), drv.In(g1_value), drv.In(extraction_efficiency), drv.In(gas_gain_value), drv.In(gas_gain_width), drv.In(spe_res), drv.In(intrinsic_res_s1), drv.In(intrinsic_res_s2), drv.In(a_pf_stdev), drv.In(pf_eff_par0), drv.In(pf_eff_par1), drv.In(s2_eff_par0), drv.In(s2_eff_par1), drv.In(dpe_prob), drv.InOut(a_s1_mc), drv.InOut(a_s2_mc))
            
            gpu_observables_func(*tArgs, **d_gpu_scale)



        hist_s1_log_mc = ax_s1_log_mc.hist2d(a_s1_mc, np.log10(a_s2_mc/a_s1_mc), bins=[l_s1_settings[0], l_log_settings[0]], range=[[l_s1_settings[1], l_s1_settings[2]], [l_log_settings[1], l_log_settings[2]]], cmap='Blues', normed=True)
        fig_s1_log.colorbar(hist_s1_log_mc[3], ax=ax_s1_log_mc)


        df_data = pd.DataFrame({'s1':d_plotting_information[cathode_setting][degree_setting]['a_s1_data'], 's2':d_plotting_information[cathode_setting][degree_setting]['a_s2_data']})
        df_data['log'] = np.log10(df_data['s2']/df_data['s1'])

        df_mc = pd.DataFrame({'s1':a_s1_mc, 's2':a_s2_mc})
        df_mc['log'] = np.log10(df_mc['s2']/df_mc['s1'])



        # add alii to dictionary
        d_plotting_information[cathode_setting][degree_setting]['fig_s1_log'] = fig_s1_log
        d_plotting_information[cathode_setting][degree_setting]['ax_s1_projection'] = ax_s1_projection
        d_plotting_information[cathode_setting][degree_setting]['ax_s1_log_data'] = ax_s1_log_data
        d_plotting_information[cathode_setting][degree_setting]['ax_s1_log_mc'] = ax_s1_log_mc
        d_plotting_information[cathode_setting][degree_setting]['hist_s1_log_data'] = hist_s1_log_data
        d_plotting_information[cathode_setting][degree_setting]['df_data'] = df_data
        d_plotting_information[cathode_setting][degree_setting]['df_mc'] = df_mc



        # get from last histogram
        s1_edges = hist_s1_log_data[1]
        log_edges = hist_s1_log_data[2]
        s2_edges = np.linspace(l_s2_settings[1], l_s2_settings[2], l_s2_settings[0]+1)

        s1_edges_mc = np.linspace(l_s1_settings[1], l_s1_settings[2], mc_bin_number_multiplier*l_s1_settings[0]+1)
        s1_mc_bin_width = (l_s1_settings[2]-l_s1_settings[1])/float(l_s1_settings[0]*mc_bin_number_multiplier)
        s1_bin_centers_mc = np.linspace(l_s1_settings[1]+s1_mc_bin_width/2., l_s1_settings[2]-s1_mc_bin_width/2., mc_bin_number_multiplier*l_s1_settings[0])
        s2_edges_mc = np.linspace(l_s2_settings[1], l_s2_settings[2], mc_bin_number_multiplier*l_s2_settings[0]+1)
        s2_mc_bin_width = (l_s2_settings[2]-l_s2_settings[1])/float(l_s2_settings[0]*mc_bin_number_multiplier)
        s2_bin_centers_mc = np.linspace(l_s2_settings[1]+s2_mc_bin_width/2., l_s2_settings[2]-s2_mc_bin_width/2., mc_bin_number_multiplier*l_s2_settings[0])
        log_edges_mc = np.linspace(l_log_settings[1], l_log_settings[2], mc_bin_number_multiplier*l_log_settings[0]+1)

        df_histograms = {}
        df_histograms['s1'] = np.zeros((num_walkers*num_steps, len(s1_edges_mc)-1), dtype=float)

        df_histograms['s2'] = {}
        d_s2_data_slices = {}
        for s1_cut_num in xrange(len(l_s1_cuts)-1):
            current_set_s1_cuts = (l_s1_cuts[s1_cut_num], l_s1_cuts[s1_cut_num+1])
            df_histograms['s2'][current_set_s1_cuts] = np.zeros((num_walkers*num_steps, len(s1_edges_mc)-1), dtype=float)
            
            # get data slices
            cut_df = df_data[(df_data['s1'] > l_s1_cuts[s1_cut_num]) & (df_data['s1'] < l_s1_cuts[s1_cut_num+1])]
            d_s2_data_slices[current_set_s1_cuts] = cut_df['s2']



        print '\nStarting bands in S1 and S2\n'
        l_dfs = [0 for i in xrange(num_walkers*num_steps)]

        for i in tqdm.tqdm(xrange(num_walkers*num_steps)):
            # create dictionary to hold relevant information
            l_dfs[i] = {}


            a_fit_parameters = a_samples[-(np.random.randint(1, num_steps_to_include*num_walkers) % total_length_sampler), :]
            #print 'debugging so no random int'
            #a_fit_parameters = a_samples[-i, :]

            # load parameters into proper variables
            num_trials = np.asarray(num_mc_events, dtype=np.int32)
            mean_field = np.asarray(d_cathode_voltages_to_field[cathode_setting], dtype=np.float32)

            w_value = np.asarray(a_fit_parameters[0], dtype=np.float32)
            kappa = np.asarray(a_fit_parameters[1], dtype=np.float32)

            g1_value = np.asarray(a_fit_parameters[4], dtype=np.float32)
            spe_res = np.asarray(a_fit_parameters[5], dtype=np.float32)
            extraction_efficiency = np.asarray(a_fit_parameters[6], dtype=np.float32)
            gas_gain_value = np.asarray(a_fit_parameters[7], dtype=np.float32)
            gas_gain_width = np.asarray(a_fit_parameters[8], dtype=np.float32)

            intrinsic_res_s1 = np.asarray(a_fit_parameters[2], dtype=np.float32)
            intrinsic_res_s2 = np.asarray(a_fit_parameters[3], dtype=np.float32)

            pf_eff_par0 = np.asarray(a_fit_parameters[9], dtype=np.float32)
            pf_eff_par1 = np.asarray(a_fit_parameters[10], dtype=np.float32)

            s2_eff_par0 = np.asarray(a_fit_parameters[11], dtype=np.float32)
            s2_eff_par1 = np.asarray(a_fit_parameters[12], dtype=np.float32)

            a_pf_stdev = np.asarray([a_fit_parameters[13], a_fit_parameters[14], a_fit_parameters[15]], dtype=np.float32)
            
            dpe_prob = np.asarray(a_fit_parameters[16], dtype=np.float32)

            a_s1_mc_current_iteration = np.full(num_mc_events, -1, dtype=np.float32)
            a_s2_mc_current_iteration = np.full(num_mc_events, -1, dtype=np.float32)
            
            if cathode_setting == 0.345:
                mult_factor = 3
                gom_par0 = np.asarray(a_fit_parameters[-mult_factor*6 + 0], dtype=np.float32)
                gom_par1 = np.asarray(a_fit_parameters[-mult_factor*6 + 1], dtype=np.float32)
                gom_par2 = np.asarray(a_fit_parameters[-mult_factor*6 + 2], dtype=np.float32)
                gom_par3 = np.asarray(a_fit_parameters[-mult_factor*6 + 3], dtype=np.float32)
            
                exciton_to_ion_ratio = np.asarray(a_fit_parameters[-mult_factor*6 + 4], dtype=np.float32)
            
                scale_par = a_fit_parameters[-mult_factor*6 + 5]
            
            elif cathode_setting == 1.054:
                mult_factor = 2
                gom_par0 = np.asarray(a_fit_parameters[-mult_factor*6 + 0], dtype=np.float32)
                gom_par1 = np.asarray(a_fit_parameters[-mult_factor*6 + 1], dtype=np.float32)
                gom_par2 = np.asarray(a_fit_parameters[-mult_factor*6 + 2], dtype=np.float32)
                gom_par3 = np.asarray(a_fit_parameters[-mult_factor*6 + 3], dtype=np.float32)
            
                exciton_to_ion_ratio = np.asarray(a_fit_parameters[-mult_factor*6 + 4], dtype=np.float32)
            
                scale_par = a_fit_parameters[-mult_factor*6 + 5]
            
            elif cathode_setting == 2.356:
                mult_factor = 1
                gom_par0 = np.asarray(a_fit_parameters[-mult_factor*6 + 0], dtype=np.float32)
                gom_par1 = np.asarray(a_fit_parameters[-mult_factor*6 + 1], dtype=np.float32)
                gom_par2 = np.asarray(a_fit_parameters[-mult_factor*6 + 2], dtype=np.float32)
                gom_par3 = np.asarray(a_fit_parameters[-mult_factor*6 + 3], dtype=np.float32)
            
                exciton_to_ion_ratio = np.asarray(a_fit_parameters[-mult_factor*6 + 4], dtype=np.float32)
            
                scale_par = a_fit_parameters[-mult_factor*6 + 5]
            
            
            
            tArgs = (local_rng_states, drv.In(num_trials), drv.In(mean_field), d_plotting_information[cathode_setting][degree_setting]['gpu_energy'], drv.In(w_value), drv.In(exciton_to_ion_ratio), drv.In(gom_par0), drv.In(gom_par1), drv.In(gom_par2), drv.In(gom_par3), drv.In(kappa), drv.In(g1_value), drv.In(extraction_efficiency), drv.In(gas_gain_value), drv.In(gas_gain_width), drv.In(spe_res), drv.In(intrinsic_res_s1), drv.In(intrinsic_res_s2), drv.In(a_pf_stdev), drv.In(pf_eff_par0), drv.In(pf_eff_par1), drv.In(s2_eff_par0), drv.In(s2_eff_par1), drv.In(dpe_prob), drv.InOut(a_s1_mc_current_iteration), drv.InOut(a_s2_mc_current_iteration))
            
            gpu_observables_func(*tArgs, **d_gpu_scale)
            
            
            l_dfs[i]['main_df'] = pd.DataFrame({'s1':a_s1_mc_current_iteration, 's2':a_s2_mc_current_iteration})
            l_dfs[i]['main_df']['log'] = np.log10(df_mc['s2']/df_mc['s1'])
            
            l_dfs[i]['s1_hist'], _ = np.histogram(l_dfs[i]['main_df']['s1'], s1_edges_mc)
            l_dfs[i]['s1_hist'] = np.asarray(l_dfs[i]['s1_hist'], dtype=float)
            
            # scale_factor for histograms
            #print scale_par, d_plotting_information[cathode_setting][degree_setting]['num_data_pts'], float(np.sum(l_dfs[i]['s1_hist']))
            scaling_factor_for_histogram = scale_par*d_plotting_information[cathode_setting][degree_setting]['num_data_pts']/float(np.sum(l_dfs[i]['s1_hist']))*mc_bin_number_multiplier
            
            l_dfs[i]['s1_hist'] *= scaling_factor_for_histogram
            
            df_histograms['s1'][i, :] = l_dfs[i]['s1_hist']
            

            l_dfs[i]['s2_hist_after_s1_cuts'] = {}
            for s1_cut_num in xrange(len(l_s1_cuts)-1):
                # make cut
                cut_df = l_dfs[i]['main_df'][(l_dfs[i]['main_df']['s1'] > l_s1_cuts[s1_cut_num]) & (l_dfs[i]['main_df']['s1'] < l_s1_cuts[s1_cut_num+1])]
                
                # add into dictionary
                l_dfs[i]['s2_hist_after_s1_cuts'][(l_s1_cuts[s1_cut_num], l_s1_cuts[s1_cut_num+1])],  _ = np.histogram(cut_df['s2'], s2_edges_mc)
                l_dfs[i]['s2_hist_after_s1_cuts'][(l_s1_cuts[s1_cut_num], l_s1_cuts[s1_cut_num+1])] = np.asarray(l_dfs[i]['s2_hist_after_s1_cuts'][(l_s1_cuts[s1_cut_num], l_s1_cuts[s1_cut_num+1])], dtype=float)
                l_dfs[i]['s2_hist_after_s1_cuts'][(l_s1_cuts[s1_cut_num], l_s1_cuts[s1_cut_num+1])] *= scaling_factor_for_histogram

                df_histograms['s2'][(l_s1_cuts[s1_cut_num], l_s1_cuts[s1_cut_num+1])][i, :] = l_dfs[i]['s2_hist_after_s1_cuts'][(l_s1_cuts[s1_cut_num], l_s1_cuts[s1_cut_num+1])]



        # now use d_histograms to fill d_quantiles
        d_data_histograms = {}
        d_quantiles = {}
        l_quantiles = [16, 50, 84]
        assert len(l_quantiles) == 3, 'Must use exactly three quantiles for analysis'

        print '\nGetting quantiles for S1 Histograms...\n'

        d_quantiles['s1'] = {}
        for quantile in l_quantiles:
            d_quantiles['s1'][quantile] = np.zeros(len(s1_edges_mc)-1)
            for bin_number in xrange(len(s1_edges_mc)-1):
                d_quantiles['s1'][quantile][bin_number] = np.percentile(df_histograms['s1'][:, bin_number], quantile)
        d_data_histograms['s1'], _ = np.histogram(df_data['s1'], s1_edges)


        print '\nGetting quantiles for S2 Histograms...\n'

        d_quantiles['s2'] = {}
        d_data_histograms['s2'] = {}
        for s1_cut_num in xrange(len(l_s1_cuts)-1):
            current_set_s1_cuts = (l_s1_cuts[s1_cut_num], l_s1_cuts[s1_cut_num+1])
            d_quantiles['s2'][current_set_s1_cuts] = {}
            for quantile in l_quantiles:
                d_quantiles['s2'][current_set_s1_cuts][quantile] = np.zeros(len(s1_edges_mc)-1)
                for bin_number in xrange(len(s1_edges_mc)-1):
                    d_quantiles['s2'][current_set_s1_cuts][quantile][bin_number] = np.percentile(df_histograms['s2'][current_set_s1_cuts][:, bin_number], quantile)
            d_data_histograms['s2'][current_set_s1_cuts], _ = np.histogram(d_s2_data_slices[current_set_s1_cuts], s2_edges)





        # remove all events outside bounds
        df_mc = df_mc[(df_mc['s1'] > l_s1_settings[1]) & (df_mc['s1'] < l_s1_settings[2]) & (df_mc['log'] > l_log_settings[1]) & (df_mc['log'] < l_log_settings[2])]


        # grab median lines for each
        assert l_s1_settings[0] == l_log_settings[0]

        a_median_line_s1 = np.zeros(l_s1_settings[0])
        a_median_line_data_log = np.zeros(l_s1_settings[0])
        a_median_line_mc_log = np.zeros(l_s1_settings[0])

        for i in xrange(l_s1_settings[0]):
            a_median_line_s1[i] = (s1_edges[i+1] + s1_edges[i]) / 2.

            current_df_data = df_data[(df_data['s1'] > s1_edges[i]) & (df_data['s1'] < s1_edges[i+1])]
            current_df_mc = df_mc[(df_mc['s1'] > s1_edges[i]) & (df_mc['s1'] < s1_edges[i+1])]

            if len(current_df_data['log']) == 0:
                a_median_line_data_log[i] = a_median_line_data_log[i-1]
            else:
                #print current_df_data['log']
                #print np.median(current_df_data['log'])
                #print '\n\n'
                a_median_line_data_log[i] = np.median(current_df_data['log'])

            if len(current_df_mc['log']) == 0:
                a_median_line_mc_log[i] = a_median_line_mc_log[i-1]
            else:
                a_median_line_mc_log[i] = np.median(current_df_mc['log'])


        # draw on data hist and get handles
        line_handle_data, = ax_s1_log_data.plot(a_median_line_s1, a_median_line_data_log, color='r', label='Data Median')
        line_handle_mc, = ax_s1_log_data.plot(a_median_line_s1, a_median_line_mc_log, color='magenta', label='MC Median')

        # draw on mc hist
        ax_s1_log_mc.plot(a_median_line_s1, a_median_line_data_log, color='r', label='Data Median')
        ax_s1_log_mc.plot(a_median_line_s1, a_median_line_mc_log, color='magenta', label='MC Median')


        # add labels to plots
        ax_s1_log_data.set_title(r'NR Band Data - $V_c$ = %.3f kV' % (cathode_setting), fontsize=12)
        ax_s1_log_data.set_xlabel('S1 [PE]')
        ax_s1_log_data.set_ylabel(r'$log_{10}(\frac{S2}{S1})$')

        ax_s1_log_mc.set_title(r'NR Band Best Fit MC - $V_c$ = %.3f kV' % (cathode_setting), fontsize=12)
        ax_s1_log_mc.set_xlabel('S1 [PE]')
        ax_s1_log_mc.set_ylabel(r'$log_{10}(\frac{S2}{S1})$')


        ax_s1_log_data.legend(handles=[line_handle_data, line_handle_mc], loc='best')






        # grab S1 information for plot
        a_s1_x_values, a_s1_y_values, a_s1_x_err_low, a_s1_x_err_high, a_s1_y_err_low, a_s1_y_err_high = neriX_analysis.prepare_hist_arrays_for_plotting(d_data_histograms['s1'], s1_edges)

        ax_s1_projection.errorbar(a_s1_x_values, a_s1_y_values, xerr=[a_s1_x_err_low, a_s1_x_err_high], yerr=[a_s1_y_err_low, a_s1_y_err_high], linestyle='', color='black')
        ax_s1_projection.plot(s1_bin_centers_mc, d_quantiles['s1'][l_quantiles[1]], color='b', linestyle='--')
        ax_s1_projection.fill_between(s1_bin_centers_mc, d_quantiles['s1'][l_quantiles[0]], d_quantiles['s1'][l_quantiles[2]], color='b', alpha=transparency)

        ax_s1_projection.set_xlabel('S1 [PE]')
        ax_s1_projection.set_ylabel('Counts')

        #print len(s1_bin_centers_mc)
        #print s1_bin_centers_mc


        # produce 1D plots of S1 and slices of S2
        fig_s2s, l_s2_axes = plt.subplots(len(l_s1_cuts)-1, figsize=figure_sizes)
        #fig_s2s.tight_layout()
        fig_s2s.subplots_adjust(**d_subplot_space)

        d_s2_plots = {}
        for i, s1_cut_num in enumerate(xrange(len(l_s1_cuts)-1)):
            current_set_s1_cuts = (l_s1_cuts[s1_cut_num], l_s1_cuts[s1_cut_num+1])
            d_s2_plots[current_set_s1_cuts] = {}

            d_s2_plots[current_set_s1_cuts]['axis'] = l_s2_axes[i]

            d_s2_plots[current_set_s1_cuts]['x_values'], d_s2_plots[current_set_s1_cuts]['y_values'], d_s2_plots[current_set_s1_cuts]['x_err_low'], d_s2_plots[current_set_s1_cuts]['x_err_high'], d_s2_plots[current_set_s1_cuts]['y_err_low'], d_s2_plots[current_set_s1_cuts]['y_err_high'] = neriX_analysis.prepare_hist_arrays_for_plotting(d_data_histograms['s2'][current_set_s1_cuts], s2_edges)


            d_s2_plots[current_set_s1_cuts]['axis'].errorbar(d_s2_plots[current_set_s1_cuts]['x_values'], d_s2_plots[current_set_s1_cuts]['y_values'], xerr=[d_s2_plots[current_set_s1_cuts]['x_err_high'], d_s2_plots[current_set_s1_cuts]['x_err_high']], yerr=[d_s2_plots[current_set_s1_cuts]['y_err_low'], d_s2_plots[current_set_s1_cuts]['y_err_high']], linestyle='', color='black')

            d_s2_plots[current_set_s1_cuts]['axis'].plot(s2_bin_centers_mc, d_quantiles['s2'][current_set_s1_cuts][l_quantiles[1]], color='b', linestyle='--')
            d_s2_plots[current_set_s1_cuts]['axis'].fill_between(s2_bin_centers_mc, d_quantiles['s2'][current_set_s1_cuts][l_quantiles[0]], d_quantiles['s2'][current_set_s1_cuts][l_quantiles[2]], color='b', alpha=transparency)

            d_s2_plots[current_set_s1_cuts]['axis'].set_xlabel('S2 [PE]')
            d_s2_plots[current_set_s1_cuts]['axis'].set_ylabel('Counts')

            # create text box
            d_s2_plots[current_set_s1_cuts]['axis'].text(0.05, 0.95, '$ %d < S1 < %d $' % current_set_s1_cuts, transform=d_s2_plots[current_set_s1_cuts]['axis'].transAxes, fontsize=10,
                verticalalignment='top')








        s_path_for_save = './'
        for directory in l_plots:
            s_path_for_save += directory + '/'

        if not os.path.exists(s_path_for_save):
            os.makedirs(s_path_for_save)
            
            
        fig_s1_log.savefig('%ss_data_mc_comparison_s1_and_2d_%s_%.3f_kV.png' % (s_path_for_save, dir_specifier_name, cathode_setting))
        fig_s2s.savefig('%ss_data_mc_comparison_s2_slices_%s_%.3f_kV.png' % (s_path_for_save, dir_specifier_name, cathode_setting))






# end GPU context
ctx.pop()
print 'Successfully ended GPU context!'





#plt.show()



