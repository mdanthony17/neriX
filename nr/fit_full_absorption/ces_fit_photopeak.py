import ROOT as root
import neriX_datasets
from neriX_analysis import neriX_analysis
import sys, random, string, pickle, time
from array import array
from rootpy.io import File
from rootpy.tree import Tree, TreeModel
from rootpy.plotting import Hist, Hist2D, Canvas, Legend
from rootpy.plotting.utils import draw
import numpy as np


def set_ellipse_parameters(dHistParameters, numPeaks, dPeakCenterGuesses):
	p1S1 = dPeakCenterGuesses['p1s1']
	p1S2 = dPeakCenterGuesses['p1s2']
	if numPeaks == 2 or numPeaks == 3:
		p2S1 = dPeakCenterGuesses['p2s1']
		p2S2 = dPeakCenterGuesses['p2s2']
	if numPeaks == 3:
		p3S1 = dPeakCenterGuesses['p3s1']
		p3S2 = dPeakCenterGuesses['p3s2']
	
	# for all top or bottom
	if numPeaks == 1:
		return  {0:[10, 0, 1e4],
			 1:[p1S1, dHistParameters['S1LB'], dHistParameters['S1UB']],
			 2:[0., 0., 0.],
			 3:[p1S2, dHistParameters['S2LB'], dHistParameters['S2UB']],
			 4:[35, 5, 400],
			 5:[2e4, 0.3e4, 7e4]}
	elif numPeaks == 2:
		return  {0:[10, 0, 1e4],
			 1:[p1S1, dHistParameters['S1LB'], dHistParameters['S1UB']],
			 #2:[3.14/4., 0., 3.14/2.],
			 2:[0., 0., 0.],
			 3:[p1S2, dHistParameters['S2LB'], dHistParameters['S2UB']],
			 4:[150, 5, 650],
			 5:[2e4, 5, 15e4],
			 6:[1, 0, 2],
			 7:[p2S1, dHistParameters['S1LB'], dHistParameters['S1UB']],
			 8:[p2S2, dHistParameters['S2LB'], dHistParameters['S2UB']]}
	elif numPeaks == 3:
		return  {0:[10, 0, 1e4],
			 1:[p1S1, dHistParameters['S1LB'], dHistParameters['S1UB']],
			 #2:[3.14/4., 0., 3.14/2.],
			 2:[0., 0., 0.],
			 3:[p1S2, dHistParameters['S2LB'], dHistParameters['S2UB']],
			 4:[150, 5, 650],
			 5:[2e4, 5, 15e4],
			 6:[1, 0, 2],
			 7:[p2S1, dHistParameters['S1LB'], dHistParameters['S1UB']],
			 8:[p2S2, dHistParameters['S2LB'], dHistParameters['S2UB']],
			 9:[1, 0, 2],
			 10:[p3S1, dHistParameters['S1LB'], dHistParameters['S1UB']],
			 11:[p3S2, dHistParameters['S2LB'], dHistParameters['S2UB']]}
	


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


