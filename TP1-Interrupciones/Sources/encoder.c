/*
 * encoder.c
 *
 *  Created on: 14 oct. 2020
 *  GRUPO 5
 */

/*******************************************************************************
 * INCLUIR HEADER
 ******************************************************************************/
#include "encoder.h"
#include "encoderHal.h"
#include "encoToDeco.h"


/******************************************************************************
 *									DEFINICIONES
 ******************************************************************************/

#define ENCODER_EVENTS	200
// VALOR/10 es los segundos de las señales
#define BACK_COUNT		20			// entre .5 y 2 segundos para que sea evento = BACK
#define ENTER_COUNT		5


/*******************************************************************************
 * VARIABLES
 ******************************************************************************/
_Bool initialized_enc = falso ;
static encoderQueue_t encoderQueue[ENCODER_EVENTS];



/*******************************************************************************
 * 				FUNCIONES DECLARADAS
 ******************************************************************************/

void InitEncoder(void);
encoderUd_t popEncoderEvent(void);
_Bool isEncEventValid(void);
void rotationCallback(void);
void initEncoderQueue(void);


/*******************************************************************************
 * 								FUNCIONES
 ******************************************************************************/

void InitEncoder()
{
	if(!initialized_enc)
	{
		//Pines que usara el ENCODER ROT
		gpioMode(STATUS0, INPUT);


		initialized = true;
	}
}


encoderUd_t popEncoderEvent(void)
{
	encoderUd_t poppedEvent;
	if(encoderQueue->top == -1)
	{
		encoderQueue->isEmpty = true;
		poppedEvent.isValid = false;
	}
	else
	{
		poppedEvent = encoderQueue[encoderQueue->top].event; //popEvent
		poppedEvent.isValid = true;
		encoderQueue->top -= 1; // Decrement queue counter
		if(encoderQueue->top == -1)
		{
			encoderQueue->isEmpty = true;
		}
	}
	return poppedEvent;
}


_Bool isEncEventValid(void)
{
	return !encoderQueue->isEmpty;
}


void rotationCallback(void)
{
	updateData(readEncoder(A), A);
	updateData(readEncoder(B), B);
	encoderUd_t eventForQueue;
	eventForQueue.isValid = true;
	counter_type event = decodeEncoder();


	if(event == COUNT_UP)
	{
		eventForQueue.input = UP;
		pushEncoderEvent(eventForQueue);
		resetData();
	}
	else if(event == COUNT_DOWN)
	{
		eventForQueue.input = DOWN;
		pushEncoderEvent(eventForQueue);
		resetData();
	}
	else
	{
		eventForQueue.isValid = false;
		resetData();
		//resetEdgeFlag();

	}

}


void initEncoderQueue(void)
{
	encoderQueue->top = -1;
	encoderQueue->isEmpty = true;
}



void buttonCallback(void)
{
	updateData(readEncoderSignalX(C), C);

	encoderQueue_t eventForQueue;
 	eventForQueue.event.isValid = true;
	if(checkEnterFallingEdge())				//si fue flanco descendente recién se presionó el botón
	{
		resetEncoderTimerCount();			//reseteo el contador
	}
	else if(checkEnterRisingEdge())		//si fue un flanco descendente me fijo cuánto tiempo se presionó el botón para saber si fue ENTER; BACK o CANCEL
	{
		if(getEncTimerCount() <= ENTER_COUNT)		//si es menor a ENTER_COUNT el evento es ENTER
		{
			eventForQueue.event.input = ENTER;
			//resetEncoderTimerCount();				//reseteo el contador
			pushEncoderEvent(eventForQueue.event);
		}
		else if(getEncTimerCount() < BACK_COUNT)	//si es menos de BACK_COUNT o mas de ENTER_COUNT el evento es BACK
		{
			eventForQueue.event.input = BACK;
			//resetEncoderTimerCount();				//reseteo el contador
			pushEncoderEvent(eventForQueue.event);
		}
		else		//si fue más de BACK_COUNT, tomó que fue evento = CANCEL
		{
			eventForQueue.event.input = CANCEL;
			//resetEncoderTimerCount();				//reseteo el contador
			pushEncoderEvent(eventForQueue.event);
		}
	}
}




