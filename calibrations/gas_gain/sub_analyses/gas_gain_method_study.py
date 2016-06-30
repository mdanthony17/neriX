import ROOT as root
from rootpy.plotting import Hist, Canvas
import neriX_analysis
from math import log, exp
import array, sys
import numpy as np
sys.path.append('../')
sys.path.append('./')
import gas_gain_systematic_results_unfixed_thresholds


if len(sys.argv) != 2:
	print 'Use is python gas_gain_method_study.py <run>'
	sys.exit()

run  = int(sys.argv[1])
methods = gas_gain_systematic_results_unfixed_thresholds.methods
radii_sets = gas_gain_systematic_results_unfixed_thresholds.radii_sets

gas_gain_results = gas_gain_systematic_results_unfixed_thresholds.gas_gain_results[run]

d_radial_cuts = {}
for radial_cut in radii_sets:
	d_radial_cuts[radial_cut] = {}
	for method in methods:
		d_radial_cuts[radial_cut][method] = {}
		
		# set color for each method
		if method == 'gate_pi':
			color = root.kBlue
			style = 3004
			movement = 0
		elif method == 'cathode_pi':
			color = root.kRed
			style = 3005
			movement = 0.05
		else:
			color = root.kGreen
			style = 3006
			movement = 0.10
		d_radial_cuts[radial_cut][method]['color'] = color
		d_radial_cuts[radial_cut][method]['fill_style'] = style
		d_radial_cuts[radial_cut][method]['tpavetext'] = root.TPaveText(.15,.8-movement,.5,.85-movement,'blNDC')
		d_radial_cuts[radial_cut][method]['tpavetext'].SetTextColor(color)
		d_radial_cuts[radial_cut][method]['tpavetext'].SetFillStyle(0)
		d_radial_cuts[radial_cut][method]['tpavetext'].SetBorderSize(0)

# get list of files since the same for all
l_files = gas_gain_results[methods[0]][radii_sets[0]].keys()
l_files.sort()

# ranges for graphs
l_time_range = [neriX_analysis.convert_name_to_unix_time(l_files[0])-3e5, neriX_analysis.convert_name_to_unix_time(l_files[-1])+3e5]
l_gas_gain_range = [13, 20]

l_path_for_save = ['results', 'systematic_method_study', 'run_%d' % run]


