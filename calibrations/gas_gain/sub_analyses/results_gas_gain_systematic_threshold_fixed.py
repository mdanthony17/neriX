gas_gain_results = {}

runs = (10, 11, 15)
methods = ['gate_pi', 'misc_e']
radii_sets = [(0., 18.)]

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



#nerix_141203_1506
gas_gain_results[11]['gate_pi'][(0.0,18.0)]['nerix_141203_1506'] = {'num_electrons':2, 'fit_successful':True, 'mean_bot':15.63, 'mean_bot_err':0.37, 'width_bot':6.03330, 'width_bot_err':0.12840, 'width_top':13.76308, 'width_top_err':0.31517, 'correlation':-0.01600, 'correlation_err':0.03621, 'p1_ampl':49372.96147, 'p1_ampl_err':1320.63549, 'p2_ampl':0.28024, 'p2_ampl_err':0.02146, 'fd_center_tot':12.60346, 'fd_center_tot_err':0.00000, 'fd_shape_tot':2.02206, 'fd_shape_tot_err':0.00000, 'fd_center_top':1.66850, 'fd_center_top_err':0.00000, 'fd_shape_top':0.62680, 'fd_shape_top_err':0.00000, 'Chi2':611.1, 'NDF':218, 'ln(L)':-14999.689}

gas_gain_results[11]['misc_e'][(0.0,18.0)]['nerix_141203_1506'] = {'num_electrons':2, 'fit_successful':True, 'mean_bot':15.88, 'mean_bot_err':0.34, 'width_bot':5.87062, 'width_bot_err':0.04113, 'width_top':12.58207, 'width_top_err':0.10208, 'correlation':-0.10426, 'correlation_err':0.01163, 'p1_ampl':355310.74002, 'p1_ampl_err':3141.79616, 'p2_ampl':0.22089, 'p2_ampl_err':0.00590, 'fd_center_tot':12.60346, 'fd_center_tot_err':0.00000, 'fd_shape_tot':2.02206, 'fd_shape_tot_err':0.00000, 'fd_center_top':1.66850, 'fd_center_top_err':0.00000, 'fd_shape_top':0.62680, 'fd_shape_top_err':0.00000, 'Chi2':1750.6, 'NDF':218, 'ln(L)':-64593.750}




#nerix_141208_1123
gas_gain_results[11]['gate_pi'][(0.0,18.0)]['nerix_141208_1123'] = {'num_electrons':2, 'fit_successful':True, 'mean_bot':16.55, 'mean_bot_err':0.62, 'width_bot':6.36384, 'width_bot_err':0.11166, 'width_top':12.04532, 'width_top_err':0.23530, 'correlation':-0.07577, 'correlation_err':0.02750, 'p1_ampl':66350.58787, 'p1_ampl_err':1429.93724, 'p2_ampl':0.21381, 'p2_ampl_err':0.01502, 'fd_center_tot':12.60346, 'fd_center_tot_err':0.00000, 'fd_shape_tot':2.02206, 'fd_shape_tot_err':0.00000, 'fd_center_top':1.66850, 'fd_center_top_err':0.00000, 'fd_shape_top':0.62680, 'fd_shape_top_err':0.00000, 'Chi2':527.3, 'NDF':218, 'ln(L)':-8457.445}

gas_gain_results[11]['misc_e'][(0.0,18.0)]['nerix_141208_1123'] = {'num_electrons':3, 'fit_successful':True, 'mean_bot':16.52, 'mean_bot_err':0.60, 'width_bot':6.34894, 'width_bot_err':0.04908, 'width_top':11.52927, 'width_top_err':0.11878, 'correlation':-0.13881, 'correlation_err':0.01133, 'p1_ampl':352932.29625, 'p1_ampl_err':3388.27505, 'p2_ampl':0.26368, 'p2_ampl_err':0.00660, 'p3_ampl':0.03000, 'p3_ampl_err':0.02150, 'fd_center_tot':12.60346, 'fd_center_tot_err':0.00000, 'fd_shape_tot':2.02206, 'fd_shape_tot_err':0.00000, 'fd_center_top':1.66850, 'fd_center_top_err':0.00000, 'fd_shape_top':0.62680, 'fd_shape_top_err':0.00000, 'Chi2':942.3, 'NDF':217, 'ln(L)':-70892.710}




