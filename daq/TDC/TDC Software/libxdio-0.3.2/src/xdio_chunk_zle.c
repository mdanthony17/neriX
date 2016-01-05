#include "common.h"

#if HAVE_STRING_H
#	include <string.h>
#endif
#if HAVE_LIBZ
#	include <zlib.h>
#endif
#if HAVE_LIBBZ2
#	include <bzlib.h>
#endif

/* CAUTION these definitions cannot be changed otherwise already written zle0 data will become unreadable */
#define GZIP_COMPRESSION_LEVEL  Z_BEST_SPEED
#define BZIP2_COMPRESSION_LEVEL 1

/* function protoypes for the 'chunk' interface */
static xdio_chunk_obj_t *xdio_create_zle0(xdio_chunk_att_t *chunk_att_);
static void xdio_destroy_zle0(xdio_chunk_obj_t *chunk_obj_);
static int xdio_write_zle0(xdio_chunk_obj_t *chunk_obj_);
static int xdio_read_zle0(xdio_chunk_obj_t *chunk_obj_);
static char *xdio_get_format_zle0(xdio_chunk_obj_t *chunk_obj_);
static int xdio_get_size_zle0(xdio_chunk_obj_t *chunk_obj_);
static int xdio_attach_data_src_zle0(int channel, int8_t *data, size_t length, xdio_chunk_obj_t *chunk_obj_);
static int xdio_attach_multi_data_src_zle0(int first, int32_t mask, int8_t *data, size_t length, xdio_chunk_obj_t *chunk_obj_);
static int xdio_attach_data_dst_zle0(int channel, int8_t *data, size_t length, xdio_chunk_obj_t *chunk_obj_);
static int xdio_attach_multi_data_dst_zle0(int nb_channels, int8_t **data, xdio_chunk_obj_t *chunk_obj_);
static int xdio_extract_data_zle0(xdio_chunk_obj_t *chunk_obj_);
static xdio_chunk_att_t * xdio_get_attributes_zle0(xdio_chunk_obj_t *chunk_obj_);

/* internal functions */
static int attach_data(int channel, int8_t *data, size_t length, xdio_chunk_zle0_t *chunk_obj);
static int get_nb_channels_included(int nb_channels, uint8_t *channel_mask);
static int get_channel_nb(int channel_index, int nb_channels, uint8_t *channel_mask);
static void add_channel_to_channel_mask(int channel, int nb_channels, uint8_t *channel_mask);
static int gzip_compress_blocks(xdio_chunk_zle0_t *chunk_obj);
static int gzip_decompress_blocks(xdio_chunk_zle0_t *chunk_obj);
static int bzip2_compress_blocks(xdio_chunk_zle0_t *chunk_obj);
static int bzip2_decompress_blocks(xdio_chunk_zle0_t *chunk_obj);

/* register zle0 chunk operations */
void
xdio_register_chunk_zle0_ops(xdio_chunk_t *chunk)
{
	xdio_chunk_ops_t *ops = (xdio_chunk_ops_t *) malloc(sizeof(xdio_chunk_ops_t));

	ops->create_ = xdio_create_zle0;
	ops->destroy_ = xdio_destroy_zle0;
	ops->write_ = xdio_write_zle0;
	ops->read_ = xdio_read_zle0;
	ops->get_format_ = xdio_get_format_zle0;
	ops->get_size_ = xdio_get_size_zle0;
	ops->attach_data_src_ = xdio_attach_data_src_zle0;
	ops->attach_multi_data_src_ = xdio_attach_multi_data_src_zle0;
	ops->attach_data_dst_ = xdio_attach_data_dst_zle0;
	ops->attach_multi_data_dst_ = xdio_attach_multi_data_dst_zle0;
	ops->extract_data_ = xdio_extract_data_zle0;
	ops->get_attributes_ = xdio_get_attributes_zle0;

	chunk->ops_ = ops;
}

