#!/usr/bin/python

import xml.dom.minidom
import sys, os, signal, re, tempfile, shutil, getopt, time

def add_slash(path):
	if path.endswith('/'):
		return path
	else:
		return path+'/'

def get_daq_xml_config_dom(xml_config_dom, dataset):
	# get the raw data directory from the xml configuration
	raw_data_dir = add_slash(str(xml_config_dom.getElementsByTagName('raw_data_dir')[0].childNodes[0].data))

	# if we still have the raw data, get the daq xml configuration from there
	daq_xml_config_path = raw_data_dir+dataset+'/'+dataset+'.xml'
	if os.path.exists(daq_xml_config_path):
		daq_xml_config_dom = xml.dom.minidom.parse(daq_xml_config_path)
	else:
		# we don't have the raw data anymore, get daq xml configuration from the runs database
		import run_database

		if run_database.connect('runs_reader', '1q0p2w9o'):
			daq_xml_config_string = run_database.get_daq_xml_config_string(dataset)
			run_database.disconnect()

			daq_xml_config_dom = xml.dom.minidom.parseString(daq_xml_config_string)
		else:
			print 'error: could not get daq xml configuration!'
			sys.exit(1)

	return daq_xml_config_dom

def fill_in_external_gains(xml_config_dom, dataset):
	# map of phillips 770 x10 amplifier to pmt numbers
	phillips_pmt_map = {0:[1, 16], 1:[17, 32], 2:[33, 48], 3:[49, 64], 4:[65, 80], 5:[81, 96], 6:[97, 98], 7:[99, 114], 8:[115, 130], 9:[131, 146], 10:[147, 162], 11:[163, 178], 12:[179, 194], 13:[195, 210], 14:[211, 226], 15:[227, 242]}

	# parse the daq xml configuration and get the external gain
	print 'Getting external gains from daq xml configuration'
	daq_xml_config_dom = get_daq_xml_config_dom(xml_config_dom, dataset)
	phillips = str(daq_xml_config_dom.getElementsByTagName('phillips')[0].childNodes[0].data)
	if not phillips:
		print 'error: could not get the external gains from the daq xml configuration!'
		sys.exit(1)
	
	daq_xml_config_dom.unlink()

	external_gains = []

	pmt_range = phillips_pmt_map[0]
	previous_state = bool((1 << 15) & int(phillips, 16))

	for phillips_nb in range(1, len(phillips_pmt_map)):
		current_state = bool((1 << (15-phillips_nb)) & int(phillips, 16))
		if current_state == previous_state:
			pmt_range[1] = phillips_pmt_map[phillips_nb][1]

			if phillips_nb == len(phillips_pmt_map)-1:
				external_gains.append('%d-%d:%0.2f' % (tuple(pmt_range)+((1, 10)[previous_state],)))
		else:
			external_gains.append('%d-%d:%0.2f' % (tuple(pmt_range)+((1, 10)[previous_state],)))
			pmt_range = phillips_pmt_map[phillips_nb]
		previous_state = current_state

	# write the external gains so that xerawdp can read them from the xml
	pmts_node = xml_config_dom.getElementsByTagName('pmts')[0]
	calibration_source_node = pmts_node.getElementsByTagName('calibration_source')[0]

	external_gains_node = xml_config_dom.createElement('external_gains')
	external_gains_node.appendChild(xml_config_dom.createTextNode(', '.join(external_gains)))

	pmts_node.insertBefore(external_gains_node, calibration_source_node)
	pmts_node.insertBefore(xml_config_dom.createTextNode('\n\t\t'), calibration_source_node)

