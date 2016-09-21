gas_gain_results = {}

runs = (10, 11, 15)
methods = ['gate_pi']
radii_sets = [(0., 18.)]

for run in runs:
	gas_gain_results[run] = {}
	for method in methods:
		gas_gain_results[run][method] = {}
		for radii_set in radii_sets:
			gas_gain_results[run][method][radii_set] = {}



# ---------------------------------------
# ---------------------------------------
#  RUN 10
# ---------------------------------------
# ---------------------------------------


#nerix_140915_1158
gas_gain_results[10]['gate_pi'][(0.0,18.0)]['nerix_140915_1158'] = {'num_electrons':3, 'fit_successful':True, 'mean_bot':17.94, 'mean_bot_err':0.42, 'mean_top':9.54658, 'mean_top_err':0.35412, 'width_bot':5.63760, 'width_bot_err':0.11769, 'width_top':7.32533, 'width_top_err':0.23211, 'correlation':-0.03505, 'correlation_err':0.03837, 'p1_ampl':25530.60213, 'p1_ampl_err':1111.67696, 'p2_ampl':0.18510, 'p2_ampl_err':0.01248, 'p3_ampl':0.03059, 'p3_ampl_err':0.05020, 'fd_center_tot':11.00067, 'fd_center_tot_err':1.49309, 'fd_shape_tot':2.33075, 'fd_shape_tot_err':0.99400, 'fd_center_top':1.40132, 'fd_center_top_err':0.44019, 'fd_shape_top':1.53540, 'fd_shape_top_err':0.39786, 'Chi2':209.6, 'NDF':388, 'ln(L)':-1119.980}




#nerix_140917_1010
gas_gain_results[10]['gate_pi'][(0.0,18.0)]['nerix_140917_1010'] = {'num_electrons':2, 'fit_successful':True, 'mean_bot':17.13, 'mean_bot_err':0.42, 'mean_top':9.53944, 'mean_top_err':0.35654, 'width_bot':5.89405, 'width_bot_err':0.17105, 'width_top':6.89935, 'width_top_err':0.28654, 'correlation':0.06699, 'correlation_err':0.05175, 'p1_ampl':15187.92344, 'p1_ampl_err':670.83074, 'p2_ampl':0.22373, 'p2_ampl_err':0.01805, 'fd_center_tot':12.97742, 'fd_center_tot_err':1.09044, 'fd_shape_tot':2.13698, 'fd_shape_tot_err':0.51317, 'fd_center_top':1.18213, 'fd_center_top_err':2.64050, 'fd_shape_top':0.26477, 'fd_shape_top_err':6.32962, 'Chi2':239.2, 'NDF':389, 'ln(L)':-775.428}




#nerix_140930_0925
gas_gain_results[10]['gate_pi'][(0.0,18.0)]['nerix_140930_0925'] = {'num_electrons':3, 'fit_successful':True, 'mean_bot':16.58, 'mean_bot_err':0.45, 'mean_top':8.63731, 'mean_top_err':0.39043, 'width_bot':5.82432, 'width_bot_err':0.19544, 'width_top':6.51335, 'width_top_err':0.27316, 'correlation':0.05062, 'correlation_err':0.06151, 'p1_ampl':14124.69203, 'p1_ampl_err':722.21740, 'p2_ampl':0.26830, 'p2_ampl_err':0.02250, 'p3_ampl':0.04827, 'p3_ampl_err':0.01578, 'fd_center_tot':13.08546, 'fd_center_tot_err':1.26859, 'fd_shape_tot':2.21342, 'fd_shape_tot_err':0.55701, 'fd_center_top':1.20372, 'fd_center_top_err':2.62538, 'fd_shape_top':0.24721, 'fd_shape_top_err':6.57292, 'Chi2':244.5, 'NDF':388, 'ln(L)':-419.598}




