#include "keyboard.h"

int cols[COLS_COUNT] = {COL_1_BCM_PIN,COL_2_BCM_PIN,COL_3_BCM_PIN,COL_4_BCM_PIN};
int rows[ROWS_COUNT] = {ROW_1_BCM_PIN,ROW_2_BCM_PIN,ROW_3_BCM_PIN,ROW_4_BCM_PIN};

void (*interruption_row_functions[ROWS_COUNT])() = {row1_interruption, row2_interruption, row3_interruption, row4_interruption};

unsigned row_debounce_delay[ROWS_COUNT] = {0,0,0,0};

char matrix_keyboard[ROWS_COUNT][COLS_COUNT] = {
		{'1', '2', '3', 'A'},
		{'4', '5', '6', 'B'},
		{'7', '8', '9', 'C'},
		{'*', '0', '#', 'D'}
};

key key_pressed = {
		.col = -1,
		.row = -1
};

fsm_table col_transition_table[] = {
	{WAIT_COL_STATE, col_timeout_check, WAIT_COL_STATE, on_col_timeout },
	{ -1, NULL, -1, NULL }
};

fsm_table key_transition_table[] = {
	{WAIT_KEY_STATE, key_pressed_check, WAIT_KEY_STATE, on_key_pressed},
	{-1, NULL, -1, NULL }
};

void ignore_debounce(int row_num){
	if (millis() < row_debounce_delay[row_num]) return;
}

void update_debouce_prevention_time(int row_num) {
	row_debounce_delay[row_num] = millis() + DEBOUNCE_PREVENTION_DELAY_MS;
}

void row1_interruption (void){
	ignore_debounce(ROW_1);
	key_pressed.row = ROW_1;
	update_debouce_prevention_time(ROW_1);
}

void row2_interruption (void) {
	ignore_debounce(ROW_2);
	key_pressed.row = ROW_2;
	update_debouce_prevention_time(ROW_2);
}

void row3_interruption (void) {
	ignore_debounce(ROW_3);
	key_pressed.row = ROW_3;
	update_debouce_prevention_time(ROW_3);
}

void row4_interruption (void) {
	ignore_debounce(ROW_4);
	key_pressed.row = ROW_4;
	update_debouce_prevention_time(ROW_4);
}

int key_pressed_check (fsm_table* this){
	return ((key_pressed.row == 0) | (key_pressed.row == 1) | (key_pressed.row == 2) | (key_pressed.row == 3));
}

int col_timeout_check (fsm_table* this) {
	return (millis()-iteration_period >= COLS_SWITCH_TIMEOUT_MS);
}

void reset_key(key* key){
	key->col = -1;
	key->row = -1;
}

void move_to_next_col(key* key){
	if(key->col == 3) {
		key->col = -1;
	} else {
		key->col++;
	}
}

void on_key_pressed(fsm_table* this){
	key *key_pressed = this->user_data;

	printf("The col is %d\n", key_pressed->row);
	printf("The row is %d\n", key_pressed->col);

	printf ("The key_pressed pressed is %c\n", matrix_keyboard[key_pressed->row][key_pressed->col]);

	reset_key(key_pressed);
}


void on_col_timeout(fsm_table* this){
	key *key_pressed = this->user_data;

	move_to_next_col(key_pressed);

	int i;
	for(i = 0; i < 4; i++){
		digitalWrite(cols[i], LOW);
	}
	digitalWrite(cols[key_pressed->col], HIGH);

	iteration_period = millis();
}
