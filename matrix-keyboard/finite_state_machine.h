#ifndef FINITE_STATE_MACHINE_H_
#define FINITE_STATE_MACHINE_H_

typedef struct fsm_table fsm_table;

typedef int (*fsm_input_function) (fsm_table*);
typedef void (*fsm_output_function) (fsm_table*);

typedef struct fsm_transition {
  int orig_state;
  fsm_input_function in;
  int dest_state;
  fsm_output_function out;
} fsm_transition;

struct fsm_table  {
	  int current_state;
	  fsm_transition* transition;
	  void* user_data;
};

fsm_table* fsm_new (int state, fsm_transition* transition, void* user_data);
void fsm_init (fsm_table* this, int state, fsm_transition* transition, void* user_data);
void fsm_fire (fsm_table* this);
void fsm_destroy (fsm_table* this);

#endif /* FINITE_STATE_MACHINE_H_ */
