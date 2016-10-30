#include <unistd.h>

extern "C"
{
#include "log/log.h"
#include "timer/speed.h"
}

void run(void *args)
{
}


int main(int argc, char **argv)
{
	if(argc < 1){
		LOG_INFO("please input something");
		return 0;
	}

	int64_t ret = speed_run(run, argv[1]);
	LOG_INFO("use time:%lldus", ret);

	return 0;
}

