#include <unistd.h>
#include "log/log.h"
#include "timer/speed.h"


void test(void *args)
{
	LOG_INFO("get a string args:%s, and start speed_run test", (char*)args);
	sleep(3);

}

int main()
{
	speed_t speed;
	speed_start(&speed);
	sleep(5);	
	speed_end(&speed);
	LOG_DEBUG("use time:%lds", speed_result_s(&speed));
	LOG_DEBUG("use time:%ldms", speed_result_ms(&speed));
	LOG_DEBUG("use time:%ldus", speed_result_us(&speed));
	

	int64_t ret = speed_run(test, "testargs");	
	LOG_DEBUG("speed_run use time:%ldus", ret);
	return 0;
}
