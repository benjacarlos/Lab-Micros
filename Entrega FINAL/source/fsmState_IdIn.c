/*
 * fsmState_IdIn.c
 *
 *  Created on: Oct 21, 2020
 *      Author: Agus
 */


#include <fsmUtils_ID.h>
#include "AdminId.h"
#include "encoder.h"
#include "displayManager.h"

#include "fsmState_Menu.h"
#include "fsmState_IdIn.h"
#include "fsmState_PinIn.h"


state_t IDInRoutine_Input(UserData_t * ud)
{
	state_t nextstate;
	nextstate.name = STAY;
	char* string;
	int j = 0;
	_Bool validID = false;

	switch(ud->encoderUd.input)
	{
		case UP:
			if(ud->choice < LAST_OPTION_ID)
			{
				ud->choice += INCREMENT;
			}
			else
			{
				ud->choice = INITIAL;
			}

			createIDString(ud);
			string = getstring();
			PrintMessage(string, false);
			break;
		case DOWN:
			if(ud->choice > INITIAL)
			{
				ud->choice -= INCREMENT;
			}
			else
			{
				ud->choice = LAST_OPTION_ID;
			}

			createIDString(ud);
			string = getstring();
			PrintMessage(string, false);
			break;
		case ENTER:
			while(ud->received_ID[j] != '\0')
			{
				j++;
			}
			switch(ud->choice)
			{
				case ERASE_LAST:
					if(j > INITIAL)
					{
						ud->received_ID[j-1] = '\0';
					}
					userDataReset(false ,false ,false ,true ,ud);
					createIDString(ud);
					string = getstring();
					PrintMessage(string, false);
					break;
				case ERASE_ALL:
					userDataReset(true ,false ,false ,true ,ud);
					createIDString(ud);
					PrintMessage(string, false);
					break;
				default:
					if((ud->choice >= INITIAL) && (j < TAMANO_ID))
					{
						ud->received_ID[j] = INT2CHAR(ud->choice);
						j++;
						userDataReset(false ,false ,false ,true ,ud);
						createIDString(ud);
						string = getstring();
						PrintMessage(string, false);
					}
					if(j == TAMANO_ID)
					{
						validID = verificoID(ud->received_ID);
						if(validID)
						{
							nextstate.name = PIN_IN;
							ud->category = verificoCategory(ud->received_ID);

							//Configuracion rutinas proximo estado

							nextstate.ev_handlers[INPUT_EV] = &PinInRoutine_Input;
							nextstate.ev_handlers[TIMER_EV] = &PinInRoutine_Timer;
							nextstate.ev_handlers[KEYCARD_EV] = &PinInRoutine_Card;

							PrintMessage("VALID ID - ENTER PIN", true);
						}
						else
						{
							PrintMessage("INVALID ID", true);
							userDataReset(true ,true ,true ,true ,ud);
						}
					}
					break;
			}

			break;
		case CANCEL:
			userDataReset(true ,true ,true ,true ,ud);
			nextstate.name = MENU;

			//Configuracion rutinas proximo estado
			nextstate.ev_handlers[INPUT_EV] = &MenuRoutine_Input;
			nextstate.ev_handlers[TIMER_EV] = &MenuRoutine_Timer;
			nextstate.ev_handlers[KEYCARD_EV] = &MenuRoutine_Card;
			PrintMessage("MENU", false);
			break;
	}
	return nextstate;
}



state_t IDInMenuRoutine_Timer(UserData_t * ud)
{
	state_t nextstate;
	nextstate.name = STAY;

	if(ud->timerUd == DISPLAY)
	{
		UpdateDisplay();
	}

	if(ud->timerUd == INACTIVITY)
	{
		userDataReset(true ,false ,false ,true ,ud);
		nextstate.name = MENU;

		//Configuracion rutinas proximo estado (MENU)
		nextstate.ev_handlers[INPUT_EV] = &MenuRoutine_Input;
		nextstate.ev_handlers[TIMER_EV] = &MenuRoutine_Timer;
		nextstate.ev_handlers[KEYCARD_EV] = &MenuRoutine_Card;
		PrintMessage("MENU", false);
		//resetear timer
	}
	return nextstate;
}


state_t IDInMenuRoutine_Card(UserData_t * ud)
{
	state_t nextstate;
	nextstate.name = STAY;

	char cardID[TAMANO_ID];

	for(int i = 0; i < TAMANO_ID; ++i)
	{
		cardID[i] = ud->magnetLectorUd.track_string[i];
	}

	_Bool IDExists = verificoID(cardID);
	if(IDExists)
	{
		// show message in display
		ud->category = verificoCategory(ud->received_ID);
		PrintMessage("VALID ID - ENTER PIN", true);
		for(int i = 0; i < TAMANO_ID; ++i){
			ud->received_ID[i] = cardID[i];
		}
		userDataReset(false, false, false, true, ud);

		nextstate.name = PIN_IN;

		//Configuracion rutinas proximo estado (PIN_IN)
	}
	else{
		// show message in display
		PrintMessage("INVALID ID", true);
	}
	return nextstate;
}



