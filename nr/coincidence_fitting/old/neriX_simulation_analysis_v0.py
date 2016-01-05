import ROOT as root
from ROOT import gROOT
import sys
from rootpy import stl
from rootpy.io import File
from rootpy.tree import Tree, TreeModel, TreeChain
from rootpy.plotting import Hist, Hist2D, Canvas
import neriX_simulation_datasets
import threading
import root_numpy
import numpy as np

# MUST INCLUDE TYPES TO AVOID SEG FAULT
stl.vector(stl.vector('float'))
stl.vector(stl.vector('int'))



def create_gaus_hist(mean, sigma, numBins, lowX, highX):
	numRandomSamples = 5e6

	fGaus = root.TF1('fGaus', 'gaus', lowX, highX)
	fGaus.SetParameters(1., mean, sigma)
	fGaus.SetNpx(numBins)
	hGaus = Hist(numBins, lowX, highX)
	hGaus.FillRandom('fGaus', int(numRandomSamples))
	hGaus.Scale(1./numRandomSamples)

	# debug
	"""
	c1 = Canvas()
	hGaus.Draw()
	c1.Update()
	raw_input('press enter')
	"""
	del fGaus
	return hGaus



# second histogram should be the spectrum you want smeared
def convolve_two_histograms(h1, h2):
	if( (h1.GetXaxis().GetXmin() != h2.GetXaxis().GetXmin()) or (h1.GetXaxis().GetXmax() != h2.GetXaxis().GetXmax()) ):
		print '\nBounds of the two histograms do not agree - please give histograms that have same bounds'
		return -1

	# if number of bins are not equal rebin the larger histogram
	if (h1.GetXaxis().GetNbins() != h2.GetXaxis().GetNbins()):
		print '\nWARNING: Number of bins are not equal - rebinning larger histogram.\n'
		hSmaller = min(h1, h2, key=lambda hist: hist.GetXaxis().GetNbins())
		hLarger = min(h1, h2, key=lambda hist: hist.GetXaxis().GetNbins())
		rebinConst = hLarger.GetXaxis().GetNbins()/hSmaller.GetXaxis().GetNbins()
		hLarger.Rebin(rebinConst)
		# delete temp references
		del hSmaller, hLarger

	hConvolve = Hist(h1.GetXaxis().GetNbins(), h1.GetXaxis().GetXmin(), h1.GetXaxis().GetXmax())

	# under and overflow not included
	aHist1 = np.array([bp.value for bp in h1])[1:-1]
	aHist2 = np.array([bp.value for bp in h2])[1:-1]

	#aConvolution = [0] + np.convolve(aHist1, aHist2, mode='same') + [0]
	aConvolution = np.convolve(aHist1, aHist2, mode='same')
	#print list(aConvolution)

	for i in xrange(len(aConvolution)):
		hConvolve[i].value = aConvolution[i]

	#debug
	c1 = Canvas()
	hConvolve.Draw()
	c1.Update()
	raw_input('press_enter')





