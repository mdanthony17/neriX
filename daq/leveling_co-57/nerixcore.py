# add a slash to a path
def add_slash(path):
	if path.endswith('/'):
		return path
	else:
		return path+'/'

# load ROOT dictionaries we need
def load_root_dictionaries():
	from ROOT import gSystem, gROOT
	import os

	#modules_directory = '/home/goetzke/XENON/nerix/analysis/python_modules'
	modules_directory = 'python_modules'
	if not os.path.exists('%s/linkdef_C.dll' % modules_directory):
		linkdef = file('%s/linkdef.C' % modules_directory, 'w+')
		linkdef.write('#include <vector>\n')
		linkdef.write('#ifdef __MAKECINT__\n')
		linkdef.write('#pragma link C++ class vector<vector<int> >+;\n')
		linkdef.write('#pragma link C++ class vector<vector<float> >+;\n')
		linkdef.write('#endif\n')
		linkdef.close()

		gROOT.ProcessLine('.L %s/linkdef.C+' % modules_directory)

		os.unlink('%s/linkdef.C' % modules_directory)

	gSystem.Load('%s/linkdef_C.so' % modules_directory)

# set directory where root files are
def set_root_files_directory(directory):
	global _current_directory
	_current_directory = directory
	print 'nerixcore: root files directory is now: %s' % directory

# set run
def set_run(runs):
	global _current_runs
	# if we have a list of runs
	if runs.__class__.__name__ == 'str':
		_current_runs = [add_slash(runs)]
	elif runs.__class__.__name__ == 'list':
		_current_runs = map(add_slash, runs)
	else:
		print 'nerixcore: error: could not set run!'

	print 'nerixcore: runs are: %s' % ', '.join(map(lambda s: s.strip('/'), _current_runs))

# list root files available
def list_files():
	import os
	print 'nerixcore: listings root files available in current directory'
	for run in _current_runs:
		filenames = os.listdir(_current_directory+run)
		for filename in filenames:
			print filename

# set current data file(s)
def set_current_file(files):
	import ROOT, os
	global _aliases_defined, _current_runs, _current_files, _current_tree, _current_livetime
	_aliases_defined = 0
	
	# if we deal only with one file
	if files.__class__.__name__ == 'str':
		# first check which tree to get from the file
		run = filter(lambda s: os.path.exists(_current_directory+s+files), _current_runs)[0]

		_current_files = ROOT.TFile(_current_directory+run+files, 'read')
		if _current_files.Get('T0'):
			_current_tree = _current_files.Get('T0')
		else:
			_current_tree = _current_files.Get('T1')
			if _current_files.Get('T2'):
				_current_tree.AddFriend('T2')
			if _current_files.Get('T3'):
				_current_tree.AddFriend('T3')

		if _current_files.Get('dataset_livetime'):
			_current_livetime = float(str(_current_files.Get('dataset_livetime')))

	# else if we have a list of files
	elif files.__class__.__name__ == 'list':
		# we need to chain files, check which tree to chain
		run = filter(lambda s: os.path.exists(_current_directory+s+files[0]), _current_runs)[0]
		_current_files = ROOT.TFile(_current_directory+run+files[0], 'read')
		if _current_files.Get('T0'):
			_current_tree = ROOT.TChain('T0')
		else:
			_current_tree = ROOT.TChain('T1')

		_friend_trees = []

		if _current_files.Get('T2'):
			_friend_trees.append(ROOT.TChain('T2'))
		if _current_files.Get('T3'):
			_friend_trees.append(ROOT.TChain('T3'))

		for file in files:
			run = filter(lambda s: os.path.exists(_current_directory+s+file), _current_runs)[0]
			_current_tree.Add(_current_directory+run+file)
			for _friend_tree in _friend_trees:
				_friend_tree.Add(_current_directory+run+file)

			_temporary_file = ROOT.TFile(_current_directory+run+file, 'read')
			if _current_livetime != -1 and _temporary_file.Get('dataset_livetime'):
				_current_livetime += float(str(_temporary_file.Get('dataset_livetime')))
			else:
				_current_livetime = -1
			_temporary_file.Close()

		for _friend_tree in _friend_trees:
			_current_tree.AddFriend(_friend_tree)
	else:
		print 'error: specify a filename or list of filenames'

# set the position reconstruction algorithm
def use_positions_from(algorithm, corrected=1):
	global _aliases_defined
	global _positions_from
	global _positions_prefix

	if _aliases_defined:
		print 'nerixcore: aliases have to be redefined using define_aliases()'
	
	if corrected and algorithm in ['svm', 'nn', 'chi2']:
		_positions_prefix = 'c'
	else:
		_positions_prefix = ''

	if algorithm in ['svm', 'nn', 'chi2', 'maxpmt', 'fann']:
		_positions_from = algorithm
	else:
		print 'nerixcore: undefined position reconstruction algorithm, using svm'
		_positions_from = 'fann'
		
