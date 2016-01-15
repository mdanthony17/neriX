#!/usr/bin/python

import ROOT as root
import sys, array, os, pickle
from rootpy.plotting import Hist, Hist2D, Canvas, Legend
import neriX_simulation_config, neriX_simulation_datasets
import numpy as np
from math import exp



def get_g2_default(g2RV):
	return neriX_simulation_datasets.default_g2 + (g2RV*neriX_simulation_datasets.default_g2_uncertainty)



def get_gas_gain_default(gasGainRV):
	return neriX_simulation_datasets.default_gas_gain + (gasGainRV*neriX_simulation_datasets.default_gas_gain_uncertainty)



def get_exciton_ion_ratio(lParsRV, meanField, meanEnergy):
	lParValues = [0. for i in xrange(len(lParsRV))]
	for parNumber, parRV in enumerate(lParsRV):
		if parNumber == 0:
			# alpha
			meanFromNEST = 1.240
			sigmaLowFromNEST = 0.073
			sigmaHighFromNEST = 0.079
		elif parNumber == 1:
			# zeta
			meanFromNEST = 0.0472
			sigmaLowFromNEST = 0.0073
			sigmaHighFromNEST = 0.0088
		elif parNumber == 2:
			# beta
			meanFromNEST = 239.0
			sigmaLowFromNEST = 8.8
			sigmaHighFromNEST = 28.0
		else:
			print 'FAILURE: Incorrect number of parameters given for exciton to ion ratio.'
			sys.exit()

		if parRV >= 0:
			valueForPar = meanFromNEST + parRV*sigmaHighFromNEST
		else:
			valueForPar = meanFromNEST - parRV*sigmaLowFromNEST

		lParValues[parNumber] = valueForPar
	
	return lParValues[0]*(meanField**(-lParValues[1])) * ( 1 - exp(-lParValues[2] * 11.5*meanEnergy*54**(-7./3.)) )
	
	
# returns 1 - probPhotonQuenching
def get_photon_quenching(lParsRV, meanEnergy):
	lParValues = [0. for i in xrange(len(lParsRV))]
	for parNumber, parRV in enumerate(lParsRV):
		if parNumber == 0:
			# eta
			meanFromNEST = 3.3
			sigmaLowFromNEST = -0.7
			sigmaHighFromNEST = 5.3
		elif parNumber == 1:
			# lambda
			meanFromNEST = 1.14
			sigmaLowFromNEST = -0.09
			sigmaHighFromNEST = 0.45
		else:
			print 'FAILURE: Incorrect number of parameters given for exciton to ion ratio.'
			sys.exit()

		if parRV >= 0:
			valueForPar = meanFromNEST + parRV*sigmaHighFromNEST
		else:
			valueForPar = meanFromNEST - parRV*sigmaLowFromNEST

		lParValues[parNumber] = valueForPar
		
	photonQuenching = 1./(1. + lParValues[0]*( 11.5*meanEnergy*54.**(-7./3.) )**lParValues[1])
	if photonQuenching < 0 or photonQuenching > 1:
		return photonQuenching
	else:
		return photonQuenching





# ************************************************
# ------------------------------------------------
# ------------------------------------------------
# ------------------------------------------------
# ------------------------------------------------
# ------------------------------------------------
#  Start actual analysis code
# ------------------------------------------------
# ------------------------------------------------
# ------------------------------------------------
# ------------------------------------------------
# ------------------------------------------------
# ************************************************




if len(sys.argv) != 5:
	print 'Use is python plot_free_parameters.py <degree> <cathode> <anode> <num walkers>'
	sys.exit()


useFakeValueInPlots = True


#nameOfResultsDirectory = neriX_simulation_config.nameOfResultsDirectory
nameOfResultsDirectory = 'fake_data/results'
pathToSamplerDictionary = nameOfResultsDirectory

degreeSetting = int(sys.argv[1])
cathodeSetting = float(sys.argv[2])
anodeSetting = float(sys.argv[3])
numWalkers = int(sys.argv[4])

sPathToFile = './%s/%ddeg_%.3fkV_%.1fkV/full_matching/sampler_dictionary.p' % (nameOfResultsDirectory, degreeSetting, cathodeSetting, anodeSetting)

