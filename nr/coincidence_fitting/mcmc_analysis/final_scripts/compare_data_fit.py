#!/usr/bin/python

import sys
import neriX_analysis
import os, re

sys.path.insert(0, '..')
sys.path.insert(0, '../..')
import neriX_simulation_config, pff_test

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


import scipy.optimize as op
import cPickle as pickle

from scipy.special import gammaln

max_s2 = 3000

num_s1_cuts = 3 # 4 edges, 3 slices

d_s1_cuts = {}
d_s1_cuts[-4] = [40, 70, 100, 150]
d_s1_cuts[3000] = [0, 7, 15, 40]
d_s1_cuts[3500] = [0, 5, 10, 40]
d_s1_cuts[4500] = [0, 10, 20, 40]
d_s1_cuts[5300] = [0, 10, 20, 40]

transparency = 0.2
l_quantiles_for_2d = [50]

# num_steps goes into how many random draws you will take from kept sampler
num_steps = 10#25
mc_bin_number_multiplier = 10

# num_steps_to_include is how large of sampler you keep
num_steps_to_include = num_steps
num_mc_events = int(2e5)

if(len(sys.argv) != 7):
	print 'Usage is python compare_data_fit_single_energy.py <num walkers> <cathode setting> <degree setting> <device number> <moved positions> <bq_fixed>'
	sys.exit(1)

num_walkers = int(sys.argv[1])
cathode_setting = float(sys.argv[2])
degree_setting = int(sys.argv[3])
device_number = int(sys.argv[4])
if str.lower(sys.argv[5]) == 't':
    b_moved_pos = True
else:
    b_moved_pos = False
if str.lower(sys.argv[6]) == 't':
    b_bq_fixed = True
else:
    b_bq_fixed = False



if degree_setting == 5300:
    data_bin_multiplier = 1
else:
    data_bin_multiplier = 2

d_degree_setting_to_energy_name = {2300:3,
                                   3000:5,
                                   3500:7,
                                   4500:10,
                                   5300:15,
                                   6200:20}

d_cathode_voltages_to_field = {0.345:190,
                               1.054:490,
                               2.356:1020} # in kV:V/cm


dir_specifier_name = 'multiple_energies_lindhard_model_with_ti'
if b_moved_pos:
    dir_specifier_name += '_moved_pos'
elif b_bq_fixed:
    dir_specifier_name += '_bq_fixed'


l_degree_settings_in_use = [-4, 3000, 3500, 4500, 5300]
s_degree_settings = ''
for name_degree_setting in l_degree_settings_in_use:
    s_degree_settings += '%s,' % (name_degree_setting)
s_degree_settings = s_degree_settings[:-1]


l_cathode_settings_in_use = [0.345, 1.054, 2.356]
s_cathode_settings = ''
for name_cathode_setting in l_cathode_settings_in_use:
    s_cathode_settings += '%.3f,' % (name_cathode_setting)
s_cathode_settings = s_cathode_settings[:-1]

# use count_... variables to track which cathode
# and degree setting in use
count_fields = l_cathode_settings_in_use.index(cathode_setting)
count_degrees = l_degree_settings_in_use.index(degree_setting)

# need to account for band difference in degrees
if count_degrees != 0:
    count_degrees -= 1


name_of_results_directory = neriX_simulation_config.results_directory_name
l_plots = ['plots', dir_specifier_name, '%s_kV_%s_deg' % (s_cathode_settings, s_degree_settings), 'preliminary']


s_path_to_file = './%s/%s/%s_kV_%s_deg/sampler_dictionary.p.reduced' % (name_of_results_directory, dir_specifier_name, s_cathode_settings, s_degree_settings)


if os.path.exists(s_path_to_file):
    dSampler = pickle.load(open(s_path_to_file, 'r'))
    l_chains = []
    l_ln_likelihoods = []
    for sampler in dSampler[num_walkers]:
        l_chains.append(sampler['_chain'])
        l_ln_likelihoods.append(sampler['_lnprob'])

    a_full_sampler = np.concatenate(l_chains, axis=1)
    a_full_ln_likelihood = np.concatenate(l_ln_likelihoods, axis=1)

    print 'Successfully loaded sampler!'
else:
    print s_path_to_file
    print 'Could not find file!'
    sys.exit()

# find best fit set of parameters
a_best_fit_indices = np.unravel_index(np.argmax(a_full_ln_likelihood), a_full_ln_likelihood.shape)
a_best_fit_parameters = a_full_sampler[a_best_fit_indices]

#print '\n\nBest Fit Value and Log-Likelihood:'
#print list(a_best_fit_parameters)
#print a_full_ln_likelihood[a_best_fit_indices]
#print a_best_fit_indices
#print '\n\n'



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

#print '\n\nUsing median instead of best fit'
#a_best_fit_parameters = np.percentile(a_samples, 50, axis=0)
#print list(a_best_fit_parameters)
#print '\n\n'








d_bin_edges = {}
d_bin_edges['s1'] = {}
d_bin_edges['s1_mc'] = {}
d_bin_edges['log'] = {}
d_bin_edges['log_mc'] = {}

d_bin_edges['s1'][cathode_setting] = {}
d_bin_edges['s1_mc'][cathode_setting] = {}
d_bin_edges['log'][cathode_setting] = {}
d_bin_edges['log_mc'][cathode_setting] = {}
    

l_s1_binning = neriX_simulation_config.d_binning['s1'][cathode_setting][degree_setting]
l_logs2s1_binning = neriX_simulation_config.d_binning['logs2s1'][cathode_setting][degree_setting]



