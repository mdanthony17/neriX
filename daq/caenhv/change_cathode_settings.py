import libcaenhv, time
from ctypes import *

#initialize system
handle = c_int()
try:
	res = libcaenhv.init_system(system=2, link_type=0, arg='129.236.254.45', username='user', password='q1w2e3r4', handle=byref(handle))
except:
	print 'Error loading Caen Crate'
	exit(1)

#check current settings
print 'Current cathode settings'

parameter_values_list = []
res = libcaenhv.get_channel_parameter(handle=handle, slot=1, parameter_name='Pw', nb_channels=1, channels_list=[11], parameter_values_list=parameter_values_list)

print 'Power: '
print parameter_values_list

parameter_values_list = []
res = libcaenhv.get_channel_parameter(handle=handle, slot=1, parameter_name='VMon', nb_channels=1, channels_list=[11], parameter_values_list=parameter_values_list)

print 'Current Voltages: '
print parameter_values_list



print 'Would you like to alter any settings?'
response = raw_input('If so, please type "Yes" otherwise press enter to exit\n')

if response != 'Yes':
	exit(1)


#start setting parameters

print '\n\nWhat settings would you like to edit'
response = raw_input('You may choose from "Pw", "V0Set", and "I0Set"\n')
setting = response

if response != 'Pw' and response != 'V0Set' and response != 'I0Set':
	print 'Invalid setting choice'
	exit(1)

response = raw_input('Please choose input value\n')
if setting == 'Pw':
	settingValue = int(response)
	if settingValue != 1 and settingValue != 0:
		print 'Must use either 0 or 1 for Pw'
		sys.exit()
else:
	settingValue = float(response)
	
if setting == 'V0Set' and (settingValue > 2500 or settingValue < 100):
	print 'Voltage you would like to set is either too high or low - please restart program and pick an acceptable value for the voltage (between 100 V and 2500 V)'
	exit(1)

print 'Please confirm your choice of setting'
print settingValue
print 'Are you certain that you would like to proceed?'
response = raw_input('If so, please type "Yes" otherwise type anything else\n')

if response != 'Yes':
	print 'Please restart program and try again'
	exit(1)

parameter_values_list = [settingValue]
res = libcaenhv.set_channel_parameter(handle=handle, slot=1, parameter_name=setting, nb_channels=1, channels_list=[11], parameter_values_list=parameter_values_list)

time.sleep(5)

print 'Please check to make sure value was successfully set by restarting the program'

#disconnect with crate
libcaenhv.deinit_system(handle)

