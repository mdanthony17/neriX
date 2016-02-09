#!/usr/bin/python

version = '0.3.0'

#log_file = '/var/log/smacd.log'
#pid_file = '/var/run/smacd.pid'
#config_file = '/etc/smacd.conf'
log_file = 'smacd.log'
pid_file = 'smacd.pid'
config_file = 'smacd.conf'

import sys, os, signal, time, re, shutil, socket, copy, serial, threading, Queue, MySQLdb

def error(message):
	print '%s error: %s' % (time.asctime(), message)

def warning(message):
	print '%s warning: %s' % (time.asctime(), message)

def debug(message):
	print '%s debug: %s' % (time.asctime(), message)

# Repeater
class Repeater(threading.Thread):
	def __init__(self, function, repeat_interval):
		threading.Thread.__init__(self)
		self.function = function
		self.repeat_interval = repeat_interval
		self.done = threading.Event()
	
	def run(self):
		if self.repeat_interval:
			while not self.done.is_set():
				self.function()
				self.done.wait(self.repeat_interval)
	
	def stop(self):
		self.done.set()

# RemoteCommand
class RemoteCommand:
	def __init__(self, command_string):
		self.command_string = command_string
		self.parse()
	
	def __str__(self):
		return self.command_string

	def parse(self):
		remote_command_regexp = '^(?P<instrument_name>\w+)>(?P<task>\w+(?:\:\w+)*)(?:\s*(?P<args>.+))?$'
		match = re.match(remote_command_regexp, self.command_string)
		if match:
			self.instrument_name = match.group('instrument_name')
			self.task = match.group('task')
			if match.group('args'):
				self.args = tuple(match.group('args').split())
			else:
				self.args = ()
		else:
			raise ValueError

# ProcessValue
class ProcessValue:
	def __init__(self, time, value, unit):
		self.time = time
		self.value = value
		self.unit = unit
	
	def __str__(self):
		return '<%d, %0.2f (%s)>' % (self.time, self.value, self.unit)

# Reading
class Reading:
	def __init__(self, instrument_name):
		self.instrument_name = instrument_name
		self.process_values_map = {}
	
	def __str__(self):
		reading_str = '%s:  ' % self.instrument_name
		process_values = self.process_values_map.items()
		if len(process_values):
			reading_str += '%d' % process_values[0][1].time
			for channel_name, process_value in process_values:
				reading_str += ', %s' % channel_name
				if process_value.unit:
					reading_str += '(%s)' % process_value.unit
				if isinstance(process_value.value, str):
					reading_str += ':%s' % process_value.value
				elif isinstance(process_value.value, int) or isinstance(process_value.value, bool):
					reading_str += ':%d' % process_value.value
				else:
					if abs(process_value.value) < 0.01 and process_value.value != 0:
						reading_str += ':%0.2e' % process_value.value
					else:
						reading_str += ':%0.2f' % process_value.value

		return reading_str

	def __getitem__(self, channel_name):
		return self.process_values_map[channel_name]
	
	def add(self, channel_name, process_value):
		self.process_values_map[channel_name] = process_value

	def matches(self, subscription):
		if self.instrument_name == subscription.instrument_name:
			if subscription.channel_name in self.process_values_map.keys():
				return True
			else:
				return False
		else:
			return False

# AlarmCondition
class AlarmCondition:
	def __init__(self, condition_string):
		self.condition_string = condition_string
		self.parse()

	def __str__(self):
		return self.condition_string

	def parse(self):
		condition_regexp = '^(?P<instrument_name>\w+)\{(?P<channel_name>\w+)\((?P<unit>[A-Za-z%]*)\)\}\s*(?P<operator>[<>])\s*(?P<limit>[+-]?\d+(\.\d+)?)$'
		match = re.match(condition_regexp, self.condition_string)
		if match:
			self.instrument_name = match.group('instrument_name')
			self.channel_name = match.group('channel_name')
			self.unit = match.group('unit')
			self.operator = {'>':lambda x, y: x > y, '<':lambda x, y: x < y}[match.group('operator')]
			#debug('AlarmCondition::parse: operator=%s' % match.group('operator'))
			self.limit = float(match.group('limit'))
			#debug('AlarmCondition::parse: limit=%f' % self.limit)
		else:
			raise ValueError

	def occurs(self, reading):
		#debug('AlarmCondition::occurs: self.instrument_name(%s) == reading.instrument_name(%s)' % (self.instrument_name, reading.instrument_name))
		if self.instrument_name == reading.instrument_name:
			try:
				process_value = reading[self.channel_name]
				#debug('AlarmCondition::occurs: process_value(%s)' % process_value)
				if self.operator(process_value.value, self.limit):
					return True
				else:
					return False
			except KeyError:
				return False
		else:
			return False

	def cleared(self, reading):
		if self.instrument_name == reading.instrument_name:
			try:
				process_value = reading[self.channel_name]
				if not self.operator(process_value.value, self.limit):
					return True
				else:
					return False
			except KeyError:
				return False
		else:
			return False

# Subscription
class Subscription:
	def __init__(self, subscription_string):
		self.subscription_string = subscription_string
		self.parse()

	def __str__(self):
		return self.subscription_string
	
	def parse(self):
		subscription_regexp = '^(?P<instrument_name>\w+)\{(?P<channel_name>\w+)\((?P<unit>[A-Za-z%]*)\)\}$'
		match = re.match(subscription_regexp, self.subscription_string)
		if match:
			self.instrument_name = match.group('instrument_name')
			self.channel_name = match.group('channel_name')
			self.unit = match.group('unit')
		else:
			raise ValueError

# StatusParameter
class StatusParameter(Subscription):
	def __init__(self, status_parameter_string):
		Subscription.__init__(self, status_parameter_string)
	
# PidController
class PidController:
	def __init__(self, setpoint, kp=0., ki=0., kd=0.):
		self.setpoint = setpoint
		self.kp = kp
		self.ki = ki
		self.kd = kd
		self.last_error = 0.
		self.last_time = time.time()
		self.integral = 0.

	def get_output(self, pv):
		error = self.setpoint-pv.value
		dt = pv.time-self.last_time
		debug('pid_controller: error=%f dt=%f' % (error, dt))

		pterm = self.kp*error
		iterm = self.ki*(self.integral+error*dt)
		dterm = self.kd*(float(error)/dt)
		output = pterm+iterm+dterm
		debug('pid_controller: pterm=%f iterm=%f dterm=%f' % (pterm, iterm, dterm))

		self.integral += error*dt
		self.last_error = error
		self.last_time = pv.time
		debug('pid_controller: integral=%f last_error=%f last_time=%f' % (self.integral, self.last_error, self.last_time))

		if output < 0.:
			return 0.
		elif output > 1.:
			return 1.
		else:
			return output

