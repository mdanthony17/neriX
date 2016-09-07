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



def get_ly_qy_from_nest_pars(energy, w_value, alpha, zeta, beta, gamma, delta, kappa, eta, lamb):

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





if len(sys.argv) != 4:
    print 'Use is python perform_full_matching.py <cathode setting> <num walkers> <fit type>'
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



cathode_setting = float(sys.argv[1])
mean_field = d_cathode_setting_to_mean_field[cathode_setting]

num_walkers = int(sys.argv[2])
fit_type = sys.argv[3]

if fit_type == 's':
    directory_descriptor = 'single_energy'
elif fit_type == 'm':
    directory_descriptor = 'multiple_energies'
elif fit_type == 'ml':
    directory_descriptor = 'multiple_energies_lindhard_model'
    l_par_names = ['w_value_rv', 'alpha', 'zeta', 'beta', 'gamma', 'delta', 'kappa', 'eta', 'lambda']

#l_degree_settings_in_use = [3500]
l_degree_settings_in_use = [2300, 3000, 3500, 4500, 5300]
l_energy_range = [1., 30.]

s_degree_settings = ''
for degree_setting in l_degree_settings_in_use:
    s_degree_settings += '%s,' % (degree_setting)
s_degree_settings = s_degree_settings[:-1]

nameOfResultsDirectory = neriX_simulation_config.results_directory_name
l_plots = ['plots']

dir_specifier_name = '%.3f_kV_%s_deg' % (cathode_setting, s_degree_settings)

nameOfResultsDirectory += '/%s' % (directory_descriptor)

sPathToFile = './%s/%s/sampler_dictionary.p' % (nameOfResultsDirectory, dir_specifier_name)

if os.path.exists(sPathToFile):
    dSampler = pickle.load(open(sPathToFile, 'r'))
    l_chains = []
    for sampler in dSampler[num_walkers]:
        l_chains.append(sampler['_chain'])

    aSampler = np.concatenate(l_chains, axis=1)

    print 'Successfully loaded sampler!'
else:
    print sPathToFile
    print 'Could not find file!'
    sys.exit()



# markov chain itself is random sample of posterior so use this
# to randomly sample parameter posterior (why do the work twice?)

num_steps = 500
num_dim = len(l_par_names)

samples = aSampler[:, -num_steps:, :num_dim].reshape((-1, num_dim))

# make energy range for histogram
num_energies = 50
a_energies = np.linspace(l_energy_range[0], l_energy_range[1], num_energies)

a_median_py = np.zeros(num_energies)
a_one_sigma_above_py = np.zeros(num_energies)
a_one_sigma_below_py = np.zeros(num_energies)

a_median_qy = np.zeros(num_energies)
a_one_sigma_above_qy = np.zeros(num_energies)
a_one_sigma_below_qy = np.zeros(num_energies)

a_median_py_nest = np.zeros(num_energies)
a_one_sigma_above_py_nest = np.zeros(num_energies)
a_one_sigma_below_py_nest = np.zeros(num_energies)

a_median_qy_nest = np.zeros(num_energies)
a_one_sigma_above_qy_nest = np.zeros(num_energies)
a_one_sigma_below_qy_nest = np.zeros(num_energies)

num_independent_samples = samples.shape[0]

a_temp_py = np.zeros(num_independent_samples)
a_temp_qy = np.zeros(num_independent_samples)

a_temp_py_nest = np.zeros(num_independent_samples)
a_temp_qy_nest = np.zeros(num_independent_samples)

