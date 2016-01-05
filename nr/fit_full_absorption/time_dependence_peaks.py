import ROOT as root
import neriX_datasets
from neriX_analysis import neriX_analysis
import sys, random, string, pickle, time
from array import array
from rootpy.io import File
from rootpy.tree import Tree, TreeModel
from rootpy.plotting import Hist, Hist2D, Canvas, Legend
from rootpy.plotting.utils import draw




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
	
	currentAnalysis.add_dt_cut()
	currentAnalysis.add_single_scatter_cut()
	currentAnalysis.add_radius_cut(0, radialCut)


	# ---------- END ERROR CHECKING ----------




	# ---------- CONSTANTS FOR FITS AND HISTOGRAMS ----------

	degree = currentAnalysis.get_degree_setting()
	if degree > -1:
		S1NB = 75
		S1LB = 0
		S1UB = 2500

		S2NB = 75
		S2LB = 0
		S2UB = 8.0e5

	elif degree == -2 or degree == -4:
		S1NB = 75
		S1LB = 0
		S1UB = 1400

		S2NB = 75
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


	sHistParameters = ''
	for key in dHistParameters:
		sHistParameters += key + '\t'

	# ---------- END CONSTANTS FOR FITS AND HISTOGRAMS ----------



	# ---------- SET EVENT LIST ----------

	#currentAnalysis.set_event_list()
	currentAnalysis.multithread_set_event_list(numProcessors)

	# ---------- END SET EVENT LIST ----------



	# ---------- MAIN LOOP ----------

	
	c1 = Canvas(width=900, height=700, name='c1')
	c1.SetLogz()
	c1.SetGridx()
	c1.SetGridy()
	
	
	h1 = Hist2D(dHistParameters['S1NB'], dHistParameters['S1LB'], dHistParameters['S1UB'], dHistParameters['S2NB'], dHistParameters['S2LB'], dHistParameters['S2UB'], name='h1', title='S2 vs. S1')
	h1.GetXaxis().SetTitle('S1 [PE]')
	h1.GetYaxis().SetTitle('S2 [PE]')
	h1.SetStats(0)

	print '\n\nSecond Parameter being used: ' + secondParameter + '\n\n'
	currentAnalysis.multithread_draw(numProcessors, 'S1sTotBottom[0]:' + secondParameter, hist=h1, selection='')

	h1.Draw('colz')

	c1.Update()
	
	print '\n\nSelect box in S1 and S2 for peak you would like to examine time dependence of.\n\n'

	s1lb = float(raw_input('\n\nPlease select s1lb: '))
	s1ub = float(raw_input('\n\nPlease select s1ub: '))
	s2lb = float(raw_input('\n\nPlease select s2lb: '))
	s2ub = float(raw_input('\n\nPlease select s2ub: '))

	print '\n\n'

	peakCut = '( (S1sTotBottom[0] > %f) && (S1sTotBottom[0] < %f) ) && ( (S2sTotBottom[0] > %f) && (S2sTotBottom[0] < %f) )' % (s1lb, s1ub, s2lb, s2ub)

	hTimeDependence = Hist(20, 0, currentAnalysis.get_livetime())
	hTimeDependence.GetXaxis().SetTitle('Seconds from beginning of dataset')
	hTimeDependence.GetYaxis().SetTitle('Counts for selected peak')
	hTimeDependence.SetStats(0)

	currentAnalysis.multithread_draw(numProcessors, 'TimeSec - ' + str(currentAnalysis.get_timestamp()), hist=hTimeDependence, selection=peakCut)

	c2 = Canvas()
	c2.SetLogy()
	hTimeDependence.Draw()
	

	fExponential = root.TF1('fExponential', 'expo', 0, currentAnalysis.get_livetime())
	hTimeDependence.Fit('fExponential')
	fExponential.Draw('same')
	
	print 'Estimated half-life: ' + str(-1./fExponential.GetParameter(1)/3600./24.) + ' +/- ' + str(1./fExponential.GetParError(1)/3600./24.)


	c2.Update()



	raw_input('Press enter to continue...')



if __name__ == '__main__':
	if len(sys.argv) != 4:
		print 'Use is python set_photopeak_fit_info.py <ROOT file> <radial cut> <number of processors>'
		sys.exit()

	fileToLoad = sys.argv[1]
	radialCut = float(sys.argv[2])
	numProcessors = int(sys.argv[3])

	fit_photopeak(fileToLoad, radialCut, numProcessors)