#nerix_140930_1029
gas_gain_results[10]['gate_pi'][(0.0,18.0)]['nerix_140930_1029'] = {'num_electrons':2, 'fit_successful':True, 'mean_bot':17.82, 'mean_bot_err':0.35, 'mean_top':7.94243, 'mean_top_err':0.62490, 'width_bot':5.95182, 'width_bot_err':0.17951, 'width_top':7.23139, 'width_top_err':0.35442, 'correlation':0.05310, 'correlation_err':0.05153, 'p1_ampl':17801.98207, 'p1_ampl_err':1396.23569, 'p2_ampl':0.13367, 'p2_ampl_err':0.01322, 'fd_center_tot':11.39628, 'fd_center_tot_err':1.33898, 'fd_shape_tot':1.96892, 'fd_shape_tot_err':0.91381, 'fd_center_top':0.82463, 'fd_center_top_err':0.74865, 'fd_shape_top':1.78606, 'fd_shape_top_err':0.62731, 'Chi2':173.6, 'NDF':389, 'ln(L)':-406.762}




#nerix_140930_1127
gas_gain_results[10]['gate_pi'][(0.0,18.0)]['nerix_140930_1127'] = {'num_electrons':2, 'fit_successful':True, 'mean_bot':17.95, 'mean_bot_err':0.24, 'mean_top':8.48250, 'mean_top_err':0.26359, 'width_bot':5.69812, 'width_bot_err':0.11586, 'width_top':6.87241, 'width_top_err':0.21182, 'correlation':-0.04675, 'correlation_err':0.03442, 'p1_ampl':21444.10572, 'p1_ampl_err':707.13300, 'p2_ampl':0.21700, 'p2_ampl_err':0.01397, 'fd_center_tot':10.50923, 'fd_center_tot_err':0.88530, 'fd_shape_tot':1.26246, 'fd_shape_tot_err':0.51517, 'fd_center_top':1.33867, 'fd_center_top_err':0.42540, 'fd_shape_top':0.24606, 'fd_shape_top_err':0.68165, 'Chi2':300.9, 'NDF':389, 'ln(L)':-1500.058}




#nerix_140930_1626
gas_gain_results[10]['gate_pi'][(0.0,18.0)]['nerix_140930_1626'] = {'num_electrons':2, 'fit_successful':True, 'mean_bot':17.34, 'mean_bot_err':0.30, 'mean_top':8.53511, 'mean_top_err':0.34493, 'width_bot':5.99156, 'width_bot_err':0.17070, 'width_top':6.67361, 'width_top_err':0.28044, 'correlation':0.03911, 'correlation_err':0.04631, 'p1_ampl':14048.12133, 'p1_ampl_err':607.92336, 'p2_ampl':0.18721, 'p2_ampl_err':0.01711, 'fd_center_tot':11.50442, 'fd_center_tot_err':0.73715, 'fd_shape_tot':1.09491, 'fd_shape_tot_err':0.52453, 'fd_center_top':1.36795, 'fd_center_top_err':9.46091, 'fd_shape_top':0.21269, 'fd_shape_top_err':6.06341, 'Chi2':203.4, 'NDF':389, 'ln(L)':-62.351}




#nerix_141003_1232
gas_gain_results[10]['gate_pi'][(0.0,18.0)]['nerix_141003_1232'] = {'num_electrons':3, 'fit_successful':True, 'mean_bot':17.22, 'mean_bot_err':0.37, 'mean_top':8.57831, 'mean_top_err':0.29619, 'width_bot':5.78674, 'width_bot_err':0.14865, 'width_top':6.32486, 'width_top_err':0.20872, 'correlation':0.00535, 'correlation_err':0.04555, 'p1_ampl':20469.22424, 'p1_ampl_err':900.37299, 'p2_ampl':0.20210, 'p2_ampl_err':0.01472, 'p3_ampl':0.03000, 'p3_ampl_err':0.02469, 'fd_center_tot':12.55838, 'fd_center_tot_err':1.21953, 'fd_shape_tot':2.44065, 'fd_shape_tot_err':0.65054, 'fd_center_top':0.02580, 'fd_center_top_err':12.90364, 'fd_shape_top':0.59687, 'fd_shape_top_err':0.54960, 'Chi2':217.4, 'NDF':388, 'ln(L)':-416.663}




