/*
 * displayHardware.c
 *
 *  Created on: 13 Oct 2020
 *      Author: Grupo 6
 */


/**************************************************************************
 * 							HEADERS INCLUIDOS
 **************************************************************************/
#include "displayHardware.h"
#include "gpio.h"
#include <stdbool.h>
/**************************************************************************
 * 						VARIABLES Y DEFINICIONES
 **************************************************************************/

// Display de 7 Segmentos
//
//const unsigned char seven_seg_digits_decode_gfedcba[75]= {
///*  0     1     2     3     4     5     6     7     8     9     :     ;     */
//    0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x00, 0x00,
///*  <     =     >     ?     @     A     B     C     D     E     F     G     */
//    0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x00, 0x39, 0x00, 0x79, 0x71, 0x3D,
///*  H     I     J     K     L     M     N     O     P     Q     R     S     */
//    0x76, 0x30, 0x1E, 0x00, 0x38, 0x00, 0x00, 0x3F, 0x73, 0x00, 0x00, 0x6D,
///*  T     U     V     W     X     Y     Z     [     \     ]     ^     _     */
//    0x00, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
///*  `     a     b     c     d     e     f     g     h     i     j     k     */
//    0x00, 0x5F, 0x7C, 0x58, 0x5E, 0x00, 0x00, 0x00, 0x74, 0x00, 0x00, 0x00,
///*  l     m     n     o     p     q     r     s     t     u     v     w     */
//    0x00, 0x00, 0x54, 0x5C, 0x00, 0x67, 0x50, 0x00, 0x78, 0x1C, 0x00, 0x00,
///*  x     y     z     */
//    0x00, 0x6E, 0x00
//};

static const unsigned char num_array[]=
{		//0		1	 2		3	 4		5	 6		7	 8		9
		0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F
};
static const unsigned char abc_array[]=
{		//A		B	  C		D	  E		F	  G		H	 I		J
		0x77, 0x7F, 0x39, 0x5E, 0x79, 0x71, 0x3D, 0x76, 0x30, 0x1E,
		//K		L	 M		N	 O		P	 Q		R	 S		T
		0x35, 0x38, 0x55, 0x54, 0x3F, 0x73, 0x67, 0x50, 0x2D, 0x78,
		//U		V	 W		X	 Y		Z
		0x3E, 0x2A, 0x6A, 0x14, 0x6E, 0x1B
};

// Flag de Inicializacion
static bool iniHDips = false;

// Cantidad de displays
enum {FIRST_DIGIT,SECOND_DIGIT,THIRD_DIGIT, FOURTH_DIGIT, NUMBER_OF_DIGITS};

// Pines de Salida K64 hacia el display
#define CSEGA PORTNUM2PIN(PC,3)// PTC3
#define CSEGB PORTNUM2PIN(PC,2)// PTC2
#define CSEGC PORTNUM2PIN(PA,2)// PTA2
#define CSEGD PORTNUM2PIN(PB,23)// PTB23
#define CSEGE PORTNUM2PIN(PA,1)// PTA1
#define CSEGF PORTNUM2PIN(PB,9)// PTB9
#define CSEGG PORTNUM2PIN(PC,17)// PTC17
#define CSEGDP PORTNUM2PIN(PC,16)// PTC16
#define SEL1 PORTNUM2PIN(PC,4)// PTC4
#define SEL2 PORTNUM2PIN(PE,24)// PTE24

/**************************************************************************
 * 								SEGMENT MASK
 **************************************************************************/
#define DP_MASK 0x80
#define G_MASK 0x40
#define F_MASK 0x20
#define E_MASK 0x10
#define D_MASK 0x08
#define C_MASK 0x04
#define B_MASK 0x02
#define A_MASK 0x01

/**************************************************************************
 * 								FUNCIONES
 **************************************************************************/
// Funcion para prender los leds de cada segmento correspondiente al valor dado
void setChar(const unsigned char val);

// Funcion de seleccion de display a la cual imprimir
void setDisplay(unsigned int sel_display);

void InitHardwareDisplay(void)
{
	if(!initialized)
		{
			//Inicializo pins de los segmentos
			gpioMode(CSEGA, OUTPUT);
			gpioMode(CSEGB, OUTPUT);
			gpioMode(CSEGC, OUTPUT);
			gpioMode(CSEGD, OUTPUT);
			gpioMode(CSEGE, OUTPUT);
			gpioMode(CSEGF, OUTPUT);
			gpioMode(CSEGG, OUTPUT);
			gpioMode(CSEGDP, OUTPUT);
			//Inicializo pins de la linea de seleccion del display a utilizar
			gpioMode(SEL1, OUTPUT);
			gpioMode(SEL2, OUTPUT);

			initialized = true;
		}
}