# Instrument
class Instrument:
	instrument_registry = {}
	def __init__(self, name, read_interval):
		self.name = name
		self.read_interval = read_interval
		self.connected = False
		self.serial_port = serial.Serial()
		self.serial_lock = threading.Lock()
		self.in_queue = Queue.Queue()
		self.out_queue = Queue.Queue()
		self.in_queue_size_max_size = 100
		self.alive = True
		self.dispatch_table = {}
		self.dispatch_table['connect'] = self._connect
		self.dispatch_table['read:start'] = self._read_start
		self.dispatch_table['read:stop'] = self._read_stop
		self.dispatch_table['subscription:data'] = self._subscription_data
		self.dispatcher_dead_time = 0.5
		self.dispatcher_thread = threading.Thread(target=self.dispatcher)
		self.dispatcher_thread.start()
		self.reader_thread = None
		self.subscribers = {}
		self.subscriptions = {}
		Instrument.instrument_registry[name] = self

	def channels(self):
		return []

#    @staticmethod
#    def get_instrument(name):
#        return Instrument.instrument_registry[name]

	def subscribe(self, subscription):
		self.subscriptions[str(subscription)] = []
		# check in the instrument list if the instrument already exists
		if subscription.instrument_name in Instrument.instrument_registry:
			instrument = Instrument.instrument_registry[subscription.instrument_name]
			if subscription.channel_name not in instrument.subscribers:
				instrument.subscribers[subscription.channel_name] = [self.in_queue]
			else:
				instrument.subscribers[subscription.channel_name].append(self.in_queue)

	def unsubscribe(self, subscription):
		if str(subscription) in self.subscriptions:
			del self.subscriptions[str(subscription)]

			# check in the instrument list if the instrument still exists
			if subscription.instrument_name in Instrument.instrument_registry:
				instrument = Instrument.instrument_registry[subscription.instrument_name]
				if subscription.channel_name in instrument.subscribers:
					if self.in_queue in instrument.subscribers[subscription.channel_name]:
						instrument.subscribers[subscription.channel_name].remove(self.in_queue)
	
	def distribute_to_subscribers(self, reading):
		for channel_name in self.subscribers.keys():
			for subscriber_queue in self.subscribers[channel_name]:
				if subscriber_queue.qsize() < self.in_queue_size_max_size:
					subscriber_queue.put(('subscription:data', (copy.deepcopy(reading),)))
	
	def set_serial_parameters(self, port=None, baudrate=9600, bytesize=serial.EIGHTBITS, parity=serial.PARITY_NONE, stopbits=serial.STOPBITS_ONE, timeout=None, xonxoff=0, rtscts=0, interCharTimeout=None):
		self.serial_port.port = port
		self.serial_port.baudrate = baudrate
		self.serial_port.bytesize = bytesize
		self.serial_port.parity = parity
		self.serial_port.stopbits = stopbits
		self.serial_port.timeout = timeout
		self.serial_port.xonxoff = xonxoff
		self.serial_port.rtscts = rtscts
		self.serial_port.interCharTimeout = interCharTimeout
	
	def dispatcher(self):
		while self.alive:
#            debug('instrument %s dispatcher running' % self.name)
			# if there is something in the inbound queue, call the appropriate function
			if not self.in_queue.empty():
				task, args = self.in_queue.get()
				if task in self.dispatch_table:
#                    debug('dispatching task ' + task + ' with args ' + str(args))
					apply(self.dispatch_table[task], args)
				else:
					warning('ignoring unimplememented task %s for instrument %s' % (task, self.name))
			else:
				time.sleep(self.dispatcher_dead_time)

	def _connect(self):
		pass
	
	def _read_start(self):
		if self.connected:
			self.reader_thread = Repeater(self._read_channels, self.read_interval)
			self.reader_thread.start()

	def _read_stop(self):
		if self.connected:
			self.reader_thread.stop()
			self.reader_thread.join()
			self.reader_thread = None
	
	def _read_channels(self):
		pass

	def _subscription_data(self, reading):
		for subscription_string in self.subscriptions.keys():
			subscription = Subscription(subscription_string)
			if reading.matches(subscription):
				debug('yes, we are subscribing to ' + subscription_string)
				self.subscriptions[str(subscription)].append(reading[subscription.channel_name])
			debug('subscriptions of intrument %s are %s' % (self.name, str(self.subscriptions[str(subscription)])))

	def command(self, task, args=()):
		self.in_queue.put((task, args))

	def connect(self):
		self.in_queue.put(('connect', ()))
	
	def read_start(self):
		self.in_queue.put(('read:start', ()))
	
	def read_stop(self):
		self.in_queue.put(('read:stop', ()))
	
	def get_reading(self):
		try:
			next_reading = self.out_queue.get(block=False)
		except Queue.Empty:
			next_reading = None
		return next_reading

	def disconnect(self):
		while self.reader_thread:
			pass
		self.connected = False
		self.serial_port.close()
		self.alive = False
		self.dispatcher_thread.join()

# Superlogics8017 analog to digital converter
class Superlogics8017(Instrument):
	def __init__(self, name, read_interval, address):
		Instrument.__init__(self, name, read_interval)
		self.address = address
		self.inputs = 8*[None]

	def channels(self):
		channels_description = []
		for input_pars in self.inputs:
			if input_pars:
				channels_description.append({'name':input_pars['name'], 'unit':input_pars['unit'], 'mysql_type':input_pars['mysql_type']})

		return channels_description
	
	def hex_to_dec(self, x):
		val = int(x,16)
		if val > 32767:
			val -= 65536
		return val

	def add_input(self, input_nb, channel_name, unit, slope, offset, mysql_type=''):
		self.inputs[input_nb] = {'name':channel_name, 'unit':unit, 'slope':slope, 'offset':offset, 'mysql_type':mysql_type}
	
	def _connect(self):
		# connect to the serial port
		try:
			self.serial_port.open()
		except serial.SerialException:
			error('could not open port to instrument %s' % self.name)
		else:
			# verify that we indeed have a superlogics8017 at the rs485 address
			self.serial_lock.acquire()
			self.serial_port.write('$%0.2xM\r' % self.address)
			self.serial_port.flush()
			reply = self.serial_port.read(8)
			self.serial_lock.release()

#            debug('module replied <%s>' % repr(reply))
			if reply[3:7] == '8017':
				self.connected = True

			# configure the module to the desired mode

	def _read_channels(self):
		self.serial_lock.acquire()
		self.serial_port.write('$%0.2xA\r' % self.address)
		self.serial_port.flush()
		now = time.time()
		reply = self.serial_port.read(34)
		self.serial_lock.release()

		if len(reply) != 34:
			error('superlogics8017 reply different than expected')
		else:
			data = reply[1:-1]
			voltages = [10.*self.hex_to_dec(data[i:i+4])/32767. for i in xrange(0, len(data), 4)]

			reading = Reading(self.name)
			for input_nb, input_pars in enumerate(self.inputs):
				if input_pars:
					value = voltages[input_nb]*input_pars['slope']+input_pars['offset']
					process_value = ProcessValue(now, value, input_pars['unit'])
					reading.add(input_pars['name'], process_value)

			self.out_queue.put(reading)
			self.distribute_to_subscribers(reading)

