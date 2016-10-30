#include <stdlib.h>
#include <stdint.h>
#include <list>
#include <unistd.h>
extern "C"
{
#include "container/queue/listqueue.h"
#include "timer/speed.h"
#include "log/log.h"
}


speed_t speed1;
speed_t speed2;
speed_t speed3;
uint64_t temp[20000000];
void test1(void *args)
{
	uint64_t i=0;
	int count = *((int*)args);
	speed_start(&speed1);
	std::list<uint64_t> list;
	for(i=0; i<count; i++){
		list.push_back(i);
	}
	speed_end(&speed1);

	speed_start(&speed2);
	i=0;
	std::list<uint64_t>::iterator it;
	for(it = list.begin(); it!=list.end(); it++){
		temp[i++] = *it;
	}
	speed_end(&speed2);

}


int main(int argc, char **argv)
{
	if(argc < 2){
		LOG_INFO("please input count");
		return 0;
	}
	int count = atoi(argv[1]);

	int64_t ret = speed_run(test1, &count);

//	int i=0;
//	for( i=0; i<count; i++){
//		LOG_INFO("%lld\n", temp[i]);
//	}


	LOG_INFO("use time:%lldus", ret);
	LOG_INFO("init and push use time:%lldus", speed_result_us(&speed1));
	LOG_INFO("scan use time:%lldus", speed_result_us(&speed2));
	LOG_INFO("close use time:%lldus", speed_result_us(&speed3));
	return 0;
}