# define all aliases that we normally use
def define_aliases():
	global _current_tree, _aliases_defined
	global _energy_calibration
	global _positions_from
	global _positions_prefix
	print 'nerixcore: setting aliases...'

	# aliases
	_current_tree.SetAlias('eventid', 'EventId')
	_current_tree.SetAlias('time_s', 'TimeSec')
	_current_tree.SetAlias('time_us', 'TimeMicroSec')

	_current_tree.SetAlias('areatotraw', 'AreaTotRaw')
	_current_tree.SetAlias('s1sraw', 'S1sRaw')

	_current_tree.SetAlias('singlepeak', 'SinglePeak')
	_current_tree.SetAlias('singlebefore', 'SingleBefore')
	_current_tree.SetAlias('singleafter', 'SingleAfter')

	_current_tree.SetAlias('s2ssaturation', 'S2sSaturation')

	_current_tree.SetAlias('nbs1peaks', 'NbS1Peaks')
	_current_tree.SetAlias('s1speak', 'S1sPeak')
	_current_tree.SetAlias('s1sleftedge', 'S1sLeftEdge')
	_current_tree.SetAlias('s1srightedge', 'S1sRightEdge')
	_current_tree.SetAlias('s1swidth', 'S1sWidth')
	_current_tree.SetAlias('s1slowwidth', 'S1sLowWidth')
	_current_tree.SetAlias('s1sheight', 'S1sHeight')

	_current_tree.SetAlias('nbs2peaks', 'NbS2Peaks')
	_current_tree.SetAlias('s2speak', 'S2sPeak')
	_current_tree.SetAlias('s2sleftedge', 'S2sLeftEdge')
	_current_tree.SetAlias('s2srightedge', 'S2sRightEdge')
	_current_tree.SetAlias('s2swidth', 'S2sWidth')
	_current_tree.SetAlias('s2slowwidth', 'S2sLowWidth')
	_current_tree.SetAlias('s2sheight', 'S2sHeight')

	_current_tree.SetAlias('liqscipeak', 'LiqSciPeak')
	_current_tree.SetAlias('liqsciheight', 'LiqSciHeight')
	_current_tree.SetAlias('liqsciraw', 'LiqSciRaw')
	_current_tree.SetAlias('liqscitailraw', 'LiqSciTailRaw')

	_current_tree.SetAlias('naipeak', 'NaiPeak')
	_current_tree.SetAlias('naiheight', 'NaiHeight')
	_current_tree.SetAlias('nairaw', 'NaiRaw')

	_current_tree.SetAlias('geheight', 'GeHeight')
	_current_tree.SetAlias('gee_kev', 'GeHeight[0]*662./1.549')

	_current_tree.SetAlias('tacheight', 'TacHeight')
	_current_tree.SetAlias('tacmaximum', 'TacMaximum')

	_current_tree.SetAlias('areatot', 'AreaTot')

	_current_tree.SetAlias('s1tot', 'S1Tot')
	_current_tree.SetAlias('s1tottop', 'S1TotTop')
	_current_tree.SetAlias('s1totbottom', 'S1TotBottom')
	_current_tree.SetAlias('s1s', 'S1s')
	_current_tree.SetAlias('s1stot', 'S1sTot')
	_current_tree.SetAlias('s1stottop', 'S1sTotTop')
	_current_tree.SetAlias('s1stotbottom', 'S1sTotBottom')
	_current_tree.SetAlias('s1scoin', 'S1sCoin')

	_current_tree.SetAlias('s2tot', 'S2Tot')
	_current_tree.SetAlias('s2tottop', 'S2TotTop')
	_current_tree.SetAlias('s2totbottom', 'S2TotBottom')
	_current_tree.SetAlias('s2s', 'S2s')
	_current_tree.SetAlias('s2stot', 'S2sTot')
	_current_tree.SetAlias('s2stottop', 'S2sTotTop')
	_current_tree.SetAlias('s2stotbottom', 'S2sTotBottom')
	_current_tree.SetAlias('s2scoin', 'S2sCoin')

	_current_tree.SetAlias('tof0_ns', 'TimeOfFlight[0]')
	_current_tree.SetAlias('tof1_ns', 'TimeOfFlight[1]')

	# corrected quantities
	_current_tree.SetAlias('cs1tot', 'cS1Tot')
	_current_tree.SetAlias('cs1stot', 'cS1sTot')

	_current_tree.SetAlias('cs2tot', 'cS2Tot')
	_current_tree.SetAlias('cs2tottop', 'cS2TotTop')
	_current_tree.SetAlias('cs2totbottom', 'cS2TotBottom')

	_current_tree.SetAlias('cs2stot', 'cS2sTot')
	_current_tree.SetAlias('cs2stottop', 'cS2sTotTop')
	_current_tree.SetAlias('cs2stotbottom', 'cS2sTotBottom')

