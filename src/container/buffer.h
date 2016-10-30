#ifndef ES_BUFFER_H
#define ES_BUFFER_H

#include <stdint.h>

typedef struct BUFFER{
	uint32_t size;
	uint32_t offset;
	uint32_t freesize;
	uint32_t count;
	uint32_t fullflag;
	struct BUFFER *next;
}buffer_t;

extern buffer_t* buffer_new(uint32_t blksize);
extern void buffer_close(buffer_t * buffer);
extern buffer_t* buffer_clean(buffer_t * buffer);
extern void* buffer_alloc(buffer_t *buffer, uint32_t size);
extern void* buffer_free(buffer_t *buffer);


/*
 * 用于更好的使用buffer_t
 * */
typedef struct {
	buffer_t *cur_buf;
	buffer_t *head_buf;
	buffer_t *recycle_buf;
	uint32_t blksize;
}queue_buffer_t;

/*
 * 初始化队列缓存，一定成功
 * */
extern queue_buffer_t* queue_buffer_init(queue_buffer_t *queue_buf, uint32_t blksize);
extern void queue_buffer_close(queue_buffer_t *queue_buf);
extern void queue_buffer_clean(queue_buffer_t *queue_buf);
extern void* queue_buffer_alloc(queue_buffer_t *queue_buf, uint32_t size);
extern void* queue_buffer_pop(queue_buffer_t *queue_buf);





#endif
