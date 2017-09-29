#!/usr/bin/python
import sys, array, os
sys.path.insert(0, '..')

import matplotlib
matplotlib.use('QT4Agg')
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


min_energy_plot = 1
max_energy_plot = 75#30

d_for_plots = pickle.load(open('all_yields.p', 'r'))
d_yields = d_for_plots['nerix']
l_cathode_settings_in_use = d_yields.keys()

# set colors for different cathode settings
for cathode_setting in l_cathode_settings_in_use:
    if cathode_setting == 0.345:
        d_yields[cathode_setting]['color'] = '#0082c8'
        d_yields[cathode_setting]['hatch'] = '|||'
        d_yields[cathode_setting]['field'] = 0.19
    elif cathode_setting == 1.054:
        d_yields[cathode_setting]['color'] = '#3cb44b'
        d_yields[cathode_setting]['hatch'] = '\x5c\x5c\x5c'
        d_yields[cathode_setting]['field'] = 0.49
    elif cathode_setting == 2.356:
        d_yields[cathode_setting]['color'] = '#e6194b'
        d_yields[cathode_setting]['hatch'] = '---'
        d_yields[cathode_setting]['field'] = 1.02


transparency_one_sigma = 0.2
transparency_two_sigma = 0.05



f, (ax1, ax2) = plt.subplots(2, sharex=True, figsize=(8.5, 12.5))
f.subplots_adjust(hspace=0)
#f.tight_layout(w_pad=1)

ax1.set_xlim(min_energy_plot, max_energy_plot)
ax1.set_ylim(3.5, 18)
ax1.set_xscale('log', nonposx='clip')
#ax1.set_yscale('log', nonposx='clip')
ax1.set_xlabel('Energy [keV]')
ax1.set_ylabel(r'$L_{y} [\frac{photons}{keV}]$')

for cathode_setting in l_cathode_settings_in_use:

    ax1.errorbar(d_yields[cathode_setting]['a_energy_mean_data'], d_yields[cathode_setting]['a_py_median_data'], xerr=d_yields[cathode_setting]['a_energy_width_data'], yerr=[d_yields[cathode_setting]['a_py_lb_data'], d_yields[cathode_setting]['a_py_ub_data']], color=d_yields[cathode_setting]['color'], marker='.', linestyle='')

    #print cathode_setting
    #print d_yields[cathode_setting]['a_py_median_band']

    # bands
    ax1.plot(d_yields[cathode_setting]['energies'], d_yields[cathode_setting]['a_py_median_band'], marker='', linestyle='-', color=d_yields[cathode_setting]['color'], linewidth=0.2)
    d_yields[cathode_setting]['ly_handler'] = ax1.fill_between(d_yields[cathode_setting]['energies'], d_yields[cathode_setting]['a_py_lb_band'], d_yields[cathode_setting]['a_py_ub_band'], facecolor=d_yields[cathode_setting]['color'], alpha=transparency_one_sigma, interpolate=True, label=r'Aprile 2017, $%.2f \, \frac{kV}{cm}$' % (d_yields[cathode_setting]['field']))
    #ax1.fill_between(d_yields[cathode_setting]['energies'], d_yields[cathode_setting]['a_py_lb_band'], d_yields[cathode_setting]['a_py_ub_band'], d_yields[cathode_setting]['a_py_ub_band'], color='none', hatch=d_yields[cathode_setting]['hatch'], edgecolor=d_yields[cathode_setting]['color'], linewidth=0, interpolate=True)


color_plante = '#000000'
color_aprile_09 = '#f58231'
color_aprile_05 = '#46f0f0'
color_manzur = '#f032e6'
color_lux = '#911eb4'
color_aprile_06 = '#808000'
color_sorenson = '#d2f53c'
color_weber = '#008080'
color_zepplin_fsr = '#aa6e28'
color_zepplin_ssr = '#800000'

