#!/bin/sh

# Directives
#PBS -N neriX_full_matching
#PBS -W group_list=yetixenon
#PBS -l nodes=1:ppn=16,walltime=01:15:00,mem=100mb
#PBS -M mda2149@columbia.edu
#PBS -m abe
#PBS -V

#set output and error directories
#PBS -o localhost:/u/3/m/mda2149/neriX/nr/coincidence_fitting/mcmc_analysis/    yeti_submit_scripts/qsub/
#PBS -e localhost:/u/3/m/mda2149/neriX/nr/coincidence_fitting/mcmc_analysis/    yeti_submit_scripts/qsub/

#Command to execute Python program
python perform_full_matching.py 45 1.054 4.5 128 5 16

#End of script


