#include "libxdio.h"

#include "config.h"

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
	struct timeval start, stop, result;

	/* open a file */
	xdio_file_t *file = xdio_open_read("xe100_090626_1633_000000.xed");

	int nb_events = xdio_get_nb_events(file);
	printf("nb_events: %d\n", nb_events);

	/* start timer */
	gettimeofday(&start, NULL);	

	int index_nb;
	printf("reading events...\n");
	for(index_nb = 0; index_nb < nb_events; index_nb++)
	{
		xdio_event_t *event = xdio_get_event(index_nb, file);

		/* get the zle0 chunk */
		xdio_chunk_t *zle0_chunk = xdio_get_chunk(0, event);
		xdio_chunk_zle0_att_t *zle0_att = xdio_get_attributes(zle0_chunk);

		/* save the channel data here */
		int8_t *zle0_data_block = (int8_t *) malloc(2*zle0_att->nb_samples*zle0_att->nb_channels);

		/* read channels only */
		int c;
		for(c = 0; c < zle0_att->nb_channels_included; c++)
			xdio_attach_data_dst(zle0_att->channels[c], zle0_data_block+c*2*zle0_att->nb_samples, 2*zle0_att->nb_samples, zle0_chunk);

		/* extract the chunk data */
		xdio_extract_data(zle0_chunk);

		free(zle0_data_block);
	}

	xdio_close(file);

	/* stop timer */
	gettimeofday(&stop, NULL);

	timersub(&stop, &start, &result);

	struct stat stats;

	stat("xe100_090626_1633_000000.xed", &stats);

	printf("time: %d.%06d s ", result.tv_sec, result.tv_usec);
	printf("speed: %0.2f MB/s\n", (stats.st_size/1024/1024)/(result.tv_sec+(result.tv_usec/1.e6)));

	return(0);
}