# AgilentE3633 power supply
class AgilentE3633(Instrument):
	def __init__(self, name, read_interval):
		Instrument.__init__(self, name, read_interval)
		self.dispatch_table['voltage:set'] = self._voltage_set

	def channels(self):
		channels_description = []

		return channels_description
	
	def _connect(self):
		# connect to the serial port
		try:
			self.serial_port.open()
		except serial.SerialException:
			error('could not open port to instrument %s' % self.name)

	def _read_channels(self):
		self.serial_lock.acquire()
		now = time.time()
		self.serial_lock.release()

		reading = Reading(self.name)
		# process_value = ProcessValue(now, )
		# reading.add('V_heater', process_value)
		self.out_queue.put(reading)
		self.distribute_to_subscribers(reading)
	
	def _voltage_set(self, voltage_string):
		try:
			voltage = float(voltage_string)
		except ValueError:
			error('%s: could not convert parameter to float in command voltage:set' % self.name)
		else:
			self.serial_lock.acquire()
#            self.serial_port.write()
			self.serial_lock.release()

# LakeShore340 temperature controller
class LakeShore340(Instrument):
	def __init__(self, name, read_interval):
		Instrument.__init__(self, name, read_interval)
		self.inputs = {'A':None, 'B':None, 'C1':None, 'C2':None, 'C3':None, 'C4':None, 'D1':None, 'D2':None, 'D3':None, 'D4':None}
	
	def channels(self):
		channels_description = []
		channels_description.append({'name':'Q_heater', 'unit':'%', 'mysql_type':'FLOAT(5,2)'})
		for input_pars in self.inputs.values():
			if input_pars:
				channels_description.append({'name':input_pars['name'], 'unit':input_pars['unit'], 'mysql_type':input_pars['mysql_type']})

		return channels_description

	def add_input(self, input_name, channel_name, unit, mysql_type=''):
		self.inputs[input_name] = {'name':channel_name, 'unit':unit, 'mysql_type':mysql_type}

	def read_heater(self):
		# read heater
		self.serial_lock.acquire()
		self.serial_port.write('HTR?\r')
		self.serial_port.flush()
		now = time.time()
		reply = self.serial_port.read(9)
		self.serial_lock.release()

#        debug('module replied <%s>' % repr(reply))
		if len(reply) != 9:
			error('lakeshore340 reply different than expected')
			process_value = None
		else:
			value = float(reply[:-1])
			process_value = ProcessValue(now, value, '%')

		return process_value

	def read_input(self, input_name, input_pars):
		# read input (A, B, etc)
		self.serial_lock.acquire()
		self.serial_port.write('CRDG? %s\r' % input_name)
		self.serial_port.flush()
		now = time.time()
		reply = self.serial_port.read(12)
		self.serial_lock.release()

#        debug('module replied <%s>' % repr(reply))
		if len(reply) != 12:
			error('lakeshore340 reply different than expected')
			process_value = None
		else:
			value = float(reply[:-1])
			process_value = ProcessValue(now, value, input_pars['unit'])

		return process_value
	
	def _connect(self):
		# connect to the serial port
		try:
			self.serial_port.open()
		except serial.SerialException:
			error('could not open port to instrument %s' % self.name)
		else:
			# verify that we indeed have a LakeShore 340 on that port
			self.serial_lock.acquire()
			self.serial_port.write('*IDN?\r')
			self.serial_port.flush()
			reply = self.serial_port.read(28)
			self.serial_lock.release()

			if reply[5:13] == 'MODEL340':
				self.connected = True

	def _read_channels(self):
		reading = Reading(self.name)

		heater_process_value = self.read_heater()
		if heater_process_value:
			reading.add('Q_heater', heater_process_value)

		for input_name, input_pars in self.inputs.iteritems():
			if input_pars:
				input_process_value = self.read_input(input_name, input_pars)
				if input_process_value:
					reading.add(input_pars['name'], input_process_value)

		if reading.process_values_map:
			self.out_queue.put(reading)
			self.distribute_to_subscribers(reading)

# PfeifferTPG261 single gauge
class PfeifferTPG261(Instrument):
	def __init__(self, name, read_interval):
		Instrument.__init__(self, name, read_interval)
		self.inputs = 2*[None]
	
	def channels(self):
		channels_description = []
		for input_pars in self.inputs:
			if input_pars:
				channels_description.append({'name':input_pars['name'], 'unit':input_pars['unit'], 'mysql_type':input_pars['mysql_type']})

		return channels_description

	def add_input(self, input_name, channel_name, unit, mysql_type=''):
		self.inputs[input_name] = {'name':channel_name, 'unit':unit, 'mysql_type':mysql_type}

	def _connect(self):
		# connect to the serial port
		try:
			self.serial_port.open()
		except serial.SerialException:
			error('could not open port to instrument %s' % self.name)
		else:
			# verify that we indeed have a Pfeiffer TPG261 on that port
			self.serial_lock.acquire()
			self.serial_port.write('TID\r')
			self.serial_port.flush()
			reply = self.serial_port.read(3)
			self.serial_lock.release()

			#debug('module replied <%s>' % repr(reply))
			if reply != '\x06\r\n':
				error('no ACK received from Pfeiffer TPG261 after TID command!')
				return

			self.serial_lock.acquire()
			self.serial_port.timeout = 1.
			self.serial_port.write('\x05')
			self.serial_port.flush()
			reply = self.serial_port.read(13)
			self.serial_port.timeout = None
			self.serial_lock.release()

			#debug('module replied <%s>' % repr(reply))
			if reply[:-2].split(',')[0] == 'PKR':
				self.connected = True

			# configure the pressure unit to Torr
			self.serial_lock.acquire()
			self.serial_port.write('UNI,1\r')
			self.serial_port.flush()
			reply = self.serial_port.read(3)
			self.serial_lock.release()

			#debug('module replied <%s>' % repr(reply))
			if reply != '\x06\r\n':
				error('no ACK received from Pfeiffer TPG261 after UNI command!')
				self.connected = False
				return

			self.serial_lock.acquire()
			self.serial_port.write('\x05')
			self.serial_port.flush()
			reply = self.serial_port.read(3)
			self.serial_lock.release()

			#debug('module replied <%s>' % repr(reply))
			if reply != '1\r\n':
				error('could not set Pfeiffer TPG261 pressure unit to Torr!')
				self.connected = False
				return

	def _read_channels(self):
		self.serial_lock.acquire()
		self.serial_port.write('PRX\r')
		self.serial_port.flush()
		reply = self.serial_port.read(3)
		self.serial_lock.release()

		#debug('module replied <%s>' % repr(reply))
		if reply != '\x06\r\n':
			error('no ACK received from Pfeiffer TPG261 after PRX command!')
			return

		self.serial_lock.acquire()
		self.serial_port.write('\x05')
		self.serial_port.flush()
		now = time.time()
		reply = self.serial_port.read(29)
		self.serial_lock.release()

		#debug('module replied <%s>' % repr(reply))
		if len(reply) != 29:
			error('Pfeiffer TPG261 reply different than expected!')
			process_value = None
		else:
			status = map(int, reply[:-2].split(',')[0:3:2])
			pressures = map(float, reply[:-2].split(',')[1:4:2])

			reading = Reading(self.name)
			for input_nb, input_pars in enumerate(self.inputs):
				if input_pars:
					if status[input_nb] == 0:
						value = pressures[input_nb]
						process_value = ProcessValue(now, value, input_pars['unit'])
						reading.add(input_pars['name'], process_value)
					elif status[input_nb] == 1:
						warning('%s: pressure reading under range' % self.name)
						value = 0.
					elif status[input_nb] == 2:
						warning('%s: pressure reading over range' % self.name)
						value = 760.
					process_value = ProcessValue(now, value, input_pars['unit'])
					reading.add(input_pars['name'], process_value)

			self.out_queue.put(reading)
			self.distribute_to_subscribers(reading)

