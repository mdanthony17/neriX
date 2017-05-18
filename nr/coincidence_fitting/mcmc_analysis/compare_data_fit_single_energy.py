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

l_contour_levels = [0.1*i for i in xrange(1, 11)]#[0.2, 0.5, 0.9]
transparency = 0.2
l_quantiles_for_2d = [50]

# num_steps goes into how many random draws you will take from kept sampler
num_steps = 5
# num_steps_to_include is how large of sampler you keep
num_steps_to_include = 1000
num_mc_events = int(1e6)
mc_bin_number_multiplier = 5

if(len(sys.argv) != 4):
	print 'Usage is python compare_data_fit_single_energy.py <cathode> <degree> <num walkers>'
	sys.exit(1)

cathode_setting = float(sys.argv[1])
degree_setting = int(sys.argv[2])
num_walkers = int(sys.argv[3])
num_independent_samples = num_steps*num_walkers

directory_descriptor = 'single_energy'

l_save_path = ['plots', directory_descriptor, '%.3f_kV' % cathode_setting, '%d_deg' % (degree_setting)]


d_degree_setting_to_energy_name = {2300:3,
                                   3000:5,
                                   3500:7,
                                   4500:10,
                                   5300:15,
                                   6200:20}

d_cathode_voltages_to_field = {0.345:190,
                               1.054:490,
                               2.356:1020} # in kV:V/cm


dir_specifier_name = '%.3f_kV_%d_deg' % (cathode_setting, degree_setting)
name_of_results_directory = neriX_simulation_config.results_directory_name
name_of_results_directory += '/%s' % (directory_descriptor)
s_path_to_file = './%s/%s/sampler_dictionary.p' % (name_of_results_directory, dir_specifier_name)


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

d_bin_edges = {}
d_bin_edges['s1'] = {}
d_bin_edges['log'] = {}

l_s1_binning = neriX_simulation_config.d_binning['s1'][cathode_setting][degree_setting]
l_logs2s1_binning = neriX_simulation_config.d_binning['logs2s1'][cathode_setting][degree_setting]

if l_s1_binning[0] == 'lin':
    d_bin_edges['s1']['data'] = np.linspace(l_s1_binning[2], l_s1_binning[3], l_s1_binning[1]+1)
    d_bin_edges['s1']['mc'] = np.linspace(l_s1_binning[2], l_s1_binning[3], mc_bin_number_multiplier*l_s1_binning[1]+1)
else:
    d_bin_edges['s1']['data'] = np.logspace(l_s1_binning[2], l_s1_binning[3], l_s1_binning[1]+1)
    d_bin_edges['s1']['mc'] = np.logspace(l_s1_binning[2], l_s1_binning[3], mc_bin_number_multiplier*l_s1_binning[1]+1)

if l_logs2s1_binning[0] == 'lin':
    d_bin_edges['log']['data'] = np.linspace(l_logs2s1_binning[2], l_logs2s1_binning[3], l_logs2s1_binning[1]+1)
    d_bin_edges['log']['mc'] = np.linspace(l_logs2s1_binning[2], l_logs2s1_binning[3], mc_bin_number_multiplier*l_logs2s1_binning[1]+1)
else:
    d_bin_edges['log']['data'] = np.logspace(l_logs2s1_binning[2], l_logs2s1_binning[3], l_logs2s1_binning[1]+1)
    d_bin_edges['log']['mc'] = np.logspace(l_logs2s1_binning[2], l_logs2s1_binning[3], mc_bin_number_multiplier*l_logs2s1_binning[1]+1)

l_s2_settings = [40, 0, 4000]

mc_bin_number_multiplier = 5

s_main_save_directory = '../../coincidence_analysis/results/%.3fkV_%ddeg/' % (cathode_setting, degree_setting)

d_s1_s2 = pickle.load(open('%ss1_s2.pkl' % (s_main_save_directory), 'r'))

#print d_s1_s2
figure_sizes = (11, 13)
d_subplot_space = {'wspace':0.2, 'hspace':0.3}
fig_s1_log, (ax_s1_projection, ax_s1_log_data, ax_s1_log_mc) = plt.subplots(3, sharex=False, sharey=False, figsize=figure_sizes)
#fig_s1_log.tight_layout()
fig_s1_log.subplots_adjust(**d_subplot_space)


