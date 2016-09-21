gas_gain_results = {}

runs = [16]
methods = ['gate_pi']
radii_sets = [(0., 0.85)]

for run in runs:
	gas_gain_results[run] = {}
	for method in methods:
		gas_gain_results[run][method] = {}
		for radii_set in radii_sets:
			gas_gain_results[run][method][radii_set] = {}



# ---------------------------------------
# ---------------------------------------
#  RUN 11
# ---------------------------------------
# ---------------------------------------


#nerix_160404_1059
gas_gain_results[16]['gate_pi'][(0.00,0.85)]['nerix_160404_1059'] = {'num_electrons':4, 'fit_successful':True, 'mean_bot':12.97, 'mean_bot_err':0.08, 'mean_top':15.24577, 'mean_top_err':0.12398, 'width_bot':5.29733, 'width_bot_err':0.06146, 'width_top':5.78234, 'width_top_err':0.10182, 'correlation':0.04553, 'correlation_err':0.01423, 'p1_ampl':28769.52771, 'p1_ampl_err':522.84385, 'p2_ampl':0.27347, 'p2_ampl_err':0.00930, 'p3_ampl':0.07999, 'p3_ampl_err':0.00511, 'p4_ampl':0.03000, 'p4_ampl_err':0.00404, 'fd_center_tot':2.01943, 'fd_center_tot_err':4.03406, 'fd_shape_tot':0.50000, 'fd_shape_tot_err':6.49184, 'fd_center_top':5.32753, 'fd_center_top_err':0.37545, 'fd_shape_top':1.71542, 'fd_shape_top_err':0.19890, 'Chi2':771.5, 'NDF':887, 'ln(L)':-206.532}




#nerix_160407_1357
gas_gain_results[16]['gate_pi'][(0.00,0.85)]['nerix_160407_1357'] = {'num_electrons':4, 'fit_successful':True, 'mean_bot':12.76, 'mean_bot_err':0.08, 'mean_top':15.12523, 'mean_top_err':0.14028, 'width_bot':5.35385, 'width_bot_err':0.06514, 'width_top':5.71162, 'width_top_err':0.10154, 'correlation':0.04845, 'correlation_err':0.01440, 'p1_ampl':30256.71673, 'p1_ampl_err':634.11707, 'p2_ampl':0.26468, 'p2_ampl_err':0.00930, 'p3_ampl':0.08085, 'p3_ampl_err':0.00493, 'p4_ampl':0.03000, 'p4_ampl_err':0.00661, 'fd_center_tot':4.22474, 'fd_center_tot_err':1.27015, 'fd_shape_tot':0.50000, 'fd_shape_tot_err':0.47463, 'fd_center_top':5.65434, 'fd_center_top_err':0.42969, 'fd_shape_top':1.98589, 'fd_shape_top_err':0.23040, 'Chi2':741.8, 'NDF':887, 'ln(L)':-220.021}




#nerix_160412_0919
gas_gain_results[16]['gate_pi'][(0.00,0.85)]['nerix_160412_0919'] = {'num_electrons':4, 'fit_successful':True, 'mean_bot':12.92, 'mean_bot_err':0.07, 'mean_top':15.15619, 'mean_top_err':0.13401, 'width_bot':5.25915, 'width_bot_err':0.06108, 'width_top':5.94821, 'width_top_err':0.10168, 'correlation':0.03199, 'correlation_err':0.01395, 'p1_ampl':32314.60316, 'p1_ampl_err':647.96506, 'p2_ampl':0.27701, 'p2_ampl_err':0.00935, 'p3_ampl':0.07408, 'p3_ampl_err':0.00551, 'p4_ampl':0.03170, 'p4_ampl_err':0.00648, 'fd_center_tot':4.10120, 'fd_center_tot_err':1.34288, 'fd_shape_tot':0.50000, 'fd_shape_tot_err':0.47755, 'fd_center_top':5.89020, 'fd_center_top_err':0.38000, 'fd_shape_top':2.03414, 'fd_shape_top_err':0.19669, 'Chi2':766.9, 'NDF':887, 'ln(L)':-204.440}




