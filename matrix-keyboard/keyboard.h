#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include "finite_state_machine.h"
#include <wiringPi.h>
#include <stdlib.h>

#define COL_1_BCM_PIN 0
#define COL_2_BCM_PIN 1
#define COL_3_BCM_PIN 2
#define COL_4_BCM_PIN 3

#define ROW_1_BCM_PIN 5
#define ROW_2_BCM_PIN 6
#define ROW_3_BCM_PIN 12
#define ROW_4_BCM_PIN 13

#define COLS_COUNT 	4
#define ROWS_COUNT	4

#define WAIT_COL_STATE 	0
#define WAIT_KEY_STATE	0

#define COLS_SWITCH_TIMEOUT_MS 25
#define DEBOUNCE_PREVENTION_DELAY_MS 13*COLS_SWITCH_TIMEOUT_MS

int cols[COLS_COUNT];
int rows[ROWS_COUNT];

void row1_interruption (void);
void row2_interruption (void);
void row3_interruption (void);
void row4_interruption (void);

void (*interruption_row_functions[ROWS_COUNT])();

unsigned row_debounce_delay[ROWS_COUNT];

static int iteration_period;

char matrix_keyboard[ROWS_COUNT][COLS_COUNT];

enum column_values {
	COL_1,
	COL_2,
	COL_3,
	COL_4,
};

enum row_values {
	ROW_1,
	ROW_2,
	ROW_3,
	ROW_4
};

typedef struct {
	int col;
	int row;
} key;

int key_pressed_check (fsm_table* this);
int col_timeout_check (fsm_table* this);
void reset_key(key* key);
void on_col_timeout(fsm_table* this);
void on_key_pressed(fsm_table* this);

key key_pressed;

fsm_table col_transition_table[2];
fsm_table key_transition_table[2];

#endif /* FINITE_STATE_MACHINE_H_ */