for radial_cut in radii_sets:
	# create canvas for all methods with the same radial cut
	d_radial_cuts[radial_cut]['canvas'] = Canvas()
	firstDraw = True
	
	for method in methods:
		d_radial_cuts[radial_cut][method]['a_time'] = np.zeros(len(l_files), dtype=np.float64)
		d_radial_cuts[radial_cut][method]['a_time_err'] = np.zeros(len(l_files), dtype=np.float64)
		d_radial_cuts[radial_cut][method]['a_gas_gain_bot'] = np.zeros(len(l_files), dtype=np.float64)
		d_radial_cuts[radial_cut][method]['a_gas_gain_bot_err'] = np.zeros(len(l_files), dtype=np.float64)
		
		
		for i, filename in enumerate(l_files):
			d_radial_cuts[radial_cut][method]['a_time'][i] = neriX_analysis.convert_name_to_unix_time(filename)
			
			# check that fit actually succeeded
			if not gas_gain_results[method][radial_cut][filename]['fit_successful']:
				continue
		
			d_radial_cuts[radial_cut][method]['a_gas_gain_bot'][i] = gas_gain_results[method][radial_cut][filename]['mean_bot']
			d_radial_cuts[radial_cut][method]['a_gas_gain_bot_err'][i] = gas_gain_results[method][radial_cut][filename]['mean_bot_err']

		d_radial_cuts[radial_cut][method]['graph'] = root.TGraphErrors(len(l_files), d_radial_cuts[radial_cut][method]['a_time'], d_radial_cuts[radial_cut][method]['a_gas_gain_bot'], d_radial_cuts[radial_cut][method]['a_time_err'], d_radial_cuts[radial_cut][method]['a_gas_gain_bot_err'])
		
		d_radial_cuts[radial_cut][method]['graph'].SetTitle('Gas Gain Method Comparison - %.1f mm < R < %.1f mm' % (radial_cut[0], radial_cut[1]))
		d_radial_cuts[radial_cut][method]['graph'].SetMarkerStyle(20)
		d_radial_cuts[radial_cut][method]['graph'].GetXaxis().SetLimits(*l_time_range)
		d_radial_cuts[radial_cut][method]['graph'].GetYaxis().SetTitle('Gas gain [PE per extracted electron]')
		d_radial_cuts[radial_cut][method]['graph'].GetHistogram().GetXaxis().SetTimeDisplay(1)
		d_radial_cuts[radial_cut][method]['graph'].GetYaxis().SetTitle('Gas gain [PE per extracted electron]')
		d_radial_cuts[radial_cut][method]['graph'].GetHistogram().GetXaxis().SetTimeFormat('%m-%d')

		d_radial_cuts[radial_cut][method]['graph'].SetMarkerColor(d_radial_cuts[radial_cut][method]['color'])
		d_radial_cuts[radial_cut][method]['graph'].SetLineColor(d_radial_cuts[radial_cut][method]['color'])
		if firstDraw:
			d_radial_cuts[radial_cut][method]['graph'].Draw('ap')
			firstDraw = False
		else:
			d_radial_cuts[radial_cut][method]['graph'].Draw('p same')

		# fit to straight line
		d_radial_cuts[radial_cut][method]['fit'] = root.TF1('f_%s_%s' % (method, str(radial_cut)), 'pol0', *l_time_range)
		d_radial_cuts[radial_cut][method]['graph'].Fit('f_%s_%s' % (method, str(radial_cut)), 'NQR')

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
			aY[i] = d_radial_cuts[radial_cut][method]['fit'].GetParameter(0)
			aYErrLow[i] = d_radial_cuts[radial_cut][method]['fit'].GetParError(0)
			aYErrHigh[i] = d_radial_cuts[radial_cut][method]['fit'].GetParError(0)
		
		d_radial_cuts[radial_cut][method]['conf_interval'] = root.TGraphAsymmErrors(numPoints, aX, aY, aXErrLow, aXErrHigh, aYErrLow, aYErrHigh)
		d_radial_cuts[radial_cut][method]['conf_interval'].SetName('conf_%s_%s' % (method, str(radial_cut)))
		d_radial_cuts[radial_cut][method]['conf_interval'].SetLineColor(d_radial_cuts[radial_cut][method]['color'])
		d_radial_cuts[radial_cut][method]['conf_interval'].SetFillColor(d_radial_cuts[radial_cut][method]['color'])
		d_radial_cuts[radial_cut][method]['conf_interval'].SetFillStyle(d_radial_cuts[radial_cut][method]['fill_style'])
		d_radial_cuts[radial_cut][method]['conf_interval'].Draw('3 same')

	# create legend for graphs
	for method in methods:
		d_radial_cuts[radial_cut][method]['tpavetext'].AddText('%s: %.2f +/- %.2f PE' % (method, d_radial_cuts[radial_cut][method]['fit'].GetParameter(0), d_radial_cuts[radial_cut][method]['fit'].GetParError(0)))
		d_radial_cuts[radial_cut][method]['tpavetext'].Draw('same')

	d_radial_cuts[radial_cut]['canvas'].Update()

	neriX_analysis.save_plot(l_path_for_save, d_radial_cuts[radial_cut]['canvas'], 'gas_gain_method_comparison_%s' % (str(radial_cut)))






"""

fG1 = root.TF1('fG1', 'pol0', lLimits[0], lLimits[1])
#fG1.FixParameter(0, 0)

fG1.SetLineStyle(9)
fG1.SetLineWidth(1)


gVoltageVsGain.Draw('ap')
gVoltageVsGain.Fit('fG1')

fG1.Draw('same')

g1Value = fG1.GetParameter(1)
g1Err = fG1.GetParError(1)

sFitInfo1 = 'g1 = %.4f +/- %.4f' % (g1Value, g1Err)
pt1 = root.TPaveText(.2,.75,.6,.85,'blNDC')
text1 = pt1.AddText(sFitInfo1)
pt1.SetTextColor(root.kBlue)
pt1.SetFillStyle(0)
pt1.SetBorderSize(0)
pt1.Draw('same')




c1.Update()
"""

raw_input('Press enter to continue...')