if os.path.exists(sPathToFile):
	dSampler = pickle.load(open(sPathToFile, 'r'))
	aSampler = dSampler[numWalkers][-1]['_chain'] # look at last sampler only (can change)
	print 'Successfully loaded sampler!'
else:
	print sPathToFile
	print 'Could not find file!'



#--------------- Start parameters to for histograms ----------------


dParametersToDraw = {'lindhard_factor':{'index':0,
									    'binning':[100, 0.1, 0.35],
										'true_value_for_fake':0.18},
					 'recombination_probability':{'index':1,
												  'binning':[100, 0.15, 0.4],
												  'true_value_for_fake':0.24},
					 'n_g2':{'index':10,
							 'binning':[100, -2, 2],
							 'true_value_for_fake':0},
					 'n_gas_gain_mean':{'index':11,
							            'binning':[100, -2, 2],
							            'true_value_for_fake':0},
					 'n_par0_e_to_i':{'index':-5,
							   'binning':[100, -2, 2],
							   'true_value_for_fake':0},
					 'n_par1_e_to_i':{'index':-4,
							   'binning':[100, -2, 2],
							   'true_value_for_fake':0},
					 'n_par2_e_to_i':{'index':-3,
							   'binning':[100, -2, 2],
							   'true_value_for_fake':0},
					 'n_par0_photon_quenching':{'index':3,
							   'binning':[100, -2, 2],
							   'true_value_for_fake':0},
					 'n_par1_photon_quenching':{'index':3,
							   'binning':[100, -2, 2],
							   'true_value_for_fake':0}
					}

lParametersPhotonYield = [50, 6, 14]
lParametersChargeYield = [50, 2, 10]



#--------------- End Parameters for histograms ----------------


# ------------------------------------------------
# Fill histograms for sampling
# ------------------------------------------------


dPlots = {}

for parameter in dParametersToDraw:
	# initialize canvas and histogram
	dPlots[parameter] = {}
	dPlots[parameter]['hist'] = Hist(dParametersToDraw[parameter]['binning'][0], dParametersToDraw[parameter]['binning'][1], dParametersToDraw[parameter]['binning'][2], name='h_%s' % parameter, title=parameter, drawstyle='hist')
	
	# fill histogram
	dPlots[parameter]['hist'].fill_array(aSampler[:,:,dParametersToDraw[parameter]['index']].flatten())

	# open current value key
	dPlots[parameter]['current_value'] = 0.



# ------------------------------------------------
# Find mean energy and field
# ------------------------------------------------


dFields = {0.345:200,
		   1.054:500,
		   2.356:1000
		  }

dEnergies = {45:10,
			 30:5
			}

try:
	meanField = dFields[cathodeSetting]
except KeyError:
	print 'No matching cathode voltage.'
	sys.exit()


try:
	meanEnergy = dEnergies[degreeSetting]
except KeyError:
	print 'No matching angle.'
	sys.exit()

wValue = 13.7e-3



# photon yield = l / w * (1-probPhotonQuenching) * ( (1 - 1./(1+Nex/Ni)) +  (1./(1+Nex/Ni))*probRecombination )
def get_photon_yield(lindhardFactor, wValue, notProbPhotonQuenching, excitonToIonRatio, probRecombination):
	return lindhardFactor / wValue * notProbPhotonQuenching * ( (1 - 1./(1+excitonToIonRatio)) +  (1./(1+excitonToIonRatio))*probRecombination )

def get_charge_yield(lindhardFactor, wValue, excitonToIonRatio, probRecombination, extractionEfficiency):
	return lindhardFactor / wValue * notProbPhotonQuenching * (1./(1+excitonToIonRatio))*(1.-probRecombination) * extractionEfficiency


# ------------------------------------------------
# Create histograms for filling
# ------------------------------------------------

hPhotonYield = Hist(*lParametersPhotonYield, name='hPhotonYield', title='Photon Yield Matching')
hPhotonYield.SetMarkerSize(0)
hPhotonYield.SetStats(0)
hPhotonYield.GetXaxis().SetTitle('Photon Yield #frac{photons}{keV}')
hPhotonYield.GetYaxis().SetTitle('Counts')

