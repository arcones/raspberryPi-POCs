#include "keyboard.h"
#include "mealy_machine.h"

#define MEALY_MACHINE_TRANSITION_PERIOD_MS	10

void wait_till_mealy_machine_period_is_over(unsigned int next) {
	unsigned int now = millis();
	if (next > now) {
		delay(next - now);
	}
}

int main() {
	init_keyboard();

	mealy_machine* cols_machine = mealy_machine_new(WAIT_COL_STATE,col_transitions);
	mealy_machine* key_pressed_machine = mealy_machine_new(WAIT_KEY_STATE, key_transitions);

	unsigned int next = millis();
	while (1) {
		mealy_machine_fire(cols_machine);
		mealy_machine_fire(key_pressed_machine);
		next += MEALY_MACHINE_TRANSITION_PERIOD_MS;
		wait_till_mealy_machine_period_is_over(next);
	}

	timer_destroy(keyboard.col_timeout_timer);
	mealy_machine_destroy(cols_machine);
	mealy_machine_destroy(key_pressed_machine);

	return 0;
}