#nerix_141210_1028
gas_gain_results[11]['gate_pi'][(0.0,18.0)]['nerix_141210_1028'] = {'num_electrons':2, 'fit_successful':True, 'mean_bot':16.58, 'mean_bot_err':0.48, 'width_bot':6.26714, 'width_bot_err':0.11031, 'width_top':11.64763, 'width_top_err':0.22636, 'correlation':-0.14674, 'correlation_err':0.02640, 'p1_ampl':60917.73738, 'p1_ampl_err':1338.96800, 'p2_ampl':0.19062, 'p2_ampl_err':0.01325, 'fd_center_tot':12.60346, 'fd_center_tot_err':0.00000, 'fd_shape_tot':2.02206, 'fd_shape_tot_err':0.00000, 'fd_center_top':1.66850, 'fd_center_top_err':0.00000, 'fd_shape_top':0.62680, 'fd_shape_top_err':0.00000, 'Chi2':396.0, 'NDF':218, 'ln(L)':-8231.102}

gas_gain_results[11]['misc_e'][(0.0,18.0)]['nerix_141210_1028'] = {'num_electrons':3, 'fit_successful':True, 'mean_bot':16.42, 'mean_bot_err':0.46, 'width_bot':6.30595, 'width_bot_err':0.05169, 'width_top':11.08125, 'width_top_err':0.12348, 'correlation':-0.17967, 'correlation_err':0.01182, 'p1_ampl':298153.44137, 'p1_ampl_err':3065.88462, 'p2_ampl':0.24847, 'p2_ampl_err':0.00647, 'p3_ampl':0.03000, 'p3_ampl_err':0.00518, 'fd_center_tot':12.60346, 'fd_center_tot_err':0.00000, 'fd_shape_tot':2.02206, 'fd_shape_tot_err':0.00000, 'fd_center_top':1.66850, 'fd_center_top_err':0.00000, 'fd_shape_top':0.62680, 'fd_shape_top_err':0.00000, 'Chi2':710.6, 'NDF':217, 'ln(L)':-55905.846}




#nerix_141217_1041
gas_gain_results[11]['gate_pi'][(0.0,18.0)]['nerix_141217_1041'] = {'num_electrons':2, 'fit_successful':True, 'mean_bot':15.51, 'mean_bot_err':0.48, 'width_bot':6.02838, 'width_bot_err':0.09172, 'width_top':10.24583, 'width_top_err':0.19894, 'correlation':-0.16831, 'correlation_err':0.02338, 'p1_ampl':70111.30779, 'p1_ampl_err':1369.66593, 'p2_ampl':0.27592, 'p2_ampl_err':0.01361, 'fd_center_tot':12.60346, 'fd_center_tot_err':0.00000, 'fd_shape_tot':2.02206, 'fd_shape_tot_err':0.00000, 'fd_center_top':1.66850, 'fd_center_top_err':0.00000, 'fd_shape_top':0.62680, 'fd_shape_top_err':0.00000, 'Chi2':385.0, 'NDF':218, 'ln(L)':-11420.531}

