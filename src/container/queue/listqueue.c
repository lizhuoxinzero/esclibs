#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "container/queue/listqueue.h"
#include "container/buffer.h"



listqueue_t* listqueue_new(uint32_t blksize)
{
	
	listqueue_t *listqueue = malloc(sizeof(listqueue_t));
	if(listqueue == NULL) return NULL;

	listqueue->cur_buf = buffer_new(blksize);
	if (listqueue->cur_buf == NULL){
		free(listqueue);
		return NULL;
	}
	listqueue->head_buf = listqueue->cur_buf;
	listqueue->recycle_buf = NULL;
	listqueue->blksize = blksize;
	listqueue->head = NULL;
	listqueue->tail = NULL;
	return listqueue;
}


int listqueue_close(listqueue_t* listqueue)
{
	buffer_t *buf = listqueue->recycle_buf;
	buffer_t *temp;
	if(buf != NULL){	//释放回收站空间
		while(1){
			temp = buf->next;
			buffer_close(buf);
			if(temp == NULL) break;
			buf = temp;
		}
	}
	
	buf = listqueue->head_buf;
	if(buf != NULL){	//释放正使用的buffer对象
		while(1){
			temp = buf->next;
			buffer_close(buf);
			if(temp == NULL) break;
			buf = temp;
		}
	}
	
	free(listqueue); //释放分配的空间

	return 0;
}

listqueue_node_t* listqueue_push(listqueue_t *listqueue, void *data, uint32_t size)
{
	//获取缓存块
	listqueue_node_t *node = (listqueue_node_t *)buffer_alloc(listqueue->cur_buf, size+sizeof(listqueue_node_t));
	if (node == NULL){	//存储不够了
		buffer_t *buffer = buffer_new(listqueue->blksize);
		if( buffer == NULL){
			return NULL;
		}
		listqueue->cur_buf = listqueue->cur_buf->next = buffer;
		node = (listqueue_node_t *)buffer_alloc(listqueue->cur_buf, size+sizeof(listqueue_node_t));
		if (node == NULL){
			return NULL;
		}
	}

	memcpy(node+1, data, size);
	node->next = NULL;

	if(listqueue->head == NULL){
		listqueue->head = node;
		listqueue->tail = node;
	}else{
		listqueue->tail->next = node;
		listqueue->tail = node;
	}

	return node;	
}


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
