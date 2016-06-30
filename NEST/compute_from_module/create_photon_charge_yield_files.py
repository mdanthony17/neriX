import sys, tqdm
from produce_nest_yields import nest_er_yields, nest_nr_yields
import numpy as np

low_energy = 0
high_energy = 122.
step_energy = 0.01

if low_energy == 0:
	low_energy += step_energy


f_photon_yield = open('nest_1.0_photon_yields.txt', 'w')
f_charge_yield = open('nest_1.0_charge_yields.txt', 'w')

l_fields = [190, 480, 1020, 2320]
l_current_photon_yields = [0. for i in xrange(len(l_fields))]
l_current_charge_yields = [0. for i in xrange(len(l_fields))]

for energy in tqdm.tqdm(np.arange(low_energy, high_energy, step_energy)):
	for i, drift_field in enumerate(l_fields):
		l_current_photon_yields[i], l_current_charge_yields[i] = nest_er_yields(energy, drift_field)

	s_photon_yield = '%.2f\t' % (energy)
	for py in l_current_photon_yields:
		s_photon_yield += '%.4f\t' % (py)
	s_photon_yield += '\n'

	s_charge_yield = '%.2f\t' % (energy)
	for py in l_current_charge_yields:
		s_charge_yield += '%.4f\t' % (py)
	s_charge_yield += '\n'

	f_photon_yield.write(s_photon_yield)
	f_charge_yield.write(s_charge_yield)

f_photon_yield.close()
f_charge_yield.close()


