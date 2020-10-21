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

#define ENCODER_EVENTS	100
// VALOR/1000 es los segundos de las señales
#define BACK_COUNT		200			// entre .5 y 2 segundos para que sea evento = BACK
#define ENTER_COUNT		5


/*******************************************************************************
 * 								VARIABLES
 ******************************************************************************/
_Bool encoderInitialized = false;
static encoderQueue_t encoderQueue[ENCODER_EVENTS];


/*******************************************************************************
 * 								FUNCIONES
 ******************************************************************************/


//	Inicio el encoder
void initEncoder()
{
	if(!encoderInitialized)
	{
		initEncoderHal(rotationCallback);		//setea gpio y timer count
		setButtonCallback(buttonCallback);
		initEncoderQueue();			//inicializo queue de encoder

		// inicializo 2 encoder_t con las señales en el instante actual y el anterior (A y B)
		int n;
		int k;
		for( n=0 ; n<STATES ; n++ )					// son 2 encoder_t
			for( k=0 ; k<ENC_SIGNAL_COUNT ; k++ )	// reciben las señales A ,B y C
				updateData(readEncoder(k), k);
		// se inicializo el encoder
		encoderInitialized = true;
	}
}




encoderUd_t pullEncoderEvent(void)
{
	encoderUd_t popEvent;
	if(encoderQueue->top == -1)
	{
		encoderQueue->isEmpty = true;
		popEvent.isValid = false;
	}
	else
	{
		popEvent = encoderQueue[encoderQueue->top].event; //popEvent
		popEvent.isValid = true;
		encoderQueue->top -= 1; // Decrement queue counter
		if(encoderQueue->top == -1)
		{
			encoderQueue->isEmpty = true;
		}
	}
	return popEvent;
}

void pushEncoderEvent(encoderUd_t newEvent){
	if(encoderQueue->top == ENCODER_EVENTS-1)
	{ // event overflow
		encoderQueue->top = 0;
		encoderQueue[encoderQueue->top].event = newEvent;
		encoderQueue->isEmpty = false;
	}
	else{
		encoderQueue->top += 1;
		encoderQueue[encoderQueue->top].event = newEvent;
		encoderQueue->isEmpty = false;
	}
	return;
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
	counter_type event = encoderRot();


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
	if(checkFallingEdge())							//si fue flanco descendente -->> se presionó el botón
	{
		resetEncoderTimerCount();					//reseteo el contador
	}
	else if(checkRisingEdge())						//si fue un flanco ascendente me fijo cuánto tiempo se presionó el botón para saber si fue ENTER o BACK
	{
		if(getEncoderTimerCount() <= ENTER_COUNT)		//si es menor a ENTER_COUNT el evento es ENTER
		{
			eventEncoderQueue.event.input = ENTER;
			eventEncoderQueue.event.isValid = true;
			pushEncoderEvent(eventEncoderQueue.event);
		}
		else 										//si es menos de BACK_COUNT o mas de ENTER_COUNT el evento es BACK
		{
			eventEncoderQueue.event.input = BACK;
			eventEncoderQueue.event.isValid = true;
			pushEncoderEvent(eventEncoderQueue.event);
		}
	}
}




