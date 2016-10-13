#!/usr/bin/python
import pickle
#print pickle.Pickler.dispatch
import dill
#print pickle.Pickler.dispatch

from fit_position_correction import fit_position_correction

num_walkers = 32
num_steps = 5000
num_steps_to_include = 3000


l_files = []

"""
# done
l_files.append({0.345:'nerix_160404_1204', 0.7:'nerix_160404_1232', 1.054:'nerix_160404_1259', 1.500:'nerix_160404_1325', 2.356:'nerix_160404_1350'})

# done
l_files.append({0.345:'nerix_160411_0612', 0.7:'nerix_160411_0644', 1.054:'nerix_160411_0712', 1.500:'nerix_160411_0739', 2.356:'nerix_160411_0925'})

# done
l_files.append({0.345:'nerix_160418_1026', 0.7:'nerix_160418_1052', 1.054:'nerix_160418_1120', 1.500:'nerix_160418_1145', 2.356:'nerix_160418_1210'})

# done
l_files.append({0.345:'nerix_160425_1206', 0.7:'nerix_160425_1234', 1.054:'nerix_160425_1327', 1.500:'nerix_160425_1355', 2.356:'nerix_160425_1442'})

# done
l_files.append({0.345:'nerix_160502_1059', 0.7:'nerix_160502_1134', 1.054:'nerix_160502_1208', 1.500:'nerix_160502_1234', 2.356:'nerix_160502_1302'})

# done
l_files.append({0.345:'nerix_160509_1157', 0.7:'nerix_160509_1226', 1.054:'nerix_160509_1252', 1.500:'nerix_160509_1320', 2.356:'nerix_160509_1350'})

# done
l_files.append({0.345:'nerix_160516_1226', 0.7:'nerix_160516_1255', 1.054:'nerix_160516_1322', 1.500:'nerix_160516_1351', 2.356:'nerix_160516_1418'})

# done
l_files.append({0.345:'nerix_160523_1215', 0.7:'nerix_160523_1242', 1.054:'nerix_160523_1308', 1.500:'nerix_160523_1337', 2.356:'nerix_160523_1405'})

# done
l_files.append({0.345:'nerix_160531_1144', 0.7:'nerix_160531_1234', 1.054:'nerix_160531_1303', 1.500:'nerix_160531_1331', 2.356:'nerix_160531_1357'})

"""
# done
l_files.append({0.345:'nerix_160606_1155', 0.7:'nerix_160606_1225', 1.054:'nerix_160606_1254', 1.500:'nerix_160606_1347', 2.356:'nerix_160606_1421'})

# done
l_files.append({0.345:'nerix_160609_1106', 0.7:'nerix_160609_1133', 1.054:'nerix_160609_1200', 1.500:'nerix_160609_1227', 2.356:'nerix_160609_1254'})
"""
# done
l_files.append({0.345:'nerix_160614_1203', 0.7:'nerix_160614_1231', 1.054:'nerix_160614_1258', 1.500:'nerix_160614_1328', 2.356:'nerix_160614_1356'})

# done
l_files.append({0.345:'nerix_160620_1158', 0.7:'nerix_160620_1226', 1.054:'nerix_160620_1253', 1.500:'nerix_160620_1322', 2.356:'nerix_160620_1349'})

# done
l_files.append({0.345:'nerix_160627_1156', 0.7:'nerix_160627_1224', 1.054:'nerix_160627_1253', 1.500:'nerix_160627_1321', 2.356:'nerix_160627_1349'})

# done
l_files.append({0.345:'nerix_160705_1205', 0.7:'nerix_160705_1232', 1.054:'nerix_160705_1300', 1.500:'nerix_160705_1325', 2.356:'nerix_160705_1354'})

# done
l_files.append({0.345:'nerix_160711_1021', 0.7:'nerix_160711_1050', 1.054:'nerix_160711_1120', 1.500:'nerix_160711_1150', 2.356:'nerix_160711_1219'})

# done
l_files.append({0.345:'nerix_160718_1138', 0.7:'nerix_160718_1207', 1.054:'nerix_160718_1249', 1.500:'nerix_160718_1318', 2.356:'nerix_160718_1350'})

# done
l_files.append({0.345:'nerix_160725_1149', 0.7:'nerix_160725_1218', 1.054:'nerix_160725_1245', 1.500:'nerix_160725_1313', 2.356:'nerix_160725_1341'})

# done
l_files.append({0.345:'nerix_160801_1145', 0.7:'nerix_160801_1300', 1.054:'nerix_160801_1328', 1.500:'nerix_160801_1356', 2.356:'nerix_160801_1424'})

"""


for d_files in l_files:
    pos_correction_fitter = fit_position_correction(d_files)
    
    for cathode_setting in d_files:
        pos_correction_fitter.run_mcmc('s1_z', cathode_setting, num_walkers, num_steps, threads=1)
        pos_correction_fitter.draw_pos_correction_plots('s1_z', cathode_setting, num_walkers, num_steps_to_include)
        pos_correction_fitter.run_mcmc('s2_dt', cathode_setting, num_walkers, num_steps, threads=1)
        pos_correction_fitter.draw_pos_correction_plots('s2_dt', cathode_setting, num_walkers, num_steps_to_include)


