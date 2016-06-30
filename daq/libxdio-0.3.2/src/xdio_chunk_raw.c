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

/* CAUTION these definitions cannot be changed otherwise already written raw0 data will become unreadable */
#define GZIP_COMPRESSION_LEVEL  Z_BEST_SPEED
#define BZIP2_COMPRESSION_LEVEL 1

/* function protoypes for the 'chunk' interface */
static xdio_chunk_obj_t *xdio_create_raw0(xdio_chunk_att_t *chunk_att_);
static void xdio_destroy_raw0(xdio_chunk_obj_t *chunk_obj_);
static int xdio_write_raw0(xdio_chunk_obj_t *chunk_obj_);
static int xdio_read_raw0(xdio_chunk_obj_t *chunk_obj_);
static char *xdio_get_format_raw0(xdio_chunk_obj_t *chunk_obj_);
static int xdio_get_size_raw0(xdio_chunk_obj_t *chunk_obj_);
static int xdio_attach_data_src_raw0(int channel, int8_t *data, size_t length, xdio_chunk_obj_t *chunk_obj_);
static int xdio_attach_multi_data_src_raw0(int first, int32_t mask, int8_t *data, size_t length, xdio_chunk_obj_t *chunk_obj_);
static int xdio_attach_data_dst_raw0(int channel, int8_t *data, size_t length, xdio_chunk_obj_t *chunk_obj_);
static int xdio_attach_multi_data_dst_raw0(int nb_channels, int8_t **data, xdio_chunk_obj_t *chunk_obj_);
static int xdio_extract_data_raw0(xdio_chunk_obj_t *chunk_obj_);
static xdio_chunk_att_t * xdio_get_attributes_raw0(xdio_chunk_obj_t *chunk_obj_);

/* internal functions */
static int attach_data(int channel, int8_t *data, size_t length, xdio_chunk_raw0_t *chunk_obj);
static int gzip_compress_blocks(xdio_chunk_raw0_t *chunk_obj);
static int gzip_decompress_blocks(xdio_chunk_raw0_t *chunk_obj);
static int bzip2_compress_blocks(xdio_chunk_raw0_t *chunk_obj);
static int bzip2_decompress_blocks(xdio_chunk_raw0_t *chunk_obj);

/* register raw0 chunk operations */
void
xdio_register_chunk_raw0_ops(xdio_chunk_t *chunk)
{
	xdio_chunk_ops_t *ops = (xdio_chunk_ops_t *) malloc(sizeof(xdio_chunk_ops_t));

	ops->create_ = xdio_create_raw0;
	ops->destroy_ = xdio_destroy_raw0;
	ops->write_ = xdio_write_raw0;
	ops->read_ = xdio_read_raw0;
	ops->get_format_ = xdio_get_format_raw0;
	ops->get_size_ = xdio_get_size_raw0;
	ops->attach_data_src_ = xdio_attach_data_src_raw0;
	ops->attach_multi_data_src_ = xdio_attach_multi_data_src_raw0;
	ops->attach_data_dst_ = xdio_attach_data_dst_raw0;
	ops->attach_multi_data_dst_ = xdio_attach_multi_data_dst_raw0;
	ops->extract_data_ = xdio_extract_data_raw0;
	ops->get_attributes_ = xdio_get_attributes_raw0;

	chunk->ops_ = ops;
}