class neriX_simulation_analysis:
	def __init__(self, lFilesToLoad = None, bReduced = False, degreeSetting = None):
	
		if isinstance(lFilesToLoad, str):
			lFilesToLoad = [lFilesToLoad]
		
		numFiles = len(lFilesToLoad)
		
		if numFiles > 1:
			assert degreeSetting != None


		DEGREE_INDEX = neriX_simulation_datasets.DEGREE_INDEX

		dRunFiles = neriX_simulation_datasets.run_files
		lRuns = neriX_simulation_datasets.runsInUse
		lParameters = None
		self.bReduced = bReduced

		# check neriX_simulation_datasets for file and parameters
		for i in xrange(len(lFilesToLoad)):
			if lFilesToLoad[i][-5:] != '.root':
				lFilesToLoad[i] += '.root'
			
		self.lFilenames = []
		for file in lFilesToLoad:
			self.lFilenames.append(file)
		
		for run in lRuns:
			try:
				lParameters = dRunFiles[run][self.lFilenames[0]]
				currentRun = run
				break
			except KeyError:
				continue

		if not lParameters:
			print str(self.lFilenames[0]) + ' does not appear in neriX_simulation_datasets.py'
			print 'Please check if the file exists and that your spelling is correct'
			sys.exit()
			
		if numFiles == 1:
			self.degreeSetting = lParameters[DEGREE_INDEX]
		else:
			self.degreeSetting = degreeSetting
		self.runNumber = currentRun
		
		
		# make final list of files that match anode, cathode, and degree settings
		self.lRootFiles = []
		# reset filenames to only include good ones
		self.lFilenames = []
		for file in lFilesToLoad:
			try:
				if dRunFiles[self.runNumber][file] == [self.degreeSetting]:
					print 'Adding ' + str(file) + ' to list.'
			
					if bReduced:
						pathToFile = neriX_simulation_datasets.pathToReducedFiles + str(file)
					else:
						pathToFile = neriX_simulation_datasets.pathToFullFiles + str(file)
				
					try:
						self.lRootFiles.append(File(pathToFile, 'read'))
					except:
						print '\n\nCould not locate ' + str(pathToFile)
						if numFiles > 1:
							print 'Continuing without file'
							numFiles -= 1
							continue
						else:
							sys.exit()
					if self.lRootFiles[-1].keys() == []:
						print 'Problem opening file - please check name entered'
						print 'Entered: ' + pathToFile
						self.lRootFiles.pop()
					else:
						print 'Successfully added ' + str(file) + '!\n'
						self.lFilenames.append(file)
				else:
					print 'File ' + str(file) + ' does not match set values for the anode, cathode, or degree (skipping).'
			except KeyError:
				print str(file) + ' does not appear in neriX_simulation_datasets.py'
				print 'Please check if the file exists and that your spelling is correct'
				
		# update number of files
		numFiles = len(self.lRootFiles)
			
		self.lT1 = [0 for i in xrange(numFiles)]
		self.lEList = [0 for i in xrange(numFiles)]
		
		for i, rootFile in enumerate(self.lRootFiles):
			self.lT1[i] = rootFile.t1
			self.lT1[i].SetName('t1_' + self.lFilenames[i])
			self.lT1[i].create_buffer()

			# create event list
			self.lEList[i] = root.TEventList('eList_' + self.lFilenames[i])
			root.SetOwnership(self.lEList[i], True)
		
			#define helpful aliases
			self.lT1[i].SetAlias('TOF','timeliqsci - time')
			self.lT1[i].SetAlias('X','xp')
			self.lT1[i].SetAlias('Y','yp')
			self.lT1[i].SetAlias('Z','zp')
			self.lT1[i].SetAlias('R','sqrt(pow(X,2)+pow(Y,2))')
		
		
		self.Xrun = '(eventid != -1)' #add a cut so that add statements work
		
		
		
	
	def get_run(self):
		return self.runNumber
	
	
	
	def get_filename(self, index=0):
		return self.lFilenames[index]
	
	
	
	def get_T1(self, index=0):
		return self.lT1[index]
	
	
	
	def get_lT1(self):
		return self.lT1



	def add_tof_cut(self, lowTOF = 10., highTOF = 70.):
		Xtof = '((TOF > ' + str(lowTOF) + ') && (TOF < '+ str(highTOF) + '))'
		self.Xrun = self.Xrun + ' && ' + Xtof



	def add_radius_cut(self, lowRadius = 0., highRadius = 20.):
		Xradius = '((R > ' + str(lowRadius) + ') && (R < '+ str(highRadius) + '))'
		self.Xrun = self.Xrun + ' && ' + Xradius
		


	def add_eDep_cut(self, lowEnergy = -2., highEnergy = 35.):
		Xedep = '((eDep > '+str(lowEnergy)+') && (eDep < '+str(highEnergy)+'))'
		self.Xrun = self.Xrun + ' && ' + Xedep



	def add_single_scatter_cut(self):
		self.Xrun += ' && ( (Alt$(ratio_s2tot1_s2top0,0.) < 0.06 || (((Alt$(Z,3.)>2.0 && Alt$(Z,3.)<5.0) || Alt$(Z,30.)>24.5) && (Alt$(ratio_s2tot1_s2top0,0.) < 0.1))) )'
	
	
	def add_cut(self, sCut):
		self.Xrun += ' && ' + sCut
		
		
		
	def reset_cuts(self):
		self.Xrun = '(eventid != -1)'



	def get_degree_setting(self):
		return self.degreeSetting



	def get_cuts(self):
		return self.Xrun
	
	
	
	def get_num_events(self):
		numEvents = 0
		for eventList in self.lEList:
			numEvents += eventList.GetN()
		return numEvents



	def get_timestamp(self, eventNumber=0, index=0):
		self.lT1[index].GetEntry(eventNumber)
		return self.lT1[index].TimeSec



	def get_livetime(self):
		return self.get_timestamp(self.T1.GetEntries() - 1) - self.get_timestamp(0)



	def set_event_list(self, cuts = None):
		if cuts == None:
			cuts = self.Xrun
		#self.reset_event_list()
		for i, currentTree in enumerate(self.lT1):
			print 'Working on ' + str(self.lFilenames[i]) + ' (' + str(i+1) + '/' + str(len(self.lT1)) + ')...'
			print 'Original number of elements: ' + str(currentTree.GetEntries())
			root.TTree.Draw(currentTree, '>>eList_' + self.lFilenames[i], root.TCut(cuts), '')
			print 'Number of elements after cuts: ' + str(self.lEList[i].GetN())
			self.lT1[i].SetEventList(self.lEList[i])



	def thread_set_event_list(self, lIndices, lock, cuts = None):
		if cuts == None:
			cuts = self.Xrun
		#self.reset_event_list()
		for i in lIndices:
			with lock:
				print 'Working on ' + str(self.lFilenames[i]) + ' (' + str(i+1) + '/' + str(len(self.lT1)) + ')...'
				print 'Original number of elements: ' + str(self.lT1[i].GetEntries()) + ' (' + str(i+1) + '/' + str(len(self.lT1)) + ')'
			root.TTree.Draw(self.lT1[i], '>>eList_' + self.lFilenames[i], root.TCut(cuts), '')
			with lock:
				print 'Number of elements after cuts: ' + str(self.lEList[i].GetN()) + ' (' + str(i+1) + '/' + str(len(self.lT1)) + ')'
			self.lT1[i].SetEventList(self.lEList[i])



	def multithread_set_event_list(self, numThreads=1, cuts=None):
		if numThreads == 1:
			self.set_event_list(cuts)
		else:
			if numThreads > len(self.lT1):
				print 'More threads than files - reducing number of processors used'
				numThreads = len(self.lT1)
			lock = threading.Lock()
			# set thread tasks
			lThreadTasks = [[] for i in xrange(numThreads)]
			for i in xrange(len(self.lFilenames)):
				lThreadTasks[i%numThreads].append(i)

			# call worker function
			lThreads = [0. for i in xrange(numThreads)]
			for i in xrange(numThreads):
				lThreads[i] = threading.Thread(target=self.thread_set_event_list, args=(lThreadTasks[i], lock, cuts))
				lThreads[i].start()

			# block calling process until threads finish
			for i in xrange(numThreads):
				lThreads[i].join()




	# overwrite standard draw to accomodate list of files
	def Draw(self, *args, **kwargs):
		for currentTree in self.lT1:
			currentTree.Draw(*args, **kwargs)



	def draw(self, *args, **kwargs):
		self.Draw(*args, **kwargs)
	
	
	
	def thread_draw(self, lock, lTreeIndices, args, kwargs):
		for index in lTreeIndices:
			with lock:
				print 'Currently drawing ' + str(self.lFilenames[index]) + ' to histogram'  + ' (' + str(index+1) + '/' + str(len(self.lT1)) + ')...'
			self.lT1[index].Draw(*args, **kwargs)


	
	# multithreads the draw command
	# call draw as you normally would BUT with additional two arguments at the beginning
	def multithread_draw(self, numThreads=1, *origArgs, **kwargs):
		try:
			origHist = kwargs['hist']
		except KeyError:
			print 'Must pass "hist" argument to draw'
			sys.exit()
		if numThreads == 1:
			self.Draw(*origArgs, **kwargs)
		else:
			if numThreads > len(self.lT1):
				print 'More threads than files - reducing number of processors used'
				numThreads = len(self.lT1)
			lock = threading.Lock()
			
			lHists = [0 for i in xrange(numThreads)]
			lKwargs = [0 for i in xrange(numThreads)]
			lThreads = [0 for i in xrange(numThreads)]
			
			# set thread tasks
			lThreadTasks = [[] for i in xrange(numThreads)]
			for i in xrange(len(self.lFilenames)):
				lThreadTasks[i%numThreads].append(i)
			
			for i in xrange(numThreads):
				lHists[i] = origHist.empty_clone()
				lKwargs[i] = dict(kwargs)
				try:
					lKwargs[i]['hist'] = lHists[i]
				except:
					print 'Must pass "hist" argument to draw'
					sys.exit()
				
				lThreads[i] = threading.Thread(target=self.thread_draw, args=(lock, lThreadTasks[i], origArgs, lKwargs[i]))
				lThreads[i].start()

			# block calling process until threads finish
			for i in xrange(numThreads):
				lThreads[i].join()

			for i in xrange(numThreads):
				origHist.Add(lHists[i])


	
	
	def reset_event_list(self):
		self.T1.SetEventList(0)
		if ('elist_' + self.filename) in locals():
			self.eList.Clear()
			self.eList.Reset()
			self.eList.SetDirectory(0)
			#del self.eList



	def get_event_list(self, index=0):
		return self.lEList[index]



	def make_reduced_file(self, newFilename=None):
		if newFilename == None:
			newFilename = self.get_filename()
			print 'NOTE: No filename given, will default to ' + newFilename

		pathForNewFile = neriX_simulation_datasets.pathToReducedFiles + newFilename

		print '\nWill create reduced tree with the following cuts:\n'
		print self.get_cuts()
		print '\n\n'

		sCutsApproved = raw_input('Please eneter "y" if you approve of the cuts: ').lower()
		if sCutsApproved != 'y':
			print 'Exiting reduced file production'
			return


		treeList = root.TList()
		for tree in self.lT1:
			currentTree = tree.CopyTree(self.get_cuts())
			treeList.Add(currentTree)

		newTree = root.TTree.MergeTrees(treeList)


		#newTree.SetName('nerixsim_' + str(int(self.degreeSetting)) + 'deg_reduced')
		#newTree.SetTitle('nerixsim_' + str(int(self.degreeSetting)) + 'deg_reduced')
		newTree.SetName('t1')
		newTree.SetTitle('t1')

		newFile = File(pathForNewFile + '.root', 'recreate')

		newTree.Write()
		newFile.Write()








if __name__ == '__main__':
	#test = neriX_simulation_analysis(['rCoincidenceAnalysis10keV0cm3.0lsscale.root', 'rCoincidenceAnalysis10keV40cm1.0lsscale.root'], False, 45.)
	#test.add_radius_cut()
	#test.add_tof_cut()
	#test.make_reduced_file('test')

	test = neriX_simulation_analysis(['test.root'], True, 45.)
	convolve_two_histograms(create_gaus_hist(0, .001, 1000, -10, 10), create_gaus_hist(-3, 1, 1000, -10, 10))