# APC SmartUPS
class APCSmartUPS(Instrument):
	def __init__(self, name, read_interval):
		Instrument.__init__(self, name, read_interval)
	
	def channels(self):
		channels_description = []
		channels_description.append({'name':'P_load', 'unit':'VA', 'mysql_type':'FLOAT(6,2)'})
		channels_description.append({'name':'Q_battery', 'unit':'%', 'mysql_type':'FLOAT(5,2)'})
		channels_description.append({'name':'B_ac_online', 'unit':'', 'mysql_type':'BOOL'})
		channels_description.append({'name':'B_battery_online', 'unit':'', 'mysql_type':'BOOL'})
		channels_description.append({'name':'B_low_battery', 'unit':'', 'mysql_type':'BOOL'})

		return channels_description

	def _connect(self):
		# connect to the serial port
		try:
			self.serial_port.open()
		except serial.SerialException:
			error('could not open port to instrument %s' % self.name)
		else:
			# set the SmartUPS into 'Smart' mode
			self.serial_lock.acquire()
			self.serial_port.timeout = 1.
			self.serial_port.write('Y')
			self.serial_port.flush()
			reply = self.serial_port.read(4)
			self.serial_port.timeout = None
			self.serial_lock.release()

			#debug('ups replied <%s>' % repr(reply))
			if reply != 'SM\r\n':
				error('no ACK received from APC SmartUPS after \'Y\' command!')
				return

			self.connected = True

	def read_parameter(self, command, reply_regexp, unit):
		# read load
		self.serial_lock.acquire()
		self.serial_port.timeout = 1.
		self.serial_port.write(command)
		self.serial_port.flush()
		now = time.time()
		reply = self.serial_port.read(8)
		self.serial_port.timeout = None
		self.serial_lock.release()

		#debug('ups replied <%s>' % repr(reply))
		if not re.search(reply_regexp, reply.strip()):
			error('APC SmartUPS reply different than expected')
			process_value = None
		else:
			value = float(reply.strip())
			process_value = ProcessValue(now, value, unit)

		return process_value

	def read_status(self):
		# read status
		self.serial_lock.acquire()
		self.serial_port.timeout = 1.
		self.serial_port.write('Q')
		self.serial_port.flush()
		now = time.time()
		reply = self.serial_port.read(8)
		self.serial_port.timeout = None
		self.serial_lock.release()

		#debug('ups replied <%s>' % repr(reply))
		if not re.search(r'^\d\d$', reply.strip()):
			error('APC SmartUPS reply different than expected')
			process_value = None
		else:
			value = int(reply.strip(), 16)
			process_value = ProcessValue(now, value, '')

		return process_value

	def _read_channels(self):
		reading = Reading(self.name)

		# check whether we switched to battery
		self.serial_lock.acquire()
		self.serial_port.timeout = 1.
		now = time.time()
		reply = self.serial_port.read(256)
		self.serial_port.timeout = None
		self.serial_lock.release()

		# read load
		load_process_value = self.read_parameter('P', r'^\d+\.\d+$', 'VA')
		if load_process_value:
			reading.add('P_load', load_process_value)

		# read battery charge
		battery_process_value = self.read_parameter('f', r'^\d+\.\d+$', '%')
		if battery_process_value:
			reading.add('Q_battery', battery_process_value)

		# read status
		status = self.read_status()
		if status:
			reading.add('B_ac_online', ProcessValue(status.time, bool(status.value & 0x08), ''))
			reading.add('B_battery_online', ProcessValue(status.time, bool(status.value & 0x10), ''))
			reading.add('B_low_battery', ProcessValue(status.time, bool(status.value & 0x40), ''))

		if reading.process_values_map:
			self.out_queue.put(reading)
			self.distribute_to_subscribers(reading)

class VirtualThermostat(Instrument):
	def __init__(self, name, control_interval, temperature_input, setpoint, control_knob):
		Instrument.__init__(self, name, read_interval=0)
		self.control_interval = control_interval
		self.temperature_input = temperature_input
		self.control_knob = control_knob
		self.pid_controller = PidController(setpoint)
		self.controller_thread = None
		self.dispatch_table['setpoint:set'] = self._setpoint_set
		self.dispatch_table['pid:set'] = self._pid_set
		self.dispatch_table['control:start'] = self._control_start
		self.dispatch_table['control:stop'] = self._control_stop

	def _setpoint_set(self, setpoint):
		debug('setpoint set to %f' % setpoint)
		self.pid_controller.setpoint = setpoint

	def _pid_set(self, kp_string, ki_string, kd_string):
		try:
			kp = float(kp_string)
			ki = float(ki_string)
			kd = float(kd_string)
		except ValueError:
			error('%s could not convert parameter to float in command pid:set' % self.name)
		else:
			debug('pid parameters set to: kp=%f, ki=%f, kd=%f' % (kp, ki, kd))
			self.pid_controller.kp = kp
			self.pid_controller.ki = ki
			self.pid_controller.kd = kd
	
	def _control_start(self):
		self.subscribe(Subscription(self.temperature_input))
		self.controller_thread = Repeater(self.control, self.control_interval)
		self.controller_thread.start()
		debug('%s is now controlling %s to %f' % (self.name, self.temperature_input, self.pid_controller.setpoint))

	def _control_stop(self):
		self.unsubscribe(Subscription(self.temperature_input))
		self.controller_thread.stop()
		self.controller_thread = None
		debug('%s is not controlling %s anymore' % (self.name, self.temperature_input))
	
	def control(self):
		while 1:
			try:
				process_value = self.subscriptions[self.temperature_input].pop(0)
			except IndexError:
				break

			output = self.pid_controller.get_output(process_value)
			self.control_knob(output)
	
