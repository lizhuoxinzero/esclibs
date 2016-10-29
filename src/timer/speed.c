#include <stdio.h>
#include <sys/time.h>
#include "timer/speed.h"


void speed_init(speed_t *speed)
{
	speed->start = 0;
	speed->end = 0;
}

void speed_start(speed_t *speed)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	speed->start = tv.tv_sec*1000000 + tv.tv_usec;
}

void speed_end(speed_t *speed)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	speed->end = tv.tv_sec*1000000 + tv.tv_usec;
}

uint64_t speed_result_s(speed_t *speed)
{
	return (speed->end - speed->start) / 1000000;
}

uint64_t speed_result_ms(speed_t *speed)
{
	return (speed->end - speed->start) / 1000;
}

uint64_t speed_result_us(speed_t *speed)
{
	return speed->end - speed->start;
}
