/*
 * 	file: encoderEvent.h
 *  Trabajo Práctico 1 - Interrupciones
 *  GRUPO 5 - Laboratorio de Microprocesadores
 */

#ifndef SOURCES_ENCODEREVENT_H_
#define SOURCES_ENCODEREVENT_H_



/*******************************************************************************
 * 								HEADERS
 ******************************************************************************/

#include <stdbool.h>
#include <stdint.h>

/******************************************************************************
 *							  DEFINICIONES
 ******************************************************************************/

#define LOW 0
#define HIGH 1
#define CANT_SIGNALS	3	//
#define STATES 2			// quiero 2 estados para checkear lo q recibe el encoder rotativo

typedef enum {COUNT_UP, COUNT_DOWN, NO_CHANGE, RESET}counter_type; // ERROR se por si hay cambios rápidos de velocidad

/******************************************************************************
 *							  ESTRUCTURAS
 ******************************************************************************/

typedef struct{
	_Bool prev_data[CANT_SIGNALS]; //estados de las señales en el instante anterior del encoder
	_Bool curr_data[CANT_SIGNALS]; //estados de las señales en el instante actual del encoder
}encoder_t;


/******************************************************************************
 *						DECLARO FUNCIONES DEL HEADER
 ******************************************************************************/

void resetData(void);						// seteo tanto las señales de A como de B
void updateData(_Bool value, int signal);	// actualizo los valores
counter_type encoderRot(void);				// me fijo que señal se activa cuando giro el encoder

//Funciones para checkear los flancos
_Bool checkRisingEdge(void);
_Bool checkFallingEdge(void);


#endif /* SOURCES_ENCODEREVENT_H_ */
