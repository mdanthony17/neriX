#!/usr/bin/python
import sys, array, os
sys.path.insert(0, '../..')

import matplotlib
matplotlib.use('QT4Agg')

matplotlib.rcParams['mathtext.fontset'] = 'stix'
matplotlib.rcParams['font.family'] = 'STIXGeneral'
#matplotlib.rcParams['font.size'] = 16

import matplotlib.pyplot as plt
import matplotlib.dates as mdates
import matplotlib.patches as mpatches

import ROOT as root
from rootpy.plotting import Hist, Hist2D, Canvas, Legend
import neriX_simulation_config
import numpy as np
import corner, time, tqdm
import cPickle as pickle



d_degree_setting_to_energy_name = {2300:3,
                                   3000:5,
                                   3500:7,
                                   4500:10,
                                   5300:15,
                                   6200:20}



num_steps = 1000
num_walkers =512
file_descriptor = 'mlti_bq_fixed'

if not (file_descriptor == 'ml' or file_descriptor == 'mlti' or file_descriptor == 'mlti_moved_pos' or file_descriptor == 'mlti_bq_fixed'):
    print 'Need to give proper file type'
    sys.exit()

l_quantiles_range = [2, 98]
l_quantiles_plot = [16, 50, 84]

if file_descriptor == 'ml':
    directory_descriptor = 'multiple_energies_lindhard_model'
elif file_descriptor == 'mlti':
    directory_descriptor = 'multiple_energies_lindhard_model_with_ti'
elif file_descriptor == 'mlti_moved_pos':
    directory_descriptor = 'multiple_energies_lindhard_model_with_ti_moved_pos'
elif file_descriptor == 'mlti_bq_fixed':
    directory_descriptor = 'multiple_energies_lindhard_model_with_ti_bq_fixed'

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
elif file_descriptor[:4] == 'mlti':
    num_dim = 21 + len(l_cathode_settings_in_use)*len(l_degree_settings_in_use)*2 + len(l_cathode_settings_in_use)*(1+1) - len(l_cathode_settings_in_use)


l_field_free_pars = []
if file_descriptor == 'ml':
    for cathode_setting in l_cathode_settings_in_use:
        l_field_free_pars += ['gom_par0_%.3f_kV' % (cathode_setting), 'gom_par1_%.3f_kV' % (cathode_setting), 'gom_par2_%.3f_kV' % (cathode_setting), 'gom_par3_%.3f_kV' % (cathode_setting), 'exciton_to_ion_ratio_%.3f_kV' % (cathode_setting)]
        
        for degree_setting in l_degree_settings_in_use:
            l_field_free_pars.append('prob_bkg_%.3f_kV_%d_deg' % (cathode_setting, degree_setting))
            l_field_free_pars.append('scale_%.3f_kV_%d_deg' % (cathode_setting, degree_setting))

        l_par_names = ['w_value', 'kappa', 'intrinsic_res_s1', 'intrinsic_res_s2', 'g1_value', 'spe_res_value', 'extraction_efficiency_value', 'gas_gain_mean_value', 'gas_gain_width_value', 'pf_eff_par0', 'pf_eff_par1', 's2_eff_par0', 's2_eff_par1', 'pf_stdev_par0', 'pf_stdev_par1', 'pf_stdev_par2', 'dpe_prob'] + l_field_free_pars + ['band_res_s1', 'band_res_s2']


elif file_descriptor[:4] == 'mlti':
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

l_exciton_to_ion_ratio = []
l_ti_par = []

a_medians = np.median(samples, axis=0)
print a_medians
print '\nPrinting median parameters...'
for i, par_name in enumerate(l_par_names):
    #print '%s = %.2e' % (par_name, a_medians[i])
    lb, med, ub = np.percentile(samples[:, i], [16., 50., 84.])
    print '%s = %.2e, + %.2e, - %.2e' % (par_name, med, ub-med, med-lb)

    if par_name[:2] == 'ti':
        l_ti_par.append(samples[:, i])
    if par_name[:4] == 'exci':
        l_exciton_to_ion_ratio.append(samples[:, i])
    if par_name == 'kappa':
        l_kappa = samples[:, i]
    if par_name == 'beta':
        l_beta = samples[:, i]
    if par_name == 'eta':
        l_eta = samples[:, i]
    if par_name == 'w_value':
        l_w = samples[:, i]

l_exciton_to_ion_ratio = np.concatenate(l_exciton_to_ion_ratio)
l_ti_par = np.concatenate(l_ti_par)

l_kappa = np.asarray(l_kappa)
l_exciton_to_ion_ratio = np.asarray(l_exciton_to_ion_ratio)
l_beta = np.asarray(l_beta)
l_ti_par = np.asarray(l_ti_par)
l_eta = np.asarray(l_eta)
l_w = np.asarray(l_w)

# reduce size of merged arrays
l_exciton_to_ion_ratio = np.random.choice(l_exciton_to_ion_ratio, size=len(l_kappa))
l_ti_par = np.random.choice(l_ti_par, size=len(l_kappa))



print 'Starting corner plot...\n'
f_pars = corner.corner(np.asarray([l_w, l_kappa, l_exciton_to_ion_ratio, l_beta, l_ti_par, l_eta]).T, labels=[r'W', r'$k$', r'$\alpha$', r'$\beta$', r'$\sigma$', r'$\eta$'], quantiles=[0.16, 0.5, 0.84], show_titles=False, title_fmt='.3e', title_kwargs={'fontsize': 8}, label_kwargs={'fontsize':20}, range=[0.990, 0.995, 0.995, (0,5000), 0.995, 0.995], use_math_text=True, plot_datapoints=False, plot_density=True, plot_contours=False, no_fill_contours=True, smooth=True)
#f_pars = corner.corner(np.asarray([l_kappa, l_exciton_to_ion_ratio, l_beta, l_ti_par, l_eta]).T)

a_pars = np.asarray([l_w, l_kappa, l_exciton_to_ion_ratio, l_ti_par, l_eta]).T
print np.mean(a_pars, axis=0)
print '\n\n'
a_cov = np.cov(a_pars.T)

for row in a_cov:
    s_vals = ''
    for val in row:
        s_vals += '%.2e, ' % (val)
    s_vals += '\n'
    print s_vals

#f_beta, ax_beta = plt.subplots(1)
#ax_beta.hist(l_beta, bins=100, range=[0,2000])

    
f_pars.savefig('./plots/corner_plot_physical_pars.png')

#plt.show()
