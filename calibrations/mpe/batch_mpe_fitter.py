#!/usr/bin/python
import pickle
#print pickle.Pickler.dispatch
import dill
#print pickle.Pickler.dispatch

import fit_mpe

num_walkers = 16
num_steps = 5000
num_steps_to_include = 1000


l_datasets = []


l_datasets.append({800:'nerix_160404_1014',
                   750:'nerix_160404_1015',
                   700:'nerix_160404_1016',
                   650:'nerix_160404_1017',
                   600:'nerix_160404_1018',
                   550:'nerix_160404_1019',
                   500:'nerix_160404_1020'})

l_datasets.append({800:'nerix_160411_1205',
                   750:'nerix_160411_1206',
                   700:'nerix_160411_1207',
                   650:'nerix_160411_1208',
                   600:'nerix_160411_1209',
                   550:'nerix_160411_1210',
                   500:'nerix_160411_1211'})


l_datasets.append({800:'nerix_160425_1553',
                   750:'nerix_160425_1554',
                   700:'nerix_160425_1555',
                   650:'nerix_160425_1556',
                   600:'nerix_160425_1557',
                   550:'nerix_160425_1558',
                   500:'nerix_160425_1559'})

l_datasets.append({800:'nerix_160502_1714',
                   750:'nerix_160502_1715',
                   700:'nerix_160502_1716',
                   650:'nerix_160502_1717',
                   600:'nerix_160502_1718',
                   550:'nerix_160502_1719',
                   500:'nerix_160502_1720'})


l_datasets.append({800:'nerix_160509_1011',
                   750:'nerix_160509_1012',
                   700:'nerix_160509_1013',
                   650:'nerix_160509_1014',
                   600:'nerix_160509_1015',
                   550:'nerix_160509_1016',
                   500:'nerix_160509_1017'})

l_datasets.append({800:'nerix_160516_1154',
                   750:'nerix_160516_1156',
                   700:'nerix_160516_1157',
                   650:'nerix_160516_1158',
                   600:'nerix_160516_1159',
                   550:'nerix_160516_1200',
                   500:'nerix_160516_1201'})

l_datasets.append({800:'nerix_160523_1031',
                   750:'nerix_160523_1032',
                   700:'nerix_160523_1033',
                   650:'nerix_160523_1034',
                   600:'nerix_160523_1035',
                   550:'nerix_160523_1036',
                   500:'nerix_160523_1037'})

l_datasets.append({800:'nerix_160531_1000',
                   750:'nerix_160531_1001',
                   700:'nerix_160531_1002',
                   650:'nerix_160531_1003',
                   600:'nerix_160531_1004',
                   550:'nerix_160531_1005',
                   500:'nerix_160531_1006'})

l_datasets.append({800:'nerix_160606_1010',
                   750:'nerix_160606_1012',
                   700:'nerix_160606_1013',
                   650:'nerix_160606_1014',
                   600:'nerix_160606_1015',
                   550:'nerix_160606_1016',
                   500:'nerix_160606_1017'})

l_datasets.append({800:'nerix_160614_1023',
                   750:'nerix_160614_1024',
                   700:'nerix_160614_1025',
                   650:'nerix_160614_1026',
                   600:'nerix_160614_1027',
                   550:'nerix_160614_1028',
                   500:'nerix_160614_1029'})

l_datasets.append({800:'nerix_160620_1011',
                   750:'nerix_160620_1012',
                   700:'nerix_160620_1014',
                   650:'nerix_160620_1015',
                   600:'nerix_160620_1017',
                   550:'nerix_160620_1018',
                   500:'nerix_160620_1019'})

l_datasets.append({800:'nerix_160627_1011',
                   750:'nerix_160627_1012',
                   700:'nerix_160627_1014',
                   650:'nerix_160627_1015',
                   600:'nerix_160627_1016',
                   550:'nerix_160627_1018',
                   500:'nerix_160627_1019'})

l_datasets.append({800:'nerix_160705_1013',
                   750:'nerix_160705_1015',
                   700:'nerix_160705_1016',
                   650:'nerix_160705_1017',
                   600:'nerix_160705_1019',
                   550:'nerix_160705_1020',
                   500:'nerix_160705_1022'})

l_datasets.append({800:'nerix_160711_0834',
                   750:'nerix_160711_0835',
                   700:'nerix_160711_0838',
                   650:'nerix_160711_0840',
                   600:'nerix_160711_0841',
                   550:'nerix_160711_0842',
                   500:'nerix_160711_0843'})

# after coincidence data
"""
l_datasets.append({800:'nerix_160718_0954',
                   750:'nerix_160718_0955',
                   700:'nerix_160718_0957',
                   650:'nerix_160718_0958',
                   600:'nerix_160718_0959',
                   550:'nerix_160718_1000',
                   500:'nerix_160718_1001'})

l_datasets.append({800:'nerix_160725_1003',
                   750:'nerix_160725_1005',
                   700:'nerix_160725_1007',
                   650:'nerix_160725_1008',
                   600:'nerix_160725_1009',
                   550:'nerix_160725_1011',
                   500:'nerix_160725_1012'})

l_datasets.append({800:'nerix_160801_0959',
                   750:'nerix_160801_1000',
                   700:'nerix_160801_1001',
                   650:'nerix_160801_1002',
                   600:'nerix_160801_1004',
                   550:'nerix_160801_1005',
                   500:'nerix_160801_1006'})

l_datasets.append({800:'nerix_160808_1103',
                   750:'nerix_160808_1105',
                   700:'nerix_160808_1107',
                   650:'nerix_160808_1109',
                   600:'nerix_160808_1111',
                   550:'nerix_160808_1112',
                   500:'nerix_160808_1114'})
"""




for d_files in l_datasets:
    mpe_fitter = fit_mpe.fit_mpe(d_files)
    
    mpe_fitter.run_mpe_mcmc(num_walkers=num_walkers, num_steps=num_steps, threads=1)
    mpe_fitter.draw_mpe_corner_plot(num_walkers=num_walkers, num_steps_to_include=num_steps_to_include)
    mpe_fitter.draw_best_fit_on_hist(num_walkers=num_walkers, num_steps_to_include=num_steps_to_include)


