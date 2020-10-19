/*
 * encoder.h
 * Created on: 14 oct. 2020
 * GRUPO 5
 */

#ifndef SOURCES_ENCODER_H_
#define SOURCES_ENCODER_H_


/*******************************************************************************
 * INCLUIR HEADER
 ******************************************************************************/

#include "gpio.h"
#include <stdbool.h>

/*******************************************************************************
 * 	DEFICIONES
 ******************************************************************************/


#define LOW 	0
#define HIGH	1

typedef enum {UP, DOWN, BACK, ENTER, CANCEL}enc_type;	// esta no es mi idea, yo preferia usar números

typedef struct{
	enc_type input;
	_Bool isValid;
}encoderUd_t;

typedef struct{
	int top; 			//contador de eventos
	encoderUd_t event;
	_Bool isEmpty;
}encoderQueue_t;

/*******************************************************************************
 * FUNCIONES
 ******************************************************************************/

void InitEncoder(void);
//
encoderUd_t popEncoderEvent(void);
_Bool isEncoderEventValid(void);
void rotationCallback(void);
void initEncoderQueue(void);

//encoderQueue_t getEncoderQueue(void);  // por ahora no hace falta tomar eventos
void buttonCallback(void);

#endif /* SOURCES_ENCODER_H_ */
