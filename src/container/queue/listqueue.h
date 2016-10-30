#ifndef ES_LISTQUEUE_H
#define ES_LISTQUEUE_H

#include <stdint.h>
#include "container/iterator.h"
#include "container/buffer.h"

typedef struct LIST_QUEUE_NODE{
	struct LIST_QUEUE_NODE *next;
}listqueue_node_t;

typedef struct{
	listqueue_node_t *head;
	listqueue_node_t *tail;
	uint32_t queue_size;
	queue_buffer_t buffer;
}listqueue_t;

/**
 * 创建和初始化listqueue_t
 *
 * listqueue_init只用于初始化，不会创建新的listqueue_t, 销毁要用listqueue_close
 * listqueue_new用于创建和初始化，销毁要用listqueue_close_and_free
 *
 * @param blksize the block buffer size, if zero then use 512byte
 * @return 出错时返回NULL
 */
extern listqueue_t* listqueue_init(listqueue_t* listqueue, uint32_t blksize);
extern listqueue_t* listqueue_new(uint32_t blksize);

extern void listqueue_close(listqueue_t* list);
extern void listqueue_close_and_free(listqueue_t* list);

/*
 * 清理链接队列，使之重复使用
 * */
extern void listqueue_clean(listqueue_t* list);
/**
 * push data into queue
 *
 * @param data 将要push进queue的数据，将拷贝data指向的内容
 * @param size data指向的内容的大小
 * @return 塞进去的新内容地址
 */
extern void* listqueue_push(listqueue_t* listqueue, void *data, uint32_t size);

/**
 * 将创建一个size大小的内存空间，并push队列尾部，然后返回该内存地址，
 * 即可向push队列再附值，可节省一次复制操作。
 */
extern void* listqueue_push_new(listqueue_t* listqueue, uint32_t size);

extern void listqueue_pop(listqueue_t* listqueue);

extern void* listqueue_head(listqueue_t* listqueue);
extern void* listqueue_tail(listqueue_t* listqueue);

extern iterator_t listqueue_iterator(listqueue_t *listqueue);
extern void* listqueue_iter_next(iterator_t *iter);


#endif