/* create a zle0 chunk */
static xdio_chunk_obj_t *
xdio_create_zle0(xdio_chunk_att_t *chunk_att_)
{
	xdio_chunk_zle0_att_t *chunk_att = (xdio_chunk_zle0_att_t *) chunk_att_;
	xdio_chunk_zle0_t *chunk_obj = (xdio_chunk_zle0_t *) malloc(sizeof(xdio_chunk_zle0_t));

	chunk_obj->header = (xdio_chunk_zle0_header_t *) malloc(sizeof(xdio_chunk_zle0_header_t));
	xdio_chunk_zle0_header_t *header = chunk_obj->header;

	xdio_set_chunk_type(header->type, "zle0");
	if(chunk_att_ == NULL)
	{
		header->size = 0;
		header->sample_type = 0;
		header->flags = 0;
		header->nb_samples = 0;
		header->voltage_range = 0.;
		header->sampling_frequency = 0.;
		header->nb_channels = 0;
		header->channel_mask = NULL;

		chunk_obj->attributes = NULL;
		chunk_obj->channel_length = 0;
	}
	else
	{
		int i;

		header->size = XDIO_CHUNK_ZLE0_HEADER_SIZE(chunk_att->nb_channels);
		header->sample_type = chunk_att->sample_type;
		header->flags = chunk_att->flags;
		header->nb_samples = chunk_att->nb_samples;
		header->voltage_range = chunk_att->voltage_range;
		header->sampling_frequency = chunk_att->sampling_frequency;
		header->nb_channels = chunk_att->nb_channels;
		
		header->channel_mask = (uint8_t *) malloc(XDIO_CHUNK_ZLE0_HEADER_CHANNEL_MASK_SIZE(header->nb_channels));
		for(i = 0; i < XDIO_CHUNK_ZLE0_HEADER_CHANNEL_MASK_SIZE(header->nb_channels); i++)
			chunk_obj->header->channel_mask[i] = 0;

		chunk_obj->attributes = xdio_create_chunk_zle0_attributes(header->sample_type, header->flags, header->nb_samples, header->voltage_range, header->sampling_frequency, header->nb_channels);
		chunk_obj->attributes->bytes_per_sample = xdio_bytes_per_sample(header->sample_type);
		chunk_obj->channel_length = xdio_bytes_per_sample(chunk_att->sample_type)*chunk_att->nb_samples;
	}

	chunk_obj->nb_offsets = 0;
	chunk_obj->data_offsets = NULL;
	chunk_obj->data_lengths = NULL;

	chunk_obj->nb_blocks = 0;
	chunk_obj->max_blocks = 0;
	chunk_obj->blocks = NULL;
	chunk_obj->compressed_block = NULL;
	chunk_obj->decompressed_block = NULL;

	return((xdio_chunk_obj_t *) chunk_obj);
}

/* destroy a zle0 chunk */
static void
xdio_destroy_zle0(xdio_chunk_obj_t *chunk_obj_)
{
	xdio_chunk_zle0_t *chunk_obj = (xdio_chunk_zle0_t *) chunk_obj_;

	int flags = chunk_obj->header->flags;

	/* free attributes (also free the channel mask array) */
	if(chunk_obj->attributes != NULL)
	{
		if(chunk_obj->attributes->nb_channels_included != 0)
		{
			free(chunk_obj->attributes->channels);
			free(chunk_obj->attributes->channel_lengths);
		}
		free(chunk_obj->attributes);
		free(chunk_obj->header->channel_mask);
	}

	/* free header */
	free(chunk_obj->header);

	/* if we had offsets, free their array */
	if(chunk_obj->nb_offsets)
	{
		free(chunk_obj->data_offsets);
		free(chunk_obj->data_lengths);
	}

	/* free the array of block pointers (the caller is responsible to free the data) */
	if(chunk_obj->max_blocks)
	{
		int b;

		for(b = 0; b < chunk_obj->max_blocks; b++)
			if(chunk_obj->blocks[b] != NULL)
				free(chunk_obj->blocks[b]);

		free(chunk_obj->blocks);
	}

	/* free the compression/decompression blocks */
	if((flags & XDIO_F_ZLE0_GZIP) || (flags & XDIO_F_ZLE0_BZIP2))
	{
		if(chunk_obj->compressed_block != NULL)
		{
			free(chunk_obj->compressed_block->data);
			free(chunk_obj->compressed_block);
		}

		if(chunk_obj->decompressed_block != NULL)
		{
			free(chunk_obj->decompressed_block->data);
			free(chunk_obj->decompressed_block);
		}
	}

	/* free the chunk_obj */
	free(chunk_obj);
}