#nerix_160414_1215
gas_gain_results[16]['gate_pi'][(0.00,0.85)]['nerix_160414_1215'] = {'num_electrons':4, 'fit_successful':True, 'mean_bot':13.00, 'mean_bot_err':0.07, 'mean_top':15.21651, 'mean_top_err':0.13573, 'width_bot':5.18678, 'width_bot_err':0.06068, 'width_top':5.88304, 'width_top_err':0.10611, 'correlation':0.04003, 'correlation_err':0.01367, 'p1_ampl':32338.28578, 'p1_ampl_err':656.10991, 'p2_ampl':0.27502, 'p2_ampl_err':0.00910, 'p3_ampl':0.06965, 'p3_ampl_err':0.00456, 'p4_ampl':0.03000, 'p4_ampl_err':0.01228, 'fd_center_tot':2.16244, 'fd_center_tot_err':3.44752, 'fd_shape_tot':0.50000, 'fd_shape_tot_err':6.12765, 'fd_center_top':5.69417, 'fd_center_top_err':0.41216, 'fd_shape_top':2.01156, 'fd_shape_top_err':0.20787, 'Chi2':796.5, 'NDF':887, 'ln(L)':-220.240}




#nerix_160418_0919
gas_gain_results[16]['gate_pi'][(0.00,0.85)]['nerix_160418_0919'] = {'num_electrons':4, 'fit_successful':True, 'mean_bot':13.49, 'mean_bot_err':0.07, 'mean_top':15.82935, 'mean_top_err':0.11488, 'width_bot':5.46602, 'width_bot_err':0.05678, 'width_top':5.98955, 'width_top_err':0.09387, 'correlation':0.04806, 'correlation_err':0.01265, 'p1_ampl':36802.06038, 'p1_ampl_err':612.43547, 'p2_ampl':0.28565, 'p2_ampl_err':0.00862, 'p3_ampl':0.07993, 'p3_ampl_err':0.00573, 'p4_ampl':0.04315, 'p4_ampl_err':0.00899, 'fd_center_tot':1.29166, 'fd_center_tot_err':14.30554, 'fd_shape_tot':0.50000, 'fd_shape_tot_err':6.97802, 'fd_center_top':5.53543, 'fd_center_top_err':0.37602, 'fd_shape_top':1.98445, 'fd_shape_top_err':0.19054, 'Chi2':875.9, 'NDF':887, 'ln(L)':-223.737}




#nerix_160421_1356
gas_gain_results[16]['gate_pi'][(0.00,0.85)]['nerix_160421_1356'] = {'num_electrons':4, 'fit_successful':True, 'mean_bot':12.84, 'mean_bot_err':0.07, 'mean_top':15.55765, 'mean_top_err':0.12422, 'width_bot':5.32570, 'width_bot_err':0.06376, 'width_top':5.76789, 'width_top_err':0.09858, 'correlation':0.03834, 'correlation_err':0.01376, 'p1_ampl':31379.07861, 'p1_ampl_err':560.59289, 'p2_ampl':0.30122, 'p2_ampl_err':0.00958, 'p3_ampl':0.09138, 'p3_ampl_err':0.00623, 'p4_ampl':0.04822, 'p4_ampl_err':0.00861, 'fd_center_tot':4.32057, 'fd_center_tot_err':1.40031, 'fd_shape_tot':0.50000, 'fd_shape_tot_err':0.62614, 'fd_center_top':5.51485, 'fd_center_top_err':0.39524, 'fd_shape_top':1.85104, 'fd_shape_top_err':0.21581, 'Chi2':792.3, 'NDF':887, 'ln(L)':-210.042}




#nerix_160428_1124
gas_gain_results[16]['gate_pi'][(0.00,0.85)]['nerix_160428_1124'] = {'num_electrons':3, 'fit_successful':True, 'mean_bot':13.18, 'mean_bot_err':0.07, 'mean_top':16.22577, 'mean_top_err':0.07123, 'width_bot':5.60067, 'width_bot_err':0.05810, 'width_top':5.46993, 'width_top_err':0.06232, 'correlation':0.01763, 'correlation_err':0.01295, 'p1_ampl':28418.92574, 'p1_ampl_err':355.19836, 'p2_ampl':0.22751, 'p2_ampl_err':0.00759, 'p3_ampl':0.06081, 'p3_ampl_err':0.00410, 'fd_center_tot':0.00013, 'fd_center_tot_err':15.96235, 'fd_shape_tot':0.50006, 'fd_shape_tot_err':7.16711, 'fd_center_top':2.95314, 'fd_center_top_err':0.30587, 'fd_shape_top':0.11923, 'fd_shape_top_err':6.56168, 'Chi2':755.1, 'NDF':888, 'ln(L)':-200.171}




