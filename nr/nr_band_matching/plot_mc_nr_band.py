#!/usr/bin/python
import matplotlib
matplotlib.use('QT4Agg')

import ROOT as root
import sys, os
import neriX_analysis, root_numpy
from rootpy.plotting import Hist2D, Hist, Legend, Canvas
from rootpy import stl
from rootpy.io import File
import numpy as np
from math import exp, factorial, erf, ceil, log, pow, floor
from scipy import optimize, misc, stats
from scipy.stats import norm
import copy_reg, types, pickle, click, time, emcee, corner
from pprint import pprint
from produce_nest_yields import nest_nr_mean_yields
import gc, nr_band_config
from scipy.stats import multivariate_normal
from tqdm import tqdm


import matplotlib.pyplot as plt
import matplotlib.mlab as mlab
import rootpy.compiled as C

stl.vector(stl.vector('float'))
stl.vector(stl.vector('double'))
stl.vector(stl.vector('int'))



import cuda_full_observables_production
from pycuda.compiler import SourceModule
import pycuda.driver as drv
import pycuda.tools
import pycuda.gpuarray
#drv.init()
#dev = drv.Device(0)
#ctx = dev.make_context(drv.ctx_flags.SCHED_AUTO | drv.ctx_flags.MAP_HOST)
import pycuda.autoinit

#gpu_observables_func = SourceModule(cuda_full_observables_production.cuda_full_observables_production_code, no_extern_c=True).get_function('gpu_full_observables_production')
gpu_observables_func = SourceModule(cuda_full_observables_production.cuda_full_observables_production_code, no_extern_c=True).get_function('gpu_full_observables_production_spline')



def gpu_mc_nr_band_hist_output(num_mc_events, mean_field, a_spline_energies, a_energy, a_py, a_qy, intrinsic_res_s1, intrinsic_res_s2, g1_value, spe_res, g2_value, gas_gain, gas_gain_width, s1_eff_par0, s1_eff_par1, s2_eff_par0, s2_eff_par1, a_pf_res, exciton_to_ion_par0_rv, exciton_to_ion_par1_rv, exciton_to_ion_par2_rv):


	# need to do slightly fewer elements than
	# originally perscribed bc energies already
	# produced
	d_gpu_scale = {}
	d_gpu_scale['block'] = (1024,1,1)
	numBlocks = floor(num_mc_events / 1024.)
	d_gpu_scale['grid'] = (int(numBlocks), 1)
	num_mc_events = int(numBlocks*1024)



	# -----------------------------------------------
	# -----------------------------------------------
	# run MC
	# -----------------------------------------------
	# -----------------------------------------------

	# hard-code nuissance parameters
	#g1_rv, spe_res_rv, g2_rv, gas_gain_rv, gas_gain_width_rv, exciton_to_ion_par0_rv, exciton_to_ion_par1_rv, exciton_to_ion_par2_rv = 0, 0, 0, 0, 0, 0, 0, 0


	seed = np.asarray(int(time.time()*1000), dtype=np.int32)
	num_trials = np.asarray(num_mc_events, dtype=np.int32)
	mean_field = np.asarray(mean_field, dtype=np.float32)
	num_spline_points = np.asarray(len(a_spline_energies), dtype=np.int32)
	
	a_spline_energies = np.asarray(a_spline_energies, dtype=np.float32)
	a_spline_photon_yields = np.asarray(a_py, dtype=np.float32)
	a_spline_charge_yields = np.asarray(a_qy, dtype=np.float32)
	
	g1_value = np.asarray(g1_value, dtype=np.float32)
	extraction_efficiency = np.asarray(g2_value/gas_gain, dtype=np.float32)
	spe_res = np.asarray(spe_res, dtype=np.float32)
	gas_gain_value = np.asarray(gas_gain, dtype=np.float32)
	gas_gain_width = np.asarray(gas_gain_width, dtype=np.float32)
	
	intrinsic_res_s1 = np.asarray(intrinsic_res_s1, dtype=np.float32)
	intrinsic_res_s2 = np.asarray(intrinsic_res_s2, dtype=np.float32)
	
	a_pf_res = np.asarray(a_pf_res, dtype=np.float32)

	exciton_to_ion_par0_rv = np.asarray(exciton_to_ion_par0_rv, dtype=np.float32)
	exciton_to_ion_par1_rv = np.asarray(exciton_to_ion_par1_rv, dtype=np.float32)
	exciton_to_ion_par2_rv = np.asarray(exciton_to_ion_par2_rv, dtype=np.float32)
	
	s1_eff_par0 = np.asarray(s1_eff_par0, dtype=np.float32)
	s1_eff_par1 = np.asarray(s1_eff_par1, dtype=np.float32)

	s2_eff_par0 = np.asarray(s2_eff_par0, dtype=np.float32)
	s2_eff_par1 = np.asarray(s2_eff_par1, dtype=np.float32)
	
	a_band_cut = np.concatenate([[-5, 0], [0, 1, 10]])
	a_band_cut = np.asarray(a_band_cut, dtype=np.float32)
	
	gpu_aEnergy = pycuda.gpuarray.to_gpu(a_energy)
	
	a_s1 = np.zeros(len(a_energy), dtype=np.float32)
	a_s1.fill(-1)
	a_s2 = np.zeros(len(a_energy), dtype=np.float32)
	a_s2.fill(-1)
	
	
	# for histogram
	tArgs = (drv.In(seed), drv.In(num_trials), drv.In(mean_field), gpu_aEnergy, drv.In(num_spline_points), drv.In(a_spline_energies), drv.In(a_spline_photon_yields), drv.In(a_spline_charge_yields), drv.InOut(a_s1), drv.InOut(a_s2), drv.In(g1_value), drv.In(extraction_efficiency), drv.In(gas_gain_value), drv.In(gas_gain_width), drv.In(spe_res), drv.In(intrinsic_res_s1), drv.In(intrinsic_res_s2), drv.In(a_pf_res), drv.In(exciton_to_ion_par0_rv), drv.In(exciton_to_ion_par1_rv), drv.In(exciton_to_ion_par2_rv), drv.In(s1_eff_par0), drv.In(s1_eff_par1), drv.In(s2_eff_par0), drv.In(s2_eff_par1), drv.In(a_band_cut))
	
	gpu_observables_func(*tArgs, **d_gpu_scale)

	return a_s1, a_s2
	






