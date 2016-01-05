import ROOT as root
import neriX_datasets
from neriX_analysis import neriX_analysis
import neriX_pmt_gain_corrections
import sys, random, string, pickle, time, os.path
from array import array
from rootpy.io import File
from rootpy.tree import Tree, TreeModel
from rootpy.plotting import Hist, Hist2D, Canvas, Legend
from rootpy.plotting.utils import draw
from rootpy import stl
import iminuit
from scipy import optimize
from math import log, sin, cos, exp, pi, factorial
import fann2
import numpy, gc
import svmutil


# MUST INCLUDE TYPES TO AVOID SEG FAULT
stl.vector(stl.vector('float'))
stl.vector(stl.vector('int'))


# --------------- END FUNCTIONS AND START MAIN CODE --------------





if len(sys.argv) != 2:
	print 'Use is python train_svm_oneclass.py <training er filename>'
	sys.exit()

filename = sys.argv[1]


MAXNUMEVENTS = 10000


currentAnalysis = neriX_analysis(filename)
currentAnalysis.set_event_list()


T1 = currentAnalysis.get_T1()
#T1.activate(['LiqSciTailRaw', 'LiqSciRaw'], exclusive=True)

# check data to see if it meets expectation
"""
c1 = Canvas()
hTrainingData = Hist2D(100, 0, 2e8, 100, -0.2, 0.7, name='hTrainingData', title='Training Data')
hTrainingData.SetMarkerStyle('smalldot')
hTrainingData.SetMarkerColor('red')

currentAnalysis.get_tree().Draw('LiqSciRaw:LiqSciTailRaw/LiqSciRaw', hist=hTrainingData)
hTrainingData.Draw()

c1.Update()
raw_input('Press enter to continue')
"""


numEvents = currentAnalysis.get_event_list().GetN()

# cap number of events
if numEvents > MAXNUMEVENTS:
	lEvents = [0 for i in xrange(MAXNUMEVENTS)]
	for i in xrange(MAXNUMEVENTS):
		lEvents[i] = random.randint(0, numEvents-1)
else:
	lEvents = range(numEvents)

# remove repeats
lEvents = list(set(lEvents))
numEvents = len(lEvents)

get_entry = T1.GetEntry
#aInput = numpy.empty([len(lEvents), 2])
#aOutput = numpy.empty([len(lEvents), 1])
aInput = [[0., 0.] for i in xrange(len(lEvents))]
aOutput = [0 for i in xrange(len(lEvents))]

lEvents.sort()

for countEvents, eventNum in enumerate(lEvents):
	if countEvents % 500 == 0:
		print 'Event ' + str(countEvents) + ' of ' + str(numEvents) + '	\r',
		sys.stdout.flush()
	
	# get current entry
	get_entry(currentAnalysis.get_event_list().GetEntry(eventNum))

	aCurrentParameter = [0., 0.]

	liqSciRaw = T1.LiqSciRaw[0]
	liqSciTailRaw = T1.LiqSciTailRaw[0]

	aCurrentParameter[0] = liqSciRaw
	aCurrentParameter[1] = liqSciTailRaw/liqSciRaw


	aInput[countEvents] = aCurrentParameter
	aOutput[countEvents] = 1

print '\n'

mOneClass = svmutil.svm_train(aOutput, aInput, '-s 2')

# +1 and -1 as output - I think +1 means it is in set
pred_labels, eval, pred_values = svmutil.svm_predict(aOutput, aInput, mOneClass)
#print pred_labels
print eval