def fill_in_pmt_gains(xml_config_dom, dataset):
	# get the database settings from the xml configuration
	pmts_node = xml_config_dom.getElementsByTagName('pmts')[0]
	calibration_source_node = pmts_node.getElementsByTagName('calibration_source')[0]
	database_node = calibration_source_node.getElementsByTagName('database')[0]

	# get the username and password
	database_user = str(database_node.getElementsByTagName('username')[0].childNodes[0].data)
	database_pass = str(database_node.getElementsByTagName('password')[0].childNodes[0].data)

	# get the header
	header = str(database_node.getElementsByTagName('header')[0].childNodes[0].data)

	# get calibration files and values for this dataset
	import pmt_database

	print 'Getting PMT calibration from database,',

	pmt_database.connect(database_user, database_pass)
	calibration_files = pmt_database.get_calibration_files(dataset)
	calibration_values = pmt_database.get_calibration_values(calibration_files, header)
	pmt_database.disconnect()

	gain_files_regexp = '(top|bottom|topveto|bottomveto)(\\d{2})(\\d{2})(\\d{2})_(\\d{2})(\\d{2}).gain'
	calibration_date = '20%s-%s-%s %s:%s' % re.match(gain_files_regexp, calibration_files[0].split('/')[-1]).groups()[1:]

	print 'will use the one from %s' % calibration_date

	# write the pmt calibration files and values so that xerawdp can read them from the xml
	calibration_files_node = xml_config_dom.createElement('files')
	database_node.appendChild(xml_config_dom.createTextNode('\t'))
	database_node.appendChild(calibration_files_node)
	calibration_files_text = xml_config_dom.createTextNode(str.join(', ', [f.split('/')[-1] for f in calibration_files]))
	calibration_files_node.appendChild(calibration_files_text)
	database_node.appendChild(xml_config_dom.createTextNode('\n\t\t\t'))

	calibration_values_node = xml_config_dom.createElement('values')
	database_node.appendChild(xml_config_dom.createTextNode('\t'))
	database_node.appendChild(calibration_values_node)
	entry_format = ((len(header.split())*'%%(%s)0.2e ') % tuple(header.split()))[:-1]
	calibration_values_text = xml_config_dom.createTextNode(str.join(', ', [('%(pmt)d:['+entry_format+']') % e for e in calibration_values]))
	calibration_values_node.appendChild(calibration_values_text)
	database_node.appendChild(xml_config_dom.createTextNode('\n\t\t\t'))

def fill_in_electron_lifetime(xml_config_dom, dataset):
	# get the signal corrections settings from the xml configuration
	signal_corrections_node = xml_config_dom.getElementsByTagName('signal_corrections')[0]
	s2_corrections_node = signal_corrections_node.getElementsByTagName('s2')[0]
	s2_axial_correction_node = s2_corrections_node.getElementsByTagName('axial_correction')[0]
	apply_correction_node = s2_axial_correction_node.getElementsByTagName('apply_correction')[0]

	try:
		electron_lifetime_node = s2_axial_correction_node.getElementsByTagName('electron_lifetime')[0]
		electron_lifetime = float(electron_lifetime_node.childNodes[0].data)
	except:
		electron_lifetime_node = 0
		electron_lifetime = -1.

	try:
		analysis_done_by_node = s2_axial_correction_node.getElementsByTagName('analysis_done_by')[0]
		analysis_done_by = str(analysis_done_by_node.childNodes[0].data)
	except:
		analysis_done_by = ''

	if electron_lifetime > 0:
		# use the electron lifetime as given in the xml configuration
		print 'Using the electron lifetime value of %0.2f us from the xml configuration' % electron_lifetime
	else:
		# getting the electron lifetime from the runs database
		import run_database

		if not analysis_done_by:
			print 'Getting electron lifetime from database,',
		else:
			print 'Getting electron lifetime from database (analysis_done_by=\'%s\'),' % analysis_done_by,

		if run_database.connect('runs_reader', '1q0p2w9o'):
			electron_lifetime = run_database.get_electron_lifetime(dataset, analysis_done_by)
			run_database.disconnect()

			# if the electron lifetime is a proper value
			if electron_lifetime > 0:
				print 'will use a value of %0.2f us' % electron_lifetime

				# write the electron lifetime value so that xerawdp can read it from the xml
				for node in electron_lifetime_node.childNodes:
					electron_lifetime_node.removeChild(node)
				electron_lifetime_node.appendChild(xml_config_dom.createTextNode('%0.2f' % electron_lifetime))

		if electron_lifetime < 0:
			# we could not get a proper value from the database, 
			print 'error, will deactivate s2 axial correction'
			for node in apply_correction_node.childNodes:
				apply_correction_node.removeChild(node)
			apply_correction_node.appendChild(xml_config_dom.createTextNode('0'))