# points
# Plante
ax1.errorbar(d_for_plots['ly_comps']['plante']['ly_energies'], d_for_plots['ly_comps']['plante']['ly'], xerr=d_for_plots['ly_comps']['plante']['ly_energies_unc'], yerr=d_for_plots['ly_comps']['plante']['ly_unc'], marker='.', color=color_plante, linestyle='', label=r'Plante 2011, $0 \, \frac{kV}{cm}$')
handler_plante, = ax1.plot(-1000, -1000, marker='.', color=color_plante, linestyle='', label=r'Plante 2011, $0 \, \frac{kV}{cm}$')
# Aprile 09
ax1.errorbar(d_for_plots['ly_comps']['aprile_09']['ly_energies'], d_for_plots['ly_comps']['aprile_09']['ly'], xerr=d_for_plots['ly_comps']['aprile_09']['ly_energies_unc'], yerr=d_for_plots['ly_comps']['aprile_09']['ly_unc'], marker='.', color=color_aprile_09, linestyle='')
handler_aprile_09, = ax1.plot(-1000, -1000, marker='.', color=color_aprile_09, linestyle='', label=r'Aprile 2009, $0 \, \frac{kV}{cm}$')
# Aprile 05
ax1.errorbar(d_for_plots['ly_comps']['aprile_05']['ly_energies'], d_for_plots['ly_comps']['aprile_05']['ly'], xerr=d_for_plots['ly_comps']['aprile_05']['ly_energies_unc'], yerr=d_for_plots['ly_comps']['aprile_05']['ly_unc'], marker='.', color=color_aprile_05, linestyle='')
handler_aprile_05, = ax1.plot(-1000, -1000, marker='.', color=color_aprile_05, linestyle='', label=r'Aprile 2005, $0 \, \frac{kV}{cm}$')
# Manzur (2010)
ax1.errorbar(d_for_plots['ly_comps']['manzur']['ly_energies'], d_for_plots['ly_comps']['manzur']['ly'], xerr=d_for_plots['ly_comps']['manzur']['ly_energies_unc'], yerr=d_for_plots['ly_comps']['manzur']['ly_unc'], marker='.', color=color_manzur, linestyle='')
handler_manzur, = ax1.plot(-1000, -1000, marker='.', color=color_manzur, linestyle='', label=r'Manzur 2010, $0 \, \frac{kV}{cm}$')
# LUX (2017)
ax1.errorbar(d_for_plots['ly_comps']['lux']['ly_energies'], d_for_plots['ly_comps']['lux']['ly'], xerr=d_for_plots['ly_comps']['lux']['ly_energies_unc'], yerr=d_for_plots['ly_comps']['lux']['ly_unc'], marker='.', color=color_lux, linestyle='')
handler_lux, = ax1.plot(-1000, -1000, marker='.', color=color_lux, linestyle='', label=r'LUX 2017, $0.18 \, \frac{kV}{cm}$')

# bands
# Zepplin FSR
handler_zepplin_fsr = ax1.fill_between(d_for_plots['ly_comps']['zepplin_fsr']['ly_energies'], d_for_plots['ly_comps']['zepplin_fsr']['ly_lb'], d_for_plots['ly_comps']['zepplin_fsr']['ly_ub'], color='none', hatch='///', edgecolor=color_zepplin_fsr, linewidth=0, interpolate=True, label=r'Horn 2011 (FSR), $3.90 \, \frac{kV}{cm}$')
# Zepplin SSR
handler_zepplin_ssr = ax1.fill_between(d_for_plots['ly_comps']['zepplin_ssr']['ly_energies'], d_for_plots['ly_comps']['zepplin_ssr']['ly_lb'], d_for_plots['ly_comps']['zepplin_ssr']['ly_ub'], color='none', hatch='\\'*3, edgecolor=color_zepplin_ssr, linewidth=0, interpolate=True, label=r'Horn 2011 (SSR), $4.00 \, \frac{kV}{cm}$')
# Sorenson 09
#ax1.fill_between(d_for_plots['ly_comps']['sorenson']['ly_energies'], d_for_plots['ly_comps']['sorenson']['ly_lb'], d_for_plots['ly_comps']['sorenson']['ly_ub'], color='none', hatch='|||', edgecolor='darkred', linewidth=0, interpolate=True)


#print list(handler_plante)
ax1.legend(handles=[d_yields[0.345]['ly_handler'], d_yields[1.054]['ly_handler'], d_yields[2.356]['ly_handler'], handler_lux, handler_manzur, handler_plante, handler_zepplin_ssr, handler_zepplin_fsr, handler_aprile_09, handler_aprile_05], loc='best', ncol=2, prop={'size': 12}, frameon=False, handletextpad=0.1, columnspacing=0.5, numpoints=1)
ax1.set_yticks([4, 6, 8, 10, 12, 14, 16])



ax2.set_xlim(min_energy_plot, max_energy_plot)
ax2.set_ylim(2, 12)
ax2.set_xscale('log', nonposx='clip')
#ax2.set_yscale('log', nonposx='clip')
ax2.set_xlabel('Energy [keV]')
ax2.set_ylabel(r'$Q_{y} [\frac{electrons}{keV}]$')

