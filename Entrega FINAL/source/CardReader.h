/*
 * 	file: CardReader.h
 *  Trabajo Práctico 1 - Interrupciones
 *  GRUPO 5 - Laboratorio de Microprocesadores
 */

#ifndef CARDREADER_H_
#define CARDREADER_H_

/******************************************************************************
 *							  DEFINICIONES
 ******************************************************************************/

#define MAX_TRACK_LEN 200

/******************************************************************************
 *							  ESTRUCTURAS
 ******************************************************************************/

typedef struct{

	char track_string[MAX_TRACK_LEN];
	int track_id;
	_Bool isValid;

}readerData_t;

/******************************************************************************
 *						DECLARO FUNCIONES DEL HEADER
 ******************************************************************************/

void Cardreader_Init(void);

_Bool lecture_ready(void); //retorna true si el buffer (cola) tiene alguna lectura, sirve para que als capsa superiores no interactuen con HAl

readerData_t get_decoded(void); //devuelve data ya decodificada

#endif /* CARDREADER_H_ */
