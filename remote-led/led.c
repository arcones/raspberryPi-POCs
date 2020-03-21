#include "led.h"

void init_leds(LED leds []){
	LED green = { .pin = GREEN_LED_BCM_PIN,  .color = "green"};
	LED blue = { .pin = BLUE_LED_BCM_PIN,  .color = "blue"};
	LED red = { .pin = RED_LED_BCM_PIN,  .color = "red"};
	LED yellow = { .pin = YELLOW_LED_BCM_PIN,  .color = "yellow"};

	leds [0] = green;
	leds [1] = blue;
	leds [2] = red;
	leds [3] = yellow;

	int i;
	for(i = 0; i < LED_COUNT; i++){
		pinMode(leds[i].pin,OUTPUT);
		digitalWrite(leds[i].pin, LOW);
	}
}

void toggleLED (int pin) {
	if (digitalRead(pin) == 1) {
		digitalWrite(pin, LOW);
	} else {
		digitalWrite(pin, HIGH);
	}
}

void blinkTimes (char key, LED leds []){
	int i;
	for(i = 0; i < LED_COUNT; i++){
		if(digitalRead(leds[i].pin) == HIGH){
			int blinkTimes;
			for(blinkTimes = 2*(key - '0'); blinkTimes > 0; blinkTimes--) {
				toggleLED(leds[i].pin);
				delay(200);
			}
		}
	}
}

void switchOFF(LED leds []) {
	int i;
	for(i = 0; i < LED_COUNT; i++){
		digitalWrite(leds[i].pin, LOW);
	}
}

void switchON(LED leds []) {
	int i;
	for(i = 0; i < LED_COUNT; i++){
		digitalWrite(leds[i].pin, HIGH);
	}
}

void invert(LED leds []) {
	int i;
	for(i = 0; i < LED_COUNT; i++){
		toggleLED(leds[i].pin);
	}
}
