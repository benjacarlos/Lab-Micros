/*
 * 	file: display.h
 *  Trabajo Práctico 1 - Interrupciones
 *  GRUPO 5 - Laboratorio de Microprocesadores
 *
 *  Funciones que se comunica con el hardware del display
 *  En este caso display de segmentos
 */

#ifndef DISPLAYSEGMENT_H_
#define DISPLAYSEGMENT_H_

/******************************************************************************
 *						DECLARO FUNCIONES DEL HEADER
 ******************************************************************************/

// Seteo IO del display con el K64
void InitSegmentDisplay(void);
//Imprime el caracter en posicion
void PrintChar(const char c,unsigned int pos);

#endif /* DISPLAYSEGMENT_H_ */
