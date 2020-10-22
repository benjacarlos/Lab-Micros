/*
 * 	file: AdminID.h
 *  Trabajo Práctico 1 - Interrupciones
 *  GRUPO 5 - Laboratorio de Microprocesadores
 */

#ifndef SOURCES_ADMINID_H_
#define SOURCES_ADMINID_H_

/*******************************************************************************
 * 								HEADERS
 ******************************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include "User.h"

/******************************************************************************
 *							  DEFINICIONES
 ******************************************************************************/

#define MAX_USERS 20	// Cantidad máxima usada en el servidor

typedef enum { EXITO , ID_YA_EN_USO , DATABASE_LLENO , ID_NO_ENCONTRADO }estado;

/******************************************************************************
 *							  ESTRUCTURAS
 ******************************************************************************/

typedef struct{
	char ID[TAMANO_ID];		//	string "ID" del usuario, puede ser menor
	char pin[PIN_MAXIMO];	//	string "pin" del usuario
	category_t categoria;	//	Categoria del usuario
}user_t;


typedef struct{
	user_t list[MAX_USERS];
	int top;
}dataBase_t;

/******************************************************************************
 *						DECLARO FUNCIONES DEL HEADER
 ******************************************************************************/

void initDataBase(void);

// funciones para usuarios
estado agregoUsuario(user_t newUser);											// agregar un nuevo usuario
estado eliminoUsuario(user_t userToDelete);										// borrar usuario
estado cambioPIN(char usersID[TAMANO_ID], char usersNewPIN[PIN_MAXIMO]);			// cambio de PIN
//falta remover id??

// funciones para corroborar acciones
_Bool buscoID (char userID[TAMANO_ID]);											// busco el ID que quiero devuelve la posicion de este
category_t verificoCategory(char usersID[TAMANO_ID]);							// verifico la categoria del usuario
bool cambioCategory(char usersID[TAMANO_ID], category_t nuevaCategory);			// cambio la categoria del usuario
bool verificoID(char usersID[TAMANO_ID]);										// verifico el ID del usuario
bool verificoPIN(char usersID[TAMANO_ID], char usersPIN[PIN_MAXIMO]);			// verifico el PIN del usuario





#endif /* SOURCES_ADMINID_H_ */
