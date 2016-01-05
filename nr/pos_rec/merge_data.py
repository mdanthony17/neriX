#!/usr/bin/python

#!/usr/bin/python

from subprocess import check_output
import sys

#lPMTEfficiencies = [0.77,0.93,0.79,1.00,0.84,0.86,0.95,1.00,0.84,0.87,0.89,1.00,0.95,0.94,1.00,0.96]
lPMTEfficiencies = [1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,]

if len(sys.argv) != 2:
	print 'Use is python merge_data.py <merged filename>'
	sys.exit()

filename = sys.argv[1]

lFilesToMerge = check_output(['ls', './raw_data/']).split('\n')
lFilesToMerge = lFilesToMerge[:-1]

newFile = open('./preprocessed_data/pp_' + filename + '.txt', 'w')

countNumberTrainingPts = 0

lFileLines = []
lMasterList = []

for fileName in lFilesToMerge:
	currentFile = open('./raw_data/' + fileName, 'r')
	lFileLines.append(currentFile.readlines())

numberOfPoints = len(lFileLines[0])

# remember to delete .data files

for i in xrange(0, numberOfPoints):
	if i % 10000 == 0:
		print 'Processing point ' + str(i) + ' out of ' + str(numberOfPoints)
	
	lMasterList.append([])

	for j in xrange(0, len(lFileLines)):
		lFileLines[j][i] = lFileLines[j][i][:-1]
		lFileLines[j][i] = lFileLines[j][i].split('\t')

	lMasterList[i].append(lFileLines[0][i][0])
	lMasterList[i].append(lFileLines[0][i][1])

	sumOfAllPMTs = 0
	for j in xrange(2, 18):
		sumOverFilesForSinglePMT = 0
		for k in xrange(0, len(lFileLines)):
			sumOverFilesForSinglePMT += float(lFileLines[k][i][j])
		lMasterList[i].append(sumOverFilesForSinglePMT)
		sumOfAllPMTs += sumOverFilesForSinglePMT

	for j in xrange(2, 18):
		lMasterList[i][j] = (lMasterList[i][j]*lPMTEfficiencies[j-2])# / sumOfAllPMTs


sFirstLine = str(numberOfPoints) + '\t16\t2\n'
newFile.write(sFirstLine)

print 'Creating new file'

for i in xrange(0, numberOfPoints):
	if i % 10000 == 0:
		print 'Printing point ' + str(i) + ' out of ' + str(numberOfPoints)

	sPosValues = str(lMasterList[i][0]) + '\t' + str(lMasterList[i][1]) + '\n'
	sPMTValues = ''
	for j in xrange(2, 17):
		sPMTValues += str(lMasterList[i][j]) + '\t'
	sPMTValues += str(lMasterList[i][17]) + '\n'

	newFile.write(sPMTValues)
	newFile.write(sPosValues)

print 'Finished creating file'
newFile.close()
