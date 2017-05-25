#!/usr/bin/python
import sys, array, os
sys.path.insert(0, '..')

import matplotlib
matplotlib.use('QT4Agg')
import matplotlib.pyplot as plt
import matplotlib.mlab as mlab

import ROOT as root
import neriX_analysis
from rootpy.plotting import Hist, Hist2D, Canvas, Legend
import neriX_simulation_config
import numpy as np
import cPickle as pickle
import tqdm



def get_ly_qy_from_nest_pars(energy, mean_field, w_value, alpha, zeta, beta, gamma, delta, kappa, eta, lamb):

    dimensionless_energy = 11.5*energy*54**(-7./3.)
    g_value = 3*dimensionless_energy**0.15 + 0.7*dimensionless_energy**0.6 + dimensionless_energy
    lindhard_factor = kappa*g_value / (1. + kappa*g_value)
    penning_factor = 1. / (1. + eta*dimensionless_energy**lamb)
    sigma = gamma*mean_field**(-delta)
    
    exciton_to_ion_ratio = alpha*mean_field**(-zeta)*(1.-np.exp(-beta*dimensionless_energy))
    prob_exciton_success = 1. - 1./(1. + exciton_to_ion_ratio)

    num_quanta = energy*lindhard_factor/w_value
    num_excitons = num_quanta*prob_exciton_success
    num_ions = num_quanta - num_excitons

    prob_recombination = 1. - np.log(1.+num_ions*sigma)/(num_ions*sigma)
    num_recombined = num_ions*prob_recombination
    num_excitons += num_recombined
    num_ions -= num_recombined

    num_photons = num_excitons*penning_factor
    num_electrons = num_ions

    return num_photons/energy, num_electrons/energy



if len(sys.argv) != 3:
    print 'Use is python perform_full_matching.py <num walkers single> <num walkers band>'
    sys.exit()

print '\n\nBy default look for all energies - change source if anything else is needed.\n'


d_degree_setting_to_energy_name = {2300:3,
                                       3000:5,
                                       3500:7,
                                       4500:10,
                                       5300:15,
                                       6200:20}

d_cathode_setting_to_mean_field = {0.345:210,
                                   1.054:490,
                                   2.356:1020}


d_degree_setting_to_energy_mean = {3000:4.93,
                                   3500:6.61,
                                   4500:9.76,
                                   5300:13.88,
                                   6200:17.20}

d_degree_setting_to_energy_width = {3000:0.83,
                                    3500:1.50,
                                    4500:1.37,
                                    5300:2.35,
                                    6200:3.17}


d_yields = {}

num_walkers = int(sys.argv[1])
num_walkers_band = int(sys.argv[2])

directory_descriptor = 'single_energy'

l_cathode_settings_in_use = [0.345, 1.054, 2.356]
l_degree_settings_in_use = [-4, 3000, 3500, 4500, 5300]

s_degree_settings = ''
for degree_setting in l_degree_settings_in_use:
    s_degree_settings += '%s,' % (degree_setting)
s_degree_settings = s_degree_settings[:-1]

name_of_results_directory = neriX_simulation_config.results_directory_name + '/%s' % (directory_descriptor)
#l_plots = ['plots']

num_steps = 100
num_steps_to_pull_from = 100

for cathode_setting in l_cathode_settings_in_use:
    d_yields[cathode_setting] = {}
    for degree_setting in l_degree_settings_in_use:
        print 'debugging'
        continue
        if degree_setting == -4:
            continue
    
        d_yields[cathode_setting][degree_setting] = {}

        dir_specifier_name = '%.3f_kV_%d_deg' % (cathode_setting, degree_setting)

        s_path_to_file = './%s/%s/sampler_dictionary.p' % (name_of_results_directory, dir_specifier_name)

        if os.path.exists(s_path_to_file):
            print '\nLoading sampler for %.3f kV, %d deg...' % (cathode_setting, degree_setting)
        
            dSampler = pickle.load(open(s_path_to_file, 'r'))
            l_chains = []
            for sampler in dSampler[num_walkers]:
                l_chains.append(sampler['_chain'])

            aSampler = np.concatenate(l_chains, axis=1)

            print 'Successfully loaded sampler!'
        else:
            print s_path_to_file
            print 'Could not find file!'
            sys.exit()

        num_dim = aSampler.shape[2]
        d_yields[cathode_setting][degree_setting]['a_samples'] = aSampler[:, -num_steps_to_pull_from:, :num_dim].reshape((-1, num_dim))



