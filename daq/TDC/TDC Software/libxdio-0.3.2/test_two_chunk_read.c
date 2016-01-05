#include <libxdio.h>

#include <stdio.h>
#ifdef HAVE_STDLIB_H
#	include <stdlib.h>
#endif
#ifdef HAVE_SYS_TIME_H
#	include <sys/time.h>
#endif
#ifdef HAVE_STRING_H
#	include <string.h>
#endif
#ifdef HAVE_SYS_TYPES_H
#	include <sys/types.h>
#endif
#ifdef HAVE_SYS_STAT_H
#	include <sys/stat.h>
#endif
#ifdef HAVE_UNISTD_H
#	include <unistd.h>
#endif


int
main(int argc, char **argv)
{
	xdio_file_t *file = xdio_open_read("./nerix_141013_0950/nerix_141013_0950_000000.xed");
	long int fileLocation = ftell(file->in);
	printf("file location start: %ld\n", fileLocation);
	printf("\nFile ptr: %p\n", file);

	int nb_events = xdio_get_nb_events(file);
	printf("nb_events: %d\n", nb_events);

	xdio_event_t *event = xdio_get_event(0, file);
	printf("\nEvent ptr: %p\n", event);

	fileLocation = ftell(file->in);
	printf("file location before chunk 0: %d\n", fileLocation);

	xdio_chunk_t *pChunk = xdio_get_chunk(0, event);
	printf("\nChunk0 ptr: %p\n", pChunk);
	fileLocation = ftell(file->in);
	printf("file location after chunk 0: %d\n", fileLocation);
	xdio_chunk_t *pChunk_tdc = xdio_get_chunk(1, event);
	printf("\nChunk1 ptr: %p\n", pChunk_tdc);

	xdio_chunk_raw0_att_t *pAtt = (xdio_chunk_raw0_att_t *) xdio_get_attributes(pChunk_tdc);

}
