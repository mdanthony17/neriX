import serial, re, time, sys
import numpy as np

nb_samples = 10
ref_cap = 2.
fast = 0

serial_port = serial.Serial('/dev/ttyS3', 9600, serial.EIGHTBITS, serial.PARITY_NONE, serial.STOPBITS_ONE)

#serial_port.open()
serial_port.timeout = None
serial_port.timeout = 1

serial_port.write('@')
time.sleep(0.5)
serial_port.write('2')
time.sleep(0.5)
if fast:
	serial_port.write('f')
else:
	serial_port.write('s')
time.sleep(0.5)

vals1 = np.ndarray((nb_samples,), float)
vals2 = np.ndarray((nb_samples,), float)
ctr = 0
sys.stdout.write('      time  c1(pF)  e1(pF)  c2(pF)  e2(pF)\n')
while 1:
	serial_port.write('m')
	#time.sleep(1)
	if fast:
		reply = serial_port.read(27).strip()
	else:
		reply = serial_port.read(37).strip()

	#print 'uti replied: <%s>' % repr(reply)
	if fast:
		matches = re.match(r'^[0-9A-F]{4} [0-9A-F]{4} [0-9A-F]{4} [0-9A-F]{4}', reply)
	else:
		matches = re.match(r'^[0-9A-F]{6} [0-9A-F]{6} [0-9A-F]{6} [0-9A-F]{6}', reply)

	if matches:
		caps = map(lambda x: int(x, 16), reply.split(' '))
		#print caps
		if caps[1] != caps[0]:
				vals1[ctr % nb_samples] = float(caps[2]-caps[0])/(caps[1]-caps[0])*ref_cap
				vals2[ctr % nb_samples] = float(caps[3]-caps[0])/(caps[1]-caps[0])*ref_cap

				ctr += 1
				if ctr % nb_samples == 0 and ctr != 0:
					mu_cap1 = vals1.sum()/nb_samples
					err_cap1 = np.sqrt(np.sum((vals1-mu_cap1)**2))/np.sqrt(nb_samples)
					mu_cap2 = vals2.sum()/nb_samples
					err_cap2 = np.sqrt(np.sum((vals2-mu_cap2)**2))/np.sqrt(nb_samples)
					sys.stdout.write('%d  %0.4f  %0.4f  %0.4f  %0.4f\n' % (time.time(), mu_cap1, err_cap1, mu_cap2, err_cap2))
					sys.stdout.flush()
	else:
		# empty the buffer of whatever might be left
		serial_port.read(256)
		



