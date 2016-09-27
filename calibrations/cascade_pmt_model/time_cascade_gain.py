#!/usr/bin/python
import pickle
#print pickle.Pickler.dispatch
import dill
#print pickle.Pickler.dispatch

import ROOT as root
import sys, os

import matplotlib
matplotlib.use('QT4Agg')
import matplotlib.pyplot as plt
import matplotlib.dates as mdates
import matplotlib.patches as mpatches

from rootpy.plotting import root2matplotlib as rplt

import emcee, corner, click
import neriX_analysis, neriX_datasets
from rootpy.plotting import Hist2D, Hist, Legend, Canvas
from rootpy.tree import Tree
import numpy as np
import tqdm, time, copy_reg, types, pickle, datetime
from root_numpy import tree2array, array2tree
import scipy.optimize as op
import scipy.special
import scipy.misc

import led_datasets


num_walkers = 64
num_steps_to_include = 500
num_dim = 11
num_events = num_walkers*num_steps_to_include

index_mean = 0
index_width = 1
l_percentiles = [16., 50., 84.]

l_files = led_datasets.l_files

s_time_dependence_plots_dir = './time_dependence/'
if not os.path.exists(s_time_dependence_plots_dir):
    os.makedirs(s_time_dependence_plots_dir)

# get list of everything in directory
l_dir_names = os.listdir('./plots/')
l_dir_names.sort()

# select only names that start with nerix
l_filenames = []
for dir_name in l_dir_names:
    if dir_name[:3] == 'ner':
        l_filenames.append(dir_name)

d_spe_gain = {}
d_spe_gain[('nerix_160407_1533', 'nerix_160407_1550')] = {'gain':9.63e5,
                                                          'gain_unc':1.93e5,
                                                          'stdev':5.56e5,
                                                          'stdev_unc':6.92e4
                                                          }
d_spe_gain[('nerix_160411_1118', 'nerix_160411_1127')] = {'gain':9.09e5,
                                                          'gain_unc':8.99e4,
                                                          'stdev':5.39e5,
                                                          'stdev_unc':5.14e4
                                                          }
d_spe_gain[('nerix_160414_1006', 'nerix_160414_1015')] = {'gain':8.96e5,
                                                          'gain_unc':5.51e4,
                                                          'stdev':5.33e5,
                                                          'stdev_unc':2.74e4
                                                          }
d_spe_gain[('nerix_160418_1523', 'nerix_160418_1531')] = {'gain':9.27e5,
                                                          'gain_unc':1.48e5,
                                                          'stdev':5.46e5,
                                                          'stdev_unc':8.55e4
                                                          }
d_spe_gain[('nerix_160421_1521', 'nerix_160421_1528')] = {'gain':9.47e5,
                                                          'gain_unc':5.26e4,
                                                          'stdev':5.57e5,
                                                          'stdev_unc':2.35e4
                                                          }
d_spe_gain[('nerix_160425_1529', 'nerix_160425_1537')] = {'gain':9.61e5,
                                                          'gain_unc':9.26e4,
                                                          'stdev':5.64e5,
                                                          'stdev_unc':5.19e4
                                                          }
d_spe_gain[('nerix_160428_1008', 'nerix_160428_1016')] = {'gain':9.33e5,
                                                          'gain_unc':4.76e4,
                                                          'stdev':5.50e5,
                                                          'stdev_unc':2.31e4
                                                          }
d_spe_gain[('nerix_160502_1650', 'nerix_160502_1658')] = {'gain':9.31e5,
                                                          'gain_unc':1.17e5,
                                                          'stdev':5.48e5,
                                                          'stdev_unc':6.71e4
                                                          }
d_spe_gain[('nerix_160505_1120', 'nerix_160505_1128')] = {'gain':9.29e5,
                                                          'gain_unc':5.45e4,
                                                          'stdev':5.47e5,
                                                          'stdev_unc':2.68e4
                                                          }