/* write a zle0 chunk */
static int
xdio_write_zle0(xdio_chunk_obj_t *chunk_obj_)
{
	xdio_chunk_zle0_t *chunk_obj = (xdio_chunk_zle0_t *) chunk_obj_;
	xdio_chunk_zle0_header_t *header = chunk_obj->header;
	xdio_file_t *file = chunk_obj->wrap_->event->to_file;

	int ret = XDIO_RET_OK;

	/* if the chunk should be gzip or bzip2 compressed */
	if(chunk_obj->nb_blocks && (header->flags & XDIO_F_ZLE0_GZIP))
		ret |= gzip_compress_blocks(chunk_obj);
	else if(chunk_obj->nb_blocks && (header->flags & XDIO_F_ZLE0_BZIP2))
		ret |= bzip2_compress_blocks(chunk_obj);

	int8_t *buf = (int8_t *) malloc(XDIO_CHUNK_ZLE0_HEADER_SIZE(header->nb_channels));
	int i, b;

	/* write zle0 header */
	xdio_save_int8(&buf[0], header->type[0]);
	xdio_save_int8(&buf[1], header->type[1]);
	xdio_save_int8(&buf[2], header->type[2]);
	xdio_save_int8(&buf[3], header->type[3]);
	xdio_save_int32(&buf[4], header->size);
	xdio_save_int16(&buf[8], header->sample_type);
	xdio_save_int16(&buf[10], header->flags);
	xdio_save_int32(&buf[12], header->nb_samples);
	xdio_save_float(&buf[16], header->voltage_range);
	xdio_save_float(&buf[20], header->sampling_frequency);
	xdio_save_int32(&buf[24], header->nb_channels);
	for(i = 0; i < XDIO_CHUNK_ZLE0_HEADER_CHANNEL_MASK_SIZE(header->nb_channels); i++)
		xdio_save_int8(&buf[28+i], header->channel_mask[i]);

	ret |= xdio_write_data(buf, XDIO_CHUNK_ZLE0_HEADER_SIZE(header->nb_channels), file);

	free(buf);

	/* write the block(s) */
	if(chunk_obj->nb_blocks && ((header->flags & XDIO_F_ZLE0_GZIP) || (header->flags & XDIO_F_ZLE0_BZIP2)))
		ret |= xdio_write_data(chunk_obj->compressed_block->data, chunk_obj->compressed_block->length, file);
	else
		for(b = 0; b < chunk_obj->max_blocks; b++)
			if(chunk_obj->blocks[b] != NULL)
				ret |= xdio_write_data(chunk_obj->blocks[b]->data, chunk_obj->blocks[b]->length, file);

	return(ret);
}

