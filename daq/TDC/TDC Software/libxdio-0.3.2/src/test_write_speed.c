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

#define EVENTS_PER_FILE     100
#define FLAGS				(XDIO_F_RAW0_BZIP2)
#define NB_CHANNELS         242
#define SAMPLE_TYPE         14
#define BYTES_PER_SAMPLE	2
#define NB_SAMPLES          16384
#define VOLTAGE_RANGE       2.0
#define SAMPLING_FREQUENCY  1.e8

int
main(int argc, char **argv)
{
	struct timeval start, stop, result;

	/* create a file */
	xdio_file_t *file = xdio_open_write("xe100_080329_1551_000001.xed", time(NULL), 0, EVENTS_PER_FILE, 1);

	int8_t *data_block = (int8_t *) malloc(NB_CHANNELS*NB_SAMPLES*BYTES_PER_SAMPLE);
	memset(data_block, 0x33, NB_CHANNELS*NB_SAMPLES*BYTES_PER_SAMPLE);

	/* just put a recognizable mark at each channel boundary */
	int channel_nb;
	for(channel_nb = 0; channel_nb < NB_CHANNELS; channel_nb++)
	{
		data_block[channel_nb*NB_SAMPLES*BYTES_PER_SAMPLE+0] = 0x54;
		data_block[channel_nb*NB_SAMPLES*BYTES_PER_SAMPLE+1] = 0x32;
		data_block[channel_nb*NB_SAMPLES*BYTES_PER_SAMPLE+2] = 0x10;
	}

	/* start timer */
	gettimeofday(&start, NULL);	

	xdio_chunk_att_t *att = xdio_create_chunk_raw0_attributes(SAMPLE_TYPE, FLAGS, NB_SAMPLES, VOLTAGE_RANGE, SAMPLING_FREQUENCY, NB_CHANNELS);

	int event_nb;
	for(event_nb = 0; event_nb < EVENTS_PER_FILE; event_nb++)
	{
		/* create an event */
		xdio_event_t *event = xdio_create_event();

		struct timeval trigger_time;
		gettimeofday(&trigger_time, NULL);
		xdio_set_trigger_time(trigger_time.tv_sec, trigger_time.tv_usec*1000, event);

		/* create a raw0 chunk */
		xdio_chunk_t *chunk = xdio_create_chunk("raw0", att);

		int channel_nb;

		if(1)
		{
			for(channel_nb = 0; channel_nb < NB_CHANNELS; channel_nb++)
				xdio_attach_data_src(channel_nb, &data_block[channel_nb*NB_SAMPLES*BYTES_PER_SAMPLE], NB_SAMPLES*BYTES_PER_SAMPLE, chunk);
		}
		else
		{
			int channel_group, nb_groups = (NB_CHANNELS % 8)?(NB_CHANNELS/8+1):(NB_CHANNELS/8);
			for(channel_group = 0; channel_group < nb_groups; channel_group++)
			{
				int channel_mask = (channel_group == nb_groups-1)?(3):(255);
				xdio_attach_multi_data_src(8*channel_group, channel_mask, &data_block[8*channel_group*NB_SAMPLES*BYTES_PER_SAMPLE],
					8*NB_SAMPLES*BYTES_PER_SAMPLE, chunk);
			}
		}
	
		/* add the chunk to the event */
		xdio_add_chunk(chunk, event);

		/* add the event to the file */
		xdio_add_event(event, file);
	}

	free(att);

	xdio_close(file);

	free(data_block);

	/* stop timer */
	gettimeofday(&stop, NULL);

	timersub(&stop, &start, &result);

	struct stat stats;

	stat("xe100_080329_1551_000001.xed", &stats);

	printf("time: %d.%06d s ", result.tv_sec, result.tv_usec);
	printf("speed: %0.2f MB/s\n", (stats.st_size/1024./1024.)/(result.tv_sec+(result.tv_usec/1.e6)));
	printf("effective speed: %0.2f MB/s\n", (EVENTS_PER_FILE*NB_CHANNELS*NB_SAMPLES*BYTES_PER_SAMPLE/1024./1024.)/(result.tv_sec+(result.tv_usec/1.e6)));

	return(0);
}

