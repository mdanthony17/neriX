#ifndef __XDIO_H__
#define __XDIO_H__

#define XDIO_FILE_FILENAME_LENGTH 64
#define XDIO_FILE_HEADER_SIZE(n) (XDIO_FILE_FILENAME_LENGTH+16+4*(n))
#define XDIO_FILE_NO_EVENT_INDEX (-1)

/* file header */
typedef struct xdio_file_header
{
	int8_t filename[XDIO_FILE_FILENAME_LENGTH];
	int32_t time;
	int32_t first_event_nb;
	int32_t nb_events;
	int32_t index_size;
	int32_t *index;
} xdio_file_header_t;

/* file object */
typedef struct xdio_file
{
	/* the file header */
	xdio_file_header_t *header;
	int mode;

	/* the file input, output */
	FILE *out, *in;

	/* writing mode parameters */
	int desired_nb_events;
	int nb_events_added;
	int event_buffer_size;

	/* reading mode parameters */

	/* event buffer */
	int nb_events_in_buffer;
	struct xdio_event **events;

} xdio_file_t;

#define XDIO_EVENT_HEADER_SIZE (XDIO_FILE_FILENAME_LENGTH+16)

/* event header */
typedef struct xdio_event_header
{
	int8_t filename[XDIO_FILE_FILENAME_LENGTH];
	int32_t time_sec;
	int32_t time_usec;
	int32_t event_nb;
	int32_t nb_chunks;
} xdio_event_header_t;

/* event object */
typedef struct xdio_event
{
	/* the event header */
	xdio_event_header_t *header;

	/* the array of chunks */
	int nb_chunks;
	int max_chunks;
	struct xdio_chunk **chunks;

	off_t offset;

	struct xdio_file *to_file, *from_file;

} xdio_event_t;

/* generic chunk object */
typedef void xdio_chunk_obj_t;

/* generic chunk attributes */
typedef void xdio_chunk_att_t;

/* chunk type */
typedef char xdio_chunk_type_t[4];

/* chunk operations */
typedef struct
{
	xdio_chunk_obj_t *(*create_)(xdio_chunk_att_t *chunk_att_);
	void (*destroy_)(xdio_chunk_obj_t *chunk_obj_);

	int (*write_)(xdio_chunk_obj_t *chunk_obj_);
	int (*read_)(xdio_chunk_obj_t *chunk_obj_);

	char *(*get_format_)(xdio_chunk_obj_t *chunk_obj_);
	int (*get_size_)(xdio_chunk_obj_t *chunk_obj_);

	int (*attach_data_src_)(int channel, int8_t *data, size_t length, xdio_chunk_obj_t *chunk_obj_);
	int (*attach_multi_data_src_)(int first, int32_t mask, int8_t *data, size_t length, xdio_chunk_obj_t *chunk_obj_);

	int (*attach_data_dst_)(int channel, int8_t *data, size_t length, xdio_chunk_obj_t *chunk_obj_);
	int (*attach_multi_data_dst_)(int nb_channels, int8_t **data, xdio_chunk_obj_t *chunk_obj_);

	int (*extract_data_)(xdio_chunk_obj_t *chunk_obj_);

	xdio_chunk_att_t *(*get_attributes_)(xdio_chunk_obj_t *chunk_obj_);
} xdio_chunk_ops_t;

/* data block object */
typedef struct
{
	int8_t *data;
	size_t length;
} xdio_block_t;

/* chunk wrapper object */
typedef struct
{
	/* the underlying chunk object */
	xdio_chunk_obj_t *obj_;

	/* the operations for the chunk object */
	xdio_chunk_ops_t *ops_;

	/* the enclosing object */
	struct xdio_event *event;	
} xdio_chunk_t;

#define XDIO_CHUNK_RAW0_HEADER_SIZE 32

#define XDIO_F_RAW0_GZIP  (1 << 0)
#define XDIO_F_RAW0_BZIP2 (1 << 1)

/* 'raw0' chunk header */
typedef struct xdio_chunk_raw0_header
{
	int8_t type[4];
	int32_t size;
	int16_t sample_type;
	int16_t flags;
	int32_t nb_samples;
	float voltage_range;
	float sampling_frequency;
	int32_t nb_channels;
	int32_t unused;
} xdio_chunk_raw0_header_t;

/* 'raw0' chunk attributes */
typedef struct xdio_chunk_raw0_att
{
	/* specified attributes */
	int sample_type;
	int flags;
	int nb_samples;
	float voltage_range;
	float sampling_frequency;
	int nb_channels;

	/* derived attributes */
	int bytes_per_sample;
	int channel_length;
} xdio_chunk_raw0_att_t;

/* 'raw0' chunk object */
typedef struct xdio_chunk_raw0
{
	/* 'raw0' header */
	xdio_chunk_raw0_header_t *header;

	/* attributes */
	xdio_chunk_raw0_att_t *attributes;

	/* reading and writing mode parameters */
	int channel_length;
	off_t data_offset;	

	/* array of data block addresses */
	int nb_blocks;
	int max_blocks;
	xdio_block_t **blocks;
	xdio_block_t *compressed_block;
	xdio_block_t *decompressed_block;

	/* the chunk wrapper object */
	xdio_chunk_t *wrap_;
} xdio_chunk_raw0_t;

