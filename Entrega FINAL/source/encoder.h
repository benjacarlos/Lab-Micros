/*
 * 	file: encoder.h
 *  Trabajo Práctico 1 - Interrupciones
 *  GRUPO 5 - Laboratorio de Microprocesadores
 */

#ifndef SOURCES_ENCODER_H_
#define SOURCES_ENCODER_H_


/*******************************************************************************
 * 								HEADERS
 ******************************************************************************/

#include "gpio.h"
#include <stdbool.h>

/******************************************************************************
 *							  DEFINICIONES
 ******************************************************************************/


#define LOW 	0
#define HIGH	1

typedef enum { UP , DOWN , ENTER , BACK, CANCEL}encoder_type;	//

/******************************************************************************
 *							  ESTRUCTURAS
 ******************************************************************************/

// valores que toma el evento
typedef struct{
	encoder_type input;
	_Bool isValid;
}encoderUd_t;

// cola de eventos
typedef struct{
	int top; 			//contador de eventos
	encoderUd_t event;
	_Bool isEmpty;
}encoderQueue_t;

/******************************************************************************
 *						DECLARO FUNCIONES DEL HEADER
 ******************************************************************************/

void initEncoder(void);					// inicio el encoder en general
//
encoderUd_t pullEncoderEvent(void);		// saco el ultimo evento de la cola de evento
void pushEncoderEvent(encoderUd_t event);	// meto un evento en la cola de evento
_Bool isEncoderEventValid(void);		//
void rotationCallback(void);			//
void initEncoderQueue(void);			// inicio la cola de evento del encoder

//encoderQueue_t getEncoderQueue(void);  // por ahora no hace falta tomar eventos
void buttonCallback(void);

#endif /* SOURCES_ENCODER_H_ */