def fill_in_use_blind_mode(xml_config_dom, dataset, use_blind_mode):
	# get the blind-mode settings from the xml configuration
	blind_mode_node = xml_config_dom.getElementsByTagName('blind_mode')[0]
	cut_node = blind_mode_node.getElementsByTagName('cut')[0]

	# write in the xml file if this dataset requires using the blind-mode
	use_blind_mode_node = xml_config_dom.createElement('use_blind_mode')
	use_blind_mode_node.appendChild(xml_config_dom.createTextNode('%d' % use_blind_mode))

	blind_mode_node.insertBefore(use_blind_mode_node, cut_node)
	blind_mode_node.insertBefore(xml_config_dom.createTextNode('\n\t\t'), cut_node)

def process_using_raw_data(xml_parameters, xerawdp_switches, xml_config, dataset, nb_processes):
	print 'Processing using raw data and %d processor(s)' % nb_processes

	# xml parameters
	raw_data_dir = xml_parameters['raw_data_dir']
	processed_data_dir = xml_parameters['processed_data_dir']
	merged_data_dir = xml_parameters['merged_data_dir']
	minimum_bias_data_dir = xml_parameters['minimum_bias_data_dir']
	minimum_bias_file_creation = xml_parameters['minimum_bias_file_creation']
	minimum_bias_prefix = xml_parameters['minimum_bias_prefix']
	minimum_bias_cut = xml_parameters['minimum_bias_cut']
	use_blind_mode = xml_parameters['use_blind_mode']
	blinding_cut = xml_parameters['blinding_cut']

	# get the xerawdp version this script comes from
	import config
	version = config.version()

	# check if we are running from an installed path
	if os.path.basename(sys.path[0]) == 'scripts':
		running_from_installed_path = 0
		src_dir = os.path.join(sys.path[0], '..', 'src')
	else:
		running_from_installed_path = 1

	# get the list of raw data files from the dataset
	files = os.listdir(raw_data_dir+dataset)
	files.sort()

	filter_datasets = lambda f: re.search(dataset + r'_\d{6}\.xed', f) != None
	dataset_files = filter(filter_datasets, files)
	nb_files = len(dataset_files)

	# if the number of files doesn't match
	if nb_files != int((dataset_files[-1])[-10:-4])+1:
		print 'error: dataset missing some files!'
		sys.exit(1)
	
	# if this is blind-mode data, make it readable first
	if use_blind_mode:
		print 'Provide the data archive password to proceed'

		import pexpect, getpass

		user = 'root'
		host = 'xecluster20'
		password = getpass.getpass('Password:')

		cmd = ''
		cmd += 'chattr -R -i %s/%s/' % (raw_data_dir, dataset)
		cmd += ' && chmod a+r %s/%s/*.xed' % (raw_data_dir, dataset)
		cmd += ' && chattr -R +i %s/%s/' % (raw_data_dir, dataset)

		child = pexpect.spawn('ssh -l %s %s \'%s\'' % (user, host, cmd))
		i = child.expect([pexpect.TIMEOUT, 'Password: '])
		if i == 0: # Timeout
			print 'error: could not login into data archive!'
			print child.before, child.after
			sys.exit(1)
		child.sendline(password)
		child.expect(pexpect.EOF)
		child.close()

		time.sleep(5)

	# split files into nb_processes lists
	split_file_ids = dict([[process_nb, []] for process_nb in range(0, nb_processes)])
	for file_id in range(0, nb_files):
		split_file_ids[file_id % nb_processes].append(file_id)

	# fork into the desired number of processes
	children = []
	for process_nb in range(0, nb_processes):
		try:
			pid = os.fork()

			if pid:
				# this is the parent so push the children on a stack
				children.append(pid)
			else:
				# this is a child
				for file_id in split_file_ids[process_nb]:
					dataset_file = '%s_%0.6d' % (dataset, file_id)
					print '  xerawdp[%d]: started file %s' % (process_nb, dataset_file)
					if running_from_installed_path:
						os.system('xerawdp-%s -w %s %s' % (version, xml_config, dataset_file))
					else:
						os.system('%s/xerawdp -w %s %s' % (src_dir, xml_config, dataset_file))
					print '  xerawdp[%d]: finished file %s' % (process_nb, dataset_file)

				print '  xerawdp[%d]: done' % process_nb
				sys.exit(0)

		except OSError:
			print 'error: couldn\'t fork!'
			sys.exit(0)

	# wait for children
	for child in children:
		try:
			os.waitpid(child, 0)
		except KeyboardInterrupt:
			for child in children:
				os.kill(child, signal.SIGTERM)
			sys.exit(0)

	# merging files
	import merge
	print 'Merging all files...',
	sys.stdout.flush()
	if merge.merge([], xml_config, dataset, 0, nb_files-1) != 0:
		print 'error: problem encountered while merging files!'
		sys.exit(1)
	print 'done'

	# make the merged file group writable
	os.system('chmod g+w %s' % (merged_data_dir+dataset+'.root'))

	# deleting temporary files
	print 'Deleting temporary files...',
	sys.stdout.flush()
	shutil.rmtree('%s/%s' % (processed_data_dir, dataset))
	print 'done'

	# merged file name with path
	merged_file_with_path = merged_data_dir+dataset+'.root'

	# if we should create a minimum bias file
	if minimum_bias_file_creation:
		print 'Creating minimum bias file...',
		sys.stdout.flush()
		minimum_bias_file_with_path = minimum_bias_data_dir+minimum_bias_prefix+dataset+'.root'

		# apply the minimum bias cut
		import weed
		weed.weed(merged_file_with_path, minimum_bias_cut, minimum_bias_file_with_path)
		print 'done'

	# if this is blind-mode data
	if use_blind_mode:
		# add .blind to the merged file name
		blind_file_with_path = merged_data_dir+dataset+'.root.blind'
		os.rename(merged_file_with_path, blind_file_with_path)

		# apply the blinding cut
		print 'Removing blind events from merged file...',
		sys.stdout.flush()
		import weed
		weed.weed(blind_file_with_path, blinding_cut, merged_file_with_path)
		print 'done'

		# make the blind merged file unreadable
		os.system('chmod a-r %s' % blind_file_with_path)

		# if we also created a minimum bias file we need to blind it
		if minimum_bias_file_creation:
			# add .blind to the minimum bias file name
			blind_minimum_bias_file_with_path = minimum_bias_data_dir+minimum_bias_prefix+dataset+'.root.blind'
			os.rename(minimum_bias_file_with_path, blind_minimum_bias_file_with_path)

			# apply the blinding cut
			print 'Removing blind events from minimum bias file...',
			sys.stdout.flush()
			import weed
			weed.weed(blind_minimum_bias_file_with_path, blinding_cut, minimum_bias_file_with_path)
			print 'done'

			# make the minimum bias blind merged file unreadable
			os.system('chmod a-r %s' % blind_minimum_bias_file_with_path)

		# make the raw data unreadable again and the processed files owned by root
		cmd = ''
		cmd += 'chattr -R -i %s/%s/' % (raw_data_dir, dataset)
		cmd += ' && chmod a-r %s/%s/*.xed' % (raw_data_dir, dataset)
		cmd += ' && chattr -R +i %s/%s/' % (raw_data_dir, dataset)
		cmd += ' && chown root:root %s %s' % (blind_file_with_path, merged_file_with_path)
		if minimum_bias_file_creation:
			cmd += ' && chown root:root %s %s' % (blind_minimum_bias_file_with_path, minimum_bias_file_with_path)
		child = pexpect.spawn('ssh -l %s %s \'%s\'' % (user, host, cmd))
		i = child.expect([pexpect.TIMEOUT, 'Password: '])
		if i == 0: # Timeout
			print 'error: could not login into data archive!'
			print child.before, child.after
			sys.exit(1)
		child.sendline(password)
		child.expect(pexpect.EOF)
		child.close()

	print 'Finshed processing dataset %s' % dataset