# Recorder
class Recorder():
	def __init__(self, database_user, database_pass):
		self.database_name = 'smac'
		self.database_user = database_user
		self.database_pass = database_pass
		self.instruments = {}
		instruments_lock.acquire()
		for instrument in Instrument.instrument_registry.values():
			self.instruments[instrument.name] = {}
			self.instruments[instrument.name]['channels'] = instrument.channels()
		instruments_lock.release()
		self.recorder_thread = None
		self.polling_interval = 10
		self.in_queue = Queue.Queue()

		self.create_tables()
	
	def create_tables(self):
		connection = MySQLdb.connect('localhost', self.database_user, self.database_pass, self.database_name)
		cursor = connection.cursor()

		for instrument_name, instrument_pars in self.instruments.items():
			# check if the table exists
			if not cursor.execute('SHOW TABLES LIKE "%s";' % instrument_name):
				# the table does not exist, create it
				sql_create_statement = ''
				sql_create_statement += 'CREATE TABLE %s (' % instrument_name
				sql_create_statement += '%s_id INT(10) UNSIGNED NOT NULL DEFAULT 0,' % instrument_name
				for channel_pars in instrument_pars['channels']:
					if channel_pars['mysql_type']:
						sql_create_statement += '%s %s DEFAULT NULL, ' % (channel_pars['name'], channel_pars['mysql_type'])
					else:
						sql_create_statement += '%s FLOAT DEFAULT NULL, ' % channel_pars['name']
				sql_create_statement += 'PRIMARY KEY (%s_id)' % instrument_name
				sql_create_statement += ');'

				print '%s creating new table for instrument %s, ' % (time.asctime(), instrument_name),
				print '\'%s\'' % sql_create_statement
				cursor.execute(sql_create_statement)
			else:
				# the table exists, check whether it contains all the columns needed
				for channel_pars in instrument_pars['channels']:
					if not cursor.execute('SHOW COLUMNS FROM %s LIKE "%s";' % (instrument_name, channel_pars['name'])):
						sql_alter_statement = ''
						sql_alter_statement += 'ALTER TABLE %s' % instrument_name
						if channel_pars['mysql_type']:
							sql_alter_statement += ' ADD COLUMN %s %s DEFAULT NULL;' % (channel_pars['name'], channel_pars['mysql_type'])
						else:
							sql_alter_statement += ' ADD COLUMN %s FLOAT DEFAULT NULL;' % channel_pars['name']
						print '%s changing table for instrument %s, ' % (time.asctime(), instrument_name),
						print '\'%s\'' % sql_alter_statement
						cursor.execute(sql_alter_statement)
				
		cursor.close()
		connection.close()

	def subscribe_to_all(self):
		instruments_lock.acquire()
		for instrument in Instrument.instrument_registry.values():
			# subscribe to the first channel of the instrument, we will get the full reading anyway
			subscription_channel_name = iter(instrument.channels()).next()['name']

			if subscription_channel_name not in instrument.subscribers:
				instrument.subscribers[subscription_channel_name] = [self.in_queue]
			else:
				instrument.subscribers[subscription_channel_name].append(self.in_queue)
		instruments_lock.release()

	def unsubscribe_from_all(self):
		pass

	def start(self):
		self.subscribe_to_all()
		self.recorder_thread = Repeater(self.record, self.polling_interval)
		self.recorder_thread.start()

	def stop(self):
		self.unsubscribe_from_all()
		self.recorder_thread.stop()
		self.recorder_thread = None

	def record(self):
		sql_insert_statements = []
		while 1:
			try:
				task, args = self.in_queue.get(False)
			except Queue.Empty:
				break

			if task == 'subscription:data':
				reading = args[0]

				if reading.instrument_name in self.instruments:
					sql_insert_statement = ''
					sql_insert_statement += 'INSERT INTO %s (' % reading.instrument_name
					sql_insert_statement += '%s_id, ' % reading.instrument_name
					sql_insert_statement += ', '.join(reading.process_values_map.keys())
					sql_insert_statement += ') '
					sql_insert_statement += 'VALUES '
					sql_insert_statement += '('
					sql_insert_statement += '%d, ' % iter(reading.process_values_map.values()).next().time
					sql_insert_statement += (', '.join(len(reading.process_values_map)*['%f'])) % tuple([p.value for p in reading.process_values_map.values()])
					sql_insert_statement += ');'

#					print sql_insert_statement

					sql_insert_statements.append(sql_insert_statement)
			else:
				error('recorder recieved invalid command!')

		# connect to the database and record the data in it
		if sql_insert_statements:
			connection = MySQLdb.connect('localhost', self.database_user, self.database_pass, self.database_name)
			cursor = connection.cursor()
			for sql_insert_statement in sql_insert_statements:
				try:
					cursor.execute(sql_insert_statement)
					connection.commit()
				except MySQLdb.IntegrityError:
					error('failed to excute insert statement \'%s\'' % sql_insert_statement)

			cursor.close()
			connection.close()

# Logger
class Logger:
	def __init__(self, f):
		self.f = f
	
	def write(self, s):
		self.f.write(s)
		self.f.flush()

def sigterm_handler(signum, frame):
	# stop instruments
	instruments_lock.acquire()
	for instrument in instruments:
		instrument.read_stop()
		instrument.disconnect()
	instruments_lock.release()

