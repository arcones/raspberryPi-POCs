#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include <wiringPi.h>

#define ROWS_COUNT 4
#define COLS_COUNT 4

#define COL_1_BCM_PIN 	0
#define COL_2_BCM_PIN 	1
#define COL_3_BCM_PIN 	2
#define COL_4_BCM_PIN 	3

#define ROW_1_BCM_PIN 	19
#define ROW_2_BCM_PIN 	26
#define ROW_3_BCM_PIN 	12
#define ROW_4_BCM_PIN 	13

typedef struct {
	char physical_keys[ROWS_COUNT][COLS_COUNT];
	int cols[COLS_COUNT];
	int rows[ROWS_COUNT];
} keyboard;

void init_keyboard (keyboard* keyboard);
void waitTillKeyRelease(keyboard* keyboard, int rowNum);

#endif /* KEYBOARD_H_ */
