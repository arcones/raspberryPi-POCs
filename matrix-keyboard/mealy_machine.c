#include <stdlib.h>

#include "mealy_machine.h"

mealy_machine* mealy_machine_new(int state, machine_transition* transition) {
	mealy_machine* this = (mealy_machine*) malloc(sizeof(mealy_machine));
	mealy_machine_init(this, state, transition);
	return this;
}

void mealy_machine_init(mealy_machine* this, int state, machine_transition* transition) {
	this->current_state = state;
	this->transition = transition;
}

void mealy_machine_destroy(mealy_machine* this) {
	free(this);
}

void mealy_machine_fire(mealy_machine* this) {
	machine_transition* transition;
	for (transition = this->transition; transition->orig_state >= 0; ++transition) {
		if ((this->current_state == transition->orig_state) && transition->in(this)) {
			this->current_state = transition->dest_state;
			if (transition->out)
				transition->out(this);
			break;
		}
	}
}
