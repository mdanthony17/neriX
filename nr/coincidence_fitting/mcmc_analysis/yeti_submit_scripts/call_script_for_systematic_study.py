#!/usr/bin/python
import sys, array, os, time
from subprocess import call

lAssumedRelativeAccidentalRates = [0.01, 0.05, 0.1, 0.5, 1.0, 2.0]
lNumFakeEvents = [3000, 6000, 8000, 12000, 24000]

for rate in lAssumedRelativeAccidentalRates:
	for num in lNumFakeEvents:
		call(['qsub', '-v', "rate=%.2f,num=%d" % (rate, num), 'submit_systematic_fake_data.sh'])
		print 'Submitted for %.2f and %d events.' % (rate, num)
		time.sleep(5)