# old way: uses binning as used in histogram
"""
if l_s1_binning[0] == 'lin':
    d_bin_edges['s1'][cathode_setting][degree_setting] = np.linspace(l_s1_binning[2], l_s1_binning[3], l_s1_binning[1]+1)
    d_bin_edges['s1_mc'][cathode_setting][degree_setting] = np.linspace(l_s1_binning[2], l_s1_binning[3], mc_bin_number_multiplier*l_s1_binning[1]+1)
else:
    d_bin_edges['s1'][cathode_setting][degree_setting] = np.logspace(l_s1_binning[2], l_s1_binning[3], l_s1_binning[1]+1)
    d_bin_edges['s1_mc'][cathode_setting][degree_setting] = np.logspace(l_s1_binning[2], l_s1_binning[3], mc_bin_number_multiplier*l_s1_binning[1]+1)

if l_logs2s1_binning[0] == 'lin':
    d_bin_edges['log'][cathode_setting][degree_setting] = np.linspace(l_logs2s1_binning[2], l_logs2s1_binning[3], l_logs2s1_binning[1]+1)
    d_bin_edges['log_mc'][cathode_setting][degree_setting] = np.linspace(l_logs2s1_binning[2], l_logs2s1_binning[3], mc_bin_number_multiplier*l_logs2s1_binning[1]+1)
else:
    d_bin_edges['log'][cathode_setting][degree_setting] = np.logspace(l_logs2s1_binning[2], l_logs2s1_binning[3], l_logs2s1_binning[1]+1)
    d_bin_edges['log_mc'][cathode_setting][degree_setting] = np.logspace(l_logs2s1_binning[2], l_logs2s1_binning[3], mc_bin_number_multiplier*l_logs2s1_binning[1]+1)
"""



# linear only
if l_s1_binning[0] == 'lin':
    d_bin_edges['s1'][cathode_setting][degree_setting] = np.linspace(l_s1_binning[2], l_s1_binning[3], data_bin_multiplier*l_s1_binning[1]+1)
    d_bin_edges['s1_mc'][cathode_setting][degree_setting] = np.linspace(l_s1_binning[2], l_s1_binning[3], mc_bin_number_multiplier*l_s1_binning[1]+1)
else:
    d_bin_edges['s1'][cathode_setting][degree_setting] = np.linspace(10**l_s1_binning[2], 10**l_s1_binning[3], data_bin_multiplier*l_s1_binning[1]+1)
    d_bin_edges['s1_mc'][cathode_setting][degree_setting] = np.linspace(10**l_s1_binning[2], 10**l_s1_binning[3], mc_bin_number_multiplier*l_s1_binning[1]+1)

if l_logs2s1_binning[0] == 'lin':
    d_bin_edges['log'][cathode_setting][degree_setting] = np.linspace(l_logs2s1_binning[2], l_logs2s1_binning[3], data_bin_multiplier*l_logs2s1_binning[1]+1)
    d_bin_edges['log_mc'][cathode_setting][degree_setting] = np.linspace(l_logs2s1_binning[2], l_logs2s1_binning[3], mc_bin_number_multiplier*l_logs2s1_binning[1]+1)
else:
    d_bin_edges['log'][cathode_setting][degree_setting] = np.linspace(10**l_logs2s1_binning[2], 10**l_logs2s1_binning[3], data_bin_multiplier*l_logs2s1_binning[1]+1)
    d_bin_edges['log_mc'][cathode_setting][degree_setting] = np.linspace(10**l_logs2s1_binning[2], 10**l_logs2s1_binning[3], mc_bin_number_multiplier*l_logs2s1_binning[1]+1)


if degree_setting == 5300:
    l_s2_settings = [40, 0, 4000]
else:
    l_s2_settings = [15, 0, 4000]
l_s2_settings_band = [10, 1000, 6000]


figure_sizes = (11, 13)
d_subplot_space = {'wspace':0.2, 'hspace':0.3}





# need to prepare GPU for MC simulations
import cuda_full_observables_production
from pycuda.compiler import SourceModule
import pycuda.driver as drv
import pycuda.tools
import pycuda.gpuarray

drv.init()
dev = drv.Device(device_number)
ctx = dev.make_context()
print 'Device Name: %s\n' % (dev.name())



# get bkg energy
path_to_reduced_energy_spectra = '%s/reduced_simulation_data/' % (neriX_simulation_config.path_to_this_module)
current_path_to_reduced_energy_spectra = '%snerixsim-pure_nr_spec.root' % (path_to_reduced_energy_spectra)
f_reduced = File(current_path_to_reduced_energy_spectra, 'read')
h_energy = f_reduced.h_mc

a_energy = np.zeros(num_mc_events, dtype=np.float32)
for i in tqdm.tqdm(xrange(num_mc_events)):
    a_energy[i] = h_energy.GetRandom()

gpu_bkg_energy = pycuda.gpuarray.to_gpu(a_energy)



d_plotting_information = {}
d_plotting_information[cathode_setting] = {}
    
d_plotting_information[cathode_setting][degree_setting] = {}

path_to_reduced_energy_spectra = '%s/reduced_simulation_data/' % (neriX_simulation_config.path_to_this_module)
if degree_setting == -4:
    current_path_to_reduced_energy_spectra = '%snerixsim-pure_nr_spec.root' % (path_to_reduced_energy_spectra)
else:
    current_path_to_reduced_energy_spectra = '%snerixsim-%dkeV.root' % (path_to_reduced_energy_spectra, d_degree_setting_to_energy_name[degree_setting])
f_reduced = File(current_path_to_reduced_energy_spectra, 'read')
h_energy = f_reduced.h_mc


a_energy = np.zeros(num_mc_events, dtype=np.float32)
for i in tqdm.tqdm(xrange(num_mc_events)):
    random_energy = h_energy.GetRandom()
    
    # special condition for band
    if degree_setting == -4:
        while random_energy < (d_bin_edges['s1'][cathode_setting][degree_setting][0]*0.025/0.11/7.):
            random_energy = h_energy.GetRandom()
    

    a_energy[i] = random_energy


print 'Putting energy array on GPU...'
d_plotting_information[cathode_setting][degree_setting]['gpu_energy'] = pycuda.gpuarray.to_gpu(a_energy)
d_plotting_information[cathode_setting][degree_setting]['a_energy'] = a_energy





