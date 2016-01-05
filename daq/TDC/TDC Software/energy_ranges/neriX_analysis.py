import ROOT as root
import sys

class neriX_analysis:
	__GERMANIUM_SLOPE = '692.905899'
	__GERMANIUM_INTERCEPT = '1.67455'

	def __init__(self, fileToLoad = ''):
		if fileToLoad == '':
			print 'Need to use a file!'
			exit()


		pathToFile = './data/run_10/' + str(fileToLoad)

		try:
			rootFile = root.TFile(pathToFile, 'read')
		except:
			print 'Problem opening file - please check name entered'
			sys.exit(1)


		self.myTree = root.TChain('T1', 'myTree')
		self.myTree.SetBranchStatus('*', 1)
		self.T2 = root.TChain('T2', 'T2')
		self.T2.SetBranchStatus('*', 1)
		self.T3 = root.TChain('T3', 'T3')
		self.T3.SetBranchStatus('*', 1)

		self.myTree.AddFriend('T2')
		self.myTree.AddFriend('T3')

		self.myTree.AddFile(pathToFile)
		self.T2.AddFile(pathToFile)
		self.T3.AddFile(pathToFile)


		#define helpful aliases
		self.myTree.SetAlias('dt','(S2sPeak[0]-S1sPeak[0])/100.')
		self.myTree.SetAlias('X','S2sPosFann[0][0]')
		self.myTree.SetAlias('Y','S2sPosFann[0][1]')
		self.myTree.SetAlias('R','sqrt(pow(S2sPosFann[0][0],2)+pow(S2sPosFann[0][1],2))')

		#will need to change these constants depending on Ge Calibration
		self.myTree.SetAlias('GeEnergy',self.__GERMANIUM_SLOPE + '*GeHeight[0] + ' + self.__GERMANIUM_INTERCEPT)
		self.myTree.SetAlias('eDep','661.657 - GeEnergy')


		self.Xrun = 'S1sCoin[0] != -1' #add a cut so that add statements work



	def get_tree(self):
		return self.myTree



	def add_dt_cut(self, lowdt = 2., highdt = 13.):
		Xdt = '(dt > ' + str(lowdt) + ') && (dt < '+ str(highdt) + ')'
		self.Xrun = self.Xrun + '&&' + Xdt



	def add_radius_cut(self, lowRadius = 0., highRadius = 20.):
		Xradius = '(R > ' + str(lowRadius) + ') && (R < '+ str(highRadius) + ')'
		self.Xrun = self.Xrun + '&&' + Xradius
		


	def add_eDep_cut(self, lowEnergy = -2., highEnergy = 35.):
		Xedep = '(eDep > '+str(lowEnergy)+') && (eDep < '+str(highEnergy)+')'
		self.Xrun = self.Xrun + '&&' + Xedep
		
		
		
	def add_s1_trig_cut(self):
		#Xtrig = '((CoinRaw[0] > (2.71e-8 - 5*2.83e-10) && CoinRaw[0] < (2.71e-8 + 5*2.83e-10)) || (CoinRaw[0] > (9.26e-8 - 5*5.87e-10) && CoinRaw[0] < (9.26e-8 + 5*5.87e-10)) || (CoinRaw[0] > (1.58e-7 - 5*7.48e-10) && CoinRaw[0] < (1.58e-7 + 5*7.48e-10)))'
		#Xtrig = '((CoinRaw[0] > .26e-7 && CoinRaw[0] < .28e-7) || (CoinRaw[0] > .9e-7 && CoinRaw[0] < .95e-7) || (CoinRaw[0] > .155e-6 && CoinRaw[0] < .16e-6))'
		#Xtrig = '(TrigLeftEdge[] - S1sPeak[0] > 0 && TrigLeftEdge[] - S1sPeak[0] < 400) && ((TrigArea[] > .15e-7 && TrigArea[] < .35e-7) || (TrigArea[] > .8e-7 && TrigArea[] < 1e-7) || (TrigArea[] > .145e-6 && TrigArea[] < .165e-6))'
		#Xtrig = '(((TrigLeftEdge[] - S1sPeak[0]) > 0.) && ((TrigLeftEdge[] - S1sPeak[0]) < 40.)) && ((TrigArea[] > (2.75e-8 - 5*2.47e-10) && TrigArea[] < (2.75e-8 + 5*2.47e-10)) || (TrigArea[] > (9.26e-8 - 5*4.14e-10) && TrigArea[] < (9.26e-8 + 5*4.14e-10)) || (TrigArea[] > (1.57e-7 - 5*5.92e-10) && TrigArea[] < (1.57e-7 + 5*5.92e-10)) || (TrigArea[] > 2.2e-7 && TrigArea[] < 2.3e-7) || (TrigArea[] > .56e-7 && TrigArea[] < .54e-7))'
		Xtrig2 = '(((TrigLeftEdge[] - S1sPeak[0]) > 0.) && ((TrigLeftEdge[] - S1sPeak[0]) < 40.)) && ((TrigArea[] > (2.3e-8) && TrigArea[] < (3.00e-8)) || (TrigArea[] > (8.2e-8) && TrigArea[] < (10.2e-8)) || (TrigArea[] > (1.50e-7) && TrigArea[] < (1.64e-7)) || (TrigArea[] > 2.2e-7 && TrigArea[] < 2.3e-7) || (TrigArea[] > .56e-7 && TrigArea[] < .54e-7))'
		self.Xrun = self.Xrun + '&&' + Xtrig2
		
		
		
	def add_all_cuts(self, lowEnergy = -2., highEnergy = 35.):
		self.add_eDep_cut(lowEnergy, highEnergy)
		self.add_coincidence_cut()
		
		
		
	def reset_cuts(self):
		self.Xrun = 'S1sCoin[0] > 0'
		



	def get_cuts(self):
		return self.Xrun



	def get_timestamp(self, eventNumber):
		self.myTree.GetEntry(eventNumber)
		return self.myTree.TimeSec



	def get_livetime(self):
		return self.get_timestamp(self.myTree.GetEntries() - 1) - self.get_timestamp(0)



	def set_event_list(self, cuts = None):
		if cuts == None:
			cuts = self.Xrun
		self.eList = root.TEventList('eList')
		self.myTree.Draw('>>eList', cuts, '')
		self.myTree.SetEventList(self.eList)



	def get_event_list(self):
		return self.eList






#test = neriX_analysis('nerix_140914_1631.root')
#test.get_timestamp(1)
#print test.get_livetime()