# load coincidence fit

s_cathode_settings = ''
for cathode_setting in l_cathode_settings_in_use:
    s_cathode_settings += '%.3f,' % (cathode_setting)
s_cathode_settings = s_cathode_settings[:-1]

name_of_results_directory = neriX_simulation_config.results_directory_name

s_path_to_file = './%s/%s/%s_kV_%s_deg/sampler_dictionary.p' % (name_of_results_directory, 'multiple_energies_lindhard_model_with_ti', s_cathode_settings, s_degree_settings)

print '\nLoading sampler for band fit...'
if os.path.exists(s_path_to_file):
    dSampler = pickle.load(open(s_path_to_file, 'r'))
    l_chains = []
    for sampler in dSampler[num_walkers_band]:
        l_chains.append(sampler['_chain'])

    a_full_sampler = np.concatenate(l_chains, axis=1)

    print 'Successfully loaded sampler!'
else:
    print s_path_to_file
    print 'Could not find file!'
    sys.exit()

total_length_sampler = len(a_full_sampler)
num_dim_band = a_full_sampler.shape[2]
a_samples_band = a_full_sampler.reshape(-1, num_dim_band)

num_degree_field_pars = 2*len(l_degree_settings_in_use) + 2 - 1

# markov chain itself is random sample of posterior so use this
# to randomly sample parameter posterior (why do the work twice?)



min_energy = 3 # 2 PE / (0.117 PE/photon) / (5.5 photons / keV)
max_energy = 75#23 # mean + 2 sigma highest energy (17 + 2*3)
num_energies = 50
a_energies = np.linspace(min_energy, max_energy, num_energies)

min_energy_plot = 1
max_energy_plot = 75#30
a_energies_nest = np.linspace(min_energy_plot, max_energy_plot, num_energies)

# gather yields from data


for cathode_setting in l_cathode_settings_in_use:
    
    
    d_yields[cathode_setting]['a_energy_mean_data'] = np.zeros(len(l_degree_settings_in_use))
    d_yields[cathode_setting]['a_energy_width_data'] = np.zeros(len(l_degree_settings_in_use))
    
    d_yields[cathode_setting]['a_py_median_data'] = np.zeros(len(l_degree_settings_in_use))
    d_yields[cathode_setting]['a_py_lb_data'] = np.zeros(len(l_degree_settings_in_use))
    d_yields[cathode_setting]['a_py_ub_data'] = np.zeros(len(l_degree_settings_in_use))
    
    d_yields[cathode_setting]['a_qy_median_data'] = np.zeros(len(l_degree_settings_in_use))
    d_yields[cathode_setting]['a_qy_lb_data'] = np.zeros(len(l_degree_settings_in_use))
    d_yields[cathode_setting]['a_qy_ub_data'] = np.zeros(len(l_degree_settings_in_use))
    
    counter = 0
    
    for degree_setting in l_degree_settings_in_use:
    
        print 'debugging'
        continue
        if degree_setting == -4:
            continue
    

        current_py = np.zeros(num_steps*num_walkers)
        current_qy = np.zeros(num_steps*num_walkers)

        for i in tqdm.tqdm(xrange(num_steps*num_walkers)):
            current_py[i] = d_yields[cathode_setting][degree_setting]['a_samples'][i, 0]
            current_qy[i] = d_yields[cathode_setting][degree_setting]['a_samples'][i, 1]

        py_one_sigma_below, py_median, py_one_sigma_above = np.percentile(current_py, [16, 50, 84])
        d_yields[cathode_setting][degree_setting]['py_median'] = py_median
        d_yields[cathode_setting][degree_setting]['py_lb'] = py_median-py_one_sigma_below
        d_yields[cathode_setting][degree_setting]['py_ub'] = py_one_sigma_above-py_median

        qy_one_sigma_below, qy_median, qy_one_sigma_above = np.percentile(current_qy, [16, 50, 84])
        d_yields[cathode_setting][degree_setting]['qy_median'] = qy_median
        d_yields[cathode_setting][degree_setting]['qy_lb'] = qy_median-qy_one_sigma_below
        d_yields[cathode_setting][degree_setting]['qy_ub'] = qy_one_sigma_above-qy_median

        # fill arrays up
        d_yields[cathode_setting]['a_energy_mean_data'][counter] = d_degree_setting_to_energy_mean[degree_setting]
        d_yields[cathode_setting]['a_energy_width_data'][counter] = d_degree_setting_to_energy_width[degree_setting]
        
        d_yields[cathode_setting]['a_py_median_data'][counter] = d_yields[cathode_setting][degree_setting]['py_median']
        d_yields[cathode_setting]['a_py_lb_data'][counter] = d_yields[cathode_setting][degree_setting]['py_lb']
        d_yields[cathode_setting]['a_py_ub_data'][counter] = d_yields[cathode_setting][degree_setting]['py_ub']
        
        d_yields[cathode_setting]['a_qy_median_data'][counter] = d_yields[cathode_setting][degree_setting]['qy_median']
        d_yields[cathode_setting]['a_qy_lb_data'][counter] = d_yields[cathode_setting][degree_setting]['qy_lb']
        d_yields[cathode_setting]['a_qy_ub_data'][counter] = d_yields[cathode_setting][degree_setting]['qy_ub']


        counter += 1





