/*
 * fsmUtils.c
 *
 *  Created on: Oct 22, 2020
 *      Author: Agus
 */

#include <fsmUtils_ID.h>



static const char idStrings[ID_OPTIONS] = {'0','1','2','3','4','5','6','7','8','9','L','A'};
static char IDstring[STRING_CANT];


void createIDString(UserData_t * ud)
{
	int i=0;
	while(ud->received_ID[i] != '\0')
	{
		IDstring[i] = ud->received_ID[i];
		i++;
	}
	if(ud->choice != -1)
	{
		IDstring[i] = idStrings[ud->choice];
		i++;
	}
	IDstring[i] = '\0';

}

char* getstring(void)
{
	return IDstring;
}