def process_using_merged_data(xml_parameters, xerawdp_switches, xml_config, dataset):
	print 'Processing using merged data, only one processor required'

	# xml parameters
	merged_data_dir = xml_parameters['merged_data_dir']

	merged_file_with_path = merged_data_dir+dataset+'.root'
	backup_merged_file_with_path = merged_data_dir+dataset+'.root.backup'

	# get the xerawdp version this script comes from
	import config
	version = config.version()

	# check if we are running from an installed path
	if os.path.basename(sys.path[0]) == 'scripts':
		running_from_installed_path = 0
		src_dir = os.path.join(sys.path[0], '..', 'src')
	else:
		running_from_installed_path = 1

	# save a copy the original merged data file
	print 'Making a backup of the merged file...',
	sys.stdout.flush()
	shutil.copy(merged_file_with_path, backup_merged_file_with_path)
	print 'done'

	# reprocess the merged file
	print '  xerawdp[0]: started file %s' % (dataset+'.root')
	if running_from_installed_path:
		ret = os.system('xerawdp-%s %s %s %s' % (version, xerawdp_switches, xml_config, dataset))
	else:
		ret = os.system('%s/xerawdp %s %s %s' % (src_dir, xerawdp_switches, xml_config, dataset))
	print '  xerawdp[0]: finished file %s' % (dataset+'.root')

	# if something went wrong replace the file with the backup file
	if ret:
		print 'error: xerawdp did not finish gracefully, restoring from backup'
		print 'Dataset %s was NOT processed' % dataset
		shutil.move(backup_merged_file_with_path, merged_file_with_path)
	else:
		os.unlink(backup_merged_file_with_path)
		print 'Finshed processing dataset %s' % dataset

