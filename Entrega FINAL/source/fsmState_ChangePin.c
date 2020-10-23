/*
 * fsmState_ChangePin.c
 *
 *  Created on: Oct 22, 2020
 *      Author: Agus
 */

#include "fsmState_PinIn.h"
#include "fsmState_Menu.h"
#include "fsmState_ChangePin.h"
#include "fsmUtils_pin.h"

#include "displayManager.h"
#include "encoder.h"
#include "AdminId.h"


state_t pinChangeRoutine_Input(UserData_t * ud)
{
	state_t nextState;
	nextState.name = STAY;
	char* string;
	int j = 0;
	switch(ud->encoderUd.input)
	{
		case UP: // change current option
			if(ud->choice < LAST_OPTION_PIN)
			{
				ud->choice += INCREMENT;
			}
			else{
				ud->choice = INITIAL;
			}
			// show option to user
			createPINString(ud);
			string = getpin();
			PrintMessage(string, false);
			break;
		case DOWN: // change current option
			if(ud->choice > INITIAL)
			{
				ud->choice -= INCREMENT;
			}
			else{
				ud->choice = LAST_OPTION_PIN;
			}
			// show option to user
			createPINString(ud);
			string = getpin();
			PrintMessage(string, false);
			break;
		case ENTER: // Selects current option
			while(ud->received_PIN[j] != '\0')
			{
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
					createPINString(ud);
					string = getpin();
					PrintMessage(string, false);
					break;
				case ERASE_ALL:
					userDataReset(true ,false ,false ,true ,ud);
					createPINString(ud);
					string = getpin();
					PrintMessage(string, false);
					break;
				case BLANK:
					if(j == PIN_MINIMO)
					{
						ud->received_PIN[j] = ' ';

						nextState.name = MENU;
						nextState.ev_handlers[INPUT_EV] = &MenuRoutine_Input;
						nextState.ev_handlers[TIMER_EV] = &MenuRoutine_Timer;
						nextState.ev_handlers[KEYCARD_EV] = &MenuRoutine_Card;
						cambioPIN(ud->received_ID, ud->received_PIN);
						PrintMessage("PIN CHANGED", true);
						userDataReset(true ,true ,true ,true ,ud);
					}
					break;
				default: // number
					if((ud->choice >= INITIAL) && (j < PIN_MAXIMO)){
						ud->received_PIN[j] = INT2CHAR(ud->choice);
						j++;
						userDataReset(false ,false ,false ,true ,ud);
						createPINString(ud);
						string = getpin();
						PrintMessage(string, false);
					}
					if(j == PIN_MAXIMO)
					{ // replace PIN
							nextState.name = MENU;

							nextState.ev_handlers[INPUT_EV] = &MenuRoutine_Input;
							nextState.ev_handlers[TIMER_EV] = &MenuRoutine_Timer;
							nextState.ev_handlers[KEYCARD_EV] = &MenuRoutine_Card;
							cambioPIN(ud->received_ID, ud->received_PIN);
							PrintMessage("PIN CHANGED", true);
							userDataReset(true ,true ,true ,true ,ud);
					}
					break;
			}
			userDataReset(false ,false ,false ,true ,ud);
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



state_t pinChangeRoutine_Timer(UserData_t * ud)
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

state_t pinChangeRoutine_Card(UserData_t * ud)
{
	state_t nextState;
	nextState.name = STAY;
	char cardID[TAMANO_ID];
	int i;
	for(i=0;i < TAMANO_ID;++i)
	{
		cardID[i] = ud->magnetLectorUd.track_string[i];
	}
	bool IDExists = verificoID(cardID);
	if(IDExists)
	{
		// show message in display
		ud->category = verificoCategory(ud->received_ID);
		PrintMessage("VALID ID - ENTER PIN", true);
		int i;
		for(i=0;i < TAMANO_ID;++i)
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

