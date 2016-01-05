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
print 'Current PMT Settings'
print 'Printed in following form [PMT 1, PMT 2, PMT 3, PMT 4, PMT 5, PMT 6]'

parameter_values_list = []
res = libcaenhv.get_channel_parameter(handle=handle, slot=0, parameter_name='Pw', nb_channels=6, channels_list=[1,2,3,4,5,6], parameter_values_list=parameter_values_list)

print 'Power: '
print parameter_values_list

parameter_values_list = []
res = libcaenhv.get_channel_parameter(handle=handle, slot=0, parameter_name='VMon', nb_channels=6, channels_list=[1,2,3,4,5,6], parameter_values_list=parameter_values_list)

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

response = raw_input('Please choose a PMT number (1-6)')
pmtNumber = int(response)

if pmtNumber != 1 and pmtNumber != 2 and pmtNumber != 3 and pmtNumber != 4 and pmtNumber != 5 and pmtNumber != 6:
	print 'Invalid PMT Choice'
	exit(1)

response = raw_input('Please choose input value\n')
if setting == 'Pw':
	settingValue = int(response)
else:
	settingValue = float(response)

print 'Please confirm your choice of setting'
print settingValue
print 'Are you certain that you would like to proceed?'
response = raw_input('If so, please type "Yes" otherwise type anything else')

if response != 'Yes':
	print 'Please restart program and try again'
	exit(1)

parameter_values_list = [settingValue]
res = libcaenhv.set_channel_parameter(handle=handle, slot=0, parameter_name=setting, nb_channels=1, channels_list=[pmtNumber], parameter_values_list=parameter_values_list)

time.sleep(5)

print 'Please check to make sure value was successfully set by restarting the program'

#disconnect with crate
libcaenhv.deinit_system(handle)

