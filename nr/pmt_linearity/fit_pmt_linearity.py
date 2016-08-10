#!/usr/bin/python

import ROOT as root
import sys
import neriX_analysis
from rootpy.plotting import Hist2D, Hist, Legend, Canvas
import math, root_numpy
import numpy as np





#--------------- Start Parameters to Change ----------------


s1NumBins = 40
s1Min = 0
s1Max = 30
s1FitMin = 4
s1FitMax = 15

s2NumBins = 40
s2Min = 0
s2Max = 3000

asym_num_bins = 40
asym_low = -1
asym_high = 1

log_num_bins = 200
log_low = 1
log_high = 4.5

z_num_bins = 40
z_low = -26
z_high = 0

width_num_bins = 40
width_low = 10
width_high = 150

s1Branch = 'cpS1sTotBottom[0]'
s2Branch = 'cpS2sTotBottom[0]'
s1_asym_branch = 's1asym' #'(%s - S1sTotTop[0])/(%s + S1sTotTop[0])' % (s1Branch, s1Branch)
s2_asym_branch = 's2asym' #'(%s - S2sTotTop[0])/(%s + S2sTotTop[0])' % (s2Branch, s2Branch)

z_branch = 'Z_any_peak'
width_branch = 'S2sWidth[0]'



#--------------- End Parameters to Change ----------------


d_g1_g2_fits = {}


# V5=485V
# correction = 0.35
# means = [0.13333501511730866, 21.69134203851344]
# cov = [[4.1423754493204272e-06, 0.0006542021582231131], [0.0006542021582231131, 0.10929706670508231]]
d_g1_g2_fits[485] = {'correction':0.35,
					 'multivariate_means':[0.13333501511730866, 21.69134203851344],
					 'multivariate_cov_matrix':[[4.1423754493204272e-06, 0.0006542021582231131], [0.0006542021582231131, 0.10929706670508231]]
					 }


# V5=500V
# correction = 0.5
# means = [0.13472348456205535, 21.719611296311832]
# cov = [[3.8844606466408181e-06, 0.00061062588697061854], [0.00061062588697061854, 0.10168065424826875]]
d_g1_g2_fits[500] = {'correction':0.5,
					 'multivariate_means':[0.13472348456205535, 21.719611296311832],
					 'multivariate_cov_matrix':[[3.8844606466408181e-06, 0.00061062588697061854], [0.00061062588697061854, 0.10168065424826875]]
					 }


# V5=515V
# correction = 0.7
# means = [0.13512817052790824, 22.050785738393504]
# cov = [[3.8264283893192635e-06, 0.00061570423949012533], [0.00061570423949012533, 0.10325399981098432]]
d_g1_g2_fits[515] = {'correction':0.7,
					 'multivariate_means':[0.13512817052790824, 22.050785738393504],
					 'multivariate_cov_matrix':[[3.8264283893192635e-06, 0.00061570423949012533], [0.00061570423949012533, 0.10325399981098432]]
					 }


# V5=530V
# correction = 1.0
# means = [0.13275184037856666, 21.429882023438761]
# cov = [[3.8408409389177058e-06, 0.00060752696201326847], [0.00060752696201326847, 0.10005434630499116]]
d_g1_g2_fits[530] = {'correction':1.0,
					 'multivariate_means':[0.13275184037856666, 21.429882023438761],
					 'multivariate_cov_matrix':[[3.8264283893192635e-06, 0.00061570423949012533], [0.00061570423949012533, 0.10325399981098432]]
					 }



mean_s2 = 5.5e5
mean_s2_uncertainty = 0.1*mean_s2

gain_used_in_processing = 1.18e4

l_voltages = sorted(d_g1_g2_fits.keys())

a_charge = np.zeros(len(l_voltages), dtype=np.float32)
a_g2_over_g1 = np.zeros(len(l_voltages), dtype=np.float32)
a_charge_uncertainty = np.zeros(len(l_voltages), dtype=np.float32)
a_g2_over_g1_uncertainty = np.zeros(len(l_voltages), dtype=np.float32)

for i, voltage in enumerate(l_voltages):
	
	g1 = d_g1_g2_fits[voltage]['multivariate_means'][0]
	g2 = d_g1_g2_fits[voltage]['multivariate_means'][1]
	
	d_g1_g2_fits[voltage]['g2_over_g1'] = g2 / g1
	
	g1_variance = d_g1_g2_fits[voltage]['multivariate_cov_matrix'][0][0]
	g2_variance = d_g1_g2_fits[voltage]['multivariate_cov_matrix'][1][1]
	g1_g2_covariance = d_g1_g2_fits[voltage]['multivariate_cov_matrix'][0][1]

	d_g1_g2_fits[voltage]['g2_over_g1_uncertainty'] = d_g1_g2_fits[voltage]['g2_over_g1'] * ( (g2_variance/g2**2) + (g1_variance/g1**2) - 2*(g1_g2_covariance/(g1*g2)) )**0.5


	# fill arrays
	a_charge[i] = mean_s2*gain_used_in_processing*d_g1_g2_fits[voltage]['correction']
	a_charge_uncertainty[i] = mean_s2_uncertainty*gain_used_in_processing*d_g1_g2_fits[voltage]['correction']
	a_g2_over_g1[i] = d_g1_g2_fits[voltage]['g2_over_g1']
	a_g2_over_g1_uncertainty[i] = d_g1_g2_fits[voltage]['g2_over_g1_uncertainty']


print a_charge
print a_charge_uncertainty
print a_g2_over_g1
print a_g2_over_g1_uncertainty


lower_bound, upper_bound = min(a_charge)*0.8, max(a_charge)*1.2


g_linearity = root.TGraphErrors(len(l_voltages), a_charge, a_g2_over_g1, a_charge_uncertainty, a_g2_over_g1_uncertainty)
g_linearity.SetTitle('PMT 5 Linearity;Charge [e-];#frac{g2}{g1};')
g_linearity.GetXaxis().SetRangeUser(lower_bound, upper_bound)

c1 = Canvas()
g_linearity.Draw('ap')

f_linearity = root.TF1('f_linearity', 'pol0', lower_bound, upper_bound)
frp_linearity = g_linearity.Fit('f_linearity', 'S')

a_fit_pars = np.asarray([f_linearity.GetParameter(0)])
a_cov_matrix = np.asarray(root_numpy.matrix(frp_linearity.GetCovarianceMatrix()))

def pyfunc_const(x, par0):
	return par0


g_conf_band = neriX_analysis.create_1d_fit_confidence_band(pyfunc_const, a_fit_pars, a_cov_matrix, lower_bound, upper_bound)
g_conf_band.Draw('3 same')


c1.Update()

raw_input('Press enter to continue...')


neriX_analysis.save_plot(['results'], c1, 'pmt_5_linearity')




