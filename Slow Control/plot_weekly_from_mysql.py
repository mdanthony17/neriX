#!/usr/bin/python

import ROOT as root
import MySQLdb
import array, time

#list with values of interests' parameters
#interior lists go [name for graph, device name, row number]
lMeasurements = [['P_chamber (mbar)', 'adc0', 1],
				['T_top (C)', 'adc0', 2],
                ['Q_circulation (SLPM)', 'adc0', 3],
                ['T_ambient (C)', 'adc0', 4],
                ['Q_heater (%)', 'lakeshore0', 1],
                ['T_ptr (C)', 'lakeshore0', 2],
                ['T_coldfinger (C)', 'lakeshore0', 3],
				['Z_lm1', 'uti0', 3],
                ['Z_lm2', 'uti0', 4]]


#earlier time is a week before current time
currentTime = time.time()
earlierTime = time.time() - 604800*3


#list of canvases we will use in loop
lCanvases = []
lGraphs = []


measurementCounter = 0

for measurement in lMeasurements:
	measurementName = measurement[0]
	deviceName = measurement[1]
	rowNumber = measurement[2]

	sqlConnect = MySQLdb.connect('astro18.astronevis.columbia.edu', 'smac_reader', 'wq12op09', 'smac')
	sqlCursor = sqlConnect.cursor()

	sqlCursor.execute('select * from ' + deviceName + ' where ' + deviceName + '_id > '+ str(earlierTime) +' and ' + deviceName + '_id < '+ str(currentTime) +';')
	deviceData = sqlCursor.fetchall()

	time = array.array('d')
	value = array.array('d')

	countRows = 0

	for row in deviceData:
		time.append(float(row[0])) #time is always first element
		value.append(float(row[rowNumber])) #index changes depending on value you choose
		countRows += 1
		
	lCanvases.append(root.TCanvas('canvas' + str(measurementCounter), 'canvas' + str(measurementCounter), 640, 480))

	lGraphs.append(root.TGraph(countRows, time, value))

	lGraphs[measurementCounter].SetMarkerStyle(6)
	lGraphs[measurementCounter].SetMarkerColor(root.kBlue)
	lGraphs[measurementCounter].SetLineColor(root.kBlue)
	lGraphs[measurementCounter].SetTitle(measurementName)
	#lGraphs[measurementCounter].GetXaxis().SetTitle('Linux Epoch Time')
	lGraphs[measurementCounter].GetXaxis().SetTimeDisplay(1);
	lGraphs[measurementCounter].GetXaxis().SetTimeFormat('%y-%m-%d')
	lGraphs[measurementCounter].GetXaxis().SetTimeOffset(0, 'gmt')
	lGraphs[measurementCounter].GetYaxis().SetTitle(measurementName)

	lGraphs[measurementCounter].Draw('APL')
	lCanvases[measurementCounter].Update()

	measurementCounter += 1


raw_input("Enter to continue...")