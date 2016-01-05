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

#define MAX_EVENT_LIST_SIZE 10000

void
usage()
{
	printf("xedselect [switches] <xed_file> [<new_xed_file>]\n\n");
	printf("switches:\n");
	printf("  -v         verbose, display as much information as possible\n");
	printf("  -n         events selected using their event number instead of their index in the file\n");
	printf("  -e         leave empty events in the new file for those not selected\n");
	printf("  -f <file>  file with event list (indexes or event numbers)\n");
	exit(0);
}

int
cmp_int(const void *a, const void *b)
{
    return(*((const int *) a) - *((const int *) b));
}

int
main(int argc, char **argv)
{
	/* switches */
	int c = 0;

	int verbose = 0;
	int index_selection = 1;
	int event_number_selection = 0;
	int leave_empty_events = 0;
	char *event_list_filename = NULL;

	/* parse switches */
	while((c = getopt(argc, argv, "vnef:")) != -1)
	{
		switch(c)
		{
			case 'v':
				verbose = 1;
				break;

			case 'n':
				index_selection = 0;
				event_number_selection = 1;
				break;

			case 'e':
				leave_empty_events = 1;
				break;

			case 'f':
				event_list_filename = strdup(optarg);
				break;

			default:
				usage();
		}
	}

	if(optind >= argc || argc-optind > 2)
		usage();

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

	/* check that the xdio input file exists */
	if(access(in_filename, R_OK))
	{
		fprintf(stderr, "xedselect: error, cannot open input file %s!\n", argv[optind]);
		exit(EXIT_FAILURE);
	}

	/* we obtain the event list either by a file (-f switch) or through stdin (pipe) */
	FILE *event_list = stdin;
	if(event_list_filename)
	{
		event_list = fopen(event_list_filename, "r");
		if(event_list == NULL)
		{
			fprintf(stderr, "xedselect: error, cannot open event list file %s!\n", event_list_filename);
			exit(EXIT_FAILURE);
		}
	}

	/* read in the event list */
	int *selected_events = (int *) malloc(MAX_EVENT_LIST_SIZE*sizeof(int));
	int nb_selected_events = 0;

	while(fscanf(event_list, "%d\n", &selected_events[nb_selected_events]) != EOF)
		nb_selected_events++;

	qsort(selected_events, nb_selected_events, sizeof(int), cmp_int);

	/* open the input file */
	xdio_file_t *in_file = xdio_open_read(in_filename);
	int nb_events = xdio_get_nb_events(in_file);
	int first_event_nb = (leave_empty_events)?(xdio_get_first_event_nb(in_file)):(-1);
	int events_per_file = in_file->header->index_size;

	/* open the ouput file */
	xdio_file_t *out_file = xdio_open_write(out_filename, time(NULL), first_event_nb, events_per_file, 1);

	if(index_selection && !leave_empty_events)
	{
		/* go over all events */
		int i;
		for(i = 0; i < nb_selected_events; i++)
		{
			/* get the event from the input file */
			xdio_event_t *in_event = xdio_get_event(selected_events[i], in_file);

			/* add the event to the new file */
			xdio_add_event(in_event, out_file);
		}
	}
	else
	{
		/* go over all events in the file */
		int index_nb;
		for(index_nb = 0; index_nb < nb_events; index_nb++)
		{
			/* get the event from the input file */
			xdio_event_t *in_event = xdio_get_event(index_nb, in_file);

			int event_nb = xdio_get_event_nb(in_event);

			int *found_event = bsearch(&event_nb, selected_events, nb_selected_events, sizeof(int), cmp_int);

			/* if this is one of the selected events, add it to the out file */
			if(found_event)
			{
				/* add the event to the new file */
				xdio_add_event(in_event, out_file);
			}
			else if(leave_empty_events)
			{
				xdio_event_t *empty_event = xdio_create_event();

				xdio_add_event(empty_event, out_file);
			}
		}
	}

	xdio_close(in_file);
	xdio_close(out_file);

	free(in_filename);
	free(out_filename);

	exit(EXIT_SUCCESS);
}