gas_gain_results[11]['misc_e'][(0.0,18.0)]['nerix_141217_1041'] = {'num_electrons':3, 'fit_successful':False, 'mean_bot':15.42, 'mean_bot_err':0.47, 'width_bot':6.05902, 'width_bot_err':0.04573, 'width_top':10.29398, 'width_top_err':0.09865, 'correlation':-0.22082, 'correlation_err':0.01022, 'p1_ampl':362695.69075, 'p1_ampl_err':3238.50392, 'p2_ampl':0.31099, 'p2_ampl_err':0.00663, 'p3_ampl':0.04474, 'p3_ampl_err':0.00744, 'fd_center_tot':12.60346, 'fd_center_tot_err':0.00000, 'fd_shape_tot':2.02206, 'fd_shape_tot_err':0.00000, 'fd_center_top':1.66850, 'fd_center_top_err':0.00000, 'fd_shape_top':0.62680, 'fd_shape_top_err':0.00000, 'Chi2':1236.5, 'NDF':217, 'ln(L)':-86945.597}




#nerix_150106_1100
gas_gain_results[11]['gate_pi'][(0.0,18.0)]['nerix_150106_1100'] = {'num_electrons':3, 'fit_successful':True, 'mean_bot':17.16, 'mean_bot_err':0.61, 'width_bot':5.77922, 'width_bot_err':0.09367, 'width_top':11.07205, 'width_top_err':0.25909, 'correlation':0.06648, 'correlation_err':0.02875, 'p1_ampl':89719.29868, 'p1_ampl_err':2185.99828, 'p2_ampl':0.30880, 'p2_ampl_err':0.01394, 'p3_ampl':0.03303, 'p3_ampl_err':0.00959, 'fd_center_tot':12.60346, 'fd_center_tot_err':0.00000, 'fd_shape_tot':2.02206, 'fd_shape_tot_err':0.00000, 'fd_center_top':1.66850, 'fd_center_top_err':0.00000, 'fd_shape_top':0.62680, 'fd_shape_top_err':0.00000, 'Chi2':368.6, 'NDF':217, 'ln(L)':-20653.173}

gas_gain_results[11]['misc_e'][(0.0,18.0)]['nerix_150106_1100'] = {'num_electrons':4, 'fit_successful':True, 'mean_bot':16.40, 'mean_bot_err':0.57, 'width_bot':5.99979, 'width_bot_err':0.03431, 'width_top':11.80581, 'width_top_err':0.06429, 'correlation':-0.06284, 'correlation_err':0.00947, 'p1_ampl':804515.96539, 'p1_ampl_err':5412.83035, 'p2_ampl':0.24484, 'p2_ampl_err':0.00451, 'p3_ampl':0.03000, 'p3_ampl_err':0.00126, 'p4_ampl':0.03000, 'p4_ampl_err':0.01220, 'fd_center_tot':12.60346, 'fd_center_tot_err':0.00000, 'fd_shape_tot':2.02206, 'fd_shape_tot_err':0.00000, 'fd_center_top':1.66850, 'fd_center_top_err':0.00000, 'fd_shape_top':0.62680, 'fd_shape_top_err':0.00000, 'Chi2':2428.2, 'NDF':216, 'ln(L)':-115023.681}




#nerix_150106_1137
gas_gain_results[11]['gate_pi'][(0.0,18.0)]['nerix_150106_1137'] = {'num_electrons':3, 'fit_successful':True, 'mean_bot':17.08, 'mean_bot_err':0.59, 'width_bot':5.82488, 'width_bot_err':0.06752, 'width_top':10.46273, 'width_top_err':0.16875, 'correlation':0.05174, 'correlation_err':0.01813, 'p1_ampl':174803.81689, 'p1_ampl_err':2750.80071, 'p2_ampl':0.25351, 'p2_ampl_err':0.00850, 'p3_ampl':0.03008, 'p3_ampl_err':0.00126, 'fd_center_tot':12.60346, 'fd_center_tot_err':0.00000, 'fd_shape_tot':2.02206, 'fd_shape_tot_err':0.00000, 'fd_center_top':1.66850, 'fd_center_top_err':0.00000, 'fd_shape_top':0.62680, 'fd_shape_top_err':0.00000, 'Chi2':604.9, 'NDF':217, 'ln(L)':-28199.050}

