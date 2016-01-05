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

#define EVENTS_PER_FILE         100

#define RAW0_NB_CHANNELS        16
#define RAW0_SAMPLE_TYPE        14
#define RAW0_FLAGS				(XDIO_F_RAW0_GZIP)
#define RAW0_BYTES_PER_SAMPLE   2
#define RAW0_NB_SAMPLES         256
#define RAW0_VOLTAGE_RANGE      1.0
#define RAW0_SAMPLING_FREQUENCY 1.e9

#define ZLE0_NB_CHANNELS        23
#define ZLE0_SAMPLE_TYPE        14
#define ZLE0_FLAGS				(XDIO_F_ZLE0_BZIP2)
#define ZLE0_BYTES_PER_SAMPLE   2
#define ZLE0_NB_SAMPLES         1024
#define ZLE0_VOLTAGE_RANGE      2.25
#define ZLE0_SAMPLING_FREQUENCY 1.e8

#define MAX_NB_EVENTS       100

int
main(int argc, char **argv)
{
	/* seed the random number engine */
	struct timeval current_time;
	gettimeofday(&current_time, NULL);

	srand(current_time.tv_usec);

	/* create a file */
	xdio_file_t *file = xdio_open_write("xe100_071219_1438_000000.xed", time(NULL), 0, EVENTS_PER_FILE, 1);

	int nb_events = (int) ((float) MAX_NB_EVENTS*rand()/RAND_MAX)+1, event_nb;
	printf("xdio file will have %d events\n", nb_events);

	/* some fake raw0 data */
	int8_t *raw0_data_block = (int8_t *) malloc(RAW0_NB_CHANNELS*RAW0_NB_SAMPLES*RAW0_BYTES_PER_SAMPLE);
	memset(raw0_data_block, 0x33, RAW0_NB_CHANNELS*RAW0_NB_SAMPLES*RAW0_BYTES_PER_SAMPLE);

	int channel_nb;
	for(channel_nb = 0; channel_nb < RAW0_NB_CHANNELS; channel_nb++)
	{
		raw0_data_block[channel_nb*RAW0_NB_SAMPLES*RAW0_BYTES_PER_SAMPLE+0] = 0x54;
		raw0_data_block[channel_nb*RAW0_NB_SAMPLES*RAW0_BYTES_PER_SAMPLE+1] = 0x32;
		raw0_data_block[channel_nb*RAW0_NB_SAMPLES*RAW0_BYTES_PER_SAMPLE+2] = 0x10;
	}

	/* some fake zle0 data */
	int8_t *zle0_data_block = (int8_t *) malloc(ZLE0_NB_CHANNELS*ZLE0_NB_SAMPLES*ZLE0_BYTES_PER_SAMPLE);
	memset(zle0_data_block, 0x33, ZLE0_NB_CHANNELS*ZLE0_NB_SAMPLES*ZLE0_BYTES_PER_SAMPLE);

	for(channel_nb = 0; channel_nb < ZLE0_NB_CHANNELS; channel_nb++)
	{
		*((int32_t *) &zle0_data_block[channel_nb*ZLE0_NB_SAMPLES*ZLE0_BYTES_PER_SAMPLE]) = (ZLE0_NB_SAMPLES*ZLE0_BYTES_PER_SAMPLE)/4;
		zle0_data_block[channel_nb*RAW0_NB_SAMPLES*RAW0_BYTES_PER_SAMPLE+4] = 0x54;
		zle0_data_block[channel_nb*RAW0_NB_SAMPLES*RAW0_BYTES_PER_SAMPLE+5] = 0x32;
		zle0_data_block[channel_nb*RAW0_NB_SAMPLES*RAW0_BYTES_PER_SAMPLE+6] = 0x10;
	}

	/* create raw0 and zle0 chunk properties */
	xdio_chunk_att_t *raw0_att = xdio_create_chunk_raw0_attributes(RAW0_SAMPLE_TYPE, RAW0_FLAGS, RAW0_NB_SAMPLES,
		RAW0_VOLTAGE_RANGE, RAW0_SAMPLING_FREQUENCY, RAW0_NB_CHANNELS);
	xdio_chunk_att_t *zle0_att = xdio_create_chunk_zle0_attributes(ZLE0_SAMPLE_TYPE, ZLE0_FLAGS, ZLE0_NB_SAMPLES,
		ZLE0_VOLTAGE_RANGE, ZLE0_SAMPLING_FREQUENCY, ZLE0_NB_CHANNELS);

	for(event_nb = 0; event_nb < nb_events; event_nb++)
	{
		/* create an event */
		xdio_event_t *event = xdio_create_event();

		struct timeval trigger_time;
		gettimeofday(&trigger_time, NULL);
		xdio_set_trigger_time(trigger_time.tv_sec, trigger_time.tv_usec*1000, event);

		int channel_nb;

		/* create a raw0 chunk */
		xdio_chunk_t *raw0_chunk = xdio_create_chunk("raw0", raw0_att);

		for(channel_nb = 0; channel_nb < RAW0_NB_CHANNELS; channel_nb++)
			xdio_attach_data_src(channel_nb, &raw0_data_block[channel_nb*RAW0_NB_SAMPLES*RAW0_BYTES_PER_SAMPLE],
				RAW0_NB_SAMPLES*RAW0_BYTES_PER_SAMPLE, raw0_chunk);

		/* add the raw0 chunk to the event */
		xdio_add_chunk(raw0_chunk, event);

		/* create a zle0 chunk */
		xdio_chunk_t *zle0_chunk = xdio_create_chunk("zle0", zle0_att);

		printf("event %d, channels-> ", event_nb);
		for(channel_nb = 0; channel_nb < ZLE0_NB_CHANNELS; channel_nb++)
			if(((float) rand())/RAND_MAX < 0.5)
			{
				xdio_attach_data_src(channel_nb, &zle0_data_block[channel_nb*ZLE0_NB_SAMPLES*ZLE0_BYTES_PER_SAMPLE],
					ZLE0_NB_SAMPLES*ZLE0_BYTES_PER_SAMPLE, zle0_chunk);
				printf("%d ", channel_nb);
			}
		printf("\n");

		/* add the raw0 chunk to the event */
		xdio_add_chunk(zle0_chunk, event);

		/* add the event to the file */
		xdio_add_event(event, file);
	}

	free(raw0_att);
	free(zle0_att);

	xdio_close(file);

	free(raw0_data_block);
	free(zle0_data_block);

	/* reopen the file for reading */
	file = xdio_open_read("xe100_071219_1438_000000.xed");

	nb_events = xdio_get_nb_events(file);

	/* get a random event */
	int index_nb = (int) ((float) nb_events*rand()/RAND_MAX);
	xdio_event_t *event = xdio_get_event(index_nb, file);
	printf("reading event at index %d...\n", index_nb);

	/* get the raw0 chunk */
	xdio_chunk_t *raw0_chunk = xdio_get_chunk(0, event);

	/* read channel 3 only */
	raw0_data_block = (int8_t *) malloc(RAW0_NB_SAMPLES*RAW0_BYTES_PER_SAMPLE);
	xdio_attach_data_dst(1, raw0_data_block, RAW0_NB_SAMPLES*RAW0_BYTES_PER_SAMPLE, raw0_chunk);

	/* extract the chunk data */
	xdio_extract_data(raw0_chunk);

	/* get the zle0 chunk */
	xdio_chunk_t *zle0_chunk = xdio_get_chunk(1, event);

	/* read channel 3 only */
	zle0_data_block = (int8_t *) malloc(ZLE0_NB_SAMPLES*ZLE0_BYTES_PER_SAMPLE);
	xdio_attach_data_dst(1, zle0_data_block, ZLE0_NB_SAMPLES*ZLE0_BYTES_PER_SAMPLE, zle0_chunk);

	/* extract the chunk data */
	xdio_extract_data(zle0_chunk);

	xdio_close(file);

	free(raw0_data_block);
	free(zle0_data_block);

	return(0);
}

