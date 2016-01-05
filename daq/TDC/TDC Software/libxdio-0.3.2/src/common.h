#ifndef __COMMON_H__
#define __COMMON_H__

#include "config.h"

#include <stdio.h>
#ifdef HAVE_STDLIB_H
#	include <stdlib.h>
#endif
#ifdef HAVE_INTTYPES_H
#	include <inttypes.h>
#endif
#ifdef HAVE_SYS_TYPES_H
#	include <sys/types.h>
#endif

#ifdef HAVE_VISIBILITY
#	define XDIO_INTERNAL __attribute__((__visibility__("hidden")))
#else
#	define XDIO_INTERNAL
#endif

#ifdef DEBUG
#	define xdio_debug(...)  { (void) fprintf(stdout, "libxdio: debug: "); fprintf (stdout, __VA_ARGS__); }
#else
#	define xdio_debug(...)  ((void) 0)
#endif

#define xdio_error(...)  { (void) fprintf(stderr, "libxdio: error: "); fprintf (stderr, __VA_ARGS__); }

#define XDIO_MODE_WRITE (1 << 0)
#define XDIO_MODE_READ  (1 << 1)

#define XDIO_RET_ERR (-1)
#define XDIO_RET_OK  (0)

#include "xdio.h"
#include "internal.h"

#endif // __COMMON_H__

