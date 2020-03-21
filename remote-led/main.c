#include <stdio.h>
#include <wiringPi.h>

#include "keyboard.h"
#include "led.h"

int main (void)
{
	wiringPiSetupGpio();//wiringPi setup that uses BCM pin numbering so it is portable among raspberryPi different versions

	keyboard keyboard;

	init_keyboard(&keyboard);

	LED leds [LED_COUNT];

	init_leds(leds);

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

					switch(keyboard.physical_keys[j][i]) {
					case 'A':
						printf("Toggling LED %s\n", leds[0].color);
						toggleLED(leds[0].pin);
						break;
					case 'B':
						printf("Toggling LED %s\n", leds[1].color);
						toggleLED(leds[1].pin);
						break;
					case 'C':
						printf("Toggling LED %s\n", leds[2].color);
						toggleLED(leds[2].pin);
						break;
					case 'D':
						printf("Toggling LED %s\n", leds[3].color);
						toggleLED(leds[3].pin);
						break;
					case '*':
						printf("Switching OFF all the LEDs\n");
						switchOFF(leds);
						break;
					case '0':
						printf("Inverting OFF all the LEDs\n");
						invert(leds);
						break;
					case '#':
						printf("Switching ON every LED\n");
						switchON(leds);
						break;
					default:
						printf("Blinking %c times every LED switched ON\n");
						blinkTimes(keyboard.physical_keys[j][i], leds);
						break;
					}
				}
			}
			digitalWrite (keyboard.cols[i],LOW);
		}
	}

	return 0;
}
