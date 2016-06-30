#!/usr/bin/python

import MySQLdb, time, os


while True:

		sqlConnect = MySQLdb.connect('astro18.astronevis.columbia.edu', 'smac_reader', 'wq12op09', 'smac')
		sqlCursor = sqlConnect.cursor()


		oneHourBefore = int(time.time()) - 60*10

		sqlCursor.execute('select I_beam from adc0 where adc0_id > %d;' % (oneHourBefore))
		deviceData = sqlCursor.fetchall()

		lCurrents = [0 for i in xrange(len(deviceData))]
		for i, set in enumerate(deviceData):
			lCurrents[i] = set[0]

		lengthOfList = len(lCurrents)

		if lengthOfList < 2:
			os.system('./smacd stop')
			time.sleep(20)
			os.system('./smacd start')
			time.sleep(60*5) # sleep 5 minutes to let the list refill
		else:
			time.sleep(60*5) # all good, rest five minutes