def read_config():
	global smacd_gid, smacd_uid
	global polling_interval, zombie_interval, status_email_interval, alarm_repeat_interval 
	global email_sender, smtp_server, smtp_port, smtp_tls, smtp_user, smtp_pass
	global email_recipients
	global experiment_name
	global status_parameters, alarm_conditions
	global recorder_user, recorder_pass

	# regular expressions for the config file
	smacd_uid_regexp = r'^smacd_uid\s*=\s*(\d+)'
	smacd_gid_regexp = r'^smacd_gid\s*=\s*(\d+)'
	zombie_interval_regexp = r'^zombie_interval\s*=\s*(\d+)'
	status_email_interval_regexp = r'^status_email_interval\s*=\s*(\d+)'
	alarm_repeat_interval_regexp = r'^alarm_repeat_interval\s*=\s*(\d+)'
	email_sender_regexp = r'^email_sender\s*=\s*"([A-Za-z0-9\.-]+@[A-Za-z0-9\.-]+)"'
	smtp_server_regexp = r'^smtp_server\s*=\s*"([A-Za-z0-9\.-]+)"'
	smtp_port_regexp = r'^smtp_port\s*=\s*(\d+)'
	smtp_tls_regexp = r'^smtp_tls\s*=\s*([01])'
	smtp_user_regexp = r'^smtp_user\s*=\s*"([A-Za-z0-9\.-]+@[A-Za-z0-9\.-]+)"'
	smtp_pass_regexp = r'^smtp_pass\s*=\s*"(.+)"'
	email_recipients_regexp = r'^email_recipients\s*=\s*((?:"[A-Za-z0-9\.-]+@[A-Za-z0-9\.-]+"\s*,\s*)*(?:"[A-Za-z0-9\.-]+@[A-Za-z0-9\.-]+"))'
	experiment_name_regexp = r'^experiment_name\s*=\s*"([A-Za-z0-9\.-_]+)"'
	status_parameters_regexp = r'^status_parameters\s*\+=\s*"([A-Za-z0-9\.-_]+\{[A-Za-z0-9\.-_]+\([A-Za-z%]*\)\})"'
	alarm_conditions_regexp = r'^alarm_conditions\s*\+=\s*"([A-Za-z0-9\.-_]+\{[A-Za-z0-9\.-_]+\([A-Za-z%]*\)\}\s*[<>]\s*[-+]?\d+(?:\.\d+))"'
	recorder_user_regexp = r'^recorder_user\s*=\s*"(.+)"'
	recorder_pass_regexp = r'^recorder_pass\s*=\s*"(.+)"'

	email_recipients = []
	status_parameters = []
	alarm_conditions = []
	soft_alarm_conditions = ['adc0{P_chamber(Bar)} > 1.0', 'adc0{P_chamber(Bar)} > 1.2', 'adc0{P_chamber(Bar)} > 1.6', 'adc0{P_chamber(Bar)} > 1.65', 'adc0{P_chamber(Bar)} > 1.7', 'adc0{P_chamber(Bar)} > 1.75', 'caen0{PMT1_VMon(V)} < 0.9*caen0{PMT1_V0Set(V)}', 'caen0{PMT2_VMon(V)} < 0.9*caen0{PMT2_V0Set(V)}', 'caen0{PMT3_VMon(V)} < 0.9*caen0{PMT3_V0Set(V)}', 'caen0{PMT4_VMon(V)} < 0.9*caen0{PMT4_V0Set(V)}', 'caen0{PMT5_VMon(V)} < 0.9*caen0{PMT5_V0Set(V)}', 'caen0{PMT6_VMon(V)} < 0.9*caen0{PMT6_V0Set(V)}']

	config_lines = open(config_file).readlines()

	for config_line in config_lines:
		if not re.search('^(\s*#|\s*$)', config_line):
			# smacd_uid
			if re.search(smacd_uid_regexp, config_line):
				smacd_uid = int(re.search(smacd_uid_regexp, config_line).group(1))
			# smacd_gid
			elif re.search(smacd_gid_regexp, config_line):
				smacd_gid = int(re.search(smacd_gid_regexp, config_line).group(1))
			# zombie_interval
			elif re.search(zombie_interval_regexp, config_line):
				zombie_interval = int(re.search(zombie_interval_regexp, config_line).group(1))
			# status_email_interval
			elif re.search(status_email_interval_regexp, config_line):
				status_email_interval = int(re.search(status_email_interval_regexp, config_line).group(1))
			# alarm_repeat_interval
			elif re.search(alarm_repeat_interval_regexp, config_line):
				alarm_repeat_interval = int(re.search(alarm_repeat_interval_regexp, config_line).group(1))
			# email_sender
			elif re.search(email_sender_regexp, config_line):
				email_sender = re.search(email_sender_regexp, config_line).group(1)
			# smtp_server
			elif re.search(smtp_server_regexp, config_line):
				smtp_server = re.search(smtp_server_regexp, config_line).group(1)
			# smtp_port
			elif re.search(smtp_port_regexp, config_line):
				smtp_port = int(re.search(smtp_port_regexp, config_line).group(1))
			# smtp_tls
			elif re.search(smtp_tls_regexp, config_line):
				smtp_tls = int(re.search(smtp_tls_regexp, config_line).group(1))
			# smtp_user
			elif re.search(smtp_user_regexp, config_line):
				smtp_user = re.search(smtp_user_regexp, config_line).group(1)
			# smtp_pass
			elif re.search(smtp_pass_regexp, config_line):
				smtp_pass = re.search(smtp_pass_regexp, config_line).group(1)
			# email_recipients
			elif re.search(email_recipients_regexp, config_line):
				email_recipients = re.search(email_recipients_regexp, config_line).group(1).replace(' ', '').replace('"', '').split(',')
			# experiment_name
			elif re.search(experiment_name_regexp, config_line):
				experiment_name = re.search(experiment_name_regexp, config_line).group(1)
			# status_parameters
			elif re.search(status_parameters_regexp, config_line):
				status_parameter_string = re.search(status_parameters_regexp, config_line).group(1)
				status_parameters.append(StatusParameter(status_parameter_string))
			# alarm_conditions
			elif re.search(alarm_conditions_regexp, config_line):
				alarm_condition_string = re.search(alarm_conditions_regexp, config_line).group(1)
				alarm_conditions.append(AlarmCondition(alarm_condition_string))
			# recorder_user
			elif re.search(recorder_user_regexp, config_line):
				recorder_user = re.search(recorder_user_regexp, config_line).group(1)
			# recorder_pass
			elif re.search(recorder_pass_regexp, config_line):
				recorder_pass = re.search(recorder_pass_regexp, config_line).group(1)
			else:
				print >>sys.stderr, 'error: invalid configuration,', config_line.strip()
				sys.exit(1)

def send_email(msg):
	import smtplib

	session = smtplib.SMTP(smtp_server, smtp_port)
	session.ehlo()
	if smtp_tls:
		session.starttls()
		session.ehlo()
		session.login(smtp_user, smtp_pass)

	result = session.sendmail(email_sender, email_recipients, msg)

	if result:
		for recipient in result.keys():
			print '%s could not deliver mail to: %s,' % (time.asctime(), recipient),
			print 'server replied: %s %s' % (result[recipient][0], result[recipient][1])

	session.quit()