#nerix_160502_0940
gas_gain_results[16]['gate_pi'][(0.00,0.85)]['nerix_160502_0940'] = {'num_electrons':4, 'fit_successful':True, 'mean_bot':12.83, 'mean_bot_err':0.06, 'mean_top':15.77496, 'mean_top_err':0.09892, 'width_bot':5.20613, 'width_bot_err':0.04970, 'width_top':5.68548, 'width_top_err':0.08148, 'correlation':0.05583, 'correlation_err':0.01158, 'p1_ampl':40731.54767, 'p1_ampl_err':632.09236, 'p2_ampl':0.31496, 'p2_ampl_err':0.00846, 'p3_ampl':0.07878, 'p3_ampl_err':0.00430, 'p4_ampl':0.03000, 'p4_ampl_err':0.00319, 'fd_center_tot':1.23648, 'fd_center_tot_err':0.11428, 'fd_shape_tot':0.50055, 'fd_shape_tot_err':0.00214, 'fd_center_top':4.90658, 'fd_center_top_err':0.40514, 'fd_shape_top':1.97658, 'fd_shape_top_err':0.23887, 'Chi2':770.6, 'NDF':887, 'ln(L)':-205.318}




#nerix_160505_0935 (BAD)
#gas_gain_results[16]['gate_pi'][(0.00,0.85)]['nerix_160505_0935'] = {'num_electrons':1, 'fit_successful':True, 'mean_bot':12.00, 'mean_bot_err':0.02, 'mean_top':20.00000, 'mean_top_err':0.03494, 'width_bot':11.87192, 'width_bot_err':0.23682, 'width_top':17.50306, 'width_top_err':0.44738, 'correlation':0.36297, 'correlation_err':0.02938, 'p1_ampl':10148.10418, 'p1_ampl_err':266.46228, 'fd_center_tot':0.00435, 'fd_center_tot_err':18.18630, 'fd_shape_tot':0.50000, 'fd_shape_tot_err':8.58027, 'fd_center_top':4.55953, 'fd_center_top_err':0.23441, 'fd_shape_top':0.58918, 'fd_shape_top_err':0.11352, 'Chi2':2138.2, 'NDF':890, 'ln(L)':-589.834}




#nerix_160509_1050
gas_gain_results[16]['gate_pi'][(0.00,0.85)]['nerix_160509_1050'] = {'num_electrons':4, 'fit_successful':True, 'mean_bot':12.80, 'mean_bot_err':0.07, 'mean_top':15.44016, 'mean_top_err':0.11372, 'width_bot':5.26997, 'width_bot_err':0.05599, 'width_top':5.72946, 'width_top_err':0.09234, 'correlation':0.04587, 'correlation_err':0.01277, 'p1_ampl':34551.01031, 'p1_ampl_err':606.70745, 'p2_ampl':0.30646, 'p2_ampl_err':0.00935, 'p3_ampl':0.10211, 'p3_ampl_err':0.00506, 'p4_ampl':0.03000, 'p4_ampl_err':0.00660, 'fd_center_tot':0.00094, 'fd_center_tot_err':3.12658, 'fd_shape_tot':0.53582, 'fd_shape_tot_err':1.53686, 'fd_center_top':5.35158, 'fd_center_top_err':0.43449, 'fd_shape_top':2.05647, 'fd_shape_top_err':0.20959, 'Chi2':995.8, 'NDF':887, 'ln(L)':-233.647}




#nerix_160512_1224
gas_gain_results[16]['gate_pi'][(0.00,0.85)]['nerix_160512_1224'] = {'num_electrons':4, 'fit_successful':True, 'mean_bot':13.01, 'mean_bot_err':0.08, 'mean_top':16.13991, 'mean_top_err':0.10403, 'width_bot':5.00000, 'width_bot_err':0.01475, 'width_top':5.67004, 'width_top_err':0.09659, 'correlation':0.02129, 'correlation_err':0.01587, 'p1_ampl':18680.27996, 'p1_ampl_err':340.21629, 'p2_ampl':0.40679, 'p2_ampl_err':0.01388, 'p3_ampl':0.18647, 'p3_ampl_err':0.01070, 'p4_ampl':0.09450, 'p4_ampl_err':0.01768, 'fd_center_tot':3.88233, 'fd_center_tot_err':2.92074, 'fd_shape_tot':0.50000, 'fd_shape_tot_err':1.76549, 'fd_center_top':4.76701, 'fd_center_top_err':0.41941, 'fd_shape_top':1.41469, 'fd_shape_top_err':0.28470, 'Chi2':918.6, 'NDF':887, 'ln(L)':-199.594}




