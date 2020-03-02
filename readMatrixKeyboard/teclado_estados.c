/*  Teclado con dos maquinas de estados y con manejo de interrupciones
 *  y antirrebotes de teclas
 *
 *  utilizamos dos maquinas de estados en paralelo
 *	una que explora columnas por medio de un temporizador
 * 	y la otra que detecta la tecla pulsada y la imprime por pantalla
 *
 *  J.M.Pardo 23-2-2017
 */


#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
/*
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>
#include <time.h>
#include <signal.h>
*/
#include "fsm.h"

//
//
#define GPIO_COL_1 0
#define GPIO_COL_2 1
#define GPIO_COL_3 2
#define GPIO_COL_4 3
#define GPIO_ROW_1 5
#define GPIO_ROW_2 6
#define GPIO_ROW_3 12
#define GPIO_ROW_4 13

#define REFRESH_TIME 40 //en ms
#define T_ANTIRREBOTES 4*REFRESH_TIME// rebound son milisengundos
#define CLK_MS 5//en ms, reloj del sistema

//
static int gpio_col[4]={GPIO_COL_1,GPIO_COL_2,GPIO_COL_3,GPIO_COL_4};  // array de columnas
static int gpio_row[4]={GPIO_ROW_1,GPIO_ROW_2,GPIO_ROW_3,GPIO_ROW_4};   // array de filas

static int row[4] = {0,0,0,0};
static int col[4] = {0,0,0,0};
static int timein=0;
static unsigned int columna=0;

char keytec[16] ={'1', '4', '7', 'A', '2', '5', '8', '0', '3', '6', '9', 'B', 'C', 'D', 'E', 'F'};
//

enum keypad_state {KEY_COL };
enum lcd_state {WAIT};


static void boton_isr (int b)
{
 static unsigned antirrebotes[4]={0,0,0,0};
 int now = millis();
 if (now < antirrebotes[b]) return; // si se detecta una repeticion pulsada antes de que pase un tiempo
 																		// se la ignora
 row[b]=1;
 antirrebotes[b] = now + T_ANTIRREBOTES;
}

static void row_1_isr (void) { boton_isr(0); }
static void row_2_isr (void) { boton_isr(1); }
static void row_3_isr (void) { boton_isr(2); }
static void row_4_isr (void) { boton_isr(3); }
static void (* array_row_p[4]) ()= {row_1_isr,row_2_isr,row_3_isr,row_4_isr};

// rutinas que detectan eventos

static int row_p (fsm_t* this){return (row[0]|row[1]|row[2]|row[3]);}
static int timer_finished (fsm_t* this) {return (millis()-timein >= REFRESH_TIME);}



// rutina principal que detecta tecla pulsada y la imprime en pantalla
//
static void write_tecla(fsm_t* this){
int i=0;
int k=0;
	for (i=0; i<4; i++){
		for (k=0; k<4; k++){
			if ((row [i] == 1) && (col [k] == 1)){printf ("%c", keytec[k*4+i]);fflush (stdout);  row[i]=0;}
		}
	}
}


static void col_x(fsm_t* this) {

	columna++;
	digitalWrite(gpio_col[(columna-1)%4], LOW);
	digitalWrite(gpio_col[columna%4 ], HIGH);
	col[(columna-1)%4]=0; col[columna%4]=1;
	timein=millis();
}

static fsm_trans_t keypad[] = {
		{KEY_COL, timer_finished, KEY_COL, col_x },
		{ -1, NULL, -1, NULL }, };


static fsm_trans_t gkm[] = {
		{WAIT, row_p, WAIT, write_tecla},
		{-1, NULL, -1, NULL },
};

// rutinas que usa main
void initialize() {
wiringPiSetupGpio();
int i=0;
for (i=0; i<4; i++){
	pinMode(gpio_col[i], OUTPUT);
	pinMode(gpio_row[i], INPUT);
	pullUpDnControl (gpio_row[i], PUD_DOWN );  // todas las filas inicialmente a pull down
	wiringPiISR(gpio_row[i], INT_EDGE_RISING, array_row_p[i]);  //array de rutinas de interrupción,
	// detectan flanco de subida
	}
}

void delay_hasta (unsigned int next)
{ unsigned int now = millis();
  if (next > now) {delay (next - now);}
}
// fin de rutinas que usa main

int main()
	{
	unsigned int next;
	fsm_t* keypad_fsm = fsm_new(KEY_COL, keypad, NULL);
	fsm_t* lcd_fsm = fsm_new(WAIT, gkm , NULL);
	initialize();

	timein= millis();
	printf ("Pulse teclas \n");
	//
	next = millis();
	while (1) {
	fsm_fire(keypad_fsm);
	fsm_fire(lcd_fsm);
	next += CLK_MS;
	delay_hasta (next);
	}
	return 0;
	}

//