def get_parameters_status():
	status = 'Status of %s Parameters\n\n' % (experiment_name)

	# get the last reading of all status parameters
	for status_parameter in status_parameters:
		last_reading_lock.acquire()
		reading = last_reading[status_parameter.instrument_name]
		try:
			process_value = reading[status_parameter.channel_name]
		except KeyError:
			error('could not get last reading from parameter "%s"' % status_parameter)
			status += '%s ' % (time.asctime())
			status += '%s error, could not read last value\n' % (status_parameter.channel_name)
		else:
			status += '%s ' % (time.asctime(time.localtime(process_value.time)))
			status += '%s ' % (status_parameter.channel_name)
			if isinstance(process_value.value, str):
				status += '%s' % process_value.value
			elif isinstance(process_value.value, int) or isinstance(process_value.value, bool):
				status += '%d' % process_value.value
			else:
				if abs(process_value.value) < 0.001 and process_value.value != 0:
					status += '%0.4e' % process_value.value
				else:
					status += '%0.4f' % process_value.value
			if process_value.unit:
				status += ' %s' % process_value.unit
			status += '\n'
		last_reading_lock.release()

	return status

def send_status_msg():
	# build status message
	status_msg = ''
	status_msg += 'From: %s\n' % email_sender
	status_msg += 'To: %s\n' % ', '.join(email_recipients)
	status_msg += 'Subject: [%s] %s Status\n' % (socket.getfqdn(), experiment_name)

	status_msg += get_parameters_status()

	debug('sending status message')
#    print status_msg
	send_email(status_msg)

def send_alarm_msg():
	# build alarm message
	alarm_msg = ''
	alarm_msg += 'From: %s\n' % email_sender
	alarm_msg += 'To: %s\n' % ', '.join(email_recipients)
	alarm_msg += 'Subject: [%s] %s Alarm\n' % (socket.getfqdn(), experiment_name)

	# print the alarm condition
	alarm_msg += 'ALARM -- ALARM -- ALARM\n'
	last_reading_lock.acquire()
	for alarm_condition in alarm_conditions:
		reading = last_reading[alarm_condition.instrument_name]
		now = time.time()
		last_time = (reading.process_values_map.values()[0]).time
		if alarm_condition.occurs(reading):
			alarm_msg += str(alarm_condition)+'\n'
		if (now-last_time) > zombie_interval:
			alarm_msg += 'Zombie instrument: '+alarm_condition.instrument_name+'\n'
	last_reading_lock.release()
	alarm_msg += '\n'

	alarm_msg += get_parameters_status()

	warning('alarm condition, sending alarm message')
#    print alarm_msg
	send_email(alarm_msg)

# Commander
class Commander:
	def __init__(self, socket_name):
		self.socket_name = socket_name
		self.listen = False
		self.listener_thread = threading.Thread(target=self.listener)
		self.command_queue = Queue.Queue()
		self.reply_queue = Queue.Queue()
	
	def start(self):
		self.listen = True
		self.listener_thread.start()
	
	def listener(self):
		self.socket = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
		try:
			os.remove(self.socket_name)
		except OSError:
			pass
		self.socket.bind(self.socket_name)
		self.socket.settimeout(2)
		self.socket.listen(1)

		while self.listen:
			try:
				connection, address = self.socket.accept()
				connection.settimeout(None)
			except socket.timeout:
				continue
			while 1:
				data = connection.recv(1024)
				if not data:
					break
				else:
					try:
						command = RemoteCommand(data)
					except ValueError:
						error('remote command has a syntax error!')
					else:
						debug('received remote command ' + str(command))
						self.command_queue.put(command)

	def get_command(self):
		try:
			next_command = self.command_queue.get(block=False)
		except Queue.Empty:
			next_command = None
		return next_command	
	
	def command_done(self):
		pass
	
	def stop(self):
		self.listen = False
		self.socket.close()
		self.listener_thread.join()

def smacd_main():
	# redirect outputs to a log file
	sys.stdout = sys.stderr = Logger(open(log_file, 'a+'))

	# make the daemon run as a normal user
	os.setegid(smacd_gid)
	os.seteuid(smacd_uid)

	# print start time of daemon
	print '%s smacd daemon started,' % time.asctime(),
	print 'uid: %d, gid: %d,' % (smacd_uid, smacd_gid),
	print 'status_email_interval: %d' % status_email_interval,
	print 'alarm_repeat_interval: %d\n' % alarm_repeat_interval,

	# create the desired instruments
	global instruments, instruments_lock
	instruments = []
	instruments_lock = threading.Lock()

	# create a global last reading for all instruments
	global last_reading, last_reading_lock
	last_reading = {}
	last_reading_lock = threading.Lock()

	# create the Superlogics8017 ADC
	adc0 = Superlogics8017('adc0', 60., 0x02)
	adc0.set_serial_parameters('/dev/ttyS0', 9600, serial.EIGHTBITS, serial.PARITY_NONE, serial.STOPBITS_ONE)
	adc0.add_input(0, 'P_chamber', 'Bar', 2.069, -0.05, 'FLOAT(5,4)')
	#adc0.add_input(1, '', '', 1., 0.)
	#adc0.add_input(2, 'I_beam', 'uA', 100., 0., 'FLOAT(5,2)')
	adc0.add_input(3, 'T_top', 'C', 25., -150., 'FLOAT(5,2)')
	adc0.add_input(4, 'Q_circulation', 'SLPM', 3.4, 0., 'FLOAT(5,3)')
	adc0.add_input(5, 'T_ambient', 'C', 37.538, -61.111, 'FLOAT(5,2)')
	instruments_lock.acquire()
	instruments.append(adc0)
	instruments_lock.release()

	#adc1 = Superlogics8017('adc1', 60., 0x02)
	#adc1.set_serial_parameters('/dev/ttyS0', 9600, serial.EIGHTBITS, serial.PARITY_NONE, serial.STOPBITS_ONE)
	#adc1.add_input(0, 'I_beam', 'uA', 300., 0.)
	#instruments_lock.acquire()
	#instruments.append(adc1)
	#instruments_lock.release()

	# create the LakeShore340 temperature controller
	lakeshore0 = LakeShore340('lakeshore0', 30.)
	lakeshore0.set_serial_parameters('/dev/ttyUSB0', 9600, serial.EIGHTBITS, serial.PARITY_NONE, serial.STOPBITS_ONE)
	lakeshore0.add_input('A', 'T_ptr', 'C', 'FLOAT(7,4)')
	lakeshore0.add_input('B', 'T_coldfinger', 'C', 'FLOAT(7,4)')
	instruments_lock.acquire()
	instruments.append(lakeshore0)
	instruments_lock.release()

	# create the Pfeiffer TPG261 gauge controller
