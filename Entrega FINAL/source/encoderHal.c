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
#define SIGNAL_PINA PORTNUM2PIN(PD,1)	// PTD3
#define SIGNAL_PINB PORTNUM2PIN(PD,3)	// PTD1
#define SIGNAL_PINC PORTNUM2PIN(PD,2)	// PTD2


#define BUTTON_FREQUENCY 	100 		// 100 milisegundos
#define ROTATION_FREQUENCY 	15			// 10 milisegundos


typedef void (*callback_t)(void);

/*******************************************************************************
 *								VARIABLES ESTATICAS
 *******************************************************************************/

static _Bool inicioEncoder = false;
static uint8_t encoderTimerCount;

/*******************************************************************************
 *								FUNCIONES LOCALES
 *******************************************************************************/

void encoderTimerRoutine(void);	// idea no utilizada

/*******************************************************************************
 *									FUNCIONES
 *******************************************************************************/

void initEncoderHal(void (*funcallback)(void))
{
	if(!inicioEncoder)
	{
		gpioMode(SIGNAL_PINA, INPUT_PULLUP);		// Seteo los puertos D1, D2 y D3 como INPUT_PULLUP
		setPassiveFilter(SIGNAL_PINA);

		gpioMode(SIGNAL_PINB, INPUT_PULLUP);
		setPassiveFilter(SIGNAL_PINB);

		gpioMode(SIGNAL_PINC, INPUT_PULLUP);
		setPassiveFilter(SIGNAL_PINC);



		// inicio el timer para encoder
		encoderTimerCount = 0;
		timerInit();
		timerStart(ROTATION_TIMER, ROTATION_FREQUENCY, funcallback);
		timerStart(BUTTON_TIMER, BUTTON_FREQUENCY, &encoderTimerRoutine);

		// se inicio el Encoder
		inicioEncoder = true;
	}
}

void setButtonCallback(void (*funcallback)(void))
{
	gpioIRQ( SIGNAL_PINC , PORT_eInterruptEither , (pinIrqFun_t) funcallback);
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




