#include <stdlib.h>
#include <stdint.h>
#include "container/buffer.h"

#include <stdio.h>

buffer_t* buffer_new(uint32_t blksize)
{
	buffer_t* buffer = (buffer_t*)malloc(sizeof(buffer_t)+blksize);
	if (buffer==NULL){
		return NULL;
	}

	buffer->size = blksize;
	buffer->freesize = blksize;
	buffer->fullflag = 0;
	buffer->offset = 0;
	buffer->count = 0;
	buffer->next = NULL;
	return buffer;
}

buffer_t* buffer_clean(buffer_t* buffer)
{
	buffer->freesize = buffer->size;
	buffer->fullflag = 0;
	buffer->offset = 0;
	buffer->count = 0;
	buffer->next = NULL;
	return buffer;
}

void buffer_close(buffer_t* buffer)
{
	free(buffer);
}

void* buffer_alloc(buffer_t *buffer,uint32_t size)
{
	//判断是否有空间
	if(buffer->freesize < size){
		buffer->fullflag = 1;
		return NULL;
	}

	buffer->freesize -= size;
	uint32_t offset = buffer->offset;
	buffer->offset += size;
	buffer->count += 1;

	return (char*)buffer + sizeof(buffer_t) + offset;
}

void* buffer_free(buffer_t *buffer)
{
	buffer->count -= 1;
}

////////////////////////////////////////////////////////////////////

queue_buffer_t* queue_buffer_init(queue_buffer_t *queue_buf, uint32_t blksize)
{
	queue_buf->cur_buf = NULL;
	queue_buf->head_buf = NULL;
	queue_buf->recycle_buf = NULL;
	queue_buf->blksize = blksize;
	return queue_buf;
}

void queue_buffer_close(queue_buffer_t *queue_buf)
{
	buffer_t *buf = queue_buf->recycle_buf;
	buffer_t *temp;
	if(buf != NULL){	//释放回收站空间
		while(1){
			temp = buf->next;
			buffer_close(buf);
			if(temp == NULL) break;
			buf = temp;
		}
	}

	buf = queue_buf->head_buf;
	if(buf != NULL){
		while(1){
			temp = buf->next;
			buffer_close(buf);
			if(temp == NULL) break;
			buf = temp;
		}
	}	
}

static buffer_t* _get_buf(queue_buffer_t *queue_buf)
{
	buffer_t *buffer = queue_buf->head_buf;
	buffer_t *tempbuffer;

	if (buffer != NULL && buffer->count == 0 && buffer->fullflag == 1){//头部有空闲,直接用
		if( queue_buf->cur_buf == buffer){
			queue_buf->cur_buf = buffer_clean(buffer);
			return buffer;
		}
		queue_buf->head_buf = buffer->next;
		queue_buf->cur_buf = queue_buf->cur_buf->next = buffer;
		return buffer;
	}
	//先从recycle_buf里找
	if (queue_buf->recycle_buf != NULL){
		tempbuffer = queue_buf->recycle_buf->next;
		buffer = buffer_clean(queue_buf->recycle_buf);
		queue_buf->recycle_buf = tempbuffer;
		if( queue_buf->head_buf != NULL)
			queue_buf->cur_buf = queue_buf->cur_buf->next = buffer;
		else
			queue_buf->head_buf = queue_buf->cur_buf = buffer;
		return buffer;
	}
	//找不到就新建立
	buffer = buffer_new(queue_buf->blksize);
	if (buffer == NULL){
		return NULL;
	}


	if (queue_buf->head_buf != NULL)
		queue_buf->cur_buf = queue_buf->cur_buf->next = buffer;
	else
		queue_buf->head_buf = queue_buf->cur_buf = buffer;

	return buffer;
}


void* queue_buffer_alloc(queue_buffer_t *queue_buf, uint32_t size)
{
	void *buf;
	if (queue_buf->cur_buf == NULL) {
		if(NULL == _get_buf(queue_buf) ) {
			return NULL;
		}
	}

		
	while(1) {
		buf = buffer_alloc(queue_buf->cur_buf, size);
		if (buf == NULL){
			if(NULL == _get_buf(queue_buf) ) {
				return NULL;
			}else {
				continue;
			}
		}
		return buf;
	}
}

void* queue_buffer_pop(queue_buffer_t *queue_buf)
{
	buffer_t *buffer = queue_buf->head_buf;
	buffer_free(buffer);
	if (buffer->count == 0 && buffer->fullflag == 1){
		if( queue_buf->cur_buf == buffer)
			queue_buf->cur_buf = NULL;
		queue_buf->head_buf = buffer->next;
		buffer->next = queue_buf->recycle_buf;
		queue_buf->recycle_buf = buffer;
	}
}

void queue_buffer_clean(queue_buffer_t *queue_buf)
{
	if (queue_buf->head_buf != NULL){
		queue_buf->cur_buf->next = queue_buf->recycle_buf;
		queue_buf->recycle_buf = queue_buf->head_buf;
		queue_buf->head_buf = NULL;
		queue_buf->cur_buf = NULL;
	}
}