# get random seeds setup
local_gpu_setup_kernel = pycuda.compiler.SourceModule(cuda_full_observables_production.cuda_full_observables_production_code, no_extern_c=True).get_function('setup_kernel')
local_rng_states = drv.mem_alloc(np.int32(num_blocks*block_dim)*pycuda.characterize.sizeof('curandStateXORWOW', '#include <curand_kernel.h>'))
local_gpu_setup_kernel(np.int32(int(num_blocks*block_dim)), local_rng_states, np.uint64(0), np.uint64(0), grid=(int(num_blocks), 1), block=(int(block_dim), 1, 1))


# get observables function
gpu_observables_func = SourceModule(cuda_full_observables_production.cuda_full_observables_production_code, no_extern_c=True).get_function('gpu_full_observables_production_with_arrays_ti_wth_bkg')


num_degree_field_pars = 2*len(l_degree_settings_in_use) + 2 - 1


print '\nCurrently working on %.3f kV at %d deg...\n' % (cathode_setting, degree_setting)

# get s1 and s2 data arrays
d_plotting_information[cathode_setting][degree_setting]['data_save_directory'] = '../../../coincidence_analysis/results/%.3fkV_%ddeg/' % (cathode_setting, degree_setting)
d_plotting_information[cathode_setting][degree_setting]['d_s1_s2_data'] = pickle.load(open('%ss1_s2.pkl' % (d_plotting_information[cathode_setting][degree_setting]['data_save_directory']), 'r'))
d_plotting_information[cathode_setting][degree_setting]['a_s1_data'] = d_plotting_information[cathode_setting][degree_setting]['d_s1_s2_data']['s1']
d_plotting_information[cathode_setting][degree_setting]['a_s2_data'] = d_plotting_information[cathode_setting][degree_setting]['d_s1_s2_data']['s2']

d_plotting_information[cathode_setting][degree_setting]['df'] = pd.DataFrame({'s1':d_plotting_information[cathode_setting][degree_setting]['d_s1_s2_data']['s1'], 's2':d_plotting_information[cathode_setting][degree_setting]['d_s1_s2_data']['s2']})
current_df_data = d_plotting_information[cathode_setting][degree_setting]['df']
current_df_data['log'] = np.log10(current_df_data['s2']/current_df_data['s1'])

# make cuts on bounds
current_df_data = current_df_data[(current_df_data['s1']>d_bin_edges['s1'][cathode_setting][degree_setting][0]) & (current_df_data['s1']<d_bin_edges['s1'][cathode_setting][degree_setting][-1]) & (current_df_data['log']>d_bin_edges['log'][cathode_setting][degree_setting][0]) & (current_df_data['log']<d_bin_edges['log'][cathode_setting][degree_setting][-1])]

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

current_df_data = current_df_data[(current_df_data['s1'] < er_cut_lb) | ((current_df_data['s1'] > er_cut_lb) & (current_df_data['log'] < (intercept + slope*current_df_data['s1'])))]

# get number of data pts
d_plotting_information[cathode_setting][degree_setting]['num_data_pts'] = len(current_df_data['s1'])


# create figure and give settings for space
fig_s1_log, (ax_s1_projection, ax_s1_log_data, ax_s1_log_mc) = plt.subplots(3, sharex=False, sharey=False, figsize=figure_sizes)
fig_s1_log.subplots_adjust(**d_subplot_space)








# -----------------------------------------------
# -----------------------------------------------
# run MC
# -----------------------------------------------
# -----------------------------------------------


num_trials = np.asarray(num_mc_events, dtype=np.int32)
mean_field = np.asarray(d_cathode_voltages_to_field[cathode_setting], dtype=np.float32)

w_value = np.asarray(a_best_fit_parameters[0], dtype=np.float32)
beta = np.asarray(a_best_fit_parameters[1], dtype=np.float32)
kappa = np.asarray(a_best_fit_parameters[2], dtype=np.float32)
eta = np.asarray(a_best_fit_parameters[3], dtype=np.float32)

if b_bq_fixed:
    lamb = np.asarray(0.5, dtype=np.float32)
else:
    lamb = np.asarray(a_best_fit_parameters[4], dtype=np.float32)

g1_value = np.asarray(a_best_fit_parameters[5], dtype=np.float32)
spe_res = np.asarray(a_best_fit_parameters[6], dtype=np.float32)
extraction_efficiency = np.asarray(a_best_fit_parameters[7], dtype=np.float32)
gas_gain_value = np.asarray(a_best_fit_parameters[8], dtype=np.float32)
gas_gain_width = np.asarray(a_best_fit_parameters[9], dtype=np.float32)


#print cathode_setting, degree_setting
#print intrinsic_res_s1, intrinsic_res_s2

pf_eff_par0 = np.asarray(a_best_fit_parameters[10], dtype=np.float32)
pf_eff_par1 = np.asarray(a_best_fit_parameters[11], dtype=np.float32)

s2_eff_par0 = np.asarray(a_best_fit_parameters[12], dtype=np.float32)
s2_eff_par1 = np.asarray(a_best_fit_parameters[13], dtype=np.float32)

a_pf_stdev_s1 = np.asarray([a_best_fit_parameters[14], a_best_fit_parameters[15], a_best_fit_parameters[16]], dtype=np.float32)
a_pf_stdev_s2 = np.asarray([a_best_fit_parameters[17], a_best_fit_parameters[18], a_best_fit_parameters[19]], dtype=np.float32)

dpe_prob = np.asarray(a_best_fit_parameters[20], dtype=np.float32)

a_s1_mc = np.zeros(num_mc_events, dtype=np.float32)
a_s2_mc = np.zeros(num_mc_events, dtype=np.float32)
a_weight = np.zeros(num_mc_events, dtype=np.float32)

ti_par = np.asarray(a_best_fit_parameters[20+1+count_fields*num_degree_field_pars], dtype=np.float32)

