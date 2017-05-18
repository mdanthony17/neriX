#!/usr/bin/python
import sys, array, os
sys.path.insert(0, '..')

import matplotlib
matplotlib.use('QT4Agg')
import matplotlib.pyplot as plt
import matplotlib.dates as mdates
import matplotlib.patches as mpatches

import ROOT as root
from rootpy.plotting import Hist, Hist2D, Canvas, Legend
import neriX_simulation_config
import numpy as np
import corner, time, tqdm
import cPickle as pickle

if len(sys.argv) != 3:
    print 'Use is python make_1d_parameter_plots_coincidence.py <num walkers> <file type string>'
    sys.exit()



d_degree_setting_to_energy_name = {2300:3,
                                   3000:5,
                                   3500:7,
                                   4500:10,
                                   5300:15,
                                   6200:20}



num_steps = 500
num_walkers = int(sys.argv[1])
file_descriptor = sys.argv[2]

if not (file_descriptor == 'ml' or file_descriptor == 'mlti'):
    print 'Need to give proper file type'
    sys.exit()

l_quantiles_range = [2, 98]
l_quantiles_plot = [16, 50, 84]

if file_descriptor == 'ml':
    directory_descriptor = 'multiple_energies_lindhard_model'
elif file_descriptor == 'mlti':
    directory_descriptor = 'multiple_energies_lindhard_model_with_ti'


l_degree_settings_in_use = [-4, 3000, 3500, 4500, 5300]
s_degree_settings = ''
for degree_setting in l_degree_settings_in_use:
    s_degree_settings += '%s,' % (degree_setting)
s_degree_settings = s_degree_settings[:-1]


l_cathode_settings_in_use = [0.345, 1.054, 2.356]
s_cathode_settings = ''
for cathode_setting in l_cathode_settings_in_use:
    s_cathode_settings += '%.3f,' % (cathode_setting)
s_cathode_settings = s_cathode_settings[:-1]

nameOfResultsDirectory = neriX_simulation_config.results_directory_name
l_plots = ['plots', directory_descriptor, '%s_kV_%s_deg' % (s_cathode_settings, s_degree_settings)]

dir_specifier_name = '%s_kV_%s_deg' % (s_cathode_settings, s_degree_settings)

nameOfResultsDirectory += '/%s' % (directory_descriptor)

sPathToFile = './%s/%s/sampler_dictionary.p' % (nameOfResultsDirectory, dir_specifier_name)

if os.path.exists(sPathToFile):
    dSampler = pickle.load(open(sPathToFile, 'r'))
    l_chains = []
    for sampler in dSampler[num_walkers]:
        l_chains.append(sampler['_chain'])

    a_full_sampler = np.concatenate(l_chains, axis=1)

    print 'Successfully loaded sampler!'
else:
    print sPathToFile
    print 'Could not find file!'
    sys.exit()



# 17 + scales + recombination + exciton-to-ion ratio + bkgs + scales
if file_descriptor == 'ml':
    num_dim = 17 + len(l_cathode_settings_in_use)*(4+1) + len(l_cathode_settings_in_use)*len(l_degree_settings_in_use)*2
elif file_descriptor == 'mlti':
    num_dim = 21 + len(l_cathode_settings_in_use)*len(l_degree_settings_in_use)*2 + len(l_cathode_settings_in_use)*(1+1) - len(l_cathode_settings_in_use)


