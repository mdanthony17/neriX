import ROOT as root
import neriX_datasets
from neriX_analysis import neriX_analysis
import numpy as np
import sys
from sklearn import preprocessing

#----- CREATE DICTIONARY -----

if len(sys.argv) != 2:
	print 'Use is python create_data_dictionary.py <name of text file>'
	sys.exit()

pathToData = './preprocessed_data/'
pathToProcessedData = './data/'

numTopPMTs = 16

filename = sys.argv[1]
#assert filename[-4:] == '.txt'

dPosRec = {}

with open(pathToData + 'pp_' + filename + '.txt', 'r') as fTextFile:
	for line in fTextFile:
		line = line[:-1]
		lLine = line.split()

		if len(lLine) == 3:
			continue
		elif len(lLine) == numTopPMTs:
			lTopPMTs = np.array([ float(x) for x in lLine ])
			continue

		# all that is left is position line
		lPos = tuple([ float(x) for x in lLine ])

		dPosRec[lPos] = lTopPMTs


# ----- COMPLETE CREATE DICTIONARY -----


# distances in mm
lowerX = -20.9
lowerY = -20.9
upperX = -1.*lowerX
upperY = -1.*lowerY

radiusSquared = lowerX**2

xIndex = 0
yIndex = 1

desiredResolution = 0.5
desiredResolutionSquared = 0.5**2

numStepsX = int((upperX - lowerX) / desiredResolution)
numStepsY = int((upperY - lowerY) / desiredResolution)

dFinalPosRecForm = {}

for i, xCenter in enumerate(np.linspace(lowerX, upperX, numStepsX)):
	for j, yCenter in enumerate(np.linspace(lowerY, upperY, numStepsY)):
		# check that point is in detector
		if (xCenter**2 + yCenter**2) > radiusSquared:
			continue

		aS2s = np.zeros(numTopPMTs)
		for tPos in dPosRec:
			if ( (tPos[xIndex]-xCenter)**2 + (tPos[yIndex]-yCenter)**2 ) < desiredResolutionSquared:
				aS2s += dPosRec[tPos]

		if not all(aS2s):
			continue

		aS2sScaled = preprocessing.scale(aS2s)

		dFinalPosRecForm[(xCenter/upperX, yCenter/upperY)] = aS2sScaled

	if i % 10 == 0:
		print 'Completed iteration ' + str(i) + ' / ' + str(numStepsX)


with open(pathToProcessedData + filename + '.txt', 'w') as fFinalData:
	numberOfPoints = len(dFinalPosRecForm)
	# write first line of file
	fFinalData.write(str(numberOfPoints) + '\t' + str(numTopPMTs) + '\t2\n')

	for tPos in dFinalPosRecForm:
		sPos = ''.join(str(entry) + '\t' for entry in tPos)
		sPos = list(sPos)
		sPos[-1] = '\n'
		sPos = ''.join(sPos)
		sS2s = ''.join(str(entry) + '\t' for entry in dFinalPosRecForm[tPos])
		sS2s = list(sS2s)
		sS2s[-1] = '\n'
		sS2s = ''.join(sS2s)

		fFinalData.write(sS2s)
		fFinalData.write(sPos)









