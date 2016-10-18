#include <stdlib.h>
#include "container/list/list.h"
#include "container/buffer.h"



list_t* list_new(int blksize)
{
	
	list_t *list = malloc(sizeof(list_t));
	list->head = NULL;
	list->tail = NULL;
	return list;
}


int list_close(list_t* list)
{
	
	return 0;
}

list_t* list_push(void *data)
{
	//获取缓存块
	

}
