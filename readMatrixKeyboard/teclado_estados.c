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
#include "fsm.h"

enum columns_values {
	COLUMNA_1,
	COLUMNA_2,
	COLUMNA_3,
	COLUMNA_4,
};

enum rows_values {
	FILA_1,
	FILA_2,
	FILA_3,
	FILA_4
};

typedef struct {
	int col;
	int row;
} TipoTecla;


//
//
#define GPIO_KEYBOARD_COL_1 0
#define GPIO_KEYBOARD_COL_2 1
#define GPIO_KEYBOARD_COL_3 2
#define GPIO_KEYBOARD_COL_4 3
#define GPIO_KEYBOARD_ROW_1 5
#define GPIO_KEYBOARD_ROW_2 6
#define GPIO_KEYBOARD_ROW_3 12
#define GPIO_KEYBOARD_ROW_4 13

#define NUM_COLUMNAS_TECLADO 	4
#define NUM_FILAS_TECLADO 		4

#define TIMEOUT_COLUMNA_TECLADO 25 //en ms
#define DEBOUNCE_TIME 13*TIMEOUT_COLUMNA_TECLADO// rebound son milisengundos
#define CLK_MS 5//en ms, reloj del sistema

//
static int columnas[NUM_COLUMNAS_TECLADO]={GPIO_KEYBOARD_COL_1,GPIO_KEYBOARD_COL_2,GPIO_KEYBOARD_COL_3,GPIO_KEYBOARD_COL_4};  // array de columnas
static int filas[NUM_FILAS_TECLADO]={GPIO_KEYBOARD_ROW_1,GPIO_KEYBOARD_ROW_2,GPIO_KEYBOARD_ROW_3,GPIO_KEYBOARD_ROW_4};   // array de filas

static int timein=0;
static unsigned debounceTime[NUM_FILAS_TECLADO]={0,0,0,0};
TipoTecla teclaPulsada = {
		.col = -1,
		.row = -1
};
char tecladoTL04[4][4] = {
		{'1', '2', '3', 'A'},
		{'4', '5', '6', 'B'},
		{'7', '8', '9', 'C'},
		{'*', '0', '#', 'D'}
};

enum keypad_state {TECLADO_ESPERA_COLUMNA };
enum lcd_state {TECLADO_ESPERA_TECLA};


static void teclado_fila_1_isr (void) {
	if (millis() < debounceTime[0]) return; // si se detecta una repeticion pulsada antes de que pase un tiempo, se la ignora
	teclaPulsada.row = 0;
	debounceTime[0] = millis() + DEBOUNCE_TIME;
}


static void teclado_fila_2_isr (void) {
	if (millis() < debounceTime[1]) return; // si se detecta una repeticion pulsada antes de que pase un tiempo, se la ignora
	teclaPulsada.row = 1;
	debounceTime[1] = millis() + DEBOUNCE_TIME;
}
static void teclado_fila_3_isr (void) {
	if (millis() < debounceTime[2]) return; // si se detecta una repeticion pulsada antes de que pase un tiempo, se la ignora
	teclaPulsada.row = 2;
	debounceTime[2] = millis() + DEBOUNCE_TIME;
}
static void teclado_fila_4_isr (void) {
	if (millis() < debounceTime[3]) return; // si se detecta una repeticion pulsada antes de que pase un tiempo, se la ignora
	teclaPulsada.row = 3;
	debounceTime[3] = millis() + DEBOUNCE_TIME;
}


static int CompruebaTeclaPulsada (fsm_t* this){
	return ((teclaPulsada.row == 0) | (teclaPulsada.row == 1) | (teclaPulsada.row == 2) | (teclaPulsada.row == 3));
}
static int CompruebaTimeoutColumna (fsm_t* this) {
	return (millis()-timein >= TIMEOUT_COLUMNA_TECLADO);
}



// rutina principal que detecta tecla pulsada y la imprime en pantalla
//
static void ProcesaTeclaPulsada(fsm_t* this){
	TipoTecla *p_teclaPulsada;
	p_teclaPulsada = (TipoTecla*)(this->user_data);

	printf("La fila pulsada es %d\n", teclaPulsada.row);
	printf("La columna pulsada es %d\n", teclaPulsada.col);

	printf ("%c\n", tecladoTL04[p_teclaPulsada->row][p_teclaPulsada->col]);

	p_teclaPulsada->col = -1;
	p_teclaPulsada->row = -1;
}


static void TecladoExcitaColumna(fsm_t* this) {
	TipoTecla *p_teclaPulsada;
	p_teclaPulsada = (TipoTecla*)(this->user_data);

	if(p_teclaPulsada->col == 3) p_teclaPulsada->col = -1;

	p_teclaPulsada->col++;

	int i;
	for(i = 0; i < 4; i++){
		digitalWrite(columnas[i], LOW);
	}
	digitalWrite(columnas[p_teclaPulsada->col], HIGH);


	timein=millis();
}

static fsm_trans_t fsm_trans_excitacion_columnas[] = {
		{TECLADO_ESPERA_COLUMNA, CompruebaTimeoutColumna, TECLADO_ESPERA_COLUMNA, TecladoExcitaColumna },
		{ -1, NULL, -1, NULL }, };


static fsm_trans_t fsm_trans_deteccion_pulsaciones[] = {
		{TECLADO_ESPERA_TECLA, CompruebaTeclaPulsada, TECLADO_ESPERA_TECLA, ProcesaTeclaPulsada},
		{-1, NULL, -1, NULL },
};

// rutinas que usa main
void InicializaTeclado() {
	wiringPiSetupGpio();

	pinMode (columnas[0], OUTPUT);
	pinMode(filas[0], INPUT);
	pullUpDnControl(filas[0], PUD_UP);
	wiringPiISR(filas[0], INT_EDGE_RISING, teclado_fila_1_isr);


	pinMode (columnas[1], OUTPUT);
	pinMode(filas[1], INPUT);
	pullUpDnControl(filas[1], PUD_DOWN);
	wiringPiISR(filas[1], INT_EDGE_RISING, teclado_fila_2_isr);


	pinMode (columnas[2], OUTPUT);
	pinMode(filas[2], INPUT);
	pullUpDnControl(filas[2], PUD_DOWN);
	wiringPiISR(filas[2], INT_EDGE_RISING, teclado_fila_3_isr);


	pinMode (columnas[3], OUTPUT);
	pinMode(filas[3], INPUT);
	pullUpDnControl(filas[3], PUD_DOWN);
	wiringPiISR(filas[3], INT_EDGE_RISING, teclado_fila_4_isr);
}

void delay_hasta (unsigned int next)
{ unsigned int now = millis();
if (next > now) {delay (next - now);}
}
// fin de rutinas que usa main

int main()
{
	TipoTecla teclaPulsada;
	teclaPulsada.col = -1;
	teclaPulsada.row = -1;
	unsigned int next;
	fsm_t* keypad_fsm = fsm_new(TECLADO_ESPERA_COLUMNA, fsm_trans_excitacion_columnas, &teclaPulsada);
	fsm_t* lcd_fsm = fsm_new(TECLADO_ESPERA_TECLA, fsm_trans_deteccion_pulsaciones , &teclaPulsada);
	InicializaTeclado();

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



