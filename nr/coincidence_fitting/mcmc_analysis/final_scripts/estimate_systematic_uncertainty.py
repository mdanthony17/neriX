#!/usr/bin/python
import sys, array, os
sys.path.insert(0, '../..')

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

from scipy.interpolate import spline


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



if len(sys.argv) != 2:
    print 'Use is python perform_full_matching.py <num walkers>'
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

num_walkers_band = int(sys.argv[1])

l_cathode_settings_in_use = [0.345, 1.054, 2.356]
l_degree_settings_in_use = [-4, 3000, 3500, 4500, 5300]

s_degree_settings = ''
for degree_setting in l_degree_settings_in_use:
    s_degree_settings += '%s,' % (degree_setting)
s_degree_settings = s_degree_settings[:-1]


num_steps = 400
num_steps_to_pull_from = 400




# load coincidence fit

s_cathode_settings = ''
for cathode_setting in l_cathode_settings_in_use:
    s_cathode_settings += '%.3f,' % (cathode_setting)
s_cathode_settings = s_cathode_settings[:-1]

name_of_results_directory = neriX_simulation_config.results_directory_name

#s_path_to_file = './%s/%s/%s_kV_%s_deg/sampler_dictionary.p' % (name_of_results_directory, 'multiple_energies_lindhard_model_with_ti', s_cathode_settings, s_degree_settings)
#b_bq_fixed = False


d_yields['norm'] = {}
d_yields['moved'] = {}

b_moved_mode = False
b_binning_mode = True

if b_moved_mode:
    d_yields['norm']['path'] = './%s/%s/%s_kV_%s_deg/sampler_dictionary.p' % (name_of_results_directory, 'multiple_energies_lindhard_model_with_ti', s_cathode_settings, s_degree_settings)
    d_yields['moved']['path'] = './%s/%s/%s_kV_%s_deg/sampler_dictionary.p' % (name_of_results_directory, 'multiple_energies_lindhard_model_with_ti_moved_pos', s_cathode_settings, s_degree_settings)
if b_binning_mode:
    d_yields['norm']['path'] = './%s/%s/%s_kV_%s_deg/sampler_dictionary.p' % (name_of_results_directory, 'multiple_energies_lindhard_model_with_ti', s_cathode_settings, s_degree_settings)
    d_yields['moved']['path'] = './%s/%s/%s_kV_%s_deg/sampler_dictionary_lin_final.p' % (name_of_results_directory, 'multiple_energies_lindhard_model_with_ti', s_cathode_settings, s_degree_settings)


for type in d_yields:
    print '\nLoading sampler for %s fit...' % (type)
    if os.path.exists(d_yields[type]['path']):
        dSampler = pickle.load(open(d_yields[type]['path'], 'r'))
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

    d_yields[type]['a_sampler'] = a_samples_band

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

for cathode_setting in l_cathode_settings_in_use:
    for type in d_yields:
        d_yields[type][cathode_setting] = {}


for type in d_yields:
    # get yields from bands
    a_temp_py_nest = np.zeros(num_steps*num_walkers_band)
    a_temp_qy_nest = np.zeros(num_steps*num_walkers_band)
    count_fields = 0
    for cathode_setting in l_cathode_settings_in_use:
        d_yields[type][cathode_setting]['energies'] = a_energies

        d_yields[type][cathode_setting]['a_py_median_band'] = np.zeros(num_energies)
        d_yields[type][cathode_setting]['a_py_lb_band'] = np.zeros(num_energies)
        d_yields[type][cathode_setting]['a_py_ub_band'] = np.zeros(num_energies)
        d_yields[type][cathode_setting]['a_py_2lb_band'] = np.zeros(num_energies)
        d_yields[type][cathode_setting]['a_py_2ub_band'] = np.zeros(num_energies)
        
        d_yields[type][cathode_setting]['a_qy_median_band'] = np.zeros(num_energies)
        d_yields[type][cathode_setting]['a_qy_lb_band'] = np.zeros(num_energies)
        d_yields[type][cathode_setting]['a_qy_ub_band'] = np.zeros(num_energies)
        d_yields[type][cathode_setting]['a_qy_2lb_band'] = np.zeros(num_energies)
        d_yields[type][cathode_setting]['a_qy_2ub_band'] = np.zeros(num_energies)

        for bin_number, energy in tqdm.tqdm(enumerate(a_energies)):
            for i in xrange(num_steps*num_walkers_band):
                
                #a_current_pars = a_samples_band[-(np.random.randint(1, num_steps_to_pull_from*num_walkers_band)), :]
                a_current_pars = d_yields[type]['a_sampler'][-i, :]
                
                w_value = a_current_pars[0]/1000.
                beta = a_current_pars[1]
                kappa = a_current_pars[2]
                eta = a_current_pars[3]
                
                #if b_bq_fixed:
                #    lamb = 0.5
                #else:
                #    lamb = a_current_pars[4]
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

            d_yields[type][cathode_setting]['a_py_median_band'][bin_number] =py_median
            d_yields[type][cathode_setting]['a_py_lb_band'][bin_number] = py_one_sigma_below
            d_yields[type][cathode_setting]['a_py_ub_band'][bin_number] = py_one_sigma_above
            d_yields[type][cathode_setting]['a_py_2lb_band'][bin_number] = py_two_sigma_below
            d_yields[type][cathode_setting]['a_py_2ub_band'][bin_number] = py_two_sigma_above
            
            d_yields[type][cathode_setting]['a_qy_median_band'][bin_number] = qy_median
            d_yields[type][cathode_setting]['a_qy_lb_band'][bin_number] = qy_one_sigma_below
            d_yields[type][cathode_setting]['a_qy_ub_band'][bin_number] = qy_one_sigma_above
            d_yields[type][cathode_setting]['a_qy_2lb_band'][bin_number] = qy_two_sigma_below
            d_yields[type][cathode_setting]['a_qy_2ub_band'][bin_number] = qy_two_sigma_above


        count_fields += 1



