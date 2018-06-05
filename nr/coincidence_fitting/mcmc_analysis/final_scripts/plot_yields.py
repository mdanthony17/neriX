#!/usr/bin/python
import sys, array, os
sys.path.insert(0, '..')

import matplotlib
matplotlib.use('QT4Agg')
import matplotlib.style
matplotlib.style.use('classic')

import matplotlib.pyplot as plt
import matplotlib.mlab as mlab

matplotlib.rcParams['mathtext.fontset'] = 'stix'
matplotlib.rcParams['font.family'] = 'STIXGeneral'
matplotlib.rcParams['font.size'] = 16


import ROOT as root
import neriX_analysis
from rootpy.plotting import Hist, Hist2D, Canvas, Legend
import numpy as np
import cPickle as pickle
import tqdm

from scipy.interpolate import spline



def produce_nest_yields(a_energies):
    a_reduced_energies = 11.5 * a_energies * 54**(-7./3.)
    g_of_e = 3*a_reduced_energies**0.15 + 0.7*a_reduced_energies**0.6 + a_reduced_energies
    field_nest = 490
    
    k_nest = 0.1396
    alpha_nest = 1.240
    zeta_nest = 0.0472
    beta_nest = 239
    gamma_nest = 0.01385
    delta_nest = 0.0620
    eta_nest = 3.3
    lambda_nest = 1.14

    bq_nest = 1. / (1. + eta_nest*a_reduced_energies**lambda_nest)
    exciton_to_ion_ratio_nest = alpha_nest * field_nest**(-zeta_nest) * (1. - np.exp(-beta_nest*a_reduced_energies))
    sigma_nest = gamma_nest * field_nest**(-delta_nest)

    lindhard_nest = k_nest*g_of_e / (1. + k_nest*g_of_e)

    num_quanta_nest = a_energies * lindhard_nest / 0.0137
    num_ions_nest = num_quanta_nest / (1. + exciton_to_ion_ratio_nest)
    num_excitons_nest = num_quanta_nest - num_ions_nest

    num_recombined_nest = (1 - np.log(1 + num_ions_nest*sigma_nest) / (num_ions_nest*sigma_nest))*num_ions_nest
    num_excitons_nest += num_recombined_nest
    num_ions_nest -= num_recombined_nest

    num_photons_nest = num_excitons_nest * bq_nest
    num_electrons_nest = num_ions_nest

    return num_photons_nest/a_energies, num_electrons_nest/a_energies




min_energy_plot = 1
max_energy_plot = 75#30
ly_min = 1
ly_max = 18
ly_diff_min = -2
ly_diff_max = 5
qy_min = 2
qy_max = 12
qy_diff_min = -2
qy_diff_max = 1.5

legend_column_spacing = 0.5

d_for_plots = pickle.load(open('all_yields.p', 'r'))
d_yields = d_for_plots['nerix']
l_cathode_settings_in_use = d_yields.keys()

# set colors for different cathode settings
for cathode_setting in l_cathode_settings_in_use:
    if cathode_setting == 0.345:
        d_yields[cathode_setting]['color'] = '#0082c8'
        d_yields[cathode_setting]['hatch'] = '||'
        d_yields[cathode_setting]['field'] = 0.19
    elif cathode_setting == 1.054:
        d_yields[cathode_setting]['color'] = '#3cb44b'
        d_yields[cathode_setting]['hatch'] = '\x5c\x5c'
        d_yields[cathode_setting]['field'] = 0.49
    elif cathode_setting == 2.356:
        d_yields[cathode_setting]['color'] = '#e6194b'
        d_yields[cathode_setting]['hatch'] = '--'
        d_yields[cathode_setting]['field'] = 1.02


transparency_one_sigma = 0.2
transparency_two_sigma = 0.05

# 10.5, 12.5 looks good with 4
# first submission: 9.5, 10.5
f, (ax_diff1, ax1, ax2, ax_diff2) = plt.subplots(4, sharex=True, figsize=(10.5, 18.5))
f.subplots_adjust(hspace=0)
#f.tight_layout(w_pad=1)

