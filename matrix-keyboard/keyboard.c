#include "keyboard.h"

#include <stdio.h>


Keyboard keyboard = {
	.cols = { COL_1_BCM_PIN, COL_2_BCM_PIN, COL_3_BCM_PIN, COL_4_BCM_PIN },
	.rows = { ROW_1_BCM_PIN, ROW_2_BCM_PIN, ROW_3_BCM_PIN, ROW_4_BCM_PIN },
	.interruption_row_functions = { row1_interruption, row2_interruption, row3_interruption, row4_interruption },
	.row_debounce_delay = { 0, 0, 0, 0 },
	.current_col = COL_1,
	.key_pressed = { .row = -1, .col = -1 },
	.col_timeout_timer = NULL,
	.flags = 0,
	.physical_keys = {
					 { '1', '2', '3', 'A' },
					 { '4', '5', '6', 'B' },
					 { '7', '8', '9', 'C' },
					 { '*', '0', '#', 'D' }
	}
 };

void init_keyboard() {
	keyboard.col_timeout_timer = timer_new(col_timeout_interruption);

	wiringPiSetupGpio();//wiringPi setup that uses BCM pin numbering so it is portable among raspberryPi different versions

	int i;
	for (i = 0; i < 4; i++) {
		pinMode(keyboard.cols[i], OUTPUT);
		digitalWrite(keyboard.cols[i], HIGH);

		pinMode(keyboard.rows[i], INPUT);
		pullUpDnControl(keyboard.rows[i], PUD_UP);
		wiringPiISR(keyboard.rows[i], INT_EDGE_RISING,keyboard.interruption_row_functions[i]);
	}

	timer_start(keyboard.col_timeout_timer, COLS_SWITCH_TIMEOUT_MS);
}

int col_timeout_check() {
	int result = 0;
	piLock(MUTEX_FLAG);
	result = (keyboard.flags & COL_TIMEOUT_FLAG);
	piUnlock(MUTEX_FLAG);

	return result;
}

void on_col_timeout() {
	piLock(MUTEX_FLAG);

	if (keyboard.current_col == COL_4) {
		keyboard.current_col = COL_1;
	} else {
		keyboard.current_col++;
	}

	int i;
	for(i = 0; i < 4; i++){
		digitalWrite(keyboard.cols[i], LOW);
	}
	digitalWrite(keyboard.cols[keyboard.current_col], HIGH);

	keyboard.flags &= (~COL_TIMEOUT_FLAG);

	piUnlock(MUTEX_FLAG);

	timer_start(keyboard.col_timeout_timer, COLS_SWITCH_TIMEOUT_MS);
}

int keystroke_check() {
	int result = 0;
	piLock(MUTEX_FLAG);
	result = (keyboard.flags & KEYSTROKE_FLAG);
	piUnlock(MUTEX_FLAG);

	return result;
}

void on_keystroke() {
	piLock(MUTEX_FLAG);
	keyboard.flags &= ~KEYSTROKE_FLAG;

	printf("%c has been pressed\n",keyboard.physical_keys[keyboard.key_pressed.row][keyboard.key_pressed.col]);

	keyboard.key_pressed.row = -1;
	keyboard.key_pressed.col = -1;
	keyboard.current_col = -1;

	piUnlock(MUTEX_FLAG);
}

//------------------------------------------------------
// SUBRUTINAS DE ATENCION A LAS INTERRUPCIONES
//------------------------------------------------------

//TODO valorar juntar las 4 funciones de filas? o sacar contenido a funciones comunes a todas ellas


void row1_interruption(void) {
	if (millis() < keyboard.row_debounce_delay[ROW_1]) {
		keyboard.row_debounce_delay[ROW_1] = millis() + DEBOUNCE_PREVENTION_DELAY_MS;
		return;
	}
	piLock(MUTEX_FLAG);
	keyboard.key_pressed.row = ROW_1;
	keyboard.key_pressed.col = keyboard.current_col;
	keyboard.flags |= KEYSTROKE_FLAG;
	piUnlock(MUTEX_FLAG);
	keyboard.row_debounce_delay[ROW_1] = millis() + DEBOUNCE_PREVENTION_DELAY_MS;
}

void row2_interruption(void) {
	if (millis() < keyboard.row_debounce_delay[ROW_2]) {
		keyboard.row_debounce_delay[ROW_2] = millis() + DEBOUNCE_PREVENTION_DELAY_MS;
		return;
	}
	piLock(MUTEX_FLAG);
	keyboard.key_pressed.row = ROW_2;
	keyboard.key_pressed.col = keyboard.current_col;
	keyboard.flags |= KEYSTROKE_FLAG;
	piUnlock(MUTEX_FLAG);
	keyboard.row_debounce_delay[ROW_2] = millis() + DEBOUNCE_PREVENTION_DELAY_MS;
}

void row3_interruption(void) {
	if (millis() < keyboard.row_debounce_delay[ROW_3]) {
		keyboard.row_debounce_delay[ROW_3] = millis() + DEBOUNCE_PREVENTION_DELAY_MS;
		return;
	}
	piLock(MUTEX_FLAG);
	keyboard.key_pressed.row = ROW_3;
	keyboard.key_pressed.col = keyboard.current_col;
	keyboard.flags |= KEYSTROKE_FLAG;
	piUnlock(MUTEX_FLAG);
	keyboard.row_debounce_delay[ROW_3] = millis() + DEBOUNCE_PREVENTION_DELAY_MS;
}

void row4_interruption(void) {
	if (millis() < keyboard.row_debounce_delay[ROW_4]) {
		keyboard.row_debounce_delay[ROW_4] = millis() + DEBOUNCE_PREVENTION_DELAY_MS;
		return;
	}
	piLock(MUTEX_FLAG);
	keyboard.key_pressed.row = ROW_4;
	keyboard.key_pressed.col = keyboard.current_col;
	keyboard.flags |= KEYSTROKE_FLAG;
	piUnlock(MUTEX_FLAG);
	keyboard.row_debounce_delay[ROW_4] = millis() + DEBOUNCE_PREVENTION_DELAY_MS;
}

void col_timeout_interruption(union sigval value) {
	piLock(MUTEX_FLAG);
	keyboard.flags |= COL_TIMEOUT_FLAG;
	piUnlock(MUTEX_FLAG);
}

machine_transition col_transitions[] = {
		{ WAIT_COL_STATE, col_timeout_check, WAIT_COL_STATE, on_col_timeout },
		{ -1, NULL, -1, NULL }, };

machine_transition key_transitions[] = {
		{ WAIT_KEY_STATE, keystroke_check, WAIT_KEY_STATE, on_keystroke }, {
		-1, NULL, -1, NULL }, };


