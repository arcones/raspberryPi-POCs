#include <wiringPi.h>
#include <stdio.h>

#include "led.h"

#define GREEN_LED_BCM_PIN  17
#define BLUE_LED_BCM_PIN   27
#define RED_LED_BCM_PIN    22
#define YELLOW_LED_BCM_PIN 21

LED leds [] = {
		{ .pin = GREEN_LED_BCM_PIN,  .color = "green"},
		{ .pin = BLUE_LED_BCM_PIN,   .color = "blue"},
		{ .pin = RED_LED_BCM_PIN,    .color = "red"},
		{ .pin = YELLOW_LED_BCM_PIN, .color = "yellow"}
};

void cleanUp(LED leds[]) {
	int i;
	for (i = 0; i < 4; i++) {
		pinMode(leds[i].pin, OUTPUT);
		digitalWrite(leds[i].pin, LOW);
	}
}

int main(){
	wiringPiSetupGpio();//wiringPi setup that uses BCM pin numbering so it is portable among raspberryPi different versions

	cleanUp(leds);

	int loops = 16;
	int current_pin = 0;
	int last_pin;
	while(loops != 0){
		current_pin = current_pin % 4;
		last_pin = current_pin == 0 ? 3 : current_pin - 1;
		printf("Switching ON LED %s\n",leds[current_pin].color);
		digitalWrite(leds[current_pin].pin, HIGH);
		printf("Switching OFF LED %s\n", leds[last_pin].color);
		digitalWrite(leds[last_pin].pin, LOW);
		delay(500);
		current_pin++;
		loops--;
	}

	cleanUp(leds);

	printf("Finished!\n");

	return 0;
}


