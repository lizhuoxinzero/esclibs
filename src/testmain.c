#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "fs/listfile.h"
#include "container/buffer.h"
#include "container/queue/listqueue.h"
#include "hash/sha1/sha1.h"
#include "log/log.h"


int main(int argc, char **argv)
{
/*
	if (argc < 2){
		printf("please input path\n");
		return 0;
	}
	listqueue_t *queue = listqueue_new(512);
	int ret = listfile(argv[1], strlen(argv[1]),1, queue);
	if (ret == -1){
		printf("the path has problem\n");
	}
	int i=0;
	iterator_t iter = listqueue_iterator(queue);
	while(1){
		char *str = (char*)listqueue_iter_next(&iter);
		if (str == NULL) break;
		printf("%d %s\n", i, str);
		i++;
	}
	
	listqueue_close(queue);
*/

/*
	buffer_t* buf = buffer_new(512);
	int i=0;
	for(i=0; i<200; i++){
		void *p = buffer_alloc(buf, 4);
		if (p==NULL){
			printf("no now :%d\n", i);
			break;
		}
	}
	buffer_close(buf);
*/	
	
/*
	while(1){
		listqueue_t *queue = listqueue_new(512);
		if(queue == NULL){
			printf("create queue error");
			return 0;
		}
	
		listqueue_node_t *node;
		int i;
		for(i=0; i<20000; i++){
			node = listqueue_push(queue, "hehehehe哈哈", sizeof("hehehehe哈哈"));
			if (node == NULL){
				printf("create queue error");
				listqueue_close(queue);
				return 0;
			}
		}
	
		i=0;
		iterator_t iter = listqueue_iterator(queue);
		while(1){
			char *str = (char*)listqueue_iter_next(&iter);
			if (str == NULL) break;
			printf("%d %s\n", i, str);
			i++;
		}
		listqueue_close(queue);

	}
*/
/*
	if (argc < 2){
		printf("please input path\n");
		return 0;
	}
	uint8_t sha1[SHA1_STRLEN];
	int ret = SHA1_file(argv[1], sha1);
	if (ret == -1){
		printf("sha1file error :%s\n", strerror(errno));
		return 0;
	}
	print_sha1(sha1);	
*/



	//test log.h
	LOG_INFO("%s-%d", "sdf",34);
	LOG_DEBUG("%s-%d", "sdf",34);
	LOG_ERROR_MSG("%s-%d", "sdf",34);
	LOG_FATAL("%s-%d", "sdf",34);
	LOG_INFO("%s-%d", "sdf",34);


	return 0;
}

