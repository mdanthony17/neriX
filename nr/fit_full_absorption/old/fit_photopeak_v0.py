import ROOT as root
import neriX_datasets
from neriX_analysis import neriX_analysis
import sys, random, string, pickle, time
from array import array
from rootpy.io import File
from rootpy.tree import Tree, TreeModel
from rootpy.plotting import Hist, Hist2D, Canvas, Legend
from rootpy.plotting.utils import draw


def set_ellipse_parameters(dHistParameters):
	# for all top or bottom
	"""
	return  {0:[10, 0, 1e4],
			 1:[(dHistParameters['S1LB']+dHistParameters['S1UB'])/2., dHistParameters['S1LB'], dHistParameters['S1UB']],
			 2:[0., 0., 0.],
			 3:[(dHistParameters['S2LB']+dHistParameters['S2UB'])/2., dHistParameters['S2LB'], dHistParameters['S2UB']],
			 4:[35, 5, 95],
			 5:[2e4, 0.3e4, 7e4]}
	"""
	return  {0:[10, 0, 1e4],
			 1:[(dHistParameters['S1LB']+dHistParameters['S1UB'])/2., dHistParameters['S1LB'], dHistParameters['S1UB']],
			 #2:[3.14/4., 0., 3.14/2.],
			 2:[0., 0., 0.],
			 3:[(dHistParameters['S2LB']+dHistParameters['S2UB'])/2., dHistParameters['S2LB'], dHistParameters['S2UB']],
			 4:[150, 5, 650],
			 5:[2e4, 5, 9e4]}


def convert_name_to_unix_time(fileName):
	if fileName[-5:] == '.root':
		fileName = fileName[:-5]
	
	# check that we know position of dates in name
	if fileName[0:5] == 'nerix':
		sTimeTaken = '20' + fileName[6:17] + '00'
		return int(time.mktime(time.strptime(sTimeTaken, '%Y%m%d_%H%M%S')))
	elif fileName[0:8] == 'ct_nerix':
		sTimeTaken = '20' + fileName[9:20] + '00'
		return int(time.mktime(time.strptime(sTimeTaken, '%Y%m%d_%H%M%S')))
	else:
		print 'Only know how to handle nerix and ct_nerix files currently'
		print 'Please add to convert_name_to_unix_time function in order to handle the new case'
		sys.exit()


