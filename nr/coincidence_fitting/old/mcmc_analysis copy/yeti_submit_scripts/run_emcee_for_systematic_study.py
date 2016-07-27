#!/usr/bin/python
import sys, array, os, time
from subprocess import call

lAssumedRelativeAccidentalRates = [0.01, 0.05, 0.1, 0.5, 1.0, 2.0]
lNumFakeEvents = [100, 700, 1300, 2000, 5000, 10000]
#lAssumedRelativeAccidentalRates = [0.1]
#lNumFakeEvents = [100]

for rate in lAssumedRelativeAccidentalRates:
	for num in lNumFakeEvents:
		call(['qsub', '-v', "rate=%.2f,num=%d" % (rate, num), 'submit_systematic_fake_data.sh'])
		print 'Submitted for %.2f and %d events.' % (rate, num)
		time.sleep(5)



