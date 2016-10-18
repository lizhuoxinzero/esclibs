#include <stdlib.h>
#include <stdint.h>
#include "container/buffer.h"


buffer_t* buffer_new(uint32_t blksize)
{
	buffer_t* buffer = (buffer_t*)malloc(sizeof(buffer_t)+blksize);
	if (buffer==NULL){
		return NULL;
	}

	buffer->size = blksize;
	buffer->freesize = blksize;
	buffer->offset = 0;
	buffer->count = 0;
	buffer->next = NULL;
	return buffer;
}

int buffer_close(buffer_t* buffer)
{
	free(buffer);
	return 0;
}

void* buffer_alloc(buffer_t *buffer,uint32_t size)
{
	//判断是否有空间
	if(buffer->freesize < size){
		return NULL;
	}

	buffer->freesize -= size;
	uint32_t offset = buffer->offset;
	buffer->offset += size;
	buffer->count += 1;

	return (char*)buffer + sizeof(buffer_t) + offset;

}

