/*
 * 	file: AdminID.c
 *  Trabajo Práctico 1 - Interrupciones
 *  GRUPO 5 - Laboratorio de Microprocesadores
 */

/*******************************************************************************
 * 								HEADERS
 ******************************************************************************/

#include "AdminID.h"
#include <stdbool.h>

/*******************************************************************************
 * 							VARIABLES ESTATICAS
 ******************************************************************************/

static dataBase_t dataBase; // arreglo de usuarios, el maximo se coloca en el header (BASE DE DATOS)


/*******************************************************************************
 * 								FUNCIONES
 ******************************************************************************/


void initDataBase(void)
{
	dataBase.top = -1;

	//aGREGO UN PAR DE USUARIOS DE PRUEBA
	user_t userMaster = {{'6','0','6','1','2','6','8','3'},{'0','0','0','0','0'}, MASTER};//no implementado
	user_t newAdmin = {{'6','0','6','1','2','6','8','4'},{'0','0','0','0','1'}, ADMIN };
	user_t newUser = {{'4','5','1','7','6','6','0','1'},{'0','0','0','1',' '}, BASIC};
	agregoUsuario(userMaster);
	agregoUsuario(newAdmin);
	agregoUsuario(newUser);

}

estado eliminoIDusuario(char usersID[TAMANO_ID])
{
	bool foundID = false;
	int position,j;
	for(position = 0 ; position < (dataBase.top + 1) ; ++position)
	{
		_Bool same = true;
		for(j=0 ; j < TAMANO_ID ; j++)
		{
			if(dataBase.list[position].ID[j] != usersID[j])
			{
				same = false;
			}
		}
		if(same)
		{
			foundID = true;
			break;
		}
	}
	if(foundID)
	{
		user_t topUser = dataBase.list[dataBase.top];
		dataBase.list[position] = topUser;
		dataBase.top -= 1;
		return EXITO;
	}
	else
	{
		return ID_NO_ENCONTRADO;
	}
}


estado agregoUsuario(user_t newUser)
{
	if(dataBase.top == MAX_USERS - 1)
	{
		return DATABASE_LLENO;
	}
	else
	{
		_Bool IDisused = verificoID(newUser.ID);
		if(!IDisused)
		{
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
		bool IDfound = false;
		int position,j;
		for(position = 0 ; position < (dataBase.top + 1) ; ++position)
		 {
			bool same = true;
			for(j = 0 ; position < TAMANO_ID ; ++position){
				if(dataBase.list[position].ID[j] != userToDelete.ID[j])
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

			user_t topUser = dataBase.list[dataBase.top];
			dataBase.list[position] = topUser;
			dataBase.top -= 1;
			return EXITO;
		}
		else
		{
			return ID_NO_ENCONTRADO;
		}
}




//	Función que busca la posicion del ID
_Bool buscoID(char userID[TAMANO_ID])
{
	bool foundID = false;
	int posicion, j;
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
	int i,j;
	for(i=0 ; i< (dataBase.top + 1) ; ++i)
	{
		bool same = true;
		for(j=0 ; j < TAMANO_ID ; ++j)
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
	return dataBase.list[i].categoria;
}



bool cambioCategory(char usersID[TAMANO_ID], category_t nuevaCategory)
{
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

	bool IDfound = false;
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
			IDfound = true;
			break;
		}
	}
	return IDfound;
}




_Bool verificoPIN(char usersID[TAMANO_ID], char usersPIN[PIN_MAXIMO])
{
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







