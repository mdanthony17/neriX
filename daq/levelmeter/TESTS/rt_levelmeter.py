import serial, re, time

nb_samples = 10
#ref_cap = 0.996 * 2
ref_cap = 12 
fast = 0

serial_port = serial.Serial('/dev/ttyUSB1', 9600, serial.EIGHTBITS, serial.PARITY_NONE, serial.STOPBITS_ONE)

serial_port.open()
serial_port.timeout = None
serial_port.timeout = 1

serial_port.write('@')
# Measuring mode
serial_port.write('0')
if fast:
	serial_port.write('f')
else:
	serial_port.write('s')

vals = []
ctr = 0
while 1:
	serial_port.write('m')
	#time.sleep(1)
	if fast:
		reply = serial_port.read(27).strip()
	else:
		reply = serial_port.read(37).strip()

#	print 'uti replied: <%s>' % repr(reply)
	if fast:
		matches = re.match(r'^[0-9A-F]{4} [0-9A-F]{4} [0-9A-F]{4} [0-9A-F]{4}', reply)
	else:
		matches = re.match(r'^[0-9A-F]{6} [0-9A-F]{6} [0-9A-F]{6} [0-9A-F]{6}', reply)

	if matches:
		caps = map(lambda x: int(x, 16), reply.split(' '))
		#print caps
		vals.append(float(caps[2]-caps[0])/(caps[1]-caps[0]))

		ctr += 1
		if ctr % nb_samples == 0 and ctr != 0:
			#print vals
#			print caps[0], caps[1], caps[2]
			print sum(vals)/nb_samples*ref_cap
			vals = []


