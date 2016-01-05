#include "libxdio.h"

#include "config.h"

#include <stdio.h>
#ifdef HAVE_STDLIB_H
#	include <stdlib.h>
#endif
#ifdef HAVE_TIME_H
#	include <time.h>
#endif
#ifdef HAVE_SYS_TIME_H
#	include <sys/time.h>
#endif
#ifdef HAVE_STRING_H
#	include <string.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

void
usage()
{
	printf("xedcompress [switches] <xed_file> [<compressed_xed_file>]\n\n");
	printf("switches:\n");
	printf("  -v verbose, display as much information as possible\n");
	exit(0);
}

int
main(int argc, char **argv)
{
	/* switches */
	int c = 0;

	int verbose = 0;

	/* parse switches */
	while((c = getopt(argc, argv, "v")) != -1)
	{
		switch(c)
		{
			case 'v':
				verbose = 1;
				break;

			default:
				usage();
		}
	}

	if(optind >= argc || argc-optind > 2)
		usage();

	/* check that the xdio input file exists */
	if(access(argv[optind], R_OK))
	{
		fprintf(stderr, "xedcompress: error, cannot open input file %s!\n", argv[optind]);
		exit(EXIT_FAILURE);
	}
	
	char *in_filename, *out_filename;
	/* if both the xdio input and output files are named */
	if(argc-optind == 2)
	{
		in_filename = strdup(argv[optind]);
		out_filename = strdup(argv[optind+1]);
	}
	/* otherwise we do the compression "in-place" */
	else
	{
		/* change the name of the xdio input file to a temporary name */
		in_filename = malloc(strlen(argv[optind])+5);
		strcpy(in_filename, argv[optind]);
		strcat(in_filename, ".old");
		rename(argv[optind], in_filename);

		out_filename = strdup(argv[optind]);
	}
	
	/* open the input file */
	xdio_file_t *in_file = xdio_open_read(in_filename);
	int nb_events = xdio_get_nb_events(in_file);
	time_t creation_time = xdio_get_time_created(in_file);
	int first_event_nb = xdio_get_first_event_nb(in_file);
	int events_per_file = in_file->header->index_size;

	/* open the ouput file */
	xdio_file_t *out_file = xdio_open_write(out_filename, creation_time, first_event_nb, events_per_file, 1);

	/* go over all events */
	int index_nb;
	for(index_nb = 0; index_nb < nb_events; index_nb++)
	{
		if(verbose)
		{
			printf("\rcompressing event %d/%d", index_nb, nb_events);
			fflush(stdout);
		}

		/* get the event from the input file */
		xdio_event_t *in_event = xdio_get_event(index_nb, in_file);
		int trigger_time_sec = xdio_get_trigger_time_sec(in_event);
		int trigger_time_usec = xdio_get_trigger_time_usec(in_event);
		int nb_chunks = xdio_get_nb_chunks(in_event);

		/* create a new event for the output file */
		xdio_event_t *out_event = xdio_create_event();
		xdio_set_trigger_time(trigger_time_sec, trigger_time_usec, out_event);

		/* where we will keep track of all the exchange buffers allocated */
		int8_t **chunks_channel_data = (int8_t **) malloc(nb_chunks*sizeof(int8_t *));

		int chunk_nb;
		for(chunk_nb = 0; chunk_nb < nb_chunks; chunk_nb++)
		{
			xdio_chunk_t *in_chunk = xdio_get_chunk(chunk_nb, in_event);

			/* if the chunk type is raw0 */
			if(!xdio_chunk_type_cmp("raw0", in_chunk))
			{
				/* get the attributes and check if the chunk is not already compressed */
				xdio_chunk_t *in_raw0_chunk = in_chunk;
				xdio_chunk_raw0_att_t *raw0_att = xdio_get_attributes(in_raw0_chunk);
				if((raw0_att->flags & XDIO_F_RAW0_GZIP) || (raw0_att->flags & XDIO_F_RAW0_BZIP2))
				{
					fprintf(stderr, "xedcompress: error, raw0 chunk is already compressed!\n");
					exit(EXIT_FAILURE);
				}

				/* get the required attributes and allocate buffers for the exchange */
				int nb_channels = raw0_att->nb_channels;
				int nb_samples = raw0_att->nb_samples;
				int bytes_per_sample = raw0_att->bytes_per_sample;
				int channel_size = nb_samples*bytes_per_sample;

				int8_t *channel_data = (int8_t *) malloc(nb_channels*channel_size);
				chunks_channel_data[chunk_nb] = channel_data;

				/* by default compress with bzip2 */
				raw0_att->flags |= XDIO_F_RAW0_BZIP2;

				/* create a new raw0 chunk */
				xdio_chunk_t *out_raw0_chunk = xdio_create_chunk("raw0", raw0_att);

				/* attach all data destinations and data sources */
				int channel_nb;
				for(channel_nb = 0; channel_nb < nb_channels; channel_nb++)
				{
					xdio_attach_data_dst(channel_nb, &channel_data[channel_nb*channel_size], channel_size, in_raw0_chunk);
					xdio_attach_data_src(channel_nb, &channel_data[channel_nb*channel_size], channel_size, out_raw0_chunk);
				}

				/* extract the data and add the compressed raw0 chunk to the event */
				xdio_extract_data(in_raw0_chunk);
				xdio_add_chunk(out_raw0_chunk, out_event);

				free(channel_data);
			}
			/* or if the chunk type is zle0 */
			else if(!xdio_chunk_type_cmp("zle0", in_chunk))
			{
				/* get the attributes and check if the chunk is not already compressed */
				xdio_chunk_t *in_zle0_chunk = in_chunk;
				xdio_chunk_zle0_att_t *zle0_att = xdio_get_attributes(in_zle0_chunk);
				if((zle0_att->flags & XDIO_F_ZLE0_GZIP) || (zle0_att->flags & XDIO_F_ZLE0_BZIP2))
				{
					fprintf(stderr, "xedcompress: error, zle0 chunk is already compressed!\n");
					exit(EXIT_FAILURE);
				}

				/* get the required attributes and allocate buffers for the exchange */
				float safety_margin = 1.1;
				int nb_channels = zle0_att->nb_channels;
				int nb_channels_included = zle0_att->nb_channels_included;
				int nb_samples = zle0_att->nb_samples;
				int bytes_per_sample = zle0_att->bytes_per_sample;
				int max_channel_size = (int) (safety_margin*nb_samples*bytes_per_sample);

				int8_t *channel_data = (int8_t *) malloc(nb_channels_included*max_channel_size);
				chunks_channel_data[chunk_nb] = channel_data;

				/* by default compress with bzip2 */
				zle0_att->flags |= XDIO_F_ZLE0_BZIP2;

				/* create a new zle0 chunk */
				xdio_chunk_t *out_zle0_chunk = xdio_create_chunk("zle0", zle0_att);

				/* attach all data destinations and data sources */
				int channel_index;
				for(channel_index = 0; channel_index < nb_channels_included; channel_index++)
				{
					int channel_nb = zle0_att->channels[channel_index];
					int channel_size = zle0_att->channel_lengths[channel_index];

					if(channel_size > max_channel_size)
					{
						fprintf(stderr, "xedcompress: error, not enough space allocated to extract data!\n");
						exit(EXIT_FAILURE);
					}

					xdio_attach_data_dst(channel_nb, &channel_data[channel_index*max_channel_size], channel_size, in_zle0_chunk);
					xdio_attach_data_src(channel_nb, &channel_data[channel_index*max_channel_size], channel_size, out_zle0_chunk);
				}

				/* extract the data and add the compressed zle0 chunk to the event */
				xdio_extract_data(in_zle0_chunk);
				xdio_add_chunk(out_zle0_chunk, out_event);
			}
		}

		/* add the event to the file */
		xdio_add_event(out_event, out_file);

		/* ok, all the data has been written to disk, we can free all the buffers */
		for(chunk_nb = 0; chunk_nb < nb_chunks; chunk_nb++)
			free(chunks_channel_data[chunk_nb]);
		free(chunks_channel_data);
	}

	xdio_close(in_file);
	xdio_close(out_file);

	free(in_filename);
	free(out_filename);

	exit(EXIT_SUCCESS);
}

