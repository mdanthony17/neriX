#!/usr/bin/python

import ROOT as root
import sys, array, math
import neriX_analysis
from rootpy.plotting import Hist, Hist2D, Canvas, Legend
from rootpy.io import root_open, File
import numpy as np
from tqdm import tqdm
import root_numpy
from math import exp

if len(sys.argv) != 2:
	print 'Usage is python reduce_nai_file.py <filename>'
	sys.exit()

filename = sys.argv[1]


current_analysis = neriX_analysis.neriX_analysis(filename, -50, 1.054, 4.5)


#--------------- Start Parameters to Change ----------------


neriX_analysis.warning_message('Hard-coded gain correction')
s2_branch = 'S2sTotBottom[0]*1.48e6/9e5'
l_s2_settings = [20, 0, 2000]


#--------------- End Parameters to Change ----------------


current_analysis.add_cut('%s > 0' % (s2_branch))
current_analysis.add_cut('%s < %f' % (s2_branch, l_s2_settings[2]))


#current_analysis.multithread_set_event_list(7)

path_and_filename_reduced = './reduced_root_files/r_%s.root' % current_analysis.get_filename_no_ext()

f_reduced = File(path_and_filename_reduced, 'create')


t1 = current_analysis.get_T1()
t2 = current_analysis.get_T2()
t3 = current_analysis.get_T3()

t2.AddFriend(t1.GetName())
t2.AddFriend(t3.GetName())

t3.AddFriend(t1.GetName())
t3.AddFriend(t2.GetName())



print 'Reducing T1...'
t1 = current_analysis.get_T1().copy_tree(current_analysis.get_cuts())
t1.SetName('T1')
t1.Write()


print 'Reducing T2...'
t2 = current_analysis.get_T2().copy_tree(current_analysis.get_cuts())
t2.SetName('T2')
t2.Write()


print 'Reducing T3...'
t3 = current_analysis.get_T3().copy_tree(current_analysis.get_cuts())
t3.SetName('T3')
t3.Write()



f_reduced.close()


