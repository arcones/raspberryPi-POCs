#ifndef _TECLADO_TL04_H_
#define _TECLADO_TL04_H_

#include "mealy_machine.h"
#include "timer.h"

#define COL_1_BCM_PIN 	0
#define COL_2_BCM_PIN 	1
#define COL_3_BCM_PIN 	2
#define COL_4_BCM_PIN 	3

#define ROW_1_BCM_PIN 	19
#define ROW_2_BCM_PIN 	26
#define ROW_3_BCM_PIN 	12
#define ROW_4_BCM_PIN 	13

#define COLS_SWITCH_TIMEOUT_MS			25
#define	DEBOUNCE_PREVENTION_DELAY_MS	10*COLS_SWITCH_TIMEOUT_MS

#define COLS_COUNT 	4
#define ROWS_COUNT 	4

#define MUTEX_FLAG			0x01

#define COL_TIMEOUT_FLAG  	0x02
#define KEYSTROKE_FLAG 		0x04

#define WAIT_COL_STATE 	0
#define WAIT_KEY_STATE	0

enum col_values {
	COL_1, COL_2, COL_3, COL_4
};

enum row_values {
	ROW_1, ROW_2, ROW_3, ROW_4
};

typedef struct {
	int col;
	int row;
} Key;

typedef struct {
	int cols[COLS_COUNT];
	int rows[ROWS_COUNT];
	int row_debounce_delay[ROWS_COUNT];
	void (*interruption_row_functions[ROWS_COUNT])(void);
	int current_col;
	Key key_pressed;
	timer* col_timeout_timer;
	int flags;
	char physical_keys[ROWS_COUNT][COLS_COUNT];
} Keyboard;

extern Keyboard keyboard;
extern machine_transition col_transitions[];
extern machine_transition key_transitions[];

void init_keyboard();

void row1_interruption(void);
void row2_interruption(void);
void row3_interruption(void);
void row4_interruption(void);

void col_timeout_interruption(union sigval value);

#endif /* _TECLADO_TL04_H_ */