# fit KDE
# uncomment for KDE
"""
a_data = np.asarray([d_s1_s2['s1'], d_s1_s2['s2']]).T

scaler = preprocessing.StandardScaler()
scaler.fit(a_data)
a_scaled_samples = scaler.transform(a_data)

#print a_sampler[:,1:3]
#print a_scaled_samples

# find the best fit bandwith since this allows us
# to play with bias vs variance
grid = grid_search.GridSearchCV(neighbors.KernelDensity(), {'bandwidth':np.linspace(0.01, 10., 20)}, cv=4, verbose=1, n_jobs=4)
print '\nDetermining best bandwidth...\n'
grid.fit(a_scaled_samples)

kde = neighbors.KernelDensity(**grid.best_params_)
kde.fit(a_scaled_samples)


# get test samples
a_test = kde.sample(num_mc_events)
a_test_transformed = scaler.inverse_transform(a_test)
"""

# uncomment for KDE
a_s1_data = d_s1_s2['s1'] # a_test_transformed[:,0]
a_s2_data = d_s1_s2['s2'] # a_test_transformed[:,1]






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


# get energy histograms and then arrays
path_to_reduced_energy_spectra = '%s/reduced_simulation_data/' % (neriX_simulation_config.path_to_this_module)
current_path_to_reduced_energy_spectra = '%snerixsim-%dkeV.root' % (path_to_reduced_energy_spectra, d_degree_setting_to_energy_name[degree_setting])
current_path_to_bkg_spectra = '%snerixsim-pure_nr_spec.root' % (path_to_reduced_energy_spectra)
f_reduced = File(current_path_to_reduced_energy_spectra, 'read')
f_reduced_bkg = File(current_path_to_bkg_spectra, 'read')
h_energy = f_reduced.h_mc
h_energy_bkg = f_reduced_bkg.h_mc


a_energy = np.zeros(num_mc_events, dtype=np.float32)
a_energy_bkg = np.zeros(num_mc_events, dtype=np.float32)
for i in tqdm.tqdm(xrange(num_mc_events)):
    a_energy[i] = h_energy.GetRandom()
    a_energy_bkg[i] = h_energy_bkg.GetRandom()



# get random seeds setup
local_gpu_setup_kernel = pycuda.compiler.SourceModule(cuda_full_observables_production.cuda_full_observables_production_code, no_extern_c=True).get_function('setup_kernel')
local_rng_states = drv.mem_alloc(np.int32(num_blocks*block_dim)*pycuda.characterize.sizeof('curandStateXORWOW', '#include <curand_kernel.h>'))
local_gpu_setup_kernel(np.int32(int(num_blocks*block_dim)), local_rng_states, np.uint64(0), np.uint64(0), grid=(int(num_blocks), 1), block=(int(block_dim), 1, 1))


# get observables function
gpu_observables_func = SourceModule(cuda_full_observables_production.cuda_full_observables_production_code, no_extern_c=True).get_function('gpu_full_observables_production_single_energy_with_bkg')

print 'Putting energy array on GPU...'
gpu_energy = pycuda.gpuarray.to_gpu(a_energy)
print 'Putting bkg energy array on GPU...'
gpu_energy_bkg = pycuda.gpuarray.to_gpu(a_energy_bkg)



# -----------------------------------------------
# -----------------------------------------------
# run MC
# -----------------------------------------------
# -----------------------------------------------

l_contour_cmaps = ['Blues', 'Greens', 'Reds']

