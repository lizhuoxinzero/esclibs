#ifndef ES_LIST_H
#define ES_LIST_H

typedef struct{
	void *data;
	list_node_t *next;
}list_node_t;

typedef struct{
	list_node_t *head;
	list_node_t *tail;
}list_t;


/**
 * Create and init a list_t
 *
 * @param blksize the block buffer size, if zero then use 512byte
 * @return a pointer to a newly initialized list_t object, if error then return NULL
 */
extern list_t* list_new(int blksize);

extern int list_close(list_t* list);

extern list_t* list_push(void *data);

extern list_node_t* list_get_head();

extern 
#endif
