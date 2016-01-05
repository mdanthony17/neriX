#ifndef __INTERNAL_H__
#define __INTERNAL_H__

/* utility functions */
XDIO_INTERNAL void xdio_save_int8(int8_t *buf, int8_t val);
XDIO_INTERNAL void xdio_save_uint8(int8_t *buf, uint8_t val);
XDIO_INTERNAL int8_t xdio_load_int8(int8_t *buf);
XDIO_INTERNAL uint8_t xdio_load_uint8(int8_t *buf);

XDIO_INTERNAL void xdio_save_int16(int8_t *buf, int16_t val);
XDIO_INTERNAL void xdio_save_uint16(int8_t *buf, uint16_t val);
XDIO_INTERNAL int16_t xdio_load_int16(int8_t *buf);
XDIO_INTERNAL uint16_t xdio_load_uint16(int8_t *buf);

XDIO_INTERNAL void xdio_save_int32(int8_t *buf, int32_t val);
XDIO_INTERNAL void xdio_save_uint32(int8_t *buf, uint32_t val);
XDIO_INTERNAL int32_t xdio_load_int32(int8_t *buf);
XDIO_INTERNAL uint32_t xdio_load_uint32(int8_t *buf);

XDIO_INTERNAL void xdio_save_int64(int8_t *buf, int64_t val);
XDIO_INTERNAL void xdio_save_uint64(int8_t *buf, uint64_t val);
XDIO_INTERNAL int64_t xdio_load_int64(int8_t *buf);
XDIO_INTERNAL uint64_t xdio_load_uint64(int8_t *buf);

XDIO_INTERNAL void xdio_save_float(int8_t *buf, float val);
XDIO_INTERNAL float xdio_load_float(int8_t *buf);

XDIO_INTERNAL int xdio_write_data(int8_t *data, size_t length, xdio_file_t *file);
XDIO_INTERNAL int xdio_read_data(int8_t *data, size_t length, xdio_file_t *file);

XDIO_INTERNAL int xdio_count_bits(uint32_t n);
XDIO_INTERNAL int xdio_bytes_per_sample(int sample_type);

/* functions for xdio events */
XDIO_INTERNAL xdio_event_t *xdio_read_event(xdio_file_t *file);
XDIO_INTERNAL int xdio_write_event(xdio_event_t *event);

/* functions for xdio chunks */
XDIO_INTERNAL int xdio_write_chunk(xdio_chunk_t *chunk);
XDIO_INTERNAL xdio_chunk_t *xdio_read_chunk(xdio_file_t *file, xdio_event_t *event);
XDIO_INTERNAL char *xdio_get_chunk_format(xdio_chunk_t *chunk);
XDIO_INTERNAL int xdio_get_chunk_size(xdio_chunk_t *chunk);
XDIO_INTERNAL void xdio_set_chunk_type(xdio_chunk_type_t type1, xdio_chunk_type_t type2);
XDIO_INTERNAL int xdio_check_chunk_type(xdio_chunk_type_t type1, xdio_chunk_type_t type2);

/* functions for xdio 'raw0' chunks */
XDIO_INTERNAL void xdio_register_chunk_raw0_ops(xdio_chunk_t *chunk);

/* functions for 'zle0' chunks */
XDIO_INTERNAL void xdio_register_chunk_zle0_ops(xdio_chunk_t *chunk);

#endif // __INTERNAL_H__

