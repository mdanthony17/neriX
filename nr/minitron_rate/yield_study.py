#!/usr/bin/python
import sys, array, os
sys.path.insert(0, '..')

import matplotlib
matplotlib.use('QT4Agg')
import matplotlib.pyplot as plt
import matplotlib.mlab as mlab
import matplotlib.colors as colors

matplotlib.rcParams['mathtext.fontset'] = 'stix'
matplotlib.rcParams['font.family'] = 'STIXGeneral'
matplotlib.rcParams['font.size'] = 16


import ROOT as root
import neriX_analysis
from rootpy.plotting import Hist, Hist2D, Canvas, Legend
import numpy as np
import cPickle as pickle
import tqdm

import emcee, click

from scipy.interpolate import spline

from scipy.stats import multivariate_normal, norm

transparency = 0.3
lb_percentage = 0.95
ub_percentage = 1.03

lb_np_rate = 0.2
ub_np_rate = 3

bc_color = '#3cb44b'
hv_color = '#0082c8'

s_bc_label = 'Beam Current [$\mu$A]'
s_hv_label = 'Minitron High Voltage [kV]'

flux_constant = 2.9e4 / 6000. / 1.6

a_hv_x_low = np.asarray([49.044, 54.110, 59.222, 64.241, 69.273, 74.273, 79.264, 84.296])
a_hv_x_high = np.asarray([49.118, 54.060, 59.218, 64.190, 69.227, 74.246, 79.274, 84.347])

a_hv_y_low = np.asarray([0.228, 0.365, 0.411, 0.578, 0.703, 1.040, 1.150, 1.553])
a_hv_y_high = np.asarray([0.320, 0.495, 0.555, 0.766, 0.923, 1.368, 1.511, 2.002])

a_hv_x_mean = (a_hv_x_low + a_hv_x_high) / 2.
a_hv_x_width = np.asarray([0.00001 for i in xrange(len(a_hv_x_mean))])
a_hv_y_mean = (a_hv_y_high + a_hv_y_low) / 2.
a_hv_y_width = (a_hv_y_high - a_hv_y_low) / 2.



a_bc_x_mean = np.asarray([33.449, 34.112, 40.361, 41.291, 51.132, 51.132, 60.616, 63.922, 71.462, 72.271, 82.816, 87.531])
a_bc_y_mean = np.asarray([0.449, 0.517, 0.534, 0.584, 0.696, 0.712, 0.808, 0.801, 0.968, 0.898, 0.919, 1.144])
a_bc_x_width = np.asarray([33.336, 33.964, 39.829, 40.385, 50.727, 49.982, 59.265, 58.745, 69.982, 72.068, 82.686, 86.034])
a_bc_x_width = a_bc_x_mean - a_bc_x_width
a_bc_y_width = np.asarray([0.514, 0.591, 0.612, 0.668, 0.793, 0.809, 0.924, 0.909, 1.095, 1.015, 1.043, 1.293])
a_bc_y_width = a_bc_y_width - a_bc_y_mean


num_sim_points = 200


def ln_likelihood_x_and_y_err(a_x1, a_y1, a_x2, a_y2, a_x2_err, a_y2_err):
    ll_sum = 0
    num_points = len(a_x2)
    
    #print multivariate_normal.logpdf([a_x1, a_y1], [a_x2[0], a_y2[0]], [[a_x2_err[0]**2, 0], [0, a_y2_err[0]**2]])

    for i in xrange(num_points):
        for j in xrange(num_sim_points):
            print [[a_x2_err[i]**2, 0], [0, a_y2_err[i]**2]]
            #print multivariate_normal.logpdf([a_x1[j], a_y1[j]], mean=[a_x2[i], a_y2[i]], cov=[[a_x2_err[i]**2, 0], [0, a_y2_err[i]**2]])
            ll_sum += multivariate_normal.logpdf([a_x1[j], a_y1[j]], mean=[a_x2[i], a_y2[i]], cov=[[a_x2_err[i]**2, 0], [0, a_y2_err[i]**2]])

    return ll_sum



def ln_likelihood_y(a_y1, a_y2, a_y2_err):
    return np.sum(norm.logpdf(a_y1, a_y2, a_y2_err))


def fit_linear(a_pars, a_x2, a_y2, a_x2_err, a_y2_err):
    slope = a_pars[0]
    #print slope

    low_x = 0.8*np.min(a_x2)
    high_x = 1.2*np.max(a_x2)

    #a_x_line = np.linspace(low_x, high_x, num_sim_points)
    a_y_line = a_x2*slope
    #print a_x_line
    #print a_y_line

    return ln_likelihood_y(a_y_line, a_y2, a_y2_err)


def fit_exp(a_pars, a_x2, a_y2, a_x2_err, a_y2_err):
    const, voltage_pow = a_pars
    #print slope

    low_x = 0.8*np.min(a_x2)
    high_x = 1.2*np.max(a_x2)

    #a_x_line = np.linspace(low_x, high_x, num_sim_points)
    a_y_line = const*(np.exp(a_x2/voltage_pow) - 1)
    #print a_x_line
    #print a_y_line

    return ln_likelihood_y(a_y_line, a_y2, a_y2_err)