a_temp_py_nest = np.zeros(num_steps)
a_temp_qy_nest = np.zeros(num_steps)

# get yields from NEST
for cathode_setting in l_cathode_settings_in_use:

    d_yields[cathode_setting]['a_py_median_nest'] = np.zeros(num_energies)
    d_yields[cathode_setting]['a_py_lb_nest'] = np.zeros(num_energies)
    d_yields[cathode_setting]['a_py_ub_nest'] = np.zeros(num_energies)
    
    d_yields[cathode_setting]['a_qy_median_nest'] = np.zeros(num_energies)
    d_yields[cathode_setting]['a_qy_lb_nest'] = np.zeros(num_energies)
    d_yields[cathode_setting]['a_qy_ub_nest'] = np.zeros(num_energies)

    for bin_number, energy in tqdm.tqdm(enumerate(a_energies_nest)):
        for i in xrange(num_steps):
            
            # get NEST values
            w_value_nest = np.random.normal(13.7, 0.4)/1000.
            
            current_rv = np.random.randn()
            if current_rv < 0:
                alpha_nest = 1.240 - current_rv*0.073
            else:
                alpha_nest = 1.240 + current_rv*0.079
            
            current_rv = np.random.randn()
            if current_rv < 0:
                zeta_nest = 0.0472 - current_rv*0.0073
            else:
                zeta_nest = 0.0472 + current_rv*0.0088

            current_rv = np.random.randn()
            if current_rv < 0:
                beta_nest = 239 - current_rv*8.8
            else:
                beta_nest = 239 + current_rv*28

            current_rv = np.random.randn()
            if current_rv < 0:
                gamma_nest = 0.01385 - current_rv*0.00073
            else:
                gamma_nest = 0.01385 + current_rv*0.00058

            current_rv = np.random.randn()
            if current_rv < 0:
                delta_nest = 0.0620 - current_rv*0.0064
            else:
                delta_nest = 0.0620 + current_rv*0.0056

            current_rv = np.random.randn()
            if current_rv < 0:
                kappa_nest = 0.1394 - current_rv*0.0026
            else:
                kappa_nest = 0.1394 + current_rv*0.0032

            current_rv = np.random.randn()
            if current_rv < 0:
                eta_nest = 3.3 - current_rv*0.7
            else:
                eta_nest = 3.3 + current_rv*5.3

            current_rv = np.random.randn()
            if current_rv < 0:
                lamb_nest = 1.14 - current_rv*0.09
            else:
                lamb_nest = 1.14 + current_rv*0.45


            a_temp_py_nest[i], a_temp_qy_nest[i] = get_ly_qy_from_nest_pars(energy, d_cathode_setting_to_mean_field[cathode_setting], w_value_nest, alpha_nest, zeta_nest, beta_nest, gamma_nest, delta_nest, kappa_nest, eta_nest, lamb_nest)



        py_one_sigma_below, py_median, py_one_sigma_above = np.percentile(a_temp_py_nest, [16, 50, 84])
        qy_one_sigma_below, qy_median, qy_one_sigma_above = np.percentile(a_temp_qy_nest, [16, 50, 84])

        d_yields[cathode_setting]['a_py_median_nest'][bin_number] =py_median
        d_yields[cathode_setting]['a_py_lb_nest'][bin_number] = py_one_sigma_below
        d_yields[cathode_setting]['a_py_ub_nest'][bin_number] = py_one_sigma_above
        
        d_yields[cathode_setting]['a_qy_median_nest'][bin_number] = qy_median
        d_yields[cathode_setting]['a_qy_lb_nest'][bin_number] = qy_one_sigma_below
        d_yields[cathode_setting]['a_qy_ub_nest'][bin_number] = qy_one_sigma_above



