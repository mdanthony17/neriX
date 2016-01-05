import libcaenhv
from ctypes import *

handle = c_int()
res = libcaenhv.init_system(system=2, link_type=0, arg='129.236.254.45', username='user', password='q1w2e3r4', handle=byref(handle))
print 'res=',res

#channel_names = []
#res = libcaenhv.get_channel_name(handle=handle, slot=0, nb_channels=2, channels_list=[0,1], channel_names=channel_names)
#print channel_names

#channel_names = []
#res = libcaenhv.get_channel_name(handle=handle, slot=0, nb_channels=5, channels_list=[1,2,3,4,5], channel_names=channel_names)
#print channel_names

print 'PMTs'

parameter_values_list = []
res = libcaenhv.get_channel_parameter(handle=handle, slot=0, parameter_name='V0Set', nb_channels=7, channels_list=[1,2,3,4,5,6,7], parameter_values_list=parameter_values_list)
print parameter_values_list

parameter_values_list = [1]
res = libcaenhv.set_channel_parameter(handle=handle, slot=0, parameter_name='VSet', nb_channels=1, channels_list=[1], parameter_values_list=parameter_values_list)

parameter_values_list = []
res = libcaenhv.get_channel_parameter(handle=handle, slot=0, parameter_name='VMon', nb_channels=5, channels_list=[1,2,3,4,5], parameter_values_list=parameter_values_list)
print parameter_values_list

print 'Anode'

parameter_values_list = []
res = libcaenhv.get_channel_parameter(handle=handle, slot=15, parameter_name='V0Set', nb_channels=1, channels_list=[0], parameter_values_list=parameter_values_list)
print parameter_values_list

parameter_values_list = []
res = libcaenhv.get_channel_parameter(handle=handle, slot=15, parameter_name='VMon', nb_channels=1, channels_list=[0], parameter_values_list=parameter_values_list)
print parameter_values_list

libcaenhv.deinit_system(handle)

