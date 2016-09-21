import ROOT as root
from rootpy.plotting import Hist, Canvas
import neriX_analysis, neriX_datasets
from math import log, exp
import array, sys
import numpy as np
sys.path.append('../')
sys.path.append('./')
import fit_results_run_16


if len(sys.argv) != 2:
	print 'Use is python gas_gain_method_study.py <run>'
	sys.exit()

run  = int(sys.argv[1])
methods = fit_results_run_16.methods
radii_sets = fit_results_run_16.radii_sets

gas_gain_results = fit_results_run_16.gas_gain_results[run]

if run == 10 or run == 11:
	l_cathode_voltages = [0.345, 1.054, 2.356, 5.500]
else:
	l_cathode_voltages = [0.345, 1.054, 2.356]


# -----------------------------------------
# -----------------------------------------
#  Set prelimary variables for each
#  cathode setting and radial cut
# -----------------------------------------
# -----------------------------------------



d_gas_gains = {}
for radial_cut in radii_sets:
	d_gas_gains[radial_cut] = {}
	
	for cathode_setting in l_cathode_voltages:
		d_gas_gains[radial_cut][cathode_setting] = {}
		if cathode_setting == 0.345:
			color = root.kMagenta
			movement = 0.0
			fill_style = 3004
		elif cathode_setting == 1.054:
			color = root.kBlue
			movement = 0.05
			fill_style = 3005
		elif cathode_setting == 2.356:
			color = root.kGreen
			movement = 0.10
			fill_style = 3006
		elif cathode_setting == 5.500:
			color = root.kRed
			movement = 0.15
			fill_style = 3007
		d_gas_gains[radial_cut][cathode_setting]['color'] = color
		d_gas_gains[radial_cut][cathode_setting]['fill_style'] = fill_style
		
		d_gas_gains[radial_cut][cathode_setting]['tpavetext'] = root.TPaveText(.15,.8-movement,.5,.85-movement,'blNDC')
		d_gas_gains[radial_cut][cathode_setting]['tpavetext'].SetTextColor(color)
		d_gas_gains[radial_cut][cathode_setting]['tpavetext'].SetFillStyle(0)
		d_gas_gains[radial_cut][cathode_setting]['tpavetext'].SetBorderSize(0)

		d_gas_gains[radial_cut][cathode_setting]['a_time'] = []
		d_gas_gains[radial_cut][cathode_setting]['a_time_err'] = []
		d_gas_gains[radial_cut][cathode_setting]['a_gas_gain_bot'] = []
		d_gas_gains[radial_cut][cathode_setting]['a_gas_gain_bot_err'] = []
		


d_method_settings = {}
for method in methods:
	d_method_settings[method] = {}
	
	# set color for each method
	if method == 'gate_pi':
		marker = 26
	elif method == 'cathode_pi':
		marker = 24
	else:
		marker = 32
	d_method_settings[method]['marker'] = marker


# get list of files since the same for all
l_files = gas_gain_results[methods[0]][radii_sets[0]].keys()
l_files.sort()

# ranges for graphs
l_time_range = [neriX_analysis.convert_name_to_unix_time(l_files[0])-3e5, neriX_analysis.convert_name_to_unix_time(l_files[-1])+3e5]

if run == 10:
	l_gas_gain_range = [14.5, 21]
elif run == 11:
	l_gas_gain_range = [14, 19]
else:
	l_gas_gain_range = [10, 15]


l_path_for_save = ['results', 'fixed_thresholds', 'run_%d' % run]


# -----------------------------------------
# -----------------------------------------
#  Fill arrays for radial cut and cathode
#  setting with weighted averages
# -----------------------------------------
# -----------------------------------------


for radial_cut in radii_sets:
	
	for file in l_files:
		l_current_gas_gains = []
		l_current_gas_gain_errs = []
		l_current_gas_gain_weights = []
		
		current_cathode_setting = neriX_datasets.run_files[run][file + '.root'][1]
		
		current_file_dictionary = gas_gain_results[method][radial_cut][file]
	
		for method in methods:
			if current_file_dictionary['fit_successful']:
				l_current_gas_gains.append(current_file_dictionary['mean_bot'])
				l_current_gas_gain_errs.append(current_file_dictionary['mean_bot_err'])
				l_current_gas_gain_weights.append(1./l_current_gas_gain_errs[-1]**2.)

		if len(l_current_gas_gains) == 0:
			continue
		
		d_gas_gains[radial_cut][current_cathode_setting]['a_time'].append(neriX_analysis.convert_name_to_unix_time(file))
		d_gas_gains[radial_cut][current_cathode_setting]['a_time_err'].append(0)
		d_gas_gains[radial_cut][current_cathode_setting]['a_gas_gain_bot'].append(np.dot(l_current_gas_gains, l_current_gas_gain_weights) / float(np.sum(l_current_gas_gain_weights)))
		d_gas_gains[radial_cut][current_cathode_setting]['a_gas_gain_bot_err'].append(1./np.sum(l_current_gas_gain_weights)**0.5)
		
	

# -----------------------------------------
# -----------------------------------------
#  Fill graphs with weighted averages
#  and fit with pol0
# -----------------------------------------
# -----------------------------------------