for bin_number, energy in tqdm.tqdm(enumerate(a_energies)):
    for i in xrange(num_independent_samples):
        # grab fit parameters
        w_value = (13.7 + samples[i][0]*0.4)/1000.
        alpha = samples[i][1]
        zeta = samples[i][2]
        beta = samples[i][3]
        gamma = samples[i][4]
        delta = samples[i][5]
        kappa = samples[i][6]
        eta = samples[i][7]
        lamb = samples[i][8]
        
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


        a_temp_py[i], a_temp_qy[i] = get_ly_qy_from_nest_pars(energy, w_value, alpha, zeta, beta, gamma, delta, kappa, eta, lamb)
        a_temp_py_nest[i], a_temp_qy_nest[i] = get_ly_qy_from_nest_pars(energy, w_value_nest, alpha_nest, zeta_nest, beta_nest, gamma_nest, delta_nest, kappa_nest, eta_nest, lamb_nest)

    a_one_sigma_below_py[bin_number], a_median_py[bin_number], a_one_sigma_above_py[bin_number] = np.percentile(a_temp_py, [16, 50, 84])
    a_one_sigma_below_qy[bin_number], a_median_qy[bin_number], a_one_sigma_above_qy[bin_number] = np.percentile(a_temp_qy, [16, 50, 84])

    a_one_sigma_below_py_nest[bin_number], a_median_py_nest[bin_number], a_one_sigma_above_py_nest[bin_number] = np.percentile(a_temp_py_nest, [16, 50, 84])
    a_one_sigma_below_qy_nest[bin_number], a_median_qy_nest[bin_number], a_one_sigma_above_qy_nest[bin_number] = np.percentile(a_temp_qy_nest, [16, 50, 84])

#print a_one_sigma_below_py_nest, a_median_py_nest, a_one_sigma_above_py_nest

def multiply_leff_array_by_co57_ly(l_leff):
    for i in xrange(len(l_leff)):
        l_leff[i] *= 63.
    return l_leff





# --------------------------------------------
# --------------------------------------------
#  Points from Plante et al
# --------------------------------------------
# --------------------------------------------

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









f, (ax1, ax2) = plt.subplots(2)

ax1.set_xlim(0.95*min(a_energies), 1.05*max(a_energies))
ax1.set_ylim(1.5, 13)
ax1.set_xscale('log', nonposx='clip')
ax1.set_yscale('log', nonposx='clip')
ax1.set_xlabel('Energy [keV]')
ax1.set_ylabel(r'$L_{y} [\frac{photons}{keV}]$')

ax1.plot(a_energies, a_median_py, 'b-')
ax1.fill_between(a_energies, a_one_sigma_below_py, a_one_sigma_above_py, facecolor='blue', alpha=0.2, interpolate=True)
ax1.plot(a_energies, a_median_py_nest, 'r--')
ax1.fill_between(a_energies, a_one_sigma_below_py_nest, a_one_sigma_above_py_nest, facecolor='red', alpha=0.1, interpolate=True)

ax1.errorbar(l_plante_energies, l_plante_ly, xerr=l_plante_energies_unc, yerr=l_plante_ly_unc, fmt='g.')
ax1.errorbar(l_lux_energies_py, l_lux_py, xerr=l_lux_energies_py_unc, yerr=l_lux_py_unc, fmt='m.')




ax2.set_xlim(0.95*min(a_energies), 1.05*max(a_energies))
ax2.set_ylim(2, 12)
ax2.set_xscale('log', nonposx='clip')
ax2.set_yscale('log', nonposx='clip')
ax2.set_xlabel('Energy [keV]')
ax2.set_ylabel(r'$Q_{y} [\frac{electrons}{keV}]$')

ax2.plot(a_energies, a_median_qy, 'b-')
ax2.fill_between(a_energies, a_one_sigma_below_qy, a_one_sigma_above_qy, facecolor='blue', alpha=0.2, interpolate=True)
ax2.plot(a_energies, a_median_qy_nest, 'r--')
ax2.fill_between(a_energies, a_one_sigma_below_qy_nest, a_one_sigma_above_qy_nest, facecolor='red', alpha=0.2, interpolate=True)

ax2.errorbar(l_lux_energies_qy, l_lux_qy, xerr=l_lux_energies_qy_unc, yerr=l_lux_qy_unc, fmt='m.')

f.savefig('./plots/ml_ly_qy_comparison.png')

plt.show()