#nerix_141006_0951
gas_gain_results[10]['gate_pi'][(0.0,18.0)]['nerix_141006_0951'] = {'num_electrons':3, 'fit_successful':True, 'mean_bot':17.13, 'mean_bot_err':0.39, 'mean_top':7.43164, 'mean_top_err':0.41856, 'width_bot':5.53655, 'width_bot_err':0.15038, 'width_top':6.92655, 'width_top_err':0.26053, 'correlation':-0.00369, 'correlation_err':0.04789, 'p1_ampl':19273.46991, 'p1_ampl_err':1069.09846, 'p2_ampl':0.19942, 'p2_ampl_err':0.01563, 'p3_ampl':0.03170, 'p3_ampl_err':0.01505, 'fd_center_tot':12.49740, 'fd_center_tot_err':0.97288, 'fd_shape_tot':1.72152, 'fd_shape_tot_err':0.55116, 'fd_center_top':0.65598, 'fd_center_top_err':0.57308, 'fd_shape_top':1.13601, 'fd_shape_top_err':0.78087, 'Chi2':162.1, 'NDF':388, 'ln(L)':-53.186}




#nerix_141014_1024
gas_gain_results[10]['gate_pi'][(0.0,18.0)]['nerix_141014_1024'] = {'num_electrons':3, 'fit_successful':True, 'mean_bot':16.74, 'mean_bot_err':0.27, 'mean_top':7.00000, 'mean_top_err':0.32353, 'width_bot':5.35388, 'width_bot_err':0.14026, 'width_top':6.92381, 'width_top_err':0.15633, 'correlation':0.03617, 'correlation_err':0.04082, 'p1_ampl':19591.44840, 'p1_ampl_err':565.39785, 'p2_ampl':0.18506, 'p2_ampl_err':0.01488, 'p3_ampl':0.03165, 'p3_ampl_err':0.00982, 'fd_center_tot':10.39958, 'fd_center_tot_err':0.81668, 'fd_shape_tot':1.18675, 'fd_shape_tot_err':0.54146, 'fd_center_top':1.38867, 'fd_center_top_err':0.26282, 'fd_shape_top':0.21498, 'fd_shape_top_err':0.53744, 'Chi2':248.5, 'NDF':388, 'ln(L)':-424.539}




#nerix_141014_1414
gas_gain_results[10]['gate_pi'][(0.0,18.0)]['nerix_141014_1414'] = {'num_electrons':3, 'fit_successful':True, 'mean_bot':16.43, 'mean_bot_err':0.27, 'mean_top':7.00000, 'mean_top_err':0.50287, 'width_bot':5.50722, 'width_bot_err':0.13927, 'width_top':7.06743, 'width_top_err':0.14738, 'correlation':0.03992, 'correlation_err':0.03812, 'p1_ampl':25306.74290, 'p1_ampl_err':724.24964, 'p2_ampl':0.24297, 'p2_ampl_err':0.01623, 'p3_ampl':0.03332, 'p3_ampl_err':0.00815, 'fd_center_tot':11.24239, 'fd_center_tot_err':0.71096, 'fd_shape_tot':1.56303, 'fd_shape_tot_err':0.39429, 'fd_center_top':0.68314, 'fd_center_top_err':0.40342, 'fd_shape_top':1.29828, 'fd_shape_top_err':0.45814, 'Chi2':317.2, 'NDF':388, 'ln(L)':-1135.867}




#nerix_141015_1155
gas_gain_results[10]['gate_pi'][(0.0,18.0)]['nerix_141015_1155'] = {'num_electrons':2, 'fit_successful':True, 'mean_bot':16.99, 'mean_bot_err':0.29, 'mean_top':7.31860, 'mean_top_err':0.33175, 'width_bot':5.71386, 'width_bot_err':0.13181, 'width_top':6.70345, 'width_top_err':0.22732, 'correlation':-0.01497, 'correlation_err':0.04232, 'p1_ampl':23489.17855, 'p1_ampl_err':1010.60376, 'p2_ampl':0.20037, 'p2_ampl_err':0.01348, 'fd_center_tot':13.15094, 'fd_center_tot_err':0.71829, 'fd_shape_tot':1.83150, 'fd_shape_tot_err':0.34042, 'fd_center_top':1.26397, 'fd_center_top_err':0.63517, 'fd_shape_top':0.24126, 'fd_shape_top_err':0.71372, 'Chi2':231.8, 'NDF':389, 'ln(L)':-775.624}




