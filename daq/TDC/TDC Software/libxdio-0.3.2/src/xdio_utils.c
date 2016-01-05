#include "common.h"

void
xdio_save_int8(int8_t *buf, int8_t val)
{
	*buf = val;
}

int8_t
xdio_load_int8(int8_t *buf)
{
	return(*buf);
}

void
xdio_save_int16(int8_t *buf, int16_t val)
{
#ifdef WORDS_BIGENDIAN
	buf[0] = (int8_t) (0xFF & (val >> 0));
	buf[1] = (int8_t) (0xFF & (val >> 8));
#else
	*((int16_t *) buf) = val;
#endif
}

void
xdio_save_uint16(int8_t *buf, uint16_t val)
{
}

int16_t
xdio_load_int16(int8_t *buf)
{
#ifdef WORDS_BIGENDIAN
	return(((int16_t) (buf[1] << 8)) + ((int16_t) buf[0]));
#else
	return(*((int16_t *) buf));
#endif
}

void
xdio_save_int32(int8_t *buf, int32_t val)
{
#ifdef WORDS_BIGENDIAN
	buf[0] = (int8_t) (0xFF & (val >> 0));
	buf[1] = (int8_t) (0xFF & (val >> 8));
	buf[2] = (int8_t) (0xFF & (val >> 16));
	buf[3] = (int8_t) (0xFF & (val >> 24));
#else
	*((int32_t *) buf) = val;
#endif
}

void
xdio_save_uint32(int8_t *buf, uint32_t val)
{
}

int32_t
xdio_load_int32(int8_t *buf)
{
#ifdef WORDS_BIGENDIAN
	return(((int32_t) (buf[3] << 24)) + ((int32_t) (buf[2] << 16)) + ((int32_t) (buf[1] << 8)) + ((int32_t) buf[0]));
#else
	return(*((int32_t *) buf));
#endif
}

void
xdio_save_int64(int8_t *buf, int64_t val)
{
#ifdef WORDS_BIGENDIAN
	buf[0] = (int8_t) (0xFF & (val >> 0));
	buf[1] = (int8_t) (0xFF & (val >> 8));
	buf[2] = (int8_t) (0xFF & (val >> 16));
	buf[3] = (int8_t) (0xFF & (val >> 24));
	buf[4] = (int8_t) (0xFF & (val >> 32));
	buf[5] = (int8_t) (0xFF & (val >> 40));
	buf[6] = (int8_t) (0xFF & (val >> 48));
	buf[7] = (int8_t) (0xFF & (val >> 56));
#else
	*((int64_t *) buf) = val;
#endif
}

void
xdio_save_uint64(int8_t *buf, uint64_t val)
{
}

int64_t
xdio_load_int64(int8_t *buf)
{
#ifdef WORDS_BIGENDIAN
	return(((int64_t) (buf[7] << 56)) + ((int64_t) (buf[6] << 48)) + ((int64_t) (buf[5] << 40)) + ((int64_t) (buf[4] << 32))
		((int64_t) (buf[3] << 24)) + ((int64_t) (buf[2] << 16)) + ((int64_t) (buf[1] << 8)) + ((int64_t) buf[0]));
#else
	return(*((int64_t *) buf));
#endif
}

void
xdio_save_float(int8_t *buf, float val)
{
#ifdef WORDS_BIGENDIAN
	buf[0] = (int8_t) (0xFF & (val >> 0));
	buf[1] = (int8_t) (0xFF & (val >> 8));
	buf[2] = (int8_t) (0xFF & (val >> 16));
	buf[3] = (int8_t) (0xFF & (val >> 24));
#else
	*((float *) buf) = val;
#endif
}

float
xdio_load_float(int8_t *buf)
{
#ifdef WORDS_BIGENDIAN
	int32_t tmp = ((int32_t) (buf[3] << 24)) + ((int32_t) (buf[2] << 16)) + ((int32_t) (buf[1] << 8)) + ((int32_t) buf[0]);
	return(*((float *) &tmp));
#else
	return(*((float *) buf));
#endif
}

int
xdio_write_data(int8_t *data, size_t length, xdio_file_t *file)
{
	if(fwrite(data, 1, length, file->out) == length)
		return(XDIO_RET_OK);
	else
	{
		error("error writing!\n");
		return(XDIO_RET_ERR);
	}
}

int
xdio_read_data(int8_t *data, size_t length, xdio_file_t *file)
{
	if(fread(data, 1, length, file->in) == length)
		return(XDIO_RET_OK);
	else
	{
		error("error reading!\n");
		return(XDIO_RET_ERR);
	}
}

/* count the number of active bits in an integer */
int
xdio_count_bits(uint32_t n)  
{  
	int count = 0;

	while(n)
	{
		count++;
		n &= (n - 1);
	}

	return count;
}

/* number of bytes per sample */
int
xdio_bytes_per_sample(int sample_type)
{
	int sample_size = abs(sample_type);
	int sample_size_bytes = 1;

	while(sample_size_bytes*8 < sample_size) sample_size_bytes *= 2;

	return(sample_size_bytes);
}

