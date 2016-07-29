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

l_filenames = ['nerix_160715_1527']
#l_filenames = ['nerix_160715_1527', 'nerix_160716_1245', 'nerix_160717_1438', 'nerix_160722_1759']


current_analysis = neriX_analysis.neriX_analysis(l_filenames, -50, 1.054, 4.5)

lowTimeDiff = 50
highTimeDiff = 100



#--------------- Start Parameters to Change ----------------

na_height_branch = 'NaiHeight[0]'
l_na_height_settings = [50, 0.105, 0.125]
na_height_guess = 0.115

mod_dt_branch = '(S2sPeak[0] - NaiPeak[0])'
dt_lb = 200.
dt_ub = 1800.

mod_dt_offset_branch = '(NaiPeak[0] - S1sPeak[0])'
l_mod_dt_offset_settings = [15, 180, 195]

neriX_analysis.warning_message('Hard-coded gain correction')
s2_branch = 'S2sTotBottom[0]*1.48e6/9e5'
l_s2_settings = [20, 0, 2000]

trig_branch = 'TrigLeftEdge-S2sLeftEdge[0]'
l_trig_settings = [50, 0, 150]
trig_lb = 40
trig_ub = 100

trial_width = 0.5
cut_width = 3.0

s1_branch = 'S1sTotBottom[0]*1.48e6/9e5'

#--------------- End Parameters to Change ----------------




c1 = Canvas()
c1.SetGridx()
c1.SetGridy()


h_na22_height = Hist(*l_na_height_settings, name='h_na22_height', title='Na22 Spectrum')
h_na22_height.SetMarkerSize(0)

current_analysis.Draw(na_height_branch, hist=h_na22_height)


f_na22_height_trial = root.TF1('f_na22_height_trial', 'gaus', l_na_height_settings[1], l_na_height_settings[2])
h_na22_height.Fit('f_na22_height_trial', 'LSQRN')
left_edge_from_trial = f_na22_height_trial.GetParameter(1) - trial_width*f_na22_height_trial.GetParameter(2)
right_edge_from_trial = f_na22_height_trial.GetParameter(1) + trial_width*f_na22_height_trial.GetParameter(2)

f_na22_height = root.TF1('f_na22_height', 'gaus', left_edge_from_trial, right_edge_from_trial)
h_na22_height.Fit('f_na22_height', 'LSR')

h_na22_height.Draw()
c1.Update()







# modified dt cut

c2 = Canvas()
c2.SetGridx()
c2.SetGridy()


h_mod_dt_offset = Hist(*l_mod_dt_offset_settings, name='h_mod_dt_offset', title='Modified dt Offset (NaI Peak - S1 Peak)')
h_mod_dt_offset.SetMarkerSize(0)

current_analysis.Draw(mod_dt_offset_branch, hist=h_mod_dt_offset)

f_mod_dt_offset = root.TF1('f_mod_dt_offset', 'gaus', l_mod_dt_offset_settings[1], l_mod_dt_offset_settings[2])
h_mod_dt_offset.Fit('f_mod_dt_offset', 'LSQR')

mean_offset = f_mod_dt_offset.GetParameter(1)

h_mod_dt_offset.Draw()
c2.Update()



s_na22_height_cut = '%s > %f && %s < %f' % (na_height_branch, f_na22_height.GetParameter(1) - cut_width*f_na22_height.GetParameter(2), na_height_branch, f_na22_height.GetParameter(1) + cut_width*f_na22_height.GetParameter(2))
s_no_large_s1 = 'S1sPeak[0] < 82 || S1sPeak[0] > 192'
s_mod_dt_cut = '%s > %f && %s < %f' % (mod_dt_branch, dt_lb-mean_offset, mod_dt_branch, dt_ub-mean_offset)



# draw S2 spectrum


c_s2_spectrum = Canvas()
c_s2_spectrum.SetGridx()
c_s2_spectrum.SetGridy()
c_s2_spectrum.SetLogy()


h_s2_spectrum = Hist(*l_s2_settings, name='h_s2_spectrum', title='Low Energy S2 Spectrum')
h_s2_spectrum.SetMarkerSize(0)
h_s2_spectrum.SetStats(0)
h_s2_spectrum.GetXaxis().SetTitle('S2 [PE]')
h_s2_spectrum.GetYaxis().SetTitle('Counts')

h_s2_spectrum_na22_cut = Hist(*l_s2_settings, name='h_s2_spectrum_na22_cut', title='Low Energy S2 Spectrum')
h_s2_spectrum_na22_cut.SetColor(root.kRed)
h_s2_spectrum_na22_cut.SetMarkerSize(0)
h_s2_spectrum_na22_cut.SetStats(0)
h_s2_spectrum_na22_cut.GetXaxis().SetTitle('S2 [PE]')
h_s2_spectrum_na22_cut.GetYaxis().SetTitle('Counts')

current_analysis.Draw(s2_branch, hist=h_s2_spectrum)
current_analysis.Draw(s2_branch, hist=h_s2_spectrum_na22_cut, selection='%s && %s && %s' % (s_na22_height_cut, s_no_large_s1, s_mod_dt_cut))

