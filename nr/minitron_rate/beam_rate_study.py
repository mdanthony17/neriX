#!/usr/bin/python

import ROOT as root
import MySQLdb
import array, math

import numpy as np
import matplotlib.pyplot as plt
import math

"""
dTimeRanges = {45: (1439927452, 1439928113),
			   55: (1439926852, 1439927273),
			   65: (1439925770, 1439926312),
			   75: (1439924209, 1439925712),
			   85: (1439923008, 1439924091),
			  }
"""

lTimeRanges = [[(1441306380, 1441308000), [334, 358, 339]],
			   [(1441308240, 1441309500), [299, 291, 280]],
			   [(1441309740, 1441310580), [731./3., 731./3., 731./3.]],
			   [(1441310760, 1441311720), [638./3., 638./3., 638./3.]],
			   [(1441311840, 1441313460), [526./3., 526./3., 526./3.]],
			   [(1441313700, 1441314600), [463./3., 463./3., 463./3.]],
			  ]

highVoltage = 70. #kV

iTime = 0
iRate = 1
iBeam = 5

iMeanRate = 0
iMeanRateErr = 1
iMeanBeam = 2
iMeanBeamErr = 3

efficiencyFromSim = 565. / 3e7
distanceNormalization = 4*math.pi*180**2
sizeOfActiveAreaNP2 = math.pi*(6.35/2)**2

bkgRate = 6. # needs to be set after  long time of minitron off


sqlConnect = MySQLdb.connect('astro18.astronevis.columbia.edu', 'smac_reader', 'wq12op09', 'smac')
sqlCursor = sqlConnect.cursor()

lData = []

for current in lTimeRanges:
	sqlCursor.execute('select * from pi0 where pi0_id > ' + str(current[0][0]) + ' and pi0_id < ' + str(current[0][1]) + ';')
	currentData = sqlCursor.fetchall()
	sqlCursor.execute('select * from adc0 where adc0_id > ' + str(current[0][0]) + ' and adc0_id < ' + str(current[0][1]) + ';')
	beamData = sqlCursor.fetchall()

	lRate = []
	lBeam = []
	for row in currentData:
		lRate.append(float(row[iRate]))
	for row in beamData:
		lBeam.append(row[iBeam])


	meanRate = (np.mean(lRate) - bkgRate) / efficiencyFromSim / distanceNormalization
	stdevRate = np.std(lRate) / efficiencyFromSim / distanceNormalization

	meanBeam = np.mean(lBeam)
	stdevBeam = np.std(lBeam)

	lData.append((meanRate, stdevRate, meanBeam, stdevBeam))

lCurrents = []
lCurrentsErr = []
lMeanRate = []
lMeanRateErr = []

lNP2Rate = []
lNP2RateErr = []

for i in xrange(len(lData)):
	lNP2Measurements = []
	totalTime = 0.
	for np2 in lTimeRanges[i][1]:
		lNP2Measurements.append(np2)
		totalTime += 300. # 300 s per measurement

	lNP2Rate.append(sum(lNP2Measurements)/totalTime * 2.9e4 / 6000  / 1.6 * distanceNormalization) # 6000 counts = 1 mrem, possibly keep efficiency of detector as free parameter and fit (see 3.2 in NP-2 manual)
	lNP2RateErr.append((sum(lNP2Measurements)**0.5 + sum(lNP2Measurements)*0.1)/totalTime  * 2.9e4 / 6000  / 1.6 * distanceNormalization)

	lMeanRate.append(lData[i][iMeanRate])
	lMeanRateErr.append(lData[i][iMeanRateErr] + (lData[i][iMeanRate])*0.05) # extra 5% is from simulation uncertainty
	lCurrents.append(lData[i][iMeanBeam])
	lCurrentsErr.append(lData[i][iMeanBeamErr])
	#lCurrents[i] -= lData[i][iMeanBeam]*10/1000. # (10 = 10 MOhm resistor * 1 uA)


plt.figure(1)
sp = plt.subplot(111)
plt.errorbar(lCurrents, lNP2Rate, fmt='bo', xerr=, yerr=lNP2RateErr)
#plt.errorbar(lCurrents, lMeanRate, fmt='ro', xerr=lCurrentsErr, yerr=lMeanRateErr)
#sp.set_yscale('log')

plt.title('Minitron Rate Study')
plt.xlabel('Current Setting [uA]')
plt.ylabel('Total minitron Rate [n / s]')

plt.show()


