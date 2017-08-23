#!/usr/bin/python
import fit_pmt_gain
import subprocess
import led_datasets


l_files = led_datasets.l_files


for files in l_files:

    print '\n\n\n\n\n\nWorking on %s and %s\n' % (files[0], files[1])

    num_walkers = '128'
    num_steps = '250'
    num_mc_events = str(int(1e7))
    num_steps_to_include = '20'

    #subprocess.call(['python', 'run_cascade_mcmc.py', files[0], files[1], num_walkers, num_steps, num_mc_events])
    subprocess.call(['python', 'create_cascade_plots.py', files[0], files[1], num_walkers, num_steps_to_include, num_mc_events])


