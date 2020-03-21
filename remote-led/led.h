#ifndef LED_H_
#define LED_H_

#include <wiringPi.h>

#define GREEN_LED_BCM_PIN  17
#define BLUE_LED_BCM_PIN   27
#define RED_LED_BCM_PIN    22
#define YELLOW_LED_BCM_PIN 21

#define LED_COUNT 4

typedef struct {
	int pin;
	char* color;
} LED;

void init_leds(LED leds []);
void toggleLED (int pin);
void blinkTimes (char key, LED leds []);
void switchOFF(LED leds []);
void switchON(LED leds []);
void invert(LED leds []);

#endif /* LED_H_ */