#    _current_tree.SetAlias('ctof0_ns', 'tof0_ns-(0.5208-74.8760*s1stot[0]^-0.6091)')
	_current_tree.SetAlias('ctof0_ns', 'tof0_ns-(6.931-28.05*s1stot[0]^-0.2047)')

	# derived quantities
	print '\n\nX and Y set for run 15!!\n\n'

	for pmt_id in range(0, 17):
		_current_tree.SetAlias('spe%d_e' % (pmt_id+1), 'SingleBefore[%d][1]+SingleBefore[%d][0]+SinglePeak[%d]+SingleAfter[%d][0]+SingleAfter[%d][1]' % (5*(pmt_id,)))

	_current_tree.SetAlias('s1asy0', '(S1sTotTop[0]-S1sTotBottom[0])/(S1sTotTop[0]+S1sTotBottom[0])')	

	_current_tree.SetAlias('s2extent', '(S2sRightEdge[0]-S2sLeftEdge[0])')

	_current_tree.SetAlias('dt0_us', '(S1sPeak[0]-S2sLeftEdge[0])/100.')
	_current_tree.SetAlias('dt1_us', '(S1sPeak[0]-S2sLeftEdge[1])/100.')
	_current_tree.SetAlias('dt2_us', '(S1sPeak[0]-S2sLeftEdge[2])/100.')
	_current_tree.SetAlias('dt3_us', '(S1sPeak[0]-S2sLeftEdge[3])/100.')
	_current_tree.SetAlias('dt_us', 'dt0_us')

	_current_tree.SetAlias('xp0_mm', 'S2sPos%s[0][0]*21.5' % _positions_from.capitalize())
	_current_tree.SetAlias('xp1_mm', 'S2sPos%s[1][0]*21.5' % _positions_from.capitalize())
	_current_tree.SetAlias('xp2_mm', 'S2sPos%s[2][0]*21.5' % _positions_from.capitalize())
	_current_tree.SetAlias('xp3_mm', 'S2sPos%s[3][0]*21.5' % _positions_from.capitalize())

	_current_tree.SetAlias('yp0_mm', 'S2sPos%s[0][1]*21.5' % _positions_from.capitalize())
	_current_tree.SetAlias('yp1_mm', 'S2sPos%s[1][1]*21.5' % _positions_from.capitalize())
	_current_tree.SetAlias('yp2_mm', 'S2sPos%s[2][1]*21.5' % _positions_from.capitalize())
	_current_tree.SetAlias('yp3_mm', 'S2sPos%s[3][1]*21.5' % _positions_from.capitalize())

	_current_tree.SetAlias('rp0_mm', 'sqrt(xp0_mm**2+yp0_mm**2)')
	_current_tree.SetAlias('rp1_mm', 'sqrt(xp1_mm**2+yp1_mm**2)')
	_current_tree.SetAlias('rp2_mm', 'sqrt(xp2_mm**2+yp2_mm**2)')
	_current_tree.SetAlias('rp3_mm', 'sqrt(xp3_mm**2+yp3_mm**2)')

	_current_tree.SetAlias('fp0_rad', 'atan2(yp0_mm, xp0_mm)')
	_current_tree.SetAlias('fp1_rad', 'atan2(yp1_mm, xp1_mm)')
	_current_tree.SetAlias('fp2_rad', 'atan2(yp2_mm, xp2_mm)')
	_current_tree.SetAlias('fp3_rad', 'atan2(yp3_mm, xp3_mm)')

	_current_tree.SetAlias('zp0_mm', 'dt0_us*2.2')
	_current_tree.SetAlias('zp1_mm', 'dt1_us*2.2')
	_current_tree.SetAlias('zp2_mm', 'dt2_us*2.2')
	_current_tree.SetAlias('zp3_mm', 'dt3_us*2.2')

	_current_tree.SetAlias('snr', 'log10(abs((S1Tot+S2Tot)/(AreaTot-S1Tot-S2Tot)))')
	_current_tree.SetAlias('psd0', 'LiqSciTailRaw[0]/LiqSciRaw[0]')
	_current_tree.SetAlias('psd1', 'LiqSciTailRaw[1]/LiqSciRaw[1]')

	_current_tree.SetAlias('liqsci0_kevee', '9.5827e-06*LiqSciRaw[0]+(-8.3207e+00)')
	_current_tree.SetAlias('liqsci1_kevee', '1.7268e-05*LiqSciRaw[1]+(-4.0517e+01)')

	_aliases_defined = 1

# initializations
load_root_dictionaries()

# default definitions
#_current_runs = ['run_02/', 'run_03/', 'run_04/', 'run_05/', 'run_06/', 'run_08/']
_current_runs = ['run_10/']
_current_directory = '/data/scratch/xenon/nerix/data/'
_current_livetime = 0.
_current_realtime = 0.

_energy_calibration = 2.2 # pe/keV
_xenon_density = 2.86 # g/cm^3

_aliases_defined = 0
_positions_from = 'fann'