def fit_photopeak(lFilesToLoad, radialCut, numProcessors, numPeaks=1):

	# ---------- ERROR CHECKING ----------

	currentAnalysis = neriX_analysis(fileToLoad)
	#currentAnalysis = neriX_analysis(['ct_nerix_141219_1913.root', 'ct_nerix_141220_0956.root', 'ct_nerix_141222_1016.root', 'ct_nerix_141222_1450.root'], 0, 1.054, 4.5)
	#currentAnalysis = neriX_analysis(lFilesToLoad, 0, 1.054, 4.5)

	#currentAnalysis.add_dt_cut(2, 13)
	currentAnalysis.add_z_cut()
	currentAnalysis.add_single_scatter_cut()
	currentAnalysis.add_radius_cut(0, radialCut)
	#currentAnalysis.add_cut('( (Alt$(ratio_s2tot1_s2top0,0.) < 0.06 || (((Alt$(Z,3.)>2.0 && Alt$(Z,3.)<5.0) || Alt$(Z,30.)>24.5) && (Alt$(ratio_s2tot1_s2top0,0.) < 0.1))) )')
	currentAnalysis.add_cut('(s1asym > 0)')
	#currentAnalysis.add_cut('(0.0137*(S1sTotBottom[0]/0.1 + S2sTotBottom[0]/24.) > 150 && 0.0137*(S1sTotBottom[0]/0.1 + S2sTotBottom[0]/24.) < 200.)')
	#currentAnalysis.add_s1_asym_cut()
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



	hOption = 'colz'
	
	numBinsCES = 50

	degree = currentAnalysis.get_degree_setting()
	if degree > -1:
		S1NB = 25
		S1LB = 0
		S1UB = 400#2500

		S2NB = 25
		S2LB = 0
		S2UB = 50e3#8.0e5

		firstParameter = 'cpS1sTotBottom[0]'
		secondParameter = 'cpS2sTotBottom[0]'
	
		upperBoundCES = 100.

	elif degree == -2 or degree == -4:
		S1NB = 25
		S1LB = 0
		S1UB = 1400

		S2NB = 25
		S2LB = 0
		S2UB = 400e3
		
		firstParameter = 'cpS1sTotBottom[0]'
		secondParameter = 'cpS2sTotBottom[0]'
	
		upperBoundCES = 400.

	elif degree == -6:
		S1NB = 50
		S1LB = 0
		S1UB = 1400

		S2NB = 50
		S2LB = 0
		S2UB = 400e3
		
		firstParameter = 'cpS1sTotBottom[0]'
		secondParameter = 'cpS2sTotBottom[0]'
	
		upperBoundCES = 400.

		# 164 and 236 keV
		#numPeaks = 2


	elif (degree == -1 or degree == -5) and currentAnalysis.get_run() == 15:
		S1NB = 80
		S1LB = 0
		S1UB = 3500

		S2NB = 80
		S2LB = 0
		S2UB = 700e3
		
		upperBoundCES = 800.
	
		firstParameter = 'cpS1sTotBottom[0]'
		secondParameter = 'cpS2sTotBottom[0]'
	
	if (currentAnalysis.get_run() == 10 or currentAnalysis.get_run() == 11):
		S1NB = 60
		S1LB = 0
		S1UB = 3500

		S2NB = 60
		S2LB = 0
		S2UB = 800e3
		
		upperBoundCES = 800.
	
		firstParameter = 'ctS1sTotBottom[0]'
		secondParameter = 'S2sTotTop[0]'

	elif currentAnalysis.get_run() == 15:
		pass
	else:
		print 'Only setup to handle coincidence and minitron and Co57 calibrations (degree = -4)'
		sys.exit()

	dHistParameters = {'S1NB':S1NB,
					   'S1LB':S1LB,
					   'S1UB':S1UB,
					   'S2NB':S2NB,
					   'S2LB':S2LB,
					   'S2UB':S2UB}


	#dEllipse =  set_ellipse_parameters(dHistParameters, numPeaks)


	responseSatisfied = ''
	fitStatus = 0

	sHistParameters = ''
	for key in dHistParameters:
		sHistParameters += key + '\t'

	if numPeaks == 1:
		sEllipse = '[0]*exp( -0.5 * (pow( (( (x-[1])*cos( [2] ) - (y-[3])*sin( [2] ) ) /([4])) , 2. )+pow(  ((x-[1])*sin( [2] ) + (y-[3])*cos( [2] )) /([5]) , 2.)) ) '
	elif numPeaks == 2:
		sEllipse = '[0]*exp( -0.5 * (pow( (( (x-[1])*cos( [2] ) - (y-[3])*sin( [2] ) ) /([4])) , 2. )+pow(  ((x-[1])*sin( [2] ) + (y-[3])*cos( [2] )) /([5]) , 2.)) )  +  [6]*[0]*exp( -0.5 * (pow( (( (x-[7])*cos( [2] ) - (y-[8])*sin( [2] ) ) /([4])) , 2. )+pow(  ((x-[7])*sin( [2] ) + (y-[8])*cos( [2] )) /([5]) , 2.)) ) + [9]'
	elif numPeaks == 3:
		sEllipse = '[0]*exp( -0.5 * (pow( (( (x-[1])*cos( [2] ) - (y-[3])*sin( [2] ) ) /([4])) , 2. )+pow(  ((x-[1])*sin( [2] ) + (y-[3])*cos( [2] )) /([5]) , 2.)) )  +  [6]*[0]*exp( -0.5 * (pow( (( (x-[7])*cos( [2] ) - (y-[8])*sin( [2] ) ) /([4])) , 2. )+pow(  ((x-[7])*sin( [2] ) + (y-[8])*cos( [2] )) /([5]) , 2.)) )+  [9]*[0]*exp( -0.5 * (pow( (( (x-[10])*cos( [2] ) - (y-[11])*sin( [2] ) ) /([4])) , 2. )+pow(  ((x-[10])*sin( [2] ) + (y-[11])*cos( [2] )) /([5]) , 2.)) ) + [12]'




	# ---------- END CONSTANTS FOR FITS AND HISTOGRAMS ----------



	# ---------- CHECK PICKLE FILE FOR OLD PARAMETERS ----------

	FILENAME = currentAnalysis.get_filename()
	cuts = currentAnalysis.get_cuts()

	print '\n\n%s\n\n' % currentAnalysis.get_cuts()

	# load old parameters if possible
	useOldParameters = False
	try:
		dCompletedFitsHistParameters[FILENAME]
		responseOldFit = raw_input('Would you like to reuse values from before.  Type "y" if so, otherwise press enter: ')
		if string.lower(responseOldFit) == 'y':
			dHistParameters = dCompletedFitsHistParameters[FILENAME]
			dEllipse = set_ellipse_parameters(dHistParameters, numPeaks)
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
	


	# ---------- MAKE CES CUT ----------

	g1 = 0.095 #0.105 #0.1
	g2 = 17. #23. #24.
	sigmaCES = 1.

	cCES = Canvas(width=900, height=700, name='cCES')
	cCES.SetGridx()
	cCES.SetGridy()

	hCES = Hist(numBinsCES, 0, upperBoundCES, name='hCES', title='Combined Energy Spectrum', drawstyle='hist')
	currentAnalysis.Draw('0.0137*(%s/%f + %s/%f)' % (firstParameter, g1, secondParameter, g2), hist=hCES)
	hCES.GetXaxis().SetTitle('CE(g1=%.4f, g2=%.2f)' % (g1, g2))
	hCES.GetYaxis().SetTitle('Counts')
	hCES.SetStats(0)

	hCES.Draw()
	cCES.Update()

	lbCES = float(raw_input('\n\nPlease enter lower bound on combined energy scale: '))
	ubCES = float(raw_input('\n\nPlease enter upper bound on combined energy scale: '))


	fGausCES = root.TF1('fGausCES', 'gaus', lbCES, ubCES)
	#fGausCES = root.TF1('fGausCES', '[0]*exp(-[1]*x) + [2]*exp(-0.5*pow((x-[3])/[4], 2))', lbCES, ubCES)
	#fGausCES.SetParameters(500, 0.01, 500, 650, 50)
	hCES.Fit(fGausCES, 'LR')
	fGausCES.Draw('same')
	cCES.Update()

	if currentAnalysis.get_run() == 10 or currentAnalysis.get_run() == 11:
		print 'Using run 10/11 slices in CES'
		lbCES = fGausCES.GetParameter(1) - 1.5*fGausCES.GetParameter(2)
		ubCES = fGausCES.GetParameter(1) + 1.5*fGausCES.GetParameter(2)
	else:
		lbCES = fGausCES.GetParameter(1) - 1.0*fGausCES.GetParameter(2)
		ubCES = fGausCES.GetParameter(1) + 0.25*fGausCES.GetParameter(2)
	raw_input('Fit okay?')
	

	#print '\nBypassing CES cut!\n'
	bypassCES = False

	if bypassCES:
		sCES = '1 > 0'
	else:
		sCES = '(0.0137*(%s/%f + %s/%f) > %f && 0.0137*(%s/%f + %s/%f) < %f)' % (firstParameter, g1, secondParameter, g2, lbCES, firstParameter, g1, secondParameter, g2, ubCES)
	currentAnalysis.add_cut(sCES)

	del cCES



	# ---------- END MAKE CES CUT ----------


	# ---------- MAIN LOOP ----------

	#currentAnalysis.get_T1().Scan('S2sTotBottom[0]:S2sTotTop[0]', currentAnalysis.get_cuts())

	
	c1 = Canvas(width=900, height=700, name='c1')
	c1.SetLogz()
	c1.SetGridx()
	c1.SetGridy()
	
	# initialize fit range and center guesses
	dFitRange = {}
	dPeakCenterGuesses = {}
	
	bandCutPlaced = False

	while string.lower(responseSatisfied) != 'y' or fitStatus != 3:
		print 'Remain in loop until fit converges and user is satisfied with fit.'

		h1 = Hist2D(dHistParameters['S1NB'], dHistParameters['S1LB'], dHistParameters['S1UB'], dHistParameters['S2NB'], dHistParameters['S2LB'], dHistParameters['S2UB'], name='h1', title='S2 vs. S1')
		h1.GetXaxis().SetTitle('S1 [PE]')
		h1.GetYaxis().SetTitle('S2 [PE]')
		h1.SetStats(0)

		print '\n\nSecond Parameter being used: ' + secondParameter + '\n\n'
		currentAnalysis.multithread_draw(numProcessors, firstParameter + ':' + secondParameter, hist=h1, selection=currentAnalysis.get_cuts())
		
		
		# use TSpectrum to search for peaks
		spec = root.TSpectrum2(50, 0.01)
		spec.Search(h1)
		bX = spec.GetPositionX()
		bX.SetSize(2)
		aX = np.array(bX, 'f')
		bY = spec.GetPositionY()
		bY.SetSize(2)
		aY = np.array(bY, 'f')
		print aX, aY
		
		h1.Draw(hOption)
		
		response = raw_input('\n\nWould you like to manually set fit range?  Type "y" if so, otherwise press enter.\n\n')
		if response != 'y':
			dFitRange['S1LB'] = dHistParameters['S1LB']
			dFitRange['S1UB'] = dHistParameters['S1UB']
			dFitRange['S2LB'] = dHistParameters['S2LB']
			dFitRange['S2UB'] = dHistParameters['S2UB']
		else:
			while True:
				print 'Current settings: '
				print dFitRange
				responseChanges = string.upper(raw_input('Parameter to change or "c" to continue: '))
				if responseChanges == 'C':
					break
				try:
					dFitRange[responseChanges]
					value = float(raw_input('Please enter value: '))
					dFitRange[responseChanges] = float(value)
				except:
					print 'Incorrect parameter or type of input, please try again.'
					continue


		response = raw_input('\n\nWould you like to manually set peak center guesses?  Type "y" if so, otherwise press enter.\n\n')
		if response != 'y':
			if numPeaks == 1:
				dPeakCenterGuesses['p1s1'] = (dHistParameters['S1LB']+dHistParameters['S1UB'])/2.
				dPeakCenterGuesses['p1s2'] = (dHistParameters['S2LB']+dHistParameters['S2UB'])/2.
			else:
				dPeakCenterGuesses['p1s1'] = (dHistParameters['S1LB']+dHistParameters['S1UB'])*2./10.
				dPeakCenterGuesses['p1s2'] = (dHistParameters['S2LB']+dHistParameters['S2UB'])*2./10.
				dPeakCenterGuesses['p2s1'] = (dHistParameters['S1LB']+dHistParameters['S1UB'])*4./10.
				dPeakCenterGuesses['p2s2'] = (dHistParameters['S2LB']+dHistParameters['S2UB'])*4./10.
				dPeakCenterGuesses['p3s1'] = (dHistParameters['S1LB']+dHistParameters['S1UB'])*6./10.
				dPeakCenterGuesses['p3s2'] = (dHistParameters['S2LB']+dHistParameters['S2UB'])*6./10.

		else:
			while True:
				print 'Current settings: '
				print dPeakCenterGuesses
				responseChanges = raw_input('Parameter to change or "c" to continue: ')
				if responseChanges == 'c':
					break
				try:
					dPeakCenterGuesses[responseChanges]
					value = float(raw_input('Please enter value: '))
					dPeakCenterGuesses[responseChanges] = float(value)
				except:
					print 'Incorrect parameter or type of input, please try again.'
					continue


		if not bandCutPlaced:
			response = raw_input('\n\nWould you like to add a band cut?  Type "y" if so, otherwise press enter.\n\n')
			if response == 'y':
				print '\n\nWill choose three points: two for the lower line and then one for the upper line.  The upper line will use the slope of the first line.\n\n'
				x1 = float(raw_input('Please choose x1: '))
				y1 = float(raw_input('Please choose y1: '))
				x2 = float(raw_input('Please choose x2: '))
				y2 = float(raw_input('Please choose y2: '))
				x3 = float(raw_input('Please choose x3: '))
				y3 = float(raw_input('Please choose y3: '))

				slope = (y2 - y1) / (x2 - x1)
				interceptLower = y2 - slope*x2
				interceptUpper = y3 - slope*x3

				sBandCut = '(%s > (%s*%d + %d)) && (%s < (%s*%d + %d))' % (secondParameter, firstParameter, slope, interceptLower, secondParameter, firstParameter, slope, interceptUpper)

				currentAnalysis.add_cut(sBandCut)
				bandCutPlaced = True




		# update ellipse parameters
		dEllipse =  set_ellipse_parameters(dFitRange, numPeaks, dPeakCenterGuesses)
		

		f1 = root.TF2('f1', sEllipse, dFitRange['S1LB'], dFitRange['S1UB'], dFitRange['S2LB'], dFitRange['S2UB'])
		for key in dEllipse:
			if key == 2:
				#print 'Trying angle'
				continue
			elif key == 1:
				f1.SetParameter(key, aX[0])
				f1.SetParLimits(key, aX[0]*0.85, aX[0]*1.15)
				print 'Must finish at line 251!!!!'
			else:
				f1.SetParameter(key, dEllipse[key][0])
				f1.SetParLimits(key, dEllipse[key][1], dEllipse[key][2])
		
		
		fitResult = h1.Fit('f1', 'NSR')

		
		#aContours = array('d', [0.1, 0.3, 0.5])
		#f1.SetContour(3, aContours)
		f1.SetContour(5)
		f1.SetLineStyle(1)
		f1.SetLineWidth(2)
		f1.SetLineColor(root.kBlack)
		f1.Draw('same')
		
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
	if len(sys.argv) != 5:
		print 'Use is python set_photopeak_fit_info.py <ROOT file> <radial cut> <number of processors> <num peaks to fit>'
		sys.exit()

	fileToLoad = sys.argv[1]
	radialCut = float(sys.argv[2])
	numProcessors = int(sys.argv[3])
	numPeaks = int(sys.argv[4])

	fit_photopeak(fileToLoad, radialCut, numProcessors, numPeaks)
