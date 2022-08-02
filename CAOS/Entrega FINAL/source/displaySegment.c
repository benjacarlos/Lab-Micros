/*
 * 	file: displaySegment.c
 *  Trabajo Práctico 1 - Interrupciones
 *  GRUPO 5 - Laboratorio de Microprocesadores
 */

/*******************************************************************************
 * 								HEADERS
 ******************************************************************************/

#include "displaySegment.h"
#include "gpio.h"
#include <stdbool.h>

/******************************************************************************
 *						  ARREGLOS Y MATRICES
 ******************************************************************************/

// Display de 7 Segmentos

//http://en.fakoo.de/siekoo.html
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

/*******************************************************************************
 * 							VARIABLES ESTATICAS
 ******************************************************************************/

// Flag de Inicializacion
static bool initialized = false;

// Cantidad de displays
enum {FIRST_DIGIT,SECOND_DIGIT,THIRD_DIGIT, FOURTH_DIGIT, NUMBER_OF_DIGITS};

/******************************************************************************
 *							  DEFINICIONES
 ******************************************************************************/

// Pines de Salida K64 hacia el display
#define CSEGA PORTNUM2PIN(PC,3)// PTC3
#define CSEGB PORTNUM2PIN(PC,2)// PTC2
#define CSEGC PORTNUM2PIN(PA,2)// PTA2
#define CSEGD PORTNUM2PIN(PB,23)// PTB23
#define CSEGE PORTNUM2PIN(PA,1)// PTA1
#define CSEGF PORTNUM2PIN(PB,9)// PTB9
#define CSEGG PORTNUM2PIN(PC,17)// PTC17
#define CSEGDP PORTNUM2PIN(PC,16)// PTC16
#define SEL0 PORTNUM2PIN(PD,0)// PTC4
#define SEL1 PORTNUM2PIN(PE,24)// PTE24

#define HIGH 1
#define LOW 0

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
#define SPACE 0x00
#define SEL0_MASK 0x01
#define SEL1_MASK 0x02

/******************************************************************************
 *						DECLARO FUNCIONES LOCALES
 ******************************************************************************/

// Funcion para prender los leds de cada segmento correspondiente al valor dado
void setDigit(const unsigned char val, unsigned int select_line);
void SelectDigit(unsigned int sel_line);

// Funcion de seleccion de display a la cual imprimir
void setDisplayPos(unsigned int pos);

/*******************************************************************************
 * 								FUNCIONES
 ******************************************************************************/

void InitSegmentDisplay(void)
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
			gpioMode(SEL0, OUTPUT);
			gpioMode(SEL1, OUTPUT);

			initialized = true;
		}
}

void PrintChar(const char c, unsigned int pos)
{
	if( pos< NUMBER_OF_DIGITS )
	{
		//setDisplayPos(pos);
		if( (c >= '0') && (c <= '9') ) //Se desea imprimir un numero
			setDigit(num_array[c-'0'], pos);
		else if( (c >= 'A') && (c <= 'Z') ) //Se desea imprimir una letra
			setDigit(abc_array[c-'A'], pos);
		else if( (c >= 'a') && (c <= 'z') ) //Se desea imprimir una letra
			setDigit(abc_array[c-'a'], pos);
		else //Casos especiales
		{
			if(c == ' ') //Espacio en blanco
				setDigit(SPACE, pos);
			else if(c == '-') //Guion
				setDigit(G_MASK, pos);
		}
	}
}

void setDigit(const unsigned char val, unsigned int select_line)
{
	//Aplico mascara para cada segmento del display
	//Segmento g
	if( val & G_MASK )
	{
		gpioWrite(CSEGG, HIGH);
	}
	else
	{
		gpioWrite(CSEGG, LOW);
	}
	//Segmento f
	if( val & F_MASK )
	{
		gpioWrite(CSEGF, HIGH);
	}
	else
	{
		gpioWrite(CSEGF, LOW);
	}
	//Segmento e
	if( val & E_MASK )
	{
		gpioWrite(CSEGE, HIGH);
	}
	else
	{
		gpioWrite(CSEGE, LOW);
	}
	//Segmento d
	if( val & D_MASK )
	{
		gpioWrite(CSEGD, HIGH);
	}
	else
	{
		gpioWrite(CSEGD, LOW);
	}
	//Segmento c
	if( val & C_MASK )
	{
		gpioWrite(CSEGC, HIGH);
	}
	else
	{
		gpioWrite(CSEGC, LOW);
	}
	//Segmento b
	if( val & B_MASK )
	{
		gpioWrite(CSEGB, HIGH);
	}
	else
	{
		gpioWrite(CSEGB, LOW);
	}
	//Segmento a
	if( val & A_MASK )
	{
		gpioWrite(CSEGA, HIGH);
	}
	else
	{
		gpioWrite(CSEGA, LOW);
	}

	SelectDigit(select_line);
}

void SelectDigit(unsigned int sel_line)
{
	switch(sel_line) //Traduzco el valor de la linea de seleccion
			{					//a los bits correspondientes.
				case FIRST_DIGIT:
					gpioWrite(SEL0, LOW);
					gpioWrite(SEL1, LOW);
					break;
				case SECOND_DIGIT:
					gpioWrite(SEL0, HIGH);
					gpioWrite(SEL1, LOW);
					break;
				case THIRD_DIGIT:
					gpioWrite(SEL0, LOW);
					gpioWrite(SEL1, HIGH);
					break;
				case FOURTH_DIGIT:
					gpioWrite(SEL0, HIGH);
					gpioWrite(SEL1, HIGH);
					break;
				default:
					//Entrada invalida
					break;


			}
}


void setDisplayPos(unsigned int pos)
{
	if(pos < NUMBER_OF_DIGITS)
	{
		gpioWrite(SEL0, (pos & SEL0_MASK));
		gpioWrite(SEL1, (pos & SEL1_MASK)>>1);
	}
}