def process(switches, xml_config_template, dataset, max_nb_processes=1):
	print 'Processing dataset %s' % dataset

	# copy the xml configuration to a temporary file
	xml_config = tempfile.mktemp()

	# parse the xml configuration
	xml_config_dom = xml.dom.minidom.parse(xml_config_template)

	# get the raw data, processed data and merged data directories
	raw_data_dir = add_slash(str(xml_config_dom.getElementsByTagName('raw_data_dir')[0].childNodes[0].data))
	processed_data_dir = add_slash(str(xml_config_dom.getElementsByTagName('processed_data_dir')[0].childNodes[0].data))
	merged_data_dir = add_slash(str(xml_config_dom.getElementsByTagName('merged_data_dir')[0].childNodes[0].data))

	# check if we need to fetch the external gains
	try:
		pmts_node = xml_config_dom.getElementsByTagName('pmts')[0]
		external_gains_node = pmts_node.getElementsByTagName('external_gains')[0]
		need_external_gains = 0
	except:
		need_external_gains = 1

	# get the pmt calibration source
	try:
		pmts_node = xml_config_dom.getElementsByTagName('pmts')[0]
		calibration_source_node = pmts_node.getElementsByTagName('calibration_source')[0]
		selected_source_node = filter(lambda n: n.nodeType == n.ELEMENT_NODE, calibration_source_node.childNodes)[0]
		pmt_calibration_source = selected_source_node.localName
	except:
		pmt_calibration_source = ''

	# check if we should apply the axial correction
	try:
		signal_corrections_node = xml_config_dom.getElementsByTagName('signal_corrections')[0]
		s2_corrections_node = signal_corrections_node.getElementsByTagName('s2')[0]
		s2_axial_correction_node = s2_corrections_node.getElementsByTagName('axial_correction')[0]
		apply_correction_node = s2_axial_correction_node.getElementsByTagName('apply_correction')[0]
		apply_s2_axial_correction = int(apply_correction_node.childNodes[0].data)
	except:
		apply_s2_axial_correction = 0

	# parse the daq xml configuration and get source used
	daq_xml_config_dom = get_daq_xml_config_dom(xml_config_dom, dataset)
	source = str(daq_xml_config_dom.getElementsByTagName('source')[0].childNodes[0].data)
	if not source:
		print 'error: could not get the source from the daq xml configuration!'
		sys.exit(1)
	daq_xml_config_dom.unlink()

	# check and set production of minimum bias merged files
	try:
		minimum_bias_data_dir = add_slash(str(xml_config_dom.getElementsByTagName('minimum_bias_data_dir')[0].childNodes[0].data))
		minimum_bias_node = xml_config_dom.getElementsByTagName('minimum_bias')[0]
		cut_node = minimum_bias_node.getElementsByTagName('cut')[0]
		minimum_bias_cut = str(cut_node.childNodes[0].data)
		prefix_node = minimum_bias_node.getElementsByTagName('prefix')[0]
		minimum_bias_prefix = str(prefix_node.childNodes[0].data)
		minimum_bias_file_creation = 1
		print 'Minimum bias file creation is ON'
		print 'Minimum bias cut used:'
		print '  '+minimum_bias_cut
	except:
		minimum_bias_data_dir = ''
		minimum_bias_cut = ''
		minimum_bias_prefix = ''
		minimum_bias_file_creation = 0
		print 'Minimum bias file creation is OFF'

	# check and set usage of blind-mode processing depending on the source
	blind_mode_source_regexp = 'Dark\s*Matter'
	if re.search(blind_mode_source_regexp, source, re.IGNORECASE):
		blind_mode_node = xml_config_dom.getElementsByTagName('blind_mode')[0]
		cut_node = blind_mode_node.getElementsByTagName('cut')[0]
		use_blind_mode = 1
		blinding_cut = str(cut_node.childNodes[0].data)
		print 'Blind-mode processing is ON'
		print 'Blind-mode cut used:'
		print '  '+blinding_cut
	else:
		use_blind_mode = 0
		blinding_cut = ''
		print 'Blind-mode processing is OFF'

	# fill in external gains in the xml configuration
	if need_external_gains:
		fill_in_external_gains(xml_config_dom, dataset)

	# fill in pmt gains from the database (if needed)
	if pmt_calibration_source == 'database':
		fill_in_pmt_gains(xml_config_dom, dataset)
	
	# fill in electron lifetime value from database (if required)
	if apply_s2_axial_correction:
		fill_in_electron_lifetime(xml_config_dom, dataset)

	# fill in usage of blind-mode processing
	try:
		xml_config_dom.getElementsByTagName('blind_mode')[0]
		fill_in_use_blind_mode(xml_config_dom, dataset, use_blind_mode)
	except:
		pass

	# write the xml config with additional values filled in (external gains, pmt gains, etc)
	xml_config_file = open(xml_config, 'w')
	xml_config_file.write(xml_config_dom.toxml())
	xml_config_file.close()

	# save the xml parameters we will need
	xml_parameters = {}
	xml_parameters['raw_data_dir'] = raw_data_dir
	xml_parameters['processed_data_dir'] = processed_data_dir
	xml_parameters['merged_data_dir'] = merged_data_dir
	xml_parameters['minimum_bias_data_dir'] = minimum_bias_data_dir
	xml_parameters['minimum_bias_file_creation'] = minimum_bias_file_creation
	xml_parameters['minimum_bias_cut'] = minimum_bias_cut
	xml_parameters['minimum_bias_prefix'] = minimum_bias_prefix
	xml_parameters['use_blind_mode'] = use_blind_mode
	xml_parameters['blinding_cut'] = blinding_cut

	# check if we are forced to reprocess the dataset using raw data
	force_use_raw_data = '-f' in map(lambda o: o[0], switches)

	# check whether to accept impure ouput files for merged datasets that cannot be updated
	force_impure = '-i' in map(lambda o: o[0], switches)

	# xerawdp switches
	xerawdp_switches = ''
	if force_impure:
		xerawdp_switches += ' -i'

	# if we already processed this dataset (and we are not forced to reprocess it using raw data)
	if os.path.exists(merged_data_dir+dataset+'.root') and not force_use_raw_data:
		process_using_merged_data(xml_parameters, xerawdp_switches, xml_config, dataset)
	else:
		process_using_raw_data(xml_parameters, xerawdp_switches, xml_config, dataset, max_nb_processes)

	os.unlink(xml_config)

def usage():
	print 'process.py [switches] <xml_config> <dataset> [<max_nb_processes>]'
	print
	print 'switches:'
	print '  -f    force reprocessing of dataset'

if __name__ == '__main__':
	try:
		switches, args = getopt.getopt(sys.argv[1:], 'f', ['force-raw'])
	except getopt.GetoptError:
		usage()

	if len(args) < 2 or len(args) > 3:
		usage()
		sys.exit(1)

	xml_config_template = args[0]
	dataset = args[1]
	if len(args) == 3:
		max_nb_processes = int(args[2])
	else:
		max_nb_processes = 1

	process(switches, xml_config_template, dataset, max_nb_processes)

	sys.exit(0)

