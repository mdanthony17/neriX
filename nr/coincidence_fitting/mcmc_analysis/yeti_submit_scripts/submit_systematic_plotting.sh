#!/bin/sh

# Directives
#PBS -N neriX_plot_full_matching
#PBS -W group_list=yetixenon
#PBS -l nodes=1:ppn=1,walltime=00:05:00,mem=4000mb
#PBS -M mda2149@columbia.edu
#PBS -m abe
#PBS -V

#set output and error directories
#PBS -o localhost:/u/3/m/mda2149/neriX/nr/coincidence_fitting/mcmc_analysis/yeti_submit_scripts/qsub/
#PBS -e localhost:/u/3/m/mda2149/neriX/nr/coincidence_fitting/mcmc_analysis/yeti_submit_scripts/qsub/

#Command to execute Python program
python ../plot_free_parameters.py 23 1.054 4.5 full_matching 2048 t ${rate} ${num}

#End of script


