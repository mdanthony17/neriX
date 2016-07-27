#!/usr/bin/python
import sys, array, os, time
from subprocess import call

if len(sys.argv) != 4 and len(sys.argv) != 5:
	print 'Use is python create_fake_data.py <degree> <cathode> <anode> [name_notes]'
	sys.exit()

degreeSetting = sys.argv[1]
cathodeSetting = sys.argv[2]
anodeSetting = sys.argv[3]

lAssumedRelativeAccidentalRates = [0.01, 0.05, 0.1, 0.5, 1.0, 2.0]
lNumFakeEvents = [100, 700, 1300, 2000, 5000, 10000]

if len(sys.argv) == 5:
	name_notes = sys.argv[len(sys.argv) - 1]
else:
	name_notes = None

for rate in lAssumedRelativeAccidentalRates:
	for num in lNumFakeEvents:
		if len(sys.argv) == 5:
			call(['python', 'create_fake_data.py', degreeSetting, cathodeSetting, anodeSetting, str(rate), str(num), name_notes])
		else:
			call(['python', 'create_fake_data.py', degreeSetting, cathodeSetting, anodeSetting, str(rate), str(num)])
		print 'Submitted for %.2f and %d events.' % (rate, num)
		time.sleep(5)