low_x_hv = 45
high_x_hv = 100
a_hv_x_line = np.linspace(low_x_hv, high_x_hv, num_sim_points)

low_x_bc = 30
high_x_bc = 100
a_bc_x_line = np.linspace(low_x_bc, high_x_bc, num_sim_points)

fig, ax1 = plt.subplots()



num_walkers = 8
num_burn_in = 100
num_steps = 1000
num_random_sample = num_steps - num_burn_in

"""
# BC
num_dim_bc = 1
a_starting_pos_bc = emcee.utils.sample_ball([0.0145], [0.0025], size=num_walkers)

sampler_bc = emcee.EnsembleSampler(num_walkers, 2, fit_linear, args=(a_bc_x_mean, a_bc_y_mean, a_bc_x_width, a_bc_y_width), threads=1)

print 'starting mcmc bc'
#sampler_bc.run_mcmc(a_starting_pos_bc, 200)
with click.progressbar(sampler_bc.sample(a_starting_pos_bc, iterations=num_steps, ), length=num_steps) as mcmc_sampler:
    for pos, lnprob, state in mcmc_sampler:
        pass

a_samples_bc = sampler_bc.chain[:, num_burn_in:, :].reshape(-1, num_dim_bc)


mean_slope = np.mean(a_samples_bc)
std_slope = np.std(a_samples_bc)

print mean_slope, std_slope


a_bc_y_line = a_bc_x_line*mean_slope



l_bc_prediction = np.zeros((num_random_sample, num_sim_points))
for i in xrange(num_random_sample):
    l_bc_prediction[i, :] = a_bc_x_line*a_samples_bc[-i]

l_bc_prediction_lb = np.zeros(num_sim_points)
l_bc_prediction_med = np.zeros(num_sim_points)
l_bc_prediction_ub = np.zeros(num_sim_points)

for i in xrange(num_sim_points):
    l_bc_prediction_lb[i], l_bc_prediction_med[i], l_bc_prediction_ub[i] = np.percentile(l_bc_prediction[:, i], [16, 50, 84])




# HV
num_dim_hv = 2
a_starting_pos_hv = emcee.utils.sample_ball([0.015, 17.7], [0.0025, 0.3], size=num_walkers)

sampler_hv = emcee.EnsembleSampler(num_walkers, 2, fit_exp, args=(a_hv_x_mean, a_hv_y_mean, a_hv_x_width, a_hv_y_width), threads=1)

print 'starting mcmc hv'
#sampler_hv.run_mcmc(a_starting_pos_hv, 200)
with click.progressbar(sampler_hv.sample(a_starting_pos_hv, iterations=num_steps, ), length=num_steps) as mcmc_sampler:
    for pos, lnprob, state in mcmc_sampler:
        pass

a_samples_hv = sampler_hv.chain[:, num_burn_in:, :].reshape(-1, num_dim_hv)

a_mean_hv = np.mean(a_samples_hv, axis=0)
a_cov_hv = np.cov(a_samples_hv.T)


l_hv_prediction = np.zeros((num_random_sample, num_sim_points))
for i in xrange(num_random_sample):
    l_hv_prediction[i, :] = a_samples_hv[-i, 0]*(np.exp(a_hv_x_line/a_samples_hv[-i, 1]) - 1)

l_hv_prediction_lb = np.zeros(num_sim_points)
l_hv_prediction_med = np.zeros(num_sim_points)
l_hv_prediction_ub = np.zeros(num_sim_points)

for i in xrange(num_sim_points):
    l_hv_prediction_lb[i], l_hv_prediction_med[i], l_hv_prediction_ub[i] = np.percentile(l_hv_prediction[:, i], [16, 50, 84])
"""


def fit_full(a_pars, a_x_bc, a_y_bc, a_y_err_bc, a_x_hv, a_y_hv, a_y_err_hv):
    slope, power = a_pars

    a_y_bc_line = slope*a_x_bc * (np.exp(70./power) - 1)
    a_y_hv_line = slope*65 * (np.exp(a_x_hv/power) - 1)

    return ln_likelihood_y(a_y_bc_line, a_y_bc, a_y_err_bc) + ln_likelihood_y(a_y_hv_line, a_y_hv, a_y_err_hv)


num_dim = 2
a_starting_pos = emcee.utils.sample_ball([0.015, 17.7], [0.025, 0.3], size=num_walkers)


sampler = emcee.EnsembleSampler(num_walkers, num_dim, fit_full, args=(a_bc_x_mean, a_bc_y_mean, a_bc_y_width, a_hv_x_mean, a_hv_y_mean, a_hv_y_width), threads=1)