#nerix_141022_1127
gas_gain_results[10]['gate_pi'][(0.0,18.0)]['nerix_141022_1127'] = {'num_electrons':3, 'fit_successful':True, 'mean_bot':16.02, 'mean_bot_err':0.26, 'mean_top':8.30941, 'mean_top_err':0.26318, 'width_bot':5.50692, 'width_bot_err':0.14852, 'width_top':6.50563, 'width_top_err':0.22452, 'correlation':0.00419, 'correlation_err':0.04646, 'p1_ampl':18931.26020, 'p1_ampl_err':685.14801, 'p2_ampl':0.26966, 'p2_ampl_err':0.01919, 'p3_ampl':0.07420, 'p3_ampl_err':0.01216, 'fd_center_tot':10.72854, 'fd_center_tot_err':0.76926, 'fd_shape_tot':1.17068, 'fd_shape_tot_err':0.50662, 'fd_center_top':1.20839, 'fd_center_top_err':0.61248, 'fd_shape_top':0.26786, 'fd_shape_top_err':0.54141, 'Chi2':246.5, 'NDF':388, 'ln(L)':-2193.378}




#nerix_141029_1110
gas_gain_results[10]['gate_pi'][(0.0,18.0)]['nerix_141029_1110'] = {'num_electrons':2, 'fit_successful':True, 'mean_bot':16.45, 'mean_bot_err':0.34, 'mean_top':7.00000, 'mean_top_err':0.32519, 'width_bot':5.64372, 'width_bot_err':0.18943, 'width_top':7.31307, 'width_top_err':0.22127, 'correlation':-0.06339, 'correlation_err':0.05518, 'p1_ampl':11349.98212, 'p1_ampl_err':441.07442, 'p2_ampl':0.15082, 'p2_ampl_err':0.01819, 'fd_center_tot':11.23446, 'fd_center_tot_err':0.83824, 'fd_shape_tot':1.03172, 'fd_shape_tot_err':0.66285, 'fd_center_top':1.26144, 'fd_center_top_err':0.89736, 'fd_shape_top':0.23772, 'fd_shape_top_err':1.35366, 'Chi2':150.4, 'NDF':389, 'ln(L)':-410.929}




#nerix_141103_1101
gas_gain_results[10]['gate_pi'][(0.0,18.0)]['nerix_141103_1101'] = {'num_electrons':3, 'fit_successful':True, 'mean_bot':15.71, 'mean_bot_err':0.32, 'mean_top':7.00000, 'mean_top_err':0.16211, 'width_bot':5.02161, 'width_bot_err':0.30961, 'width_top':7.36056, 'width_top_err':0.20446, 'correlation':-0.05421, 'correlation_err':0.05021, 'p1_ampl':11584.95143, 'p1_ampl_err':421.97290, 'p2_ampl':0.15917, 'p2_ampl_err':0.01741, 'p3_ampl':0.03000, 'p3_ampl_err':0.01198, 'fd_center_tot':11.10598, 'fd_center_tot_err':0.70985, 'fd_shape_tot':0.92428, 'fd_shape_tot_err':0.37821, 'fd_center_top':1.38372, 'fd_center_top_err':0.50616, 'fd_shape_top':0.20881, 'fd_shape_top_err':1.73065, 'Chi2':165.8, 'NDF':388, 'ln(L)':-59.614}




#nerix_141103_1553
gas_gain_results[10]['gate_pi'][(0.0,18.0)]['nerix_141103_1553'] = {'num_electrons':3, 'fit_successful':True, 'mean_bot':15.45, 'mean_bot_err':0.29, 'mean_top':7.00000, 'mean_top_err':0.35750, 'width_bot':5.00000, 'width_bot_err':0.14209, 'width_top':6.82867, 'width_top_err':0.14382, 'correlation':0.01398, 'correlation_err':0.03832, 'p1_ampl':21611.13686, 'p1_ampl_err':592.04405, 'p2_ampl':0.22164, 'p2_ampl_err':0.01501, 'p3_ampl':0.03000, 'p3_ampl_err':0.00818, 'fd_center_tot':11.36090, 'fd_center_tot_err':0.69823, 'fd_shape_tot':1.68493, 'fd_shape_tot_err':0.46027, 'fd_center_top':1.29378, 'fd_center_top_err':0.46225, 'fd_shape_top':0.25216, 'fd_shape_top_err':0.54005, 'Chi2':246.4, 'NDF':388, 'ln(L)':-1840.743}