#    pfeiffer0 = PfeifferTPG261('pfeiffer0', 30.)
#    pfeiffer0.set_serial_parameters('/dev/ttyS4', 9600, serial.EIGHTBITS, serial.PARITY_NONE, serial.STOPBITS_ONE)
#    pfeiffer0.add_input(0, 'P_insulation', 'Torr', 'FLOAT')
#	pfeiffer0.add_input(0, 'P_chamber', 'Torr', 'FLOAT')
#    instruments_lock.acquire()
#    instruments.append(pfeiffer0)
#    instruments_lock.release()

	# create the APC SmartUPS
	apc0 = APCSmartUPS('apc0', 30.)
	apc0.set_serial_parameters('/dev/ttyS2', 2400, serial.EIGHTBITS, serial.PARITY_NONE, serial.STOPBITS_ONE)
	instruments_lock.acquire()
	instruments.append(apc0)
	instruments_lock.release()

	# check which polling interval to use and connect the instruments
	polling_interval = 60.
	instruments_lock.acquire()
	for instrument in instruments:
		if instrument.read_interval < polling_interval:
			polling_interval = instrument.read_interval
		instrument.connect()
	polling_interval *= 0.5	
	instruments_lock.release()

	# tell instruments to start reading
	instruments_lock.acquire()
	for instrument in instruments:
		instrument.read_start()
	instruments_lock.release()


	# we need to wait until we have some readings to start the status email repeater
	status_repeater = 0
	
	# leave the zombie and alarm repeater out of the game for now
	zombie_repeater = 0
	alarm_repeater = 0

	# create and start the command listener
	commander = Commander('/tmp/smacd.socket')
	commander.start()

	# create and start the database data recorder
	recorder = Recorder(recorder_user, recorder_pass)
	recorder.start()

	# check if some commands should be sent to instruments and get the readings
	while 1:
		time.sleep(polling_interval)

		# check if we still have instruments alive
		instruments_alive = False
		instruments_lock.acquire()
		for instrument in instruments:
			if instrument.alive:
				instruments_alive = True
		instruments_lock.release()
		# if not, exit
		if not instruments_alive:
			commander.stop()
			recorder.stop()
			if zombie_repeater:
				zombie_repeater.stop()
				zombie_repeater.join()
			if status_repeater:
				status_repeater.stop()
				status_repeater.join()
			if alarm_repeater:
				alarm_repeater.stop()
				alarm_repeater.join()

			print '%s smacd stopped' % time.asctime()
			os.remove(pid_file)
			sys.exit(0)

		# check for commands
		remote_command = commander.get_command()
		instruments_lock.acquire()
		if remote_command:
			if remote_command.instrument_name in Instrument.instrument_registry:
				instrument = Instrument.instrument_registry[remote_command.instrument_name]
				instrument.command(remote_command.task, remote_command.args)
		instruments_lock.release()

		# checking for instrument readings
		instruments_lock.acquire()
		for instrument in instruments:
			reading = instrument.get_reading()
			if reading:
				print reading

				# store the last reading
				last_reading_lock.acquire()
				last_reading[instrument.name] = reading
				last_reading_lock.release()

				# if an alarm condition occurs, start a repeating alarm
				for alarm_condition in alarm_conditions:
					now = time.time()
					last_time = (reading.process_values_map.values()[0]).time

					if ((now-last_time) > zombie_interval) and not zombie_repeater:
						if email_recipients:
							warning('zombie instrument, starting zombie repeater')
							zombie_repeater = Repeater(send_alarm_msg, alarm_repeat_interval)
							zombie_repeater.start()
						else:
							warning('zombie instrument, no zombie repeater started (no email recipients)')
					elif zombie_repeater and ((now-last_time) < zombie_interval):
						zombie_repeater.stop()
						zombie_repeater = 0

					if alarm_condition.occurs(reading) and not alarm_repeater:
						if email_recipients:
							warning('alarm occured, starting alarm repeater')
							alarm_repeater = Repeater(send_alarm_msg, alarm_repeat_interval)
							alarm_repeater.start()
						else:
							warning('alarm occured, no alarm repeater started (no email recipients)')
					elif alarm_repeater and alarm_condition.cleared(reading):
						alarm_repeater.stop()
						alarm_repeater = 0
		instruments_lock.release()

		# start a status email repeater if we have readings for all instruments
		if not status_repeater:
			all_readings = True
			instruments_lock.acquire()
			for instrument in instruments:
				last_reading_lock.acquire()
				if instrument.name not in last_reading:
					all_readings = False
				last_reading_lock.release()
			instruments_lock.release()
			if all_readings and email_recipients:
#                last_reading_lock.acquire()
#                for instrument in last_reading:
#                    debug('last_reading = %s' % str(last_reading[instrument]))
#                last_reading_lock.release()
				status_repeater = Repeater(send_status_msg, status_email_interval)
				status_repeater.start()

def smacd_start():
	# check that smacd is not already running
	if os.path.exists(pid_file):
		# the pid_file exists, get the pid from it
		pid = int(open(pid_file, 'r').read().strip())

		# check if there is a process named smacd with that pid
		cmd = 'ps -p %s --no-heading' % pid
		cmd_output = os.popen(cmd).read().strip().split()
		if cmd_output and cmd_output[-1] == 'smacd':
			print >>sys.stderr, 'error: smacd is already running!'
			sys.exit(0)
		else:
			# the pid doesn't exist or is not for smacd, remove the pid_file and go on
			os.remove(pid_file)

	# read the config file
	read_config()

	# do the UNIX double-fork magic, see Stevens' "Advanced
	# Programming in the UNIX Environment" for details (ISBN 0201563177)
	try:
		pid = os.fork()
		if pid > 0:
			# exit first parent
			sys.exit(0)

	except OSError, e:
		print >>sys.stderr, 'fork #1 failed: %d (%s)' % (e.errno, e.strerror)
		sys.exit(1)

	# decouple from parent environment
#    os.chdir('/')
	os.setsid()
	os.umask(0)

	# do second fork
	try:
		pid = os.fork()
		if pid > 0:
			# exit from second parent, print eventual PID before
			open(pid_file, 'w').write('%d' % pid)
			sys.exit(0)

	except OSError, e:
		print >>sys.stderr, 'fork #2 failed: %d (%s)' % (e.errno, e.strerror)
		sys.exit(1)
	
	# register a SIGTERM handler
	signal.signal(signal.SIGTERM, sigterm_handler)

	# start the daemon main loop
	smacd_main()

def smacd_stop():
	if os.path.exists(pid_file):
		pid = int(open(pid_file, 'r').read().strip())
		try:
			os.kill(pid, signal.SIGTERM)
		except OSError:
			os.remove(pid_file)
		sys.exit(0)
	else:
		print >>sys.stderr, 'error: smacd is not running!'
		sys.exit(1)

if __name__ == '__main__':

	if len(sys.argv) < 2:
		print >>sys.stderr, 'usage: smacd [start|stop|restart]'
		sys.exit(1)

	cmd = sys.argv[1]

	if cmd == 'start':
		smacd_start()
	elif cmd == 'stop':
		smacd_stop()
	elif cmd == 'restart':
		smacd_stop()
		smacd_start()
	else:
		print >>sys.stderr, 'usage: smacd [start|stop|restart]'
		sys.exit(1)

