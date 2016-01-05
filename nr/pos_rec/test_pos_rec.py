#!/usr/bin/python

import sys
import numpy as np
import fann2
#import matplotlib.pyplot as plt
import ROOT as root
from sklearn import preprocessing



maxRadius = 1
edgeCut = 0
rmsCut = 0

# create object and then functions are c functions
# without the fann_ before them

if len(sys.argv) != 2:
	print 'Use is python test_pos_rec.py <name of net (no ext)>'
	sys.exit()

filename = sys.argv[1]
ann = fann2.libfann.neural_net()

annInUse = fann2.libfann.neural_net()
annInUse.create_from_file('./nets/pr_' + filename + '.net')
print 'Neural Networks Loaded'


# test actual run using data from file
file = open('./data/' + filename + '.txt', 'r')
lFile = file.readlines()
lenFile = len(lFile)

nPMTChannels = 16

lRootMeanSquares = []
counter = 1

h1 = root.TH2F('h1', 'h1', 100, 0, 1.2, 100, 0, 1.2)
h2 = root.TH2F('h2', 'h2', 100, -1.2, 1.2, 100, -1.2, 1.2)
h3 = root.TH2F('h3', 'h3', 100, -1.2, 1.2, 100, -1.2, 1.2)
h4 = root.TH2F('h4', 'h4', 100, 0, 1.2, 100, 0, 0.2)
h5 = root.TH2F('h5', 'h5', 12, 0, 12, 100, 0, (maxRadius+1))
h6 = root.TH2F('h6', 'h6', 100, -1.2, 1.2, 100, -1.2, 1.2)

print 'start looking at file'

for index in range(1, lenFile, 2):
	linePMTValues = lFile[index]
	lLinePMTValues = linePMTValues.split()
	linePosition = lFile[index+1]
	lLinePosition = linePosition.split()
	
	lPMTs = [i for i in xrange(0, nPMTChannels)]
	lPMTValuesFull = [0 for x in xrange(0, 16)] # not normalized
	for i in xrange(0, 16):
		lPMTValuesFull[i] = float(lLinePMTValues[i])
	
	lPMTValues = preprocessing.scale(lPMTValuesFull)

	lPositions = [0 for x in xrange(0, 2)]
	for i in xrange(0, 2):
		lPositions[i] = float(lLinePosition[i])

	# edge cut
	if (lPositions[0]**2 + lPositions[1]**2)**0.5 < edgeCut:
		continue

	lNetPositions = annInUse.run(lPMTValues)

	for i in xrange(0, 2):
		lNetPositions[i] *= maxRadius

	rootMeanSquare = ( (lPositions[0] - lNetPositions[0])**2 + (lPositions[1] - lNetPositions[1])**2 )**0.5

	# RMS cut
	if rootMeanSquare < rmsCut:
		continue
	
	# S2 ratio cut
	# cut and complement make sense: inner vs outer may now be
	# separated using the S2 only :)!
	#if (lPMTValuesOrdered[nPMTChannels-9])/(lPMTValuesOrdered[nPMTChannels-2]) < 0.42:
	#	continue

	h1.Fill(( (lPositions[0])**2 + (lPositions[1])**2 )**0.5, ( (lNetPositions[0])**2 + (lNetPositions[1])**2 )**0.5)
	h2.Fill(lPositions[0], lNetPositions[0])
	h3.Fill(lPositions[1], lNetPositions[1])
	h4.Fill(((lPositions[0])**2 + (lPositions[1])**2 )**0.5, rootMeanSquare)
	#h5.Fill(maxIndex, ((lNetPositions[0])**2 + (lNetPositions[1])**2 )**0.5)
	h6.Fill(lNetPositions[0], lNetPositions[1])

	#lRootMeanSquares.append(rootMeanSquare)
	"""
	print 'Test Number ' + str(counter)
	print 'Actual Position: ' + str(lPositions[0]) + '\t' + str(lPositions[1]) + '\n'
	print 'ANN Reconstructed Position: ' + str(lNetPositions[0]) + '\t' + str(lNetPositions[1]) + '\n'
	print 'Root Mean Square = ' + str(rootMeanSquare)
	print '\n\n\n'
	"""
	counter += 1



#print 'Mean of root mean squares = ' + str(np.mean(lRootMeanSquares))
#print 'Standard Deviation of root mean squares = ' + str(np.std(lRootMeanSquares))

h1.SetTitle('Radial Reconstruction, 12 Subdivisions')
h1.GetXaxis().SetTitle('Simulated Radius (mm)')
h1.GetYaxis().SetTitle('Reconstructed Radius (mm)')

c1 = root.TCanvas('c1','c1',200,200,1000,1000)
line1 = root.TLine(0,0,22,22)
line1.SetLineColor(root.kRed)
line1.SetLineWidth(4)
h1.Draw('colz')
line1.Draw('same')
h1.SetStats(0)
c1.SetGrid()
c1.Update()

h2.SetTitle('X Reconstruction, 12 Subdivisions')
h2.GetXaxis().SetTitle('Simulated X (mm)')
h2.GetYaxis().SetTitle('Reconstructed X (mm)')

c2 = root.TCanvas('c2','c2',200,200,1000,1000)
line2 = root.TLine(-22,-22,22,22)
line2.SetLineColor(root.kRed)
line2.SetLineWidth(4)
h2.Draw('colz')
line2.Draw('same')
h2.SetStats(0)
c2.SetGrid()
c2.Update()

h3.SetTitle('Y Reconstruction, 12 Subdivisions')
h3.GetXaxis().SetTitle('Simulated Y (mm)')
h3.GetYaxis().SetTitle('Reconstructed Y (mm)')

c3 = root.TCanvas('c3','c3',200,200,1000,1000)
line3 = root.TLine(-22,-22,22,22)
line3.SetLineColor(root.kRed)
line3.SetLineWidth(4)
h3.Draw('colz')
line3.Draw('same')
h3.SetStats(0)
c3.SetGrid()
c3.Update()

h4.SetTitle('Radial Reconstruction, 12 Subdivisions')
h4.GetXaxis().SetTitle('Radial Reconstruction (X**2 + Y**2)**0.5 (mm)')
h4.GetYaxis().SetTitle('Distance Between Reconstructed and Simulated')

c4 = root.TCanvas('c4','c4',200,200,1000,1000)
h4.Draw('colz')
h4.SetStats(1)
c4.SetGrid()
c4.Update()

c5 = root.TCanvas('c5','c5',200,200,1000,1000)
h5.Draw('colz')
h5.SetStats(0)
c5.SetGrid()
c5.Update()

c6 = root.TCanvas('c6','c6',200,200,1000,1000)
h6.Draw('colz')
h6.SetStats(0)
c6.SetGrid()
c6.Update()


c1.Print('./test_images/radial_reconstruction.png')
c2.Print('./test_images/x_reconstruction.png')
c3.Print('./test_images/y_reconstruction.png')
c4.Print('./test_images/radial_reconstruction_vs_rms.png')

#hist = np.histogram(lRootMeanSquares, 100, (0, 3))
#plt.hist(lRootMeanSquares, 50, (0, 5))
#plt.show()

# end actual test of run function

annInUse.destroy()

raw_input("Enter to continue...")