leg_s2_spectrum = Legend(2, leftmargin=0.3, textsize=0.02)
leg_s2_spectrum.AddEntry(h_s2_spectrum, label='Raw S2 Spectrum', style='LEP')
leg_s2_spectrum.AddEntry(h_s2_spectrum_na22_cut, label='S2 Spectrum after NaI coincidence cuts', style='LEP')


h_s2_spectrum.Draw()
h_s2_spectrum_na22_cut.Draw('same')
leg_s2_spectrum.Draw('same')
c_s2_spectrum.Update()






#current_analysis.add_cut(s_na22_height_cut)
#current_analysis.add_cut(s_mod_dt_cut)
#current_analysis.add_cut(s_no_large_s1)
#current_analysis.add_z_cut()
current_analysis.add_cut('%s > 0' % (s2_branch))
current_analysis.add_cut('%s < %f' % (s2_branch, l_s2_settings[2]))
current_analysis.add_cut('(%s < %f)' % (s1_branch, 20))
#current_analysis.add_cut('(%s < %f) || (%s < %f && (%s) > %f)' % (s1_branch, 20, s1_branch, 100, 'S2sPeak[0]-S1sPeak[0]', 250))
current_analysis.add_cut('S2sPeak[0] > 200 && S2sPeak[0] < 1800')

current_analysis.multithread_set_event_list(7)


#current_analysis.get_T1().Scan('EventId:S1sPeak[0]:S2sPeak[0]:NaiPeak[0]:%s:TrigLeftEdge' % s2_branch, current_analysis.get_cuts())


c4 = Canvas()
c4.SetGridx()
c4.SetGridy()


h_trig_time = Hist(*l_trig_settings, name='h_trig_time')
h_trig_time.SetMarkerSize(0)

current_analysis.Draw(trig_branch, hist=h_trig_time)

#f_mod_dt_offset = root.TF1('f_mod_dt_offset', 'gaus', l_mod_dt_offset_settings[1], l_mod_dt_offset_settings[2])
#h_mod_dt_offset.Fit('f_mod_dt_offset', 'LSQR')

h_trig_time.Draw()
c4.Update()




h_no_cut = Hist(*l_s2_settings, name='h_no_cut')
current_analysis.Draw(s2_branch, hist=h_no_cut, selection=current_analysis.get_cuts())

current_analysis.add_cut('%s > %f && %s < %f' % (trig_branch, trig_lb, trig_branch, trig_ub))
h_with_cut = Hist(*l_s2_settings, name='h_with_cut')
current_analysis.Draw(s2_branch, hist=h_with_cut, selection=current_analysis.get_cuts())

c3 = Canvas()
c3.SetGridx()
c3.SetGridy()

h_eff = root.TEfficiency(h_with_cut, h_no_cut)
h_eff.SetTitle('S2 Trigger Efficiency from Gas Gain Calibration; S2 Size [PE]; Percentage Causing Trigger');

g_eff = h_eff.CreateGraph()
#g_eff = neriX_analysis.convert_hist_to_graph_with_poisson_errors(h_eff)
g_eff.GetXaxis().SetRangeUser(l_s2_settings[1], l_s2_settings[2])
g_eff.Draw('AP')

"""
f_eff = root.TF1('f_eff', 'exp(-[0]*exp(-x*[1]))', l_s2_settings[1], l_s2_settings[2])
f_eff.SetParameters(7., 0.0168)
f_eff.SetParLimits(0, 0, 10)
f_eff.SetParLimits(1, 1e-3, 1e-1)
"""

"""
f_eff = root.TF1('f_eff', '1. / (1. + exp(-(x-[0])/[1]))', l_s2_settings[1], l_s2_settings[2])
f_eff.SetParameters(300., 300.)
f_eff.SetParLimits(0, 1e2, 1e3)
f_eff.SetParLimits(1, 1e2, 1e3)
"""

f_eff = root.TF1('f_eff', '(1. - exp(-(x-[0])/[1])) * (x > [0] ? 1. : 0.)', l_s2_settings[1], l_s2_settings[2])
f_eff.SetParameters(100., 300.)
f_eff.SetParLimits(0, 50, 500)
f_eff.SetParLimits(1, 10, 1000)


frp_eff = g_eff.Fit('f_eff', 'SNRMELL')
f_eff.Draw('same')


a_fit_pars = np.asarray([f_eff.GetParameter(0), f_eff.GetParameter(1)])
a_cov_matrix = np.asarray(root_numpy.matrix(frp_eff.GetCovarianceMatrix()))
print a_fit_pars
print a_cov_matrix



def pyfunc_eff(x, center, shape):
	if x < center:
		return 0
	else:
		return 1. - exp(-(x-center)/shape)


g_conf_band = neriX_analysis.create_1d_fit_confidence_band(pyfunc_eff, a_fit_pars, a_cov_matrix, l_s2_settings[1], l_s2_settings[2])
g_conf_band.Draw('3 same')



c3.Update()


raw_input('Press enter to continue...')