d_spe_gain[('nerix_160509_0946', 'nerix_160509_0953')] = {'gain':9.22e5,
                                                          'gain_unc':1.05e5,
                                                          'stdev':5.43e5,
                                                          'stdev_unc':5.66e4
                                                          }
d_spe_gain[('nerix_160512_1155', 'nerix_160512_1202')] = {'gain':9.60e5,
                                                          'gain_unc':1.85e5,
                                                          'stdev':5.56e5,
                                                          'stdev_unc':8.40e4
                                                          }
d_spe_gain[('nerix_160516_1126', 'nerix_160516_1136')] = {'gain':8.91e5,
                                                          'gain_unc':8.87e4,
                                                          'stdev':5.29e5,
                                                          'stdev_unc':5.10e4
                                                          }
d_spe_gain[('nerix_160519_0952', 'nerix_160519_1007')] = {'gain':9.34e5,
                                                          'gain_unc':3.00e4,
                                                          'stdev':5.51e5,
                                                          'stdev_unc':1.37e4
                                                          }
d_spe_gain[('nerix_160523_0956', 'nerix_160523_1003')] = {'gain':9.18e5,
                                                          'gain_unc':9.23e4,
                                                          'stdev':5.43e5,
                                                          'stdev_unc':5.26e4
                                                          }
d_spe_gain[('nerix_160525_1217', 'nerix_160525_1225')] = {'gain':9.66e5,
                                                          'gain_unc':4.21e4,
                                                          'stdev':5.66e5,
                                                          'stdev_unc':1.97e4
                                                          }
d_spe_gain[('nerix_160531_0936', 'nerix_160531_0945')] = {'gain':9.32e5,
                                                          'gain_unc':4.80e4,
                                                          'stdev':5.51e5,
                                                          'stdev_unc':2.34e4
                                                          }
d_spe_gain[('nerix_160604_1241', 'nerix_160604_1248')] = {'gain':9.42e5,
                                                          'gain_unc':5.75e4,
                                                          'stdev':5.56e5,
                                                          'stdev_unc':2.81e4
                                                          }
d_spe_gain[('nerix_160606_0941', 'nerix_160606_0950')] = {'gain':9.88e5,
                                                          'gain_unc':3.43e4,
                                                          'stdev':5.78e5,
                                                          'stdev_unc':1.53e4
                                                          }
d_spe_gain[('nerix_160609_0956', 'nerix_160609_1005')] = {'gain':9.54e5,
                                                          'gain_unc':9.53e4,
                                                          'stdev':5.60e5,
                                                          'stdev_unc':5.33e4
                                                          }
d_spe_gain[('nerix_160614_0956', 'nerix_160614_1004')] = {'gain':9.63e5,
                                                          'gain_unc':9.98e4,
                                                          'stdev':5.64e5,
                                                          'stdev_unc':4.99e4
                                                          }
d_spe_gain[('nerix_160617_0946', 'nerix_160617_0953')] = {'gain':9.42e5,
                                                          'gain_unc':9.22e4,
                                                          'stdev':5.54e5,
                                                          'stdev_unc':5.24e4
                                                          }
d_spe_gain[('nerix_160620_0945', 'nerix_160620_0954')] = {'gain':9.64e5,
                                                          'gain_unc':6.55e4,
                                                          'stdev':5.65e5,
                                                          'stdev_unc':2.48e4
                                                          }
d_spe_gain[('nerix_160623_1044', 'nerix_160623_1052')] = {'gain':9.69e5,
                                                          'gain_unc':6.45e4,
                                                          'stdev':5.67e5,
                                                          'stdev_unc':2.81e4
                                                          }
d_spe_gain[('nerix_160627_0944', 'nerix_160627_0952')] = {'gain':9.66e5,
                                                          'gain_unc':1.14e5,
                                                          'stdev':5.64e5,
                                                          'stdev_unc':4.49e4
                                                          }