for quantile_number, a_fit_parameters in enumerate(a_percentile_values):

    num_trials = np.asarray(num_mc_events, dtype=np.int32)
    mean_field = np.asarray(d_cathode_voltages_to_field[cathode_setting], dtype=np.float32)

    py = np.asarray(a_fit_parameters[0], dtype=np.float32)
    qy = np.asarray(a_fit_parameters[1], dtype=np.float32)

    g1_value = np.asarray(a_fit_parameters[4], dtype=np.float32)
    extraction_efficiency = np.asarray(a_fit_parameters[6], dtype=np.float32)
    spe_res = np.asarray(a_fit_parameters[5], dtype=np.float32)
    gas_gain_value = np.asarray(a_fit_parameters[7], dtype=np.float32)
    gas_gain_width = np.asarray(a_fit_parameters[8], dtype=np.float32)

    intrinsic_res_s1 = np.asarray(a_fit_parameters[2], dtype=np.float32)
    intrinsic_res_s2 = np.asarray(a_fit_parameters[3], dtype=np.float32)

    exciton_to_ion_par0_rv = np.asarray(a_fit_parameters[15], dtype=np.float32)
    exciton_to_ion_par1_rv = np.asarray(a_fit_parameters[16], dtype=np.float32)
    exciton_to_ion_par2_rv = np.asarray(a_fit_parameters[17], dtype=np.float32)

    pf_eff_par0 = np.asarray(a_fit_parameters[9], dtype=np.float32)
    pf_eff_par1 = np.asarray(a_fit_parameters[10], dtype=np.float32)

    s2_eff_par0 = np.asarray(a_fit_parameters[11], dtype=np.float32)
    s2_eff_par1 = np.asarray(a_fit_parameters[12], dtype=np.float32)

    pf_res_s1 = np.asarray(a_fit_parameters[13], dtype=np.float32)
    pf_res_s2 = np.asarray(a_fit_parameters[14], dtype=np.float32)

    prob_bkg_event = np.asarray(a_fit_parameters[18], dtype=np.float32)
    
    a_s1_mc = np.zeros(num_mc_events, dtype=np.float32)
    a_s2_mc = np.zeros(num_mc_events, dtype=np.float32)


    tArgs = (local_rng_states, drv.In(num_trials), drv.In(mean_field), gpu_energy, gpu_energy_bkg, drv.In(py), drv.In(qy), drv.In(g1_value), drv.In(extraction_efficiency), drv.In(gas_gain_value), drv.In(gas_gain_width), drv.In(spe_res), drv.In(intrinsic_res_s1), drv.In(intrinsic_res_s2), drv.In(pf_res_s1), drv.In(pf_res_s2), drv.In(exciton_to_ion_par0_rv), drv.In(exciton_to_ion_par1_rv), drv.In(exciton_to_ion_par2_rv), drv.In(pf_eff_par0), drv.In(pf_eff_par1), drv.In(s2_eff_par0), drv.In(s2_eff_par1), drv.In(prob_bkg_event), drv.InOut(a_s1_mc), drv.InOut(a_s2_mc))
    
    gpu_observables_func(*tArgs, **d_gpu_scale)



hist_s1_log_mc = ax_s1_log_mc.hist2d(a_s1_mc, np.log10(a_s2_mc/a_s1_mc), bins=[d_bin_edges['s1']['mc'], d_bin_edges['log']['mc']], cmap='Blues', normed=True)
fig_s1_log.colorbar(hist_s1_log_mc[3], ax=ax_s1_log_mc)


df_data = pd.DataFrame({'s1':a_s1_data, 's2':a_s2_data})
df_data['log'] = np.log10(df_data['s2']/df_data['s1'])

df_data = df_data[(df_data['s1'] > d_bin_edges['s1']['mc'][0]) & (df_data['s1'] < d_bin_edges['s1']['mc'][-1]) & (df_data['log'] > d_bin_edges['log']['mc'][0]) & (df_data['log'] < d_bin_edges['log']['mc'][-1])]


s1_edges_mc = d_bin_edges['s1']['mc']
s1_bin_centers_mc = (s1_edges_mc[1:] + s1_edges_mc[:-1])/2.
s2_edges_mc = np.linspace(l_s2_settings[1], l_s2_settings[2], mc_bin_number_multiplier*l_s2_settings[0]+1)
s2_bin_centers_mc = (s2_edges_mc[1:] + s2_edges_mc[:-1])/2.
log_edges_mc = d_bin_edges['log']['mc']




num_data_pts = len(df_data['s1'])

df_mc = pd.DataFrame({'s1':a_s1_mc, 's2':a_s2_mc})
df_mc['log'] = np.log10(df_mc['s2']/df_mc['s1'])

print '\nCleaning up NR band...\n\n'

for bin_number in xrange(len(s1_edges_mc)-1):
    # get data frame for this bin region
    current_df_mc = df_mc[(df_mc['s1'] > s1_edges_mc[bin_number]) & (df_mc['s1'] < s1_edges_mc[bin_number+1]) & (df_mc['log'] > log_edges_mc[0]) & (df_mc['log'] < log_edges_mc[-1])]
    #print current_df_mc

    # get mean and rms
    current_mean_log = np.mean(current_df_mc['log'])
    current_std_log = np.std(current_df_mc['log'])
    
    #print s1_edges_mc[bin_number], s1_edges_mc[bin_number+1], current_mean_log, current_std_log

    if (np.isnan(current_mean_log)) or (np.isnan(current_std_log)):
        continue

    num_std_away = 3

    # must apply to both data and MC!
    df_data = df_data[(df_data['s1'] < s1_edges_mc[bin_number]) | (df_data['s1'] > s1_edges_mc[bin_number+1]) | ((df_data['log'] > (current_mean_log-num_std_away*current_std_log)) & (df_data['log'] < (current_mean_log+num_std_away*current_std_log)))]
    df_mc = df_mc[(df_mc['s1'] < s1_edges_mc[bin_number]) | (df_mc['s1'] > s1_edges_mc[bin_number+1]) | ((df_mc['log'] > (current_mean_log-num_std_away*current_std_log)) & (df_mc['log'] < (current_mean_log+num_std_away*current_std_log)))]


