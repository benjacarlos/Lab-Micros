/*
 *
 *
 *  Created on: 15 oct. 2020
 *  GRUPO 5
 */


#include "encoderEvent.h"

/******************************************************************************
 *								DEFINICIONES
 ******************************************************************************/

typedef enum {A, B, C, CANT_TOTAL_SEÑALES}encoder_signal;


/*******************************************************************************
 * 								VARIABLES
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

counter_type decodeEncoder()
{
	counter_type event = NO_CHANGE;
	if(encoder.curr_data[B] != encoder.prev_data[B])	//flanco descendente de B
	{
		if(encoder.prev_data[A])
			event = COUNT_UP;
	}
	else if (encoder.curr_data[A] != encoder.prev_data[A])	//flanco descendente de A
	{
		if(encoder.prev_data[B])	//si la señal anterior de B estaba en HIGH, fue primer flanco de A
			event = COUNT_DOWN;
	}
	else
		resetData();
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
	return encoder.curr_data[C];			//true si se deja de presionar el botón
}

_Bool checkFallingEdge(void)
{
	return !encoder.curr_data[C];   //true si se presiona el botón (flanco descendente)
}