exciton_to_ion_ratio = np.asarray(a_best_fit_parameters[20+2+count_fields*num_degree_field_pars], dtype=np.float32)

if degree_setting == -4:
    prob_bkg = np.asarray(0., dtype=np.float32)
    scale_par = np.asarray(a_best_fit_parameters[20+3+count_fields*num_degree_field_pars], dtype=np.float32)
else:
    prob_bkg = np.asarray(a_best_fit_parameters[20+4+count_fields*num_degree_field_pars + 2*count_degrees], dtype=np.float32)
    scale_par = np.asarray(a_best_fit_parameters[20+5+count_fields*num_degree_field_pars + 2*count_degrees], dtype=np.float32)

#print 'prob bkg: %.3f\n' % (prob_bkg)


tArgs = (local_rng_states, drv.In(num_trials), drv.In(mean_field), d_plotting_information[cathode_setting][degree_setting]['gpu_energy'], gpu_bkg_energy, drv.In(w_value), drv.In(exciton_to_ion_ratio), drv.In(ti_par), drv.In(beta), drv.In(kappa), drv.In(eta), drv.In(lamb), drv.In(g1_value), drv.In(extraction_efficiency), drv.In(gas_gain_value), drv.In(gas_gain_width), drv.In(spe_res), drv.In(a_pf_stdev_s1), drv.In(a_pf_stdev_s2), drv.In(pf_eff_par0), drv.In(pf_eff_par1), drv.In(s2_eff_par0), drv.In(s2_eff_par1), drv.In(dpe_prob), drv.In(prob_bkg), drv.InOut(a_s1_mc), drv.InOut(a_s2_mc), drv.InOut(a_weight))

gpu_observables_func(*tArgs, **d_gpu_scale)

#print a_s1_mc
#print a_s2_mc
#print a_weight



#plt.show()

# get from last histogram
s1_edges = d_bin_edges['s1'][cathode_setting][degree_setting]
log_edges = d_bin_edges['log'][cathode_setting][degree_setting]

if degree_setting == -4:
    s2_edges = np.linspace(l_s2_settings_band[1], l_s2_settings_band[2], data_bin_multiplier*l_s2_settings_band[0]+1)
else:
    s2_edges = np.linspace(l_s2_settings[1], l_s2_settings[2], data_bin_multiplier*l_s2_settings[0]+1)


s1_edges_mc = d_bin_edges['s1_mc'][cathode_setting][degree_setting]
s1_bin_centers_mc = (s1_edges_mc[1:] + s1_edges_mc[:-1])/2.

log_edges_mc = d_bin_edges['log_mc'][cathode_setting][degree_setting]



df_mc = pd.DataFrame({'s1':a_s1_mc, 's2':a_s2_mc})
df_mc['log'] = np.log10(df_mc['s2']/df_mc['s1'])
df_mc['weights'] = a_weight
df_mc.dropna()



# for consistency, get data integral BEFORE extra cuts
a_hist2d_before_cuts_data, _, _ = np.histogram2d(current_df_data['s1'], current_df_data['log'], bins=[d_bin_edges['s1'][cathode_setting][degree_setting], d_bin_edges['log'][cathode_setting][degree_setting]])
data_integral = np.sum(a_hist2d_before_cuts_data)

mc_integral = float(num_mc_events)


hist_s1_log_mc, _, _ = np.histogram2d(a_s1_mc, np.log10(a_s2_mc/a_s1_mc), bins=[d_bin_edges['s1'][cathode_setting][degree_setting], d_bin_edges['log'][cathode_setting][degree_setting]], weights=a_weight)
hist_s1_log_mc = np.multiply(hist_s1_log_mc, scale_par * data_integral / mc_integral)
pcm = ax_s1_log_mc.pcolor(d_bin_edges['s1'][cathode_setting][degree_setting], d_bin_edges['log'][cathode_setting][degree_setting], hist_s1_log_mc.T, cmap='Blues')

hist_s1_log_mc_fine, _, _ = np.histogram2d(a_s1_mc, np.log10(a_s2_mc/a_s1_mc), bins=[d_bin_edges['s1_mc'][cathode_setting][degree_setting], d_bin_edges['log_mc'][cathode_setting][degree_setting]], weights=a_weight)
hist_s1_log_mc_fine = np.multiply(hist_s1_log_mc_fine, scale_par * data_integral * mc_bin_number_multiplier / mc_integral)

ax_s1_log_mc.set_xlim(d_bin_edges['s1'][cathode_setting][degree_setting][0], d_bin_edges['s1'][cathode_setting][degree_setting][-1])
ax_s1_log_mc.set_ylim(d_bin_edges['log'][cathode_setting][degree_setting][0], d_bin_edges['log'][cathode_setting][degree_setting][-1])
fig_s1_log.colorbar(pcm, ax=ax_s1_log_mc)




print 'Cleaning up noise outside NR band...'
# going to get mean and RMS of each MC slice and cut down
# data DF to clean up the comparisons since these rare events
# not included in match
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

    num_std_away = 2.5

    # must apply to both data and MC!
    current_df_data = current_df_data[(current_df_data['s1'] < s1_edges_mc[bin_number]) | (current_df_data['s1'] > s1_edges_mc[bin_number+1]) | ((current_df_data['log'] > (current_mean_log-num_std_away*current_std_log)) & (current_df_data['log'] < (current_mean_log+num_std_away*current_std_log)))]
    df_mc = df_mc[(df_mc['s1'] < s1_edges_mc[bin_number]) | (df_mc['s1'] > s1_edges_mc[bin_number+1]) | ((df_mc['log'] > (current_mean_log-num_std_away*current_std_log)) & (df_mc['log'] < (current_mean_log+num_std_away*current_std_log)))]