d_spe_gain[('nerix_160630_0934', 'nerix_160630_0942')] = {'gain':1.06e6,
                                                          'gain_unc':6.67e4,
                                                          'stdev':6.07e5,
                                                          'stdev_unc':2.78e4
                                                          }
d_spe_gain[('nerix_160705_0945', 'nerix_160705_0955')] = {'gain':9.60e5,
                                                          'gain_unc':5.05e4,
                                                          'stdev':5.64e5,
                                                          'stdev_unc':2.31e4
                                                          }
d_spe_gain[('nerix_160711_0808', 'nerix_160711_0816')] = {'gain':9.53e5,
                                                          'gain_unc':1.26e5,
                                                          'stdev':5.58e5,
                                                          'stdev_unc':5.13e4
                                                          }
d_spe_gain[('nerix_160715_0939', 'nerix_160715_0948')] = {'gain':9.40e5,
                                                          'gain_unc':4.89e4,
                                                          'stdev':5.54e5,
                                                          'stdev_unc':2.34e4
                                                          }
d_spe_gain[('nerix_160718_0929', 'nerix_160718_0936')] = {'gain':9.74e5,
                                                          'gain_unc':9.88e4,
                                                          'stdev':5.69e5,
                                                          'stdev_unc':3.78e4
                                                          }
d_spe_gain[('nerix_160722_0933', 'nerix_160722_0940')] = {'gain':9.40e5,
                                                          'gain_unc':7.71e4,
                                                          'stdev':5.54e5,
                                                          'stdev_unc':3.48e4
                                                          }
d_spe_gain[('nerix_160725_0936', 'nerix_160725_0944')] = {'gain':9.33e5,
                                                          'gain_unc':4.80e4,
                                                          'stdev':5.51e5,
                                                          'stdev_unc':2.34e4
                                                          }
d_spe_gain[('nerix_160728_0938', 'nerix_160728_0947')] = {'gain':9.26e5,
                                                          'gain_unc':6.17e4,
                                                          'stdev':5.47e5,
                                                          'stdev_unc':3.03e4
                                                          }
d_spe_gain[('nerix_160801_0933', 'nerix_160801_0940')] = {'gain':9.65e5,
                                                          'gain_unc':4.62e4,
                                                          'stdev':5.66e5,
                                                          'stdev_unc':2.19e4
                                                          }
d_spe_gain[('nerix_160808_1038', 'nerix_160808_1045')] = {'gain':9.63e5,
                                                          'gain_unc':1.06e5,
                                                          'stdev':5.64e5,
                                                          'stdev_unc':5.63e4
                                                          }

l_spe_gain_mean = []
l_spe_gain_mean_unc = []
l_spe_gain_stdev = []
l_spe_gain_stdev_unc = []
l_time = []

for l_filenames in l_files:

    l_time.append(neriX_analysis.convert_name_to_unix_time(l_filenames[0]))
    l_spe_gain_mean.append(d_spe_gain[l_filenames]['gain'])
    l_spe_gain_mean_unc.append(d_spe_gain[l_filenames]['gain_unc'])
    l_spe_gain_stdev.append(d_spe_gain[l_filenames]['stdev'])
    l_spe_gain_stdev_unc.append(d_spe_gain[l_filenames]['stdev_unc'])
    
a_spe_gain_mean = np.asarray(l_spe_gain_mean)
a_spe_gain_mean_unc = np.asarray(l_spe_gain_mean_unc)
a_spe_gain_stdev = np.asarray(l_spe_gain_stdev)
a_spe_gain_stdev_unc = np.asarray(l_spe_gain_stdev_unc)


for i, unixtime in enumerate(l_time):
    l_time[i] = datetime.datetime.fromtimestamp(unixtime)


# likelihood function should be the integral
# of the integral of two gaussians sharing one variable
# the first gaussian should be the trial gaussian
# the second gaussian should be the point with statistical uncertainty


