#include "common.h"

#if HAVE_STRING_H
#	include <string.h>
#endif

#define XDIO_MAX_FORMAT_SIZE 2048
#define CHUNK_INC 8

/* create an event */
xdio_event_t *
xdio_create_event()
{
	xdio_event_t *event = (xdio_event_t *) malloc(sizeof(xdio_event_t));

	event->header = (xdio_event_header_t *) malloc(sizeof(xdio_event_header_t));
	memset(event->header->filename, 0, XDIO_FILE_FILENAME_LENGTH);
	event->header->time_sec = 0;
	event->header->time_usec = 0;
	event->header->event_nb = 0;
	event->header->nb_chunks = 0;

	event->nb_chunks = 0;
	event->max_chunks = 0;
	event->chunks = NULL;

	event->offset = 0;

	event->to_file = NULL;
	event->from_file = NULL;

	return(event);
}

/* destroy an event */
void
xdio_destroy_event(xdio_event_t *event)
{
	int c;

	/* free header */
	free(event->header);

	/* destroy the chunks */
	for(c = 0; c < event->nb_chunks; c++)
		xdio_destroy_chunk((xdio_chunk_t *) event->chunks[c]);

	/* free the chunk array itself */
	if(event->nb_chunks)
		free(event->chunks);

	/* free the event */
	free(event);
}

/* get the format of an event */
char *
xdio_get_format(xdio_event_t *event)
{
	static char format[XDIO_MAX_FORMAT_SIZE];

	char line[64];

	strcpy(format, "<event>\n");

	/* filename */
	strcat(format, "  <filename>");
	strcat(format, event->from_file->header->filename);
	strcat(format, "</filename>\n");

	/* number of chunks */
	strcat(format, "  <nb_chunks>");
	sprintf(line, "%d", event->nb_chunks);
	strcat(format, line);
	strcat(format, "</nb_chunks>\n");

	/* chunks */
	strcat(format, "  <chunks>\n");
	int c;
	for(c = 0; c < event->nb_chunks; c++)
		strcat(format, xdio_get_chunk_format((xdio_chunk_t *) event->chunks[c]));
	strcat(format, "  </chunks>\n");

	strcat(format, "</event>\n");

	return format;
}

/* set the trigger time of an event */
void
xdio_set_trigger_time(int32_t time_sec, int32_t time_usec, xdio_event_t *event)
{
	event->header->time_sec = time_sec;
	event->header->time_usec = time_usec;
}

/* get the seconds of the trigger time of an event */
int
xdio_get_trigger_time_sec(xdio_event_t *event)
{
	return(event->header->time_sec);
}

/* get the micro seconds of the trigger time of an event */
int
xdio_get_trigger_time_usec(xdio_event_t *event)
{
	return(event->header->time_usec);
}

/* get the event number */
int
xdio_get_event_nb(xdio_event_t *event)
{
	return(event->header->event_nb);
}

/* get the number of chunks of an event */
int
xdio_get_nb_chunks(xdio_event_t *event)
{
	return(event->nb_chunks);
}

