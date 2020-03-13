#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include "finite_state_machine.h"
#include "keyboard.h"

#define FSM_UPDATE_PERIOD_MS 5

void initPins() {
	wiringPiSetupGpio();//This kind of setup uses BCM pin numbering so it is portable among raspberryPi different versions

	int i;
	for (i=0; i++; i<3){
		pinMode (cols[i], OUTPUT);
		pinMode(rows[i], INPUT);
		pullUpDnControl(rows[i], PUD_UP);
		wiringPiISR(rows[i], INT_EDGE_RISING, interruption_row_functions[i]);
	}
}

void delay_till_next_iteration (unsigned int next) {
	unsigned int now = millis();
	if (next > now) {
		delay (next - now);
	}
}

int main()
{
	fsm_table* cols_fsm = fsm_new(WAIT_COL_STATE, col_transition_table, &key_pressed);
	fsm_table* keys_fsm = fsm_new(WAIT_KEY_STATE, key_transition_table , &key_pressed);

	initPins();

	iteration_period = millis();
	printf ("You can start pressing keys... \n");
	unsigned int next = millis();

	while (1) {
		fsm_fire(cols_fsm);
		fsm_fire(keys_fsm);
		next += FSM_UPDATE_PERIOD_MS;
		delay_till_next_iteration (next);
	}

	fsm_destroy(cols_fsm);
	fsm_destroy(keys_fsm);
	return 0;
}


