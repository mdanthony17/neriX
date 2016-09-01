#!/usr/bin/python
import sys, array, os
sys.path.insert(0, '..')

import ROOT as root
from rootpy.plotting import Hist, Hist2D, Canvas, Legend
import neriX_simulation_config
import numpy as np
import corner, time
import cPickle as pickle

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


cathode_setting = float(sys.argv[1])
num_walkers = int(sys.argv[2])
fit_type = sys.argv[3]

if fit_type == 's':
    directory_descriptor = 'single_energy'
elif fit_type == 'm':
    directory_descriptor = 'multiple_energies'
elif fit_type == 'ml':
    directory_descriptor = 'multiple_energies_lindhard_model'

#l_degree_settings_in_use = [3500]
l_degree_settings_in_use = [2300, 3000, 3500, 4500, 5300]

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



if fit_type == 's':
    numDim = 19
    lLabelsForCorner = ['py', 'qy', 'intrinsic_res_s1', 'intrinsic_res_s2', 'g1_value', 'spe_res_rv', 'g2_value', 'gas_gain_rv', 'gas_gain_width_rv', 'pf_eff_par0', 'pf_eff_par1', 's2_eff_par0', 's2_eff_par1', 'pf_stdev_par0', 'pf_stdev_par1', 'pf_stdev_par2', 'exciton_to_ion_par0_rv', 'exciton_to_ion_par1_rv', 'exciton_to_ion_par2_rv']
elif fit_type == 'm':
    numDim = 31
    
    l_py = ['py_%d' % (i) for i in xrange(7)]
    l_qy = ['qy_%d' % (i) for i in xrange(7)]
    
    lLabelsForCorner = l_py + l_qy + ['intrinsic_res_s1', 'intrinsic_res_s2', 'g1_value', 'spe_res_rv', 'g2_value', 'gas_gain_rv', 'gas_gain_width_rv', 'pf_eff_par0', 'pf_eff_par1', 's2_eff_par0', 's2_eff_par1', 'pf_stdev_par0', 'pf_stdev_par1', 'pf_stdev_par2', 'exciton_to_ion_par0_rv', 'exciton_to_ion_par1_rv', 'exciton_to_ion_par2_rv']
elif fit_type == 'ml':
    numDim = 23
    lLabelsForCorner = ['w_value_rv', 'alpha', 'zeta', 'beta', 'gamma', 'delta', 'kappa', 'eta', 'lambda', 'intrinsic_res_s1', 'intrinsic_res_s2', 'g1_value', 'spe_res_rv', 'g2_value', 'gas_gain_rv', 'gas_gain_width_rv', 'pf_eff_par0', 'pf_eff_par1', 's2_eff_par0', 's2_eff_par1', 'pf_stdev_par0', 'pf_stdev_par1', 'pf_stdev_par2']

num_steps = 500

samples = aSampler[:, -num_steps:, :].reshape((-1, numDim))
start_time = time.time()
print 'Starting corner plot...\n'
fig = corner.corner(samples, labels=lLabelsForCorner, quantiles=[0.16, 0.5, 0.84], show_titles=True, title_fmt='.3e', title_kwargs={"fontsize": 12})
print 'Corner plot took %.3f minutes.\n\n' % ((time.time()-start_time)/60.)

# path for save
sPathForSave = './'
for directory in l_plots:
    sPathForSave += directory + '/'

if not os.path.exists(sPathForSave):
    os.makedirs(sPathForSave)

fig.savefig('%s%s_corner_plot_%s.png' % (sPathForSave, fit_type, dir_specifier_name))



#raw_input('Enter to continue...')