for radial_cut in radii_sets:
	# create canvas for all methods with the same radial cut
	d_gas_gains[radial_cut]['canvas'] = Canvas()
	firstDraw = True
	
	for cathode_setting in l_cathode_voltages:

		d_gas_gains[radial_cut][cathode_setting]['graph'] = root.TGraphErrors(len(d_gas_gains[radial_cut][cathode_setting]['a_time']), np.array(d_gas_gains[radial_cut][cathode_setting]['a_time'], dtype=np.float64), np.array(d_gas_gains[radial_cut][cathode_setting]['a_gas_gain_bot'], dtype=np.float64), np.array(d_gas_gains[radial_cut][cathode_setting]['a_time_err'], dtype=np.float64), np.array(d_gas_gains[radial_cut][cathode_setting]['a_gas_gain_bot_err'], dtype=np.float64))
		
		d_gas_gains[radial_cut][cathode_setting]['graph'].SetTitle('Gas Gain Run Average, Method Average - %.2f < R < %.2f' % (radial_cut[0], radial_cut[1]))
		d_gas_gains[radial_cut][cathode_setting]['graph'].SetMarkerStyle(20)
		d_gas_gains[radial_cut][cathode_setting]['graph'].GetXaxis().SetLimits(*l_time_range)
		d_gas_gains[radial_cut][cathode_setting]['graph'].GetYaxis().SetRangeUser(*l_gas_gain_range)

		d_gas_gains[radial_cut][cathode_setting]['graph'].SetMarkerColor(d_gas_gains[radial_cut][cathode_setting]['color'])
		d_gas_gains[radial_cut][cathode_setting]['graph'].SetLineColor(d_gas_gains[radial_cut][cathode_setting]['color'])
		if firstDraw:
			d_gas_gains[radial_cut][cathode_setting]['graph'].Draw('ap')
			firstDraw = False
			d_gas_gains[radial_cut][cathode_setting]['graph'].GetYaxis().SetTitle('Gas gain [PE per extracted electron]')
			d_gas_gains[radial_cut][cathode_setting]['graph'].GetHistogram().GetXaxis().SetTimeDisplay(1)
			d_gas_gains[radial_cut][cathode_setting]['graph'].GetHistogram().GetXaxis().SetTimeFormat('%m-%d')
		
		else:
			d_gas_gains[radial_cut][cathode_setting]['graph'].Draw('p same')

		# fit to straight line
		d_gas_gains[radial_cut][cathode_setting]['fit'] = root.TF1('f_%s_%s' % (cathode_setting, str(radial_cut)), 'pol0', *l_time_range)
		d_gas_gains[radial_cut][cathode_setting]['graph'].Fit('f_%s_%s' % (cathode_setting, str(radial_cut)), 'NR')
		


		# create confidence interval
		numPoints = len(l_time_range)
		aX = np.zeros(numPoints, dtype=np.float64)
		aXErrLow = np.zeros(numPoints, dtype=np.float64)
		aXErrHigh = np.zeros(numPoints, dtype=np.float64)
		aY = np.zeros(numPoints, dtype=np.float64)
		aYErrLow = np.zeros(numPoints, dtype=np.float64)
		aYErrHigh = np.zeros(numPoints, dtype=np.float64)
		for i in xrange(numPoints):
			aX[i] = l_time_range[i]
			aY[i] = d_gas_gains[radial_cut][cathode_setting]['fit'].GetParameter(0)
			aYErrLow[i] = d_gas_gains[radial_cut][cathode_setting]['fit'].GetParError(0)
			aYErrHigh[i] = d_gas_gains[radial_cut][cathode_setting]['fit'].GetParError(0)
		
		d_gas_gains[radial_cut][cathode_setting]['conf_interval'] = root.TGraphAsymmErrors(numPoints, aX, aY, aXErrLow, aXErrHigh, aYErrLow, aYErrHigh)
		d_gas_gains[radial_cut][cathode_setting]['conf_interval'].SetName('conf_%s_%s' % (cathode_setting, str(radial_cut)))
		d_gas_gains[radial_cut][cathode_setting]['conf_interval'].SetLineColor(d_gas_gains[radial_cut][cathode_setting]['color'])
		d_gas_gains[radial_cut][cathode_setting]['conf_interval'].SetFillColor(d_gas_gains[radial_cut][cathode_setting]['color'])
		d_gas_gains[radial_cut][cathode_setting]['conf_interval'].SetFillStyle(d_gas_gains[radial_cut][cathode_setting]['fill_style'])
		d_gas_gains[radial_cut][cathode_setting]['conf_interval'].Draw('3 same')



		d_gas_gains[radial_cut]['canvas'].Update()

	# create legend for graphs

	for cathode_setting in l_cathode_voltages:
		d_gas_gains[radial_cut][cathode_setting]['tpavetext'].AddText('V_{c} = %skV: %.2f +/- %.2f PE' % (cathode_setting, d_gas_gains[radial_cut][cathode_setting]['fit'].GetParameter(0), d_gas_gains[radial_cut][cathode_setting]['fit'].GetParError(0)))
		d_gas_gains[radial_cut][cathode_setting]['tpavetext'].Draw('same')

	d_gas_gains[radial_cut]['canvas'].Update()

	neriX_analysis.save_plot(l_path_for_save, d_gas_gains[radial_cut]['canvas'], 'gas_gain_run_%d_method_avg_%s' % (run, str(radial_cut)))




raw_input('Press enter to continue...')