/* read a zle0 chunk */
static int
xdio_read_zle0(xdio_chunk_obj_t *chunk_obj_)
{
	xdio_chunk_zle0_t *chunk_obj = (xdio_chunk_zle0_t *) chunk_obj_;
	xdio_chunk_zle0_header_t *header = chunk_obj->header;
	xdio_file_t *file = chunk_obj->wrap_->event->from_file;

	int8_t *buf = (int8_t *) malloc(XDIO_CHUNK_ZLE0_HEADER_SIZE(0));
	int i, ret;

	/* assume that the chunk type has already been read, and reading until the channel mask */
	ret = xdio_read_data(&buf[4], XDIO_CHUNK_ZLE0_HEADER_SIZE(0)-4, file);

	/* read the zle0 header */
	header->size = xdio_load_int32(&buf[4]);
	header->sample_type = xdio_load_int16(&buf[8]);
	header->flags = xdio_load_int16(&buf[10]);
	header->nb_samples = xdio_load_int32(&buf[12]);
	header->voltage_range = xdio_load_float(&buf[16]);
	header->sampling_frequency = xdio_load_float(&buf[20]);
	header->nb_channels = xdio_load_int32(&buf[24]);

	int channel_mask_size = XDIO_CHUNK_ZLE0_HEADER_CHANNEL_MASK_SIZE(header->nb_channels);
	buf = realloc(buf, channel_mask_size);

	ret |= xdio_read_data(buf, channel_mask_size, file);

	header->channel_mask = (uint8_t *) malloc(channel_mask_size);
	for(i = 0; i < channel_mask_size; i++)
		header->channel_mask[i] = buf[i];

	/* attributes are now known */
	chunk_obj->attributes = xdio_create_chunk_zle0_attributes(header->sample_type, header->flags, header->nb_samples, header->voltage_range, header->sampling_frequency, header->nb_channels);
	chunk_obj->attributes->bytes_per_sample = xdio_bytes_per_sample(header->sample_type);
	chunk_obj->channel_length = xdio_bytes_per_sample(header->sample_type)*header->nb_samples;

	/* compute the actual number of channels included in the chunk */
	int nb_channels_included = get_nb_channels_included(header->nb_channels, header->channel_mask);
	chunk_obj->attributes->nb_channels_included = nb_channels_included;
	chunk_obj->nb_offsets = nb_channels_included;

	/* save the file offset to where the chunk data is */
	chunk_obj->data_offset = ftell(file->in);

	/* if no channels are included there is nothing left to do */
	if(!nb_channels_included)
	{
		free(buf);

		return(ret);
	}

	/* we need to parse the channel data to get the channel lengths and offsets */

	/* allocate the array for the included channels */
	chunk_obj->attributes->channels = (int *) malloc(nb_channels_included*sizeof(int));
	chunk_obj->attributes->channel_lengths = (int *) malloc(nb_channels_included*sizeof(int));

	/* allocate the array to save the file offset to where each channel data is */
	chunk_obj->data_offsets = (off_t *) malloc(header->nb_channels*sizeof(off_t));
	memset(chunk_obj->data_offsets, 0, header->nb_channels*sizeof(off_t));
	chunk_obj->data_lengths = (size_t *) malloc(header->nb_channels*sizeof(size_t));
	memset(chunk_obj->data_lengths, 0, header->nb_channels*sizeof(size_t));

	/* if the chunk is gzip or bzip2 compressed, decompress it first */
	int8_t *decompressed_data_ptr;
	if((header->flags & XDIO_F_ZLE0_GZIP) || (header->flags & XDIO_F_ZLE0_BZIP2))
	{
		xdio_block_t *compressed_block = (xdio_block_t *) malloc(sizeof(xdio_block_t));
		compressed_block->data = (int8_t *) malloc(header->size-XDIO_CHUNK_ZLE0_HEADER_SIZE(header->nb_channels));
		compressed_block->length = header->size-XDIO_CHUNK_ZLE0_HEADER_SIZE(header->nb_channels);
		
		ret = xdio_read_data(compressed_block->data, compressed_block->length, file);
		chunk_obj->compressed_block = compressed_block;

		/* decompress it */
		if(header->flags & XDIO_F_ZLE0_GZIP)
			ret |= gzip_decompress_blocks(chunk_obj);
		else if(header->flags & XDIO_F_ZLE0_BZIP2)
			ret |= bzip2_decompress_blocks(chunk_obj);

		decompressed_data_ptr = chunk_obj->decompressed_block->data;
	}

	/* parse the data to get the channel offsets */
	int channel_count = 0;
	while(channel_count < chunk_obj->nb_offsets)
	{
		off_t data_offset;
		size_t data_length;

		/* if the chunk is gzip or bzip2 compressed */
		if((header->flags & XDIO_F_ZLE0_GZIP) || (header->flags & XDIO_F_ZLE0_BZIP2))
		{
			/* get the offset and read the length of the channel written by the V1724 (number of 4-byte blocks) */
			data_offset = decompressed_data_ptr-chunk_obj->decompressed_block->data;
			data_length = 4*xdio_load_int32(decompressed_data_ptr);

			/* jump to the next channel */
			decompressed_data_ptr += data_length;
		}
		/* the chunk was not compressed */
		else
		{
			/* get the offset and read the length of the channel written by the V1724 (number of 4-byte blocks) */
			data_offset = ftell(file->in);
			ret |= xdio_read_data(buf, 4, file);
			data_length = 4*xdio_load_int32(buf);

			/* seek to the next channel */
			fseek(file->in, data_length - 4, SEEK_CUR);
		}

		/* save the data channel, offset and length */
		int data_channel = get_channel_nb(channel_count, header->nb_channels, header->channel_mask);
		chunk_obj->attributes->channels[channel_count] = data_channel;
		chunk_obj->attributes->channel_lengths[channel_count] = data_length;
		chunk_obj->data_offsets[data_channel] = data_offset;
		chunk_obj->data_lengths[data_channel] = data_length;
		channel_count++;
	}	

	/* seek to the next chunk - possibly double counts */
	//fseek(file->in, header->size - XDIO_CHUNK_ZLE0_HEADER_SIZE(header->nb_channels), SEEK_CUR);
	
	free(buf);

	return(ret);
}

