#!/usr/bin/python

import ROOT as root
from rootpy import stl
from rootpy.io import File
from rootpy.tree import Tree, TreeModel, TreeChain
from rootpy.plotting import Hist, Hist2D, Canvas, Graph, func
import MySQLdb

import numpy as np
import matplotlib.pyplot as plt
import math

RUNNB = 10
deviceName = 'run' + str(RUNNB) + 'pmt0'
measurementName = 'PMT Gain'

sqlConnect = MySQLdb.connect('astro18.astronevis.columbia.edu', 'smac_reader', 'wq12op09', 'smac')
sqlCursor = sqlConnect.cursor()


sqlCursor.execute('select * from pi0 where pi0_id > 1448482991 and pi0_id < 1453321409;')
deviceData = sqlCursor.fetchall()

lTime = []
lRate = []


for i, row in enumerate(deviceData):
	if float(row[1]) < 800:
		continue
	lTime.append(float(row[0]))
	lRate.append(float(row[1]))

numPoints = len(lTime)
aTime = np.asarray(lTime)
aRate = np.asarray(lRate)

c1 = Canvas()

gRate = root.TGraph(numPoints, aTime, aRate)
gRate.Draw('AP')


c1.Update()

raw_input('Press enter to continue...')