print 'Cutting on edges of 2D Hist...'
current_df_data = current_df_data[(current_df_data['s1'] > s1_edges_mc[0]) & (current_df_data['s1'] < s1_edges_mc[-1]) & (current_df_data['log'] > log_edges_mc[0]) & (current_df_data['log'] < log_edges_mc[-1])]
df_mc = df_mc[(df_mc['s1'] > s1_edges_mc[0]) & (df_mc['s1'] < s1_edges_mc[-1]) & (df_mc['log'] > log_edges_mc[0]) & (df_mc['log'] < log_edges_mc[-1])]




best_fit_mc_2d_hist = hist_s1_log_mc

print '\n\nFinding p-value via PFF test...\n'


num_d_calculations = 200*5
a_ps = np.zeros(num_d_calculations)
num_to_choose_mc = 2000
num_to_choose_data = 200#len(current_df_data['s1'])

a_ds_data = np.zeros(num_d_calculations)
a_ds_synthetic_data = np.zeros(num_d_calculations)

for current_p_iteration in xrange(num_d_calculations):
    if len(df_mc['s1']) < num_to_choose_mc:
        num_to_choose_mc = len(df_mc['s1'])
    

    # create arrays for finding pvalue
    a_s1_mc = np.asarray(df_mc['s1'])
    a_log_mc = np.asarray(df_mc['log'])
    a_weights_mc = np.asarray(df_mc['weights'])
    a_probs = a_weights_mc/np.sum(a_weights_mc)
    
    idx_mc = np.random.choice(len(a_s1_mc), num_to_choose_mc, replace=True, p=a_probs)
    idx_data = np.random.choice(len(current_df_data['s1']), num_to_choose_data, replace=True)
    idx_synthetic_data = np.random.choice(len(a_s1_mc), num_to_choose_data, replace=True, p=a_probs)
    
    p_value_pff, D_pff = pff_test.ks2d2s(a_s1_mc[idx_mc], a_log_mc[idx_mc], np.asarray(current_df_data['s1'])[idx_data], np.asarray(current_df_data['log'])[idx_data], extra=True)
    
    a_ps[current_p_iteration] = p_value_pff
    a_ds_data[current_p_iteration] = D_pff

    p_value_pff_synthetic, D_pff_synthetic = pff_test.ks2d2s(a_s1_mc[idx_mc], a_log_mc[idx_mc], a_s1_mc[idx_synthetic_data], a_log_mc[idx_synthetic_data], extra=True)
    
    a_ds_synthetic_data[current_p_iteration] = D_pff_synthetic
    #print D_pff, D_pff_synthetic
    

#fig, ax_1 = plt.subplots(1)
#ax_1.hist(a_ps, bins=20)
#plt.show()
    
#print 'p-value: %f' % (np.median(a_ps))
#print 'D: %f\n\n\n' % (D_pff)

#print a_ds_synthetic_data
#print a_ds_synthetic_data[(a_ds_synthetic_data > np.median(a_ds_data))]
print 'median D: %f' % (np.median(a_ds_data))
print 'Percentage synthetic above median: %.4f\n\n\n' % (len(a_ds_synthetic_data[a_ds_synthetic_data > np.median(a_ds_data)]) / float(num_d_calculations))


print 'Debug!'
sys.exit()


# create 2d histogram of data
hist_s1_log_data = ax_s1_log_data.hist2d(current_df_data['s1'], current_df_data['log'], bins=[d_bin_edges['s1'][cathode_setting][degree_setting], d_bin_edges['log'][cathode_setting][degree_setting]], cmap='Blues')
fig_s1_log.colorbar(hist_s1_log_data[3], ax=ax_s1_log_data)

hist_s1_log_data_for_save, _, _ = np.histogram2d(current_df_data['s1'], current_df_data['log'], bins=[d_bin_edges['s1'][cathode_setting][degree_setting], d_bin_edges['log'][cathode_setting][degree_setting]])

# see above before cuts
#data_integral = np.sum(hist_s1_log_data[0])

#print scale_par, data_integral, mc_integral
#print 'hist2d integral data: %f' % (data_integral)

# find the chi2
indices_for_log_likelihood = ((hist_s1_log_data[0] > 3) & (best_fit_mc_2d_hist > 0.01))
#print best_fit_mc_2d_hist
#print hist_s1_log_data[0]
#print indices_for_log_likelihood
current_chi2 = np.sum((best_fit_mc_2d_hist[indices_for_log_likelihood] - hist_s1_log_data[0][indices_for_log_likelihood])**2 / hist_s1_log_data[0][indices_for_log_likelihood])
current_ndf = np.sum(indices_for_log_likelihood)
print 'Chi2 / NDF: %f / %d' % (current_chi2, current_ndf)
#small_number = 1e-10
#current_ln_likelihood = np.sum(hist_s1_log_data[0][indices_for_log_likelihood]*np.log(best_fit_mc_2d_hist[indices_for_log_likelihood]+small_number) - (best_fit_mc_2d_hist[indices_for_log_likelihood]+small_number) - gammaln(hist_s1_log_data[0][indices_for_log_likelihood]+1.))
#current_ndf = np.sum(indices_for_log_likelihood)
#print 'ln(L) / NDF: %f / %d' % (current_ln_likelihood, current_ndf)

#print 'remove continue and addition'
#if degree_setting != -4:
#    count_degrees += 1
#continue

d_data_histograms = {}
d_data_histograms['s1'], _ = np.histogram(current_df_data['s1'], s1_edges)


# add alii to dictionary
d_plotting_information[cathode_setting][degree_setting]['fig_s1_log'] = fig_s1_log
d_plotting_information[cathode_setting][degree_setting]['ax_s1_projection'] = ax_s1_projection
d_plotting_information[cathode_setting][degree_setting]['ax_s1_log_data'] = ax_s1_log_data
d_plotting_information[cathode_setting][degree_setting]['ax_s1_log_mc'] = ax_s1_log_mc
d_plotting_information[cathode_setting][degree_setting]['hist_s1_log_data'] = hist_s1_log_data[0]
d_plotting_information[cathode_setting][degree_setting]['df_data'] = current_df_data
d_plotting_information[cathode_setting][degree_setting]['df_mc'] = df_mc







