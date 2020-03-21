#include <string.h>

#include "keyboard.h"

void init_keyboard (keyboard* keyboard)
{
	memcpy(keyboard->cols, (int[]) {COL_1_BCM_PIN, COL_2_BCM_PIN, COL_3_BCM_PIN, COL_4_BCM_PIN}, sizeof keyboard->cols);
	memcpy(keyboard->rows, (int[]) {ROW_1_BCM_PIN, ROW_2_BCM_PIN, ROW_3_BCM_PIN, ROW_4_BCM_PIN}, sizeof keyboard->rows);

	char physical_keys [ROWS_COUNT][COLS_COUNT] = {{ '1', '2', '3', 'A' },
												   { '4', '5', '6', 'B' },
												   { '7', '8', '9', 'C' },
												   { '*', '0', '#', 'D' }};

	memcpy(keyboard->physical_keys, physical_keys, sizeof physical_keys);

	int i;
	for (i = 0; i <4; i++){
		pullUpDnControl(keyboard->rows[i], PUD_DOWN);

		pinMode(keyboard->cols[i],OUTPUT);
		digitalWrite (keyboard->cols[i],LOW);

		digitalWrite (keyboard->rows[i],LOW);
		pinMode(keyboard->rows[i],INPUT);
		digitalWrite (keyboard->rows[i],LOW);
	}
}
