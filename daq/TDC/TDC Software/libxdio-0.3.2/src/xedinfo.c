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
	printf("xedinfo [switches] <xed_file>\n\n");
	printf("switches:\n");
	printf("  -v            verbose, display as much information as possible\n");
	printf("  -e <index>    event, display information related to event at index <index>\n");
	exit(0);
}

int
main(int argc, char **argv)
{
	// switches
	int c = 0;

	int verbose = 0;
	int event_index = -1;

	// parse switches
	while((c = getopt(argc, argv, "ve:")) != -1)
	{
		switch(c)
		{
			case 'v':
				verbose = 1;
				break;

			case 'e':
				event_index = atoi(optarg);
				break;

			default:
				usage();
		}
	}

	if(argc < 2)
		usage();

	char *filename = strdup(argv[optind]);
	
	/* open a file */
	xdio_file_t *file = xdio_open_read(filename);

	int nb_events = xdio_get_nb_events(file);

	/* print the file layer information */
	printf("-- xdio file layer --\n");
	printf("filename:            %s\n", xdio_get_filename(file));
	time_t creation_time = xdio_get_time_created(file);
	printf("creation time (UTC): %s", asctime(gmtime((&creation_time))));
	printf("number of events:    %d\n", nb_events);

	/* consider that the events all have the same format (same chunk types, etc) */
	int same_event_format = 1;

	/* save the format of the first event */
	char *event_format;
	if(nb_events)
	{
		xdio_event_t *event = xdio_get_event(0, file);
		event_format = strdup(xdio_get_format(event));
	}

	/* check if all chunks have the same format */
	int index_nb;
	for(index_nb = 0; index_nb < nb_events; index_nb++)
	{
		xdio_event_t *event = xdio_get_event(index_nb, file);

		if(strcmp(event_format, xdio_get_format(event)))
		{
			same_event_format = 0;
			break;
		}
	}

	if(nb_events && same_event_format)
	{
		printf("event format:       ");
		xdio_event_t *event = xdio_get_event(0, file);
		
		int chunk_nb;
		for(chunk_nb = 0; chunk_nb < xdio_get_nb_chunks(event); chunk_nb++)
		{
			xdio_chunk_t *chunk = xdio_get_chunk(chunk_nb, event);

			xdio_chunk_type_t type;
			xdio_get_chunk_type(type, chunk);
			printf(" %c%c%c%c", type[0], type[1], type[2], type[3]);
		}
		printf("\n");
	}
	else
		printf("event format: miscellaneous\n");

	if(event_index != -1)
	{
		xdio_event_t *event = xdio_get_event(event_index, file);

		printf("-- xdio event layer --\n");
		printf("event number:        %d\n", xdio_get_event_nb(event));
		printf("trigger time (s):    %u\n", xdio_get_trigger_time_sec(event));
		printf("trigger time (us):   %u\n", xdio_get_trigger_time_usec(event));
		printf("number of chunks:    %d\n", xdio_get_nb_chunks(event));

		printf("-- xdio chunk layer --\n");
		int chunk_nb;
		for(chunk_nb = 0; chunk_nb < xdio_get_nb_chunks(event); chunk_nb++)
		{
			xdio_chunk_t *chunk = xdio_get_chunk(chunk_nb, event);

			xdio_chunk_type_t type;
			xdio_get_chunk_type(type, chunk);

			printf("chunk index:         %d\n", chunk_nb);
			if(!strncmp(type, "raw0", 4))
			{
				xdio_chunk_raw0_att_t *raw0_att = xdio_get_attributes(chunk);

				printf("type:                raw0\n");
				printf("sample type:         %d bits\n", raw0_att->sample_type);
				printf("number of samples:   %d\n", raw0_att->nb_samples);
				printf("voltage range:       %0.2f V\n", raw0_att->voltage_range);
				printf("sampling frequency:  %.0f Hz\n", raw0_att->sampling_frequency);
				printf("number of channels:  %d\n", raw0_att->nb_channels);
			}
			else if(!strncmp(type, "zle0", 4))
			{
				xdio_chunk_zle0_att_t *zle0_att = xdio_get_attributes(chunk);

				printf("type:                zle0\n");
				printf("sample type:         %d bits\n", zle0_att->sample_type);
				printf("number of samples:   %d\n", zle0_att->nb_samples);
				printf("voltage range:       %0.2f V\n", zle0_att->voltage_range);
				printf("sampling frequency:  %.0f Hz\n", zle0_att->sampling_frequency);
				printf("number of channels:  %d\n", zle0_att->nb_channels);
				printf("channels included:   ");
				int ch_nb, previous_channel = zle0_att->channels[0], nb_consecutive_channels = 1;
				printf("%d", previous_channel);
				for(ch_nb = 1; ch_nb < zle0_att->nb_channels_included; ch_nb++)
				{
					int channel = zle0_att->channels[ch_nb];

					if(channel > previous_channel+1)
					{
						/* finish the previous group */
						if(nb_consecutive_channels > 1)
							printf("-%d, ", previous_channel);
						else
							printf(", ");

						/* start the new one */
						printf("%d", channel);

						nb_consecutive_channels = 1;
					}
					else
						nb_consecutive_channels++;

					previous_channel = channel;
				}
				printf("\n");
			}
			else
				printf("no details available for ths chunk type\n");
		}
	}

	xdio_close(file);

	return(0);
}

