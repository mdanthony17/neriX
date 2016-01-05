#!/usr/bin/python

from ROOT import TChain, TFile, TObject, TObjString
import sys, os, re, shutil, xml.dom.minidom, getopt

def add_slash(path):
	if path.endswith('/'):
		return path
	else:
		return path+'/'

def merge(switches, xml_config, dataset, first, last):
	# get the xerawdp version we are using
	import config
	version = config.version()

	ignore_missing_files = '-i' in map(lambda o: o[0], switches)

	# get the processed data and merged data directories
	xml_config_dom = xml.dom.minidom.parse(xml_config)
	raw_data_dir = add_slash(str(xml_config_dom.getElementsByTagName('raw_data_dir')[0].childNodes[0].data))
	processed_data_dir = add_slash(str(xml_config_dom.getElementsByTagName('processed_data_dir')[0].childNodes[0].data))
	merged_data_dir = add_slash(str(xml_config_dom.getElementsByTagName('merged_data_dir')[0].childNodes[0].data))
	xml_config_dom.unlink()

	# get the livetime from the run database (or the log file)
	import run_database

	if run_database.connect('runs_reader', '1q0p2w9o'):
		livetime = run_database.get_livetime(dataset)
		run_database.disconnect()
	else:
		daq_log_filename = raw_data_dir+dataset+'/'+dataset+'.log'
		livetime = 0.
		if os.path.exists(daq_log_filename):
			total_measuring_time_regexp = '^Total Measuring Time = (\d+\.\d+) s'

			daq_log = open(daq_log_filename, 'r')
			for line in daq_log:
				if re.search(total_measuring_time_regexp, line):
					livetime += float(re.search(total_measuring_time_regexp, line).group(1))

	# open the first file to check whether to also merge T2 and T3
	do_chain0, do_chain1, do_chain2, do_chain3 = 0, 0, 0, 0
	first_file = TFile('%s/%s/v%s/%s_%06i.root' % (processed_data_dir, dataset, version, dataset, 0), 'read')
	if first_file.Get('T0'):
		do_chain0 = 1
	if first_file.Get('T1'):
		do_chain1 = 1
	if first_file.Get('T2'):
		do_chain2 = 1
	if first_file.Get('T3'):
		do_chain3 = 1

	# create the merged file
	new_file = TFile(merged_data_dir+'%s.root.merging' % dataset, 'recreate')

	# create the tree chains
	if do_chain0:
		chain0 = TChain('T0')
	if do_chain1:
		chain1 = TChain('T1')
	if do_chain2:
		chain2 = TChain('T2')
	if do_chain3:
		chain3 = TChain('T3')

	if do_chain0:
		chain0.SetMaxTreeSize(100000000000)
	if do_chain1:
		chain1.SetMaxTreeSize(100000000000)
	if do_chain2:
		chain2.SetMaxTreeSize(100000000000)
	if do_chain3:
		chain3.SetMaxTreeSize(100000000000)

	# check and add the dataset files to the tree chains
	for i in range(first, last+1):
		file = TFile('%s/%s/v%s/%s_%06i.root' % (processed_data_dir, dataset, version, dataset, i), 'read')

		# check that the file is fine
		if ignore_missing_files and (not file or file.IsZombie()):
			print 'file missing %s_%06i.root' % (dataset, i)
			continue
		elif not file or file.IsZombie() or (do_chain0 and not file.Get('T0')) or (do_chain1 and not file.Get('T1')) or (do_chain2 and not file.Get('T2')) or (do_chain3 and not file.Get('T3')):
			os.unlink(merged_data_dir+'%s.root.merging' % dataset)
			return 1
		else:
			file.Close()
			new_file.cd()

		# add it to the chains
		if do_chain0:
			chain0.Add('%s/%s/v%s/%s_%06i.root' % (processed_data_dir, dataset, version, dataset, i))
		if do_chain1:
			chain1.Add('%s/%s/v%s/%s_%06i.root' % (processed_data_dir, dataset, version, dataset, i))
		if do_chain2:
			chain2.Add('%s/%s/v%s/%s_%06i.root' % (processed_data_dir, dataset, version, dataset, i))
		if do_chain3:
			chain3.Add('%s/%s/v%s/%s_%06i.root' % (processed_data_dir, dataset, version, dataset, i))

	# clone the trees
	if do_chain0:
		new_tree0 = chain0.CloneTree(-1, 'fast')
	if do_chain1:
		new_tree1 = chain1.CloneTree(-1, 'fast')
	if do_chain2:
		new_tree2 = chain2.CloneTree(-1, 'fast')
	if do_chain3:
		new_tree3 = chain3.CloneTree(-1, 'fast')

	# unreference the chains
	chain0 = 0
	chain1 = 0
	chain2 = 0
	chain3 = 0

	# write the xerawdp_version and xml configuration used
	xerawdp_version = TObjString(version)
	xerawdp_version.Write('xerawdp_version', TObject.kOverwrite)

	xml_config_file = open(xml_config, 'r')
	xerawdp_xml_config = TObjString(xml_config_file.read())
	xerawdp_xml_config.Write('xerawdp_xml_config', TObject.kOverwrite)
	xml_config_file.close()

	# write the dataset livetime
	dataset_livetime = TObjString(str(livetime))
	dataset_livetime.Write('dataset_livetime', TObject.kOverwrite)

	# write the new file
	new_file.Write('', TObject.kOverwrite)
	new_file.Close()

	shutil.move(merged_data_dir+'%s.root.merging' % dataset, merged_data_dir+'%s.root' % dataset)

	return 0

def usage():
	print 'merge.py [switches] <xml_config> <dataset> <first> <last>'
	print
	print 'switches:'

if __name__ == '__main__':
	try:
		switches, args = getopt.getopt(sys.argv[1:], 'i', ['ignore-missing-files'])
	except getopt.GetoptError:
		usage()

	if len(args) != 4:
		usage()
		sys.exit(1)

	xml_config = args[0]
	dataset = args[1]
	first = int(args[2])
	last = int(args[3])

	print 'Merging all files...',
	sys.stdout.flush()

	ret = merge(switches, xml_config, dataset, first, last)

	print 'done'

	sys.exit(ret)