#nerix_141103_1627
gas_gain_results[10]['gate_pi'][(0.0,18.0)]['nerix_141103_1627'] = {'num_electrons':2, 'fit_successful':True, 'mean_bot':16.03, 'mean_bot_err':0.26, 'mean_top':7.31627, 'mean_top_err':0.24032, 'width_bot':5.50150, 'width_bot_err':0.09084, 'width_top':6.99048, 'width_top_err':0.17396, 'correlation':0.00341, 'correlation_err':0.02657, 'p1_ampl':40311.23223, 'p1_ampl_err':1142.88425, 'p2_ampl':0.22477, 'p2_ampl_err':0.01053, 'fd_center_tot':11.05081, 'fd_center_tot_err':0.41207, 'fd_shape_tot':1.15454, 'fd_shape_tot_err':0.23379, 'fd_center_top':1.34603, 'fd_center_top_err':0.03395, 'fd_shape_top':0.19084, 'fd_shape_top_err':0.04431, 'Chi2':334.0, 'NDF':389, 'ln(L)':-2563.289}




#nerix_141103_1655
gas_gain_results[10]['gate_pi'][(0.0,18.0)]['nerix_141103_1655'] = {'num_electrons':3, 'fit_successful':True, 'mean_bot':15.93, 'mean_bot_err':0.28, 'mean_top':7.84532, 'mean_top_err':0.22905, 'width_bot':5.51477, 'width_bot_err':0.09719, 'width_top':7.11514, 'width_top_err':0.18212, 'correlation':-0.02994, 'correlation_err':0.03080, 'p1_ampl':46116.10118, 'p1_ampl_err':1274.40355, 'p2_ampl':0.25460, 'p2_ampl_err':0.01210, 'p3_ampl':0.03547, 'p3_ampl_err':0.00616, 'fd_center_tot':11.42438, 'fd_center_tot_err':0.46305, 'fd_shape_tot':1.33832, 'fd_shape_tot_err':0.30250, 'fd_center_top':1.32774, 'fd_center_top_err':0.36726, 'fd_shape_top':0.25246, 'fd_shape_top_err':0.51581, 'Chi2':472.3, 'NDF':388, 'ln(L)':-1522.254}



# ---------------------------------------
# ---------------------------------------
#  RUN 11
# ---------------------------------------
# ---------------------------------------


#nerix_141203_1506
gas_gain_results[11]['gate_pi'][(0.0,18.0)]['nerix_141203_1506'] = {'num_electrons':2, 'fit_successful':True, 'mean_bot':16.35, 'mean_bot_err':0.37, 'mean_top':18.78062, 'mean_top_err':0.51646, 'width_bot':6.04415, 'width_bot_err':0.11469, 'width_top':11.28568, 'width_top_err':0.38127, 'correlation':-0.12943, 'correlation_err':0.02849, 'p1_ampl':6540.89381, 'p1_ampl_err':264.41890, 'p2_ampl':0.27373, 'p2_ampl_err':0.01916, 'fd_center_tot':12.90132, 'fd_center_tot_err':0.89495, 'fd_shape_tot':1.31778, 'fd_shape_tot_err':0.64845, 'fd_center_top':2.50686, 'fd_center_top_err':0.96596, 'fd_shape_top':2.66720, 'fd_shape_top_err':0.89751, 'Chi2':922.0, 'NDF':1589, 'ln(L)':-999.672}




#nerix_141208_1123
gas_gain_results[11]['gate_pi'][(0.0,18.0)]['nerix_141208_1123'] = {'num_electrons':2, 'fit_successful':True, 'mean_bot':15.91, 'mean_bot_err':0.64, 'mean_top':15.30420, 'mean_top_err':0.54871, 'width_bot':6.56976, 'width_bot_err':0.13790, 'width_top':11.56187, 'width_top_err':0.33517, 'correlation':0.02677, 'correlation_err':0.03818, 'p1_ampl':10074.01190, 'p1_ampl_err':408.84105, 'p2_ampl':0.19846, 'p2_ampl_err':0.01393, 'fd_center_tot':18.83385, 'fd_center_tot_err':0.72552, 'fd_shape_tot':2.79496, 'fd_shape_tot_err':0.32446, 'fd_center_top':1.58140, 'fd_center_top_err':0.26799, 'fd_shape_top':0.49948, 'fd_shape_top_err':0.18555, 'Chi2':957.8, 'NDF':1589, 'ln(L)':-1353.858}




