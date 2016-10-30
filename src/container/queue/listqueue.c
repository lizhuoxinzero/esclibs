#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "container/queue/listqueue.h"
#include "container/buffer.h"



listqueue_t* listqueue_new(uint32_t blksize)
{
	
	listqueue_t *listqueue = malloc(sizeof(listqueue_t));
	if(listqueue == NULL) return NULL;
	void *temp = listqueue_init(listqueue, blksize);
	if (temp == NULL){
		free(listqueue);
		return NULL;
	}
	return listqueue;
}

listqueue_t* listqueue_init(listqueue_t *listqueue, uint32_t blksize)
{
	if (blksize == 0){
		blksize = 512; 
	}

	queue_buffer_init(&(listqueue->buffer), blksize);

	listqueue->queue_size = 0;
	listqueue->head = NULL;
	listqueue->tail = NULL;
	return listqueue;
}

void listqueue_clean(listqueue_t* listqueue)
{
	listqueue->queue_size = 0;
	listqueue->head = NULL;
	listqueue->tail = NULL;
	queue_buffer_clean(&(listqueue->buffer));
}

void listqueue_close(listqueue_t* listqueue)
{
	queue_buffer_close(&(listqueue->buffer));
}

void listqueue_close_and_free(listqueue_t* listqueue)
{
	listqueue_close(listqueue);
	free(listqueue); //释放分配的空间
}


void* listqueue_push(listqueue_t *listqueue, void *data, uint32_t size)
{
	//获取缓存块
	listqueue_node_t *node = (listqueue_node_t *)queue_buffer_alloc(&(listqueue->buffer), size+sizeof(listqueue_node_t));
	if (node == NULL){	//严重失败，内存都申请不到了
		return NULL;
	}

	memcpy(node+1, data, size);	//将data指向的内容拷贝进node+1的位置
	node->next = NULL;

	if(listqueue->head != NULL){
		listqueue->tail->next = node;
		listqueue->tail = node;
	}else{
		listqueue->head = node;
		listqueue->tail = node;
	}

	listqueue->queue_size += 1;
	return node+1;	//返回内存可用地址	
}

void* listqueue_push_new(listqueue_t *listqueue, uint32_t size)
{
	//获取缓存块
	listqueue_node_t *node = (listqueue_node_t *)queue_buffer_alloc(&(listqueue->buffer), size+sizeof(listqueue_node_t));
	if (node == NULL){	//严重失败，内存都申请不到了
		return NULL;
	}

	node->next = NULL;

	if(listqueue->head != NULL){
		listqueue->tail->next = node;
		listqueue->tail = node;
	}else{
		listqueue->head = node;
		listqueue->tail = node;
	}
	listqueue->queue_size += 1;
	return node+1;	
}

void listqueue_pop(listqueue_t* listqueue)
{
	if (listqueue->head == listqueue->tail)
		listqueue->head = listqueue->tail = NULL;
	else
		listqueue->head = listqueue->head->next;
	listqueue->queue_size -= 1;
	queue_buffer_pop(&(listqueue->buffer));	
}

void* listqueue_head(listqueue_t* listqueue)
{
	if (listqueue->head == NULL)
		return NULL;

	return listqueue->head +1;
}

void* listqueue_tail(listqueue_t* listqueue)
{
	if (listqueue->tail == NULL)
		return NULL;

	return listqueue->tail +1;
}
///////////////////////////////////////////////////////////////////

iterator_t listqueue_iterator(listqueue_t *listqueue)
{
	iterator_t iter = {listqueue->head};
	return iter;
}

void* listqueue_iter_next(iterator_t *iter)
{
	void* data = iter->data;
	if(data == NULL) return NULL;
	iter->data = (void *)((listqueue_node_t*)data)->next;
	return (listqueue_node_t*)data+1;
}
