/*
 * fsmUtils_pin.c
 *
 *  Created on: Oct 22, 2020
 *      Author: Agus
 */

#include "fsmUtils_pin.h"

static const char pinStrings[PIN_OPTIONS] = {'0','1','2','3','4','5','6','7','8','9',' ','L','A'};
static char PINstring[STRING_CANT];


void createPINString(UserData_t * ud)
{
	int i=0;
	while(ud->received_PIN[i] != '\0')
	{
		PINstring[i] = HIDDEN;
		i++;
	}
	if(ud->choice != -1)
	{
		PINstring[i] = pinStrings[ud->choice];
		i++;
	}
	PINstring[i] = '\0';
}

char* getpin(void)
{
	return PINstring;
}