/* get the format of a zle0 chunk */
static char *
xdio_get_format_zle0(xdio_chunk_obj_t *chunk_obj_)
{
	xdio_chunk_zle0_t *chunk_obj = (xdio_chunk_zle0_t *) chunk_obj_;
	xdio_chunk_zle0_header_t *header = chunk_obj->header;

	static char format[512];
	char line[64];

	/* chunk type */
	strcpy(format, "    <zle0>\n");

	/* sample type */
	sprintf(line, "      <sample_type>%d</sample_type>\n", header->sample_type);
	strcat(format, line);

	/* flags */
	sprintf(line, "      <flags>%hx</flags>\n", header->flags);
	strcat(format, line);

	/* number of samples */
	sprintf(line, "      <nb_samples>%d</nb_samples>\n", header->nb_samples);
	strcat(format, line);

	/* voltage range */
	sprintf(line, "      <voltage_range>%0.2f</voltage_range>\n", header->voltage_range);
	strcat(format, line);

	/* sampling frequency */
	sprintf(line, "      <sampling_frequency>%.0f</sampling_frequency>\n", header->sampling_frequency);
	strcat(format, line);

	/* number of channels */
	sprintf(line, "      <nb_channels>%d</nb_channels>\n", header->nb_channels);
	strcat(format, line);

	strcat(format, "    </zle0>\n");

	return(format);
}

/* get the size of a zle0 chunk */
static int
xdio_get_size_zle0(xdio_chunk_obj_t *chunk_obj_)
{
	xdio_chunk_zle0_t *chunk_obj = (xdio_chunk_zle0_t *) chunk_obj_;
	xdio_chunk_zle0_header_t *header = chunk_obj->header;

	return(header->size);
}

/* attach data source to a zle0 chunk */
static int
xdio_attach_data_src_zle0(int channel, int8_t *data, size_t length, xdio_chunk_obj_t *chunk_obj_)
{
	xdio_chunk_zle0_t *chunk_obj = (xdio_chunk_zle0_t *) chunk_obj_;

	return(attach_data(channel, data, length, chunk_obj));
}

/* attach multi data source to a zle0 chunk */
static int
xdio_attach_multi_data_src_zle0(int first, int32_t mask, int8_t *data, size_t length, xdio_chunk_obj_t *chunk_obj_)
{
	xdio_chunk_zle0_t *chunk_obj = (xdio_chunk_zle0_t *) chunk_obj_;

	/* count the number of channels */
	int nb_channels_to_add = xdio_count_bits(mask);

	int channel = first;
	int8_t *channel_data = data;

	int bit = 0;
	while(bit < 32)
	{
		if((1 << bit++) & mask)
		{
			/* read the length of the channel written by the V1724 (number of 4-byte blocks) */
			size_t channel_length = 4*xdio_load_int32(channel_data);

			if(attach_data(channel, channel_data, channel_length, chunk_obj) != XDIO_RET_OK)
				return(XDIO_RET_ERR);

			channel_data = channel_data + channel_length;
		}

		channel++;
	}

	return(XDIO_RET_OK);
}

/* attach data destination to a zle0 chunk */
static int
xdio_attach_data_dst_zle0(int channel, int8_t *data, size_t length, xdio_chunk_obj_t *chunk_obj_)
{
	xdio_chunk_zle0_t *chunk_obj = (xdio_chunk_zle0_t *) chunk_obj_;

	return(attach_data(channel, data, length, chunk_obj));
}

/* attach multi data destination to a zle0 chunk */
static int
xdio_attach_multi_data_dst_zle0(int nb_channels, int8_t **data, xdio_chunk_obj_t *chunk_obj_)
{
	xdio_error("'zle0' chunk has no implementation for attach_multi_data_dst!\n");

	return(XDIO_RET_ERR);
}

