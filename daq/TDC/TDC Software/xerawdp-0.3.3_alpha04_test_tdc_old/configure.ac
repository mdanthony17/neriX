# package info
AC_INIT([xerawdp], [0.3.3_alpha04], [guillaume.plante@astro.columbia.edu])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])

# check for programs
AC_PROG_CXX
AC_PROG_LN_S
PKG_PROG_PKG_CONFIG

AM_PATH_PYTHON([2.4])

# check for libraries
AC_CHECK_LIB([fftw3], [fftw_execute], [], [AC_MSG_ERROR([libfftw3 not found.])])
AC_CHECK_LIB([xdio], [xdio_open_read], [], [AC_MSG_ERROR([libxdio not found.])])
AC_CHECK_LIB([svm], [svm_load_model])
AC_CHECK_LIB([pmtpattern], [main])
AC_CHECK_LIB([nn], [nn_missing_9_12_39_58], [], [AC_MSG_ERROR([libnn >= 0.1.1 not found.])])

# check for header files
AC_HEADER_STDC
AC_CHECK_HEADERS([sys/time.h])
AC_CHECK_HEADERS([getopt.h])

# check for the root package
ROOT_PATH(5.16/00, AC_DEFINE([HAVE_ROOT], [1], [The ROOT analysis framework]))

# check for pkg-config packages
PKG_CHECK_MODULES([LIBXMLPP], [libxml++-2.6])
PKG_CHECK_MODULES([LIBCHI2], [libchi2-1], AC_DEFINE([HAVE_LIBCHI2], [1], [Chi2 Position Reconstruction]))
if $PKG_CONFIG --exists libchi2-1 || test ".${ac_cv_lib_rzCrctn_rzCrctn_init}" = ".yes"; then
	PKG_CHECK_MODULES([LAPACK], [lapack])
fi
PKG_CHECK_MODULES([LIBFANN], [fann], AC_DEFINE([HAVE_LIBFANN], [1], [Fann Position Reconstruction]))

# check for types
AC_TYPE_INT8_T
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_UINT8_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T

# check for compiler characteristics
AC_C_BIGENDIAN

# enable liquid scintillator quantities
AC_ARG_ENABLE([liq-sci], [  --enable-liq-sci  enable liquid scintillator channels.], [liq_sci=yes])
AS_IF([test "x$enable_liq_sci" = "xyes"], AC_DEFINE([ENABLE_LIQ_SCI], [], [Enable liquid scintillator channels.]))

# enable sodium iodide quantities
AC_ARG_ENABLE([nai], [  --enable-nai  enable sodium iodide channels.], [nai=yes])
AS_IF([test "x$enable_nai" = "xyes"], AC_DEFINE([ENABLE_NAI], [], [Enable sodium iodide channels.]))

# enable high purity germanium quantities
AC_ARG_ENABLE([hpge], [  --enable-hpge  enable high purity germanium channels.], [hpge=yes])
AS_IF([test "x$enable_hpge" = "xyes"], AC_DEFINE([ENABLE_HPGE], [], [Enable high purity germanium channels.]))

# enable time to amplitude quantities
AC_ARG_ENABLE([tac], [  --enable-tac  enable time-to-analog converter channels.], [tac=yes])
AS_IF([test "x$enable_tac" = "xyes"], AC_DEFINE([ENABLE_TAC], [], [Enable time-to-analog converter channels.]))

# files
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
	Makefile
	src/Makefile
	scripts/Makefile
	scripts/config.py
	models/Makefile
])
AC_OUTPUT
