#!/usr/bin/python

import sys, math, re, time
import MySQLdb

# connect to the pmt database
def connect(database_user, database_pass):
	global connection, cursor

	# open the connection to the database
	try:
		connection = MySQLdb.connect('xecluster01', database_user, database_pass, 'pmt_database')
		cursor = connection.cursor()
	except MySQLdb.Error, e:
		print 'problem connection to database, error %d: %s' % (e.args[0], e.args[1])
		cursor = 0
	
	return cursor

# disconnect from the pmt database
def disconnect():
	global connection, cursor

	# close the cursor
	cursor.close()

	# close the connection to the database
	connection.close()

# get which calibration files should be used for a given dataset
def get_calibration_files(dataset_name):
	global cursor

	dataset_time = time.mktime(time.strptime(dataset_name, 'xe100_%y%m%d_%H%M'))

	# get the list of calibration dates from the database
	query = 'SELECT DISTINCT file_name FROM gain_calibration;'
	cursor.execute(query)
	calibrations = [[row[0], row[0].split('/')[-1]] for row in cursor.fetchall()]

	# now get the closest calibration before and after the dataset
	pmt_array_calibrations = {'before':{}, 'after':{}}
	for pmt_array in ['top', 'bottom', 'topveto', 'bottomveto']:
		closest_before = 0
		closest_after = 0
		for calibration in calibrations:
			if re.match('^%s\\d{6}_\\d{4}.gain' % pmt_array, calibration[1]):
				calibration_file = calibration[0]
				calibration_time = time.mktime(time.strptime(calibration[1], '%s%%y%%m%%d_%%H%%M.gain' % pmt_array))
				# if the calibration is before
				if dataset_time-calibration_time > 0:
					if (dataset_time-calibration_time) < math.fabs(closest_before-dataset_time):
						pmt_array_calibrations['before'][pmt_array] = [calibration_file, calibration_time]
						closest_before = calibration_time
				else:
					# the calibration is after
					if calibration_time-dataset_time < math.fabs(closest_after-dataset_time):
						pmt_array_calibrations['after'][pmt_array] = [calibration_file, calibration_time]
						closest_after = calibration_time
	
	# get the high voltages for the calibration before the dataset
	query = 'SELECT hv_value FROM gain_calibration'
	query += ' WHERE file_name = \'%s\' OR file_name = \'%s\' OR file_name = \'%s\' OR file_name = \'%s\''
	query += ' ORDER BY pmt_no_in_xenon100'
	cursor.execute(query % tuple(map(lambda x: x[0], pmt_array_calibrations['before'].values())))
	pmt_hvs_before = [row[0] for row in cursor.fetchall()]
	closest_before = pmt_array_calibrations['before']['top'][1]

	# if we have a calibration after the dataset, get the high voltages
	if pmt_array_calibrations['after']:
		cursor.execute(query % tuple(map(lambda x: x[0], pmt_array_calibrations['after'].values())))
		pmt_hvs_after = [row[0] for row in cursor.fetchall()]
		closest_after = pmt_array_calibrations['after']['top'][1]

		# if the high voltages are the same, choose the calibration closest in time
		if pmt_hvs_before == pmt_hvs_after:
			if math.fabs(closest_after-dataset_time) < math.fabs(dataset_time-closest_before):
				chosen_time = 'after'
			else:
				chosen_time = 'before'
		else:
			chosen_time = 'before'
	else:
		chosen_time = 'before'

	calibration_files = [x[0] for x in pmt_array_calibrations[chosen_time].values()]

	return calibration_files

# get the gains for calibration files
def get_calibration_values(calibration_files, header):
	global cursor

	def unnone(x):
		if x:
			return x
		else:
			return 0

	columns = header.split()

	# get the PMT gains, sigmas and HVs for the calibration before and after the dataset
	query = 'SELECT gain_calibration.pmt_no_in_xenon100,' + ','.join(columns) + ' FROM gain_calibration'
	query += ' LEFT JOIN pmt ON gain_calibration.pmt_no_in_xenon100=pmt.pmt_no_in_xenon100'
	query += ' LEFT JOIN pmt_quantum_efficiency ON pmt_quantum_efficiency.serial_no=pmt.serial_no'
	query += ' WHERE file_name = \'%s\' OR file_name = \'%s\' OR file_name = \'%s\' OR file_name = \'%s\''
	query += ' ORDER BY pmt_no_in_xenon100'
	cursor.execute(query % tuple(calibration_files))
	columns.insert(0,'pmt')
	calibration_values = [dict([(h,unnone(row[i])) for i,h in enumerate(columns)]) for row in cursor.fetchall()]

	return calibration_values

