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

static int cols[COLS_COUNT] = {COL_1_BCM_PIN,COL_2_BCM_PIN,COL_3_BCM_PIN,COL_4_BCM_PIN};
static int rows[ROWS_COUNT] = {ROW_1_BCM_PIN,ROW_2_BCM_PIN,ROW_3_BCM_PIN,ROW_4_BCM_PIN};

static void row1_interruption (void);
static void row2_interruption (void);
static void row3_interruption (void);
static void row4_interruption (void);
static void (*interruption_row_functions[ROWS_COUNT])() = {row1_interruption, row2_interruption, row3_interruption, row4_interruption};
static unsigned row_debounce_delay[ROWS_COUNT] = {0,0,0,0};
static int iteration_period;

char matrix_keyboard[ROWS_COUNT][COLS_COUNT] = {
		{'1', '2', '3', 'A'},
		{'4', '5', '6', 'B'},
		{'7', '8', '9', 'C'},
		{'*', '0', '#', 'D'}
};

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

static int key_pressed_check (fsm_table* this);
static int col_timeout_check (fsm_table* this);
static void reset_key(key* key);
static void on_col_timeout(fsm_table* this);
static void on_key_pressed(fsm_table* this);

key key_pressed = {
		.col = -1,
		.row = -1
};

static fsm_table col_transition_table[] = {
		{WAIT_COL_STATE, col_timeout_check, WAIT_COL_STATE, on_col_timeout },
		{ -1, NULL, -1, NULL }, };


static fsm_table key_transition_table[] = {
		{WAIT_KEY_STATE, key_pressed_check, WAIT_KEY_STATE, on_key_pressed},
		{-1, NULL, -1, NULL },
};
#endif /* FINITE_STATE_MACHINE_H_ */

