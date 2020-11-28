/*
 * fsmUtils_pin.h
 *
 *      Author: Agus
 */

#ifndef FSMUTILS_PIN_H_
#define FSMUTILS_PIN_H_

#include "User.h"

#define PIN_OPTIONS	13
#define LAST_OPTION_PIN	(PIN_OPTIONS - 1)
#define INCREMENT	1
#define INITIAL	0
#define MAX_TRIES 3
#define HIDDEN '-'
#define STRING_CANT	(PIN_MAXIMO + 1)
#define INT2CHAR(x)	((char)(x+48))


//typedef enum {ZERO,ONE,TWO,THREE,FOUR,FIVE,SIX,SEVEN,EIGHT,NINE,BLANK,ERASE_LAST,ERASE_ALL}idOption_name;


void createPINString(UserData_t * ud);

char* getpin(void);

#endif /* FSMUTILS_PIN_H_ */