for cathode_setting in l_cathode_settings_in_use:

    a_squared_diff_median = (d_yields['norm'][cathode_setting]['a_py_median_band'] - d_yields['moved'][cathode_setting]['a_py_median_band'])**2
    a_avg_uncertainty = (d_yields[type][cathode_setting]['a_py_ub_band'] - d_yields[type][cathode_setting]['a_py_lb_band']) / 2.
    #print d_yields[type][cathode_setting]['a_py_ub_band'] - d_yields[type][cathode_setting]['a_py_lb_band']
    #print d_yields[type][cathode_setting]['a_py_ub_band']
    #print d_yields[type][cathode_setting]['a_py_lb_band']

    print cathode_setting
    #print a_squared_diff_median**0.5
    print 'Average systematic difference over statistical uncertainy for light yield'
    print np.sum(a_squared_diff_median**0.5 / a_avg_uncertainty) / len(a_avg_uncertainty)
    print 'Average systematic difference over median light yield'
    print np.sum(a_squared_diff_median**0.5 / d_yields['norm'][cathode_setting]['a_py_median_band']) / len(a_avg_uncertainty)


    a_squared_diff_median = (d_yields['norm'][cathode_setting]['a_qy_median_band'] - d_yields['moved'][cathode_setting]['a_qy_median_band'])**2
    a_avg_uncertainty = (d_yields[type][cathode_setting]['a_qy_ub_band'] - d_yields[type][cathode_setting]['a_qy_lb_band']) / 2.

    print cathode_setting
    #print a_squared_diff_median**0.5
    print 'Average systematic difference over statistical uncertainy for charge yield'
    print np.sum(a_squared_diff_median**0.5 / a_avg_uncertainty) / len(a_avg_uncertainty)
    print 'Average systematic difference over median charge yield'
    print np.sum(a_squared_diff_median**0.5 / d_yields['norm'][cathode_setting]['a_qy_median_band']) / len(a_avg_uncertainty)