#nerix_160516_0958
gas_gain_results[16]['gate_pi'][(0.00,0.85)]['nerix_160516_0958'] = {'num_electrons':4, 'fit_successful':True, 'mean_bot':12.98, 'mean_bot_err':0.07, 'mean_top':15.81008, 'mean_top_err':0.12161, 'width_bot':5.21844, 'width_bot_err':0.05545, 'width_top':5.75366, 'width_top_err':0.09149, 'correlation':0.04518, 'correlation_err':0.01302, 'p1_ampl':32492.88239, 'p1_ampl_err':592.40094, 'p2_ampl':0.26023, 'p2_ampl_err':0.00842, 'p3_ampl':0.05741, 'p3_ampl_err':0.00485, 'p4_ampl':0.03044, 'p4_ampl_err':0.02657, 'fd_center_tot':1.57324, 'fd_center_tot_err':4.97258, 'fd_shape_tot':0.50000, 'fd_shape_tot_err':6.79229, 'fd_center_top':5.22471, 'fd_center_top_err':0.44222, 'fd_shape_top':1.95106, 'fd_shape_top_err':0.25434, 'Chi2':790.9, 'NDF':887, 'ln(L)':-202.465}





#nerix_160524_1140
gas_gain_results[16]['gate_pi'][(0.00,0.85)]['nerix_160524_1140'] = {'num_electrons':3, 'fit_successful':True, 'mean_bot':13.68, 'mean_bot_err':0.04, 'mean_top':15.85410, 'mean_top_err':0.12211, 'width_bot':4.97093, 'width_bot_err':0.05173, 'width_top':6.00730, 'width_top_err':0.09499, 'correlation':0.02348, 'correlation_err':0.01303, 'p1_ampl':33288.55741, 'p1_ampl_err':581.39583, 'p2_ampl':0.22208, 'p2_ampl_err':0.00729, 'p3_ampl':0.05132, 'p3_ampl_err':0.00353, 'fd_center_tot':0.00002, 'fd_center_tot_err':12.55871, 'fd_shape_tot':0.50000, 'fd_shape_tot_err':9.45709, 'fd_center_top':6.04413, 'fd_center_top_err':0.34885, 'fd_shape_top':1.84757, 'fd_shape_top_err':0.16879, 'Chi2':800.6, 'NDF':888, 'ln(L)':-232.803}




#nerix_160531_1031
gas_gain_results[16]['gate_pi'][(0.00,0.85)]['nerix_160531_1031'] = {'num_electrons':3, 'fit_successful':True, 'mean_bot':13.92, 'mean_bot_err':0.03, 'mean_top':16.30209, 'mean_top_err':0.10106, 'width_bot':4.96058, 'width_bot_err':0.04633, 'width_top':5.88941, 'width_top_err':0.08166, 'correlation':0.00323, 'correlation_err':0.01173, 'p1_ampl':35327.30227, 'p1_ampl_err':545.27768, 'p2_ampl':0.22859, 'p2_ampl_err':0.00699, 'p3_ampl':0.05816, 'p3_ampl_err':0.00357, 'fd_center_tot':1.25815, 'fd_center_tot_err':0.18194, 'fd_shape_tot':0.55027, 'fd_shape_tot_err':0.02366, 'fd_center_top':5.99498, 'fd_center_top_err':0.36836, 'fd_shape_top':1.90755, 'fd_shape_top_err':0.17189, 'Chi2':915.8, 'NDF':888, 'ln(L)':-236.492}




#nerix_160606_1046
gas_gain_results[16]['gate_pi'][(0.00,0.85)]['nerix_160606_1046'] = {'num_electrons':3, 'fit_successful':True, 'mean_bot':13.98, 'mean_bot_err':0.04, 'mean_top':16.35758, 'mean_top_err':0.11510, 'width_bot':5.02615, 'width_bot_err':0.05028, 'width_top':6.13236, 'width_top_err':0.09331, 'correlation':0.03327, 'correlation_err':0.01299, 'p1_ampl':32236.63330, 'p1_ampl_err':537.22076, 'p2_ampl':0.23298, 'p2_ampl_err':0.00752, 'p3_ampl':0.06252, 'p3_ampl_err':0.00394, 'fd_center_tot':0.00043, 'fd_center_tot_err':18.91421, 'fd_shape_tot':0.50000, 'fd_shape_tot_err':6.31784, 'fd_center_top':5.83008, 'fd_center_top_err':0.35681, 'fd_shape_top':1.77779, 'fd_shape_top_err':0.18478, 'Chi2':853.6, 'NDF':888, 'ln(L)':-221.901}