/* extract the chunk data to the attached data destinations */
static int
xdio_extract_data_zle0(xdio_chunk_obj_t *chunk_obj_)
{
	xdio_chunk_zle0_t *chunk_obj = (xdio_chunk_zle0_t *) chunk_obj_;
	xdio_file_t *file = chunk_obj->wrap_->event->from_file;
	xdio_chunk_zle0_header_t *header = chunk_obj->header;
	int flags = header->flags;

	int ret = XDIO_RET_OK;

	/* nothing to do if the chunk doesn't have any channel data */
	if(chunk_obj->nb_offsets == 0)
		return(XDIO_RET_OK);

	/* for all attached blocks */
	int b;
	for(b = 0; b < chunk_obj->max_blocks; b++)
 	{
 		if(chunk_obj->blocks[b] != NULL)
 		{
			/* if this was compressed data just copy the channel data from the decompressed block */
			if((flags & XDIO_F_ZLE0_GZIP) || (flags & XDIO_F_ZLE0_BZIP2))
				memcpy(chunk_obj->blocks[b]->data, chunk_obj->decompressed_block->data+chunk_obj->data_offsets[b], chunk_obj->data_lengths[b]);
			else
			{
				/* fseek to the channel data and write data to the user supplied space */
				fseek(file->in, chunk_obj->data_offsets[b], SEEK_SET);
				ret |= xdio_read_data(chunk_obj->blocks[b]->data, chunk_obj->data_lengths[b], file);
			}
 		}
 	}

	return(ret);
}

/* get attributes of a zle0 chunk */
static xdio_chunk_att_t *
xdio_get_attributes_zle0(xdio_chunk_obj_t *chunk_obj_)
{
	xdio_chunk_zle0_t *chunk_obj = (xdio_chunk_zle0_t *) chunk_obj_;

	return(chunk_obj->attributes);
}

/* create a zle0 chunk attributes object */
xdio_chunk_att_t *
xdio_create_chunk_zle0_attributes(int sample_type, int flags, int nb_samples,
	float voltage_range, float sampling_frequency, int nb_channels)
{
	xdio_chunk_zle0_att_t *chunk_att = (xdio_chunk_zle0_att_t *) malloc(sizeof(xdio_chunk_zle0_att_t));

	chunk_att->sample_type = sample_type;
	chunk_att->flags = flags;
	chunk_att->nb_samples = nb_samples;
	chunk_att->voltage_range = voltage_range;
	chunk_att->sampling_frequency = sampling_frequency;
	chunk_att->nb_channels = nb_channels;

	chunk_att->bytes_per_sample = xdio_bytes_per_sample(chunk_att->sample_type);
	chunk_att->nb_channels_included = 0;
	chunk_att->channels = NULL;
	chunk_att->channel_lengths = NULL;

	return(chunk_att);
}

/* attach data source or destination to a zle0 chunk */
static int
attach_data(int channel, int8_t *data, size_t length, xdio_chunk_zle0_t *chunk_obj)
{
	/* check that the attributes are known */
	if(chunk_obj->attributes == NULL)
	{
		xdio_error("'zle0' chunk attributes are not known!\n");
		return(XDIO_RET_ERR);
	}

	/* if the channel is out of bounds */
	if((channel < 0) || (channel > chunk_obj->header->nb_channels-1))
	{
		xdio_error("channel %d is out of bounds!\n", channel);
		return(XDIO_RET_ERR);
	}

	/* if it's the first time we are attaching a data source or destination */
	if(!chunk_obj->max_blocks)
	{
		/* allocate the array of blocks and initialize it */
		chunk_obj->max_blocks = chunk_obj->header->nb_channels;
		chunk_obj->blocks = (xdio_block_t **) malloc(chunk_obj->max_blocks*sizeof(xdio_block_t *));

		int b;

		for(b = 0; b < chunk_obj->max_blocks; b++)
			chunk_obj->blocks[b] = NULL;
	}

	/* if the channel is already in use */
	if(chunk_obj->blocks[channel] != NULL)
	{
		xdio_error("'zle0' chunk channel %d already has a source or destination!\n", channel);
		return(XDIO_RET_ERR);
	}

	/* everything seems fine, add the block */
	xdio_block_t *block = (xdio_block_t *) malloc(sizeof(xdio_block_t));
	block->data = data;
	block->length = length;

	chunk_obj->blocks[channel] = block;
	chunk_obj->nb_blocks++;

	/* if there is no event associated yet, we are in writing mode */
	if(chunk_obj->wrap_->event == NULL)
	{
		add_channel_to_channel_mask(channel, chunk_obj->header->nb_channels, chunk_obj->header->channel_mask);
		chunk_obj->header->size += length;
	}

	return(XDIO_RET_OK);
}

