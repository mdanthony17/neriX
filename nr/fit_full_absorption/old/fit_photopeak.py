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
import matplotlib.pyplot as plt
from rootpy.plotting import root2matplotlib as rplt


def set_ellipse_parameters(dHistParameters, numPeaks):
	# for all top or bottom
	if numPeaks == 1:
		return  {0:[10, 0, 1e4],
			 1:[(dHistParameters['S1LB']+dHistParameters['S1UB'])/2., dHistParameters['S1LB'], dHistParameters['S1UB']],
			 2:[0., 0., 0.],
			 3:[(dHistParameters['S2LB']+dHistParameters['S2UB'])/2., dHistParameters['S2LB'], dHistParameters['S2UB']],
			 4:[35, 5, 95],
			 5:[2e4, 0.3e4, 7e4]}
	elif numPeaks == 2:
		return  {0:[10, 0, 1e4],
			 1:[(dHistParameters['S1LB']+dHistParameters['S1UB'])*2./10., dHistParameters['S1LB'], dHistParameters['S1UB']],
			 #2:[3.14/4., 0., 3.14/2.],
			 2:[0., 0., 0.],
			 3:[(dHistParameters['S2LB']+dHistParameters['S2UB'])*2./10., dHistParameters['S2LB'], dHistParameters['S2UB']],
			 4:[150, 5, 650],
			 5:[2e4, 5, 15e4],
			 6:[1, 0, 2],
			 7:[(dHistParameters['S1LB']+dHistParameters['S1UB'])*4./10., dHistParameters['S1LB'], dHistParameters['S1UB']],
			 8:[(dHistParameters['S2LB']+dHistParameters['S2UB'])*4./10., dHistParameters['S2LB'], dHistParameters['S2UB']]}
	


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

	currentAnalysis.add_z_cut()
	currentAnalysis.add_single_scatter_cut()
	currentAnalysis.add_radius_cut(0, radialCut)
	currentAnalysis.add_s1_asym_cut()
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

	numPeaks = 1

	degree = currentAnalysis.get_degree_setting()
	if degree > -1:
		S1NB = 25
		S1LB = 0
		S1UB = 400#2500

		S2NB = 25
		S2LB = 0
		S2UB = 50e3#8.0e5

		secondParameter = 'S2sTotBottom[0]'

	elif degree == -2 or degree == -4:
		S1NB = 25
		S1LB = 0
		S1UB = 1400

		S2NB = 25
		S2LB = 0
		S2UB = 400e3
		
		secondParameter = 'S2sTotBottom[0]'

	elif degree == -6:
		S1NB = 25
		S1LB = 0
		S1UB = 800

		S2NB = 25
		S2LB = 0
		S2UB = 100e3
		
		secondParameter = 'S2sTotBottom[0]'

		# 164 and 236 keV
		numPeaks = 2


	elif degree == -1:
		S1NB = 25
		S1LB = 0
		S1UB = 3500

		S2NB = 25
		S2LB = 0
		S2UB = 1100e3
		
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


	dEllipse =  set_ellipse_parameters(dHistParameters, numPeaks)


	responseSatisfied = ''
	fitStatus = 0

	sHistParameters = ''
	for key in dHistParameters:
		sHistParameters += key + '\t'

	if numPeaks == 1:
		def elliptical_gaussian((x,y), amplitude, xo, yo, sigma_x, sigma_y, theta, offset):
			xo = float(xo)
			yo = float(yo)    
			a = (np.cos(theta)**2)/(2*sigma_x**2) + (np.sin(theta)**2)/(2*sigma_y**2)
			b = -(np.sin(2*theta))/(4*sigma_x**2) + (np.sin(2*theta))/(4*sigma_y**2)
			c = (np.sin(theta)**2)/(2*sigma_x**2) + (np.cos(theta)**2)/(2*sigma_y**2)
			return offset + amplitude*np.exp( - (a*((x-xo)**2) + 2*b*(x-xo)*(y-yo) + c*((y-yo)**2)))
	elif numPeaks == 2:
		print 'need to implement'
		sys.exit()




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



	# ---------- MAIN LOOP ----------

	def onClick(event):
		print 'button=%d, x=%d, y=%d, xdata=%f, ydata=%f' % (event.button, event.x, event.y, event.xdata, event.ydata)
		print event.dblclick
		print 'Please press "l" or "u" for lower or upper corners, respectively.'
		plt.waitforbuttonpress()
		#response = raw_input('Please press "l" or "u" for lower or upper corners, respectively: ')
		#print response
		print 'after wait'
	
	def onKeyPress(event):
		print 'Pressed: ' + event.key
		if event.key == 'l':
			print event.xdata
			#dHistParameters['S1LB'] = event.xdata

	# create figure
	fig1, ax1 = plt.subplots()
	cid = fig1.canvas.mpl_connect('button_press_event', onClick)
	cid = fig1.canvas.mpl_connect('key_press_event', onKeyPress)
	# use plt.draw() to update plot

	print 'Remain in loop until fit converges and user is satisfied with fit.'

	h1 = Hist2D(dHistParameters['S1NB'], dHistParameters['S1LB'], dHistParameters['S1UB'], dHistParameters['S2NB'], dHistParameters['S2LB'], dHistParameters['S2UB'], name='h1', title='S2 vs. S1')
	h1.GetXaxis().SetTitle('S1 [PE]')
	h1.GetYaxis().SetTitle('S2 [PE]')
	h1.SetStats(0)

	print '\n\nSecond Parameter being used: ' + secondParameter + '\n\n'
	currentAnalysis.multithread_draw(numProcessors, 'S1sTotBottom[0]:' + secondParameter, hist=h1, selection='')
	rplt.hist2d(h1, axes=ax1)

	im = rplt.imshow(h1, axes=ax1)
	fig1.colorbar(im)
	plt.show()

		

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