def reduce_method(m):
	return (getattr, (m.__self__, m.__func__.__name__))


d_cathode_voltage_to_field = {0.345:210,
							  1.054:490,
							  2.356:1000}



# variables to change

num_mc_events = int(5e6)
intrinsic_res_s1 = 0.05
intrinsic_res_s2 = 0.15
exciton_to_ion_par0_rv = 0
exciton_to_ion_par1_rv = 0
exciton_to_ion_par2_rv = 0

l_dictionaries_efficiencies = [{'title':'No Efficiencies', 'l_efficiencies':[-100, 1, -100, 1]},
							   {'title':'S1 Efficiency Only', 'l_efficiencies':[1.96, 0.46, -100, 1]},
							   {'title':'S2 Efficiency Only', 'l_efficiencies':[-100, 1, 300, 75]},
							   {'title':'S1 and S2 Efficiencies Applied', 'l_efficiencies':[1.96, 0.46, 300, 75]}]


s1_branch = 'cpS1sTotBottom[0]'
s2_branch = 'cpS2sTotBottom[0]'

s1_settings_mc = (400, 0, 30)
log_settings_mc = (400, 1, 4.5)

s1_settings = (40, 0, 30)
s2_settings = (40, 0, 3000)
log_settings = (40, 1, 4.5)

# -----------------------------------------------
# -----------------------------------------------
# instantiate neriX_analysis and make cuts
# -----------------------------------------------
# -----------------------------------------------

current_analysis = neriX_analysis.neriX_analysis('nerix_160419_1331')

