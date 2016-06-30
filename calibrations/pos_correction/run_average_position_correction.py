import ROOT as root
from rootpy.plotting import Hist, Canvas
import neriX_analysis, neriX_datasets
from math import log, exp
import sys
import numpy as np
sys.path.append('../')
sys.path.append('./')
import pos_correction_fit_results


if len(sys.argv) != 2:
	print 'Use is python gas_gain_method_study.py <run>'
	sys.exit()

run  = int(sys.argv[1])

d_pos_correction = pos_correction_fit_results.d_pos_correction_fits

if run == 15 or run == 16:
	l_cathode_voltages = [0.345, 0.700, 1.054, 1.500, 2.356]
else:
	print 'Only setup for runs 15 and 16.'
	sys.exit()

# -----------------------------------------
# -----------------------------------------
#  Set prelimary variables for each
#  cathode setting and radial cut
# -----------------------------------------
# -----------------------------------------



d_avgs_to_complete = {'s1_z_intercept':[0,1,[0.7, 1.1]], 's1_z_slope':[0,2,[-0.03, 0.01]], 's1_r_intercept':[0,3,[0.7, 1.1]], 's1_r_slope':[0,4,[-0.03, 0.01]], 'e_lifetime':[1,1,[0, 1000]], 's2_r_intercept':[1,3,[0.8, 1.2]], 's2_r_slope':[1,4,[-0.15, 0.45]]}
d_cathode_setting_data = {}
for cathode_setting in l_cathode_voltages:
	d_cathode_setting_data[cathode_setting] = {'a_time':[],
											   'a_time_err':[]
											   }


	if cathode_setting == 0.345:
		color = root.kMagenta
		movement = 0.0
		fill_style = 3004
	elif cathode_setting == 0.700:
		color = root.kBlue
		movement = 0.05
		fill_style = 3005
	elif cathode_setting == 1.054:
		color = root.kGreen+1
		movement = 0.1
		fill_style = 3006
	elif cathode_setting == 1.500:
		color = root.kOrange+1
		movement = 0.15
		fill_style = 3007
	elif cathode_setting == 2.356:
		color = root.kRed
		movement = 0.2
		fill_style = 3008
	

	d_cathode_setting_data[cathode_setting]['color'] = color
	d_cathode_setting_data[cathode_setting]['fill_style'] = fill_style

	for cat_name in d_avgs_to_complete:
		d_cathode_setting_data[cathode_setting]['a_%s' % (cat_name)] = []
		d_cathode_setting_data[cathode_setting]['a_%s_err' % (cat_name)] = []

		d_cathode_setting_data[cathode_setting]['tpavetext_%s' % (cat_name)] = root.TPaveText(.15,.8-movement,.5,.85-movement,'blNDC')
		d_cathode_setting_data[cathode_setting]['tpavetext_%s' % (cat_name)] = root.TPaveText(.15,.8-movement,.5,.85-movement,'blNDC')
		d_cathode_setting_data[cathode_setting]['tpavetext_%s' % (cat_name)].SetTextColor(color)
		d_cathode_setting_data[cathode_setting]['tpavetext_%s' % (cat_name)].SetFillStyle(0)
		d_cathode_setting_data[cathode_setting]['tpavetext_%s' % (cat_name)].SetBorderSize(0)




# get list of files since the same for all
l_files = d_pos_correction.keys()
l_files.sort()

# ranges for graphs
l_time_range = [neriX_analysis.convert_name_to_unix_time(l_files[0])-3e5, neriX_analysis.convert_name_to_unix_time(l_files[-1])+3e5]

l_gate_time_range = [1.3, 1.7]
l_drift_velocity_range = [1.4, 2.5]

l_path_for_save = ['results', 'run_avgs', 'run_%d' % run]


# -----------------------------------------
# -----------------------------------------
#  Fill arrays for radial cut and cathode
#  setting with weighted averages
# -----------------------------------------
# -----------------------------------------

d_first_draw = {}
d_run_average = {}
for file in l_files:
	current_file_dictionary = d_pos_correction[file]
	
	for current_cathode_setting in current_file_dictionary:
		
		d_cathode_setting_data[current_cathode_setting]['a_time'].append(neriX_analysis.convert_name_to_unix_time(file))
		d_cathode_setting_data[current_cathode_setting]['a_time_err'].append(0)
		
		d_run_average[current_cathode_setting] = {}
		
		for cat_name in d_avgs_to_complete:
			d_cathode_setting_data[current_cathode_setting]['a_%s' % (cat_name)].append(current_file_dictionary[current_cathode_setting][cat_name])
			d_cathode_setting_data[current_cathode_setting]['a_%s_err' % (cat_name)].append(current_file_dictionary[current_cathode_setting]['%s_err' % (cat_name)])
		
			d_first_draw[cat_name] = True

# -----------------------------------------
# -----------------------------------------
#  Fill graphs with weighted averages
#  and fit with pol0
# -----------------------------------------
# -----------------------------------------

