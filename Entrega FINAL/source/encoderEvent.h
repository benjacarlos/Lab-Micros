/*
 * encoToDeco.h
 *
 *  Created on: 15 oct. 2020
 *      Author: Paulo
 */

#ifndef SOURCES_ENCODEREVENT_H_
#define SOURCES_ENCODEREVENT_H_



/*******************************************************************************
 * 						INCLUIR HEADER
 ******************************************************************************/

#include <stdbool.h>
#include <stdint.h>

/*******************************************************************************
 * 						DEFICIONES
 ******************************************************************************/

#define LOW 0
#define HIGH 1
#define CANT_SEÑALES	3	//
#define STATES 2			// quiero 2 estados para checkear lo q recibe el encoder rotativo

typedef enum {COUNT_UP, COUNT_DOWN, NO_CHANGE, ERROR}counter_type; // ERROR se por si hay cambios rápidos de velocidad

typedef struct{
	_Bool prev_data[CANT_SEÑALES]; //estados de las señales en el instante anterior del encoder
	_Bool curr_data[CANT_SEÑALES]; //estados de las señales en el instante actual del encoder
}encoder_t;


/*******************************************************************************
 * 						FUNCIONES
 ******************************************************************************/

void resetData(void);						// seteo tanto las señales de A como de B
void updateData(_Bool value, int signal);	// actualizo los valores
counter_type encoderRot(void);				// me fijo que señal se activa cuando giro el encoder

//Funciones para checkear los flancos
_Bool checkRisingEdge(void);
_Bool checkFallingEdge(void);


#endif /* SOURCES_ENCODEREVENT_H_ */
