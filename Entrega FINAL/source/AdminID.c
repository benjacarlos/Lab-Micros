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

static dataBase_t dataBase; // arreglo de usuarios, el maximo se coloca en el header


/*******************************************************************************
 	 	 	 	 	 	 	 FUNCIONES
 *******************************************************************************/


void initDataBase(void)
{
	dataBase.top = -1;
	user_t userMaster = {{'6','0','6','1','2','6','8','3'},{'0','0','0','0','0'}, MASTER};
	user_t newAdmin = {{'6','0','6','1','2','6','8','4'},{'0','0','0','0','1'}, ADMIN };
	user_t newUser = {{'4','5','1','7','6','6','0','1'},{'0','0','0','1',' '}, BASIC};
	agregoUsuario(userMaster);
	agregoUsuario(newAdmin);
	agregoUsuario(newUser);

}


estado agregoUsuario(user_t newUser)
{
	if(dataBase.top == MAX_USERS-1)
	{ // user overflow
		return DATABASE_LLENO;
	}
	else
	{
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


estado eliminoUsuario(user_t userToDelete)
{
		// checks if ID is on list
		bool IDfound = false;
		int i,j; //position where ID is, if found
		for(i=0 ; i< (dataBase.top + 1) ; ++i){
			bool same = true;
			for(j=0 ; i < TAMANO_ID ; ++i){
				if(dataBase.list[i].ID[j] != userToDelete.ID[j])
				{
					same = false;
				}
			}
			if(same)
			{
				IDfound = true;
				break;
			}
		}
		if(IDfound)
		{
			// if on list, user is deleted
			user_t topUser = dataBase.list[dataBase.top];
			dataBase.list[i] = topUser; // overwrites user to be removed
			dataBase.top -= 1; // decrements top pointer
			return EXITO;
		}
		else
		{
			return ID_NO_ENCONTRADO;
		}
}



//{
//	// check si id esta en la lista
//	int posicion = buscoID( userToDelete.ID[TAMANO_ID] );
//	if( posicion < MAX_USERS+1 )
//	{
//		// si encuentro el ID lo borro
//		user_t topUser = dataBase.list[dataBase.top];	// guardo el ultimo usuario de la lista
//		dataBase.list[posicion] = topUser; 				// sobreescribo sobre el usuario q quiero eliminar
//		dataBase.top -= 1; 								// decremento el puntero
//		return EXITO;
//	}
//	else
//	{
//		return ID_NO_ENCONTRADO;
//	}
//}


//	Función que busca la posicion del ID
_Bool buscoID(char userID[TAMANO_ID])
{
	bool foundID = false;
	int posicion, j; //busco el ID
	for(posicion=0 ; posicion < (dataBase.top + 1) ; posicion++)
	{
		bool same = true;
		for(j=0 ; j < TAMANO_ID ; j++)
		{
			if(dataBase.list[posicion].ID[j] != userID[j])
			{
				same = false;
				break;
			}
		}
		if(same)
		{
			foundID = true;
			break;
		}
	}
	return foundID;
}

estado cambioPIN(char usersID[TAMANO_ID], char usersNewPIN[PIN_MAXIMO])
{
	// checks if ID is on list
	int i,j;
	for(i=0 ; i< (dataBase.top + 1) ; i++)
	{
		bool same = true;
		for(j=0 ; j<TAMANO_ID ; j++){
			if(dataBase.list[i].ID[j] != usersID[j])
			{
				same = false;
			}
		}
		if(same)
		{
			break;
		}
	}
	for(j=0;j<PIN_MAXIMO;j++)
	{
		dataBase.list[i].pin[j] = usersNewPIN[j];
	}
}


category_t verificoCategory(char usersID[TAMANO_ID])
{
	// checks if ID is on list
	int i,j;
	for(i=0 ; i< (dataBase.top + 1) ; ++i)
	{
		bool same = true;
		for(j=0 ; j < TAMANO_ID ; ++j){
			if(dataBase.list[i].ID[j] != usersID[j])
			{
				same = false;
				break;
			}
		}
		if(same)
		{
			break;
		}
	}
	return dataBase.list[i].categoria;
}

// checks if ID is on list
	//int posicion = buscoID( usersID[TAMANO_ID] );
	//return dataBase.list[posicion].categoria;  // devuelvo la categoria del ID
//}


bool cambioCategory(char usersID[TAMANO_ID], category_t nuevaCategory){
	// busco el ID en la base de datos
	int usuario = buscoID(usersID[TAMANO_ID]);
	category_t categoryUser = verificoCategory( usersID[TAMANO_ID] );
	bool value=false;
	if( (categoryUser != MASTER) | (nuevaCategory == MASTER) ){
		dataBase.list[usuario].categoria = nuevaCategory;
		value = true;
	}
	return value;
}


_Bool verificoID(char usersID[TAMANO_ID])
{
	// checks if ID is on list
	bool IDfound = false;
	int i,j;
	for(i=0 ; i< (dataBase.top + 1) ; i++)
	{
		bool same = true;
		for(j=0 ; j < TAMANO_ID ; j++){
			if(dataBase.list[i].ID[j] != usersID[j])
			{
				same = false;
				break;
			}
		}
		if(same)
		{
			IDfound = true;
			break;
		}
	}
	return IDfound;
}



//{
//// checks if ID is on list
//	bool IDfound = buscoID(usersID[TAMANO_ID]);
////	int posicion = buscoID(usersID[TAMANO_ID]);
////	if (posicion < MAX_USERS + 1)
////	{
////		IDfound = true;
////	}
//	return IDfound;
//}



_Bool verificoPIN(char usersID[TAMANO_ID], char usersPIN[PIN_MAXIMO])
{
	// checks if ID is on list
	int i,j;
	for(i=0 ; i< (dataBase.top + 1) ; i++)
	{
		bool same = true;
		for(j=0 ; j < TAMANO_ID ; j++)
		{
			if(dataBase.list[i].ID[j] != usersID[j])
			{
				same = false;
				break;
			}
		}
		if(same)
		{
			break;
		}
	}

	bool correctPIN = true;
	for(j=0 ; j < PIN_MAXIMO ; j++)
	{
		if(dataBase.list[i].pin[j] != usersPIN[j])
		{
			correctPIN = false;
			break;
		}
	}
	return correctPIN;
}







