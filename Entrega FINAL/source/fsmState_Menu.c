/*
 * fsmState_Menu.c
 *
 *      Author: Grupo 5
 */

#include "fsmState_menu.h"
#include "fsmState_IdIn.h"
#include "fsmState_Brightness.h"

#include "displayManager.h"
#include "encoder.h"
#include "User.h"
#include "timer.h"
#include "AdminID.h"

#define INCREMENT	1
#define INITIAL	0

typedef enum {ENTER_ENCODER_ID, BRIGHTNESS, MENU_OPTIONS}option_name;
static const char * menuStrings[MENU_OPTIONS] = {"INPT", "BRGT"};


state_t MenuRoutine_Input(UserData_t * ud)
{
	state_t nextstate;
	nextstate.name = STAY; //por defecto se queda

	switch(ud->encoderUd.input)
		{
			case UP:
				if(ud->choice < (MENU_OPTIONS - 1))
				{
					ud->choice += INCREMENT;
				}
				else{
					ud->choice = INITIAL;
				}

				// muestro en display
				PrintMessage(menuStrings[ud->choice], false);
				break;
			case DOWN:
				if(ud->choice > INITIAL)
				{
					ud->choice -= INCREMENT;
				}
				else{
					ud->choice = MENU_OPTIONS - 1;
				}
				// muestro en display
				PrintMessage(menuStrings[ud->choice], false);
				break;
			case ENTER:
				switch(ud->choice)
				{
					case ENTER_ENCODER_ID:
						userDataReset(false, false, false, true, ud);
						nextstate.name = ID_IN;

						//Configuracion handlers para siguiente estado
						nextstate.ev_handlers[INPUT_EV] = &IDInRoutine_Input;
						nextstate.ev_handlers[TIMER_EV] = &IDInMenuRoutine_Timer;
						nextstate.ev_handlers[KEYCARD_EV] = &IDInMenuRoutine_Card;
						PrintMessage("ENTER ID", true);
						break;
					case BRIGHTNESS:
						userDataReset(false, false, false, true, ud);
						nextstate.name = CHANGE_BRIGTHNESS;

						//Configuracion handlers para siguiente estado
						nextstate.ev_handlers[INPUT_EV] = &brightnessRoutine_Input;
						nextstate.ev_handlers[TIMER_EV] = &brightnessRoutine_Timer;
						nextstate.ev_handlers[KEYCARD_EV] = &brightnessRoutine_Card;


						PrintMessage("SELECT BRIGHTNESS", true);
						break;
				}
				break;
			default:
				break;
		}

	return nextstate;
}



state_t MenuRoutine_Timer(UserData_t * ud)
{
	state_t nextState;
	nextState.name = STAY;
	if(ud->timerUd == DISPLAY)
	{
		UpdateDisplay();
	}
	return nextState;
}

state_t MenuRoutine_Card(UserData_t * ud)
{
	state_t nextState;
	nextState.name = STAY; //por defecto se queda

	char cardID[TAMANO_ID];
	for(int i = 0; i < TAMANO_ID; ++i)
	{
		cardID[i] = ud->magnetLectorUd.track_string[i];
	}

	_Bool IDExists = verificoID(cardID);
	if(IDExists)
	{
		ud->category = verificoCategory(ud->received_ID);

		PrintMessage("VALID ID - ENTER PIN", true);

		for(int i = 0; i < TAMANO_ID; ++i)
		{
			ud->received_ID[i] = cardID[i];
		}

		userDataReset(false, false, false, true, ud);

		nextState.name = ID_IN;

		//Configuracion handlers para siguiente estado
	}
	else
	{
		// muestro en display
		PrintMessage("INVALID ID", true);
	}
	return nextState;
}
