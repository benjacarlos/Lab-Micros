/*
 * User.h
 *
 *  Created on: Oct 21, 2020
 *      Author: Agus
 */

#ifndef USER_H_
#define USER_H_

#include "encoder.h"
#include "CardReader.h"
#include "timer.h"


#define TAMANO_ID 8 	// Longitud del ID del usuario.
#define PIN_MAXIMO 5	// Longitud maxima del PIN del usuario.
#define PIN_MINIMO 4	// Longitud minima del PIN del usuario

typedef enum { MASTER , ADMIN , BASIC ,  NONE }category_t;

//L siguiente estructura tiene la informacion que hace de unterfaz entre los evento, los drivers y la FSM
typedef struct{
	timerData_t timerUd;
	readerData_t magnetLectorUd;
	encoderUd_t encoderUd;
	char received_ID[TAMANO_ID]; //String con el ID recibido del usuario.
	char received_PIN[PIN_MAXIMO]; //String con el PIN recibido del usuario.
	int option; // Si usuario emitio un pedido
	category_t category;
}UserData_t;



#endif /* USER_H_ */