/* get the number of channels included in the channel mask */
static int
get_nb_channels_included(int nb_channels, uint8_t *channel_mask)
{
	/* compute the number of groups of four bytes */
	int nb_words = XDIO_CHUNK_ZLE0_HEADER_CHANNEL_MASK_SIZE(nb_channels)/4;

	int channel_count = 0, word;
	for(word = 0; word < nb_words; word++)
		channel_count += xdio_count_bits(*((uint32_t *) &channel_mask[4*word]));

	return(channel_count);
}

/* get the channel number in the channel mask */
static int
get_channel_nb(int channel_index, int nb_channels, uint8_t *channel_mask)
{
    int nb_bytes = XDIO_CHUNK_ZLE0_HEADER_CHANNEL_MASK_SIZE(nb_channels);

	int channel_count = -1, byte = nb_bytes-1;
	do
	{
        channel_count += xdio_count_bits(channel_mask[byte--]);
	} while ((channel_index > channel_count) && (byte > 0));

	int channel_nb = -1;
	if(channel_index <= channel_count)
	{
		uint8_t mask = channel_mask[++byte];
		channel_count -= xdio_count_bits(mask);

		channel_nb = 8*(nb_bytes-1-byte)-1;

		do
		{
			if(mask & 1)
				channel_count++;

			channel_nb++;
			mask >>= 1;
		} while(channel_index > channel_count);
	}

    return(channel_nb);
}

/* add a channel to a channel mask bit field */
static void
add_channel_to_channel_mask(int channel, int nb_channels, uint8_t *channel_mask)
{
	channel_mask[XDIO_CHUNK_ZLE0_HEADER_CHANNEL_MASK_SIZE(nb_channels)-1-channel/8] |= (1 << (channel % 8));
}

/* compress the data blocks of a zle0 chunk into a single gzip compressed block */
static int
gzip_compress_blocks(xdio_chunk_zle0_t *chunk_obj)
{
	z_stream strm;

	strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;

	if(deflateInit(&strm, GZIP_COMPRESSION_LEVEL) != Z_OK)
	{
		xdio_error("could not initialize zlib!\n");
		return(XDIO_RET_ERR);
	}

	int maximum_length = chunk_obj->header->nb_channels*chunk_obj->channel_length;

	xdio_block_t *compressed_block = (xdio_block_t *) malloc(sizeof(xdio_block_t));
	compressed_block->data = (int8_t *) malloc(maximum_length);

	strm.avail_out = maximum_length;
	strm.next_out = compressed_block->data;

	int b;

	/* pass attached blocks to deflate */
	for(b = 0; b < chunk_obj->max_blocks; b++)
	{
 		if(chunk_obj->blocks[b] != NULL)
 		{
			strm.avail_in = chunk_obj->blocks[b]->length;
			strm.next_in = chunk_obj->blocks[b]->data;
			
			if(deflate(&strm, Z_NO_FLUSH) != Z_OK)
			{
				deflateEnd(&strm);
				xdio_error("zlib compression failed!\n");
				return(XDIO_RET_ERR);
			}
		}
	}

	if(deflate(&strm, Z_FINISH) != Z_STREAM_END)
	{
		deflateEnd(&strm);
		xdio_error("unexpected return value from zlib!\n");
		return(XDIO_RET_ERR);
	}
	
	compressed_block->length = maximum_length-strm.avail_out;
	chunk_obj->header->size = XDIO_CHUNK_ZLE0_HEADER_SIZE(chunk_obj->header->nb_channels) + compressed_block->length;
	chunk_obj->compressed_block = compressed_block;

	deflateEnd(&strm);

	return(XDIO_RET_OK);
}

