include_HEADERS = libxdio.h xdio.h
lib_LTLIBRARIES = libxdio.la
libxdio_la_SOURCES = libxdio.h xdio.h common.h internal.h \
	xdio_utils.c xdio_file.c xdio_event.c \
	xdio_chunk.c xdio_chunk_raw.c xdio_chunk_zle.c
libxdio_la_LDFLAGS = -version-info 0:0:0

noinst_PROGRAMS = test_write_speed test_read_speed test_memory_leaks
test_write_speed_SOURCES = test_write_speed.c libxdio.h
test_write_speed_LDADD = libxdio.la
test_read_speed_SOURCES = test_read_speed.c libxdio.h
test_read_speed_LDADD = libxdio.la
test_memory_leaks_SOURCES = test_memory_leaks.c libxdio.h
test_memory_leaks_LDADD = libxdio.la

bin_PROGRAMS = xedinfo xedcompress xedselect
xedinfo_SOURCES = xedinfo.c libxdio.h
xedinfo_LDADD = libxdio.la
xedcompress_SOURCES = xedcompress.c libxdio.h
xedcompress_LDADD = libxdio.la
xedselect_SOURCES = xedselect.c libxdio.h
xedselect_LDADD = libxdio.la
