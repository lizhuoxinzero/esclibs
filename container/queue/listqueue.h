#ifndef LISTQUEUE_H
#define LISTQUEUE_H

#include <stdint.h>
#include "container/buffer.h"

typedef struct LIST_QUEUE_NODE{
	struct LIST_QUEUE_NODE *next;
}listqueue_node_t;

typedef struct{
	listqueue_node_t *head;
	listqueue_node_t *tail;
	uint32_t blksize;
	buffer_t *cur_buf;
	buffer_t *head_buf;
	buffer_t *recycle_buf;
}listqueue_t;


typedef struct{
	void *data;
}iterator_t;

/**
 * 创建和初始化listqueue_t
 *
 * @param blksize the block buffer size, if zero then use 512byte
 * @return a pointer to a newly initialized listqueue_t object, if error then return NULL
 */
extern listqueue_t* listqueue_new(uint32_t blksize);

extern int listqueue_close(listqueue_t* list);

/**
 * push data into queue
 *
 * @param data 将要push进queue的数据，将拷贝data指向的内容
 * @param size data指向的内容的大小
 */
extern listqueue_node_t* listqueue_push(listqueue_t* listqueue, void *data, uint32_t size);

extern iterator_t listqueue_iterator(listqueue_t *listqueue);

extern void* listqueue_iter_next(iterator_t *iter);


#endif