hChargeYield = Hist(*lParametersChargeYield, name='hChargeYield', title='Charge Yield Matching')
hChargeYield.SetMarkerSize(0)
hChargeYield.SetStats(0)
hChargeYield.GetXaxis().SetTitle('Charge Yield #frac{electrons}{keV}')
hChargeYield.GetYaxis().SetTitle('Counts')


# ------------------------------------------------
# Run MC to find distribution of photon yield
# ------------------------------------------------


numMCTrials = int(1e4)
root.gRandom.SetSeed(0)

for i in xrange(numMCTrials):
	# get current values from distribution
	for parameter in dParametersToDraw:
		dPlots[parameter]['current_value'] = dPlots[parameter]['hist'].GetRandom()

	# find notProbPhotonQuenching
	notProbPhotonQuenching = get_photon_quenching([dPlots['n_par0_photon_quenching']['current_value'], dPlots['n_par1_photon_quenching']['current_value']], meanEnergy)

	# find exciton to ion ratio
	excitonToIonRatio = get_exciton_ion_ratio([dPlots['n_par0_e_to_i']['current_value'], dPlots['n_par1_e_to_i']['current_value'], dPlots['n_par2_e_to_i']['current_value']], meanField, meanEnergy)

	g2Value = get_g2_default(dPlots['n_g2']['current_value'])
	gasGainValue = get_g2_default(dPlots['n_gas_gain_mean']['current_value'])

	#print dPlots['lindhard_factor']['current_value'], wValue, notProbPhotonQuenching, excitonToIonRatio, dPlots['recombination_probability']['current_value']

	hPhotonYield.Fill(get_photon_yield(dPlots['lindhard_factor']['current_value'], wValue, notProbPhotonQuenching, excitonToIonRatio, dPlots['recombination_probability']['current_value']))

	hChargeYield.Fill(get_charge_yield(dPlots['lindhard_factor']['current_value'], wValue, excitonToIonRatio, dPlots['recombination_probability']['current_value'], g2Value / gasGainValue))



# ------------------------------------------------
# Find true values
# ------------------------------------------------

if useFakeValueInPlots:

	# find notProbPhotonQuenching
	notProbPhotonQuenching = get_photon_quenching([dParametersToDraw['n_par0_photon_quenching']['true_value_for_fake'], dParametersToDraw['n_par1_photon_quenching']['true_value_for_fake']], meanEnergy)

	# find exciton to ion ratio
	excitonToIonRatio = get_exciton_ion_ratio([dParametersToDraw['n_par0_e_to_i']['true_value_for_fake'], dParametersToDraw['n_par1_e_to_i']['true_value_for_fake'], dParametersToDraw['n_par2_e_to_i']['true_value_for_fake']], meanField, meanEnergy)

	trueValuePhotonYield = get_photon_yield(dParametersToDraw['lindhard_factor']['true_value_for_fake'], wValue, notProbPhotonQuenching, excitonToIonRatio, dParametersToDraw['recombination_probability']['true_value_for_fake'])

	print 'Simulated photon yield: %.3f photons/keV' % trueValuePhotonYield


	g2Value = get_g2_default(dParametersToDraw['n_g2']['true_value_for_fake'])
	gasGainValue = get_g2_default(dParametersToDraw['n_gas_gain_mean']['true_value_for_fake'])

	trueValueChargeYield = get_charge_yield(dParametersToDraw['lindhard_factor']['true_value_for_fake'], wValue, excitonToIonRatio, dParametersToDraw['recombination_probability']['true_value_for_fake'], g2Value / gasGainValue)
	
	print 'Simulated charge yield: %.3f photons/keV' % trueValueChargeYield


# ------------------------------------------------
# Draw Photon Yield
# ------------------------------------------------

c1 = Canvas()
hPhotonYield.Draw()
aQuantilesPhotonYield = hPhotonYield.quantiles([.5-.341, .5, .5+.341])