#define XDIO_CHUNK_ZLE0_HEADER_CHANNEL_MASK_SIZE(n) (4*((n)?(((n % 32)?(n/32+1):(n/32))):(0)))
#define XDIO_CHUNK_ZLE0_HEADER_SIZE(n) (28+(4*((n)?(((n % 32)?(n/32+1):(n/32))):(0))))

#define XDIO_F_ZLE0_GZIP  (1 << 0)
#define XDIO_F_ZLE0_BZIP2 (1 << 1)

/* 'zle0' chunk header */
typedef struct xdio_chunk_zle0_header
{
	int8_t type[4];
	int32_t size;	
	int16_t sample_type;
	int16_t flags;
	int32_t nb_samples;
	float voltage_range;
	float sampling_frequency;
	int32_t nb_channels;
	int8_t *channel_mask;
} xdio_chunk_zle0_header_t;

/* 'zle0' chunk attributes */
typedef struct xdio_chunk_zle0_att
{
	/* specified attributes */
	int sample_type;
	int flags;
	int nb_samples;
	float voltage_range;
	float sampling_frequency;
	int nb_channels;

	/* derived attributes */
	int bytes_per_sample;
	int nb_channels_included;
	int *channels;
	int *channel_lengths;
} xdio_chunk_zle0_att_t;

/* 'zle0' chunk object */
typedef struct xdio_chunk_zle0
{
	/* 'zle0' header */
	xdio_chunk_zle0_header_t *header;

	/* attributes */
	xdio_chunk_zle0_att_t *attributes;

	/* reading and writing mode parameters */
	off_t data_offset;
	int channel_length;
	int nb_offsets;
	off_t *data_offsets;
	size_t *data_lengths;

	/* array of data block addresses */
	int nb_blocks;
	int max_blocks;
	xdio_block_t **blocks;
	xdio_block_t *compressed_block;
	xdio_block_t *decompressed_block;

	/* the chunk wrapper object */
	xdio_chunk_t *wrap_;
} xdio_chunk_zle0_t;

/* functions for xdio files */
xdio_file_t *xdio_open_write(const char *filename, time_t time, int first_event_nb, int desired_nb_events, int event_buffer_size);
xdio_file_t *xdio_open_read(const char *filename);
void xdio_close(xdio_file_t *file);

char *xdio_get_filename(xdio_file_t *file);
time_t xdio_get_time_created(xdio_file_t *file);
int xdio_get_first_event_nb(xdio_file_t *file);
int xdio_get_nb_events(xdio_file_t *file);

int xdio_add_event(xdio_event_t *event, xdio_file_t *file);
xdio_event_t *xdio_remove_event(int event_nb, xdio_file_t *file);
xdio_event_t *xdio_get_event(int index_nb, xdio_file_t *file);

/* functions for xdio events */
xdio_event_t *xdio_create_event();
void xdio_destroy_event(xdio_event_t *event);

char *xdio_get_format(xdio_event_t *event);
void xdio_set_trigger_time(int32_t time_sec, int32_t time_usec, xdio_event_t *event);
int xdio_get_trigger_time_sec(xdio_event_t *event);
int xdio_get_trigger_time_usec(xdio_event_t *event);
int xdio_get_event_nb(xdio_event_t *event);
int xdio_get_nb_chunks(xdio_event_t *event);

void xdio_add_chunk(xdio_chunk_t *chunk, xdio_event_t *event);
xdio_chunk_t *xdio_remove_chunk(int chunk_nb, xdio_event_t *event);
xdio_chunk_t *xdio_get_chunk(int chunk_nb, xdio_event_t *event);

/* functions for xdio chunks */
xdio_chunk_t *xdio_create_chunk(xdio_chunk_type_t chunk_type, xdio_chunk_att_t *att);
void xdio_destroy_chunk(xdio_chunk_t *chunk);

int xdio_attach_data_src(int channel, int8_t *data, size_t length, xdio_chunk_t *chunk);
int xdio_attach_multi_data_src(int first, int32_t mask, int8_t *data, size_t length, xdio_chunk_t *chunk);
int xdio_attach_data_dst(int channel, int8_t *data, size_t length, xdio_chunk_t *chunk);
int xdio_attach_multi_data_dst(int nb_channels, int8_t **data, xdio_chunk_t *chunk);

void xdio_set_attributes(xdio_chunk_att_t *chunk_att, xdio_chunk_t *chunk);
xdio_chunk_att_t *xdio_get_attributes(xdio_chunk_t *chunk);

int xdio_extract_data(xdio_chunk_t *chunk);

void xdio_get_chunk_type(xdio_chunk_type_t type, xdio_chunk_t *chunk);

int xdio_chunk_type_cmp(xdio_chunk_type_t type, xdio_chunk_t *chunk);

/* functions for xdio 'raw0' chunks */
xdio_chunk_att_t *xdio_create_chunk_raw0_attributes(int sample_type, int flags, int nb_samples, float voltage_range, float sampling_frequency, int nb_channels);

/* functions for 'zle0' chunks */
xdio_chunk_att_t *xdio_create_chunk_zle0_attributes(int sample_type, int flags, int nb_samples, float voltage_range, float sampling_frequency, int nb_channels);

#endif // __XDIO_H__
