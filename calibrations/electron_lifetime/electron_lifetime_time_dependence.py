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
import electron_lifetime_fit_results

run = 15

dParameters = {'tof_efficiency':{'num_parameters':1,
								 'fit_func':'pol0',
								 'path_to_results':'/Users/Matt/Desktop/Xenon/neriX/nr/tof_efficiency/results/run_15/',
								 'root_filename_prefix':'efficiency_fit_',
								 'func_name':'fEfficiency',
								 'units':'1/PE'
								}
			  }

lSavePath = ['results', 'run_15', 'time_dependence']

neriX_analysis.warning_message('Currently only have non-correlated parameters implemented!')


d_electron_lifetime = electron_lifetime_fit_results.d_electron_lifetime

lTime = []
lElectronLifetime = []
lElectronLifetimeErr = []

for file in d_electron_lifetime:
	if d_electron_lifetime[file]['run'] != run:
		continue

	lTime.append(neriX_analysis.convert_name_to_unix_time(file))
	lElectronLifetime.append(d_electron_lifetime[file]['electron_lifetime'])
	lElectronLifetimeErr.append(d_electron_lifetime[file]['electron_lifetime_err'])

lTime, lElectronLifetime, lElectronLifetimeErr = zip(*sorted(zip(lTime, lElectronLifetime, lElectronLifetimeErr)))

aTime = np.asarray(lTime, dtype='float64')
aTimeErr = np.zeros(len(lTime), dtype='float64')
aElectronLifetime = np.asarray(lElectronLifetime, dtype='float64')
aElectronLifetimeErr = np.asarray(lElectronLifetimeErr, dtype='float64')

#print aTime, aElectronLifetime, aElectronLifetimeErr

c1 = Canvas()

gElectronLifetime = root.TGraphErrors(len(lTime), aTime, aElectronLifetime, aTimeErr, aElectronLifetimeErr)

gElectronLifetime.SetTitle('Electron Lifetime in Time')
gElectronLifetime.GetXaxis().SetTimeDisplay(1)
gElectronLifetime.GetYaxis().SetTitle('Electron Lifetime [us]')
gElectronLifetime.Draw('AP')



fG1 = root.TF1('fG1', 'pol0', lTime[0], lTime[-1])
#fG1 = root.TF1('fG1', 'pol0', lTime[0], lTime[-1])

fG1.SetLineStyle(9)
fG1.SetLineWidth(1)

gElectronLifetime.Fit('fG1', 'L')

gConfIntervalShapeBot = root.TGraphAsymmErrors(
    *neriX_analysis.create_graph_with_confidence_interval_for_fit(gElectronLifetime, root.TVirtualFitter.GetFitter()))
gConfIntervalShapeBot.SetLineColor(root.kBlue)
gConfIntervalShapeBot.SetFillColor(root.kBlue)
gConfIntervalShapeBot.SetFillStyle(3005)
gConfIntervalShapeBot.Draw('3 same')

sFitInfo1 = 'Run Average e^{-} lifetime = (%.2f +/- %.2f) us' % ( fG1.GetParameter(0), fG1.GetParError(0) )
pt1 = root.TPaveText(.1,.65,.9,.9,'blNDC')
text1 = pt1.AddText(sFitInfo1)
pt1.SetTextColor(root.kBlue)
pt1.SetFillStyle(0)
pt1.SetBorderSize(0)
pt1.Draw('same')

c1.Update()


raw_input('Press enter to continue...')

neriX_analysis.save_plot(['results', 'run_%d' % run, 'time_dependence'], c1, 'time_average_electron_lifetime_run_%d' % run)

