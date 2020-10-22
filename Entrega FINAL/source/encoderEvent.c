/*
 * 	file: encoderEvent.c
 *  Trabajo Práctico 1 - Interrupciones
 *  GRUPO 5 - Laboratorio de Microprocesadores
 */

/*******************************************************************************
 * 								HEADERS
 ******************************************************************************/

#include "encoderEvent.h"

/******************************************************************************
 *							  DEFINICIONES
 ******************************************************************************/

typedef enum {A, B, C, CANT_TOTAL_SIGNALS}encoder_signal;


/*******************************************************************************
 * 							VARIABLES ESTATICAS
 ******************************************************************************/

static encoder_t encoder;


/*******************************************************************************
 * 								FUNCIONES
 ******************************************************************************/


void updateData(_Bool value, int signal)
{
	encoder.prev_data[signal] = encoder.curr_data[signal];
	encoder.curr_data[signal] = value;
}

counter_type encoderRot()
{
	counter_type event = NO_CHANGE;
	if((encoder.curr_data[B] == LOW) && (encoder.prev_data[B] == HIGH))	//flanco descendente de B
	{
		if(encoder.prev_data[A])
		{
			event = COUNT_UP;
		}
	}
	else if ((encoder.curr_data[B] == HIGH) && (encoder.prev_data[B] == LOW))	//flanco descendente de A
	{
		event = RESET;
	}

	if((encoder.prev_data[A] == HIGH) && (encoder.curr_data[A] == LOW))
	{
		if(encoder.prev_data[B])	//si la señal anterior de B estaba en HIGH, fue primer flanco de A
			event = COUNT_DOWN;
	}
	else if((encoder.prev_data[A] == LOW) && (encoder.curr_data[A] == HIGH))
	{
		event = RESET;
	}

	return event;
}


void resetData(void)
{
	uint8_t i;
	for(i = 0; i<STATES; i++)
	{
		updateData(HIGH, A);
		updateData(HIGH, B);
	}
}

_Bool checkRisingEdge(void)
{
	bool value = false;
	if((encoder.prev_data[C] == LOW) && (encoder.curr_data[C] == HIGH)){	// true si hay flanco ASCENDENTE
		value = true;
	}
	return value;
}

_Bool checkFallingEdge(void)
{
	bool value = false;
	if((encoder.prev_data[C] == HIGH) && (encoder.curr_data[C] == LOW)){	// true si hay flanco DESCENDENTE
		value = true;
	}
	return value;
}
