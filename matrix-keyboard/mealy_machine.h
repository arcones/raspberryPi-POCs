#ifndef MEALY_MACHINE_H_
#define MEALY_MACHINE_H_

typedef struct mealy_machine mealy_machine;

typedef int (*in_function)(mealy_machine*);
typedef void (*out_funtion)(mealy_machine*);

typedef struct machine_transition {
	int orig_state;
	in_function in;
	int dest_state;
	out_funtion out;
} machine_transition;

struct mealy_machine {
	int current_state;
	machine_transition* transition;
};

mealy_machine* mealy_machine_new(int state, machine_transition* transition);
void mealy_machine_init(mealy_machine* this, int state, machine_transition* transition);
void mealy_machine_fire(mealy_machine* this);
void mealy_machine_destroy(mealy_machine* this);

#endif /* MEALY_MACHINE_H_ */
