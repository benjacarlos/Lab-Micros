/*
 * 	file: CardReaderHAL.h
 *  Trabajo Práctico 1 - Interrupciones
 *  GRUPO 5 - Laboratorio de Microprocesadores
 */

#ifndef CARDREADERHAL_H_
#define CARDREADERHAL_H_

/*******************************************************************************
 * 								HEADERS
 ******************************************************************************/

#include <stdint.h>

/******************************************************************************
 *							  DEFINICIONES
 ******************************************************************************/

//#define NO_TRACK_DATAID -1
//#define TRACK1_DATAID 1
//#define TRACK2_DATAID 2
//#define TRACK3_DATAID 3


#define DATA_LEN 256
#define MAX_NUM_ITEMS 5

typedef uint8_t CardEncodedData_t;

/******************************************************************************
 *						DECLARO FUNCIONES DEL HEADER
 ******************************************************************************/

/**
 * @brief Inicializa driver de hardware de lector de tarjeta magnetizada
 */
void CardReaderHW_Init(void);

/**
 * @brief devuelve puntero a los datos que tengo guardados de la cola del lector
 */
CardEncodedData_t * get_buffer(void);

_Bool queue_not_empty(void); //devuelve true si NO esta vacia la cola, es decir si vino un evento de lectura

#endif /* CARDREADERHAL_H_ */

