/*
 * AdminID.h
 *
 *  Created on: 2 oct. 2020
 *  Author: Grupo 5 Lab. de Microprocesadores
 */

#ifndef SOURCES_ADMINID_H_
#define SOURCES_ADMINID_H_


/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdint.h>
#include <stdbool.h>


/*******************************************************************
		ESTRUCTUTRAS,  ENUMERACIONES  Y  TYPEDEFS
*******************************************************************/

enum{enter, esc, delete};

typedef uint8_t digitCode;
typedef uint64_t ID;
digitCode PIN[5];

typedef struct{
	ID 	ID;
	digitCode	digitPIN[5];
	bool	admin;
	bool	master;

}users;


/*******************************************************************
 	 	 	 	 FUNCIONES
 *********************************************************************/

bool enterCodeID(ID IDregister, users usuario);







#endif /* SOURCES_ADMINID_H_ */
