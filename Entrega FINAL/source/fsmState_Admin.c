/*
 * fsmState_Admin.c
 *
 *  Created on: Oct 22, 2020
 *      Author: Agus
 */

#include <fsmState_Admin.h>
#include "fsmState_Menu.h"
#include "fsmState_PinIn.h"

#include "fsmState_AddUser.h"
#include "fsmState_RemoveUser.h"

#include "displayManager.h"
#include "encoder.h"
#include "AdminID.h"

#define INCREMENT	1
#define INITIAL	0

typedef enum {ADD_USER,DELETE_USER,MENU_OPTIONS}option_name;

const char * menuStrings[MENU_OPTIONS] = {"ADD","DLT"};

state_t AdminRoutine_Input(UserData_t * ud)
{
	state_t nextState;
	nextState.name = STAY;
	switch(ud->encoderUd.input)
	{
		case UP: // change current option
			if(ud->choice < (MENU_OPTIONS-1)){
				ud->choice += INCREMENT;
			}
			else{
				ud->choice = INITIAL;
			}
			// show option to user
			PrintMessage(menuStrings[ud->choice], false);
			break;
		case DOWN: // change current option
			if(ud->choice > INITIAL){
				ud->choice -= INCREMENT;
			}
			else{
				ud->choice = (MENU_OPTIONS - 1); //last option
			}
			// show option to user
			PrintMessage(menuStrings[ud->choice], false);
			break;
		case ENTER: // Selects current option
			switch(ud->choice)
			{
				case ADD_USER:
					userDataReset(true ,true ,true ,true ,ud);
					nextState.name = ADD_USER_MODE;
					nextState.ev_handlers[INPUT_EV] = &AddUSerRoutine_Input;
					nextState.ev_handlers[TIMER_EV] = &AddUSerRoutine_Timer;
					nextState.ev_handlers[KEYCARD_EV] = &AddUSerRoutine_Card;
					PrintMessage("ENTER USER ID TO ADD", true);
					break;
				case DELETE_USER:
					userDataReset(true ,true ,true ,true ,ud);
					nextState.name = REMOVE_USER_MODE;
					nextState.ev_handlers[INPUT_EV] = &removeUserRoutine_Input;
					nextState.ev_handlers[TIMER_EV] = &removeUserRoutine_Timer;
					nextState.ev_handlers[KEYCARD_EV] = &removeUserRoutine_Card;
					PrintMessage("ENTER USER ID TO DELETE", true);
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

state_t AdminRoutine_Timer(UserData_t * ud)
{
	state_t nextState;
	nextState.name = STAY;
	if(ud->timerUd == DISPLAY){
		UpdateDisplay();
	}
	if(ud->timerUd == INACTIVITY){
		userDataReset(true ,false ,false ,true ,ud);
		nextState.name = MENU;
		nextState.ev_handlers[INPUT_EV] = &MenuRoutine_Input;
		nextState.ev_handlers[TIMER_EV] = &MenuRoutine_Timer;
		nextState.ev_handlers[KEYCARD_EV] = &MenuRoutine_Card;
		PrintMessage("MENU", false);
		//resetear timer
	}
	return nextState;
}

state_t AdminRoutine_Card(UserData_t * ud)
{
	state_t nextState;
	nextState.name = STAY;
	char cardID[TAMANO_ID];
	int i;
	for(i=0;i<TAMANO_ID;++i){
		cardID[i] = ud->magnetLectorUd.track_string[i];
	}
	bool IDExists = verificoID(cardID);
	if(IDExists){
		// show message in display
		ud->category = verificoCategory(ud->received_ID);
		PrintMessage("VALID ID - ENTER PIN", true);
		int i;
		for(i=0;i<TAMANO_ID;++i){
			ud->received_ID[i] = cardID[i];
		}
		userDataReset(false, true, true, true, ud);
		nextState.name = PIN_IN;
		nextState.ev_handlers[INPUT_EV] = &PinInRoutine_Input;
		nextState.ev_handlers[TIMER_EV] = &PinInRoutine_Timer;
		nextState.ev_handlers[KEYCARD_EV] = &PinInRoutine_Card;
		PrintMessage("ENTER PIN", true);
	}
	else{
		// show message in display
		PrintMessage("INVALID ID", true);
	}
	return nextState;
}
