#!/usr/bin/python

import sys, math, re, time
import MySQLdb

# connect to the run database
def connect(database_user, database_pass):
	global connection, cursor

	return 0

	# open the connection to the database
	try:
		connection = MySQLdb.connect('xecluster01', database_user, database_pass, 'runs')
		cursor = connection.cursor()
	except MySQLdb.Error, e:
		print 'problem connecting to run database, error %d: %s' % (e.args[0], e.args[1])
		cursor = 0
	
	return cursor

# disconnect from the run database
def disconnect():
	global connection, cursor

	# close the cursor
	cursor.close()

	# close the connection to the database
	connection.close()

# get the daq xml configuration for a dataset
def get_daq_xml_config_string(dataset_name):
	global cursor

	query = 'SELECT daq_config FROM dataset WHERE dataset_name = "%s";' % dataset_name
	cursor.execute(query)
	daq_xml_config_string = cursor.fetchone()

	return daq_xml_config_string

# get the livetime for a dataset
def get_livetime(dataset_name):
	global cursor

	query = 'SELECT livetime FROM dataset WHERE dataset_name = "%s";' % dataset_name
	cursor.execute(query)
	livetime = float(cursor.fetchone()[0])

	return livetime

# get the electron lifetime (or a linearly interpolated value) for a dataset
def get_electron_lifetime(dataset_name, analysis_done_by=''):
	global cursor

	dataset_time = time.mktime(time.strptime(dataset_name, 'xe100_%y%m%d_%H%M'))

	# get the list of electron lifetimes from the runs.analysis table
	query = 'SELECT dataset_name,electron_lifetime FROM analysis INNER JOIN dataset ON dataset.run_id=analysis.run_id AND dataset.dataset_id=analysis.dataset_id'
	if analysis_done_by:
		query += ' WHERE analysis_done_by = "%s"' % (analysis_done_by)
	query += ';'
	cursor.execute(query)
	electron_lifetime_analyses = dict(cursor.fetchall())

	# if we have the electron lifetime for that dataset
	if dataset_name in electron_lifetime_analyses:
		electron_lifetime = electron_lifetime_analyses[dataset_name]
	else:
		# we have to interpolate, find the closest two electron lifetimes analyzed
		closest_before = 0
		closest_after = 0
		for analysis_dataset, analysis_electron_lifetime in electron_lifetime_analyses.iteritems():
			analysis_time = time.mktime(time.strptime(analysis_dataset, 'xe100_%y%m%d_%H%M'))
			# if the analysis is before
			if dataset_time-analysis_time > 0:
				if (dataset_time-analysis_time) < math.fabs(closest_before-dataset_time):
					electron_lifetime_before = analysis_electron_lifetime
					closest_before = analysis_time
			else:
				# the analysis is after
				if analysis_time-dataset_time < math.fabs(closest_after-dataset_time):
					electron_lifetime_after = analysis_electron_lifetime
					closest_after = analysis_time

		if closest_before and closest_after:
			electron_lifetime_slope = (electron_lifetime_after-electron_lifetime_before)/(closest_after-closest_before)
			electron_lifetime = electron_lifetime_slope*(dataset_time-closest_before)+electron_lifetime_before
		else:
			electron_lifetime = -1.

	return electron_lifetime