gas_gain_results[11]['misc_e'][(0.0,18.0)]['nerix_150106_1137'] = {'num_electrons':3, 'fit_successful':True, 'mean_bot':15.43, 'mean_bot_err':0.56, 'width_bot':5.91170, 'width_bot_err':0.15131, 'width_top':9.82758, 'width_top_err':0.21661, 'correlation':0.03646, 'correlation_err':0.03547, 'p1_ampl':54926.79579, 'p1_ampl_err':1526.76794, 'p2_ampl':0.36613, 'p2_ampl_err':0.02240, 'p3_ampl':0.12102, 'p3_ampl_err':0.01692, 'fd_center_tot':12.60346, 'fd_center_tot_err':0.00000, 'fd_shape_tot':2.02206, 'fd_shape_tot_err':0.00000, 'fd_center_top':1.66850, 'fd_center_top_err':0.00000, 'fd_shape_top':0.62680, 'fd_shape_top_err':0.00000, 'Chi2':309.5, 'NDF':217, 'ln(L)':-6088.763}




#nerix_150106_1234
gas_gain_results[11]['gate_pi'][(0.0,18.0)]['nerix_150106_1234'] = {'num_electrons':2, 'fit_successful':True, 'mean_bot':17.40, 'mean_bot_err':0.60, 'width_bot':5.95554, 'width_bot_err':0.06701, 'width_top':10.88011, 'width_top_err':0.17857, 'correlation':0.06198, 'correlation_err':0.01927, 'p1_ampl':172556.60709, 'p1_ampl_err':2973.10799, 'p2_ampl':0.22076, 'p2_ampl_err':0.00809, 'fd_center_tot':12.60346, 'fd_center_tot_err':0.00000, 'fd_shape_tot':2.02206, 'fd_shape_tot_err':0.00000, 'fd_center_top':1.66850, 'fd_center_top_err':0.00000, 'fd_shape_top':0.62680, 'fd_shape_top_err':0.00000, 'Chi2':402.6, 'NDF':218, 'ln(L)':-19953.679}

gas_gain_results[11]['misc_e'][(0.0,18.0)]['nerix_150106_1234'] = {'num_electrons':3, 'fit_successful':True, 'mean_bot':15.44, 'mean_bot_err':0.55, 'width_bot':5.78767, 'width_bot_err':0.12417, 'width_top':10.24771, 'width_top_err':0.19222, 'correlation':0.01738, 'correlation_err':0.02992, 'p1_ampl':72381.89375, 'p1_ampl_err':1711.38934, 'p2_ampl':0.34908, 'p2_ampl_err':0.01860, 'p3_ampl':0.11786, 'p3_ampl_err':0.01394, 'fd_center_tot':12.60346, 'fd_center_tot_err':0.00000, 'fd_shape_tot':2.02206, 'fd_shape_tot_err':0.00000, 'fd_center_top':1.66850, 'fd_center_top_err':0.00000, 'fd_shape_top':0.62680, 'fd_shape_top_err':0.00000, 'Chi2':475.1, 'NDF':217, 'ln(L)':-10016.192}




#nerix_150106_1404
gas_gain_results[11]['gate_pi'][(0.0,18.0)]['nerix_150106_1404'] = {'num_electrons':2, 'fit_successful':True, 'mean_bot':17.05, 'mean_bot_err':0.59, 'width_bot':5.96133, 'width_bot_err':0.07411, 'width_top':11.04101, 'width_top_err':0.21388, 'correlation':0.08548, 'correlation_err':0.02202, 'p1_ampl':146497.96471, 'p1_ampl_err':3006.30072, 'p2_ampl':0.18945, 'p2_ampl_err':0.00832, 'fd_center_tot':12.60346, 'fd_center_tot_err':0.00000, 'fd_shape_tot':2.02206, 'fd_shape_tot_err':0.00000, 'fd_center_top':1.66850, 'fd_center_top_err':0.00000, 'fd_shape_top':0.62680, 'fd_shape_top_err':0.00000, 'Chi2':475.2, 'NDF':218, 'ln(L)':-14993.149}

