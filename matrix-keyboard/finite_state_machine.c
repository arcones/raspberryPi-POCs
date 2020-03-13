#include <stdlib.h>
#include "finite_state_machine.h"

fsm_table* fsm_new (int state, fsm_transition* transition, void* user_data)
{
  fsm_table* this = (fsm_table*) malloc (sizeof (fsm_table));
  fsm_init (this, state, transition, user_data);
  return this;
}

void fsm_init (fsm_table* this, int state, fsm_transition* transition, void* user_data)
{
  this->current_state = state;
  this->tt = transition;
  this->user_data = user_data;
}

void finite_state_machine_destroy (fsm_table* this)
{
  free(this);
}

void fsm_fire (fsm_table* this)
{
  fsm_transition* transition;
  for (transition = this->tt; transition->orig_state >= 0; ++transition) {
    if ((this->current_state == transition->orig_state) && transition->in(this)) {
      this->current_state = transition->dest_state;
      if (transition->out)
        transition->out(this);
      break;
    }
  }
}