current_analysis.add_z_cut()
current_analysis.add_radius_cut(0, 0.85)
current_analysis.add_single_scatter_cut()
current_analysis.add_xs1asym_cut()
current_analysis.add_xs2asym_cut()
current_analysis.add_cut('%s < %f' % (s1_branch, s1_settings[2]))
current_analysis.add_cut('%s < %f' % (s2_branch, s2_settings[2]))
current_analysis.add_cut('log10((S1Tot + S2Tot)/(AreaTot - S1Tot - S2Tot)) > 0.')

current_analysis.set_event_list()

filename = current_analysis.get_filename_no_ext()
cathode_setting = current_analysis.get_cathode_setting()
anode_setting = current_analysis.get_anode_setting()

# find mean field
mean_field = d_cathode_voltage_to_field[cathode_setting]


h_s1_s2 = Hist2D(*(s1_settings+s2_settings))
current_analysis.Draw('%s:%s' % (s1_branch, s2_branch), hist=h_s1_s2, selection=current_analysis.get_cuts())

a_s1_bin_edges = np.linspace(s1_settings[1], s1_settings[2], num=s1_settings[0]+1, dtype=np.float32)
a_s2_bin_edges = np.linspace(s2_settings[1], s2_settings[2], num=s2_settings[0]+1, dtype=np.float32)


g1_value, g2_value = [0.12961637894547148, 20.88715549107226]
gas_gain_value = 21.85
gas_gain_width = 9.2
spe_res_value = 0.66

a_spline_energies = np.asarray([0.5, 2.96, 4.93, 6.61, 9.76, 13.88, 17.5, 25], dtype=np.float32)
a_pf_res = np.asarray([0.0147, 0.528, 4.323], dtype=np.float32)

a_nest_photon_yields = np.zeros(len(a_spline_energies), dtype=np.float32)
a_nest_charge_yields = np.zeros(len(a_spline_energies), dtype=np.float32)
for i in xrange(len(a_spline_energies)):
	l_yields = nest_nr_mean_yields(a_spline_energies[i], mean_field)
	a_nest_photon_yields[i] = l_yields[0]
	a_nest_charge_yields[i] = l_yields[1]



f_mc_nr_band = File('../coincidence_fitting/simulated_data/nerixsim-pure_nr_spec.root')
#f_mc_nr_band = File('../coincidence_fitting/simulated_data/nerixsim-20keV.root')
t_mc = f_mc_nr_band.t2
print '\nGeant4 output loaded!\n'

xRadius = '(sqrt(xpos[0]**2+ypos[0]**2) < 18)'
xZ = '((zpos[0]>-20 && zpos[0]<-4))'
xSingleScatter = '(nsteps_target==1)'
#xLXeEnergy = '(etot_target>0)' # this includes ER
xSingleScatter_2 = '(Alt$(ed_nr_target[1],-1)==-1)'
xLXeEnergy = '(ed_nr_target[0] > 0.)'

xLiqSciHeight = '(etotliqsci>700)'
xTOF = '(timeliqsci-tpos[0]> 10 && timeliqsci-tpos[0] < 41)'

xAll = '%s && %s && %s && %s && %s' % (xRadius, xZ, xSingleScatter, xLXeEnergy, xSingleScatter_2)
#xAll = '%s && %s && %s && %s && %s && %s && %s' % (xRadius, xZ, xSingleScatter, xLXeEnergy, xSingleScatter_2, xLiqSciHeight, xTOF)

l_energy_settings_large_window = [1000, 0, 100]
l_energy_settings_small_window = [1000, 0, 30]

h_energy_large_window = Hist(*l_energy_settings_large_window, name='h_energy_large_window', title='Geant4 NR Band Energy Spectrum')
h_energy_large_window.SetMarkerSize(0)
h_energy_large_window.SetStats(0)
h_energy_large_window.GetXaxis().SetTitle('Energy [keV]')
h_energy_large_window.GetYaxis().SetTitle('Counts')
h_energy_small_window = Hist(*l_energy_settings_small_window, name='h_energy_small_window', title='Geant4 NR Band Energy Spectrum')
h_energy_small_window.SetMarkerSize(0)
h_energy_small_window.SetStats(0)
h_energy_small_window.GetXaxis().SetTitle('Energy [keV]')
h_energy_small_window.GetYaxis().SetTitle('Counts')