def ln_likelihood_gaussian_uncertainty_on_pts(a_parameters, a_means, a_stds):
    fit_mean, fit_std = a_parameters
    
    if fit_std < 0:
        return -np.inf


    exponential_term = -(a_means-fit_mean)**2 / (a_stds**2 + fit_std**2) / 2.
    return np.sum(exponential_term - np.log(a_stds) - np.log(fit_std) - 1./2.*np.log(1./a_stds**2+1./fit_std**2) - 1./2.*np.log(2*np.pi))




"""
def neg_log_likelihood_means_diff_ev(a_guesses):
    return -ln_likelihood_gaussian_uncertainty_on_pts(l_gas_gain_mean_median, l_gas_gain_mean_mad, *a_guesses)

def neg_log_likelihood_widths_diff_ev(a_guesses):
    return -ln_likelihood_gaussian_uncertainty_on_pts(l_gas_gain_width_median, l_gas_gain_width_mad, *a_guesses)

a_bounds = [(10, 20), (1e-3, 5)]
result_mean = op.differential_evolution(neg_log_likelihood_means_diff_ev, a_bounds, disp=True, maxiter=100, tol=0.01, polish=True)
print result_mean
"""


num_steps = 2000
steps_to_keep = 1000*num_walkers

l_guesses_mean = [9.5e5, 8e4]
l_stds_mean = [1e5, 2e4]
l_labels_mean = ['mean_of_spe_mean', 'std_of_spe_mean']
a_starting_pos_mean = emcee.utils.sample_ball(l_guesses_mean, l_stds_mean, size=num_walkers)

l_guesses_width = [5e5, 5e4]
l_stds_width = [5e4, 1e4]
l_labels_width = ['mean_of_spe_width', 'std_of_spe_width']
a_starting_pos_width = emcee.utils.sample_ball(l_guesses_width, l_stds_width, size=num_walkers)


sampler_spe_gain_mean = emcee.DESampler(num_walkers, 2, ln_likelihood_gaussian_uncertainty_on_pts, threads=1, autoscale_gamma=True, args=[a_spe_gain_mean, a_spe_gain_mean_unc])

with click.progressbar(sampler_spe_gain_mean.sample(a_starting_pos_mean, iterations=num_steps, ), length=num_steps) as mcmc_sampler:
        for pos, lnprob, state in mcmc_sampler:
            pass

a_samples_spe_mean = sampler_spe_gain_mean.chain[:, :, :2].reshape((-1, 2))
a_samples_spe_mean = a_samples_spe_mean[-steps_to_keep:, :]

# extract final values
mean_for_spe_mean, stdev_for_spe_mean = np.percentile(a_samples_spe_mean, 50., axis=0)


sampler_spe_gain_width = emcee.DESampler(num_walkers, 2, ln_likelihood_gaussian_uncertainty_on_pts, threads=1, autoscale_gamma=True, args=[a_spe_gain_stdev, a_spe_gain_stdev_unc])

with click.progressbar(sampler_spe_gain_width.sample(a_starting_pos_width, iterations=num_steps, ), length=num_steps) as mcmc_sampler:
        for pos, lnprob, state in mcmc_sampler:
            pass

a_samples_spe_width = sampler_spe_gain_width.chain[:, :, :2].reshape((-1, 2))
a_samples_spe_width = a_samples_spe_width[-steps_to_keep:, :]

# extract final values
mean_for_spe_width, stdev_for_spe_width = np.percentile(a_samples_spe_width, 50., axis=0)

#print mean_for_spe_mean, stdev_for_spe_mean
#print mean_for_spe_width, stdev_for_spe_width


f_corner_mean = corner.corner(a_samples_spe_mean, labels=l_labels_mean, quantiles=[0.16, 0.5, 0.84], show_titles=True, title_fmt='.3e', title_kwargs={"fontsize": 9})
f_corner_mean.savefig('%sposterior_spe_mean.png' % (s_time_dependence_plots_dir))

