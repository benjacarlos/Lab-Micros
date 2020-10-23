/*
 * fsmUtils.h
 *
 *  Created on: Oct 22, 2020
 *      Author: Agus
 */

#ifndef FSMUTILS_ID_H_
#define FSMUTILS_ID_H_

#include "User.h"

#define ID_OPTIONS	12
#define LAST_OPTION_ID	(ID_OPTIONS - 1)
#define INCREMENT	1
#define INITIAL	0
#define STRING_CANT	(TAMANO_ID + 1)
#define INT2CHAR(x)	((char)(x + 48))

typedef enum {ZERO,ONE,TWO,THREE,FOUR,FIVE,SIX,SEVEN,EIGHT,NINE,ERASE_LAST,ERASE_ALL}idOption_name;

void createIDString(UserData_t * ud);
char* getstring(void);

#endif /* FSMUTILS_ID_H_ */
