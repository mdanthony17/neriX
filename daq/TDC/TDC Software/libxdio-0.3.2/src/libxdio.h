/* src/libxdio.h.  Generated from libxdio.h.in by configure.  */
#ifndef __LIBXDIO_H__
#define __LIBXDIO_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>	

#ifndef HAVE_INTTYPES_H
#	define HAVE_INTTYPES_H 1
#endif
#ifndef HAVE_STDINT_H
#	define HAVE_STDINT_H 1
#endif
#ifndef HAVE_SYS_TYPES_H
#	define HAVE_SYS_TYPES_H 1
#endif
#ifndef HAVE_SYS_TYPES_H
#	define HAVE_SYS_TYPES_H 1
#endif

#if HAVE_INTTYPES_H
#	include <inttypes.h>
#else
#	if HAVE_STDINT_H
#		include <stdint.h>
#	else
#		if HAVE_SYS_TYPES_H
#			include <sys/types.h>
#		endif
#	endif
#endif
#if HAVE_SYS_TYPES_H
#	include <sys/types.h>
#endif

#ifndef int32_t
/* #undef int32_t */
#endif

#include <xdio.h>

#ifdef __cplusplus
}
#endif

#endif // __LIBXDIO_H__
