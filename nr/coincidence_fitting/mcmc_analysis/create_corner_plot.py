#!/usr/bin/python
import sys, array, os
sys.path.insert(0, '..')

import ROOT as root
from rootpy.plotting import Hist, Hist2D, Canvas, Legend
import neriX_simulation_config
import numpy as np
import corner, time
import cPickle as pickle

if len(sys.argv) != 3:
	print 'Use is python perform_full_matching.py <cathode setting> <num walkers>'
	sys.exit()

print '\n\nBy default look for all energies - change source if anything else is needed.\n'

cathode_setting = float(sys.argv[1])
num_walkers = int(sys.argv[2])

l_degree_settings_in_use = [2300, 3000, 3500, 4500, 5300, 6200]
s_degree_settings = ''
for degree_setting in l_degree_settings_in_use:
	s_degree_settings += '%s,' % (degree_setting)
s_degree_settings = s_degree_settings[:-1]

nameOfResultsDirectory = neriX_simulation_config.results_directory_name
l_plots = ['plots']

dir_specifier_name = '%.3f_kV_%s_deg' % (cathode_setting, s_degree_settings)

nameOfResultsDirectory += '/yields_fit'

sPathToFile = './%s/%s/sampler_dictionary.p' % (nameOfResultsDirectory, dir_specifier_name)

if os.path.exists(sPathToFile):
	dSampler = pickle.load(open(sPathToFile, 'r'))
	aSampler = dSampler[num_walkers][-1]['_chain'] # look at last sampler only (can change)
	print 'Successfully loaded sampler!'
else:
	print sPathToFile
	print 'Could not find file!'
	sys.exit()



# need to figure this out
numDim = 41

lLabelsForCorner = ['py_0', 'py_1', 'py_2', 'py_3', 'py_4', 'py_5', 'py_6', 'py_7', 'qy_0', 'qy_1', 'qy_2', 'qy_3', 'qy_4', 'qy_5', 'qy_6', 'qy_7', 'intrinsic_res_s1', 'intrinsic_res_s2', 'g1_value', 'spe_res_rv', 'g2_value', 'gas_gain_rv', 'gas_gain_width_rv', 'pf_eff_par0', 'pf_eff_par1', 's1_eff_par0', 's1_eff_par1', 's2_eff_par0', 's2_eff_par1', 'pf_stdev_par0', 'pf_stdev_par1', 'pf_stdev_par2', 'exciton_to_ion_par0_rv', 'exciton_to_ion_par1_rv', 'exciton_to_ion_par2_rv', 'scale_par_0', 'scale_par_1', 'scale_par_2', 'scale_par_3', 'scale_par_4', 'scale_par_5']

samples = aSampler[:, -5:, :].reshape((-1, numDim))
start_time = time.time()
print 'Starting corner plot...\n'
fig = corner.corner(samples, labels=lLabelsForCorner, quantiles=[0.16, 0.5, 0.84], show_titles=True, title_kwargs={"fontsize": 12})
print 'Corner plot took %.3f minutes.\n\n' % ((time.time()-start_time)/60.)

# path for save
sPathForSave = './'
for directory in l_plots:
	sPathForSave += directory + '/'

if not os.path.exists(sPathForSave):
	os.makedirs(sPathForSave)

fig.savefig('%scorner_plot_%s.png' % (sPathForSave, dir_specifier_name))



#raw_input('Enter to continue...')
