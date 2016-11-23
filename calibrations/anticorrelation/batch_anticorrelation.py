#!/usr/bin/python
import pickle
#print pickle.Pickler.dispatch
import dill
#print pickle.Pickler.dispatch
import ROOT as root

from fit_anticorrelation import fit_anticorrelation

num_walkers = 64
num_steps = 5000
num_steps_to_include = 3000


l_files = []

# done
l_files.append({'cs137':[['nerix_160404_1204', 'nerix_160404_1232', 'nerix_160404_1259', 'nerix_160404_1325', 'nerix_160404_1350']],
                'na22':[['nerix_160404_1421', 'nerix_160404_1447', 'nerix_160404_1530', 'nerix_160404_1555', 'nerix_160404_1621']]})


# done
l_files.append({'cs137':[['nerix_160411_0612', 'nerix_160411_0644', 'nerix_160411_0712', 'nerix_160411_0739', 'nerix_160411_0925']],
                'na22':[['nerix_160411_1245', 'nerix_160411_1313', 'nerix_160411_1341', 'nerix_160411_1419', 'nerix_160411_1447']]})


# done
l_files.append({'cs137':[['nerix_160418_1026', 'nerix_160418_1052', 'nerix_160418_1120', 'nerix_160418_1145', 'nerix_160418_1210']],
                'na22':[['nerix_160418_1239', 'nerix_160418_1307', 'nerix_160418_1337', 'nerix_160418_1405', 'nerix_160418_1434']]})


# done
l_files.append({'cs137':[['nerix_160502_1059', 'nerix_160502_1134', 'nerix_160502_1208', 'nerix_160502_1234', 'nerix_160502_1302']],
                'na22':[['nerix_160502_1334', 'nerix_160502_1404', 'nerix_160502_1433', 'nerix_160502_1537', 'nerix_160502_1605']]})


# done
l_files.append({'cs137':[['nerix_160509_1157', 'nerix_160509_1226', 'nerix_160509_1252', 'nerix_160509_1320', 'nerix_160509_1350']],
                'na22':[['nerix_160509_1419', 'nerix_160509_1448', 'nerix_160509_1515', 'nerix_160509_1544', 'nerix_160509_1619']]})


# done
l_files.append({'cs137':[['nerix_160516_1226', 'nerix_160516_1255', 'nerix_160516_1322', 'nerix_160516_1351', 'nerix_160516_1418']],
                'na22':[['nerix_160516_1447', 'nerix_160516_1516', 'nerix_160516_1545', 'nerix_160516_1613', 'nerix_160516_1641']]})

# done
l_files.append({'cs137':[['nerix_160523_1215', 'nerix_160523_1242', 'nerix_160523_1308', 'nerix_160523_1337', 'nerix_160523_1405']],
                'na22':[['nerix_160523_1436', 'nerix_160523_1506', 'nerix_160523_1534', 'nerix_160523_1600', 'nerix_160523_1628']]})


# done
l_files.append({'cs137':[['nerix_160531_1144', 'nerix_160531_1234', 'nerix_160531_1303', 'nerix_160531_1331', 'nerix_160531_1357']],
                'na22':[['nerix_160531_1428', 'nerix_160531_1457', 'nerix_160531_1525', 'nerix_160531_1553', 'nerix_160531_1621']]})

# done
l_files.append({'cs137':[['nerix_160606_1155', 'nerix_160606_1225', 'nerix_160606_1254', 'nerix_160606_1347', 'nerix_160606_1421']],
                'na22':[['nerix_160606_1451', 'nerix_160606_1521', 'nerix_160606_1549', 'nerix_160606_1617', 'nerix_160606_1646']]})

# done
l_files.append({'cs137':[['nerix_160614_1203', 'nerix_160614_1231', 'nerix_160614_1258', 'nerix_160614_1328', 'nerix_160614_1356']],
                'na22':[['nerix_160614_1426', 'nerix_160614_1455', 'nerix_160614_1523', 'nerix_160614_1555', 'nerix_160614_1623']]})

# done
l_files.append({'cs137':[['nerix_160620_1158', 'nerix_160620_1226', 'nerix_160620_1253', 'nerix_160620_1322', 'nerix_160620_1349']],
                'na22':[['nerix_160620_1419', 'nerix_160620_1452', 'nerix_160620_1522', 'nerix_160620_1551', 'nerix_160620_1621']]})

# done
l_files.append({'cs137':[['nerix_160627_1156', 'nerix_160627_1224', 'nerix_160627_1253', 'nerix_160627_1321', 'nerix_160627_1349']],
                'na22':[['nerix_160627_1417', 'nerix_160627_1448', 'nerix_160627_1531', 'nerix_160627_1601', 'nerix_160627_1630']]})

# done
l_files.append({'cs137':[['nerix_160705_1205', 'nerix_160705_1232', 'nerix_160705_1300', 'nerix_160705_1325', 'nerix_160705_1354']],
                'na22':[['nerix_160705_1423', 'nerix_160705_1454', 'nerix_160705_1523', 'nerix_160705_1553', 'nerix_160705_1623']]})

# done
l_files.append({'cs137':[['nerix_160711_1021', 'nerix_160711_1050', 'nerix_160711_1120', 'nerix_160711_1150', 'nerix_160711_1219']],
                'na22':[['nerix_160711_1253', 'nerix_160711_1333', 'nerix_160711_1402', 'nerix_160711_1448', 'nerix_160711_1519']]})


# below are outside coincidence time period
"""
# done
l_files.append({'cs137':[['nerix_160718_1138', 'nerix_160718_1207', 'nerix_160718_1249', 'nerix_160718_1318', 'nerix_160718_1350']],
                'na22':[['nerix_160718_1421', 'nerix_160718_1452', 'nerix_160718_1521', 'nerix_160718_1600', 'nerix_160718_1651']]})

# done
l_files.append({'cs137':[['nerix_160725_1149', 'nerix_160725_1218', 'nerix_160725_1245', 'nerix_160725_1313', 'nerix_160725_1341']],
                'na22':[['nerix_160725_1411', 'nerix_160725_1442', 'nerix_160725_1510', 'nerix_160725_1540', 'nerix_160725_1610']]})

# done
l_files.append({'cs137':[['nerix_160801_1145', 'nerix_160801_1300', 'nerix_160801_1328', 'nerix_160801_1356', 'nerix_160801_1424']],
                'na22':[['nerix_160801_1453', 'nerix_160801_1530', 'nerix_160801_1601', 'nerix_160801_1631', 'nerix_160801_1702']]})

"""


for d_files in l_files:
    root.gROOT.SetBatch(1)

    try:
        anticorrelation_fitter = fit_anticorrelation(d_files)
        anticorrelation_fitter.run_mcmc(num_walkers, num_steps)
        anticorrelation_fitter.draw_anticorrelation_plots(num_walkers, num_steps_to_include)
    except:
        print '\n\n\n\nERROR:\n\n\n\n\n'
        print d_files
        print '\n\n\n\nPlease check the above files\n\n\n\n'



