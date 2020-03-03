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

static int row[4] = {0,0,0,0};
static int col[4] = {0,0,0,0};//Array q almacena la columna pulsada actual... o eso parece...
static int timein=0;
static unsigned int columna=0;
static unsigned debounceTime[NUM_FILAS_TECLADO]={0,0,0,0};

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
	row[0]=1;//teclado.teclaPulsada.row = index!
	debounceTime[0] = millis() + DEBOUNCE_TIME;
}


static void teclado_fila_2_isr (void) {
	if (millis() < debounceTime[1]) return; // si se detecta una repeticion pulsada antes de que pase un tiempo, se la ignora
	row[1]=1;//teclado.teclaPulsada.row = index! ROW_1
	debounceTime[1] = millis() + DEBOUNCE_TIME;
}
static void teclado_fila_3_isr (void) {
	if (millis() < debounceTime[2]) return; // si se detecta una repeticion pulsada antes de que pase un tiempo, se la ignora
	row[2]=1;//teclado.teclaPulsada.row = index!
	debounceTime[2] = millis() + DEBOUNCE_TIME;
}
static void teclado_fila_4_isr (void) {
	if (millis() < debounceTime[3]) return; // si se detecta una repeticion pulsada antes de que pase un tiempo, se la ignora
	row[3]=1;//teclado.teclaPulsada.row = index!
	debounceTime[3] = millis() + DEBOUNCE_TIME;
}



static void (* rutinas_ISR[NUM_FILAS_TECLADO]) ()= {teclado_fila_1_isr,teclado_fila_2_isr,teclado_fila_3_isr,teclado_fila_4_isr};

// rutinas que detectan eventos

static int CompruebaTeclaPulsada (fsm_t* this){
	return (row[0]|row[1]|row[2]|row[3]);
}
static int CompruebaTimeoutColumna (fsm_t* this) {
	return (millis()-timein >= TIMEOUT_COLUMNA_TECLADO);
}



// rutina principal que detecta tecla pulsada y la imprime en pantalla
//
static void ProcesaTeclaPulsada(fsm_t* this){
	int i=0;
	int k=0;
	for (i=0; i<4; i++){
		for (k=0; k<4; k++){
			if ((row [i] == 1) && (col [k] == 1)){
				printf ("%c", tecladoTL04[i][k]);
				fflush (stdout);
				row[i]=0;
			}
		}
	}
}


static void TecladoExcitaColumna(fsm_t* this) {
	TipoTecla *p_teclaPulsada;
	p_teclaPulsada = (TipoTecla*)(this->user_data);

	columna++;
	digitalWrite(columnas[(columna-1)%4], LOW);
	digitalWrite(columnas[columna%4 ], HIGH);
	col[(columna-1)%4]=0;
	col[columna%4]=1;//teclado.teclaPulsada.col = index!
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
	int i=0;
	for (i=0; i<4; i++){
		pinMode(columnas[i], OUTPUT);
		pinMode(filas[i], INPUT);
		pullUpDnControl (filas[i], PUD_DOWN );  // todas las filas inicialmente a pull down
		wiringPiISR(filas[i], INT_EDGE_RISING, rutinas_ISR[i]);  //array de rutinas de interrupción,
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
	TipoTecla teclaPulsada;
	teclaPulsada.col = 0;
	teclaPulsada.row = 0;
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