# need axis image for colorbar
hist_s1_log_data = ax_s1_log_data.hist2d(df_data['s1'], df_data['log'], bins=[d_bin_edges['s1']['data'], d_bin_edges['log']['data']], cmap='Blues')
fig_s1_log.colorbar(hist_s1_log_data[3], ax=ax_s1_log_data)


s1_edges = hist_s1_log_data[1]
log_edges = hist_s1_log_data[2]
s2_edges = np.linspace(l_s2_settings[1], l_s2_settings[2], l_s2_settings[0]+1)


df_histograms = {}
df_histograms['s1'] = np.zeros((num_walkers*num_steps, len(s1_edges_mc)-1), dtype=float)

df_histograms['s2'] = {}
d_s2_data_slices = {}
for s1_cut_num in xrange(len(l_s1_cuts)-1):
    current_set_s1_cuts = (l_s1_cuts[s1_cut_num], l_s1_cuts[s1_cut_num+1])
    df_histograms['s2'][current_set_s1_cuts] = np.zeros((num_walkers*num_steps, l_s2_settings[0]*mc_bin_number_multiplier), dtype=float)
    
    # get data slices
    cut_df = df_data[(df_data['s1'] > l_s1_cuts[s1_cut_num]) & (df_data['s1'] < l_s1_cuts[s1_cut_num+1])]
    d_s2_data_slices[current_set_s1_cuts] = cut_df['s2']



print '\nStarting bands in S1 and S2\n'
l_dfs = [0 for i in xrange(num_walkers*num_steps)]

