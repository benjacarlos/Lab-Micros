/*
 *	file: door.h
 *  Trabajo Práctico 1 - Interrupciones
 *  GRUPO 5 - Laboratorio de Microprocesadores
 */

#ifndef DOOR_H_
#define DOOR_H_

/******************************************************************************
 *								DEFINICIONES
 ******************************************************************************/

#define TIMEOUT_DOOR 6000 //en ms

/******************************************************************************
 *						DECLARO FUNCIONES DEL HEADER
 ******************************************************************************/

void doorInit(void);
void openDoor(void);
void closeDoor(void);
void toggleDoor(void);
void openDoorTemporally(void);



#endif /* DOOR_H_ */
