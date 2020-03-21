#include <stdio.h>

#include "keyboard.h"

#include <wiringPi.h>

void waitTillKeyRelease(const keyboard* keyboard, int j) {
	do {
		delay(25);
	} while (digitalRead(keyboard->rows[j]) == HIGH);
}

void main (void)
{
	wiringPiSetupGpio();//wiringPi setup that uses BCM pin numbering so it is portable among raspberryPi different versions

	keyboard keyboard;

	init_keyboard(&keyboard);

	while(1){

		int i, j;
		for(i = 0; i < COLS_COUNT; i++)
		{
			digitalWrite (keyboard.cols[i],HIGH);
			delay (25);

			for(j = 0; j < ROWS_COUNT; j++)
			{
				if(digitalRead(keyboard.rows[j]) == HIGH)
				{

					waitTillKeyRelease(&keyboard, j);

					printf ("It was pressed the key %c\n", keyboard.physical_keys[j][i]);
				}
			}
			digitalWrite (keyboard.cols[i],LOW);
		}
	}

}