ax1.set_xlim(min_energy_plot, max_energy_plot)
ax1.set_ylim(ly_min, ly_max)
ax1.set_xscale('log', nonposx='clip')
#ax1.set_yscale('log', nonposx='clip')
ax1.set_xlabel('Energy [keV]')
ax1.set_ylabel(r'$L_{y} [\frac{\mathrm{photons}}{\mathrm{keV}}]$')


a_energies = d_yields[cathode_setting]['energies']
a_ly_nest, a_qy_nest = produce_nest_yields(a_energies)

handler_nest_ly, = ax1.plot(a_energies, a_ly_nest, marker='', linestyle='-', color='k', linewidth=2, label='NEST 2015')
handler_nest_qy, = ax2.plot(a_energies, a_qy_nest, marker='', linestyle='-', color='k', linewidth=2, label='NEST 2015')


for cathode_setting in l_cathode_settings_in_use:

    ax1.errorbar(d_yields[cathode_setting]['a_energy_mean_data'], d_yields[cathode_setting]['a_py_median_data'], xerr=d_yields[cathode_setting]['a_energy_width_data'], yerr=[d_yields[cathode_setting]['a_py_lb_data'], d_yields[cathode_setting]['a_py_ub_data']], color=d_yields[cathode_setting]['color'], marker='.', linestyle='', label=r'This Work, $%.2f \, \, \frac{\mathrm{kV}}{\mathrm{cm}}$' % (d_yields[cathode_setting]['field']), markersize='14', elinewidth=2)
    d_yields[cathode_setting]['ly_handler_point'], = ax1.plot(-1000, -1000, color=d_yields[cathode_setting]['color'], marker='.', linestyle='', label=r'This Work, $%.2f \, \, \frac{\mathrm{kV}}{\mathrm{cm}}$' % (d_yields[cathode_setting]['field']))

    #print cathode_setting
    #print d_yields[cathode_setting]['a_py_median_band']
    
    # light yield systemiatic uncertainties moved pos (0.345, 1.054, 2.356)
    # rel stat uncertainty: 0.0244, 0.0267, 0.0311
    # rel median: 0.00174, 0.00189, 0.00222
    # light yield systemiatic uncertainties binning (0.345, 1.054, 2.356)
    # rel stat uncertainty: 0.0933, 0.0944, 0.0959
    # rel median: 0.0065, 0.00654, 0.00667
    d_sys_uncertainty = {0.345:0.00174, 1.054:0.0189, 2.356:0.00222}

    ax1.plot(d_yields[cathode_setting]['energies'], d_yields[cathode_setting]['a_py_median_band'], marker='', linestyle='-', color=d_yields[cathode_setting]['color'], linewidth=2)
    
    # bands
    d_yields[cathode_setting]['ly_handler'], = ax_diff1.plot(d_yields[cathode_setting]['energies'], d_yields[cathode_setting]['a_py_median_band']-a_ly_nest, marker='', linestyle='-', color=d_yields[cathode_setting]['color'], linewidth=2, label=r'This Work, $%.2f \, \, \frac{\mathrm{kV}}{\mathrm{cm}}$' % (d_yields[cathode_setting]['field']))
    #d_yields[cathode_setting]['ly_handler'] = ax_diff1.fill_between(d_yields[cathode_setting]['energies'], d_yields[cathode_setting]['a_py_lb_band'] - a_ly_nest, d_yields[cathode_setting]['a_py_ub_band'] - a_ly_nest, facecolor=d_yields[cathode_setting]['color'], alpha=transparency_one_sigma, interpolate=True, label=r'This Work, $%.2f \, \, \frac{\mathrm{kV}}{\mathrm{cm}}$' % (d_yields[cathode_setting]['field']))
    #a_lb =
    ax_diff1.fill_between(d_yields[cathode_setting]['energies'], d_yields[cathode_setting]['a_py_lb_band']-a_ly_nest, d_yields[cathode_setting]['a_py_ub_band']-a_ly_nest, color='none', hatch=d_yields[cathode_setting]['hatch'], edgecolor=d_yields[cathode_setting]['color'], linewidth=1, interpolate=True, label=r'This Work, $%.2f \, \, \frac{\mathrm{kV}}{\mathrm{cm}}$' % (d_yields[cathode_setting]['field']))


