from ctypes import *

# xdio_file_header_t
#class XdioFileHeader(Structure):
#    _fields_ = []
#    _fields_.append(('filename', c_char*64))
#    _fields_.append(('time', c_int32))
#    _fields_.append(('first_event_nb', c_int32))
#    _fields_.append(('nb_events', c_int32))
#    _fields_.append(('index_size', c_int32))
#    _fields_.append(('index', c_void_p))

# xdio_file_t
#class XdioFile(Structure):
#    _fields_ = []
#    _fields_.append(('header', POINTER(XdioFileHeader)))
#    _fields_.append(('mode', c_int))
#    _fields_.append(('out', c_void_p))
#    _fields_.append(('in', c_void_p))
#    _fields_.append(('desired_nb_events', c_int))
#    _fields_.append(('nb_events_added', c_int))
#    _fields_.append(('event_buffer_size', c_int))
#    _fields_.append(('nb_events_in_buffer', c_int))
#    _fields_.append(('events', c_void_p))

# xdio_event_header_t
#class XdioEventHeader(Structure):
#    _fields_ = []
#    _fields_.append(('filename', c_char*64))
#    _fields_.append(('time_sec', c_int32))
#    _fields_.append(('time_usec', c_int32))
#    _fields_.append(('event_nb', c_int32))
#    _fields_.append(('nb_chunks', c_int32))

# xdio_event_t
#class XdioEvent(Structure):
#    _fields_ = []
#    _fields_.append(('header', POINTER(XdioEventHeader)))
#    _fields_.append(('nb_chunks', c_int))
#    _fields_.append(('max_chunks', c_int))
#    _fields_.append(('chunks', c_void_p))
#    _fields_.append(('offset', c_int))
#    _fields_.append(('to_file', c_void_p))
#    _fields_.append(('from_file', c_void_p))

# xdio_chunk_t
#class XdioChunk(Structure):
#    _fields_ = []
#    _fields_.append(('obj_', c_void_p))
#    _fields_.append(('ops_', c_void_p))
#    _fields_.append(('event', c_void_p))

# xdio_chunk_raw0_att_t
#class XdioChunkRaw0Att(Structure):
#    _fields_ = []
#    _fields_.append(('sample_type', c_int))
#    _fields_.append(('flags', c_int))
#    _fields_.append(('nb_samples', c_int))
#    _fields_.append(('voltage_range', c_float))
#    _fields_.append(('sampling_frequency', c_float))
#    _fields_.append(('nb_channels', c_int))
#    _fields_.append(('bytes_per_sample', c_int))
#    _fields_.append(('channel_length', c_int))

# xdio_chunk_zle0_att_t
#class XdioChunkZle0Att(Structure):
#    _fields_ = []
#    _fields_.append(('sample_type', c_int))
#    _fields_.append(('flags', c_int))
#    _fields_.append(('nb_samples', c_int))
#    _fields_.append(('voltage_range', c_float))
#    _fields_.append(('sampling_frequency', c_float))
#    _fields_.append(('nb_channels', c_int))
#    _fields_.append(('bytes_per_sample', c_int))
#    _fields_.append(('nb_channels_included', c_int))
#    _fields_.append(('channels', POINTER(c_int)))
#    _fields_.append(('channel_lengths', POINTER(c_int)))

#def open_write(filename, creation_time, first_event_nb, events_per_file):
#    return libxdio_so.xdio_open_write(filename, c_int(int(creation_time)), first_event_nb, events_per_file, 1)

#def open_read(filename):
#    return libxdio_so.xdio_open_read(filename)

#def create_chunk_raw0_attributes(sample_type, flags, nb_samples, voltage_range, sampling_frequency, nb_channels):
#    return libxdio_so.xdio_create_chunk_raw0_attributes(sample_type, flags, nb_samples, c_float(voltage_range), c_float(sampling_frequency), nb_channels)

#def create_chunk_zle0_attributes(sample_type, flags, nb_samples, voltage_range, sampling_frequency, nb_channels):
#    return libxdio_so.xdio_create_chunk_zle0_attributes(sample_type, flags, nb_samples, c_float(voltage_range), c_float(sampling_frequency), nb_channels)

#def get_attributes(chunk):
#    return libxdio_so.xdio_get_attributes(chunk)

#def chunk_type_cmp(type, chunk):
#    return libxdio_so.xdio_chunk_type_cmp(type, chunk)

#def create_event():
#    return libxdio_so.xdio_create_event()

#def get_event(event_index, file):
#    return libxdio_so.xdio_get_event(event_index, file)

#def set_trigger_time(time_sec, time_usec, event):
#    libxdio_so.xdio_set_trigger_time(time_sec, time_usec, event)

#def create_chunk(chunk_type, chunk_att):
#    return libxdio_so.xdio_create_chunk(chunk_type, chunk_att)

#def get_nb_chunks(event):
#    return libxdio_so.xdio_get_nb_chunks(event)

#def get_chunk(chunk_index, event):
#    return libxdio_so.xdio_get_chunk(chunk_index, event)

#def attach_data_src(channel, data, length, chunk):
#    global _data_buffer