/* write an event */
int
xdio_write_event(xdio_event_t *event)
{
	xdio_event_header_t *header = event->header;
	xdio_file_t *to_file = event->to_file;
	int ret = XDIO_RET_OK;

	/* check that the event is part of a file */
	if(to_file == NULL)
	{
		xdio_error("cannot write event not added to a file!\n");
		return(XDIO_RET_ERR);
	}

	/* if the event does not come from another xdio file */
	if(event->from_file == NULL)
	{
		int8_t *buf = (int8_t *) malloc(sizeof(xdio_event_header_t));
		int i, c;

		header->nb_chunks = event->nb_chunks;

		/* before writing anything, get the event offset in the file */
		event->offset = ftell(to_file->out);

		/* write event header */
		for(i = 0; i < XDIO_FILE_FILENAME_LENGTH; i++)
			xdio_save_int8(&buf[i], header->filename[i]);

		xdio_save_int32(&buf[XDIO_FILE_FILENAME_LENGTH+0], header->time_sec);
		xdio_save_int32(&buf[XDIO_FILE_FILENAME_LENGTH+4], header->time_usec);
		xdio_save_int32(&buf[XDIO_FILE_FILENAME_LENGTH+8], header->event_nb);
		xdio_save_int32(&buf[XDIO_FILE_FILENAME_LENGTH+12], header->nb_chunks);
		ret = xdio_write_data(buf, XDIO_EVENT_HEADER_SIZE, to_file);

		free(buf);

		/* write all chunks */
		for(c = 0; c < event->nb_chunks; c++)
			ret |= xdio_write_chunk((xdio_chunk_t *) event->chunks[c]);
	}
	else
	{
		/* before doing anything save the current offset in the "from" file */
		off_t saved_offset = ftell(event->from_file->in);

		/* get the offset in the file the event is from and seek there */
		off_t event_from_offset = event->offset;
		fseek(event->from_file->in, event_from_offset, SEEK_SET);

		/* now compute the size of the event */
		int event_size = XDIO_EVENT_HEADER_SIZE;
		int c;
		for(c = 0; c < event->nb_chunks; c++)
			event_size += xdio_get_chunk_size((xdio_chunk_t *) event->chunks[c]);

		/* get the offset in the file where it will be written */
		event->offset = ftell(to_file->out);

		/* copy the event */
		int8_t *buf = (int8_t *) malloc(event_size);
		ret |= xdio_read_data(buf, event_size, event->from_file);
		ret |= xdio_write_data(buf, event_size, event->to_file);
		free(buf);

		/* put the "from" file offset back to what it was */
		fseek(event->from_file->in, saved_offset, SEEK_SET);
	}

	return(ret);
}

/* read an event */
xdio_event_t *
xdio_read_event(xdio_file_t *file)
{
	xdio_event_t *event = xdio_create_event();

	int8_t *buf = (int8_t *) malloc(XDIO_EVENT_HEADER_SIZE);
	int i, c, ret;

	/* get the offset in the file */
	off_t event_offset = ftell(file->in);

	/* read event header */	
	ret = xdio_read_data(buf, XDIO_EVENT_HEADER_SIZE, file);

	for(i = 0; i < XDIO_FILE_FILENAME_LENGTH; i++)
		event->header->filename[i] = xdio_load_int8(&buf[i]);

	event->header->time_sec = xdio_load_int32(&buf[XDIO_FILE_FILENAME_LENGTH+0]);
	event->header->time_usec = xdio_load_int32(&buf[XDIO_FILE_FILENAME_LENGTH+4]);
	event->header->event_nb = xdio_load_int32(&buf[XDIO_FILE_FILENAME_LENGTH+8]);
	event->header->nb_chunks = xdio_load_int32(&buf[XDIO_FILE_FILENAME_LENGTH+12]);

	free(buf);

	event->nb_chunks = event->header->nb_chunks;
	event->max_chunks = CHUNK_INC*((event->nb_chunks % 8 == 0)?(event->nb_chunks/8):(event->nb_chunks/8 + 1));
	event->chunks = (struct xdio_chunk **) malloc(event->max_chunks*sizeof(xdio_chunk_t *));

	event->offset = event_offset;
	event->from_file = file;

	for(c = 0; c < event->nb_chunks; c++)
		event->chunks[c] = (struct xdio_chunk *) xdio_read_chunk(file, event);

	return(event);
}

/* add a chunk to an event */
void
xdio_add_chunk(xdio_chunk_t *chunk, xdio_event_t *event)
{
	if(event->nb_chunks >= event->max_chunks)
	{
		struct xdio_chunk **old_chunks = event->chunks;

		int c;

		event->chunks = (struct xdio_chunk **) malloc((event->max_chunks+CHUNK_INC)*sizeof(xdio_chunk_t *));

		for(c = 0; c < event->max_chunks; c++)
			event->chunks[c] =  old_chunks[c];

		free(old_chunks);

		event->max_chunks += CHUNK_INC;
	}

	event->chunks[event->nb_chunks] = (struct xdio_chunk *) chunk;
	event->nb_chunks++;

	chunk->event = event;
}

/* remove a chunk from an event */
xdio_chunk_t *
xdio_remove_chunk(int chunk_nb, xdio_event_t *event)
{
}

/* get a chunk from an event */
xdio_chunk_t *
xdio_get_chunk(int chunk_nb, xdio_event_t *event)
{
	if(chunk_nb >= event->nb_chunks)
	{
		xdio_error("event has only %d chunks\n", event->nb_chunks);
		return(NULL);
	}

	return((xdio_chunk_t *) event->chunks[chunk_nb]);
}

