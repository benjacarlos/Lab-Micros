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
#include "encoderEvent.h"
#include "encoderHal.h"


/******************************************************************************
 *									DEFINICIONES
 ******************************************************************************/

#define ENCODER_EVENTS	200
// VALOR/10 es los segundos de las señales
#define BACK_COUNT		20			// entre .5 y 2 segundos para que sea evento = BACK
#define ENTER_COUNT		5


/*******************************************************************************
 * 								VARIABLES
 ******************************************************************************/
_Bool encoderInitialized = falso ;
static encoderQueue_t encoderQueue[ENCODER_EVENTS];


/*******************************************************************************
 * 								FUNCIONES
 ******************************************************************************/


//	Inicio el encoder
void InitEncoder()
{
	if(!encoderInitialized)
	{
		initEncoderHAL(rotationCallback);		//setea gpio y timer count
		setButtonCallback(buttonCallback);
		initEncoderQueue();			//inicializo queue de encoder

		// inicializo 2 encoder_t con las señales en el instante actual y el anterior (A y B)
		int n;
		int k;
		for( n=0 ; n<STATES ; n++ )					// son 2 encoder_t
			for( k=0 ; k<CANT_TOTAL_SEÑALES ; i++ )	// reciben las señales A ,B y C
				updateData(readEncoder(i), i);
		// se inicializo el encoder
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


//	Verifico si la cola de eventos del encoder esta vacía
_Bool isEncoderEventValid(void)
{
	return !encoderQueue->isEmpty;
}



void rotationCallback(void)
{
	updateData(readEncoder(A), A);
	updateData(readEncoder(B), B);
	encoderUd_t eventEncoderQueue;
	eventEncoderQueue.isValid = true;
	counter_type event = decodeEncoder();


	if(event == COUNT_UP)
	{
		eventEncoderQueue.input = UP;
		pushEncoderEvent(eventEncoderQueue);
		resetData();
	}
	else if(event == COUNT_DOWN)
	{
		eventEncoderQueue.input = DOWN;
		pushEncoderEvent(eventEncoderQueue);
		resetData();
	}
	else
	{
		eventEncoderQueue.isValid = false;
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
	updateData(readEncoder(C), C);

	encoderQueue_t eventEncoderQueue;
 	eventEncoderQueue.event.isValid = true;
	if(checkFallingEdge())				//si fue flanco descendente recién se presionó el botón
	{
		resetEncoderTimerCount();			//reseteo el contador
	}
	else if(checkRisingEdge())		//si fue un flanco descendente me fijo cuánto tiempo se presionó el botón para saber si fue ENTER; BACK o CANCEL
	{
		if(getEncTimerCount() <= ENTER_COUNT)		//si es menor a ENTER_COUNT el evento es ENTER
		{
			eventEncoderQueue.event.input = ENTER;
			//resetEncoderTimerCount();				//reseteo el contador
			pushEncoderEvent(eventEncoderQueue.event);
		}
		else if(getEncTimerCount() < BACK_COUNT)	//si es menos de BACK_COUNT o mas de ENTER_COUNT el evento es BACK
		{
			eventEncoderQueue.event.input = BACK;
			//resetEncoderTimerCount();				//reseteo el contador
			pushEncoderEvent(eventEncoderQueue.event);
		}
		else		//si fue más de BACK_COUNT, tomó que fue evento = CANCEL
		{
			eventEncoderQueue.event.input = CANCEL;
			//resetEncoderTimerCount();				//reseteo el contador
			pushEncoderEvent(eventEncoderQueue.event);
		}
	}
}




