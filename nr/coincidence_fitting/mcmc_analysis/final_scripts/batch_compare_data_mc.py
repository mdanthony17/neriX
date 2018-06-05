#!/usr/bin/python
from subprocess import call

l_cathode_settings = [0.345, 1.054, 2.356]
l_degree_settings = [-4, 3000, 3500, 4500, 5300]

for cathode_setting in l_cathode_settings:
    for degree_setting in l_degree_settings:
    
        print '\n\n\nWorking on %f kV at %d degrees\n\n\n' % (cathode_setting, degree_setting)
    
        call(['python', 'compare_data_fit.py', '512', str(cathode_setting), str(degree_setting), '4', 'f', 't'])



