#!/usr/bin/python

import ROOT as root
import MySQLdb
import array, math

import numpy as np
import matplotlib.pyplot as plt
import math

"""
dTimeRanges = {45: (1439911260, 1439911980),
			   50: (1439912220, 1439913300),
			   55: (1439913600, 1439914680),
			   60: (1439915100, 1439915940),
			   65: (1439916180, 1439917020),
			   70: (1439919960, 1439921100),
			   75: (1439921340, 1439922540)
			  }
"""
dTimeRanges = {45: [(1441292280, 1441293540), [36, 39, 50]],
			   50: [(1441293720, 1441295940), [93, 89, 67]],
			   55: [(1441296180, 1441297800), [145, 116, 130]],
			   60: [(1441297920, 1441298760), [150, 133, 150]],
			   65: [(1441298940, 1441299840), [195, 195, 221]],
			   70: [(1441300200, 1441301100), [251, 247, 249]],
			   75: [(1441301160, 1441302960), [351, 366]],
			   80: [(1441303140, 1441304520), [410, 411, 385]],
			   85: [(1441304760, 1441305960), [533, 538, 536]],
			  }



iTime = 0
iRate = 1
iBeam = 5

iMeanRate = 0
iMeanRateErr = 1
iMeanBeam = 2
iMeanBeamErr = 3

efficiencyFromSim = 565. / 3e7
distanceNormalization = 4*math.pi*180**2

bkgRate = 6. # needs to be set after  long time of minitron off

sqlConnect = MySQLdb.connect('astro18.astronevis.columbia.edu', 'smac_reader', 'wq12op09', 'smac')
sqlCursor = sqlConnect.cursor()

dData = {}

for voltage in dTimeRanges:
	sqlCursor.execute('select * from pi0 where pi0_id > ' + str(dTimeRanges[voltage][0][0]) + ' and pi0_id < ' + str(dTimeRanges[voltage][0][1]) + ';')
	voltageData = sqlCursor.fetchall()
	sqlCursor.execute('select * from adc0 where adc0_id > ' + str(dTimeRanges[voltage][0][0]) + ' and adc0_id < ' + str(dTimeRanges[voltage][0][1]) + ';')
	beamData = sqlCursor.fetchall()

	lRate = []
	lBeam = []
	for row in voltageData:
		lRate.append(float(row[iRate]))
	for row in beamData:
		lBeam.append(row[iBeam])


	meanRate = (np.mean(lRate) - bkgRate) / efficiencyFromSim / distanceNormalization
	stdevRate = np.std(lRate) / efficiencyFromSim / distanceNormalization

	meanBeam = np.mean(lBeam)
	stdevBeam = np.std(lBeam)

	dData[voltage] = (meanRate, stdevRate, meanBeam, stdevBeam)

lVoltages = dTimeRanges.keys()
lVoltages.sort()
lMeanRate = []
lMeanRateErr = []

lNP2Rate = []
lNP2RateErr = []

for i in xrange(len(lVoltages)):
	lNP2Measurements = []
	totalTime = 0.
	for np2 in dTimeRanges[lVoltages[i]][1]:
		lNP2Measurements.append(np2)
		totalTime += 300. # 300 s per measurement
	
	
	lNP2Rate.append(sum(lNP2Measurements)/totalTime * 2.9e4 / 6000  / 1.6 / 65) # 6000 counts = 1 mrem, possibly keep efficiency of detector as free parameter and fit (see 3.2 in NP-2 manual)
	lNP2RateErr.append((sum(lNP2Measurements)**0.5 + sum(lNP2Measurements)*0.1)/totalTime * 2.9e4 / 6000 / 65)
	
	lMeanRate.append(dData[lVoltages[i]][iMeanRate] / dData[lVoltages[i]][iMeanBeam])
	lMeanRateErr.append(dData[lVoltages[i]][iMeanRateErr] / dData[lVoltages[i]][iMeanBeam] + (dData[lVoltages[i]][iMeanRate] / dData[lVoltages[i]][iMeanBeam])*0.05)
	lVoltages[i] -= dData[lVoltages[i]][iMeanBeam]*10/1000. # (10 = 10 MOhm resistor * 1 uA)


plt.figure(1)
sp = plt.subplot(111)
plt.errorbar(lVoltages, lNP2Rate, fmt='bo', yerr=lNP2RateErr)
plt.errorbar(lVoltages, lMeanRate, fmt='ro', yerr=lMeanRateErr)
sp.set_yscale('log')

plt.title('Minitron Rate Study')
plt.xlabel('Voltage Setting [kV]')
plt.ylabel('Normalized Rate [n / s / cm^2 / uA]')

plt.show()


