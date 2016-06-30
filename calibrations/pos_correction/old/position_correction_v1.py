#!/usr/bin/python

import ROOT as root
import sys
import neriX_analysis
from rootpy.plotting import Hist2D, Hist, Legend, Canvas
import numpy as np
from math import exp, log
import tqdm
from root_numpy import tree2array
import scipy.optimize as op

from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt


#--------------- Start Parameters to Change ----------------


s1NumBins = 40
s1Min = 0
s1Max = 2000


s2NumBins = 40
s2Min = 0
s2Max = 900e3

zNumBins = 24
zMin = -24
zMax = 0

rNumBins = 20
rMin = 0
rMax = 1


numBinsCES = 50
upperBoundCES = 1200.
secondParameter = 'S2sTotBottom[0]'


#--------------- End Parameters to Change ----------------



if(len(sys.argv) != 2):
	print 'Usage is python coin_analysis <ROOT filename>'
	sys.exit(1)

file1 = sys.argv[1]
currentAnalysis = neriX_analysis.neriX_analysis(file1)


#choose options
option1 = 'COLZ'
optionSame = 'SAME'


#choose cuts for run 1
currentAnalysis.add_z_cut()
currentAnalysis.add_single_scatter_cut()
#currentAnalysis.add_cut('S2sTotBottom[0] > 300e3')
#currentAnalysis.add_cut('(s1asym > 0)')
currentAnalysis.add_radius_cut(0, 0.85)

# cut on 40 keV peak
#currentAnalysis.add_cut('S1sTotBottom[0] > 80 && S1sTotBottom[0] < 380')
#currentAnalysis.add_cut('S2sTotTop[0] > 20e3 && S2sTotTop[0] < 70e3')

# cut on 164 keV peak
#currentAnalysis.add_cut('S1sTotBottom[0] > 400 && S1sTotBottom[0] < 900')
#currentAnalysis.add_cut('S2sTotBottom[0] > 200e3 && S2sTotBottom[0] < 400e3')

#currentAnalysis.get_T1().Scan('EventId:S2sTot[0]:S2sSaturation[0]', currentAnalysis.get_cuts())



# ---------- MAKE CES CUT ----------

g1 = 0.095 #0.14 #0.1
g2 = 17. #26.2 #24.
sigmaCES = 1.

cCES = Canvas(width=900, height=700, name='cCES')
cCES.SetGridx()
cCES.SetGridy()

hCES = Hist(numBinsCES, 0, upperBoundCES, name='hCES', title='hCES', drawstyle='hist')
currentAnalysis.Draw('0.0137*(S1sTotBottom[0]/%f + %s/%f)' % (g1, secondParameter, g2), hist=hCES, selection=currentAnalysis.get_cuts())

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

lbCES = fGausCES.GetParameter(1) - sigmaCES*fGausCES.GetParameter(2)
ubCES = fGausCES.GetParameter(1) + sigmaCES*fGausCES.GetParameter(2)
raw_input('Fit okay?')


sCES = '(0.0137*(S1sTotBottom[0]/%f + %s/%f) > %f && 0.0137*(S1sTotBottom[0]/%f + %s/%f) < %f)' % (g1, secondParameter, g2, lbCES, g1, secondParameter, g2, ubCES)
currentAnalysis.add_cut(sCES)
#currentAnalysis.set_event_list()

del hCES
del cCES




currentAnalysis.set_event_list()


aBranches = tree2array(currentAnalysis.get_T1(), [currentAnalysis.get_T1().GetAlias('Z'), currentAnalysis.get_T1().GetAlias('R') + '^2', 'S1sTotBottom[0]', 'S2sTotBottom[0]'], selection=currentAnalysis.get_cuts())

lColumnNames = aBranches.dtype.names
meanS1 = aBranches[lColumnNames[2]].mean()
meanS2 = aBranches[lColumnNames[3]].mean()
print meanS1, meanS2
aBranches[lColumnNames[2]] /= meanS1
aBranches[lColumnNames[3]] /= meanS2
print aBranches[lColumnNames[3]]


fig = plt.figure()
ax_s1 = fig.add_subplot(211, projection='3d')
ax_s2 = fig.add_subplot(212, projection='3d')

ax_s1.scatter(aBranches[lColumnNames[0]], aBranches[lColumnNames[1]], aBranches[lColumnNames[2]])
ax_s2.scatter(aBranches[lColumnNames[0]], aBranches[lColumnNames[1]], aBranches[lColumnNames[3]])




def ln_like(fit_parameters, x1, x2, y):
	a, b, c, lnf = fit_parameters
	
	if lnf > 1. or lnf < -10:
		return -np.inf
	
	y_model = a*x1 + b*x2 + c

	"""
	# quick check if guesses are any good
	print '\n\n\n'
	print y.mean()
	print y_model.mean()
	"""
	
	try:
		sigma2 = np.exp(2*lnf) * y_model**2 # sigma^2 = f^2*y^2
		return -0.5 * np.sum(( y-y_model)**2/sigma2 - np.log(1./sigma2) )
	except:
		return -np.inf


# guesses from good fits
s1_guesses = [-0.0230391, -0.06627135, 0.70814616, -1.75355416]
s2_guesses = [ 0.01633487, 0.14299838, 1.17308076, -1.64798265]

neg_ln_like = lambda *args: -ln_like(*args)
result_s1 = op.minimize(neg_ln_like, s1_guesses, args=(aBranches[lColumnNames[0]], aBranches[lColumnNames[1]], aBranches[lColumnNames[2]]))
#print result_s1['x']


Z_surf = np.linspace(zMin, zMax, zNumBins)
R2_surf = np.linspace(rMin**2, rMax**2, rNumBins)
Z_surf, R2_surf = np.meshgrid(Z_surf, R2_surf)
S1_surf = result_s1['x'][0]*Z_surf + result_s1['x'][1]*R2_surf + result_s1['x'][2]

ax_s1.plot_surface(Z_surf, R2_surf, S1_surf, alpha=0.2, color=[0,1,0])



result_s2 = op.minimize(neg_ln_like, s2_guesses, args=(aBranches[lColumnNames[0]], aBranches[lColumnNames[1]], aBranches[lColumnNames[3]]))
#print result_s2['x']

S2_surf = result_s2['x'][0]*Z_surf + result_s2['x'][1]*R2_surf + result_s2['x'][2]

ax_s2.plot_surface(Z_surf, R2_surf, S2_surf, alpha=0.2, color=[0,1,0])


plt.show()



