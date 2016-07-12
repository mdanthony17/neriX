#!/usr/bin/python
import matplotlib
matplotlib.use('QT4Agg')
import ROOT as root
from ROOT import gROOT
import sys, os, root_numpy, threading, random, emcee, corner, signal, re
import neriX_analysis
from rootpy import stl
from rootpy.io import File
from rootpy.tree import Tree, TreeModel, TreeChain
from rootpy.plotting import Hist, Hist2D, Canvas, Graph, func
import numpy as np
from math import exp, factorial, erf, ceil, log, pow, floor
from scipy import optimize, misc, stats
from scipy.stats import norm
import copy_reg, types, pickle, click, time
from subprocess import call
from tqdm import tqdm

with open('./sim_inputs.txt', 'r') as f_sim_input:
	s_sim_input = f_sim_input.read()

#print s_sim_input

# find all matches to the reg exp in file
r_exp = re.compile('S1\s\d+.\d+')
l_matches_found = re.findall(r_exp, s_sim_input)
#print l_matches_found


# output is in the form of "S1\t%f"
# where %f is a float
# can extract float by starting at 3rd index
a_sim_inputs = np.zeros(len(l_matches_found))
for i in xrange(len(l_matches_found)):
	a_sim_inputs[i] = float(l_matches_found[i][3:])
#print a_sim_inputs


# fill histograms as appropriate
s1_settings = [30, 0, 15]
gain_adjustment_term = 1.4e6/8.5e5
s1_branch = 'S1sTotBottom[0]*%f' % gain_adjustment_term

# adjust S1 inputs for appropriate gain
a_sim_inputs *= gain_adjustment_term

h_sim_inputs = Hist(*s1_settings, name='h_sim_inputs')
h_sim_inputs.fill_array(a_sim_inputs)

#h_processed_outputs = Hist(*s1_settings, name='h_processed_outputs')
#h_processed_outputs.SetColor(root.kRed)

h_pf_found = Hist(*s1_settings, name='h_pf_found')
h_pf_found.SetColor(root.kBlue)

f_processed_outputs = File('./nerix_111111_1111.root', 'r')
t1 = f_processed_outputs.T1
t2 = f_processed_outputs.T2
t2.AddFriend(t1)

#t2.Scan('EventId:S1sTotBottom[0]:S1sPeak[0]')

a_processed_outputs = root_numpy.tree2array(tree=t2, branches=['EventId', 'S1sTotBottom', 'S1sPeak'])

assert len(a_processed_outputs) == len(a_sim_inputs)

a_distance_from_actual = np.zeros(len(a_processed_outputs))

for i in tqdm(xrange(len(a_distance_from_actual))):
	input_value = a_sim_inputs[i]
	
	l_current_output = a_processed_outputs[i]
	a_s1 = l_current_output[1]
	s1_peak = l_current_output[2]
	if len(a_s1) == 0 or (s1_peak < 898 or s1_peak > 902):
		continue
	else:
		h_pf_found.Fill(input_value)




#t2.Draw(s1_branch, hist=h_processed_outputs, selection='%s > 0.005' % (s1_branch))

c1 = Canvas()
#h_pf_found.Draw()
#h_sim_inputs.Draw('same')

h_pf_efficiency = root.TEfficiency(h_pf_found, h_sim_inputs)
h_pf_efficiency.SetTitle('S1 Peak Finder Efficiency; Simulated PE [PE]; Percentage Found by Peak Finder');

g_pf_efficiency = h_pf_efficiency.CreateGraph()
g_pf_efficiency.GetXaxis().SetRangeUser(s1_settings[1], s1_settings[2])
g_pf_efficiency.Draw('AP')

f_eff = root.TF1('f_eff', '(1. - exp(-(x-[0])/[1]))', 1.5, 15)
f_eff.SetParameters(2., 2.5)
frp_eff = g_pf_efficiency.Fit('f_eff', 'SNRLL')
#f_eff.Draw('same')

a_fit_pars = np.asarray([f_eff.GetParameter(0), f_eff.GetParameter(1)])
a_cov_matrix = np.asarray(root_numpy.matrix(frp_eff.GetCovarianceMatrix()))
def pyfunc_eff(x, center, shape):
	if x < center:
		return 0
	else:
		return 1. - exp(-(x-center)/shape)
g_eff_conf_band = neriX_analysis.create_1d_fit_confidence_band(pyfunc_eff, a_fit_pars, a_cov_matrix, s1_settings[1], s1_settings[2])
g_eff_conf_band.Draw('3 same')


c1.Update()

raw_input('Press enter to continue...')

f_processed_outputs.Close()