color_plante = '#000000'
color_aprile_09 = '#f58231'
color_aprile_05 = '#000080'
color_manzur = '#800000'
color_lux = '#911eb4'
color_aprile_06 = '#808000'
color_sorenson = '#aa6e28'
color_weber = '#008080'
color_zepplin_fsr = '#aa6e28'
color_zepplin_ssr = '#800000'

# points
# Plante
ax1.errorbar(d_for_plots['ly_comps']['plante']['ly_energies'], d_for_plots['ly_comps']['plante']['ly'], xerr=d_for_plots['ly_comps']['plante']['ly_energies_unc'], yerr=d_for_plots['ly_comps']['plante']['ly_unc'], marker='.', color=color_plante, linestyle='', label=r'Plante 2011, $0 \, \, \frac{\mathrm{kV}}{\mathrm{cm}}$')
handler_plante, = ax1.plot(-1000, -1000, marker='.', color=color_plante, linestyle='', label=r'Plante 2011, $0 \, \, \frac{\mathrm{kV}}{\mathrm{cm}}$')
# Aprile 09
ax1.errorbar(d_for_plots['ly_comps']['aprile_09']['ly_energies'], d_for_plots['ly_comps']['aprile_09']['ly'], xerr=d_for_plots['ly_comps']['aprile_09']['ly_energies_unc'], yerr=d_for_plots['ly_comps']['aprile_09']['ly_unc'], marker='.', color=color_aprile_09, linestyle='')
handler_aprile_09, = ax1.plot(-1000, -1000, marker='.', color=color_aprile_09, linestyle='', label=r'Aprile 2009, $0 \, \, \frac{\mathrm{kV}}{\mathrm{cm}}$')
# Aprile 05
ax1.errorbar(d_for_plots['ly_comps']['aprile_05']['ly_energies'], d_for_plots['ly_comps']['aprile_05']['ly'], xerr=d_for_plots['ly_comps']['aprile_05']['ly_energies_unc'], yerr=d_for_plots['ly_comps']['aprile_05']['ly_unc'], marker='.', color=color_aprile_05, linestyle='')
handler_aprile_05, = ax1.plot(-1000, -1000, marker='.', color=color_aprile_05, linestyle='', label=r'Aprile 2005, $0 \, \, \frac{\mathrm{kV}}{\mathrm{cm}}$')
# Manzur (2010)
ax1.errorbar(d_for_plots['ly_comps']['manzur']['ly_energies'], d_for_plots['ly_comps']['manzur']['ly'], xerr=d_for_plots['ly_comps']['manzur']['ly_energies_unc'], yerr=d_for_plots['ly_comps']['manzur']['ly_unc'], marker='.', color=color_manzur, linestyle='')
handler_manzur, = ax1.plot(-1000, -1000, marker='.', color=color_manzur, linestyle='', label=r'Manzur 2010, $0 \, \, \frac{\mathrm{kV}}{\mathrm{cm}}$')
# LUX (2017)
ax1.errorbar(d_for_plots['ly_comps']['lux']['ly_energies'], d_for_plots['ly_comps']['lux']['ly'], xerr=d_for_plots['ly_comps']['lux']['ly_energies_unc'], yerr=d_for_plots['ly_comps']['lux']['ly_unc'], marker='.', color=color_lux, linestyle='')
handler_lux, = ax1.plot(-1000, -1000, marker='.', color=color_lux, linestyle='', label=r'LUX 2017, $0.18 \, \, \frac{\mathrm{kV}}{\mathrm{cm}}$')

# bands
# Zepplin FSR
#handler_zepplin_fsr = ax1.fill_between(d_for_plots['ly_comps']['zepplin_fsr']['ly_energies'], d_for_plots['ly_comps']['zepplin_fsr']['ly_lb'], d_for_plots['ly_comps']['zepplin_fsr']['ly_ub'], color='none', hatch='///', edgecolor=color_zepplin_fsr, linewidth=0, interpolate=True, label=r'Horn 2011 (FSR), $3.90 \, \, \frac{\mathrm{kV}}{\mathrm{cm}}$')
# Zepplin SSR
#handler_zepplin_ssr = ax1.fill_between(d_for_plots['ly_comps']['zepplin_ssr']['ly_energies'], d_for_plots['ly_comps']['zepplin_ssr']['ly_lb'], d_for_plots['ly_comps']['zepplin_ssr']['ly_ub'], color='none', hatch='\\'*3, edgecolor=color_zepplin_ssr, linewidth=0, interpolate=True, label=r'Horn 2011 (SSR), $4.00 \, \, \frac{\mathrm{kV}}{\mathrm{cm}}$')
# Sorenson 09
#ax1.fill_between(d_for_plots['ly_comps']['sorenson']['ly_energies'], d_for_plots['ly_comps']['sorenson']['ly_lb'], d_for_plots['ly_comps']['sorenson']['ly_ub'], color='none', hatch='|||', edgecolor='darkred', linewidth=0, interpolate=True)


