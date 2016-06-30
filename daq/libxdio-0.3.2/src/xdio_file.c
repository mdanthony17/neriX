#include "common.h"

#if HAVE_STRING_H
#	include <string.h>
#endif

/* static functions */
static int write_file_header(xdio_file_t *file);
static int read_file_header(xdio_file_t *file);
static int flush_event_buffer_to_disk(xdio_file_t *file);

/* open an xdio file for writing  */
xdio_file_t *
xdio_open_write(const char *filename, time_t time, int first_event_nb, int desired_nb_events, int event_buffer_size)
{
	xdio_file_t *file;
	char *slash, *filename_base;
	int i;

	/* check if there is a path associated with the filename */
	if(slash = rindex(filename, '/'))
		filename_base = slash+1;
	else
		filename_base = (char *) filename;

	/* check filename size limit (set by the xdio format) */
	if(strlen(filename_base) > XDIO_FILE_FILENAME_LENGTH)
	{
		xdio_error("filename is limited to %d characters\n", XDIO_FILE_FILENAME_LENGTH);
		return(NULL);
	}

	file = (xdio_file_t *) malloc(sizeof(xdio_file_t));
	file->header = (xdio_file_header_t *) malloc(sizeof(xdio_file_header_t));

	/* initialize the file header */
	memset(file->header->filename, 0, XDIO_FILE_FILENAME_LENGTH);
	file->header->time = time;
	file->header->first_event_nb = (int32_t) first_event_nb;
	file->header->nb_events = 0;
	file->header->index_size = desired_nb_events;
	file->header->index = (int32_t *) malloc(4*desired_nb_events);
	for(i = 0; i < desired_nb_events; i++)
		file->header->index[i] = XDIO_FILE_NO_EVENT_INDEX;

	memcpy(file->header->filename, filename_base, strlen(filename_base));

	/* set the file mode */
	file->mode = XDIO_MODE_WRITE;
	file->in = NULL;
	file->out = fopen(filename, "wb");

	if(file->out == NULL)
	{
		free(file->header);
		free(file);

		xdio_error("cannot open file %s\n", filename);
		return(NULL);
	}

	/* set file parameters */
	file->desired_nb_events = desired_nb_events;
	file->nb_events_added = 0;
	file->event_buffer_size = event_buffer_size;

	file->nb_events_in_buffer = 0;
	file->events = (struct xdio_event **) malloc(event_buffer_size*sizeof(xdio_event_t *));

	/* skip space for the file header */
	fseek(file->out, XDIO_FILE_HEADER_SIZE(file->desired_nb_events), SEEK_SET);

	return(file);
}

/* open an xdio file for reading  */
xdio_file_t *
xdio_open_read(const char *filename)
{
	xdio_file_t *file;
	int i;

	file = (xdio_file_t *) malloc(sizeof(xdio_file_t));
	file->header = (xdio_file_header_t *) malloc(sizeof(xdio_file_header_t));

	/* set the file mode */
	file->mode = XDIO_MODE_READ;
	file->out = NULL;
	file->in = fopen(filename, "rb");

	if(file->in == NULL)
	{
		free(file->header);
		free(file);

		xdio_error("cannot open file %s\n", filename);
		return(NULL);
	}

	/* read the file header */
	read_file_header(file);

	/* set file parameters */
	file->desired_nb_events = -1;
	file->nb_events_added = -1;
	file->event_buffer_size = -1;

	/* we keep space for one event at a time in the event buffer */
	file->nb_events_in_buffer = 0;
	file->events = (struct xdio_event **) malloc(sizeof(xdio_event_t *));

	return(file);
}

/* close an xdio file */
void
xdio_close(xdio_file_t *file)
{
	if(file->mode & XDIO_MODE_WRITE)
	{
		/* write any events still in the event buffer */
		if(file->nb_events_in_buffer)
			flush_event_buffer_to_disk(file);

		/* now that we know where all events are, write the file header */
		fseek(file->out, 0, SEEK_SET); 
		write_file_header(file);

		fclose(file->out);
	}
	else if(file->mode & XDIO_MODE_READ)
	{
		/* delete everything still left in the event buffer */
		if(file->nb_events_in_buffer)
		{
			file->nb_events_in_buffer = 0;
			xdio_destroy_event(file->events[0]);
		}

		fclose(file->in);
	}

	/* free the header */
	free(file->header->index);
	free(file->header);

	/* free the event buffer */
	free(file->events);

	/* free the file */
	free(file);
}

/* get the original filename of an xdio file */
char *
xdio_get_filename(xdio_file_t *file)
{
	return(file->header->filename);
}

/* get the creation time of an xdio file */
time_t
xdio_get_time_created(xdio_file_t *file)
{
	return(file->header->time);
}

/* get the event number of the first event */
int
xdio_get_first_event_nb(xdio_file_t *file)
{
	return(file->header->first_event_nb);
}

/* get the number of events in an xdio file */
int
xdio_get_nb_events(xdio_file_t *file)
{
	if(file->mode & XDIO_MODE_READ)
		return(file->header->nb_events);
	else if(file->mode & XDIO_MODE_WRITE)
		return(file->nb_events_added);
}

