#ifndef TIMER_H_
#define TIMER_H_

#include <signal.h>
#include <time.h>

struct timer {
	timer_t timerid;
	struct itimerspec spec;
	struct sigevent se;
};

typedef struct timer timer;

typedef void (*notify_func_t)(union sigval);

timer* timer_new(notify_func_t isr);
void timer_init(timer* this, notify_func_t isr);
void timer_destroy(timer* this);
void timer_start(timer* this, int ms);
void timer_stop(timer* this);

#endif /* TIMER_H_ */