gas_gain_results[11]['misc_e'][(0.0,18.0)]['nerix_150106_1404'] = {'num_electrons':3, 'fit_successful':True, 'mean_bot':14.96, 'mean_bot_err':0.53, 'width_bot':5.86726, 'width_bot_err':0.13434, 'width_top':9.46565, 'width_top_err':0.19740, 'correlation':-0.01120, 'correlation_err':0.03160, 'p1_ampl':68611.82853, 'p1_ampl_err':1764.26441, 'p2_ampl':0.35108, 'p2_ampl_err':0.02003, 'p3_ampl':0.09169, 'p3_ampl_err':0.01226, 'fd_center_tot':12.60346, 'fd_center_tot_err':0.00000, 'fd_shape_tot':2.02206, 'fd_shape_tot_err':0.00000, 'fd_center_top':1.66850, 'fd_center_top_err':0.00000, 'fd_shape_top':0.62680, 'fd_shape_top_err':0.00000, 'Chi2':395.6, 'NDF':217, 'ln(L)':-15691.094}




#nerix_150114_1407
gas_gain_results[11]['gate_pi'][(0.0,18.0)]['nerix_150114_1407'] = {'num_electrons':3, 'fit_successful':False, 'mean_bot':16.94, 'mean_bot_err':2.13, 'width_bot':5.94830, 'width_bot_err':2.37445, 'width_top':10.94321, 'width_top_err':3.67763, 'correlation':0.03100, 'correlation_err':0.61042, 'p1_ampl':98236.05614, 'p1_ampl_err':171825.42396, 'p2_ampl':0.23969, 'p2_ampl_err':0.26052, 'p3_ampl':0.03000, 'p3_ampl_err':0.18104, 'fd_center_tot':12.60346, 'fd_center_tot_err':0.00000, 'fd_shape_tot':2.02206, 'fd_shape_tot_err':0.00000, 'fd_center_top':1.66850, 'fd_center_top_err':0.00000, 'fd_shape_top':0.62680, 'fd_shape_top_err':0.00000, 'Chi2':402.5, 'NDF':217, 'ln(L)':-13190.344}

gas_gain_results[11]['misc_e'][(0.0,18.0)]['nerix_150114_1407'] = {'num_electrons':3, 'fit_successful':True, 'mean_bot':16.28, 'mean_bot_err':0.55, 'width_bot':5.94550, 'width_bot_err':0.03820, 'width_top':10.78585, 'width_top_err':0.06692, 'correlation':-0.07450, 'correlation_err':0.01008, 'p1_ampl':616996.67896, 'p1_ampl_err':4570.31227, 'p2_ampl':0.22978, 'p2_ampl_err':0.00486, 'p3_ampl':0.05935, 'p3_ampl_err':0.00361, 'fd_center_tot':12.60346, 'fd_center_tot_err':0.00000, 'fd_shape_tot':2.02206, 'fd_shape_tot_err':0.00000, 'fd_center_top':1.66850, 'fd_center_top_err':0.00000, 'fd_shape_top':0.62680, 'fd_shape_top_err':0.00000, 'Chi2':2238.8, 'NDF':217, 'ln(L)':-96540.846}




#nerix_150128_1158
gas_gain_results[11]['gate_pi'][(0.0,18.0)]['nerix_150128_1158'] = {'num_electrons':3, 'fit_successful':True, 'mean_bot':17.14, 'mean_bot_err':0.43, 'width_bot':5.80138, 'width_bot_err':0.07901, 'width_top':10.42359, 'width_top_err':0.20567, 'correlation':-0.01061, 'correlation_err':0.02357, 'p1_ampl':109886.09206, 'p1_ampl_err':2171.95802, 'p2_ampl':0.27380, 'p2_ampl_err':0.01099, 'p3_ampl':0.03000, 'p3_ampl_err':0.00639, 'fd_center_tot':12.60346, 'fd_center_tot_err':0.00000, 'fd_shape_tot':2.02206, 'fd_shape_tot_err':0.00000, 'fd_center_top':1.66850, 'fd_center_top_err':0.00000, 'fd_shape_top':0.62680, 'fd_shape_top_err':0.00000, 'Chi2':379.0, 'NDF':217, 'ln(L)':-19231.165}

