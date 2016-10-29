#ifndef ES_SPEED_H
#define ES_SPEED_H

#include <stdint.h>

typedef struct{
	int64_t start;
	int64_t end;
}speed_t;


extern void speed_init(speed_t *speed);
extern void speed_start(speed_t *speed);
extern void speed_end(speed_t *speed);
extern int64_t speed_run(void(*run)(void *args), void *args);


extern int64_t  speed_result_s(speed_t *speed);
extern int64_t  speed_result_ms(speed_t *speed);
extern int64_t  speed_result_us(speed_t *speed);


#endif
