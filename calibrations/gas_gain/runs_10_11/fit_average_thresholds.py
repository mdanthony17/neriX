import ROOT as root
from rootpy.plotting import Hist, Canvas
import neriX_analysis
from math import log, exp
import array, sys
import numpy as np
sys.path.append('/Users/Matt/Desktop/Xenon/neriX/calibrations/gas_gain/sub_analyses/')
import gas_gain_systematic_results_unfixed_thresholds



def fit_average_thresholds(run, draw_fits=False):
	methods = gas_gain_systematic_results_unfixed_thresholds.methods
	radial_cut = (0., 18.)

	gas_gain_results = gas_gain_systematic_results_unfixed_thresholds.gas_gain_results[run]




	# get list of files since the same for all
	l_files = gas_gain_results[methods[0]][radial_cut].keys()
	l_files.sort()

	# ranges for graphs
	l_time_ranges = [neriX_analysis.convert_name_to_unix_time(l_files[0])-3e5]
	if run == 10:
		pass
	elif run == 11:
		l_time_ranges.append(1419272384) # Dec 22, 2014
	l_time_ranges.append(neriX_analysis.convert_name_to_unix_time(l_files[-1])+3e5)
	
	
	l_fd_center_tot_range = [3, 25]
	l_fd_center_top_range = [0, 8]
	l_fd_shape_tot_range = [0, 15]
	l_fd_shape_top_range = [0, 4]

	l_path_for_save = ['sub_analyses', 'results', 'threshold_parameter_trends', 'run_%d' % run]

	d_pars_weighted_avg = {'fd_center_tot':[], 'fd_shape_tot':[], 'fd_center_top':[], 'fd_shape_top':[]}
	for par in d_pars_weighted_avg:
		d_pars_weighted_avg[par] = {'values':[], 'errs':[], 'canvas':0, 'fits':[], 'graph':0, 'range':0, 'conf_intervals':[]}

		# set range according to parameter
		if par == 'fd_center_tot':
			d_pars_weighted_avg[par]['range'] = [8, 17]
		elif par == 'fd_shape_tot':
			d_pars_weighted_avg[par]['range'] = [0, 5]
		elif par == 'fd_center_top':
			d_pars_weighted_avg[par]['range'] = [0, 6]
		elif par == 'fd_shape_top':
			d_pars_weighted_avg[par]['range'] = [0, 4]



	l_times = []

	for i, filename in enumerate(l_files):
		# check that fit actually succeeded for all methods
		# if not, don't look at point
		b_methods_all_successful = True
		for method in methods:
			if not gas_gain_results[method][radial_cut][filename]['fit_successful']:
				b_methods_all_successful = False
		if not b_methods_all_successful:
			continue
		
		l_times.append(neriX_analysis.convert_name_to_unix_time(filename))

		# calculate values and weights
		for par in d_pars_weighted_avg:
			l_values = [0. for i in xrange(len(methods))]
			l_errs = [0. for i in xrange(len(methods))]
			l_weights = [0. for i in xrange(len(methods))]
			for j, method in enumerate(methods):
				l_values[j] = gas_gain_results[method][radial_cut][filename][par]
				l_errs[j] = gas_gain_results[method][radial_cut][filename][par+'_err']
				l_weights[j] = 1./l_errs[j]**2.

			# calculate weighted avg and uncertainty
			d_pars_weighted_avg[par]['values'].append(np.dot(l_values, l_weights) / float(np.sum(l_weights)))
			d_pars_weighted_avg[par]['errs'].append(1./np.sum(l_weights)**0.5)



	for par in d_pars_weighted_avg:
		if draw_fits:
			d_pars_weighted_avg[par]['canvas'] = Canvas()
		d_pars_weighted_avg[par]['graph'] = root.TGraphErrors(len(l_times), np.array(l_times, dtype=np.float64), np.array(d_pars_weighted_avg[par]['values'], dtype=np.float64), np.zeros(len(l_times), dtype=np.float64), np.array(d_pars_weighted_avg[par]['errs'], dtype=np.float64))

		# set graph parameters
		d_pars_weighted_avg[par]['graph'].SetTitle('%s Weighted Average - run %d' % (par, run))
		d_pars_weighted_avg[par]['graph'].SetMarkerStyle(20)
		d_pars_weighted_avg[par]['graph'].GetXaxis().SetLimits(l_time_ranges[0], l_time_ranges[-1])
		d_pars_weighted_avg[par]['graph'].GetYaxis().SetRangeUser(*d_pars_weighted_avg[par]['range'])

		if draw_fits:
			d_pars_weighted_avg[par]['graph'].Draw('ap')
		
		# loop through each of the time ranges (length of list minus one)
		for j in xrange(len(l_time_ranges) - 1):
			d_pars_weighted_avg[par]['fits'].append(root.TF1('f_%s_%d' % (par, j), 'pol0', l_time_ranges[j], l_time_ranges[j+1]))
			d_pars_weighted_avg[par]['graph'].Fit('f_%s_%d' % (par, j), 'NQR')
			
			d_pars_weighted_avg[par]['conf_intervals'].append(root.TGraphAsymmErrors(*neriX_analysis.create_graph_with_confidence_interval_for_fit(d_pars_weighted_avg[par]['graph'], root.TVirtualFitter.GetFitter())))
			d_pars_weighted_avg[par]['conf_intervals'][-1].SetLineColor(root.kBlue)
			d_pars_weighted_avg[par]['conf_intervals'][-1].SetFillColor(root.kBlue)
			d_pars_weighted_avg[par]['conf_intervals'][-1].SetFillStyle(3005)
			
			if draw_fits:
				d_pars_weighted_avg[par]['fits'][-1].Draw('same')
				d_pars_weighted_avg[par]['conf_intervals'][-1].Draw('3 same')
				d_pars_weighted_avg[par]['canvas'].Update()
				
		if draw_fits:
			neriX_analysis.save_plot(l_path_for_save, d_pars_weighted_avg[par]['canvas'], 'trend_par_%s' % (par))
			

	if draw_fits:
		raw_input('Press enter to continue...')

	return l_time_ranges, d_pars_weighted_avg





if __name__ == '__main__':
	if len(sys.argv) != 2:
		print 'Use is python gas_gain_method_study.py <run>'
		sys.exit()

	fit_average_thresholds(int(sys.argv[1]), draw_fits=True)