#nerix_141210_1028
gas_gain_results[11]['gate_pi'][(0.0,18.0)]['nerix_141210_1028'] = {'num_electrons':2, 'fit_successful':False, 'mean_bot':15.89, 'mean_bot_err':0.50, 'mean_top':15.08167, 'mean_top_err':0.53637, 'width_bot':6.30882, 'width_bot_err':0.12234, 'width_top':11.25317, 'width_top_err':0.31729, 'correlation':-0.05745, 'correlation_err':0.03597, 'p1_ampl':9207.67848, 'p1_ampl_err':375.36884, 'p2_ampl':0.18470, 'p2_ampl_err':0.01216, 'fd_center_tot':18.77982, 'fd_center_tot_err':0.78211, 'fd_shape_tot':2.97607, 'fd_shape_tot_err':0.36957, 'fd_center_top':0.90478, 'fd_center_top_err':0.66478, 'fd_shape_top':0.10073, 'fd_shape_top_err':4.24867, 'Chi2':1045.2, 'NDF':1589, 'ln(L)':-658.980}




#nerix_141217_1041
gas_gain_results[11]['gate_pi'][(0.0,18.0)]['nerix_141217_1041'] = {'num_electrons':2, 'fit_successful':True, 'mean_bot':15.18, 'mean_bot_err':0.50, 'mean_top':15.24581, 'mean_top_err':0.35014, 'width_bot':6.14998, 'width_bot_err':0.10569, 'width_top':10.11023, 'width_top_err':0.25709, 'correlation':-0.09161, 'correlation_err':0.03193, 'p1_ampl':10362.82645, 'p1_ampl_err':299.79638, 'p2_ampl':0.26245, 'p2_ampl_err':0.01334, 'fd_center_tot':16.84193, 'fd_center_tot_err':0.63724, 'fd_shape_tot':2.32441, 'fd_shape_tot_err':0.33088, 'fd_center_top':1.58563, 'fd_center_top_err':0.23449, 'fd_shape_top':0.53088, 'fd_shape_top_err':0.15680, 'Chi2':1166.9, 'NDF':1589, 'ln(L)':-1028.752}




#nerix_150106_1100
gas_gain_results[11]['gate_pi'][(0.0,18.0)]['nerix_150106_1100'] = {'num_electrons':3, 'fit_successful':True, 'mean_bot':17.35, 'mean_bot_err':0.61, 'mean_top':10.38268, 'mean_top_err':0.31299, 'width_bot':5.64313, 'width_bot_err':0.08576, 'width_top':10.44933, 'width_top_err':0.24276, 'correlation':0.01297, 'correlation_err':0.02950, 'p1_ampl':11780.21815, 'p1_ampl_err':311.65854, 'p2_ampl':0.31567, 'p2_ampl_err':0.01380, 'p3_ampl':0.04398, 'p3_ampl_err':0.00984, 'fd_center_tot':11.77182, 'fd_center_tot_err':0.68129, 'fd_shape_tot':1.73460, 'fd_shape_tot_err':0.41634, 'fd_center_top':1.23336, 'fd_center_top_err':0.13971, 'fd_shape_top':0.41851, 'fd_shape_top_err':0.09584, 'Chi2':1138.0, 'NDF':1588, 'ln(L)':-1011.802}




#nerix_150114_1407
gas_gain_results[11]['gate_pi'][(0.0,18.0)]['nerix_150114_1407'] = {'num_electrons':3, 'fit_successful':True, 'mean_bot':17.08, 'mean_bot_err':0.60, 'mean_top':9.94491, 'mean_top_err':0.35773, 'width_bot':5.84737, 'width_bot_err':0.09233, 'width_top':10.32248, 'width_top_err':0.26110, 'correlation':-0.00567, 'correlation_err':0.03185, 'p1_ampl':12853.52270, 'p1_ampl_err':376.96438, 'p2_ampl':0.24503, 'p2_ampl_err':0.01199, 'p3_ampl':0.03496, 'p3_ampl_err':0.00763, 'fd_center_tot':12.22920, 'fd_center_tot_err':0.55459, 'fd_shape_tot':1.70056, 'fd_shape_tot_err':0.33090, 'fd_center_top':1.12798, 'fd_center_top_err':0.13614, 'fd_shape_top':0.59141, 'fd_shape_top_err':0.12660, 'Chi2':1203.1, 'NDF':1588, 'ln(L)':-828.106}




