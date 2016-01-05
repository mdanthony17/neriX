#!/usr/bin/python

import ROOT as root
import MySQLdb
import array, time, smtplib, libcaenhv, os
from ctypes import *

#email settings
sender = 'xedaq@astro18.astronevis.columbia.edu'
receiver = ['mda2149@columbia.edu', 'luke.goetzke@gmail.com', 'marc.weber@astro.columbia.edu']
message = """From: xedaq <xedaq@astro18.astronevis.columbia.edu>
To: Matt <mda2149@columbia.edu>, Luke <luke.goetzke@gmail.com>, Marc <marc.weber@astro.columbia.edu>
Subject: PMT Tripped - automatic restart occuring

PMT Tripped - commencing automatic restart
"""

prevTime = 0

deviceName = 'caen0'
measurementName = ''

sqlConnect = MySQLdb.connect('astro18.astronevis.columbia.edu', 'smac_reader', 'wq12op09', 'smac')
sqlCursor = sqlConnect.cursor()

while 1:
	sqlCursor.execute('select * from ' + deviceName + ' where ' + deviceName + '_id > ' + str(time.time() - 30) + ';')
	deviceData = sqlCursor.fetchall()

	#print len(deviceData)
	#print deviceData

	length = len(deviceData)

	#check length of device data to see if something new is available
	if length > 0 and deviceData[0][0] != prevTime:
		prevTime = deviceData[length - 1][0]
		for i in xrange(0, 6):
			if deviceData[length - 1][3 + 3*i] > 7:
				#send email that you are resetting PMT
				smtpObj = smtplib.SMTP('localhost')
				smtpObj.sendmail(sender, receiver, message)
				
				#reset PMT voltage
				handle = c_int()
				res = libcaenhv.init_system(system=2, link_type=0, arg='129.236.254.45', username='user', password='q1w2e3r4', handle=byref(handle))

				parameter_values_list = [int(1)]
				res = libcaenhv.set_channel_parameter_bypass(handle=handle, slot=0, parameter_name='Pw', nb_channels=1, channels_list=[i + 1], parameter_values_list=parameter_values_list)

				libcaenhv.deinit_system(handle)




	time.sleep(17)