/* write the file header of an xdio file */
static int
write_file_header(xdio_file_t *file)
{
	xdio_file_header_t *header = file->header;

	int8_t *buf = (int8_t *) malloc(XDIO_FILE_HEADER_SIZE(file->desired_nb_events));
	int i, ret;

	for(i = 0; i < XDIO_FILE_FILENAME_LENGTH; i++)
		xdio_save_int8(&buf[i], header->filename[i]);

	xdio_save_int32(&buf[XDIO_FILE_FILENAME_LENGTH], header->time);
	xdio_save_int32(&buf[XDIO_FILE_FILENAME_LENGTH+4], header->first_event_nb);
	xdio_save_int32(&buf[XDIO_FILE_FILENAME_LENGTH+8], header->nb_events);
	xdio_save_int32(&buf[XDIO_FILE_FILENAME_LENGTH+12], header->index_size);

	for(i = 0; i < header->index_size; i++)
		xdio_save_int32(&buf[XDIO_FILE_FILENAME_LENGTH+16+4*i], header->index[i]);

	ret = xdio_write_data(buf, XDIO_FILE_HEADER_SIZE(file->desired_nb_events), file);

	free(buf);

	return(ret);
}

/* add an event to an xdio file */
int
xdio_add_event(xdio_event_t *event, xdio_file_t *file)
{
	int ret = XDIO_RET_OK;

	/* link the event to its file */
	event->to_file = file;

	/* if the event does not come from another xdio file */
	if(event->from_file == NULL)
	{
		/* save the filename and event number in the header */
		memcpy(event->header->filename, file->header->filename, XDIO_FILE_FILENAME_LENGTH);	
		event->header->event_nb = file->header->first_event_nb + file->nb_events_added;
	}

	/* insert the event in the event buffer */
	file->events[file->nb_events_in_buffer] = event;
	file->nb_events_in_buffer++;
	file->nb_events_added++;

	/* if we filled the buffer, write the events to file */
	if(file->nb_events_in_buffer == file->event_buffer_size)
		flush_event_buffer_to_disk(file);

	return(ret);
}

/* remove an xdio event */
xdio_event_t *
xdio_remove_event(int event_nb, xdio_file_t *file)
{
}

/* get an xdio event */
xdio_event_t *
xdio_get_event(int index_nb, xdio_file_t *file)
{
	/* check that we are in reading mode */
	if(!(file->mode & XDIO_MODE_READ))
	{
		xdio_error("file is not opened for reading\n");
		return(NULL);
	}

	/* check that the index_nb is in bounds */
	if((index_nb > file->header->index_size) || (file->header->index[index_nb] == XDIO_FILE_NO_EVENT_INDEX))
	{
		xdio_error("file has no event at index %d\n", index_nb);
		return(NULL);
	}

	/* position the file to the event offset */
    if(fseek(file->in, (uint32_t) file->header->index[index_nb], SEEK_SET) != 0)
    {
        xdio_error("problem seeking to event at index %d\n", index_nb);
        return(NULL);
    }

	xdio_event_t *event = xdio_read_event(file);

	/* if there is another event in the buffer free it and put this one instead */
	if(file->nb_events_in_buffer)
	{
		file->nb_events_in_buffer = 0;
		xdio_destroy_event(file->events[0]);
	}
	file->events[0] = event;
	file->nb_events_in_buffer = 1;

	return(event);
}

/* read the file header of an xdio file */
static int
read_file_header(xdio_file_t *file)
{
	xdio_file_header_t *header = file->header;

	int8_t *buf = (int8_t *) malloc(XDIO_FILE_HEADER_SIZE(0));
	int i, ret;

	ret = xdio_read_data(buf, XDIO_FILE_HEADER_SIZE(0), file);

	for(i = 0; i < XDIO_FILE_FILENAME_LENGTH; i++)
		header->filename[i] = xdio_load_int8(&buf[i]);

	header->time = xdio_load_int32(&buf[XDIO_FILE_FILENAME_LENGTH]);
	header->first_event_nb = xdio_load_int32(&buf[XDIO_FILE_FILENAME_LENGTH+4]);
	header->nb_events = xdio_load_int32(&buf[XDIO_FILE_FILENAME_LENGTH+8]);
	header->index_size = xdio_load_int32(&buf[XDIO_FILE_FILENAME_LENGTH+12]);

	file->header->index = (int32_t *) malloc(4*header->index_size);	

	buf = realloc(buf, 4*header->index_size);
	ret |= xdio_read_data(buf, 4*header->index_size, file);

	for(i = 0; i < header->index_size; i++)
		header->index[i] = xdio_load_int32(&buf[4*i]);

	free(buf);

	return(ret);
}

/* flush all events in the event buffer to the disk */
static int
flush_event_buffer_to_disk(xdio_file_t *file)
{
	int ret = XDIO_RET_OK, e = 0;

	while(file->nb_events_in_buffer)
	{
		xdio_event_t *event = file->events[e++];

		if((ret |= xdio_write_event(event)) == XDIO_RET_ERR)
			break;

		file->nb_events_in_buffer--;
		file->header->index[file->header->nb_events] = event->offset;
		file->header->nb_events++;
		if(event->from_file == NULL)
			xdio_destroy_event(event);
	}

	return(ret);
}