def fit_photopeak(lFilesToLoad, radialCut, numProcessors):

	# ---------- ERROR CHECKING ----------

	currentAnalysis = neriX_analysis(fileToLoad)
	#currentAnalysis = neriX_analysis(['ct_nerix_141219_1913.root', 'ct_nerix_141220_0956.root', 'ct_nerix_141222_1016.root', 'ct_nerix_141222_1450.root'], 0, 1.054, 4.5)
	#currentAnalysis = neriX_analysis(lFilesToLoad, 0, 1.054, 4.5)

	currentAnalysis.add_dt_cut()
	currentAnalysis.add_single_scatter_cut()
	currentAnalysis.add_radius_cut(0, radialCut)
	# Marc's old cut used R_old < 21
	#currentAnalysis.add_cut('R_old < ' + str(radialCut))

	# try to load pickle file that has saved histogram parameters
	try:
		dCompletedFitsHistParameters = pickle.load(open('completed_fits_hist_parameters.p', 'r'))
	except:
		print 'Pickled file of old parameters does not exist yet'
		dCompletedFitsHistParameters = {}


	# ---------- END ERROR CHECKING ----------




	# ---------- CONSTANTS FOR FITS AND HISTOGRAMS ----------

	sEllipse = '[0]*exp( -0.5 * (pow( (( (x-[1])*cos( [2] ) - (y-[3])*sin( [2] ) ) /([4])) , 2. )+pow(  ((x-[1])*sin( [2] ) + (y-[3])*cos( [2] )) /([5]) , 2.)) ) '

	hOption = 'colz'


	degree = currentAnalysis.get_degree_setting()
	if degree > -1:
		S1NB = 25
		S1LB = 0
		S1UB = 2500

		S2NB = 25
		S2LB = 0
		S2UB = 8.0e5

	elif degree == -2 or degree == -4:
		S1NB = 25
		S1LB = 0
		S1UB = 1400

		S2NB = 25
		S2LB = 0
		S2UB = 400e3
		
		#print 'NOTE: lowered for 3kV anode'
	
		secondParameter = 'S2sTotBottom[0]'

	else:
		print 'Only setup to handle coincidence and minitron and Co57 calibrations (degree = -4)'
		sys.exit()

	dHistParameters = {'S1NB':S1NB,
					   'S1LB':S1LB,
					   'S1UB':S1UB,
					   'S2NB':S2NB,
					   'S2LB':S2LB,
					   'S2UB':S2UB}


	dEllipse =  set_ellipse_parameters(dHistParameters)


	responseSatisfied = ''
	fitStatus = 0

	sHistParameters = ''
	for key in dHistParameters:
		sHistParameters += key + '\t'

	# ---------- END CONSTANTS FOR FITS AND HISTOGRAMS ----------



	# ---------- CHECK PICKLE FILE FOR OLD PARAMETERS ----------

	FILENAME = currentAnalysis.get_filename()
	cuts = currentAnalysis.get_cuts()

	# load old parameters if possible
	useOldParameters = False
	try:
		dCompletedFitsHistParameters[FILENAME]
		responseOldFit = raw_input('Would you like to reuse values from before.  Type "y" if so, otherwise press enter: ')
		if string.lower(responseOldFit) == 'y':
			dHistParameters = dCompletedFitsHistParameters[FILENAME]
			dEllipse = set_ellipse_parameters(dHistParameters)
			useOldParameters = True
	except:
		try:
			dCompletedFitsHistParameters[FILENAME]
		except:
			dCompletedFitsHistParameters[FILENAME] = {}




	# ---------- END CHECK PICKLE FILE FOR OLD PARAMETERS ----------



	# ---------- SET EVENT LIST ----------

	#currentAnalysis.set_event_list()
	currentAnalysis.multithread_set_event_list(numProcessors)

	# ---------- END SET EVENT LIST ----------



	# ---------- MAIN LOOP ----------

	
	c1 = Canvas(width=900, height=700, name='c1')
	c1.SetLogz()
	c1.SetGridx()
	c1.SetGridy()

	while string.lower(responseSatisfied) != 'y' or fitStatus != 3:
		print 'Remain in loop until fit converges and user is satisfied with fit.'

		h1 = Hist2D(dHistParameters['S1NB'], dHistParameters['S1LB'], dHistParameters['S1UB'], dHistParameters['S2NB'], dHistParameters['S2LB'], dHistParameters['S2UB'], name='h1', title='S2 vs. S1')
		h1.GetXaxis().SetTitle('S1 [PE]')
		h1.GetYaxis().SetTitle('S2 [PE]')
		h1.SetStats(0)

		print '\n\nSecond Parameter being used: ' + secondParameter + '\n\n'
		currentAnalysis.multithread_draw(numProcessors, 'S1sTotBottom[0]:' + secondParameter, hist=h1, selection='')

		f1 = root.TF2('f1', sEllipse, dHistParameters['S1LB'], dHistParameters['S1UB'], dHistParameters['S2LB'], dHistParameters['S2UB'])
		for key in dEllipse:
			if key == 2:
				#print 'Trying angle'
				continue
			f1.SetParameter(key, dEllipse[key][0])
			f1.SetParLimits(key, dEllipse[key][1], dEllipse[key][2])
		

		fitResult = h1.Fit('f1', 'NS')
		h1.Draw(hOption)
		
		#aContours = array('d', [0.1, 0.3, 0.5])
		#f1.SetContour(3, aContours)
		f1.SetContour(5)
		f1.SetLineStyle(1)
		f1.SetLineWidth(2)
		f1.SetLineColor(root.kBlack)
		#f1.Draw('same')
		
		c1.Update()

		fitStatus = fitResult.CovMatrixStatus()
		if fitStatus != 3:
			print 'Fit failed!!!'
			print 'You may change one or more of the following parameters to try and improve the fit.'
			print 'When finished changing parameters, type "continue" to exit loop and see result.'
			responseChanges = ''
			while responseChanges != 'continue':
				print sHistParameters + '\n'
				print 'Current settings: '
				print dHistParameters
				responseChanges = string.upper(raw_input('Parameter to change or "c" to continue: '))
				if responseChanges == 'C':
					del f1
					del h1
					break
				try:
					dHistParameters[responseChanges]
					value = float(raw_input('Please enter value: '))
					if responseChanges == 'S1NB' or responseChanges == 'S2NB':
						dHistParameters[responseChanges] = int(value)
					else:
						dHistParameters[responseChanges] = float(value)
				except:
					print 'Incorrect parameter or type of input, please try again.'
					continue
		else:
			print 'Fit completed successfully!'
			print 'If you are satisfied with the fit please enter "y".  If not, please enter anything else.'
			responseSatisfied = string.lower(raw_input('Satisfied: '))
			if responseSatisfied == 'y':
				break
			else:
				responseChanges = ''
				while responseChanges != 'continue':
					print sHistParameters + '\n'
					print 'Current settings: '
					print dHistParameters
					responseChanges = string.upper(raw_input('Parameter to change or "c" to continue: '))
					if responseChanges == 'C':
						del f1
						del h1
						break
					try:
						dHistParameters[responseChanges]
						value = float(raw_input('Please enter value: '))
						if responseChanges == 'S1NB' or responseChanges == 'S2NB':
							dHistParameters[responseChanges] = int(value)
						else:
							dHistParameters[responseChanges] = float(value)
					except:
						print 'Incorrect parameter or type of input, please try again.'
						continue


		# update ellipse parameters
		dEllipse =  set_ellipse_parameters(dHistParameters)


	print 'Chi^2 / NDF = ', fitResult.Chi2(), ' / ', fitResult.Ndf()
	raw_input('Fit was successful! Press enter to exit.')

	startTime = convert_name_to_unix_time(FILENAME)

	dParametersToPrint = {'start_time':startTime, 'run':currentAnalysis.get_run(), 'anode':currentAnalysis.get_anode_setting(), 'cathode':currentAnalysis.get_cathode_setting(), 'degree':currentAnalysis.get_degree_setting(), 's1_mean':f1.GetParameter(1), 's1_mean_err':f1.GetParError(1), 's1_sigma':f1.GetParameter(4), 's1_sigma_err':f1.GetParError(4), 's2_mean':f1.GetParameter(3), 's2_mean_err':f1.GetParError(3), 's2_sigma':f1.GetParameter(5), 's2_sigma_err':f1.GetParError(5), 'Chi2':fitResult.Chi2(), 'NDF':fitResult.Ndf()}

	print '\n\nPlease copy below into fits_completed.py\n'
	print '#' + str(FILENAME)
	print 'dFitParameters[\'' + str(FILENAME) + '\'] =', dParametersToPrint
	print '\n\n'

	dCompletedFitsHistParameters[FILENAME] = dHistParameters
	pickle.dump(dCompletedFitsHistParameters, open('completed_fits_hist_parameters.p', 'w'))

	return dParametersToPrint


if __name__ == '__main__':
	if len(sys.argv) != 4:
		print 'Use is python set_photopeak_fit_info.py <ROOT file> <radial cut> <number of processors>'
		sys.exit()

	fileToLoad = sys.argv[1]
	radialCut = float(sys.argv[2])
	numProcessors = int(sys.argv[3])

	fit_photopeak(fileToLoad, radialCut, numProcessors)