# get yields from bands
a_temp_py_nest = np.zeros(num_steps*num_walkers_band)
a_temp_qy_nest = np.zeros(num_steps*num_walkers_band)
count_fields = 0
for cathode_setting in l_cathode_settings_in_use:
    d_yields[cathode_setting]['a_py_median_band'] = np.zeros(num_energies)
    d_yields[cathode_setting]['a_py_lb_band'] = np.zeros(num_energies)
    d_yields[cathode_setting]['a_py_ub_band'] = np.zeros(num_energies)
    d_yields[cathode_setting]['a_py_2lb_band'] = np.zeros(num_energies)
    d_yields[cathode_setting]['a_py_2ub_band'] = np.zeros(num_energies)
    
    d_yields[cathode_setting]['a_qy_median_band'] = np.zeros(num_energies)
    d_yields[cathode_setting]['a_qy_lb_band'] = np.zeros(num_energies)
    d_yields[cathode_setting]['a_qy_ub_band'] = np.zeros(num_energies)
    d_yields[cathode_setting]['a_qy_2lb_band'] = np.zeros(num_energies)
    d_yields[cathode_setting]['a_qy_2ub_band'] = np.zeros(num_energies)

    for bin_number, energy in tqdm.tqdm(enumerate(a_energies)):
        for i in xrange(num_steps*num_walkers_band):
            
            #a_current_pars = a_samples_band[-(np.random.randint(1, num_steps_to_pull_from*num_walkers_band)), :]
            a_current_pars = a_samples_band[-i, :]
            
            w_value = a_current_pars[0]/1000.
            beta = a_current_pars[1]
            kappa = a_current_pars[2]
            eta = a_current_pars[3]
            lamb = a_current_pars[4]
            
            
            dimensionless_energy = 11.5*energy*54**(-7./3.)
            g_value = 3*dimensionless_energy**0.15 + 0.7*dimensionless_energy**0.6 + dimensionless_energy
            lindhard_factor = kappa*g_value / (1. + kappa*g_value)
            penning_factor = 1. / (1. + eta*dimensionless_energy**lamb)
            
            num_quanta = energy*lindhard_factor/w_value
            
            exciton_to_ion_ratio = a_current_pars[22+count_fields*num_degree_field_pars]*(1-np.exp(-beta*dimensionless_energy))
            
            num_excitons = num_quanta*(1. - 1./(1. + exciton_to_ion_ratio))
            num_ions = num_quanta - num_excitons
            
            #recombination_prob = a_current_pars[17+num_degree_field_pars*count_fields] + a_current_pars[18+num_degree_field_pars*count_fields]*np.exp(-a_current_pars[19+num_degree_field_pars*count_fields]*np.exp(-a_current_pars[20+num_degree_field_pars*count_fields]*num_ions))
            recombination_prob = 1. - np.log(1 + num_ions*a_current_pars[21+num_degree_field_pars*count_fields])/(num_ions*a_current_pars[21+num_degree_field_pars*count_fields])
            
            #print i, exciton_to_ion_ratio, a_current_pars[17+num_degree_field_pars*count_fields], recombination_prob
            
            num_recombined = num_ions*recombination_prob
            num_excitons += num_recombined
            num_ions -= num_recombined

            num_excitons *= penning_factor

            a_temp_py_nest[i], a_temp_qy_nest[i] = num_excitons/energy, num_ions/energy



        py_two_sigma_below, py_one_sigma_below, py_median, py_one_sigma_above, py_two_sigma_above = np.percentile(a_temp_py_nest, [2.5, 16, 50, 84, 97.5])
        qy_two_sigma_below, qy_one_sigma_below, qy_median, qy_one_sigma_above, qy_two_sigma_above = np.percentile(a_temp_qy_nest, [2.5, 16, 50, 84, 97.5])

        d_yields[cathode_setting]['a_py_median_band'][bin_number] =py_median
        d_yields[cathode_setting]['a_py_lb_band'][bin_number] = py_one_sigma_below
        d_yields[cathode_setting]['a_py_ub_band'][bin_number] = py_one_sigma_above
        d_yields[cathode_setting]['a_py_2lb_band'][bin_number] = py_two_sigma_below
        d_yields[cathode_setting]['a_py_2ub_band'][bin_number] = py_two_sigma_above
        
        d_yields[cathode_setting]['a_qy_median_band'][bin_number] = qy_median
        d_yields[cathode_setting]['a_qy_lb_band'][bin_number] = qy_one_sigma_below
        d_yields[cathode_setting]['a_qy_ub_band'][bin_number] = qy_one_sigma_above
        d_yields[cathode_setting]['a_qy_2lb_band'][bin_number] = qy_two_sigma_below
        d_yields[cathode_setting]['a_qy_2ub_band'][bin_number] = qy_two_sigma_above


    count_fields += 1