for cathode_setting in l_cathode_settings_in_use:

    ax2.errorbar(d_yields[cathode_setting]['a_energy_mean_data'], d_yields[cathode_setting]['a_qy_median_data'], xerr=d_yields[cathode_setting]['a_energy_width_data'], yerr=[d_yields[cathode_setting]['a_qy_lb_data'], d_yields[cathode_setting]['a_qy_ub_data']], color=d_yields[cathode_setting]['color'], marker='.', linestyle='')

    # band
    ax2.plot(d_yields[cathode_setting]['energies'], d_yields[cathode_setting]['a_qy_median_band'], marker='', linestyle='-', color=d_yields[cathode_setting]['color'], linewidth=0.2)
    d_yields[cathode_setting]['qy_handler'] = ax2.fill_between(d_yields[cathode_setting]['energies'], d_yields[cathode_setting]['a_qy_lb_band'], d_yields[cathode_setting]['a_qy_ub_band'], facecolor=d_yields[cathode_setting]['color'], alpha=transparency_one_sigma, interpolate=True)
    #ax2.fill_between(d_yields[cathode_setting]['energies'], d_yields[cathode_setting]['a_qy_lb_band'], d_yields[cathode_setting]['a_qy_ub_band'], d_yields[cathode_setting]['a_qy_ub_band'], color='none', hatch=d_yields[cathode_setting]['hatch'], edgecolor=d_yields[cathode_setting]['color'], linewidth=0, interpolate=True)

# points
# LUX
ax2.errorbar(d_for_plots['qy_comps']['lux']['qy_energies'], d_for_plots['qy_comps']['lux']['qy'], xerr=d_for_plots['qy_comps']['lux']['qy_energies_unc'], yerr=d_for_plots['qy_comps']['lux']['qy_unc'], marker='.', color=color_lux, linestyle='')
# Manzur
ax2.errorbar(d_for_plots['qy_comps']['manzur']['qy_energies'], d_for_plots['qy_comps']['manzur']['qy'], xerr=d_for_plots['qy_comps']['manzur']['qy_energies_unc'], yerr=d_for_plots['qy_comps']['manzur']['qy_unc'], marker='.', color=color_manzur, linestyle='')
# Aprile 06
ax2.errorbar(d_for_plots['qy_comps']['aprile_06']['qy_energies'], d_for_plots['qy_comps']['aprile_06']['qy'], xerr=d_for_plots['qy_comps']['aprile_06']['qy_energies_unc'], yerr=d_for_plots['qy_comps']['aprile_06']['qy_unc'], marker='.', color=color_aprile_06, linestyle='')
handler_aprile_06, = ax2.plot(-1000, -1000, marker='.', color=color_aprile_06, linestyle='', label=r'Aprile 2006, $2.00 \, \frac{kV}{cm}$')
# Sorenson
# https://arxiv.org/pdf/0807.0459.pdf
ax2.errorbar(d_for_plots['qy_comps']['sorenson']['qy_energies'], d_for_plots['qy_comps']['sorenson']['qy'], xerr=d_for_plots['qy_comps']['sorenson']['qy_energies_unc'], yerr=d_for_plots['qy_comps']['sorenson']['qy_unc'], marker='.', color=color_sorenson, linestyle='')
handler_sorenson, = ax2.plot(-1000, -1000, marker='.', color=color_sorenson, linestyle='', label=r'Sorenson 2008, $0.73 \, \frac{kV}{cm}$')


# bands
# Weber
handler_weber = ax2.fill_between(d_for_plots['qy_comps']['weber']['qy_energies'], d_for_plots['qy_comps']['weber']['qy_lb'], d_for_plots['qy_comps']['weber']['qy_ub'], color='none', hatch='|||', edgecolor=color_weber, linewidth=0, interpolate=True, label=r'Aprile 2013, $0.53 \, \frac{kV}{cm}$')
# Zepplin FSR (2011)
ax2.fill_between(d_for_plots['qy_comps']['zepplin_fsr']['qy_energies'], d_for_plots['qy_comps']['zepplin_fsr']['qy_lb'], d_for_plots['qy_comps']['zepplin_fsr']['qy_ub'], color='none', hatch='///', edgecolor=color_zepplin_fsr, linewidth=0, interpolate=True, label=r'ZEPLIN FSR, $3.90 \, \frac{kV}{cm}$')
# Zepplin SSR (paper in 2011)
ax2.fill_between(d_for_plots['qy_comps']['zepplin_ssr']['qy_energies'], d_for_plots['qy_comps']['zepplin_ssr']['qy_lb'], d_for_plots['qy_comps']['zepplin_ssr']['qy_ub'], color='none', hatch='\\'*3, edgecolor=color_zepplin_ssr, linewidth=0, interpolate=True, label=r'ZEPLIN SSR, $4.00 \, \frac{kV}{cm}$')

#print d_for_plots['qy_comps']['zepplin_ssr']['qy_ub']

ax2.legend(handles=[d_yields[0.345]['ly_handler'], d_yields[1.054]['ly_handler'], d_yields[2.356]['ly_handler'], handler_lux, handler_weber, handler_zepplin_ssr, handler_zepplin_fsr, handler_manzur, handler_sorenson, handler_aprile_06], loc='best', ncol=2, prop={'size': 12}, frameon=False, handletextpad=0.1, columnspacing=0.5, numpoints=1)
ax2.set_yticks([3, 5, 7, 9, 11])


f.savefig('./plots/s_ly_qy_comparison.png', bbox_inches='tight')

plt.show()