lLinesPhotonYield = []
for value in aQuantilesPhotonYield:
	lLinesPhotonYield.append(root.TLine(value, hPhotonYield.GetYaxis().GetXmin(), value, hPhotonYield.GetMaximum()))
	lLinesPhotonYield[-1].SetLineColor(root.kBlue)
	lLinesPhotonYield[-1].SetLineStyle(7)
	lLinesPhotonYield[-1].Draw('same')

if useFakeValueInPlots:
	lLinesPhotonYield.append(root.TLine(trueValuePhotonYield, hPhotonYield.GetYaxis().GetXmin(), trueValuePhotonYield, hPhotonYield.GetMaximum()))
	lLinesPhotonYield[-1].SetLineColor(root.kRed)
	lLinesPhotonYield[-1].SetLineStyle(7)
	lLinesPhotonYield[-1].Draw('same')

lowSigmaPhotonYield = aQuantilesPhotonYield[1] - aQuantilesPhotonYield[0]
highSigmaPhotonYield = aQuantilesPhotonYield[2] - aQuantilesPhotonYield[1]

sFitInfo1 = 'P_{y} = %.2f^{+%.2f}_{-%.2f} #frac{photons}{keV}' % (aQuantilesPhotonYield[1], lowSigmaPhotonYield, highSigmaPhotonYield)
pt1 = root.TPaveText(.55,.65,.8,.7,'blNDC')
pt1.AddText(sFitInfo1)
pt1.SetTextColor(root.kBlack)
pt1.SetFillStyle(0)
pt1.SetBorderSize(0)
pt1.Draw('same')

if useFakeValueInPlots:
	sFitInfo2 = 'Simulated P_{y} = %.2f #frac{photons}{keV}' % (trueValuePhotonYield)
	pt2 = root.TPaveText(.55,.55,.8,.6,'blNDC')
	pt2.AddText(sFitInfo2)
	pt2.SetTextColor(root.kBlack)
	pt2.SetFillStyle(0)
	pt2.SetBorderSize(0)
	pt2.Draw('same')


c1.Update()



# ------------------------------------------------
# Draw Charge Yield
# ------------------------------------------------


c2 = Canvas()
hChargeYield.Draw()
aQuantilesChargeYield = hChargeYield.quantiles([.5-.341, .5, .5+.341])

lLinesChargeYield = []
for value in aQuantilesChargeYield:
	lLinesChargeYield.append(root.TLine(value, hChargeYield.GetYaxis().GetXmin(), value, hChargeYield.GetMaximum()))
	lLinesChargeYield[-1].SetLineColor(root.kBlue)
	lLinesChargeYield[-1].SetLineStyle(7)
	lLinesChargeYield[-1].Draw('same')

if useFakeValueInPlots:
	lLinesChargeYield.append(root.TLine(trueValueChargeYield, hChargeYield.GetYaxis().GetXmin(), trueValueChargeYield, hChargeYield.GetMaximum()))
	lLinesChargeYield[-1].SetLineColor(root.kRed)
	lLinesChargeYield[-1].SetLineStyle(7)
	lLinesChargeYield[-1].Draw('same')

lowSigmaChargeYield = aQuantilesChargeYield[1] - aQuantilesChargeYield[0]
highSigmaChargeYield = aQuantilesChargeYield[2] - aQuantilesChargeYield[1]

sFitInfo1 = 'Q_{y} = %.2f^{+%.2f}_{-%.2f} #frac{photons}{keV}' % (aQuantilesChargeYield[1], lowSigmaChargeYield, highSigmaChargeYield)
pt3 = root.TPaveText(.6,.65,.88,.7,'blNDC')
pt3.AddText(sFitInfo1)
pt3.SetTextColor(root.kBlack)
pt3.SetFillStyle(0)
pt3.SetBorderSize(0)
pt3.Draw('same')

if useFakeValueInPlots:
	sFitInfo2 = 'Simulated Q_{y} = %.2f #frac{photons}{keV}' % (trueValueChargeYield)
	pt4 = root.TPaveText(.6,.55,.88,.6,'blNDC')
	pt4.AddText(sFitInfo2)
	pt4.SetTextColor(root.kBlack)
	pt4.SetFillStyle(0)
	pt4.SetBorderSize(0)
	pt4.Draw('same')


c2.Update()





raw_input('Press enter to continue...')