/* decompress the gzip block into data blocks of a zle0 chunk */
static int
gzip_decompress_blocks(xdio_chunk_zle0_t *chunk_obj)
{
	z_stream strm;

	strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;

	if(inflateInit(&strm) != Z_OK)
	{
		xdio_error("could not initialize zlib!\n");
		return(XDIO_RET_ERR);
	}

	strm.next_in = chunk_obj->compressed_block->data;
	strm.avail_in = chunk_obj->compressed_block->length;

	int maximum_length = chunk_obj->header->nb_channels*chunk_obj->channel_length;

	xdio_block_t *decompressed_block = (xdio_block_t *) malloc(sizeof(xdio_block_t));
	decompressed_block->data = (int8_t *) malloc(maximum_length);	

	/* inflate */
	strm.next_out = decompressed_block->data;
	strm.avail_out = maximum_length;

	if(inflate(&strm, Z_NO_FLUSH) != Z_STREAM_END)
	{
		inflateEnd(&strm);
		xdio_error("zlib decompression failed!\n");
		return(XDIO_RET_ERR);
	}

	decompressed_block->length = maximum_length-strm.avail_out;
	chunk_obj->decompressed_block = decompressed_block;

	inflateEnd(&strm);

	return(XDIO_RET_OK);
}

/* compress the data blocks of a zle0 chunk into a single bzip2 compressed block */
static int
bzip2_compress_blocks(xdio_chunk_zle0_t *chunk_obj)
{
	bz_stream strm;

	strm.bzalloc = NULL;
    strm.bzfree = NULL;
    strm.opaque = NULL;

	if(BZ2_bzCompressInit(&strm, BZIP2_COMPRESSION_LEVEL, 0, 30) != BZ_OK)
	{
		xdio_error("could not initialize bzip2!\n");
		return(XDIO_RET_ERR);
	}

	int maximum_length =  chunk_obj->header->nb_channels*chunk_obj->channel_length;

	xdio_block_t *compressed_block = (xdio_block_t *) malloc(sizeof(xdio_block_t));
	compressed_block->data = (int8_t *) malloc(maximum_length);

	strm.avail_out = maximum_length;
	strm.next_out = compressed_block->data;

	int b;

	/* pass all blocks to bzip2 */
	for(b = 0; b < chunk_obj->max_blocks; b++)
	{
 		if(chunk_obj->blocks[b] != NULL)
 		{
			strm.avail_in = chunk_obj->blocks[b]->length;
			strm.next_in = chunk_obj->blocks[b]->data;
			
			if(BZ2_bzCompress(&strm, BZ_RUN) != BZ_RUN_OK)
			{
				xdio_error("bzip2 compression failed!\n");
				return(XDIO_RET_ERR);
			}
		}
	}

	if(BZ2_bzCompress(&strm, BZ_FINISH) != BZ_STREAM_END)
	{
		xdio_error("unexpected return value from bzip2!\n");
		return(XDIO_RET_ERR);
	}
	
	compressed_block->length = maximum_length-strm.avail_out;
	chunk_obj->header->size = XDIO_CHUNK_ZLE0_HEADER_SIZE(chunk_obj->header->nb_channels) + compressed_block->length;
	chunk_obj->compressed_block = compressed_block;

	BZ2_bzCompressEnd(&strm);

	return(XDIO_RET_OK);
}

/* decompress the bzip2 block into data blocks of a zle0 chunk */
static int
bzip2_decompress_blocks(xdio_chunk_zle0_t *chunk_obj)
{
	bz_stream strm;

	strm.bzalloc = NULL;
    strm.bzfree = NULL;
    strm.opaque = NULL;

	if(BZ2_bzDecompressInit(&strm, 0, 0) != BZ_OK)
	{
		xdio_error("could not initialize bzip2!\n");
		return(XDIO_RET_ERR);
	}

	strm.next_in = chunk_obj->compressed_block->data;
	strm.avail_in = chunk_obj->compressed_block->length;

	int maximum_length = chunk_obj->header->nb_channels*chunk_obj->channel_length;

	xdio_block_t *decompressed_block = (xdio_block_t *) malloc(sizeof(xdio_block_t));
	decompressed_block->data = (int8_t *) malloc(maximum_length);	

	/* bunzip */
	strm.next_out = decompressed_block->data;
	strm.avail_out = maximum_length;

	if(BZ2_bzDecompress(&strm) != BZ_STREAM_END)
	{
		BZ2_bzDecompressEnd(&strm);
		xdio_error("bzip2 decompression failed!\n");
		return(XDIO_RET_ERR);
	}

	decompressed_block->length = maximum_length-strm.avail_out;
	chunk_obj->decompressed_block = decompressed_block;

	BZ2_bzDecompressEnd(&strm);

	return(XDIO_RET_OK);
}