print 'starting mcmc hv'
#sampler_hv.run_mcmc(a_starting_pos_hv, 200)
with click.progressbar(sampler.sample(a_starting_pos, iterations=num_steps, ), length=num_steps) as mcmc_sampler:
    for pos, lnprob, state in mcmc_sampler:
        pass


a_samples = sampler.chain[:, num_burn_in:, :].reshape(-1, num_dim)

mean_slope, mean_power = np.mean(a_samples, axis=0)
print mean_slope, mean_power




l_bc_prediction = np.zeros((num_random_sample, num_sim_points))
for i in xrange(num_random_sample):
    l_bc_prediction[i, :] = a_samples[-i, 0]*a_bc_x_line*(np.exp(70/a_samples[-i, 1]) - 1)

l_bc_prediction_lb = np.zeros(num_sim_points)
l_bc_prediction_med = np.zeros(num_sim_points)
l_bc_prediction_ub = np.zeros(num_sim_points)

for i in xrange(num_sim_points):
    l_bc_prediction_lb[i], l_bc_prediction_med[i], l_bc_prediction_ub[i] = np.percentile(l_bc_prediction[:, i], [16, 50, 84])



l_hv_prediction = np.zeros((num_random_sample, num_sim_points))
for i in xrange(num_random_sample):
    l_hv_prediction[i, :] = a_samples[-i, 0]*65*(np.exp(a_hv_x_line/a_samples[-i, 1]) - 1)

l_hv_prediction_lb = np.zeros(num_sim_points)
l_hv_prediction_med = np.zeros(num_sim_points)
l_hv_prediction_ub = np.zeros(num_sim_points)

for i in xrange(num_sim_points):
    l_hv_prediction_lb[i], l_hv_prediction_med[i], l_hv_prediction_ub[i] = np.percentile(l_hv_prediction[:, i], [16, 50, 84])








ax1.errorbar(a_bc_x_mean, a_bc_y_mean, xerr=a_bc_x_width, yerr=a_bc_y_width, color=bc_color, linestyle='')
ax1.plot(a_bc_x_line, l_bc_prediction_med, color=bc_color, linestyle='--')
ax1.fill_between(a_bc_x_line, l_bc_prediction_lb, l_bc_prediction_ub, alpha=transparency, color=bc_color)

ax1.set_xlim(low_x_bc, high_x_bc)
ax1.set_ylim(lb_np_rate, ub_np_rate)
ax1.set_xlabel(s_bc_label)





ax2 = ax1.twiny()
ax2.errorbar(a_hv_x_mean, a_hv_y_mean, xerr=a_hv_x_width, yerr=a_hv_y_width, color=hv_color, linestyle='')
ax2.plot(a_hv_x_line, l_hv_prediction_med, color=hv_color, linestyle='--')
ax2.fill_between(a_hv_x_line, l_hv_prediction_lb, l_hv_prediction_ub, alpha=transparency, color=hv_color)

ax2.set_yscale('log')
ax2.set_xlabel(s_hv_label)
ax1.set_ylabel('NP-2 Signal Rate [Hz]')
#ax2.set_ylabel('NP-2 Signal Rate')
ax2.set_xlim(low_x_hv, high_x_hv)
ax2.set_ylim(lb_np_rate, ub_np_rate)




# add text to plots
ax2.text(50, 2.2, 'High Voltage Constant, 70 kV', color=bc_color)
ax2.text(49.9, 1.8, 'Beam Current Constant, 65 $\mu$A', color=hv_color)





xx, yy = np.meshgrid(a_bc_x_line, a_hv_x_line)
zz = xx*mean_slope * (np.exp(yy/mean_power) - 1) # NP2 counts
zz *= flux_constant # flux
#zz *= np.pi * 180**2


fig_2d, ax_2d = plt.subplots()
pcm = ax_2d.pcolormesh(xx, yy, zz, cmap='jet', norm=colors.LogNorm(vmin=zz.min(), vmax=zz.max()))
cbar = fig_2d.colorbar(pcm, ax=ax_2d)
cbar.ax.tick_params(labelsize=10)
cbar.set_ticks([1e-1, 2e-1, 3e-1, 4e-1, 5e-1, 6e-1, 7e-1, 8e-1, 9e-1, 1e0, 2e0, 3e0, 4e0, 5e0, 6e0, 7e0, 8e0, 9e0, 1e1, 2e1])
#cbar.set_ticklabels([1e5,1e6])
cbar.set_label(r'Neutron Flux [n/s/$\mathrm{cm}^2$]', rotation=-90, labelpad=15)

ax_2d.set_xlim(low_x_bc, high_x_bc)
ax_2d.set_ylim(low_x_hv, high_x_hv)
ax_2d.set_xlabel(s_bc_label)
ax_2d.set_ylabel(s_hv_label)
#ax_2d.set_title('Neutron Rate [n/s]')


fig.tight_layout()
fig_2d.tight_layout()


fig.savefig('./plots/nerix_minitron_fits.png')
fig_2d.savefig('./plots/nerix_minitron_rate.png')


#plt.show()