# --------------------------------------------
# --------------------------------------------
#  Points from Plante et al
# --------------------------------------------
# --------------------------------------------


def multiply_leff_array_by_co57_ly(l_leff):
    for i in xrange(len(l_leff)):
        l_leff[i] *= 63.
    return l_leff

l_plante_energies = [3, 3.9, 5.0, 6.5, 8.4, 10.7, 14.8, 55.2]
l_plante_energies_unc = [0.6, 0.7, 0.8, 1.0, 1.3, 1.6, 1.3, 8.8]
l_plante_ly = [0.088, 0.095, 0.098, 0.121, 0.139, 0.143, 0.144, 0.268]
l_plante_ly_unc = [[0.015, 0.016, 0.015, 0.010, 0.011, 0.010, 0.009, 0.013], [0.014, 0.015, 0.014, 0.010, 0.011, 0.010, 0.009, 0.013]]

l_plante_ly = multiply_leff_array_by_co57_ly(l_plante_ly)
l_plante_ly_unc[0] = multiply_leff_array_by_co57_ly(l_plante_ly_unc[0])
l_plante_ly_unc[1] = multiply_leff_array_by_co57_ly(l_plante_ly_unc[1])



# --------------------------------------------
# --------------------------------------------
#  Points from Lux
# --------------------------------------------
# --------------------------------------------

l_lux_energies_qy = [0.70, 1.10, 1.47, 2.00, 2.77, 3.86, 5.55, 8.02, 11.52, 16.56, 24.2]
l_lux_energies_qy_unc = [0.13, 0.18, 0.12, 0.10, 0.10, 0.08, 0.09, 0.10, 0.12, 0.16, 0.2]
l_lux_qy = [8.2, 7.4, 10.1, 8.0, 7.5, 7.3, 7.2, 6.8, 5.88, 5.28, 4.62]
l_lux_qy_unc = [[2.1, 1.7, 1.6, 0.6, 0.5, 0.3, 0.2, 0.17, 0.13, 0.13, 0.10], [2.4, 1.9, 1.5, 0.9, 0.5, 0.3, 0.2, 0.15, 0.12, 0.11, 0.13]]

l_lux_energies_py = [1.08, 1.92, 3.13, 4.45, 5.89, 7.44, 9.1, 10.9, 12.8]
l_lux_energies_py_unc = [0.13, 0.09, 0.11, 0.11, 0.13, 0.17, 0.2, 0.3, 0.3]
l_lux_py = [4.9, 5.2, 4.9, 6.4, 6.1, 7.4, 7.9, 8.1, 8.9]
l_lux_py_unc = [[1.0, 0.4, 0.4, 0.4, 0.3, 0.4, 0.4, 0.5, 0.4], [1.2, 0.6, 0.5, 0.4, 0.4, 0.4, 0.4, 0.4, 0.6]]




# set colors for different cathode settings
for cathode_setting in l_cathode_settings_in_use:
    if cathode_setting == 0.345:
        d_yields[cathode_setting]['color'] = 'b'
    elif cathode_setting == 1.054:
        d_yields[cathode_setting]['color'] = 'g'
    elif cathode_setting == 2.356:
        d_yields[cathode_setting]['color'] = 'r'


transparency_one_sigma = 0.2
transparency_two_sigma = 0.1



f, (ax1, ax2) = plt.subplots(2)

ax1.set_xlim(min_energy_plot, max_energy_plot)
ax1.set_ylim(3.5, 16)
ax1.set_xscale('log', nonposx='clip')
ax1.set_yscale('log', nonposx='clip')
ax1.set_xlabel('Energy [keV]')
ax1.set_ylabel(r'$L_{y} [\frac{photons}{keV}]$')

