/*
 * encoderHal.c
 *
 *
 *  Author: Paulo
 */

#include "encoderHal.h"
#include "timer.h"
#include "gpio.h"


/**************************************************************************
 * 									DEFINICIONES
 **************************************************************************/
#define SIGNAL_PINA PORTNUM2PIN(PD,3)	// PTD3
#define SIGNAL_PINB PORTNUM2PIN(PD,1)	// PTD1
#define SIGNAL_PINC PORTNUM2PIN(PD,2)	// PTD2


#define BUTTON_TIME 	100000 		// 100 milisegundos
#define ROTATION_FREQUENCY 66000	//80 milisegundos


typedef void (*callback_t)(void);

/*******************************************************************************
 *								VARIABLES ESTATICAS
 *******************************************************************************/

static _Bool inicioEncoder = false;
static uint8_t encoderTimerCount;

/*******************************************************************************
 *								FUNCIONES LOCALES
 *******************************************************************************/

// void encoderTimerRoutine(void);	// idea no utilizada

/*******************************************************************************
 *									FUNCIONES
 *******************************************************************************/

void initEncoderHAL(void (*funcallback)(void))
{
	if(!inicioEncoder)
	{
		gpioMode(SIGNAL_PINA, INPUT);		// Seteo los puertos D1, D2 y D3 como INPUT
		gpioMode(SIGNAL_PINB, INPUT);
		gpioMode(SIGNAL_PINC, INPUT);
		//setPassiveFilter(SIGNAL_PINC);

		// mini bloque de timer para encoder
		encoderTimerCount = 0;
		// Inicio el Timer del encoder
		timerInit();
		timerStart(CHECK_ROTATION_TIMER, ROTATION_FREQUENCY, funcallback);
		timerStart(BUTTON_TIMER, BUTTON_TIME, &encTimerRoutine);

		//EnableTimer(ENCODER_TIMER);
		inicioEncoder = true;
	}
}

void setButtonCallback(void (*funcallback)(void))
{
	gpioIRQ(SIGNAL_PINC, GPIO_IRQ_MODE_BOTH_EDGES, (pinIrqFun_t) funcallback);
}

_Bool readEncoder (encoderSignal signal)
{
	_Bool value;
	switch(signal)
	{
		case A:
			value = gpioRead(SIGNAL_PINA);
			break;
		case B:
			value = gpioRead(SIGNAL_PINB);
			break;
		case C:
			value = gpioRead(SIGNAL_PINC);
			break;
		default:
			break;
	}
	return value;
}

void resetEncoderTimerCount(void)
{
	encoderTimerCount = 0;
}

uint8_t getEncoderTimerCount(void)
{
	return encoderTimerCount;
}


// Función Local
void encoderTimerRoutine(void)
{
	if(!gpioRead(SIGNAL_PINC))		//si el botón está presionado aumento el contador
		encoderTimerCount++;
}




