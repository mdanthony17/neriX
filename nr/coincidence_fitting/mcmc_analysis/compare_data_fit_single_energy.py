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


s1_nb = 40
s1_min = 1.5
s1_max = 40

s2_nb = 40
s2_min = 180
s2_max = 4000

kde_nb = 100

l_contour_levels = [0.1*i for i in xrange(1, 11)]#[0.2, 0.5, 0.9]
transparency = 0.3
l_quantiles = [50]



if(len(sys.argv) != 4):
	print 'Usage is python compare_data_fit_single_energy.py <cathode> <degree> <num walkers>'
	sys.exit(1)

cathode_setting = float(sys.argv[1])
degree_setting = int(sys.argv[2])
num_walkers = int(sys.argv[3])


l_save_path = ['plots', '%.3f_kV' % cathode_setting, '%d_deg' % (degree_setting)]


d_degree_setting_to_energy_name = {2300:3,
                                   3000:5,
                                   3500:7,
                                   4500:10,
                                   5300:15,
                                   6200:20}


directory_descriptor = 'single_energy'


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


num_steps = 1000

num_mc_events = int(2e4)
# get block and grid size
d_gpu_scale = {}
block_dim = 1024
d_gpu_scale['block'] = (block_dim,1,1)
num_blocks = floor(num_mc_events / float(block_dim))
d_gpu_scale['grid'] = (int(num_blocks), 1)
num_mc_events = int(num_blocks*block_dim)




# get significant value sets to loop over later
a_samples = a_full_sampler[:, -num_steps:, :].reshape((-1, a_full_sampler.shape[2]))
a_percentile_values = np.percentile(a_samples, l_quantiles, axis=0)




l_s1_settings = [20, 1, 40]
l_log_settings = [20, 1, 3.5]

s_main_save_directory = '../../coincidence_analysis/results/%.3fkV_%ddeg/' % (cathode_setting, degree_setting)

d_s1_s2 = pickle.load(open('%ss1_s2.pkl' % (s_main_save_directory), 'r'))

#print d_s1_s2

fig_s1_log, (ax_s1_log_data, ax_s1_log_mc) = plt.subplots(2, sharex=True, sharey=True)



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

# need axis image for colorbar
hist_s1_log_data = ax_s1_log_data.hist2d(a_s1_data, np.log10(a_s2_data/a_s1_data), bins=[l_s1_settings[0], l_log_settings[0]], range=[[l_s1_settings[1], l_s1_settings[2]], [l_log_settings[1], l_log_settings[2]]], cmap='Blues')
fig_s1_log.colorbar(hist_s1_log_data[3], ax=ax_s1_log_data)


