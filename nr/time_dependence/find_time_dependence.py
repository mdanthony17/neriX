#!/usr/bin/python

import ROOT as root
import neriX_analysis, neriX_datasets
from rootpy import stl
from rootpy.io import File
from rootpy.tree import Tree, TreeModel, TreeChain
from rootpy.plotting import Hist, Hist2D, Canvas, Graph, func
import sys, os
import numpy as np
import copy

run = 15

dParameters = {'tof_efficiency':{'num_parameters':1,
								 'fit_func':'pol0',
								 'path_to_results':'/Users/Matt/Desktop/Xenon/neriX/nr/tof_efficiency/results/run_15/',
								 'root_filename_prefix':'efficiency_fit_',
								 'func_name':'fEfficiency',
								 'units':'1/PE'
								}
			  }

dFitsInTime = {}

neriX_analysis.warning_message('Currently only have non-correlated parameters implemented!')


for parameterToFit in dParameters:
	# dFitsInTime[parameterToFit] = {'canvas':[canvas_of_par0, canvas_of_par1, ...], 'fit':[fit_of_par0, fit_of_par1, ...]}
	dFitsInTime[parameterToFit] = {'canvas':[], 'graph':[], 'fit':[], 'arr_par':[], 'arr_par_err':[], 'arr_cathode_setting':[], 'arr_time':[]}
	
	lSavePath = ['results', 'run_15', parameterToFit]

	# get list of files that were fit
	lFiles = os.listdir(dParameters[parameterToFit]['path_to_results'])
	
	try:
		lFiles.remove('.DS_Store')
	except:
		pass

	# create arrays for parameters and their errors
	for i in xrange(dParameters[parameterToFit]['num_parameters']):
		dFitsInTime[parameterToFit]['arr_par'].append(np.zeros(len(lFiles)))
		dFitsInTime[parameterToFit]['arr_par_err'].append(np.zeros(len(lFiles)))
		dFitsInTime[parameterToFit]['arr_cathode_setting'].append(np.zeros(len(lFiles)))
		dFitsInTime[parameterToFit]['arr_time'].append(np.zeros(len(lFiles)))

	# loop through files and pull parameters for array
	for i in xrange(len(lFiles)):
		
		# load file and function
		currentFile = File(dParameters[parameterToFit]['path_to_results'] + lFiles[i] + '/' + dParameters[parameterToFit]['root_filename_prefix'] + lFiles[i] + '.root')
		currentFunc = currentFile.Get(dParameters[parameterToFit]['func_name'])
		
		# save first for template
		if i == 0:
			functionToSave = copy.deepcopy(currentFunc)

		for parNumber in xrange(dParameters[parameterToFit]['num_parameters']):
			dFitsInTime[parameterToFit]['arr_par'][parNumber][i] = currentFunc.GetParameter(parNumber)
			dFitsInTime[parameterToFit]['arr_par_err'][parNumber][i] = currentFunc.GetParError(parNumber)
			dFitsInTime[parameterToFit]['arr_cathode_setting'][parNumber][i] =neriX_analysis.check_for_file_info(lFiles[i])[1]
			dFitsInTime[parameterToFit]['arr_time'][parNumber][i] =neriX_analysis.convert_name_to_unix_time(lFiles[i])
			
		currentFile.Close()

	for parNumber in xrange(dParameters[parameterToFit]['num_parameters']):
		aTimeErrors = np.zeros(len(lFiles))
		dFitsInTime[parameterToFit]['canvas'].append(Canvas())
		dFitsInTime[parameterToFit]['graph'].append(root.TGraphErrors(len(lFiles), dFitsInTime[parameterToFit]['arr_time'][parNumber], dFitsInTime[parameterToFit]['arr_par'][parNumber], aTimeErrors, dFitsInTime[parameterToFit]['arr_par_err'][parNumber]))
		
		# clean up graph
		dFitsInTime[parameterToFit]['graph'][parNumber].SetTitle('%s, par%d in Time' % (parameterToFit, parNumber))
		dFitsInTime[parameterToFit]['graph'][parNumber].GetXaxis().SetTimeDisplay(1)
		dFitsInTime[parameterToFit]['graph'][parNumber].GetYaxis().SetTitle('%s, par%d [%s]' % (parameterToFit, parNumber, dParameters[parameterToFit]['units']))
		dFitsInTime[parameterToFit]['graph'][parNumber].Draw('AP')
		
		# fit graph
		dFitsInTime[parameterToFit]['fit'].append(root.TF1('f_par%d' % parNumber, dParameters[parameterToFit]['fit_func']))
		dFitsInTime[parameterToFit]['fit'][parNumber].SetLineColor(root.kBlue)
		dFitsInTime[parameterToFit]['graph'][parNumber].Fit('f_par%d' % parNumber, 'MELL')
		dFitsInTime[parameterToFit]['fit'][parNumber].Draw('same')
		

		meanFromFit = dFitsInTime[parameterToFit]['fit'][parNumber].GetParameter(0)

		# -----------------------------------------
		# PDG perscription for fitting average
		# -----------------------------------------
		"""
		chi2 = dFitsInTime[parameterToFit]['fit'][parNumber].GetChisquare()
		ndf = dFitsInTime[parameterToFit]['fit'][parNumber].GetNDF()
		print chi2, ndf
		
		if chi2 / float(ndf) > 1:
			dFitsInTime[parameterToFit]['graph'][parNumber] = root.TGraphErrors(len(lFiles), dFitsInTime[parameterToFit]['arr_time'][parNumber], dFitsInTime[parameterToFit]['arr_par'][parNumber], aTimeErrors, dFitsInTime[parameterToFit]['arr_par_err'][parNumber] * (chi2 / float(ndf))**0.5)
			dFitsInTime[parameterToFit]['graph'][parNumber].Fit('f_par%d' % parNumber, 'MELL')
			dFitsInTime[parameterToFit]['fit'][parNumber].Draw('same')
			dFitsInTime[parameterToFit]['canvas'][parNumber].Update()
		
		chi2 = dFitsInTime[parameterToFit]['fit'][parNumber].GetChisquare()
		ndf = dFitsInTime[parameterToFit]['fit'][parNumber].GetNDF()
		print chi2, ndf
		"""

		# -----------------------------------------
		# Variance for systematic uncertainty
		# -----------------------------------------

		variance = 1./float(len(lFiles))
		sumOfTerms = 0
		for value in dFitsInTime[parameterToFit]['arr_par'][parNumber]:
			sumOfTerms += (meanFromFit - value)**2.
		
		variance *= sumOfTerms
		stdev = variance**0.5

		# draw +/- one sigma lines
		lowTimeLimit = dFitsInTime[parameterToFit]['graph'][parNumber].GetXaxis().GetXmin()
		highTimeLimit = dFitsInTime[parameterToFit]['graph'][parNumber].GetXaxis().GetXmax()

		minusSigmaLine = root.TLine()
		minusSigmaLine.SetLineColor(root.kBlue + 1)
		minusSigmaLine.SetLineStyle(7)
		minusSigmaLine.DrawLine(lowTimeLimit, meanFromFit - stdev, highTimeLimit, meanFromFit - stdev)

		plusSigmaLine = root.TLine()
		plusSigmaLine.SetLineColor(root.kBlue + 1)
		plusSigmaLine.SetLineStyle(7)
		plusSigmaLine.DrawLine(lowTimeLimit, meanFromFit + stdev, highTimeLimit, meanFromFit + stdev)


		dFitsInTime[parameterToFit]['canvas'][parNumber].Update()

		neriX_analysis.save_plot(lSavePath, dFitsInTime[parameterToFit]['canvas'][parNumber], '%s_par%d' % (parameterToFit, parNumber))

		functionToSave.SetParameter(parNumber, meanFromFit)
		functionToSave.SetParError(parNumber, stdev)

	functionToSave.SetTitle(parameterToFit)
	functionToSave.SetName(parameterToFit)

	# save object
	# this is temporary until rate dependence is implemented
	neriX_analysis.write_root_object(['..', 'coincidence_fitting', 'efficiency_files'], functionToSave, parameterToFit)


raw_input('Press enter to continue...')