el_mc_energy = root.TEventList('el_mc_energy')
print '\nSetting energy event list...\n'
root.TTree.Draw(t_mc, '>>el_mc_energy', xAll, '')
t_mc.SetEventList(el_mc_energy)

t_mc.Draw('ed_nr_target[0]', hist=h_energy_large_window, selection=xAll)
t_mc.Draw('ed_nr_target[0]', hist=h_energy_small_window, selection=xAll)


c_energy_spectrum = Canvas(1100, 700)
c_energy_spectrum.Divide(2)

c_energy_spectrum.cd(1)
c_energy_spectrum.cd(1).SetLogy()
h_energy_small_window.Draw()

c_energy_spectrum.cd(2)
c_energy_spectrum.cd(2).SetLogy()
h_energy_large_window.Draw()

c_energy_spectrum.Update()



print '\nFilling energy array\n'
a_energy = np.zeros(num_mc_events, dtype=np.float32)
for i in tqdm(xrange(num_mc_events)):
	a_energy[i] = h_energy_large_window.GetRandom()
	#a_energy[i] = 10




c_mc_spectra = Canvas(1200, 1400)
c_mc_spectra.Divide(2, 2)

for i, d_efficiency_settings in enumerate(l_dictionaries_efficiencies):

	s1_eff_par0, s1_eff_par1, s2_eff_par0, s2_eff_par1 = d_efficiency_settings['l_efficiencies']
	d_efficiency_settings['hist'] = Hist2D(*(s1_settings_mc+log_settings_mc), title=d_efficiency_settings['title'])
	d_efficiency_settings['hist'].SetStats(0)
	d_efficiency_settings['hist'].GetXaxis().SetTitle('S1 [PE]')
	d_efficiency_settings['hist'].GetYaxis().SetTitle('Log10(S2/S1)')

	print '\nCreating MC spectrum...\n'
	a_s1, a_s2 = gpu_mc_nr_band_hist_output(num_mc_events, mean_field, a_spline_energies, a_energy, a_nest_photon_yields, a_nest_charge_yields, intrinsic_res_s1, intrinsic_res_s2, g1_value, spe_res_value, g2_value, gas_gain_value, gas_gain_width, s1_eff_par0, s1_eff_par1, s2_eff_par0, s2_eff_par1, a_pf_res, exciton_to_ion_par0_rv, exciton_to_ion_par1_rv, exciton_to_ion_par2_rv)

	c_mc_spectra.cd(i+1)
	c_mc_spectra.cd(i+1).SetGridx()
	c_mc_spectra.cd(i+1).SetGridy()
	
	a_2d_array = np.asarray([a_s1, np.log10(a_s2/a_s1)])
	d_efficiency_settings['hist'].fill_array(a_2d_array.T)
	d_efficiency_settings['hist'].Draw('colz')
	
	c_mc_spectra.cd(i+1).SetLogz()

	c_mc_spectra.Update()



c_nr_band_data = Canvas()

h_nr_band_data = Hist2D(*(s1_settings+log_settings), title='NR Band Data')
h_nr_band_data.SetStats(0)
h_nr_band_data.GetXaxis().SetTitle('S1 [PE]')
h_nr_band_data.GetYaxis().SetTitle('Log10(S2/S1)')

current_analysis.Draw('%s:log10(%s/%s)' % (s1_branch, s2_branch, s1_branch), hist=h_nr_band_data, selection=current_analysis.get_cuts())
h_nr_band_data.Draw('colz')

c_nr_band_data.SetLogz()
c_nr_band_data.SetGridx()
c_nr_band_data.SetGridy()
c_nr_band_data.Update()

neriX_analysis.save_plot(['mc_plots'], c_energy_spectrum, 'energy_spectrum_mc')
neriX_analysis.save_plot(['mc_plots'], c_mc_spectra, 'nr_band_mc')
neriX_analysis.save_plot(['mc_plots'], c_nr_band_data, 'nr_band_data')


raw_input('Press enter to continue')