#    if str(chunk) not in _data_buffer:
#        _data_buffer[str(chunk)] = {}
#    _data_buffer[str(chunk)][channel] = (c_int8 * length) ()
#    _data_buffer[str(chunk)][channel][:] = data
#    libxdio_so.xdio_attach_data_src(channel, pointer(_data_buffer[str(chunk)][channel]), length, chunk)

#def attach_data_dst(channel, data, length, chunk):
#    global _data_buffer, _data

#    if str(chunk) not in _data_buffer:
#        _data_buffer[str(chunk)] = {}
#        _data[str(chunk)] = {}
#    _data_buffer[str(chunk)][channel] = (c_int8 * length) ()
#    _data[str(chunk)][channel] = data
#    libxdio_so.xdio_attach_data_dst(channel, pointer(_data_buffer[str(chunk)][channel]), length, chunk)

#def add_chunk(chunk, event):
#    libxdio_so.xdio_add_chunk(chunk, event)

#def add_event(event, file):
#    global _data_buffer, _data

#    libxdio_so.xdio_add_event(event, file)
#    _data_buffer = {}

#def extract_data(chunk):
#    global _data_buffer

#    libxdio_so.xdio_extract_data(chunk)
#    for channel in _data_buffer[str(chunk)].iterkeys():
#        _data[str(chunk)][channel] += (_data_buffer[str(chunk)][channel])
#    _data_buffer = {}

#def close(file):
#    libxdio_so.xdio_close(file)

#libxdio_so = cdll.LoadLibrary('libxdio.so')
#libxdio_so.xdio_open_write.restype = POINTER(XdioFile)
#libxdio_so.xdio_open_read.restype = POINTER(XdioFile)
#libxdio_so.xdio_create_event.restype = POINTER(XdioEvent)
#libxdio_so.xdio_get_event.restype = POINTER(XdioEvent)
#libxdio_so.xdio_create_chunk_raw0_attributes.restype = POINTER(XdioChunkRaw0Att)
#libxdio_so.xdio_get_attributes.restype = c_void_p
#libxdio_so.xdio_create_chunk.restype = POINTER(XdioChunk)
#libxdio_so.xdio_get_chunk.restype = POINTER(XdioChunk)

#_data_buffer = {}
#_data = {}

MAX_CH_NAME = 12
MAX_PARAM_NAME = 10

def get_list_of_type(parameter_name, nb_channels):
	if(parameter_name == 'V0Set' or parameter_name == 'I0Set' or parameter_name == 'VMon' or parameter_name == 'IMon'):
		list = (c_float * nb_channels)()
		return list
	elif(parameter_name == 'Status' or parameter_name == 'Pw'):
		list = (c_int * nb_channels)()
		return list
	else:
		print 'Status parameter type must be added before use'

def convert_list_of_type(parameter_name, parameter_values_list, nb_channels):
	if(parameter_name == 'V0Set' or parameter_name == 'I0Set' or parameter_name == 'VMon' or parameter_name == 'IMon'):
		list = (c_float * nb_channels)(*parameter_values_list)
		return list
	elif(parameter_name == 'Status' or parameter_name == 'Pw'):
		list = (c_int * nb_channels)(*parameter_values_list)
		return list
	else:
		print 'Status parameter type must be added before use'
	

def init_system(system, link_type, arg, username, password, handle):
	return libcaenhvwrapper_so.CAENHV_InitSystem(system, link_type, arg, username, password, handle)

def deinit_system(handle):
	return libcaenhvwrapper_so.CAENHV_DeinitSystem(handle)

def get_channel_name(handle, slot, nb_channels, channels_list, channel_names):
	c_channels_list = (c_ushort * nb_channels)(*channels_list)
	c_channel_names = ((c_char * MAX_CH_NAME) * len(channels_list))()
	libcaenhvwrapper_so.CAENHV_GetChName(handle, c_ushort(slot), c_ushort(nb_channels), c_channels_list, c_channel_names)
	for c_channel_name in c_channel_names:
		channel_names.append(c_channel_name.value)

def set_channel_name():
	pass

def get_channel_parameter(handle, slot, parameter_name, nb_channels, channels_list, parameter_values_list):
	c_channels_list = (c_ushort * nb_channels)(*channels_list)
	c_parameter_values_list = get_list_of_type(parameter_name, nb_channels)
	libcaenhvwrapper_so.CAENHV_GetChParam(handle, c_ushort(slot), parameter_name, c_ushort(nb_channels), c_channels_list, cast(c_parameter_values_list, c_void_p))
	for c_parameter_value in c_parameter_values_list:
		parameter_values_list.append(c_parameter_value)

def set_channel_parameter(handle, slot, parameter_name, nb_channels, channels_list, parameter_values_list):
	c_channels_list = (c_ushort * nb_channels)(*channels_list)
	c_parameter_values_list = convert_list_of_type(parameter_name, parameter_values_list, nb_channels)
	libcaenhvwrapper_so.CAENHV_SetChParam(handle, c_ushort(slot), parameter_name, c_ushort(nb_channels), c_channels_list, cast(c_parameter_values_list, c_void_p))

libcaenhvwrapper_so = cdll.LoadLibrary('libcaenhvwrapper.so')