#print list(handler_plante)
#ax1.legend(handles=[d_yields[0.345]['ly_handler'], d_yields[1.054]['ly_handler'], d_yields[2.356]['ly_handler'], handler_lux, handler_plante, handler_zepplin_ssr, handler_zepplin_fsr, handler_manzur, handler_aprile_09, handler_aprile_05], loc='best', ncol=2, prop={'size': 12}, frameon=False, handletextpad=0.1, columnspacing=0.5, numpoints=1)

ax_diff1.set_ylim(ly_diff_min, ly_diff_max)
ax_diff1.set_yticks([-1, 0, 1, 2, 3, 4, 5])
ax_diff1.set_ylabel(r'$\Delta L_y [\frac{\mathrm{photons}}{\mathrm{keV}}]$')
ax_diff1.axhline(0, linestyle='-', color='k', linewidth=2)
ax_diff1.legend(handles=[handler_nest_ly, d_yields[0.345]['ly_handler'], d_yields[1.054]['ly_handler'], d_yields[2.356]['ly_handler']], loc='best', ncol=2, prop={'size': 12}, frameon=False, handletextpad=0.2, columnspacing=legend_column_spacing, numpoints=1)

# without Horn
ax1.legend(handles=[handler_nest_ly, d_yields[0.345]['ly_handler_point'], d_yields[1.054]['ly_handler_point'], d_yields[2.356]['ly_handler_point'], handler_lux, handler_plante, handler_manzur, handler_aprile_09, handler_aprile_05], loc='best', ncol=3, prop={'size': 12}, frameon=False, handletextpad=0.2, columnspacing=legend_column_spacing, numpoints=1)
ax1.set_yticks([2, 4, 6, 8, 10, 12, 14, 16])



ax2.set_xlim(min_energy_plot, max_energy_plot)
ax2.set_ylim(qy_min, qy_max)
ax2.set_xscale('log', nonposx='clip')
#ax2.set_yscale('log', nonposx='clip')
ax2.set_xlabel('Energy [keV]')
ax_diff2.set_xlabel('Energy [keV]')
ax2.set_ylabel(r'$Q_{y} [\frac{\mathrm{electrons}}{\mathrm{keV}}]$')

