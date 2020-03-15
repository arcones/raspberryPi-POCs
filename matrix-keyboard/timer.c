#include <stdlib.h>
#include <signal.h>
#include <time.h>

#include "timer.h"

timer* timer_new(notify_func_t interruption) {
	timer* this = (timer*) malloc(sizeof(timer));
	timer_init(this, interruption);
	return this;
}

void timer_init(timer* this, notify_func_t isr) {
	this->se.sigev_notify = SIGEV_THREAD;
	this->se.sigev_value.sival_ptr = &(this->timerid);
	this->se.sigev_notify_function = isr;
	this->se.sigev_notify_attributes = NULL;
	timer_create(CLOCK_REALTIME, &(this->se), &(this->timerid));
}

void timer_destroy(timer* this) {
	timer_stop(this);
	free(this);
}

void timer_start(timer* this, int ms) {
	this->spec.it_value.tv_sec = ms / 1000;
	this->spec.it_value.tv_nsec = (ms % 1000) * 1000000;
	this->spec.it_interval.tv_sec = 0;
	this->spec.it_interval.tv_nsec = 0;
	timer_settime(this->timerid, 0, &(this->spec), NULL);
}

void timer_stop(timer* this) {
	timer_delete(this->timerid);
}

