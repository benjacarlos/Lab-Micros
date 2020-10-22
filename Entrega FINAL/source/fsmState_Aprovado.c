/*
 * fsmState_Aprovado.c
 *
 *  Created on: Oct 22, 2020
 *      Author: Agus
 */

#include "fsmState_Aprovado.h"
#include "fsmState_Menu.h"
#include "fsmState_PinIn.h"


#include "door.h"
#include "AdminID.h"
#include "displayManager.h"
#include "encoder.h"

#define INCREMENT	1
#define INITIAL	0

typedef enum {GRANT_ACCESS,CHANGE_PIN,ADMIN_OPTION,MENU_OPTIONS}option_name;
static const char * menuStrings[MENU_OPTIONS] = {"OPEN","PIN","ADM"};

state_t AprovadoRoutine_Input(UserData_t * ud)
{
	state_t nextState;
	nextState.name = STAY;
	char category;
	switch(ud->encoderUd.input)
	{
		case UP: // change current option
			if(ud->choice < (MENU_OPTIONS-1))
			{
				ud->choice += INCREMENT;
			}
			else{
				ud->choice = INITIAL;
			}
			// show option to user
			PrintMessage(menuStrings[ud->choice], false);
			break;
		case DOWN: // change current option
			if(ud->choice > INITIAL)
			{
				ud->choice -= INCREMENT;
			}
			else{
				ud->choice = (MENU_OPTIONS-1); //last option
			}
			// show option to user
			PrintMessage(menuStrings[ud->choice], false);
			break;
		case ENTER: // Selects current option
			switch(ud->choice)
			{
				case GRANT_ACCESS:
					userDataReset(true ,true ,true ,true ,ud);
					nextState.name = MENU;

					//estado proximo: menu
					nextState.ev_handlers[INPUT_EV] = &MenuRoutine_Input;
					nextState.ev_handlers[TIMER_EV] = &MenuRoutine_Timer;
					nextState.ev_handlers[KEYCARD_EV] = &MenuRoutine_Card;
					PrintMessage("MENU", false);
					openDoorTemporally();
					break;
				case CHANGE_PIN:
					userDataReset(false ,true ,false ,true ,ud);
					nextState.name = CHANGE_PIN_MODE;

					//proximo estado cambio pin
//					nextState.routines[INPUT_EV] = &CPinputEvHandler;
//					nextState.routines[TIMER_EV] = &CPtimerEvHandler;
//					nextState.routines[KEYCARD_EV] = &CPkeycardEvHandler;
					PrintMessage("ENTER NEW PIN", true);
					break;
				case ADMIN_OPTION:
					category = verificoCategory(ud->received_ID);
					if(category == ADMIN)
					{
						userDataReset(false ,false ,false ,true ,ud);
						nextState.name = MODO_ADMIN;

						//proximo estado: admin
//						nextState.routines[INPUT_EV] = &AMinputEvHandler;
//						nextState.routines[TIMER_EV] = &AMtimerEvHandler;
//						nextState.routines[KEYCARD_EV] = &AMkeycardEvHandler;
						PrintMessage("ADMIN MENU", true);
					}
					else{
						PrintMessage("ACCESS DENIED - USER NOT ADMIN", true);
					}
					break;
			}
			break;
		case CANCEL:
			userDataReset(true ,true ,true ,true ,ud);
			nextState.name = MENU;
			nextState.ev_handlers[INPUT_EV] = &MenuRoutine_Input;
			nextState.ev_handlers[TIMER_EV] = &MenuRoutine_Timer;
			nextState.ev_handlers[KEYCARD_EV] = &MenuRoutine_Card;
			PrintMessage("MENU", false);
			break;
	}
	return nextState;
}
state_t AprovadoRoutine_Timer(UserData_t * ud)
{
	state_t nextState;
	nextState.name = STAY;
	if(ud->timerUd == DISPLAY)
	{
		UpdateDisplay();
	}
	if(ud->timerUd == INACTIVITY)
	{
		userDataReset(true ,true ,false ,true ,ud);
		nextState.name = MENU;
		nextState.ev_handlers[INPUT_EV] = &MenuRoutine_Input;
		nextState.ev_handlers[TIMER_EV] = &MenuRoutine_Timer;
		nextState.ev_handlers[KEYCARD_EV] = &MenuRoutine_Card;
		PrintMessage("MENU", false);
		//resetear timer
	}
	return nextState;
}

state_t AprovadoRoutine_Card(UserData_t * ud)
{
//do
}