for cathode_setting in l_cathode_settings_in_use:

    ax2.errorbar(d_yields[cathode_setting]['a_energy_mean_data'], d_yields[cathode_setting]['a_qy_median_data'], xerr=d_yields[cathode_setting]['a_energy_width_data'], yerr=[d_yields[cathode_setting]['a_qy_lb_data'], d_yields[cathode_setting]['a_qy_ub_data']], color=d_yields[cathode_setting]['color'], marker='.', linestyle='', markersize='14', elinewidth=2)
    d_yields[cathode_setting]['qy_handler_point'], = ax2.plot(-1000, -1000, color=d_yields[cathode_setting]['color'], marker='.', linestyle='', label=r'This Work, $%.2f \, \, \frac{\mathrm{kV}}{\mathrm{cm}}$' % (d_yields[cathode_setting]['field']))
    
    
    # light charge systemiatic uncertainties moved pos (0.345, 1.054, 2.356)
    # rel stat uncertainty: 0.0979, 0.0638, 0.0699
    # rel median: 0.00386, 0.0249, 0.00272
    # light charge systemiatic uncertainties binning (0.345, 1.054, 2.356)
    # rel stat uncertainty: 0.0537, 0.0806, 0.0619
    # rel median: 0.00217, 0.00319, 0.00243
    d_sys_uncertainty = {0.345:0.00386, 1.054:0.00249, 2.356:0.00272}

    ax2.plot(d_yields[cathode_setting]['energies'], d_yields[cathode_setting]['a_qy_median_band'], marker='', linestyle='-', color=d_yields[cathode_setting]['color'], linewidth=2)

    # band
    d_yields[cathode_setting]['qy_handler'], = ax_diff2.plot(d_yields[cathode_setting]['energies'], d_yields[cathode_setting]['a_qy_median_band']-a_qy_nest, marker='', linestyle='-', color=d_yields[cathode_setting]['color'], linewidth=2, label=r'This Work, $%.2f \, \, \frac{\mathrm{kV}}{\mathrm{cm}}$' % (d_yields[cathode_setting]['field']))
    #d_yields[cathode_setting]['ly_handler'] = ax_diff1.fill_between(d_yields[cathode_setting]['energies'], d_yields[cathode_setting]['a_py_lb_band'] - a_ly_nest, d_yields[cathode_setting]['a_py_ub_band'] - a_ly_nest, facecolor=d_yields[cathode_setting]['color'], alpha=transparency_one_sigma, interpolate=True, label=r'This Work, $%.2f \, \, \frac{\mathrm{kV}}{\mathrm{cm}}$' % (d_yields[cathode_setting]['field']))
    ax_diff2.fill_between(d_yields[cathode_setting]['energies'], d_yields[cathode_setting]['a_qy_lb_band']-a_qy_nest, d_yields[cathode_setting]['a_qy_ub_band']-a_qy_nest, color='none', hatch=d_yields[cathode_setting]['hatch'], edgecolor=d_yields[cathode_setting]['color'], linewidth=1, interpolate=True, label=r'This Work, $%.2f \, \, \frac{\mathrm{kV}}{\mathrm{cm}}$' % (d_yields[cathode_setting]['field']))

# points
# LUX
ax2.errorbar(d_for_plots['qy_comps']['lux']['qy_energies'], d_for_plots['qy_comps']['lux']['qy'], xerr=d_for_plots['qy_comps']['lux']['qy_energies_unc'], yerr=d_for_plots['qy_comps']['lux']['qy_unc'], marker='.', color=color_lux, linestyle='')
# Manzur
ax2.errorbar(d_for_plots['qy_comps']['manzur']['qy_energies'], d_for_plots['qy_comps']['manzur']['qy'], xerr=d_for_plots['qy_comps']['manzur']['qy_energies_unc'], yerr=d_for_plots['qy_comps']['manzur']['qy_unc'], marker='.', color=color_manzur, linestyle='')
handler_manzur_qy, = ax2.plot(-1000, -1000, marker='.', color=color_manzur, linestyle='', label=r'Manzur 2010, $1.00 \, \, \frac{\mathrm{kV}}{\mathrm{cm}}$')
# Aprile 06
ax2.errorbar(d_for_plots['qy_comps']['aprile_06']['qy_energies'], d_for_plots['qy_comps']['aprile_06']['qy'], xerr=d_for_plots['qy_comps']['aprile_06']['qy_energies_unc'], yerr=d_for_plots['qy_comps']['aprile_06']['qy_unc'], marker='.', color=color_aprile_06, linestyle='')
handler_aprile_06, = ax2.plot(-1000, -1000, marker='.', color=color_aprile_06, linestyle='', label=r'Aprile 2006, $2.00 \, \, \frac{\mathrm{kV}}{\mathrm{cm}}$')
# Sorenson
# https://arxiv.org/pdf/0807.0459.pdf
ax2.errorbar(d_for_plots['qy_comps']['sorenson']['qy_energies'], d_for_plots['qy_comps']['sorenson']['qy'], xerr=d_for_plots['qy_comps']['sorenson']['qy_energies_unc'], yerr=d_for_plots['qy_comps']['sorenson']['qy_unc'], marker='.', color=color_sorenson, linestyle='')
handler_sorenson, = ax2.plot(-1000, -1000, marker='.', color=color_sorenson, linestyle='', label=r'Sorensen 2009, $0.73 \, \, \frac{\mathrm{kV}}{\mathrm{cm}}$')