/* create a raw0 chunk */
xdio_chunk_obj_t *
xdio_create_raw0(xdio_chunk_att_t *chunk_att_)
{
	xdio_chunk_raw0_att_t *chunk_att = (xdio_chunk_raw0_att_t *) chunk_att_;
	xdio_chunk_raw0_t *chunk_obj = (xdio_chunk_raw0_t *) malloc(sizeof(xdio_chunk_raw0_t));

	chunk_obj->header = (xdio_chunk_raw0_header_t *) malloc(sizeof(xdio_chunk_raw0_header_t));
	xdio_chunk_raw0_header_t *header = chunk_obj->header;

	xdio_set_chunk_type(header->type, "raw0");
	header->size = XDIO_CHUNK_RAW0_HEADER_SIZE;
	if(chunk_att_ == NULL)
	{
		header->sample_type = 0;
		header->flags = 0;
		header->nb_samples = 0;
		header->voltage_range = 0.;
		header->sampling_frequency = 0.;
		header->nb_channels = 0;
		header->unused = 0;

		chunk_obj->attributes = NULL;
		chunk_obj->channel_length = 0;
	}
	else
	{
		header->sample_type = chunk_att->sample_type;
		header->flags = chunk_att->flags;
		header->nb_samples = chunk_att->nb_samples;
		header->voltage_range = chunk_att->voltage_range;
		header->sampling_frequency = chunk_att->sampling_frequency;
		header->nb_channels = chunk_att->nb_channels;
		header->unused = 0;

		chunk_obj->attributes = xdio_create_chunk_raw0_attributes(header->sample_type, header->flags, header->nb_samples, header->voltage_range, header->sampling_frequency, header->nb_channels);;
		chunk_obj->channel_length = xdio_bytes_per_sample(header->sample_type)*header->nb_samples;
	}

	chunk_obj->data_offset = 0;

	chunk_obj->nb_blocks = 0;
	chunk_obj->max_blocks = 0;
	chunk_obj->blocks = NULL;
	chunk_obj->compressed_block = NULL;
	chunk_obj->decompressed_block = NULL;

	return((xdio_chunk_obj_t *) chunk_obj);
}