#nerix_160614_1054
gas_gain_results[16]['gate_pi'][(0.00,0.85)]['nerix_160614_1054'] = {'num_electrons':3, 'fit_successful':True, 'mean_bot':13.80, 'mean_bot_err':0.04, 'mean_top':16.23766, 'mean_top_err':0.09053, 'width_bot':5.14725, 'width_bot_err':0.04979, 'width_top':5.77858, 'width_top_err':0.08347, 'correlation':0.02745, 'correlation_err':0.01231, 'p1_ampl':33677.28981, 'p1_ampl_err':509.80082, 'p2_ampl':0.32157, 'p2_ampl_err':0.00898, 'p3_ampl':0.11115, 'p3_ampl_err':0.00508, 'fd_center_tot':0.00037, 'fd_center_tot_err':19.50557, 'fd_shape_tot':0.50001, 'fd_shape_tot_err':6.28496, 'fd_center_top':5.77050, 'fd_center_top_err':0.33510, 'fd_shape_top':1.76868, 'fd_shape_top_err':0.17763, 'Chi2':977.8, 'NDF':888, 'ln(L)':-242.242}




#nerix_160620_1048
gas_gain_results[16]['gate_pi'][(0.00,0.85)]['nerix_160620_1048'] = {'num_electrons':4, 'fit_successful':True, 'mean_bot':13.76, 'mean_bot_err':0.04, 'mean_top':16.27686, 'mean_top_err':0.09521, 'width_bot':4.96232, 'width_bot_err':0.05118, 'width_top':5.89570, 'width_top_err':0.08658, 'correlation':0.02669, 'correlation_err':0.01294, 'p1_ampl':29515.83969, 'p1_ampl_err':431.90652, 'p2_ampl':0.31567, 'p2_ampl_err':0.00908, 'p3_ampl':0.08839, 'p3_ampl_err':0.00511, 'p4_ampl':0.03000, 'p4_ampl_err':0.00745, 'fd_center_tot':0.00004, 'fd_center_tot_err':23.68520, 'fd_shape_tot':0.50000, 'fd_shape_tot_err':7.36294, 'fd_center_top':5.24835, 'fd_center_top_err':0.31557, 'fd_shape_top':1.36777, 'fd_shape_top_err':0.18332, 'Chi2':856.3, 'NDF':887, 'ln(L)':-223.020}




#nerix_160627_1046
gas_gain_results[16]['gate_pi'][(0.00,0.85)]['nerix_160627_1046'] = {'num_electrons':4, 'fit_successful':True, 'mean_bot':13.76, 'mean_bot_err':0.04, 'mean_top':16.16060, 'mean_top_err':0.11727, 'width_bot':4.96737, 'width_bot_err':0.05504, 'width_top':5.93849, 'width_top_err':0.09635, 'correlation':0.03563, 'correlation_err':0.01351, 'p1_ampl':29134.12178, 'p1_ampl_err':505.83731, 'p2_ampl':0.27196, 'p2_ampl_err':0.00868, 'p3_ampl':0.07030, 'p3_ampl_err':0.00475, 'p4_ampl':0.03000, 'p4_ampl_err':0.01805, 'fd_center_tot':3.43200, 'fd_center_tot_err':3.80965, 'fd_shape_tot':0.50000, 'fd_shape_tot_err':1.83551, 'fd_center_top':5.91501, 'fd_center_top_err':0.39453, 'fd_shape_top':1.93287, 'fd_shape_top_err':0.18058, 'Chi2':759.4, 'NDF':887, 'ln(L)':-207.662}




