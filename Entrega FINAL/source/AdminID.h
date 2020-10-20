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

#define TAMAÑO_ID 8 	// Longitud del ID del usuario.
#define PIN_MAXIMO 5	// Longitud maxima del PIN del usuario.
#define PIN_MINIMO 4	// Longitud minima del PIN del usuario
#define MAX_USERS 20	// Cantidad máxima usada en el servidor

typedef enum { MASTER , ADMIN , BASIC ,  NONE }category_t;
typedef enum { EXITO , ID_YA_EN_USO , DATABASE_LLENO , ID_NO_ENCONTRADO }estado;


typedef struct{
	// falta la targeta magnetica
	char ID[TAMAÑO_ID];		//	string "ID" del usuario, puede ser menor
	char pin[PIN_MAXIMO];	//	string "pin" del usuario
	category_t categoria;	//	Categoria del usuario
}user_t;


typedef struct{
	user_t list[MAX_USERS];
	int top;
}dataBase_t;


/*******************************************************************
 	 	 	 	 FUNCIONES
 *********************************************************************/

void initDataBase(void);

// funciones para usuarios
estado agregoUsuario(user_t newUser);											// agregar un nuevo usuario
estado eliminoUsuario(user_t userToDelete);										// borrar usuario
void cambioPIN(char usersID[TAMAÑO_ID], char usersNewPIN[PIN_MAXIMO]);			// cambio de PIN
// funciones para corroborar acciones
int buscoID (char userID[TAMAÑO_ID]);											// busco el ID que quiero devuelve la posicion de este
category_t verificoCategory(char usersID[TAMAÑO_ID]);							// verifico la categoria del usuario
bool cambioCatagory(char usersID[TAMAÑO_ID], category_t nuevaCategory);			// cambio la categoria del usuario
bool verificoID(char usersID[TAMAÑO_ID]);										// verifico el ID del usuario
bool verificoPIN(char usersID[TAMAÑO_ID], char usersPIN[PIN_MAXIMO]);			// verifico el PIN del usuario





#endif /* SOURCES_ADMINID_H_ */
