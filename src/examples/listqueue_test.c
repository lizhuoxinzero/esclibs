#include "log/log.h"
#include "timer/speed.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include "container/queue/listqueue.h"



speed_t speed1;
speed_t speed2;
speed_t speed3;
uint64_t temp[20000000];
void test1(void *args)
{
	int i=0;
	int count = *((int*)args);
	listqueue_t listqueue;
	speed_start(&speed1);
	listqueue_init(&listqueue, 8192);
	for(i=0; i<count; i++){
		uint64_t *node = (uint64_t*)listqueue_push_new(&listqueue, sizeof(uint64_t));
		*node = i;
	}
	speed_end(&speed1);

	speed_start(&speed2);
	iterator_t iter = listqueue_iterator(&listqueue);
	i=0;
	uint64_t *value;
	while(value = (uint64_t*)listqueue_iter_next(&iter), value) {
		temp[i++] = *value;
	}
	speed_end(&speed2);

	speed_start(&speed3);
	listqueue_close(&listqueue);
	speed_end(&speed3);
}

void test2(void *args)
{
	int i=0;
	int count = *((int*)args);
	listqueue_t listqueue;
	listqueue_init(&listqueue, 64);

int k=30;
while(1){
	for(i=0; i<count; i++){
		uint64_t *node = (uint64_t*)listqueue_push_new(&listqueue, sizeof(uint64_t));
		*node = i;
		uint64_t head = *(uint64_t*)listqueue_head(&listqueue);
		uint64_t tail = *(uint64_t*)listqueue_tail(&listqueue);
//		printf("head:%lld\ttail:%lld\tsize:%d\n", head, tail, listqueue.queue_size);
	}

	for(i=0; i<count; i++) {
		uint64_t head = *(uint64_t*)listqueue_head(&listqueue);
		uint64_t tail = *(uint64_t*)listqueue_tail(&listqueue);
//		printf("head:%lld\ttail:%lld\tsize:%d\n", head, tail, listqueue.queue_size);
		listqueue_pop(&listqueue);
	}
}

	listqueue_close(&listqueue);
}

int main(int argc, char **argv)
{
	if(argc < 2){
		LOG_INFO("please input count");
		return 0;
	}
	int count = atoi(argv[1]);

//while(1){
	int64_t ret = speed_run(test1, &count);

//	int i=0;
//	for( i=0; i<count; i++){
//		LOG_INFO("%lld\n", temp[i]);
//	}


	LOG_INFO("use time:%ldus", ret);
	LOG_INFO("init and push use time:%ldus", speed_result_us(&speed1));
	LOG_INFO("scan use time:%ldus", speed_result_us(&speed2));
	LOG_INFO("close use time:%ldus", speed_result_us(&speed3));
//}
	return 0;
}

