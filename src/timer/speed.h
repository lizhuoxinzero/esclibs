#ifndef ES_SPEED_H
#define ES_SPEED_H

#include <stdint.h>

typedef struct{
	uint64_t start;
	uint64_t end;
}speed_t;


extern void speed_init(speed_t *speed);
extern void speed_start(speed_t *speed);
extern void speed_end(speed_t *speed);

extern uint64_t  speed_result_s(speed_t *speed);
extern uint64_t  speed_result_ms(speed_t *speed);
extern uint64_t  speed_result_us(speed_t *speed);


#endif