#nerix_150128_1158
gas_gain_results[11]['gate_pi'][(0.0,18.0)]['nerix_150128_1158'] = {'num_electrons':3, 'fit_successful':True, 'mean_bot':17.30, 'mean_bot_err':0.44, 'mean_top':10.87579, 'mean_top_err':0.24322, 'width_bot':5.70408, 'width_bot_err':0.07797, 'width_top':9.98879, 'width_top_err':0.20801, 'correlation':-0.04607, 'correlation_err':0.02476, 'p1_ampl':14720.11689, 'p1_ampl_err':316.51018, 'p2_ampl':0.27636, 'p2_ampl_err':0.01088, 'p3_ampl':0.03161, 'p3_ampl_err':0.01257, 'fd_center_tot':12.32976, 'fd_center_tot_err':0.47620, 'fd_shape_tot':1.41277, 'fd_shape_tot_err':0.26846, 'fd_center_top':1.20699, 'fd_center_top_err':0.12675, 'fd_shape_top':0.49321, 'fd_shape_top_err':0.10280, 'Chi2':1179.7, 'NDF':1588, 'ln(L)':-1026.808}




#nerix_150128_1327
gas_gain_results[11]['gate_pi'][(0.0,18.0)]['nerix_150128_1327'] = {'num_electrons':3, 'fit_successful':False, 'mean_bot':17.03, 'mean_bot_err':0.46, 'mean_top':10.25460, 'mean_top_err':0.33760, 'width_bot':5.81427, 'width_bot_err':0.10237, 'width_top':10.26032, 'width_top_err':0.25281, 'correlation':0.07100, 'correlation_err':0.03348, 'p1_ampl':12871.76936, 'p1_ampl_err':356.44499, 'p2_ampl':0.24642, 'p2_ampl_err':0.01217, 'p3_ampl':0.03292, 'p3_ampl_err':0.00844, 'fd_center_tot':13.40297, 'fd_center_tot_err':0.58836, 'fd_shape_tot':2.02642, 'fd_shape_tot_err':0.32398, 'fd_center_top':0.84988, 'fd_center_top_err':0.28891, 'fd_shape_top':0.10097, 'fd_shape_top_err':7.86530, 'Chi2':1087.3, 'NDF':1588, 'ln(L)':-1366.741}




#nerix_150128_1454
gas_gain_results[11]['gate_pi'][(0.0,18.0)]['nerix_150128_1454'] = {'num_electrons':2, 'fit_successful':True, 'mean_bot':17.55, 'mean_bot_err':0.45, 'mean_top':10.23027, 'mean_top_err':0.36796, 'width_bot':5.92573, 'width_bot_err':0.09345, 'width_top':10.40893, 'width_top_err':0.26681, 'correlation':0.02596, 'correlation_err':0.02913, 'p1_ampl':12454.71151, 'p1_ampl_err':365.41286, 'p2_ampl':0.19878, 'p2_ampl_err':0.01034, 'fd_center_tot':12.19242, 'fd_center_tot_err':0.55257, 'fd_shape_tot':1.50276, 'fd_shape_tot_err':0.35068, 'fd_center_top':1.40100, 'fd_center_top_err':0.15102, 'fd_shape_top':0.54607, 'fd_shape_top_err':0.10901, 'Chi2':1019.4, 'NDF':1589, 'ln(L)':-999.456}




#nerix_150128_1622
gas_gain_results[11]['gate_pi'][(0.0,18.0)]['nerix_150128_1622'] = {'num_electrons':2, 'fit_successful':True, 'mean_bot':17.20, 'mean_bot_err':0.47, 'mean_top':9.33349, 'mean_top_err':0.50078, 'width_bot':5.88800, 'width_bot_err':0.10762, 'width_top':10.62609, 'width_top_err':0.32424, 'correlation':0.01453, 'correlation_err':0.03719, 'p1_ampl':10692.91858, 'p1_ampl_err':416.72015, 'p2_ampl':0.18051, 'p2_ampl_err':0.01090, 'fd_center_tot':12.91477, 'fd_center_tot_err':0.75949, 'fd_shape_tot':2.11750, 'fd_shape_tot_err':0.41048, 'fd_center_top':1.63020, 'fd_center_top_err':0.14632, 'fd_shape_top':0.55231, 'fd_shape_top_err':0.08351, 'Chi2':938.0, 'NDF':1589, 'ln(L)':-291.509}


