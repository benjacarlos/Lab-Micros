/*
 * Cardreader.h
 *
 *  Created on: Oct 12, 2020
 *      Author: Agus
 */

#ifndef CARDREADER_H_
#define CARDREADER_H_

#define MAX_TRACK_LEN 200

typedef struct{

	char track_string[MAX_TRACK_LEN];
	int track_id;
	_Bool isValid; //ver mejor nombre

}readerData_t;


void Cardreader_Init(void);

_Bool lecture_ready(void); //retorna true si el buffer (cola) tiene alguna lectura, sirve para que als capsa superiores no interactuen con HAl

readerData_t get_decoded(void); //devuelve data ya decodificada

#endif /* CARDREADER_H_ */