if degree_setting == -4:
    s2_edges_mc = np.linspace(l_s2_settings_band[1], l_s2_settings_band[2], mc_bin_number_multiplier*l_s2_settings_band[0]+1)
    s2_mc_bin_width = (l_s2_settings_band[2]-l_s2_settings_band[1])/float(l_s2_settings_band[0]*mc_bin_number_multiplier)
    s2_bin_centers_mc = np.linspace(l_s2_settings_band[1]+s2_mc_bin_width/2., l_s2_settings_band[2]-s2_mc_bin_width/2., mc_bin_number_multiplier*l_s2_settings_band[0])
else:
    s2_edges_mc = np.linspace(l_s2_settings[1], l_s2_settings[2], mc_bin_number_multiplier*l_s2_settings[0]+1)
    s2_mc_bin_width = (l_s2_settings[2]-l_s2_settings[1])/float(l_s2_settings[0]*mc_bin_number_multiplier)
    s2_bin_centers_mc = np.linspace(l_s2_settings[1]+s2_mc_bin_width/2., l_s2_settings[2]-s2_mc_bin_width/2., mc_bin_number_multiplier*l_s2_settings[0])



df_histograms = {}
df_histograms['s1'] = np.zeros((num_walkers*num_steps, len(s1_edges_mc)-1), dtype=float)

df_histograms['s2'] = {}
d_s2_data_slices = {}
for s1_cut_num in xrange(num_s1_cuts):
    current_set_s1_cuts = (d_s1_cuts[degree_setting][s1_cut_num], d_s1_cuts[degree_setting][s1_cut_num+1])
    df_histograms['s2'][current_set_s1_cuts] = np.zeros((num_walkers*num_steps, len(s2_edges_mc)-1), dtype=float)
    
    # get data slices
    cut_df = current_df_data[(current_df_data['s1'] > d_s1_cuts[degree_setting][s1_cut_num]) & (current_df_data['s1'] < d_s1_cuts[degree_setting][s1_cut_num+1])]
    d_s2_data_slices[current_set_s1_cuts] = cut_df['s2']



print '\nStarting bands in S1 and S2\n'
l_dfs = [0 for i in xrange(num_walkers*num_steps)]