gas_gain_results[11]['misc_e'][(0.0,18.0)]['nerix_150128_1158'] = {'num_electrons':3, 'fit_successful':True, 'mean_bot':16.73, 'mean_bot_err':0.41, 'width_bot':6.04638, 'width_bot_err':0.02914, 'width_top':11.57491, 'width_top_err':0.05453, 'correlation':-0.08817, 'correlation_err':0.00812, 'p1_ampl':1072602.67604, 'p1_ampl_err':6118.57990, 'p2_ampl':0.23752, 'p2_ampl_err':0.00380, 'p3_ampl':0.04076, 'p3_ampl_err':0.00249, 'fd_center_tot':12.60346, 'fd_center_tot_err':0.00000, 'fd_shape_tot':2.02206, 'fd_shape_tot_err':0.00000, 'fd_center_top':1.66850, 'fd_center_top_err':0.00000, 'fd_shape_top':0.62680, 'fd_shape_top_err':0.00000, 'Chi2':3195.1, 'NDF':217, 'ln(L)':-175940.362}




#nerix_150128_1327
gas_gain_results[11]['gate_pi'][(0.0,18.0)]['nerix_150128_1327'] = {'num_electrons':3, 'fit_successful':True, 'mean_bot':17.03, 'mean_bot_err':0.44, 'width_bot':5.88503, 'width_bot_err':0.09979, 'width_top':10.63560, 'width_top_err':0.24717, 'correlation':0.07982, 'correlation_err':0.02816, 'p1_ampl':94782.88524, 'p1_ampl_err':2232.80385, 'p2_ampl':0.24750, 'p2_ampl_err':0.01254, 'p3_ampl':0.03000, 'p3_ampl_err':0.01955, 'fd_center_tot':12.60346, 'fd_center_tot_err':0.00000, 'fd_shape_tot':2.02206, 'fd_shape_tot_err':0.00000, 'fd_center_top':1.66850, 'fd_center_top_err':0.00000, 'fd_shape_top':0.62680, 'fd_shape_top_err':0.00000, 'Chi2':551.4, 'NDF':217, 'ln(L)':-10364.194}

gas_gain_results[11]['misc_e'][(0.0,18.0)]['nerix_150128_1327'] = {'num_electrons':3, 'fit_successful':True, 'mean_bot':16.60, 'mean_bot_err':0.41, 'width_bot':6.05395, 'width_bot_err':0.04099, 'width_top':11.13341, 'width_top_err':0.07324, 'correlation':-0.08027, 'correlation_err':0.01107, 'p1_ampl':545994.43382, 'p1_ampl_err':4327.29851, 'p2_ampl':0.23650, 'p2_ampl_err':0.00530, 'p3_ampl':0.04928, 'p3_ampl_err':0.00389, 'fd_center_tot':12.60346, 'fd_center_tot_err':0.00000, 'fd_shape_tot':2.02206, 'fd_shape_tot_err':0.00000, 'fd_center_top':1.66850, 'fd_center_top_err':0.00000, 'fd_shape_top':0.62680, 'fd_shape_top_err':0.00000, 'Chi2':1753.6, 'NDF':217, 'ln(L)':-76456.126}




