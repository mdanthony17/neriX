#include "common.h"

#if HAVE_STRING_H
#	include <string.h>
#endif

xdio_chunk_t *
xdio_create_chunk(xdio_chunk_type_t chunk_type, xdio_chunk_att_t *chunk_att)
{
	xdio_chunk_t *chunk = (xdio_chunk_t *) malloc(sizeof(xdio_chunk_t));

	if(xdio_check_chunk_type(chunk_type, "raw0"))
	{
		xdio_register_chunk_raw0_ops(chunk);
		chunk->obj_ = chunk->ops_->create_(chunk_att);
		((xdio_chunk_raw0_t *) chunk->obj_)->wrap_ = chunk;
		chunk->event = NULL;
	}
	else if(xdio_check_chunk_type(chunk_type, "zle0"))
	{
		xdio_register_chunk_zle0_ops(chunk);
		chunk->obj_ = chunk->ops_->create_(chunk_att);
		((xdio_chunk_zle0_t *) chunk->obj_)->wrap_ = chunk;
		chunk->event = NULL;
	}
	else
	{
		xdio_error("%4s, unknown chunk type!\n", chunk_type);
		return(NULL);
	}

	return(chunk);
}

void
xdio_destroy_chunk(xdio_chunk_t *chunk)
{
	chunk->ops_->destroy_(chunk->obj_);
	free(chunk->ops_);
	free(chunk);
}

int
xdio_write_chunk(xdio_chunk_t *chunk)
{
	/* check that chunk is part of an event */
	if(chunk->event == NULL)
	{
		xdio_error("cannot write chunk not added to an event!\n");
		return(XDIO_RET_ERR);
	}

	return(chunk->ops_->write_(chunk->obj_));
}

xdio_chunk_t *
xdio_read_chunk(xdio_file_t *file, xdio_event_t *event)
{
	xdio_chunk_t *chunk = NULL;
	int ret;

	/* read the chunk type */
	xdio_chunk_type_t chunk_type;
	if((ret = xdio_read_data(chunk_type, 4, file)) & XDIO_RET_ERR)
		return(NULL);

	/* create a new chunk object (we don't know its attributes yet, leave them undefined) */
	if((chunk = xdio_create_chunk(chunk_type, NULL)) == NULL)
		return(NULL);

	/* link it to its event */
	chunk->event = event;

	/* read the chunk */
	if(chunk->ops_->read_(chunk->obj_) & XDIO_RET_ERR)
	{
		xdio_destroy_chunk(chunk);
		return(NULL);
	}

	return(chunk);
}

char *
xdio_get_chunk_format(xdio_chunk_t *chunk)
{
	chunk->ops_->get_format_(chunk->obj_);
}

int
xdio_get_chunk_size(xdio_chunk_t *chunk)
{
	chunk->ops_->get_size_(chunk->obj_);
}

int
xdio_attach_data_src(int channel, int8_t *data, size_t length, xdio_chunk_t *chunk)
{
	return(chunk->ops_->attach_data_src_(channel, data, length, chunk->obj_));
}

int xdio_attach_multi_data_src(int first, int32_t mask, int8_t *data, size_t length, xdio_chunk_t *chunk)
{
	return(chunk->ops_->attach_multi_data_src_(first, mask, data, length, chunk->obj_));
}

int
xdio_attach_data_dst(int channel, int8_t *data, size_t length, xdio_chunk_t *chunk)
{
	return(chunk->ops_->attach_data_dst_(channel, data, length, chunk->obj_));
}

int
xdio_extract_data(xdio_chunk_t *chunk)
{
	return(chunk->ops_->extract_data_(chunk->obj_));
}

xdio_chunk_att_t *
xdio_get_attributes(xdio_chunk_t *chunk)
{
	return(chunk->ops_->get_attributes_(chunk->obj_));
}

/* get the type of a chunk */
void
xdio_get_chunk_type(xdio_chunk_type_t type, xdio_chunk_t *chunk)
{
	xdio_set_chunk_type(type, *((void **) chunk->obj_));
}

/* compare two chunk types */
int
xdio_chunk_type_cmp(xdio_chunk_type_t type, xdio_chunk_t *chunk)
{
	void *ptr = *((void **) chunk->obj_);
	return(!xdio_check_chunk_type(ptr, type));
}

void
xdio_set_chunk_type(xdio_chunk_type_t type1, xdio_chunk_type_t type2)
{
	type1[0] = type2[0];
	type1[1] = type2[1];
	type1[2] = type2[2];
	type1[3] = type2[3];
}

int
xdio_check_chunk_type(xdio_chunk_type_t type1, xdio_chunk_type_t type2)
{
	return((type1[0] == type2[0]) && (type1[1] == type2[1]) && (type1[2] == type2[2]) && (type1[3] == type2[3]));
}