for cathode_setting in l_cathode_settings_in_use:

    print 'debugging'
    #ax1.errorbar(d_yields[cathode_setting]['a_energy_mean_data'], d_yields[cathode_setting]['a_py_median_data'], xerr=d_yields[cathode_setting]['a_energy_width_data'], yerr=[d_yields[cathode_setting]['a_py_lb_data'], d_yields[cathode_setting]['a_py_ub_data']], fmt='%s.' % (d_yields[cathode_setting]['color']))

    ax1.plot(a_energies_nest, d_yields[cathode_setting]['a_py_median_nest'], marker='', linestyle='-.', color=d_yields[cathode_setting]['color'])
    #ax1.fill_between(a_energies, d_yields[cathode_setting]['a_py_lb_nest'], d_yields[cathode_setting]['a_py_ub_nest'], facecolor=d_yields[cathode_setting]['color'], alpha=0.1, interpolate=True)

    ax1.plot(a_energies, d_yields[cathode_setting]['a_py_median_band'], marker='', linestyle='--', color=d_yields[cathode_setting]['color'])
    ax1.fill_between(a_energies, d_yields[cathode_setting]['a_py_lb_band'], d_yields[cathode_setting]['a_py_ub_band'], facecolor=d_yields[cathode_setting]['color'], alpha=transparency_one_sigma, interpolate=True)
    ax1.fill_between(a_energies, d_yields[cathode_setting]['a_py_2lb_band'], d_yields[cathode_setting]['a_py_2ub_band'], facecolor=d_yields[cathode_setting]['color'], alpha=transparency_two_sigma, interpolate=True)

ax1.errorbar(l_plante_energies, l_plante_ly, xerr=l_plante_energies_unc, yerr=l_plante_ly_unc, marker='.', color='black', linestyle='')
ax1.errorbar(l_lux_energies_py, l_lux_py, xerr=l_lux_energies_py_unc, yerr=l_lux_py_unc, fmt='c.')




ax2.set_xlim(min_energy_plot, max_energy_plot)
ax2.set_ylim(1, 14)
ax2.set_xscale('log', nonposx='clip')
ax2.set_yscale('log', nonposx='clip')
ax2.set_xlabel('Energy [keV]')
ax2.set_ylabel(r'$Q_{y} [\frac{electrons}{keV}]$')

for cathode_setting in l_cathode_settings_in_use:

    print 'debugging'
    #ax2.errorbar(d_yields[cathode_setting]['a_energy_mean_data'], d_yields[cathode_setting]['a_qy_median_data'], xerr=d_yields[cathode_setting]['a_energy_width_data'], yerr=[d_yields[cathode_setting]['a_qy_lb_data'], d_yields[cathode_setting]['a_qy_ub_data']], fmt='%s.' % (d_yields[cathode_setting]['color']))

    # NEST
    ax2.plot(a_energies_nest, d_yields[cathode_setting]['a_qy_median_nest'], marker='', linestyle='-.', color=d_yields[cathode_setting]['color'])
    #ax2.fill_between(a_energies, d_yields[cathode_setting]['a_qy_lb_nest'], d_yields[cathode_setting]['a_qy_ub_nest'], facecolor=d_yields[cathode_setting]['color'], alpha=0.1, interpolate=True)
    #ax2.fill_between(a_energies, d_yields[cathode_setting]['a_qy_lb_nest'], d_yields[cathode_setting]['a_qy_ub_nest'], color=None, edgecolor=d_yields[cathode_setting]['color'], hatch='x')

    # band
    ax2.plot(a_energies, d_yields[cathode_setting]['a_qy_median_band'], marker='', linestyle='--', color=d_yields[cathode_setting]['color'])
    ax2.fill_between(a_energies, d_yields[cathode_setting]['a_qy_lb_band'], d_yields[cathode_setting]['a_qy_ub_band'], facecolor=d_yields[cathode_setting]['color'], alpha=transparency_one_sigma, interpolate=True)
    ax2.fill_between(a_energies, d_yields[cathode_setting]['a_qy_2lb_band'], d_yields[cathode_setting]['a_qy_2ub_band'], facecolor=d_yields[cathode_setting]['color'], alpha=transparency_two_sigma, interpolate=True)


ax2.errorbar(l_lux_energies_qy, l_lux_qy, xerr=l_lux_energies_qy_unc, yerr=l_lux_qy_unc, fmt='c.')



f.savefig('./plots/s_ly_qy_comparison.png')



plt.show()






