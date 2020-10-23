/*
 * fsmState_RemoveUser.c
 *
 *  Created on: Oct 22, 2020
 *      Author: Agus
 */


#include "fsmState_RemoveUser.h"
#include "fsmState_Menu.h"
#include "fsmstate_PinIn.h"
#include "fsmUtils_ID.h"

#include "AdminID.h"
#include "displayManager.h"
#include "encoder.h"


state_t removeUserRoutine_Input(UserData_t * ud)
{
	state_t nextState;
	nextState.name = STAY;
	char* string;
	int j = 0;
	switch(ud->encoderUd.input)
	{
		case UP: // change current option
			if(ud->choice < LAST_OPTION_ID){
				ud->choice += INCREMENT;
			}
			else{
				ud->choice = INITIAL;
			}
			// show option to user
			createIDString(ud);
			string = getstring();
			PrintMessage(string, false);
			break;
		case DOWN: // change current option
			if(ud->choice > INITIAL){
				ud->choice -= INCREMENT;
			}
			else{
				ud->choice = LAST_OPTION_ID;
			}
			// show option to user
			createIDString(ud);
			string = getstring();
			PrintMessage(string, false);
			break;
		case ENTER: // Selects current option
			while(ud->received_ID[j] != '\0'){
				j++;
			}
			switch(ud->choice)
			{
				case ERASE_LAST:
					if(j>INITIAL)
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
					string = getstring();
					PrintMessage(string, false);
					break;
				default: // number
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

						switch(eliminoIDusuario(ud->received_ID))
						{
							case EXITO:
								PrintMessage("USER REMOVED", true);
								nextState.name = MENU;

								nextState.ev_handlers[INPUT_EV] = &MenuRoutine_Input;
								nextState.ev_handlers[TIMER_EV] = &MenuRoutine_Timer;
								nextState.ev_handlers[KEYCARD_EV] = &MenuRoutine_Card;
								break;
							case ID_NO_ENCONTRADO:
								PrintMessage("USER NOT FOUND", true);
								userDataReset(true ,false ,false ,true ,ud);
								break;
						}
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
			break; // Cancels selection and back to menu
	}
	return nextState;
}

state_t removeUserRoutine_Timer(UserData_t * ud)
{
	state_t nextState;
	nextState.name = STAY;
	if(ud->timerUd == DISPLAY)
	{
		UpdateDisplay();
	}
	if(ud->timerUd == INACTIVITY)
	{
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

state_t removeUserRoutine_Card(UserData_t * ud)
{
	state_t nextState;
	nextState.name = STAY;
	char cardID[TAMANO_ID];
	int i;
	for(i=0;i<TAMANO_ID;++i)
	{
		cardID[i] = ud->magnetLectorUd.track_string[i];
	}
	bool IDExists = verificoID(cardID);
	if(IDExists){
		// show message in display
		ud->category = verificoCategory(ud->received_ID);
		PrintMessage("VALID ID - ENTER PIN", true);
		int i;
		for(i=0;i<TAMANO_ID;++i)
		{
			ud->received_ID[i] = cardID[i];
		}
		userDataReset(false, false, false, true, ud);

		nextState.name = PIN_IN;

		nextState.ev_handlers[INPUT_EV] = &PinInRoutine_Input;
		nextState.ev_handlers[TIMER_EV] = &PinInRoutine_Timer;
		nextState.ev_handlers[KEYCARD_EV] = &PinInRoutine_Card;
		PrintMessage("ENTER PIN", false);
	}
	else{
		// show message in display
		PrintMessage("INVALID ID", true);
	}
	return nextState;
}
