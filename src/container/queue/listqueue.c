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

	listqueue->cur_buf = buffer_new(blksize);
	if (listqueue->cur_buf == NULL){
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
	
	return 0;
}

int listqueue_close_and_free(listqueue_t* listqueue)
{
	listqueue_close(listqueue);
	free(listqueue); //释放分配的空间
	return 0;
}

static buffer_t* _get_buf(listqueue_t *listqueue)
{
	buffer_t *buffer;
	//先从recycle_buf里找
	if (listqueue->recycle_buf != NULL) {
		buffer = listqueue->recycle_buf;
		listqueue->recycle_buf = buffer->next;
		return buffer;
	}
	buffer = buffer_new(listqueue->blksize);
	if (buffer == NULL) {
		return NULL;
	}
	
	if (listqueue->cur_buf != NULL)
		listqueue->cur_buf = listqueue->cur_buf->next = buffer;
	else
		listqueue->cur_buf = buffer;
	
	return buffer;
}

static listqueue_node_t* _get_node(listqueue_t *listqueue, uint32_t size)
{
	listqueue_node_t *node;
	while(1) {
		node = (listqueue_node_t *)buffer_alloc(listqueue->cur_buf, size+sizeof(listqueue_node_t));
		if (node == NULL){
			if (NULL == _get_buf(listqueue) ) {
				return NULL;
			}else {
				continue;
			}
		}
		return node;
	}
}

void* listqueue_push(listqueue_t *listqueue, void *data, uint32_t size)
{
	//获取缓存块
	listqueue_node_t *node = _get_node(listqueue, size);
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

	return node+1;	//返回内存可用地址	
}

void* listqueue_push_new(listqueue_t *listqueue, uint32_t size)
{
	//获取缓存块
	listqueue_node_t *node = _get_node(listqueue, size);
	if (node == NULL){	//严重失败，内存都申请不到了
		return NULL;
	}

	node->next = NULL;

	if(listqueue->head == NULL){
		listqueue->tail->next = node;
		listqueue->tail = node;
	}else{
		listqueue->head = node;
		listqueue->tail = node;
	}
	return node+1;	
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