l_field_free_pars = []
if file_descriptor == 'ml':
    for cathode_setting in l_cathode_settings_in_use:
        l_field_free_pars += ['gom_par0_%.3f_kV' % (cathode_setting), 'gom_par1_%.3f_kV' % (cathode_setting), 'gom_par2_%.3f_kV' % (cathode_setting), 'gom_par3_%.3f_kV' % (cathode_setting), 'exciton_to_ion_ratio_%.3f_kV' % (cathode_setting)]
        
        for degree_setting in l_degree_settings_in_use:
            l_field_free_pars.append('prob_bkg_%.3f_kV_%d_deg' % (cathode_setting, degree_setting))
            l_field_free_pars.append('scale_%.3f_kV_%d_deg' % (cathode_setting, degree_setting))

        l_par_names = ['w_value', 'kappa', 'intrinsic_res_s1', 'intrinsic_res_s2', 'g1_value', 'spe_res_value', 'extraction_efficiency_value', 'gas_gain_mean_value', 'gas_gain_width_value', 'pf_eff_par0', 'pf_eff_par1', 's2_eff_par0', 's2_eff_par1', 'pf_stdev_par0', 'pf_stdev_par1', 'pf_stdev_par2', 'dpe_prob'] + l_field_free_pars + ['band_res_s1', 'band_res_s2']


elif file_descriptor == 'mlti':
    for cathode_setting in l_cathode_settings_in_use:
        l_field_free_pars += ['ti_par_%.3f_kV' % (cathode_setting), 'exciton_to_ion_ratio_%.3f_kV' % (cathode_setting)]
        
        for degree_setting in l_degree_settings_in_use:
            if degree_setting != -4:
                l_field_free_pars.append('prob_bkg_%.3f_kV_%d_deg' % (cathode_setting, degree_setting))
            
            l_field_free_pars.append('scale_%.3f_kV_%d_deg' % (cathode_setting, degree_setting))


    l_par_names = ['w_value', 'beta', 'kappa', 'eta', 'lambda', 'g1_value', 'spe_res_value', 'extraction_efficiency_value', 'gas_gain_mean_value', 'gas_gain_width_value', 'pf_eff_par0', 'pf_eff_par1', 's2_eff_par0', 's2_eff_par1', 'pf_stdev_s1_par0', 'pf_stdev_s1_par1', 'pf_stdev_s1_par2', 'pf_stdev_s2_par0', 'pf_stdev_s2_par1', 'pf_stdev_s2_par2', 'dpe_prob'] + l_field_free_pars


num_rows = 1
for i in xrange(1, 10):
    if i**2 > len(l_par_names):
        num_rows = i
        break

for num_columns in xrange(1, num_rows+1):
    if (num_columns-1)*num_rows >= num_dim:
        break


assert num_dim == len(l_par_names)
samples = a_full_sampler[:, -num_steps:, :].reshape((-1, num_dim))


# use pars from corner to make figure
factor = 2.0           # size of one side of one panel
lbdim = 0.5 * factor   # size of left/bottom margin
trdim = 0.2 * factor   # size of top/right margin
whspace = 0.05         # w/hspace size
plotdim = factor * num_dim + factor * (num_dim - 1.) * whspace
dim = lbdim + plotdim + trdim

lb = lbdim / dim
tr = (lbdim + plotdim) / dim


f_pars, l_a_pars = plt.subplots(1*num_rows, 1*num_columns, figsize=(dim, dim))
f_pars.subplots_adjust(left=lb, bottom=lb, right=tr, top=tr,
                        wspace=whspace, hspace=whspace)
l_a_pars = l_a_pars.flatten()

for par_number, par_name in enumerate(l_par_names):
    min_range, max_range = np.percentile(samples[:, par_number], l_quantiles_range)
    q25, q75 = np.percentile(samples[:, par_number], [25, 75])
    bin_width = 2 * (q75-q25) * (samples.shape[0])**(-1./3.)
    
    a_bins = np.arange(min_range, max_range+bin_width, bin_width)
    l_a_pars[par_number].hist(samples[:, par_number], bins=a_bins, color='k')

    l_a_pars[par_number].set_title(par_name, fontsize=12)

#plt.tight_layout()
#plt.rcParams.update({'font.size': 10})

# path for save
s_path_for_save = './'
for directory in l_plots:
    s_path_for_save += directory + '/'

if not os.path.exists(s_path_for_save):
    os.makedirs(s_path_for_save)

    
f_pars.savefig('%ss_1d_pars_%s.png' % (s_path_for_save, dir_specifier_name))



#raw_input('Enter to continue...')