#nerix_160705_1049
gas_gain_results[16]['gate_pi'][(0.00,0.85)]['nerix_160705_1049'] = {'num_electrons':4, 'fit_successful':True, 'mean_bot':13.39, 'mean_bot_err':0.04, 'mean_top':15.89985, 'mean_top_err':0.11275, 'width_bot':4.82049, 'width_bot_err':0.05417, 'width_top':5.98067, 'width_top_err':0.09700, 'correlation':0.03995, 'correlation_err':0.01403, 'p1_ampl':27758.35323, 'p1_ampl_err':479.12116, 'p2_ampl':0.29466, 'p2_ampl_err':0.00948, 'p3_ampl':0.09351, 'p3_ampl_err':0.00621, 'p4_ampl':0.03242, 'p4_ampl_err':0.00886, 'fd_center_tot':0.00003, 'fd_center_tot_err':12.87952, 'fd_shape_tot':0.50000, 'fd_shape_tot_err':5.74984, 'fd_center_top':5.66947, 'fd_center_top_err':0.35606, 'fd_shape_top':1.78301, 'fd_shape_top_err':0.19001, 'Chi2':765.5, 'NDF':887, 'ln(L)':-208.892}




#nerix_160711_0911
gas_gain_results[16]['gate_pi'][(0.00,0.85)]['nerix_160711_0911'] = {'num_electrons':4, 'fit_successful':True, 'mean_bot':13.43, 'mean_bot_err':0.04, 'mean_top':16.07971, 'mean_top_err':0.09319, 'width_bot':4.79621, 'width_bot_err':0.05368, 'width_top':5.67415, 'width_top_err':0.08630, 'correlation':0.00477, 'correlation_err':0.01344, 'p1_ampl':25392.58894, 'p1_ampl_err':404.62699, 'p2_ampl':0.36669, 'p2_ampl_err':0.01086, 'p3_ampl':0.15243, 'p3_ampl_err':0.00788, 'p4_ampl':0.08008, 'p4_ampl_err':0.01336, 'fd_center_tot':3.50445, 'fd_center_tot_err':3.79735, 'fd_shape_tot':0.50000, 'fd_shape_tot_err':1.97649, 'fd_center_top':4.67657, 'fd_center_top_err':0.41996, 'fd_shape_top':1.66546, 'fd_shape_top_err':0.23539, 'Chi2':877.9, 'NDF':887, 'ln(L)':-211.912}




#nerix_160718_1028
gas_gain_results[16]['gate_pi'][(0.00,0.85)]['nerix_160718_1028'] = {'num_electrons':4, 'fit_successful':True, 'mean_bot':13.44, 'mean_bot_err':0.04, 'mean_top':16.05850, 'mean_top_err':0.09759, 'width_bot':4.86891, 'width_bot_err':0.05685, 'width_top':5.57320, 'width_top_err':0.08760, 'correlation':0.01068, 'correlation_err':0.01427, 'p1_ampl':22959.12678, 'p1_ampl_err':392.55635, 'p2_ampl':0.38509, 'p2_ampl_err':0.01196, 'p3_ampl':0.17401, 'p3_ampl_err':0.00877, 'p4_ampl':0.08392, 'p4_ampl_err':0.01454, 'fd_center_tot':6.88761, 'fd_center_tot_err':1.33024, 'fd_shape_tot':0.50000, 'fd_shape_tot_err':0.78274, 'fd_center_top':5.24345, 'fd_center_top_err':0.40491, 'fd_shape_top':1.66795, 'fd_shape_top_err':0.24407, 'Chi2':730.2, 'NDF':887, 'ln(L)':-184.060}




#nerix_160725_1039
gas_gain_results[16]['gate_pi'][(0.00,0.85)]['nerix_160725_1039'] = {'num_electrons':4, 'fit_successful':True, 'mean_bot':13.25, 'mean_bot_err':0.04, 'mean_top':16.03802, 'mean_top_err':0.10501, 'width_bot':4.77356, 'width_bot_err':0.06145, 'width_top':5.84865, 'width_top_err':0.09642, 'correlation':0.02948, 'correlation_err':0.01561, 'p1_ampl':20298.92734, 'p1_ampl_err':354.30487, 'p2_ampl':0.41594, 'p2_ampl_err':0.01333, 'p3_ampl':0.18764, 'p3_ampl_err':0.01017, 'p4_ampl':0.11902, 'p4_ampl_err':0.01707, 'fd_center_tot':4.43493, 'fd_center_tot_err':3.02065, 'fd_shape_tot':0.50000, 'fd_shape_tot_err':6.25902, 'fd_center_top':5.53492, 'fd_center_top_err':0.34290, 'fd_shape_top':1.50578, 'fd_shape_top_err':0.19059, 'Chi2':848.3, 'NDF':887, 'ln(L)':-205.799}