"""
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
#l_plante_ly_unc = [[0.015, 0.016, 0.015, 0.010, 0.011, 0.010, 0.009, 0.013], [0.014, 0.015, 0.014, 0.010, 0.011, 0.010, 0.009, 0.013]]
l_plante_ly_unc = [[0.015, 0.016, 0.015, 0.010, 0.011, 0.010, 0.009, 0.013], [0.115-0.088, 0.124-0.095, 0.124-0.098, 0.134-0.121, 0.150-0.139, 0.154-0.143, 0.155-0.144, 0.281-0.268]] # take from pg. 218 of Guillaume's thesis

l_plante_ly = multiply_leff_array_by_co57_ly(l_plante_ly)
l_plante_ly_unc[0] = multiply_leff_array_by_co57_ly(l_plante_ly_unc[0])
l_plante_ly_unc[1] = multiply_leff_array_by_co57_ly(l_plante_ly_unc[1])


# --------------------------------------------
# --------------------------------------------
#  Points from Aprile 09
# --------------------------------------------
# --------------------------------------------

l_aprile_09_energies = [5, 8, 10, 22.94]
l_aprile_09_energies_unc = [0.68, 0.91, 1.06, 4.34]
l_aprile_09_ly = [0.141, 0.137, 0.140, 0.205]
l_aprile_09_ly_unc = [[0.037, 0.016, 0.016, 0.039], [0.025, 0.016, 0.016, 0.039]]

l_aprile_09_ly = multiply_leff_array_by_co57_ly(l_aprile_09_ly)
l_aprile_09_ly_unc[0] = multiply_leff_array_by_co57_ly(l_aprile_09_ly_unc[0])
l_aprile_09_ly_unc[1] = multiply_leff_array_by_co57_ly(l_aprile_09_ly_unc[1])



# --------------------------------------------
# --------------------------------------------
#  Points from Aprile 06
# --------------------------------------------
# --------------------------------------------

l_aprile_06_energies = [24.85, 34.75, 44.62, 54.79, 64.81, 74.32]
l_aprile_06_energies_unc = [0, 0, 0, 0, 0, 0]
l_aprile_06_qy = [4.72, 4.02, 3.64, 3.38, 3.05, 2.88]
l_aprile_06_qy_unc_low = np.asarray([4.39, 3.73, 3.33, 3.07, 2.79, 2.62])
l_aprile_06_qy_unc_high = np.asarray([5.08, 4.35, 3.95, 3.66, 3.30, 3.133])

l_aprile_06_qy_unc_low = l_aprile_06_qy - l_aprile_06_qy_unc_low
l_aprile_06_qy_unc_high = l_aprile_06_qy_unc_high - l_aprile_06_qy



# --------------------------------------------
# --------------------------------------------
#  Points from Aprile 05
# --------------------------------------------
# --------------------------------------------

l_aprile_05_energies = [10.4, 15.6, 25.6, 46.8, 53.2, 56.5]
l_aprile_05_energies_unc = [0, 0, 0, 0, 0, 0]
l_aprile_05_ly = [0.130, 0.163, 0.167, 0.238, 0.240, 0.227]
l_aprile_05_ly_unc = [[0.024, 0.023, 0.021, 0.030, 0.019, 0.016], [0.024, 0.023, 0.021, 0.030, 0.019, 0.016]]

l_aprile_05_ly = multiply_leff_array_by_co57_ly(l_aprile_05_ly)
l_aprile_05_ly_unc[0] = multiply_leff_array_by_co57_ly(l_aprile_05_ly_unc[0])
l_aprile_05_ly_unc[1] = multiply_leff_array_by_co57_ly(l_aprile_05_ly_unc[1])



# --------------------------------------------
# --------------------------------------------
#  Points from Sorenson 09
# --------------------------------------------
# --------------------------------------------

l_sorenson_ly_energies = [2, 5, 10, 15, 25, 50, 100]
l_sorenson_ly = np.asarray([0.160, 0.156, 0.162, 0.194, 0.220, 0.237, 0.274])
l_sorenson_ly_unc = np.asarray([(0.014**2+0.012**2+0.009**2)**0.5, (0.011**2+0.006**2+0.001**2)**0.5, (0.012**2+0.005**2+0.002**2)**0.5, (0.011**2+0.006**2+0.005**2)**0.5, (0.012**2+0.005**2+0.001**2)**0.5, (0.009**2+0.004**2+0.001**2)**0.5, (0.010**2+0.004**2+0.005**2)**0.5])
l_sorenson_ly_lb = multiply_leff_array_by_co57_ly(l_sorenson_ly - l_sorenson_ly_unc)
l_sorenson_ly_ub = multiply_leff_array_by_co57_ly(l_sorenson_ly + l_sorenson_ly_unc)


l_sorenson_qy_energies = [5.61, 9.56, 14.21, 18.35, 24.84, 29.49, 37.96, 54.94, 67.04]
l_sorenson_qy = [6.17, 5.52, 5.28, 5.30, 4.75, 4.36, 3.84, 2.90, 2.49]
l_sorenson_qy_unc_lb = np.asarray([5.96, 5.33, 5.06, 5.04, 4.39, 3.80, 3.37, 2.26, 1.99])
l_sorenson_qy_unc_ub = np.asarray([6.38, 5.75, 5.50, 5.54, 5.19, 4.92, 4.30, 3.63, 2.97])

l_sorenson_qy_unc_lb = l_sorenson_qy - l_sorenson_qy_unc_lb
l_sorenson_qy_unc_ub = l_sorenson_qy_unc_ub - l_sorenson_qy



# --------------------------------------------
# --------------------------------------------
#  Points from Manzur 0 field
# --------------------------------------------
# --------------------------------------------


l_manzur_0_energies = [3.9, 4.9, 5.7, 6.4, 7.6, 9.8, 15.1, 19.6, 25.7, 34.9, 46.1, 57.7, 66.7]
l_manzur_0_energies_unc = [0.9, 0.9, 1.1, 1.1, 1.2, 1.3, 1.5, 2.6, 2.0, 2.1, 4.9, 3.2, 3.3]
l_manzur_0_ly = [0.073, 0.088, 0.077, 0.094, 0.105, 0.118, 0.123, 0.157, 0.132, 0.133, 0.158, 0.182, 0.178]
l_manzur_0_ly_unc = [[0.025, 0.023, 0.022, 0.022, 0.022, 0.022, 0.023, 0.036, 0.019, 0.029, 0.039, 0.009, 0.016], [0.034, 0.026, 0.028, 0.027, 0.028, 0.027, 0.030, 0.046, 0.025, 0.042, 0.038, 0.009, 0.018]]
l_manzur_0_ly_unc_avg = (np.asarray(l_manzur_0_ly_unc[0]) + np.asarray(l_manzur_0_ly_unc[1])) / 2.

l_manzur_0_ly = multiply_leff_array_by_co57_ly(l_manzur_0_ly)
l_manzur_0_ly_unc[0] = multiply_leff_array_by_co57_ly(l_manzur_0_ly_unc[0])
l_manzur_0_ly_unc[1] = multiply_leff_array_by_co57_ly(l_manzur_0_ly_unc[1])



# --------------------------------------------
# --------------------------------------------
#  Points from Zepplin FSR
# --------------------------------------------
# --------------------------------------------


l_zepplin_fsr_ly_lb_x = [4, 4.512, 5.079, 5.746, 6.579, 7.375, 8.272, 9.734, 11.644, 13.131, 14.471, 16.105, 17.466, 19.406, 21.730, 24.298, 27.491, 31.104, 35.202, 39.480, 44.927, 51.152, 58.108, 67.434, 76.633, 80]
l_zepplin_fsr_ly_lb_y = [0.094, 0.098, 0.102, 0.105, 0.109, 0.110, 0.110, 0.111, 0.115, 0.122, 0.129, 0.136, 0.142, 0.148, 0.154, 0.160, 0.166, 0.170, 0.173, 0.176, 0.178, 0.180, 0.179, 0.179, 0.177, 0.176]
l_zepplin_fsr_ly_lb_y = multiply_leff_array_by_co57_ly(l_zepplin_fsr_ly_lb_y)

l_zepplin_fsr_ly_ub_x = [4, 4.623, 5.238, 5.915, 6.845, 7.752, 8.808, 9.716, 11.155, 12.496, 13.903, 15.426, 16.999, 18.987, 21.238, 23.837, 27.607,31.394, 35.179, 39.422, 44.717, 50.368, 56.540, 61.923, 69.891, 80]
l_zepplin_fsr_ly_ub_y = [0.127, 0.127, 0.125, 0.125, 0.125, 0.127, 0.132, 0.136, 0.143, 0.147, 0.152, 0.157, 0.162, 0.168, 0.174, 0.180, 0.185, 0.191, 0.195, 0.198, 0.200, 0.202, 0.204, 0.207, 0.210, 0.212]
l_zepplin_fsr_ly_ub_y = multiply_leff_array_by_co57_ly(l_zepplin_fsr_ly_ub_y)



l_zepplin_fsr_qy_lb_x = [4, 4.573, 5.185, 5.884, 6.930, 8.008, 9.369, 10.480, 12.013, 13.631, 16.023, 18.146, 20.842, 23.527, 26.232, 29.500, 33.449, 37.704, 42.842, 48.034, 53.753, 61.701, 71.325, 80]
l_zepplin_fsr_qy_lb_y = [4.868, 5.032, 5.115, 5.199, 5.254, 5.273, 5.242, 5.180, 5.108, 5.025, 4.901, 4.774, 4.517, 4.392, 4.232, 4.072, 3.877, 3.715, 3.556, 3.395, 3.256, 3.067, 2.904, 2.763]

l_zepplin_fsr_qy_ub_x = [4, 4.663, 5.290, 6.057, 6.911, 7.941, 9.178, 10.850, 12.691, 14.800, 17.103, 20.037, 23.861, 28.653, 31.540, 35.223, 39.601, 44.867, 50.655, 57.649, 65.121, 73.999, 80]
l_zepplin_fsr_qy_ub_y = [7.361, 7.209, 7.084, 6.896, 6.742, 6.555, 6.361, 6.081, 5.860, 5.661, 5.481, 5.245, 5.014, 4.730, 4.575, 4.404, 4.211, 4.013, 3.815, 3.630, 3.450, 3.269, 3.180]


l_zepplin_fsr_energies = np.logspace(np.log10(4), np.log10(80), 100)

l_zepplin_fsr_ly_lb = spline(l_zepplin_fsr_ly_lb_x, l_zepplin_fsr_ly_lb_y, l_zepplin_fsr_energies)
l_zepplin_fsr_ly_ub = spline(l_zepplin_fsr_ly_ub_x, l_zepplin_fsr_ly_ub_y, l_zepplin_fsr_energies)

l_zepplin_fsr_qy_lb = spline(l_zepplin_fsr_qy_lb_x, l_zepplin_fsr_qy_lb_y, l_zepplin_fsr_energies)
l_zepplin_fsr_qy_ub = spline(l_zepplin_fsr_qy_ub_x, l_zepplin_fsr_qy_ub_y, l_zepplin_fsr_energies)



# --------------------------------------------
# --------------------------------------------
#  Points from Zepplin SSR
# --------------------------------------------
# --------------------------------------------


l_zepplin_ssr_ly_lb_x = [4, 4.424, 4.895, 5.430, 6.128, 6.942, 7.666, 8.431, 9.409, 10.381, 11.651, 12.874, 14.422, 15.606, 16.982, 18.298, 19.898, 22.190, 24.661, 28.520, 32.246, 36.407, 41.051, 45.883, 52.391, 59.579, 65.670, 76.019, 80]
l_zepplin_ssr_ly_lb_y = [0.060, 0.064, 0.069, 0.077, 0.087, 0.095, 0.101, 0.107, 0.113, 0.119, 0.125, 0.131, 0.137, 0.141, 0.145, 0.147, 0.150, 0.155, 0.158, 0.164, 0.168, 0.171, 0.176, 0.178, 0.180, 0.183, 0.184, 0.184, 0.184]
l_zepplin_ssr_ly_lb_y = multiply_leff_array_by_co57_ly(l_zepplin_ssr_ly_lb_y)


l_zepplin_ssr_ly_ub_x = [4, 4.666, 5.356, 6.164, 6.850, 7.738, 8.538, 9.365, 10.349, 11.345, 12.793, 14.568, 16.232, 18.513, 21.532, 24.905, 27.173, 30.273, 33.756, 37.661, 42.196, 47.468, 53.160, 59.173, 67.515, 73.327, 80]
l_zepplin_ssr_ly_ub_y = [0.091, 0.096, 0.101, 0.110, 0.116, 0.123, 0.129, 0.134, 0.139, 0.143, 0.148, 0.154, 0.159, 0.165, 0.171, 0.180, 0.184, 0.190, 0.195, 0.200, 0.205, 0.208, 0.213, 0.217, 0.224, 0.228, 0.231]
l_zepplin_ssr_ly_ub_y = multiply_leff_array_by_co57_ly(l_zepplin_ssr_ly_ub_y)



l_zepplin_ssr_qy_lb_x = [4, 4.465, 5.357, 6.088, 6.800, 7.687, 8.898, 10.219, 11.732, 13.183, 14.587, 16.054, 18.409, 20.021, 22.492, 25.925, 29.638, 33.023, 37.248, 40.729, 45.107, 49.166, 54.122, 59.473, 64.768, 71.338, 80]
l_zepplin_ssr_qy_lb_y = [4, 4.116, 4.066, 4.048, 4.056, 4.089, 4.167, 4.282, 4.387, 4.426, 4.455, 4.455, 4.421, 4.387, 4.318, 4.175, 4.049, 3.882, 3.757, 3.640, 3.527, 3.411, 3.313, 3.200, 3.103, 3.007, 2.893]

l_zepplin_ssr_qy_ub_x = [4, 4.497, 5.064, 5.691, 6.428, 7.054, 7.645, 8.342, 9.150, 10.099, 11.222, 12.240, 13.508, 14.839, 16.441, 18.746, 23.041, 25.540, 28.197, 31.597, 35.805, 38.930, 42.670, 45.931, 48.543, 53.088, 58.730, 63.886, 68.171, 72.855, 77.083, 80]
l_zepplin_ssr_qy_ub_y = [6.282, 5.958, 5.689, 5.452, 5.286, 5.187, 5.118, 5.080, 5.101, 5.099, 5.099, 5.099, 5.114, 5.138, 5.137, 5.079, 4.946, 4.818, 4.716, 4.559, 4.389, 4.250, 4.119, 4.033, 3.953, 3.804, 3.689, 3.572, 3.496, 3.442, 3.353, 3.312]


l_zepplin_ssr_energies = np.logspace(np.log10(4), np.log10(80), 100)

l_zepplin_ssr_ly_lb = spline(l_zepplin_ssr_ly_lb_x, l_zepplin_ssr_ly_lb_y, l_zepplin_ssr_energies)
l_zepplin_ssr_ly_ub = spline(l_zepplin_ssr_ly_ub_x, l_zepplin_ssr_ly_ub_y, l_zepplin_ssr_energies)

l_zepplin_ssr_qy_lb = spline(l_zepplin_ssr_qy_lb_x, l_zepplin_ssr_qy_lb_y, l_zepplin_ssr_energies)
l_zepplin_ssr_qy_ub = spline(l_zepplin_ssr_qy_ub_x, l_zepplin_ssr_qy_ub_y, l_zepplin_ssr_energies)




# --------------------------------------------
# --------------------------------------------
#  Points from Manzur 730 V/cm
# --------------------------------------------
# --------------------------------------------

l_manzur_730_energies_qy = np.asarray([3.98, 4.92, 6.46, 7.70, 15.26, 25.92, 57.97, 66.17])
l_manzur_730_energies_qy_unc_low = np.asarray([3.02, 3.99, 5.26, 6.34, 13.55, 23.69, 54.45, 62.27])
l_manzur_730_energies_qy_unc_high = np.asarray([4.98, 5.90, 7.71, 9.09, 16.82, 27.70, 60.59, 69.41])

l_manzur_730_energies_qy_unc_low = l_manzur_730_energies_qy - l_manzur_730_energies_qy_unc_low
l_manzur_730_energies_qy_unc_high = l_manzur_730_energies_qy_unc_high - l_manzur_730_energies_qy

l_manzur_730_qy = [8.81, 7.16, 6.27, 5.18, 4.52, 4.14, 3.28, 3.02]
l_manzur_730_qy_unc_low = np.asarray([7.32, 5.79, 5.16, 4.34, 3.80, 3.35, 2.85, 2.61])
l_manzur_730_qy_unc_high = np.asarray([10.52, 8.98, 7.66, 6.20, 5.38, 4.88, 3.80, 3.53])

l_manzur_730_qy_unc_low = l_manzur_730_qy - l_manzur_730_qy_unc_low
l_manzur_730_qy_unc_high = l_manzur_730_qy_unc_high - l_manzur_730_qy


l_manzur_730_energies_ly = l_manzur_0_energies
l_manzur_730_energies_ly_unc = l_manzur_0_energies_unc
l_manzur_sn = np.asarray([1.19, 1.16, 1.35, 0.92, 0.79, 0.91, 0.83, 0.70, 0.95, 1.33, ])


l_manzur_730_ly = multiply_leff_array_by_co57_ly(l_manzur_730_ly)
l_manzur_730_ly_unc[0] = multiply_leff_array_by_co57_ly(l_manzur_730_ly_unc[0])
l_manzur_730_ly_unc[1] = multiply_leff_array_by_co57_ly(l_manzur_730_ly_unc[1])



# --------------------------------------------
# --------------------------------------------
#  Points from Weber
# --------------------------------------------
# --------------------------------------------


l_weber_ub_x = [3.0, 3.450, 3.910, 4.431, 5.119, 5.666, 6.365, 6.995, 7.945, 8.989, 9.967, 11.222, 12.685, 14.287, 16.275, 18.590, 21.293, 24.136, 27.661, 30.935, 34.202, 38.238, 42.945, 48.967, 55.865, 62.531, 71.559, 85.029, 96.655]
l_weber_ub_y = [7.759, 7.683, 7.596, 7.478, 7.349, 7.263, 7.133, 7.022, 6.850, 6.688, 6.525, 6.301, 6.091, 5.914, 5.754, 5.583, 5.416, 5.215, 4.983, 4.758, 4.542, 4.293, 3.988, 3.675, 3.419, 3.202, 2.994, 2.714, 2.551]


l_weber_median_x = [3.0, 3.490, 4.039, 4.620, 5.301, 6.090, 7.024, 8.055, 9.202, 10.539, 11.996, 13.721, 15.579, 17.822, 20.281, 22.958, 25.954, 29.446, 33.290, 37.667, 42.721, 48.788, 55.659, 62.564, 71.739, 84.790, 96.861]
l_weber_median_y = [6.537, 6.538, 6.489, 6.462, 6.438, 6.374, 6.295, 6.217, 6.049, 5.918, 5.733, 5.568, 5.418, 5.311, 5.175, 5.034, 4.869, 4.656, 4.456, 4.219, 3.945, 3.653, 3.386, 3.173, 2.948, 2.651, 2.510]


l_weber_lb_x = [3.0, 3.455, 3.970, 4.502, 5.155, 5.929, 6.800, 7.772, 8.909, 10.163, 11.774, 13.642, 15.607, 17.861, 20.243, 23.047, 26.040, 29.365, 33.191, 36.961, 41.408, 46.352, 52.916, 59.170, 65.878, 74.233, 82.445, 89.920, 98.632]
l_weber_lb_y = [5.454, 5.518, 5.567, 5.637, 5.701, 5.746, 5.736, 5.705, 5.611, 5.517, 5.385, 5.240, 5.129, 5.003, 4.886, 4.750, 4.600, 4.439, 4.243, 4.072, 3.881, 3.677, 3.453, 3.251, 3.047, 2.840, 2.669, 2.559, 2.439]


# make splines of lb, ub, and median
l_weber_energies = np.logspace(np.log10(3), np.log10(80), 100)
l_weber_qy_lb = spline(l_weber_lb_x, l_weber_lb_y, l_weber_energies)
l_weber_qy_ub = spline(l_weber_ub_x, l_weber_ub_y, l_weber_energies)
l_weber_qy_median = spline(l_weber_median_x, l_weber_median_y, l_weber_energies)


# --------------------------------------------
# --------------------------------------------
#  Points from Lux
# --------------------------------------------
# --------------------------------------------

l_lux_energies_qy = [0.70, 1.10, 1.47, 2.00, 2.77, 3.86, 5.55, 8.02, 11.52, 16.56, 24.2, 74.0]
l_lux_energies_qy_unc = [0.13, 0.18, 0.12, 0.10, 0.10, 0.08, 0.09, 0.10, 0.12, 0.16, 0.2, 0.]
l_lux_qy = [8.2, 7.4, 10.1, 8.0, 7.5, 7.3, 7.2, 6.8, 5.88, 5.28, 4.62, 3.06]
l_lux_qy_unc = [[2.1, 1.7, 1.6, 0.6, 0.5, 0.3, 0.2, 0.17, 0.13, 0.13, 0.10, 0.2], [2.4, 1.9, 1.5, 0.9, 0.5, 0.3, 0.2, 0.15, 0.12, 0.11, 0.13, 0.4]]

l_lux_energies_py = [1.08, 1.92, 3.13, 4.45, 5.89, 7.44, 9.1, 10.9, 12.8, 74.0]
l_lux_energies_py_unc = [0.13, 0.09, 0.11, 0.11, 0.13, 0.17, 0.2, 0.3, 0.3, 0.]
l_lux_py = [4.9, 5.2, 4.9, 6.4, 6.1, 7.4, 7.9, 8.1, 8.9, 14.0]
l_lux_py_unc = [[1.0, 0.4, 0.4, 0.4, 0.3, 0.4, 0.4, 0.5, 0.4, 2.7], [1.2, 0.6, 0.5, 0.4, 0.4, 0.4, 0.4, 0.4, 0.6, 1.1]]




# set colors for different cathode settings
for cathode_setting in l_cathode_settings_in_use:
    if cathode_setting == 0.345:
        d_yields[cathode_setting]['color'] = 'b'
    elif cathode_setting == 1.054:
        d_yields[cathode_setting]['color'] = 'g'
    elif cathode_setting == 2.356:
        d_yields[cathode_setting]['color'] = 'r'


transparency_one_sigma = 0.1
transparency_two_sigma = 0.05



f, (ax1, ax2) = plt.subplots(2)

ax1.set_xlim(min_energy_plot, max_energy_plot)
ax1.set_ylim(3.5, 20)
ax1.set_xscale('log', nonposx='clip')
ax1.set_yscale('log', nonposx='clip')
ax1.set_xlabel('Energy [keV]')
ax1.set_ylabel(r'$L_{y} [\frac{photons}{keV}]$')

for cathode_setting in l_cathode_settings_in_use:

    #print 'debugging'
    #ax1.errorbar(d_yields[cathode_setting]['a_energy_mean_data'], d_yields[cathode_setting]['a_py_median_data'], xerr=d_yields[cathode_setting]['a_energy_width_data'], yerr=[d_yields[cathode_setting]['a_py_lb_data'], d_yields[cathode_setting]['a_py_ub_data']], fmt='%s.' % (d_yields[cathode_setting]['color']))

    #ax1.plot(a_energies_nest, d_yields[cathode_setting]['a_py_median_nest'], marker='', linestyle='-.', color=d_yields[cathode_setting]['color'])
    #ax1.fill_between(a_energies, d_yields[cathode_setting]['a_py_lb_nest'], d_yields[cathode_setting]['a_py_ub_nest'], facecolor=d_yields[cathode_setting]['color'], alpha=0.1, interpolate=True)
    
    
    ax1.plot(a_energies, d_yields[cathode_setting]['a_py_median_band'], marker='', linestyle='-', color=d_yields[cathode_setting]['color'])
    ax1.fill_between(a_energies, d_yields[cathode_setting]['a_py_lb_band'], d_yields[cathode_setting]['a_py_ub_band'], facecolor=d_yields[cathode_setting]['color'], alpha=transparency_one_sigma, interpolate=True)
    #ax1.fill_between(a_energies, d_yields[cathode_setting]['a_py_2lb_band'], d_yields[cathode_setting]['a_py_2ub_band'], facecolor=d_yields[cathode_setting]['color'], alpha=transparency_two_sigma, interpolate=True)

ax1.errorbar(l_plante_energies, l_plante_ly, xerr=l_plante_energies_unc, yerr=l_plante_ly_unc, marker='.', color='black', linestyle='')
ax1.errorbar(l_aprile_09_energies, l_aprile_09_ly, xerr=l_aprile_09_energies_unc, yerr=l_aprile_09_ly_unc, marker='.', color='orange', linestyle='')
ax1.errorbar(l_aprile_05_energies, l_aprile_05_ly, xerr=l_aprile_05_energies_unc, yerr=l_aprile_05_ly_unc, marker='.', color='moccasin', linestyle='')
ax1.errorbar(l_manzur_0_energies, l_manzur_0_ly, xerr=l_manzur_0_energies_unc, yerr=l_manzur_0_ly_unc, marker='.', color='grey', linestyle='')
#ax1.errorbar(l_manzur_730_energies, l_manzur_730_ly, xerr=l_manzur_730_energies_unc, yerr=l_manzur_730_ly_unc, marker='.', color='pink', linestyle='')
ax1.errorbar(l_lux_energies_py, l_lux_py, xerr=l_lux_energies_py_unc, yerr=l_lux_py_unc, fmt='c.')
ax1.fill_between(l_zepplin_fsr_energies, l_zepplin_fsr_ly_lb, l_zepplin_fsr_ly_ub, facecolor='y', alpha=transparency_one_sigma, interpolate=True)
ax1.fill_between(l_zepplin_ssr_energies, l_zepplin_ssr_ly_lb, l_zepplin_ssr_ly_ub, facecolor='olive', alpha=transparency_one_sigma, interpolate=True)
ax1.fill_between(l_sorenson_ly_energies, l_sorenson_ly_lb, l_sorenson_ly_ub, facecolor='darkred', alpha=transparency_one_sigma, interpolate=True)



ax2.set_xlim(min_energy_plot, max_energy_plot)
ax2.set_ylim(2, 14)
ax2.set_xscale('log', nonposx='clip')
ax2.set_yscale('log', nonposx='clip')
ax2.set_xlabel('Energy [keV]')
ax2.set_ylabel(r'$Q_{y} [\frac{electrons}{keV}]$')

for cathode_setting in l_cathode_settings_in_use:

    #print 'debugging'
    #ax2.errorbar(d_yields[cathode_setting]['a_energy_mean_data'], d_yields[cathode_setting]['a_qy_median_data'], xerr=d_yields[cathode_setting]['a_energy_width_data'], yerr=[d_yields[cathode_setting]['a_qy_lb_data'], d_yields[cathode_setting]['a_qy_ub_data']], fmt='%s.' % (d_yields[cathode_setting]['color']))

    # NEST
    #ax2.plot(a_energies_nest, d_yields[cathode_setting]['a_qy_median_nest'], marker='', linestyle='-.', color=d_yields[cathode_setting]['color'])
    #ax2.fill_between(a_energies, d_yields[cathode_setting]['a_qy_lb_nest'], d_yields[cathode_setting]['a_qy_ub_nest'], facecolor=d_yields[cathode_setting]['color'], alpha=0.1, interpolate=True)
    #ax2.fill_between(a_energies, d_yields[cathode_setting]['a_qy_lb_nest'], d_yields[cathode_setting]['a_qy_ub_nest'], color=None, edgecolor=d_yields[cathode_setting]['color'], hatch='x')

    # band
    ax2.plot(a_energies, d_yields[cathode_setting]['a_qy_median_band'], marker='', linestyle='-', color=d_yields[cathode_setting]['color'])
    ax2.fill_between(a_energies, d_yields[cathode_setting]['a_qy_lb_band'], d_yields[cathode_setting]['a_qy_ub_band'], facecolor=d_yields[cathode_setting]['color'], alpha=transparency_one_sigma, interpolate=True)
    #ax2.fill_between(a_energies, d_yields[cathode_setting]['a_qy_2lb_band'], d_yields[cathode_setting]['a_qy_2ub_band'], facecolor=d_yields[cathode_setting]['color'], alpha=transparency_two_sigma, interpolate=True)


ax2.errorbar(l_lux_energies_qy, l_lux_qy, xerr=l_lux_energies_qy_unc, yerr=l_lux_qy_unc, fmt='c.')
ax2.errorbar(l_manzur_730_energies_qy, l_manzur_730_qy, xerr=[l_manzur_730_energies_qy_unc_low, l_manzur_730_energies_qy_unc_high], yerr=[l_manzur_730_qy_unc_low, l_manzur_730_qy_unc_high], color='pink', marker='.', linestyle='')
ax2.errorbar(l_aprile_06_energies, l_aprile_06_qy, yerr=[l_aprile_06_qy_unc_low, l_aprile_06_qy_unc_high], color='deepskyblue', marker='.', linestyle='')

ax2.plot(l_weber_energies, l_weber_qy_median, marker='', linestyle='-', color='chocolate')
ax2.fill_between(l_weber_energies, l_weber_qy_lb, l_weber_qy_ub, facecolor='chocolate', alpha=transparency_one_sigma, interpolate=True)
ax2.fill_between(l_zepplin_fsr_energies, l_zepplin_fsr_qy_lb, l_zepplin_fsr_qy_ub, facecolor='y', alpha=transparency_one_sigma, interpolate=True)
ax2.fill_between(l_zepplin_fsr_energies, l_zepplin_fsr_qy_lb, l_zepplin_fsr_qy_ub, facecolor='olive', alpha=transparency_one_sigma, interpolate=True)
ax2.errorbar(l_sorenson_qy_energies, l_sorenson_qy, yerr=[l_sorenson_qy_unc_lb, l_sorenson_qy_unc_ub], color='darkred', marker='.', linestyle='')


#f.savefig('./plots/s_ly_qy_comparison.png')



# pickle all relevant data
d_for_save = {}
d_for_save['nerix'] = d_yields

d_for_save['ly_comps'] = {}
# points
d_for_save['ly_comps']['plante'] = {'ly_energies':l_plante_energies, 'ly':l_plante_ly, 'ly_energies_unc':l_plante_energies_unc, 'ly_unc':l_plante_ly_unc}
d_for_save['ly_comps']['aprile_09'] = {'ly_energies':l_aprile_09_energies, 'ly':l_aprile_09_ly, 'ly_energies_unc':l_aprile_09_energies_unc, 'ly_unc':l_aprile_09_ly_unc}
d_for_save['ly_comps']['aprile_05'] = {'ly_energies':l_aprile_05_energies, 'ly':l_aprile_05_ly, 'ly_energies_unc':l_aprile_05_energies_unc, 'ly_unc':l_aprile_05_ly_unc}
d_for_save['ly_comps']['manzur'] = {'ly_energies':l_manzur_0_energies, 'ly':l_manzur_0_ly, 'ly_energies_unc':l_manzur_0_energies_unc, 'ly_unc':l_manzur_0_ly_unc}
d_for_save['ly_comps']['lux'] = {'ly_energies':l_lux_energies_py, 'ly':l_lux_py, 'ly_energies_unc':l_lux_energies_py_unc, 'ly_unc':l_lux_py_unc}
# bands
d_for_save['ly_comps']['zepplin_fsr'] = {'ly_energies':l_zepplin_fsr_energies, 'ly_lb':l_zepplin_fsr_ly_lb, 'ly_ub':l_zepplin_fsr_ly_ub}
d_for_save['ly_comps']['zepplin_ssr'] = {'ly_energies':l_zepplin_ssr_energies, 'ly_lb':l_zepplin_ssr_ly_lb, 'ly_ub':l_zepplin_ssr_ly_ub}
d_for_save['ly_comps']['sorenson'] = {'ly_energies':l_sorenson_ly_energies, 'ly_lb':l_sorenson_ly_lb, 'ly_ub':l_sorenson_ly_ub}



d_for_save['qy_comps'] = {}
# points
d_for_save['qy_comps']['lux'] = {'qy_energies':l_lux_energies_qy, 'qy':l_lux_qy, 'qy_energies_unc':l_lux_energies_qy_unc, 'qy_unc':l_lux_qy_unc}
d_for_save['qy_comps']['manzur'] = {'qy_energies':l_manzur_730_energies_qy, 'qy':l_manzur_730_qy, 'qy_energies_unc':[l_manzur_730_energies_qy_unc_low, l_manzur_730_energies_qy_unc_high], 'qy_unc':[l_manzur_730_qy_unc_low, l_manzur_730_qy_unc_high]}
d_for_save['qy_comps']['aprile_06'] = {'qy_energies':l_aprile_06_energies, 'qy':l_aprile_06_qy, 'qy_energies_unc':[0 for i in xrange(len(l_aprile_06_qy))], 'qy_unc':[l_aprile_06_qy_unc_low, l_aprile_06_qy_unc_high]}
d_for_save['qy_comps']['sorenson'] = {'qy_energies':l_sorenson_qy_energies, 'qy':l_sorenson_qy, 'qy_energies_unc':[0 for i in xrange(len(l_sorenson_qy))], 'qy_unc':[l_sorenson_qy_unc_lb, l_sorenson_qy_unc_ub]}
# bands
d_for_save['qy_comps']['zepplin_fsr'] = {'qy_energies':l_zepplin_fsr_energies, 'qy_lb':l_zepplin_fsr_qy_lb, 'qy_ub':l_zepplin_fsr_qy_ub}
d_for_save['qy_comps']['zepplin_ssr'] = {'qy_energies':l_zepplin_ssr_energies, 'qy_lb':l_zepplin_ssr_qy_lb, 'qy_ub':l_zepplin_ssr_qy_ub}
d_for_save['qy_comps']['weber'] = {'qy_energies':l_weber_energies, 'qy_lb':l_weber_qy_lb, 'qy_ub':l_weber_qy_ub}




pickle.dump(d_for_save, open('./final_scripts/all_yields.p', 'w'))

plt.show()



"""


