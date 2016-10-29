#include <unistd.h>
#include "log/log.h"
#include "timer/speed.h"

extern void  speed_start(speed_t *speed);

int main()
{
	speed_t speed;
	speed_start(&speed);
	sleep(5);	
	speed_end(&speed);
	LOG_DEBUG("use time:%lds", speed_result_s(&speed));
	LOG_DEBUG("use time:%ldms", speed_result_ms(&speed));
	LOG_DEBUG("use time:%ldus", speed_result_us(&speed));
	return 0;
}
