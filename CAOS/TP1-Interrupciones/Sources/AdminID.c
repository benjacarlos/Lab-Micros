/*
 * AdminID.c
 *
 *  Created on: 2 oct. 2020
 *  Author: Grupo 5 Lab. de Microprocesadores
 */


/*******************************************************************************
 * 						INCLUDE HEADER FILES
 ******************************************************************************/

#include "AdminID.h"
#include <stdbool.h>

/*******************************************************************************
 * 						VARIABLES LOCALES
 ******************************************************************************/

static dataBase_t dataBase; // arreglo de usuario, el maximo se coloca en el header


/*******************************************************************************
 	 	 	 	 	 	 	 FUNCIONES
 *******************************************************************************/


void initDataBase(void)
{
	dataBase.top = -1;
	user_t userMaster = {{'6','0','6','1','2','6','8','3'},{'0','0','0','0','0'}, MASTER};
	user_t newAdmin = {{'6','0','6','1','2','6','8','4'},{'0','0','0','0','1'}, ADMIN };
	user_t newUser = {{'4','5','1','7','6','6','0','1'},{'0','0','0','1',' '}, BASIC};
	addUser(userMaster);
	addUser(newAdmin);
	addUser(newUser);

}


estado agregoUsuario(user_t newUser){
	// me fijo si no hay ovweflow
	if(dataBase.top == MAX_CANT_USERS-1){
			return DATABASE_FULL;
		}
	else{
		// checks if ID already on list
		bool IDused = verificoID(newUser.ID);
		if(!IDused)
		{
			// if not on list, user is added
			dataBase.top += 1;
			dataBase.list[dataBase.top] = newUser;
			return EXITO;
		}
		else
		{
			return ID_YA_EN_USO;
		}
	}
}


estado eliminoUsuario(user_t userToDelete){
	// checks if ID is on list
	posicion = buscoID(userToDelete.ID)
	if(posicion < (MAX_USERS+1) )
	{
		// si encuentro el ID lo borro
		user_t topUser = dataBase.list[dataBase.top];	// guardo el ultimo usuario de la lista
		dataBase.list[posicion] = topUser; 				// sobreescribo sobre el usuario q quiero eliminar
		dataBase.top -= 1; 								// decremento el puntero
		return EXITO;
	}
	else
	{
		return ID_YA_EN_USO;
	}
}


//	Intento de función que busca la posicion del ID
int buscoID (char userID[TAMAÑO_ID]){
	bool IDfound = false;
	int posicion,j; //busco el ID
	for(posicion=0 ; posicion< (dataBase.top + 1) ; ++posicion){
		bool same = true;
		for(j=0 ; i<TAMAÑO_ID ; ++i){
			if(dataBase.list[posicion].ID[j] != userToDelete.ID[j]){
				same = false;	// mientras sea falso sigo en el for
			}
		}
		if(same){
			IDfound = true;
			break;
		}
	if (IFfound){
		return posicion;
	}
	else{
		return (MAX_USERS+1);
	}

}

void cambioPIN(char usersID[TAMAÑO_ID], char usersNewPIN[PIN_MAXIMO]){
	// busco el ID en la base de datos
	int posicion = buscoID(usersID[TAMAÑO_ID]);
	int j;
	for(j=0;j<PIN_MAXIMO;j++){
		dataBase.list[posicion].PIN[j] = usersNewPIN[j]; //reemplazo el PIN
	}
}


category_t verificoCategory(char usersID[TAMAÑO_ID]){
// checks if ID is on list
	int posicion = buscoID(char usersID[TAMAÑO_ID]);
	return dataBase.list[posicion].category;  // devuelvo la categoria del ID
}


bool cambioCatagory(char usersID[TAMAÑO_ID], category_t nuevaCategory){
	// busco el ID en la base de datos
	int usuario = buscoID(usersID[TAMAÑO_ID]);
	category_t categoryUser = verificoCategory( usersID[TAMAÑO_ID] );
	bool value=false;
	if(categoryUser != MASTER){
		dataBase.list[usuario].category = nuevaCategory;
		value = true;
	}

	return value;
}


bool verificoID(char usersID[TAMAÑO_ID]){
// checks if ID is on list
	bool IDfound = false;
	int posicion = buscoId(usersID[TAMAÑO_ID]);
	if (posicion < MAX_USERS+1){
		IDfound = true;
	}
	return IDfound;
}



bool verificoPIN(char usersID[TAMAÑO_ID], char usersPIN[PIN_MAXIMO]){
	// Busco el ID en la base de datos
	int posicion = buscoID(usersID[TAMAÑO_ID]);
	bool correctPIN = true;
	int j;
		for(j=0 ; j<PIN_MAX_LENGTH ; j++){
			if(dataBase.userList[posicion].usersPIN[j] != usersPIN[j]){  // verifico el PIN
				correctPIN = false;
				break;
			}
		}
	return correctPIN;
}






