#include "keyboard.h"

static void ignore_debounce(int row_num){
	if (millis() < row_debounce_delay[row_num]) return;
}

static void update_debouce_prevention_time(int row_num) {
	row_debounce_delay[row_num] = millis() + DEBOUNCE_PREVENTION_DELAY_MS;
}

static void row1_interruption (void){
	ignore_debounce(ROW_1);
	key_pressed.row = ROW_1;
	update_debouce_prevention_time(ROW_1);
}

static void row2_interruption (void) {
	ignore_debounce(ROW_2);
	key_pressed.row = ROW_2;
	update_debouce_prevention_time(ROW_2);
}

static void row3_interruption (void) {
	ignore_debounce(ROW_3);
	key_pressed.row = ROW_3;
	update_debouce_prevention_time(ROW_3);
}

static void row4_interruption (void) {
	ignore_debounce(ROW_4);
	key_pressed.row = ROW_4;
	update_debouce_prevention_time(ROW_4);
}

static int key_pressed_check (fsm_table* this){
	return ((key_pressed.row == 0) | (key_pressed.row == 1) | (key_pressed.row == 2) | (key_pressed.row == 3));
}
static int col_timeout_check (fsm_table* this) {
	return (millis()-iteration_period >= COLS_SWITCH_TIMEOUT_MS);
}

static void reset_key(key* key){
	key->col = -1;
	key->row = -1;
}

static void move_to_next_col(key* key){
	if(key->col == 3) {
		key->col = -1;
	} else {
		key->col++;
	}
}

static void on_key_pressed(fsm_table* this){
	key *key_pressed = this->user_data;

	printf("The col is %d\n", key_pressed->row);
	printf("The row is %d\n", key_pressed->col);

	printf ("The key_pressed pressed is %c\n", matrix_keyboard[key_pressed->row][key_pressed->col]);

	reset_key(key_pressed);
}


static void on_col_timeout(fsm_table* this){
	key *key_pressed = this->user_data;

	move_to_next_col(key_pressed);

	int i;
	for(i = 0; i < 4; i++){
		digitalWrite(cols[i], LOW);
	}
	digitalWrite(cols[key_pressed->col], HIGH);

	iteration_period = millis();
}