#nerix_150128_1454
gas_gain_results[11]['gate_pi'][(0.0,18.0)]['nerix_150128_1454'] = {'num_electrons':2, 'fit_successful':True, 'mean_bot':17.36, 'mean_bot_err':0.44, 'width_bot':6.02192, 'width_bot_err':0.09317, 'width_top':10.74563, 'width_top_err':0.24637, 'correlation':0.05885, 'correlation_err':0.02643, 'p1_ampl':92148.26003, 'p1_ampl_err':2224.06885, 'p2_ampl':0.19722, 'p2_ampl_err':0.01050, 'fd_center_tot':12.60346, 'fd_center_tot_err':0.00000, 'fd_shape_tot':2.02206, 'fd_shape_tot_err':0.00000, 'fd_center_top':1.66850, 'fd_center_top_err':0.00000, 'fd_shape_top':0.62680, 'fd_shape_top_err':0.00000, 'Chi2':363.0, 'NDF':218, 'ln(L)':-10703.046}

gas_gain_results[11]['misc_e'][(0.0,18.0)]['nerix_150128_1454'] = {'num_electrons':3, 'fit_successful':True, 'mean_bot':16.29, 'mean_bot_err':0.40, 'width_bot':5.86508, 'width_bot_err':0.04386, 'width_top':10.20954, 'width_top_err':0.07266, 'correlation':-0.04054, 'correlation_err':0.01140, 'p1_ampl':463888.87873, 'p1_ampl_err':3934.71745, 'p2_ampl':0.24190, 'p2_ampl_err':0.00565, 'p3_ampl':0.06954, 'p3_ampl_err':0.00432, 'fd_center_tot':12.60346, 'fd_center_tot_err':0.00000, 'fd_shape_tot':2.02206, 'fd_shape_tot_err':0.00000, 'fd_center_top':1.66850, 'fd_center_top_err':0.00000, 'fd_shape_top':0.62680, 'fd_shape_top_err':0.00000, 'Chi2':1617.4, 'NDF':217, 'ln(L)':-61853.080}




#nerix_150128_1622
gas_gain_results[11]['gate_pi'][(0.0,18.0)]['nerix_150128_1622'] = {'num_electrons':2, 'fit_successful':True, 'mean_bot':17.19, 'mean_bot_err':0.44, 'width_bot':5.92161, 'width_bot_err':0.10062, 'width_top':10.48153, 'width_top_err':0.26332, 'correlation':0.00386, 'correlation_err':0.02908, 'p1_ampl':74442.09790, 'p1_ampl_err':1969.31838, 'p2_ampl':0.18450, 'p2_ampl_err':0.01115, 'fd_center_tot':12.60346, 'fd_center_tot_err':0.00000, 'fd_shape_tot':2.02206, 'fd_shape_tot_err':0.00000, 'fd_center_top':1.66850, 'fd_center_top_err':0.00000, 'fd_shape_top':0.62680, 'fd_shape_top_err':0.00000, 'Chi2':272.6, 'NDF':218, 'ln(L)':-6088.196}

gas_gain_results[11]['misc_e'][(0.0,18.0)]['nerix_150128_1622'] = {'num_electrons':3, 'fit_successful':True, 'mean_bot':16.25, 'mean_bot_err':0.40, 'width_bot':5.97167, 'width_bot_err':0.05024, 'width_top':9.66498, 'width_top_err':0.07480, 'correlation':-0.02624, 'correlation_err':0.01213, 'p1_ampl':376137.77861, 'p1_ampl_err':3467.62003, 'p2_ampl':0.19315, 'p2_ampl_err':0.00568, 'p3_ampl':0.05041, 'p3_ampl_err':0.00407, 'fd_center_tot':12.60346, 'fd_center_tot_err':0.00000, 'fd_shape_tot':2.02206, 'fd_shape_tot_err':0.00000, 'fd_center_top':1.66850, 'fd_center_top_err':0.00000, 'fd_shape_top':0.62680, 'fd_shape_top_err':0.00000, 'Chi2':1113.9, 'NDF':217, 'ln(L)':-42918.423}




