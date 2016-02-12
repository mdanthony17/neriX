#!/bin/sh

# Directives
#PBS -N neriX_full_matching
#PBS -W group_list=xenon
#PBS -l nodes=1,walltime=01:15:00
#PBS -M mda2149@columbia.edu
#PBS -m abe
#PBS -V

#Command to execute Python program
python perform_full_matching.py 45 1.054 4.5 128 5 16

#End of script


