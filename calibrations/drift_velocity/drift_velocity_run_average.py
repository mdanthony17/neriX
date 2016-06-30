import ROOT as root
from rootpy.plotting import Hist, Canvas
import neriX_analysis, neriX_datasets
from math import log, exp
import sys
import numpy as np
sys.path.append('../')
sys.path.append('./')
import drift_velocity_fit_results


if len(sys.argv) != 2:
	print 'Use is python gas_gain_method_study.py <run>'
	sys.exit()

run  = int(sys.argv[1])

d_drift_velocity = drift_velocity_fit_results.d_drift_velocity

if run == 15 or run == 16:
	l_cathode_voltages = [0.345, 0.700, 1.054, 1.500, 2.356]
else:
	l_cathode_voltages = [0.345, 1.054, 2.356, 5.500]


# -----------------------------------------
# -----------------------------------------
#  Set prelimary variables for each
#  cathode setting and radial cut
# -----------------------------------------
# -----------------------------------------

d_cathode_setting_data = {}
for cathode_setting in l_cathode_voltages:
	d_cathode_setting_data[cathode_setting] = {'a_time':[],
											   'a_time_err':[],
											   'a_drift_velocities':[],
											   'a_drift_velocities_err':[],
											   'a_gate_time':[],
											   'a_gate_time_err':[]
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
	d_cathode_setting_data[cathode_setting]['tpavetext_dv'] = root.TPaveText(.15,.8-movement,.5,.85-movement,'blNDC')
	d_cathode_setting_data[cathode_setting]['tpavetext_dv'].SetTextColor(color)
	d_cathode_setting_data[cathode_setting]['tpavetext_dv'].SetFillStyle(0)
	d_cathode_setting_data[cathode_setting]['tpavetext_dv'].SetBorderSize(0)

	d_cathode_setting_data[cathode_setting]['tpavetext_gtm'] = root.TPaveText(.15,.8-movement,.5,.85-movement,'blNDC')
	d_cathode_setting_data[cathode_setting]['tpavetext_gtm'].SetTextColor(color)
	d_cathode_setting_data[cathode_setting]['tpavetext_gtm'].SetFillStyle(0)
	d_cathode_setting_data[cathode_setting]['tpavetext_gtm'].SetBorderSize(0)


# get list of files since the same for all
l_files = d_drift_velocity.keys()
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


for file in l_files:
	current_file_dictionary = d_drift_velocity[file]
	current_cathode_setting = current_file_dictionary['cathode']
	
	d_cathode_setting_data[current_cathode_setting]['a_time'].append(neriX_analysis.convert_name_to_unix_time(file))
	d_cathode_setting_data[current_cathode_setting]['a_time_err'].append(0)
	d_cathode_setting_data[current_cathode_setting]['a_drift_velocities'].append(current_file_dictionary['drift_velocity'])
	d_cathode_setting_data[current_cathode_setting]['a_drift_velocities_err'].append(current_file_dictionary['drift_velocity_err'])
	d_cathode_setting_data[current_cathode_setting]['a_gate_time'].append(current_file_dictionary['mean_gate'])
	d_cathode_setting_data[current_cathode_setting]['a_gate_time_err'].append(current_file_dictionary['mean_gate_err'])

	

# -----------------------------------------
# -----------------------------------------
#  Fill graphs with weighted averages
#  and fit with pol0
# -----------------------------------------
# -----------------------------------------

c1 = Canvas(1900, 700)
c1.Divide(2)

firstDrawDV = True
firstDrawGateTime = True

for cathode_setting in l_cathode_voltages:

	# -----------------------------------------
	# -----------------------------------------
	#  Start with drift velocity
	# -----------------------------------------
	# -----------------------------------------


	c1.cd(1)

	d_cathode_setting_data[cathode_setting]['graph_dv'] = root.TGraphErrors(len(d_cathode_setting_data[cathode_setting]['a_time']), np.array(d_cathode_setting_data[cathode_setting]['a_time'], dtype=np.float64), np.array(d_cathode_setting_data[cathode_setting]['a_drift_velocities'], dtype=np.float64), np.array(d_cathode_setting_data[cathode_setting]['a_time_err'], dtype=np.float64), np.array(d_cathode_setting_data[cathode_setting]['a_drift_velocities_err'], dtype=np.float64))
	
	d_cathode_setting_data[cathode_setting]['graph_dv'].SetTitle('Drift Velocity Run Average')
	d_cathode_setting_data[cathode_setting]['graph_dv'].SetMarkerStyle(20)
	d_cathode_setting_data[cathode_setting]['graph_dv'].GetXaxis().SetLimits(*l_time_range)
	d_cathode_setting_data[cathode_setting]['graph_dv'].GetYaxis().SetRangeUser(*l_drift_velocity_range)

	d_cathode_setting_data[cathode_setting]['graph_dv'].SetMarkerColor(d_cathode_setting_data[cathode_setting]['color'])
	d_cathode_setting_data[cathode_setting]['graph_dv'].SetLineColor(d_cathode_setting_data[cathode_setting]['color'])
	if firstDrawDV:
		d_cathode_setting_data[cathode_setting]['graph_dv'].Draw('ap')
		firstDrawDV = False
		d_cathode_setting_data[cathode_setting]['graph_dv'].GetYaxis().SetTitle('Drift Velocity [mm/s]')
		d_cathode_setting_data[cathode_setting]['graph_dv'].GetHistogram().GetXaxis().SetTimeDisplay(1)
		d_cathode_setting_data[cathode_setting]['graph_dv'].GetHistogram().GetXaxis().SetTimeFormat('%m-%d')
	
	else:
		d_cathode_setting_data[cathode_setting]['graph_dv'].Draw('p same')

	# fit to straight line
	d_cathode_setting_data[cathode_setting]['fit_dv'] = root.TF1('f_dv_%s' % (cathode_setting), 'pol0', *l_time_range)
	d_cathode_setting_data[cathode_setting]['graph_dv'].Fit('f_dv_%s' % (cathode_setting), 'NR')
	
	d_cathode_setting_data[cathode_setting]['fit_dv'].SetLineColor(d_cathode_setting_data[cathode_setting]['color'])
	d_cathode_setting_data[cathode_setting]['fit_dv'].Draw('same')


	# -----------------------------------------
	# -----------------------------------------
	#  Then do gate time mean
	# -----------------------------------------
	# -----------------------------------------

	c1.cd(2)

	d_cathode_setting_data[cathode_setting]['graph_gtm'] = root.TGraphErrors(len(d_cathode_setting_data[cathode_setting]['a_time']), np.array(d_cathode_setting_data[cathode_setting]['a_time'], dtype=np.float64), np.array(d_cathode_setting_data[cathode_setting]['a_gate_time'], dtype=np.float64), np.array(d_cathode_setting_data[cathode_setting]['a_time_err'], dtype=np.float64), np.array(d_cathode_setting_data[cathode_setting]['a_gate_time_err'], dtype=np.float64))
	
	d_cathode_setting_data[cathode_setting]['graph_gtm'].SetTitle('Gate Time Mean Run Average')
	d_cathode_setting_data[cathode_setting]['graph_gtm'].SetMarkerStyle(20)
	d_cathode_setting_data[cathode_setting]['graph_gtm'].GetXaxis().SetLimits(*l_time_range)
	d_cathode_setting_data[cathode_setting]['graph_gtm'].GetYaxis().SetRangeUser(*l_gate_time_range)

	d_cathode_setting_data[cathode_setting]['graph_gtm'].SetMarkerColor(d_cathode_setting_data[cathode_setting]['color'])
	d_cathode_setting_data[cathode_setting]['graph_gtm'].SetLineColor(d_cathode_setting_data[cathode_setting]['color'])
	if firstDrawGateTime:
		d_cathode_setting_data[cathode_setting]['graph_gtm'].Draw('ap')
		firstDrawGateTime = False
		d_cathode_setting_data[cathode_setting]['graph_gtm'].GetYaxis().SetTitle('Gate Time [us after main S2]')
		d_cathode_setting_data[cathode_setting]['graph_gtm'].GetHistogram().GetXaxis().SetTimeDisplay(1)
		d_cathode_setting_data[cathode_setting]['graph_gtm'].GetHistogram().GetXaxis().SetTimeFormat('%m-%d')
	
	else:
		d_cathode_setting_data[cathode_setting]['graph_gtm'].Draw('p same')

	# fit to straight line
	d_cathode_setting_data[cathode_setting]['fit_gtm'] = root.TF1('f_gtm_%s' % (cathode_setting), 'pol0', *l_time_range)
	d_cathode_setting_data[cathode_setting]['graph_gtm'].Fit('f_gtm_%s' % (cathode_setting), 'NR')
	
	d_cathode_setting_data[cathode_setting]['fit_gtm'].SetLineColor(d_cathode_setting_data[cathode_setting]['color'])
	d_cathode_setting_data[cathode_setting]['fit_gtm'].Draw('same')

	c1.Update()



# create legend for graphs
for cathode_setting in l_cathode_voltages:
	c1.cd(1)
	d_cathode_setting_data[cathode_setting]['tpavetext_gtm'].AddText('V_{c} = %skV: %.2f +/- %.2e mm/s' % (cathode_setting, d_cathode_setting_data[cathode_setting]['fit_dv'].GetParameter(0), d_cathode_setting_data[cathode_setting]['fit_dv'].GetParError(0)))
	d_cathode_setting_data[cathode_setting]['tpavetext_gtm'].Draw('same')

	c1.cd(2)
	d_cathode_setting_data[cathode_setting]['tpavetext_dv'].AddText('V_{c} = %skV: %.2f +/- %.2e us' % (cathode_setting, d_cathode_setting_data[cathode_setting]['fit_gtm'].GetParameter(0), d_cathode_setting_data[cathode_setting]['fit_gtm'].GetParError(0)))
	d_cathode_setting_data[cathode_setting]['tpavetext_dv'].Draw('same')

c1.Update()

neriX_analysis.save_plot(l_path_for_save, c1, 'drift_velocity_run_%d' % (run))


raw_input('Press enter to continue...')