"""
#h_data, x_edges, y_edges = np.histogram2d(a_test_transformed[:,0], np.log10(a_test_transformed[:,1]/a_test_transformed[:,0]), range=[l_s1_settings[1:], l_log_settings[1:]], bins=[l_s1_settings[0], l_log_settings[0]], normed=True)

#ax_s1_log.hist2d(a_test_transformed[:,0], np.log10(a_test_transformed[:,1]/a_test_transformed[:,0]), range=[l_s1_settings[1:], l_log_settings[1:]], bins=[l_s1_settings[0], l_log_settings[0]], normed=True)

contour_extent = [x_edges[0], x_edges[-1], y_edges[0], y_edges[-1]]
contour_levels = np.asarray(l_contour_levels)*np.max(h_data)
c_data = ax_s1_log.contourf(h_data.T, contour_levels, extent=contour_extent, cmap='Greys', alpha=transparency)
"""



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
    mean_field = np.asarray(cathode_setting, dtype=np.float32)

    py = np.asarray(a_fit_parameters[0], dtype=np.float32)
    qy = np.asarray(a_fit_parameters[1], dtype=np.float32)

    g1_value = np.asarray(a_fit_parameters[4], dtype=np.float32)
    extraction_efficiency = np.asarray(a_fit_parameters[6], dtype=np.float32)
    spe_res = np.asarray(a_fit_parameters[5], dtype=np.float32)
    gas_gain_value = np.asarray(a_fit_parameters[7], dtype=np.float32)
    gas_gain_width = np.asarray(a_fit_parameters[8], dtype=np.float32)

    intrinsic_res_s1 = np.asarray(a_fit_parameters[2], dtype=np.float32)
    intrinsic_res_s2 = np.asarray(a_fit_parameters[3], dtype=np.float32)

    exciton_to_ion_par0_rv = np.asarray(a_fit_parameters[16], dtype=np.float32)
    exciton_to_ion_par1_rv = np.asarray(a_fit_parameters[17], dtype=np.float32)
    exciton_to_ion_par2_rv = np.asarray(a_fit_parameters[18], dtype=np.float32)

    pf_eff_par0 = np.asarray(a_fit_parameters[9], dtype=np.float32)
    pf_eff_par1 = np.asarray(a_fit_parameters[10], dtype=np.float32)

    s2_eff_par0 = np.asarray(a_fit_parameters[11], dtype=np.float32)
    s2_eff_par1 = np.asarray(a_fit_parameters[12], dtype=np.float32)

    a_pf_stdev = np.asarray([a_fit_parameters[13], a_fit_parameters[14], a_fit_parameters[15]], dtype=np.float32)

    prob_bkg_event = np.asarray(a_fit_parameters[19], dtype=np.float32)
    
    a_s1_mc = np.zeros(num_mc_events, dtype=np.float32)
    a_s2_mc = np.zeros(num_mc_events, dtype=np.float32)


    tArgs = (local_rng_states, drv.In(num_trials), drv.In(mean_field), gpu_energy, gpu_energy_bkg, drv.In(py), drv.In(qy), drv.In(g1_value), drv.In(extraction_efficiency), drv.In(gas_gain_value), drv.In(gas_gain_width), drv.In(spe_res), drv.In(intrinsic_res_s1), drv.In(intrinsic_res_s2), drv.In(a_pf_stdev), drv.In(exciton_to_ion_par0_rv), drv.In(exciton_to_ion_par1_rv), drv.In(exciton_to_ion_par2_rv), drv.In(pf_eff_par0), drv.In(pf_eff_par1), drv.In(s2_eff_par0), drv.In(s2_eff_par1), drv.In(prob_bkg_event), drv.InOut(a_s1_mc), drv.InOut(a_s2_mc))
    
    gpu_observables_func(*tArgs, **d_gpu_scale)





hist_s1_log_mc = ax_s1_log_mc.hist2d(a_s1_mc, np.log10(a_s2_mc/a_s1_mc), bins=[l_s1_settings[0], l_log_settings[0]], range=[[l_s1_settings[1], l_s1_settings[2]], [l_log_settings[1], l_log_settings[2]]], cmap='Blues', normed=True)
fig_s1_log.colorbar(hist_s1_log_mc[3], ax=ax_s1_log_mc)

# end GPU context
ctx.pop()
print 'Successfully ended GPU context!'

s1_edges = hist_s1_log_data[1]
log_edges = hist_s1_log_data[2]


df_data = pd.DataFrame({'s1':a_s1_data, 's2':a_s2_data})
df_data['log'] = np.log10(df_data['s2']/df_data['s1'])

df_mc = pd.DataFrame({'s1':a_s1_mc, 's2':a_s2_mc})
df_mc['log'] = np.log10(df_mc['s2']/df_mc['s1'])

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
ax_s1_log_data.set_title(r'%d keV Data - $V_c$ = %.3f kV' % (d_degree_setting_to_energy_name[degree_setting], cathode_setting))
ax_s1_log_data.set_xlabel('S1 [PE]')
ax_s1_log_data.set_ylabel(r'$log_{10}(\frac{S2}{S1})$')

ax_s1_log_mc.set_title(r'%d keV Best Fit MC - $V_c$ = %.3f kV' % (d_degree_setting_to_energy_name[degree_setting], cathode_setting))
ax_s1_log_mc.set_xlabel('S1 [PE]')
ax_s1_log_mc.set_ylabel(r'$log_{10}(\frac{S2}{S1})$')


ax_s1_log_data.legend(handles=[line_handle_data, line_handle_mc], loc='best')



s_path_for_save = './'
for directory in l_save_path:
    s_path_for_save += directory + '/'

if not os.path.exists(s_path_for_save):
    os.makedirs(s_path_for_save)
    
    
fig_s1_log.savefig('%ss_data_mc_comparison_%s.png' % (s_path_for_save, dir_specifier_name))


#plt.show()



