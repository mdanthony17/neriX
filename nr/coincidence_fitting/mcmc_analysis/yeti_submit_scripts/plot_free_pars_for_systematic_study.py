#!/usr/bin/python
import sys, array, os, time
from subprocess import call

lAssumedRelativeAccidentalRates = [0.01, 0.05, 0.1, 0.5, 1.0, 2.0]
lNumFakeEvents = [400, 700, 1500, 3000, 6000]

for rate in lAssumedRelativeAccidentalRates:
	for num in lNumFakeEvents:
		call(['qsub', '-v', "rate=%.2f,num=%d" % (rate, num), 'submit_systematic_plotting.sh'])
		print 'Submitted for %.2f and %d events.' % (rate, num)
		time.sleep(5)



