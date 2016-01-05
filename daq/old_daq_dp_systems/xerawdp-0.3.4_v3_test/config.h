/* config.h.  Generated from config.h.in by configure.  */
/* config.h.in.  Generated from configure.ac by autoheader.  */

/* Define if building universal (internal helper macro) */
/* #undef AC_APPLE_UNIVERSAL_BUILD */

/* Enable high purity germanium channels. */
#define ENABLE_HPGE /**/

/* Enable liquid scintillator channels. */
/* #undef ENABLE_LIQ_SCI */

/* Enable sodium iodide channels. */
/* #undef ENABLE_NAI */

/* Enable time-to-analog converter channels. */
/* #undef ENABLE_TAC */

/* Define to 1 if you have the <getopt.h> header file. */
#define HAVE_GETOPT_H 1

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Chi2 Position Reconstruction */
#define HAVE_LIBCHI2 1

/* Fann Position Reconstruction */
#define HAVE_LIBFANN 1

/* Define to 1 if you have the `fftw3' library (-lfftw3). */
#define HAVE_LIBFFTW3 1

/* Define to 1 if you have the `nn' library (-lnn). */
#define HAVE_LIBNN 1

/* Define to 1 if you have the `pmtpattern' library (-lpmtpattern). */
/* #undef HAVE_LIBPMTPATTERN */

/* Define to 1 if you have the `svm' library (-lsvm). */
/* #undef HAVE_LIBSVM */

/* Define to 1 if you have the `xdio' library (-lxdio). */
#define HAVE_LIBXDIO 1

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* The ROOT analysis framework */
#define HAVE_ROOT 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/time.h> header file. */
#define HAVE_SYS_TIME_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Name of package */
#define PACKAGE "xerawdp"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "guillaume.plante@astro.columbia.edu"

/* Define to the full name of this package. */
#define PACKAGE_NAME "xerawdp"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "xerawdp 0.3.3_alpha04"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "xerawdp"

/* Define to the home page for this package. */
#define PACKAGE_URL ""

/* Define to the version of this package. */
#define PACKAGE_VERSION "0.3.3_alpha04"

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Version number of package */
#define VERSION "0.3.3_alpha04"

/* Define WORDS_BIGENDIAN to 1 if your processor stores words with the most
   significant byte first (like Motorola and SPARC, unlike Intel). */
#if defined AC_APPLE_UNIVERSAL_BUILD
# if defined __BIG_ENDIAN__
#  define WORDS_BIGENDIAN 1
# endif
#else
# ifndef WORDS_BIGENDIAN
/* #  undef WORDS_BIGENDIAN */
# endif
#endif

/* Define for Solaris 2.5.1 so the uint32_t typedef from <sys/synch.h>,
   <pthread.h>, or <semaphore.h> is not used. If the typedef were allowed, the
   #define below would cause a syntax error. */
/* #undef _UINT32_T */

/* Define for Solaris 2.5.1 so the uint8_t typedef from <sys/synch.h>,
   <pthread.h>, or <semaphore.h> is not used. If the typedef were allowed, the
   #define below would cause a syntax error. */
/* #undef _UINT8_T */

/* Define to the type of a signed integer type of width exactly 16 bits if
   such a type exists and the standard includes do not define it. */
/* #undef int16_t */

/* Define to the type of a signed integer type of width exactly 32 bits if
   such a type exists and the standard includes do not define it. */
/* #undef int32_t */

/* Define to the type of a signed integer type of width exactly 8 bits if such
   a type exists and the standard includes do not define it. */
/* #undef int8_t */

/* Define to the type of an unsigned integer type of width exactly 16 bits if
   such a type exists and the standard includes do not define it. */
/* #undef uint16_t */

/* Define to the type of an unsigned integer type of width exactly 32 bits if
   such a type exists and the standard includes do not define it. */
/* #undef uint32_t */

/* Define to the type of an unsigned integer type of width exactly 8 bits if
   such a type exists and the standard includes do not define it. */
/* #undef uint8_t */