for i in tqdm.tqdm(xrange(num_walkers*num_steps)):
    # create dictionary to hold relevant information
    l_dfs[i] = {}


    #a_fit_parameters = a_samples[-(np.random.randint(1, num_steps_to_include*num_walkers) % total_length_sampler), :]
    #print 'debugging so no random int'
    a_fit_parameters = a_samples[-i, :]

    # load parameters into proper variables
    num_trials = np.asarray(num_mc_events, dtype=np.int32)
    mean_field = np.asarray(d_cathode_voltages_to_field[cathode_setting], dtype=np.float32)

    w_value = np.asarray(a_fit_parameters[0], dtype=np.float32)
    beta = np.asarray(a_fit_parameters[1], dtype=np.float32)
    kappa = np.asarray(a_fit_parameters[2], dtype=np.float32)
    eta = np.asarray(a_fit_parameters[3], dtype=np.float32)
    
    if b_bq_fixed:
        lamb = np.asarray(0.5, dtype=np.float32)
    else:
        lamb = np.asarray(a_fit_parameters[4], dtype=np.float32)

    g1_value = np.asarray(a_fit_parameters[5], dtype=np.float32)
    spe_res = np.asarray(a_fit_parameters[6], dtype=np.float32)
    extraction_efficiency = np.asarray(a_fit_parameters[7], dtype=np.float32)
    gas_gain_value = np.asarray(a_fit_parameters[8], dtype=np.float32)
    gas_gain_width = np.asarray(a_fit_parameters[9], dtype=np.float32)


    #print cathode_setting, degree_setting
    #print intrinsic_res_s1, intrinsic_res_s2

    pf_eff_par0 = np.asarray(a_fit_parameters[10], dtype=np.float32)
    pf_eff_par1 = np.asarray(a_fit_parameters[11], dtype=np.float32)

    s2_eff_par0 = np.asarray(a_fit_parameters[12], dtype=np.float32)
    s2_eff_par1 = np.asarray(a_fit_parameters[13], dtype=np.float32)

    a_pf_stdev_s1 = np.asarray([a_fit_parameters[14], a_fit_parameters[15], a_fit_parameters[16]], dtype=np.float32)
    a_pf_stdev_s2 = np.asarray([a_fit_parameters[17], a_fit_parameters[18], a_fit_parameters[19]], dtype=np.float32)
    
    dpe_prob = np.asarray(a_fit_parameters[20], dtype=np.float32)

    a_s1_mc_current_iteration = np.zeros(num_mc_events, dtype=np.float32)
    a_s2_mc_current_iteration = np.zeros(num_mc_events, dtype=np.float32)
    a_weight = np.zeros(num_mc_events, dtype=np.float32)
    
    ti_par = np.asarray(a_fit_parameters[20+1+count_fields*num_degree_field_pars], dtype=np.float32)

    exciton_to_ion_ratio = np.asarray(a_fit_parameters[20+2+count_fields*num_degree_field_pars], dtype=np.float32)

    if degree_setting == -4:
        prob_bkg = np.asarray(0., dtype=np.float32)
        scale_par = np.asarray(a_fit_parameters[20+3+count_fields*num_degree_field_pars], dtype=np.float32)
    else:
        prob_bkg = np.asarray(a_fit_parameters[20+4+count_fields*num_degree_field_pars + 2*count_degrees], dtype=np.float32)
        scale_par = np.asarray(a_fit_parameters[20+5+count_fields*num_degree_field_pars + 2*count_degrees], dtype=np.float32)

    #print prob_bkg, scale_par

    tArgs = (local_rng_states, drv.In(num_trials), drv.In(mean_field), d_plotting_information[cathode_setting][degree_setting]['gpu_energy'], gpu_bkg_energy, drv.In(w_value), drv.In(exciton_to_ion_ratio), drv.In(ti_par), drv.In(beta), drv.In(kappa), drv.In(eta), drv.In(lamb), drv.In(g1_value), drv.In(extraction_efficiency), drv.In(gas_gain_value), drv.In(gas_gain_width), drv.In(spe_res), drv.In(a_pf_stdev_s1), drv.In(a_pf_stdev_s2), drv.In(pf_eff_par0), drv.In(pf_eff_par1), drv.In(s2_eff_par0), drv.In(s2_eff_par1), drv.In(dpe_prob), drv.In(prob_bkg), drv.InOut(a_s1_mc_current_iteration), drv.InOut(a_s2_mc_current_iteration), drv.InOut(a_weight))
    
    gpu_observables_func(*tArgs, **d_gpu_scale)
    
    
    l_dfs[i]['main_df'] = pd.DataFrame({'s1':a_s1_mc_current_iteration, 's2':a_s2_mc_current_iteration, 'weights':a_weight})
    l_dfs[i]['main_df'] = l_dfs[i]['main_df'][(l_dfs[i]['main_df']['s1'] > 0)]
    l_dfs[i]['main_df']['log'] = np.log10(df_mc['s2']/df_mc['s1'])


    
    l_dfs[i]['s1_hist'], _ = np.histogram(l_dfs[i]['main_df']['s1'], s1_edges_mc, weights=l_dfs[i]['main_df']['weights'])
    l_dfs[i]['s1_hist'] = np.asarray(l_dfs[i]['s1_hist'], dtype=float)
    
    
    # scale_factor for histograms
    scaling_factor_for_histogram = scale_par*data_integral*mc_bin_number_multiplier/mc_integral / data_bin_multiplier
    
    
    l_dfs[i]['s1_hist'] *= scaling_factor_for_histogram
    
    df_histograms['s1'][i, :] = l_dfs[i]['s1_hist']
    

    l_dfs[i]['s2_hist_after_s1_cuts'] = {}
    for s1_cut_num in xrange(num_s1_cuts):
        # make cut
        cut_df = l_dfs[i]['main_df'][(l_dfs[i]['main_df']['s1'] > d_s1_cuts[degree_setting][s1_cut_num]) & (l_dfs[i]['main_df']['s1'] < d_s1_cuts[degree_setting][s1_cut_num+1])]
        
        # add into dictionary
        l_dfs[i]['s2_hist_after_s1_cuts'][(d_s1_cuts[degree_setting][s1_cut_num], d_s1_cuts[degree_setting][s1_cut_num+1])],  _ = np.histogram(cut_df['s2'], s2_edges_mc, weights=cut_df['weights'])
        l_dfs[i]['s2_hist_after_s1_cuts'][(d_s1_cuts[degree_setting][s1_cut_num], d_s1_cuts[degree_setting][s1_cut_num+1])] = np.asarray(l_dfs[i]['s2_hist_after_s1_cuts'][(d_s1_cuts[degree_setting][s1_cut_num], d_s1_cuts[degree_setting][s1_cut_num+1])], dtype=float)
        l_dfs[i]['s2_hist_after_s1_cuts'][(d_s1_cuts[degree_setting][s1_cut_num], d_s1_cuts[degree_setting][s1_cut_num+1])] *= scaling_factor_for_histogram

        df_histograms['s2'][(d_s1_cuts[degree_setting][s1_cut_num], d_s1_cuts[degree_setting][s1_cut_num+1])][i, :] = l_dfs[i]['s2_hist_after_s1_cuts'][(d_s1_cuts[degree_setting][s1_cut_num], d_s1_cuts[degree_setting][s1_cut_num+1])]



# now use d_histograms to fill d_quantiles
d_quantiles = {}
l_quantiles = [16, 50, 84]
assert len(l_quantiles) == 3, 'Must use exactly three quantiles for analysis'

print '\nGetting quantiles for S1 Histograms...\n'

d_quantiles['s1'] = {}
for quantile in l_quantiles:
    d_quantiles['s1'][quantile] = np.zeros(len(s1_edges_mc)-1)
    for bin_number in xrange(len(s1_edges_mc)-1):
        d_quantiles['s1'][quantile][bin_number] = np.percentile(df_histograms['s1'][:, bin_number], quantile)
        #print df_histograms['s1'][:, bin_number]


print '\nGetting quantiles for S2 Histograms...\n'

d_quantiles['s2'] = {}
d_data_histograms['s2'] = {}
for s1_cut_num in xrange(num_s1_cuts):
    current_set_s1_cuts = (d_s1_cuts[degree_setting][s1_cut_num], d_s1_cuts[degree_setting][s1_cut_num+1])
    d_quantiles['s2'][current_set_s1_cuts] = {}
    for quantile in l_quantiles:
        d_quantiles['s2'][current_set_s1_cuts][quantile] = np.zeros(len(s2_edges_mc)-1)
        for bin_number in xrange(len(s2_edges_mc)-1):
            d_quantiles['s2'][current_set_s1_cuts][quantile][bin_number] = np.percentile(df_histograms['s2'][current_set_s1_cuts][:, bin_number], quantile)
    d_data_histograms['s2'][current_set_s1_cuts], _ = np.histogram(d_s2_data_slices[current_set_s1_cuts], s2_edges)





# remove all events outside bounds
df_mc = df_mc[(df_mc['s1'] > s1_edges_mc[0]) & (df_mc['s1'] < s1_edges_mc[-1]) & (df_mc['log'] > log_edges_mc[0]) & (df_mc['log'] < log_edges_mc[-1])]


# grab median lines for each
#assert l_s1_settings[0] == l_log_settings[0]

num_s1_bins = len(d_bin_edges['s1'][cathode_setting][degree_setting])-1