for i in tqdm.tqdm(xrange(num_walkers*num_steps)):
    # create dictionary to hold relevant information
    l_dfs[i] = {}


    #a_fit_parameters = a_samples[-(np.random.randint(1, num_steps_to_include*num_walkers)), :]
    a_fit_parameters = a_samples[-i, :]

    # load parameters into proper variables
    num_trials = np.asarray(num_mc_events, dtype=np.int32)
    mean_field = np.asarray(d_cathode_voltages_to_field[cathode_setting], dtype=np.float32)

    py = np.asarray(a_fit_parameters[0], dtype=np.float32)
    qy = np.asarray(a_fit_parameters[1], dtype=np.float32)

    g1_value = np.asarray(a_fit_parameters[4], dtype=np.float32)
    extraction_efficiency = np.asarray(a_fit_parameters[6], dtype=np.float32)
    spe_res = np.asarray(a_fit_parameters[5], dtype=np.float32)
    gas_gain_value = np.asarray(a_fit_parameters[7], dtype=np.float32)
    gas_gain_width = np.asarray(a_fit_parameters[8], dtype=np.float32)

    intrinsic_res_s1 = np.asarray(a_fit_parameters[2], dtype=np.float32)
    intrinsic_res_s2 = np.asarray(a_fit_parameters[3], dtype=np.float32)

    exciton_to_ion_par0_rv = np.asarray(a_fit_parameters[15], dtype=np.float32)
    exciton_to_ion_par1_rv = np.asarray(a_fit_parameters[16], dtype=np.float32)
    exciton_to_ion_par2_rv = np.asarray(a_fit_parameters[17], dtype=np.float32)

    pf_eff_par0 = np.asarray(a_fit_parameters[9], dtype=np.float32)
    pf_eff_par1 = np.asarray(a_fit_parameters[10], dtype=np.float32)

    s2_eff_par0 = np.asarray(a_fit_parameters[11], dtype=np.float32)
    s2_eff_par1 = np.asarray(a_fit_parameters[12], dtype=np.float32)

    pf_res_s1 = np.asarray(a_fit_parameters[13], dtype=np.float32)
    pf_res_s2 = np.asarray(a_fit_parameters[14], dtype=np.float32)

    prob_bkg_event = np.asarray(a_fit_parameters[18], dtype=np.float32)
    
    scale_par = a_fit_parameters[19]
    
    a_s1_mc_current_iteration = np.zeros(num_mc_events, dtype=np.float32)
    a_s2_mc_current_iteration = np.zeros(num_mc_events, dtype=np.float32)

    tArgs = (local_rng_states, drv.In(num_trials), drv.In(mean_field), gpu_energy, gpu_energy_bkg, drv.In(py), drv.In(qy), drv.In(g1_value), drv.In(extraction_efficiency), drv.In(gas_gain_value), drv.In(gas_gain_width), drv.In(spe_res), drv.In(intrinsic_res_s1), drv.In(intrinsic_res_s2), drv.In(pf_res_s1), drv.In(pf_res_s2), drv.In(exciton_to_ion_par0_rv), drv.In(exciton_to_ion_par1_rv), drv.In(exciton_to_ion_par2_rv), drv.In(pf_eff_par0), drv.In(pf_eff_par1), drv.In(s2_eff_par0), drv.In(s2_eff_par1), drv.In(prob_bkg_event), drv.InOut(a_s1_mc_current_iteration), drv.InOut(a_s2_mc_current_iteration))
    
    gpu_observables_func(*tArgs, **d_gpu_scale)
    
    
    l_dfs[i]['main_df'] = pd.DataFrame({'s1':a_s1_mc_current_iteration, 's2':a_s2_mc_current_iteration})
    l_dfs[i]['main_df']['log'] = np.log10(l_dfs[i]['main_df']['s2']/l_dfs[i]['main_df']['s1'])
    
    l_dfs[i]['s1_hist'], _ = np.histogram(l_dfs[i]['main_df']['s1'], s1_edges_mc)
    l_dfs[i]['s1_hist'] = np.asarray(l_dfs[i]['s1_hist'], dtype=float)
    
    # scale_factor for histograms
    scaling_factor_for_histogram = scale_par*num_data_pts/float(num_mc_events)*mc_bin_number_multiplier
    
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
        d_quantiles['s2'][current_set_s1_cuts][quantile] = np.zeros(mc_bin_number_multiplier*l_s2_settings[0])
        for bin_number in xrange(l_s2_settings[0]*mc_bin_number_multiplier):
            d_quantiles['s2'][current_set_s1_cuts][quantile][bin_number] = np.percentile(df_histograms['s2'][current_set_s1_cuts][:, bin_number], quantile)
    d_data_histograms['s2'][current_set_s1_cuts], _ = np.histogram(d_s2_data_slices[current_set_s1_cuts], s2_edges)






# end GPU context
ctx.pop()
print 'Successfully ended GPU context!'




# remove all events outside bounds
df_mc = df_mc[(df_mc['s1'] > d_bin_edges['s1']['mc'][0]) & (df_mc['s1'] < d_bin_edges['s1']['mc'][-1]) & (df_mc['log'] > d_bin_edges['log']['mc'][0]) & (df_mc['log'] < d_bin_edges['log']['mc'][-1])]

num_s1_bins = len(d_bin_edges['s1']['data'])-1

a_median_line_s1 = np.zeros(num_s1_bins)
a_median_line_data_log = np.zeros(num_s1_bins)
a_median_line_mc_log = np.zeros(num_s1_bins)

for i in xrange(num_s1_bins):
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
ax_s1_log_data.set_title(r'%d keV Data - $V_c$ = %.3f kV' % (d_degree_setting_to_energy_name[degree_setting], cathode_setting), fontsize=12)
ax_s1_log_data.set_xlabel('S1 [PE]')
ax_s1_log_data.set_ylabel(r'$log_{10}(\frac{S2}{S1})$')

ax_s1_log_mc.set_title(r'%d keV Best Fit MC - $V_c$ = %.3f kV' % (d_degree_setting_to_energy_name[degree_setting], cathode_setting), fontsize=12)
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
for directory in l_save_path:
    s_path_for_save += directory + '/'

if not os.path.exists(s_path_for_save):
    os.makedirs(s_path_for_save)
    
    
fig_s1_log.savefig('%ss_data_mc_comparison_s1_and_2d_%s.png' % (s_path_for_save, dir_specifier_name))
fig_s2s.savefig('%ss_data_mc_comparison_s2_slices_%s.png' % (s_path_for_save, dir_specifier_name))


#plt.show()



