# package info
AC_INIT([libxdio], [0.3.2], [guillaume.plante@astro.columbia.edu])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])

# check for programs
AC_PROG_CC
AC_PROG_LIBTOOL
AC_PROG_SED
AC_PROG_INSTALL

# check for libraries
AC_CHECK_LIB([z], [deflateInit_])
AC_CHECK_LIB([bz2], [BZ2_bzCompressInit])

# check for header files
AC_HEADER_STDC
AC_CHECK_HEADERS([time.h])
AC_CHECK_HEADERS([sys/time.h])

# check for types
AC_TYPE_INT8_T
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_UINT8_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T

# check for compiler characteristics
AC_C_BIGENDIAN
AC_CHECK_SIZEOF([long])
gl_VISIBILITY

# files
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h src/libxdio.h])
AC_CONFIG_FILES([
	Makefile
	src/Makefile
])
AC_OUTPUT