l_canvases = [Canvas(1600, 1400), Canvas(1600, 1400)]
l_canvases[0].Divide(2, 2)
l_canvases[1].Divide(2, 2)



for cathode_setting in l_cathode_voltages:

	# -----------------------------------------
	# -----------------------------------------
	#  Start with drift velocity
	# -----------------------------------------
	# -----------------------------------------

	for cat_name in d_avgs_to_complete:
		current_canvas = l_canvases[d_avgs_to_complete[cat_name][0]]
		current_pad = d_avgs_to_complete[cat_name][1]
		current_y_range = d_avgs_to_complete[cat_name][2]

		current_canvas.cd(current_pad)

		d_cathode_setting_data[cathode_setting]['g_%s' % (cat_name)] = root.TGraphErrors(len(d_cathode_setting_data[cathode_setting]['a_time']), np.array(d_cathode_setting_data[cathode_setting]['a_time'], dtype=np.float64), np.array(d_cathode_setting_data[cathode_setting]['a_%s' % (cat_name)], dtype=np.float64), np.array(d_cathode_setting_data[cathode_setting]['a_time_err'], dtype=np.float64), np.array(d_cathode_setting_data[cathode_setting]['a_%s_err' % (cat_name)], dtype=np.float64))
	
		d_cathode_setting_data[cathode_setting]['g_%s' % (cat_name)].SetTitle(cat_name)
		d_cathode_setting_data[cathode_setting]['g_%s' % (cat_name)].SetMarkerStyle(20)
		d_cathode_setting_data[cathode_setting]['g_%s' % (cat_name)].GetXaxis().SetLimits(*l_time_range)
		d_cathode_setting_data[cathode_setting]['g_%s' % (cat_name)].GetYaxis().SetRangeUser(*current_y_range)

		d_cathode_setting_data[cathode_setting]['g_%s' % (cat_name)].SetMarkerColor(d_cathode_setting_data[cathode_setting]['color'])
		d_cathode_setting_data[cathode_setting]['g_%s' % (cat_name)].SetLineColor(d_cathode_setting_data[cathode_setting]['color'])
		if d_first_draw[cat_name]:
			d_cathode_setting_data[cathode_setting]['g_%s' % (cat_name)].Draw('ap')
			d_first_draw[cat_name] = False
			d_cathode_setting_data[cathode_setting]['g_%s' % (cat_name)].GetYaxis().SetTitle('')
			d_cathode_setting_data[cathode_setting]['g_%s' % (cat_name)].GetHistogram().GetXaxis().SetTimeDisplay(1)
			d_cathode_setting_data[cathode_setting]['g_%s' % (cat_name)].GetHistogram().GetXaxis().SetTimeFormat('%m-%d')
		
		else:
			d_cathode_setting_data[cathode_setting]['g_%s' % (cat_name)].Draw('p same')

		# fit to straight line
		d_cathode_setting_data[cathode_setting]['f_%s' % (cat_name)] = root.TF1('f_dv_%s' % (cathode_setting), 'pol0', *l_time_range)
		d_cathode_setting_data[cathode_setting]['g_%s' % (cat_name)].Fit('f_dv_%s' % (cathode_setting), 'NR')
		
		d_cathode_setting_data[cathode_setting]['f_%s' % (cat_name)].SetLineColor(d_cathode_setting_data[cathode_setting]['color'])
		d_cathode_setting_data[cathode_setting]['f_%s' % (cat_name)].Draw('same')


		d_cathode_setting_data[cathode_setting]['tpavetext_%s' % (cat_name)].AddText('V_{c} = %skV: %.2e +/- %.2e mm/s' % (cathode_setting, d_cathode_setting_data[cathode_setting]['f_%s' % (cat_name)].GetParameter(0), d_cathode_setting_data[cathode_setting]['f_%s' % (cat_name)].GetParError(0)))
		d_cathode_setting_data[cathode_setting]['tpavetext_%s' % (cat_name)].Draw('same')

		current_canvas.Update()


		d_run_average[cathode_setting][cat_name] = d_cathode_setting_data[cathode_setting]['f_%s' % (cat_name)].GetParameter(0)
		d_run_average[cathode_setting]['%s_err' % (cat_name)] = d_cathode_setting_data[cathode_setting]['f_%s' % (cat_name)].GetParError(0)



neriX_analysis.save_plot(l_path_for_save, l_canvases[0], 's1_position_correction_run_%d' % (run))
neriX_analysis.save_plot(l_path_for_save, l_canvases[1], 's2_position_correction_run_%d' % (run))

print '\n\nCopy and paste into fit summary file!\n\n\n'

s_to_return = ''

s_to_return += '# run %d position correction averages\n' % (run)
s_to_return += 'd_run_average[%d] = %s' % (run, str(d_run_average))
s_to_return += '\n\n'

print s_to_return

#raw_input('Press enter to continue...')