f_corner_width = corner.corner(a_samples_spe_width, labels=l_labels_width, quantiles=[0.16, 0.5, 0.84], show_titles=True, title_fmt='.3e', title_kwargs={"fontsize": 9})
f_corner_width.savefig('%sposterior_spe_width.png' % (s_time_dependence_plots_dir))





fig1, ax1 = plt.subplots(1)
fig2, ax2 = plt.subplots(1)
points_for_lines = 50

start_time = l_time[0] - datetime.timedelta(days=1)
stop_time = l_time[-1] + datetime.timedelta(days=1)
start_time = mdates.date2num(start_time)
stop_time = mdates.date2num(stop_time)



ax1.errorbar(l_time, a_spe_gain_mean, yerr=a_spe_gain_mean_unc, color='b', marker='o', linestyle='')

x_line = np.linspace(start_time, stop_time, points_for_lines)
x_line = mdates.num2date(x_line)
y_mean_for_spe_mean_line = np.asarray([mean_for_spe_mean for i in xrange(points_for_lines)])
y_mean_plus_one_sigma_for_spe_mean_line = np.asarray([mean_for_spe_mean+stdev_for_spe_mean for i in xrange(points_for_lines)])
y_mean_minus_one_sigma_for_spe_mean_line = np.asarray([mean_for_spe_mean-stdev_for_spe_mean for i in xrange(points_for_lines)])

ax1.plot(x_line, y_mean_for_spe_mean_line, color='g', linestyle='--')
ax1.fill_between(x_line, y_mean_minus_one_sigma_for_spe_mean_line, y_mean_plus_one_sigma_for_spe_mean_line, color='g', alpha=0.2)


ax2.errorbar(l_time, a_spe_gain_stdev, yerr=a_spe_gain_stdev_unc, color='b', marker='o', linestyle='')

x_line = np.linspace(start_time, stop_time, points_for_lines)
x_line = mdates.num2date(x_line)
y_mean_for_spe_width_line = np.asarray([mean_for_spe_width for i in xrange(points_for_lines)])
y_mean_plus_one_sigma_for_spe_width_line = np.asarray([mean_for_spe_width+stdev_for_spe_width for i in xrange(points_for_lines)])
y_mean_minus_one_sigma_for_spe_width_line = np.asarray([mean_for_spe_width-stdev_for_spe_width for i in xrange(points_for_lines)])

ax2.plot(x_line, y_mean_for_spe_width_line, color='g', linestyle='--')
ax2.fill_between(x_line, y_mean_minus_one_sigma_for_spe_width_line, y_mean_plus_one_sigma_for_spe_width_line, color='g', alpha=0.2)


s_spe_gain_mean = r'SPE Mean = %.2e +/- %.2e [$e^{-}$]' % (mean_for_spe_mean, stdev_for_spe_mean)
s_spe_gain_width = r'SPE Width = %.2e +/- %.2e [$e^{-}$]' % (mean_for_spe_width, stdev_for_spe_width)

ax1.text(0.7, 0.9, s_spe_gain_mean, ha='center', va='center', transform=ax1.transAxes)
ax2.text(0.7, 0.9, s_spe_gain_width, ha='center', va='center', transform=ax2.transAxes)

ax1.set_title('SPE Gain Mean - Run 16 Average')
ax1.set_ylabel(r'SPE Gain Mean [$e^{-}$]', labelpad=-5)
fig1.autofmt_xdate()

ax2.set_title('SPE Gain Width - Run 16 Average')
ax2.set_ylabel(r'SPE Gain Width [$e^{-}$]')
fig2.autofmt_xdate()


fig1.savefig('%ssummary_spe_mean.png' % (s_time_dependence_plots_dir))
fig2.savefig('%ssummary_spe_width.png' % (s_time_dependence_plots_dir))
#plt.show()