a_median_line_s1 = np.zeros(num_s1_bins)
a_median_line_data_log = np.zeros(num_s1_bins)
a_median_line_mc_log = np.zeros(num_s1_bins)

for i in xrange(num_s1_bins):
    a_median_line_s1[i] = (s1_edges[i+1] + s1_edges[i]) / 2.

    df_data_median_line = current_df_data[(current_df_data['s1'] > s1_edges[i]) & (current_df_data['s1'] < s1_edges[i+1])]
    current_df_mc = df_mc[(df_mc['s1'] > s1_edges[i]) & (df_mc['s1'] < s1_edges[i+1])]

    if len(df_data_median_line['log']) == 0:
        a_median_line_data_log[i] = a_median_line_data_log[i-1]
    else:
        #print current_df_data['log']
        #print np.median(current_df_data['log'])
        #print '\n\n'
        a_median_line_data_log[i] = np.median(df_data_median_line['log'])

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

ax_s1_projection.set_xlim(d_bin_edges['s1'][cathode_setting][degree_setting][0], d_bin_edges['s1'][cathode_setting][degree_setting][-1])

#print len(s1_bin_centers_mc)
#print s1_bin_centers_mc


# produce 1D plots of S1 and slices of S2
fig_s2s, l_s2_axes = plt.subplots(num_s1_cuts, figsize=figure_sizes)
#fig_s2s.tight_layout()
fig_s2s.subplots_adjust(**d_subplot_space)

d_s2_plots = {}
for i, s1_cut_num in enumerate(xrange(num_s1_cuts)):
    current_set_s1_cuts = (d_s1_cuts[degree_setting][s1_cut_num], d_s1_cuts[degree_setting][s1_cut_num+1])
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
    
    
fig_s1_log.savefig('%s%.3f_kV_%d_deg_data_mc_1.png' % (s_path_for_save, cathode_setting, degree_setting))
fig_s2s.savefig('%s%.3f_kV_%d_deg_data_mc_2.png' % (s_path_for_save, cathode_setting, degree_setting))


d_for_save = {}
d_for_save_high_stats = {}
# want to save down pickle files
# S1 cuts used
d_for_save['s1_cuts'] = d_s1_cuts
# 2D histograms
d_for_save['s1_bin_edges'] = d_bin_edges['s1'][cathode_setting][degree_setting]
d_for_save['log_bin_edges'] = d_bin_edges['log'][cathode_setting][degree_setting]
d_for_save['2d_hist_data'] = hist_s1_log_data_for_save
d_for_save['2d_hist_mc'] = hist_s1_log_mc

d_for_save['s1_bin_edges_fine'] = d_bin_edges['s1_mc'][cathode_setting][degree_setting]
d_for_save['log_bin_edges_fine'] = d_bin_edges['log_mc'][cathode_setting][degree_setting]
d_for_save['2d_hist_mc_fine'] = hist_s1_log_mc_fine
d_for_save_high_stats['s1_bin_edges_fine'] = d_bin_edges['s1_mc'][cathode_setting][degree_setting]
d_for_save_high_stats['log_bin_edges_fine'] = d_bin_edges['log_mc'][cathode_setting][degree_setting]
d_for_save_high_stats['2d_hist_mc_fine'] = hist_s1_log_mc_fine

# median lines
d_for_save['median_line_s1'] = a_median_line_s1
d_for_save['median_line_data_log'] = a_median_line_data_log
d_for_save['median_line_mc_log'] = a_median_line_mc_log

# S1 projections with band and best fit
d_for_save['s1_projection_x_values'] = a_s1_x_values
d_for_save['s1_projection_y_values'] = a_s1_y_values
d_for_save['s1_projection_x_err_low'] = a_s1_x_err_low
d_for_save['s1_projection_x_err_high'] = a_s1_x_err_high
d_for_save['s1_projection_y_err_low'] = a_s1_y_err_low
d_for_save['s1_projection_y_err_high'] = a_s1_y_err_high
d_for_save['s1_projection_bin_centers_mc'] = s1_bin_centers_mc
d_for_save['s1_projection_best_fit_line'] = d_quantiles['s1'][l_quantiles[1]]
d_for_save['s1_projection_unc_band_low'] = d_quantiles['s1'][l_quantiles[0]]
d_for_save['s1_projection_unc_band_high'] = d_quantiles['s1'][l_quantiles[2]]

# S2 matches
# remove axis
d_for_save['s2_projection_best_fit_line'] = {}
d_for_save['s2_projection_unc_band_low'] = {}
d_for_save['s2_projection_unc_band_high'] = {}

for i, s1_cut_num in enumerate(xrange(num_s1_cuts)):
    current_set_s1_cuts = (d_s1_cuts[degree_setting][s1_cut_num], d_s1_cuts[degree_setting][s1_cut_num+1])
    del d_s2_plots[current_set_s1_cuts]['axis']
    d_for_save['s2_projection_best_fit_line'][current_set_s1_cuts] = d_quantiles['s2'][current_set_s1_cuts][l_quantiles[1]]
    d_for_save['s2_projection_unc_band_low'][current_set_s1_cuts] = d_quantiles['s2'][current_set_s1_cuts][l_quantiles[0]]
    d_for_save['s2_projection_unc_band_high'][current_set_s1_cuts] = d_quantiles['s2'][current_set_s1_cuts][l_quantiles[2]]

d_for_save['d_s2_plots'] = d_s2_plots
d_for_save['s2_bin_centers_mc'] = s2_bin_centers_mc



# pickle file
pickle.dump(d_for_save, open('%s%.3f_kV_%d_deg_comps.p' % (s_path_for_save, cathode_setting, degree_setting), 'wb'))
pickle.dump(d_for_save_high_stats, open('%s%.3f_kV_%d_deg_comps_high_stats.p' % (s_path_for_save, cathode_setting, degree_setting), 'wb'))



# end GPU context
ctx.pop()
print 'Successfully ended GPU context!'





#plt.show()