/* destroy a raw0 chunk */
static void
xdio_destroy_raw0(xdio_chunk_obj_t *chunk_obj_)
{
	xdio_chunk_raw0_t *chunk_obj = (xdio_chunk_raw0_t *) chunk_obj_;

	int flags = chunk_obj->header->flags;

	/* free attributes */
	if(chunk_obj->attributes != NULL)
		free(chunk_obj->attributes);

	/* free header */
	free(chunk_obj->header);

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
	if((flags & XDIO_F_RAW0_GZIP) || (flags & XDIO_F_RAW0_BZIP2))
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

/* write a raw0 chunk */
static int
xdio_write_raw0(xdio_chunk_obj_t *chunk_obj_)
{
	xdio_chunk_raw0_t *chunk_obj = (xdio_chunk_raw0_t *) chunk_obj_;
	xdio_chunk_raw0_header_t *header = chunk_obj->header;
	xdio_file_t *file = chunk_obj->wrap_->event->to_file;

	int ret = XDIO_RET_OK;

	/* do we have all channels? */
	if(chunk_obj->nb_blocks != header->nb_channels)
	{
		xdio_error("not all channels added to a 'raw0' chunk!\n");
		return(XDIO_RET_ERR);
	}

	/* if the chunk should be gzip or bzip2 compressed */
	if(header->flags & XDIO_F_RAW0_GZIP)
		ret |= gzip_compress_blocks(chunk_obj);
	else if(header->flags & XDIO_F_RAW0_BZIP2)
		ret |= bzip2_compress_blocks(chunk_obj);

	int8_t *buf = (int8_t *) malloc(sizeof(xdio_chunk_raw0_header_t));
	int b;

	/* write raw0 header */
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
	xdio_save_int32(&buf[28], header->unused);
	ret |= xdio_write_data(buf, XDIO_CHUNK_RAW0_HEADER_SIZE, file);

	free(buf);

	/* write the block(s) */
	if((header->flags & XDIO_F_RAW0_GZIP) || (header->flags & XDIO_F_RAW0_BZIP2))
		ret |= xdio_write_data(chunk_obj->compressed_block->data, chunk_obj->compressed_block->length, file);
	else
		for(b = 0; b < chunk_obj->nb_blocks; b++)
			ret |= xdio_write_data(chunk_obj->blocks[b]->data, chunk_obj->blocks[b]->length, file);

	return(ret);
}

/* read a raw0 chunk */
static int
xdio_read_raw0(xdio_chunk_obj_t *chunk_obj_)
{
	xdio_chunk_raw0_t *chunk_obj = (xdio_chunk_raw0_t *) chunk_obj_;
	xdio_chunk_raw0_header_t *header = chunk_obj->header;
	xdio_file_t *file = chunk_obj->wrap_->event->from_file;

	int8_t *buf = (int8_t *) malloc(XDIO_CHUNK_RAW0_HEADER_SIZE);
	int ret;

	/* assume that the chunk type has already been read */
	ret = xdio_read_data(&buf[4], XDIO_CHUNK_RAW0_HEADER_SIZE-4, file);

	/* read the raw0 header */
	header->size = xdio_load_int32(&buf[4]);
	header->sample_type = xdio_load_int16(&buf[8]);
	header->flags = xdio_load_int16(&buf[10]);
	header->nb_samples = xdio_load_int32(&buf[12]);
	header->voltage_range = xdio_load_float(&buf[16]);
	header->sampling_frequency = xdio_load_float(&buf[20]);
	header->nb_channels = xdio_load_int32(&buf[24]);
	header->unused = xdio_load_int32(&buf[28]);

	chunk_obj->attributes = xdio_create_chunk_raw0_attributes(header->sample_type, header->flags, header->nb_samples, header->voltage_range, header->sampling_frequency, header->nb_channels);
	chunk_obj->channel_length = xdio_bytes_per_sample(header->sample_type)*header->nb_samples;

	/* save the file offset to where the chunk data is */
	chunk_obj->data_offset = ftell(file->in);

	/* seek to the next chunk - this possibly double counts */
	//fseek(file->in, header->size - XDIO_CHUNK_RAW0_HEADER_SIZE, SEEK_CUR);

	free(buf);

	return(ret);
}

/* get the format of a raw0 chunk */
static char *
xdio_get_format_raw0(xdio_chunk_obj_t *chunk_obj_)
{
	xdio_chunk_raw0_t *chunk_obj = (xdio_chunk_raw0_t *) chunk_obj_;
	xdio_chunk_raw0_header_t *header = chunk_obj->header;

	static char format[512];
	char line[64];

	strcpy(format, "    <raw0>\n");

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
	sprintf(line, "      <sampling_frequency>%0.f</sampling_frequency>\n", header->sampling_frequency);
	strcat(format, line);

	/* number of channels */
	sprintf(line, "      <nb_channels>%d</nb_channels>\n", header->nb_channels);
	strcat(format, line);

	strcat(format, "    </raw0>\n");

	return(format);
}

/* get the size of a raw0 chunk */
static int
xdio_get_size_raw0(xdio_chunk_obj_t *chunk_obj_)
{
	xdio_chunk_raw0_t *chunk_obj = (xdio_chunk_raw0_t *) chunk_obj_;
	xdio_chunk_raw0_header_t *header = chunk_obj->header;

	return(header->size);
}

/* attach data source to a raw0 chunk */
static int
xdio_attach_data_src_raw0(int channel, int8_t *data, size_t length, xdio_chunk_obj_t *chunk_obj_)
{
	xdio_chunk_raw0_t *chunk_obj = (xdio_chunk_raw0_t *) chunk_obj_;

	return(attach_data(channel, data, length, chunk_obj));
}

/* attach multi data source to a raw0 chunk */
static int
xdio_attach_multi_data_src_raw0(int first, int32_t mask, int8_t *data, size_t length, xdio_chunk_obj_t *chunk_obj_)
{
	xdio_chunk_raw0_t *chunk_obj = (xdio_chunk_raw0_t *) chunk_obj_;

	/* count the number of channels */
	// count_bits counts the number of bits saved as 1 (not 0!)
	int nb_channels_to_add = xdio_count_bits(mask);

	int channel = first;
	int8_t *channel_data = data;

	int bit = 0;
	while(bit < 32)
	{
		if((1 << bit++) & mask)
		{
			if(attach_data(channel, channel_data, chunk_obj->channel_length, chunk_obj) != XDIO_RET_OK)
				return(XDIO_RET_ERR);

			channel_data = channel_data + chunk_obj->channel_length;
		}

		channel++;
	}

	return(XDIO_RET_OK);
}

/* attach data destination to a raw0 chunk */
static int
xdio_attach_data_dst_raw0(int channel, int8_t *data, size_t length, xdio_chunk_obj_t *chunk_obj_)
{
	xdio_chunk_raw0_t *chunk_obj = (xdio_chunk_raw0_t *) chunk_obj_;

	return(attach_data(channel, data, length, chunk_obj));
}

/* attach multi data destination to a raw0 chunk */
static int
xdio_attach_multi_data_dst_raw0(int nb_channels, int8_t **data, xdio_chunk_obj_t *chunk_obj_)
{
	xdio_error("'raw0' chunk has no implementation for attach_multi_data_dst!\n");

	return(XDIO_RET_ERR);
}

/* extract the chunk data to the attached data destinations */
static int
xdio_extract_data_raw0(xdio_chunk_obj_t *chunk_obj_)
{
	xdio_chunk_raw0_t *chunk_obj = (xdio_chunk_raw0_t *) chunk_obj_;
	xdio_file_t *file = chunk_obj->wrap_->event->from_file;
	int flags = chunk_obj->header->flags;
	int channel_length = chunk_obj->channel_length;

	int ret = XDIO_RET_OK;

	/* if the chunk is gzip or bzip2 compressed */
	if((flags & XDIO_F_RAW0_GZIP) || (flags & XDIO_F_RAW0_BZIP2))
	{
		/* fseek to the channel data and read in the compressed data */
		fseek(file->in, chunk_obj->data_offset, SEEK_SET);

		xdio_block_t *compressed_block = (xdio_block_t *) malloc(sizeof(xdio_block_t));
		compressed_block->data = (int8_t *) malloc(chunk_obj->header->size-XDIO_CHUNK_RAW0_HEADER_SIZE);
		compressed_block->length = chunk_obj->header->size-XDIO_CHUNK_RAW0_HEADER_SIZE;
		
		ret = xdio_read_data(compressed_block->data, compressed_block->length, file);
		chunk_obj->compressed_block = compressed_block;

		/* decompress it */
		if(flags & XDIO_F_RAW0_GZIP)
			ret |= gzip_decompress_blocks(chunk_obj);
		else if(flags & XDIO_F_RAW0_BZIP2)
			ret |= bzip2_decompress_blocks(chunk_obj);
	}

	/* for all blocks */
	int b;
	for(b = 0; b < chunk_obj->max_blocks; b++)
	{
		/* if the block was attached */
		if(chunk_obj->blocks[b] != NULL)
		{
			/* if this was compressed data */
			if((flags & XDIO_F_RAW0_GZIP) || (flags & XDIO_F_RAW0_BZIP2))
				memcpy(chunk_obj->blocks[b]->data, chunk_obj->decompressed_block->data + b*channel_length, channel_length);
			else
			{
				/* fseek to the channel data */
				fseek(file->in, chunk_obj->data_offset + b*channel_length, SEEK_SET);

				/* write data to the user supplied space */
				ret |= xdio_read_data(chunk_obj->blocks[b]->data, channel_length, file);
			}
		}
	}

	return(ret);
}

/* get attributes of a raw0 chunk */
static xdio_chunk_att_t *
xdio_get_attributes_raw0(xdio_chunk_obj_t *chunk_obj_)
{
	xdio_chunk_raw0_t *chunk_obj = (xdio_chunk_raw0_t *) chunk_obj_;

	return(chunk_obj->attributes);
}

/* create a raw0 chunk attributes object */
xdio_chunk_att_t *
xdio_create_chunk_raw0_attributes(int sample_type, int flags, int nb_samples,
	float voltage_range, float sampling_frequency, int nb_channels)
{
	xdio_chunk_raw0_att_t *chunk_att = (xdio_chunk_raw0_att_t *) malloc(sizeof(xdio_chunk_raw0_att_t));

	chunk_att->sample_type = sample_type;
	chunk_att->flags = flags;
	chunk_att->nb_samples = nb_samples;
	chunk_att->voltage_range = voltage_range;
	chunk_att->sampling_frequency = sampling_frequency;
	chunk_att->nb_channels = nb_channels;

	chunk_att->bytes_per_sample = xdio_bytes_per_sample(chunk_att->sample_type);
	chunk_att->channel_length = chunk_att->nb_samples*chunk_att->bytes_per_sample;

	return(chunk_att);
}

/* attach data source or destination to a raw0 chunk */
static int
attach_data(int channel, int8_t *data, size_t length, xdio_chunk_raw0_t *chunk_obj)
{
	/* check that the attributes are known */
	if(chunk_obj->attributes == NULL)
	{
		xdio_error("'raw0' chunk attributes not known!\n");
		return(XDIO_RET_ERR);
	}

	/* if the channel is out of bounds */
	if((channel < 0) || (channel > chunk_obj->header->nb_channels-1))
	{
		xdio_error("channel %d is out of bounds!\n", channel);
		return(XDIO_RET_ERR);
	}

	/* if the length is different than what it should be */
	if(length != chunk_obj->channel_length)
	{
		xdio_error("data length %d not appropriate, should be %d!\n", length, chunk_obj->channel_length);
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
		xdio_error("'raw0' chunk channel %d already has a source or destination!\n", channel);
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
		chunk_obj->header->size += length;

	return(XDIO_RET_OK);
}

/* compress the data blocks of a raw0 chunk into a single gzip compressed block */
static int
gzip_compress_blocks(xdio_chunk_raw0_t *chunk_obj)
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

	/* pass all blocks to deflate */
	for(b = 0; b < chunk_obj->nb_blocks; b++)
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

	if(deflate(&strm, Z_FINISH) != Z_STREAM_END)
	{
		deflateEnd(&strm);
		xdio_error("unexpected return value from zlib!\n");
		return(XDIO_RET_ERR);
	}
	
	compressed_block->length = maximum_length-strm.avail_out;
	chunk_obj->header->size = XDIO_CHUNK_RAW0_HEADER_SIZE + compressed_block->length;
	chunk_obj->compressed_block = compressed_block;

	deflateEnd(&strm);

	return(XDIO_RET_OK);
}

/* decompress the gzip block into data blocks of a raw0 chunk */
static int
gzip_decompress_blocks(xdio_chunk_raw0_t *chunk_obj)
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

/* compress the data blocks of a raw0 chunk into a single bzip2 compressed block */
static int
bzip2_compress_blocks(xdio_chunk_raw0_t *chunk_obj)
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
	for(b = 0; b < chunk_obj->nb_blocks; b++)
	{
		strm.avail_in = chunk_obj->blocks[b]->length;
		strm.next_in = chunk_obj->blocks[b]->data;
		
		if(BZ2_bzCompress(&strm, BZ_RUN) != BZ_RUN_OK)
		{
			xdio_error("bzip2 compression failed!\n");
			return(XDIO_RET_ERR);
		}
	}

	if(BZ2_bzCompress(&strm, BZ_FINISH) != BZ_STREAM_END)
	{
		xdio_error("unexpected return value from bzip2!\n");
		return(XDIO_RET_ERR);
	}
	
	compressed_block->length = maximum_length-strm.avail_out;
	chunk_obj->header->size = XDIO_CHUNK_RAW0_HEADER_SIZE + compressed_block->length;
	chunk_obj->compressed_block = compressed_block;

	BZ2_bzCompressEnd(&strm);

	return(XDIO_RET_OK);
}

/* decompress the bzip2 block into data blocks of a raw0 chunk */
static int
bzip2_decompress_blocks(xdio_chunk_raw0_t *chunk_obj)
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

