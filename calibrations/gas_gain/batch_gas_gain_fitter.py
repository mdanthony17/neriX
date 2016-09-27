#!/usr/bin/python
import pickle
#print pickle.Pickler.dispatch
import dill
#print pickle.Pickler.dispatch

import fit_gas_gain

num_walkers = 64
num_steps = 2000
num_steps_to_include = 1000


l_files = []

#l_files.append('nerix_160329_2245') # weird
"""
l_files.append('nerix_160404_1059')
l_files.append('nerix_160407_1357')
l_files.append('nerix_160412_0919')
l_files.append('nerix_160414_1215')
l_files.append('nerix_160418_0919')
l_files.append('nerix_160421_1356')
#l_files.append('nerix_160425_0918') # missing
l_files.append('nerix_160428_1124')
#l_files.append('nerix_160429_1124') # missing
l_files.append('nerix_160502_0940')
#l_files.append('nerix_160505_0935') # bad
l_files.append('nerix_160509_1050')
l_files.append('nerix_160512_1224')
l_files.append('nerix_160516_0958')
l_files.append('nerix_160524_1140')
l_files.append('nerix_160531_1031')
l_files.append('nerix_160606_1046')
l_files.append('nerix_160614_1054')
l_files.append('nerix_160620_1048')
l_files.append('nerix_160627_1046')
l_files.append('nerix_160705_1049')
l_files.append('nerix_160711_0911')
l_files.append('nerix_160718_1028')
l_files.append('nerix_160725_1039')
"""
l_files.append('nerix_160728_1046')
l_files.append('nerix_160801_1036')
l_files.append('nerix_160808_1141')


for file in l_files:
    gas_gain_fitter = fit_gas_gain.fit_gas_gain(file)
    
    gas_gain_fitter.run_gas_gain_mcmc(num_walkers=num_walkers, num_steps=num_steps, threads=7)
    gas_gain_fitter.draw_gas_gain_corner_plot(num_walkers=num_walkers, num_steps_to_include=num_steps_to_include)
    gas_gain_fitter.draw_best_fit_on_hist(num_walkers=num_walkers, num_steps_to_include=num_steps_to_include)


