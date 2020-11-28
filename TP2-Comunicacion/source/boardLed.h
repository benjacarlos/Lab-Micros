/*
 * 	file: boardLed.h
 *  Trabajo Práctico 1 - Interrupciones
 *  GRUPO 5 - Laboratorio de Microprocesadores
 */

#ifndef BOARDLED_H_
#define BOARDLED_H_

/*******************************************************************************
 * 								HEADERS
 ******************************************************************************/

#include <stdbool.h>

/******************************************************************************
 *							  DEFINICIONES
 ******************************************************************************/

enum {BR_LED_RED, BR_LED_GREEN, BR_LED_BLUE, BR_TOTAL_LED};

/******************************************************************************
 *						DECLARO FUNCIONES DEL HEADER
 ******************************************************************************/

void ledBoardInit();
void setLedBoard(int led_color, bool on_off);

#endif /* BOARDLED_H_ */
