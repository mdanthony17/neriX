#!/usr/bin/python
import sys, array, os
sys.path.insert(0, '..')

import ROOT as root
import neriX_analysis, neriX_simulation_analysis
from rootpy.plotting import Hist, Hist2D, Canvas, Legend
import neriX_simulation_config, string
import numpy as np
import cPickle as pickle

if len(sys.argv) != 6 and len(sys.argv) != 7:
	print 'Use is python perform_full_matching.py <degree> <cathode> <anode> <relative accidental rate> <num fake events> [name_notes]'
	sys.exit()



degreeSetting = int(sys.argv[1])
cathodeSetting = float(sys.argv[2])
anodeSetting = float(sys.argv[3])
relativeAccidentalRate = float(sys.argv[4])
numFakeEvents = int(sys.argv[5])


if len(sys.argv) == 7:
	name_notes = sys.argv[len(sys.argv) - 1]
else:
	name_notes = None


full_matching_analysis = neriX_simulation_analysis.neriX_simulation_analysis(15, anodeSetting, cathodeSetting, degreeSetting, create_fake_data=True, assumeRelativeAccidentalRate=relativeAccidentalRate, num_fake_events=numFakeEvents, name_notes=name_notes)
full_matching_analysis.create_fake_data(7.43, 5.5, 0.3, 0.05, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)