# bands
# Weber
handler_weber = ax2.fill_between(d_for_plots['qy_comps']['weber']['qy_energies'], d_for_plots['qy_comps']['weber']['qy_lb'], d_for_plots['qy_comps']['weber']['qy_ub'], color='none', hatch='||', edgecolor=color_weber, linewidth=0, interpolate=True, label=r'Aprile 2013, $0.53 \, \, \frac{\mathrm{kV}}{\mathrm{cm}}$')
# Zepplin FSR (2011)
#ax2.fill_between(d_for_plots['qy_comps']['zepplin_fsr']['qy_energies'], d_for_plots['qy_comps']['zepplin_fsr']['qy_lb'], d_for_plots['qy_comps']['zepplin_fsr']['qy_ub'], color='none', hatch='///', edgecolor=color_zepplin_fsr, linewidth=0, interpolate=True, label=r'ZEPLIN FSR, $3.90 \, \, \frac{\mathrm{kV}}{\mathrm{cm}}$')
# Zepplin SSR (paper in 2011)
#ax2.fill_between(d_for_plots['qy_comps']['zepplin_ssr']['qy_energies'], d_for_plots['qy_comps']['zepplin_ssr']['qy_lb'], d_for_plots['qy_comps']['zepplin_ssr']['qy_ub'], color='none', hatch='\\'*3, edgecolor=color_zepplin_ssr, linewidth=0, interpolate=True, label=r'ZEPLIN SSR, $4.00 \, \, \frac{\mathrm{kV}}{\mathrm{cm}}$')

ax_diff2.set_ylim(qy_diff_min, qy_diff_max)
ax_diff2.set_yticks([-2, -1, 0, 1])
ax_diff2.set_ylabel(r'$\Delta Q_y [\frac{\mathrm{electrons}}{\mathrm{keV}}]$')
ax_diff2.axhline(0, linestyle='-', color='k', linewidth=2)
ax_diff2.legend(handles=[handler_nest_qy, d_yields[0.345]['qy_handler'], d_yields[1.054]['qy_handler'], d_yields[2.356]['qy_handler']], loc='lower right', ncol=2, prop={'size': 12}, frameon=False, handletextpad=0.2, columnspacing=legend_column_spacing, numpoints=1)

#print d_for_plots['qy_comps']['zepplin_ssr']['qy_ub']

#ax2.legend(handles=[d_yields[0.345]['ly_handler'], d_yields[1.054]['ly_handler'], d_yields[2.356]['ly_handler'], handler_lux, handler_weber, handler_zepplin_ssr, handler_zepplin_fsr, handler_manzur, handler_sorenson, handler_aprile_06], loc='best', ncol=2, prop={'size': 12}, frameon=False, handletextpad=0.1, columnspacing=0.5, numpoints=1)

# without Horn
ax2.legend(handles=[d_yields[0.345]['qy_handler_point'], d_yields[1.054]['qy_handler_point'], d_yields[2.356]['qy_handler_point'], handler_lux, handler_weber, handler_manzur_qy, handler_sorenson, handler_aprile_06], loc='best', ncol=2, prop={'size': 12}, frameon=False, handletextpad=0.2, columnspacing=legend_column_spacing, numpoints=1)
ax2.set_yticks([3, 5, 7, 9, 11])

ax2.set_xticks([3, 6, 10, 20, 70])
ax2.get_xaxis().set_major_formatter(matplotlib.ticker.ScalarFormatter())

# make grey squares
box_alpha = 0.1
ax_diff1.fill_between([min_energy_plot, 3], [ly_diff_min, ly_diff_min], [ly_diff_max, ly_diff_max], color='grey', alpha=box_alpha)
ax1.fill_between([min_energy_plot, 3], [ly_min, ly_min], [ly_max, ly_max], color='grey', alpha=box_alpha)
ax2.fill_between([min_energy_plot, 3], [qy_min, qy_min], [qy_max, qy_max], color='grey', alpha=box_alpha)
ax_diff2.fill_between([min_energy_plot, 3], [qy_diff_min, qy_diff_min], [qy_diff_max, qy_diff_max], color='grey', alpha=box_alpha)


f.savefig('./plots/s_ly_qy_comparison.png', bbox_inches='tight')

#plt.show()



