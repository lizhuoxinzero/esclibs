#ifndef ES_BUFFER_H
#define ES_BUFFER_H

#include <stdint.h>

typedef struct BUFFER{
	uint32_t size;
	uint32_t offset;
	uint32_t freesize;
	uint32_t count;
	struct BUFFER *next;
}buffer_t;


extern buffer_t* buffer_new(uint32_t blksize);

extern int buffer_close(buffer_t * buffer);

extern void* buffer_alloc(buffer_t *buffer, uint32_t size);
#endif
