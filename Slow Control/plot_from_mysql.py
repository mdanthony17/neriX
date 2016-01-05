#!/usr/bin/python

import ROOT as root
import MySQLdb
import array

deviceName = 'adc0'
measurementName = 'I_beam'
#change index below

sqlConnect = MySQLdb.connect('astro18.astronevis.columbia.edu', 'smac_reader', 'wq12op09', 'smac')
sqlCursor = sqlConnect.cursor()

sqlCursor.execute('select ' + str(deviceName) + '_id, ' + str(measurementName) + ' from ' + deviceName + ' where ' + deviceName + '_id > 1446642021 and ' + deviceName + '_id < 1447160421;')
deviceData = sqlCursor.fetchall()

time = array.array('d')
value = array.array('d')
countRows = 0

for row in deviceData:
	time.append(float(row[0])) #time is always first element
	value.append(float(row[1])) #index changes depending on value you choose
	countRows += 1

gDeviceProgression = root.TGraph(countRows, time, value)

gDeviceProgression.SetMarkerStyle(6)
gDeviceProgression.SetMarkerColor(root.kBlue)
gDeviceProgression.SetLineColor(root.kBlue)
gDeviceProgression.SetTitle('Slow Control Diagnostics')
#gDeviceProgression.GetXaxis().SetTitle('Linux Epoch Time')
gDeviceProgression.GetXaxis().SetTimeDisplay(1);
gDeviceProgression.GetXaxis().SetTimeFormat('%m-%d:%H')
gDeviceProgression.GetXaxis().SetTimeOffset(0, 'gmt')
gDeviceProgression.GetYaxis().SetTitle(measurementName)

canvas = root.TCanvas('canvas', 'canvas', 1)
gDeviceProgression.Draw('APL')
canvas.Update()

raw_input("Enter to continue...")