/*
 * 	file: CardReaderDecoder.h
 *  Trabajo Práctico 1 - Interrupciones
 *  GRUPO 5 - Laboratorio de Microprocesadores
 */

#ifndef CARDREADERDECODER_H_
#define CARDREADERDECODER_H_

/*******************************************************************************
 * 								HEADERS
 ******************************************************************************/

#include <stdint.h>

/******************************************************************************
 *							  DEFINICIONES
 ******************************************************************************/

#define BITS_CAPACITY 32

#if BITS_CAPACITY >= 32
#define UINT_T uint32_t
#else
#define UINT_T unit8_t
#endif

#define MAX_TRACK_LEN 200
#define DATA_LEN 256
#define TERMINATOR_DATA_DECODED '\0'

typedef uint8_t CardEncodedData_t;
typedef char CardDecodedData_t;

/******************************************************************************
 *						DECLARO FUNCIONES DEL HEADER
 ******************************************************************************/

_Bool dataParser(CardEncodedData_t * dataIn, CardDecodedData_t * dataOut, int * foundTrack);

#endif /* CARDREADERDECODER_H_ */
