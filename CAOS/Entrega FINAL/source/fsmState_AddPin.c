/*
 * fsmState_AddPin.c
 *
 *      Author: Agus
 */


#include "fsmState_AddPin.h"
#include "fsmState_Menu.h"
#include "fsmState_PinIn.h"
#include "fsmUtils_pin.h"

#include "AdminID.h"
#include "displayManager.h"
#include "encoder.h"

typedef enum {ZERO,ONE,TWO,THREE,FOUR,FIVE,SIX,SEVEN,EIGHT,NINE,BLANK,ERASE_LAST,ERASE_ALL}idOption_name;

state_t AddPinRoutine_Input(UserData_t * ud)
{
	state_t nextState;
	nextState.name = STAY; //por defecto me quedo

	char* string;
	int j = 0;
	int k = 0;

	//se ve que tipo de dato de input hay
	switch(ud->encoderUd.input)
	{
		case UP:
			if(ud->choice < LAST_OPTION_PIN)
			{
				ud->choice += INCREMENT;
			}
			else
			{
				ud->choice = INITIAL;
			}

			createPINString(ud);
			string = getpin();
			PrintMessage(string, false);
			break;
		case DOWN:
			if(ud->choice > INITIAL)
			{
				ud->choice -= INCREMENT;
			}
			else
			{
				ud->choice = LAST_OPTION_PIN;
			}

			createPINString(ud);
			string = getpin();
			PrintMessage(string, false);
			break;
		case ENTER:
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
						user_t newUser;
						for(k = 0;k<TAMANO_ID;k++)
						{
							newUser.ID[k] = ud->received_ID[k];
						}
						for(k = 0;k<PIN_MAXIMO;k++)
						{
							newUser.pin[k] = ud->received_PIN[k];
						}
						newUser.categoria = BASIC;
						switch(agregoUsuario(newUser))
						{
							case EXITO:
								PrintMessage("NEW USER ADDED TO DATABASE", true);
								break;
							case DATABASE_LLENO:
								PrintMessage("ERROR - DATABASE FULL", true);
								break;
						}
						nextState.name = MENU;
						nextState.ev_handlers[INPUT_EV] = &MenuRoutine_Input;
						nextState.ev_handlers[TIMER_EV] = &MenuRoutine_Timer;
						nextState.ev_handlers[KEYCARD_EV] = &MenuRoutine_Card;
						userDataReset(true ,true ,true ,true ,ud);
					}
					break;
				default:
					if((ud->choice >= INITIAL) && (j < PIN_MAXIMO))
					{
						ud->received_PIN[j] = INT2CHAR(ud->choice);
						j++;
						userDataReset(false ,false ,false ,true ,ud);
						createPINString(ud);
						string = getpin();
						PrintMessage(string, false);
					}
					if(j == PIN_MAXIMO)
					{
						user_t newUser;
						for(k = 0;k<TAMANO_ID;k++)
						{
							newUser.ID[k] = ud->received_ID[k];
						}
						for(k = 0;k<PIN_MAXIMO;k++)
						{
							newUser.pin[k] = ud->received_PIN[k];
						}
						newUser.categoria = BASIC;
						switch(agregoUsuario(newUser))
						{
							case EXITO:
								PrintMessage("NEW USER ADDED TO DATABASE", true);
								break;
							case DATABASE_LLENO:
								PrintMessage("ERROR - DATABASE FULL", true);
								break;
						}
						nextState.name = MENU;
						nextState.ev_handlers[INPUT_EV] = &MenuRoutine_Input;
						nextState.ev_handlers[TIMER_EV] = &MenuRoutine_Timer;
						nextState.ev_handlers[KEYCARD_EV] = &MenuRoutine_Card;
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
			break;
	}
	return nextState;
}

state_t AddPinRoutine_Timer(UserData_t * ud)
{
	state_t nextState;
	nextState.name = STAY;
	if(ud->timerUd == DISPLAY)
	{
		UpdateDisplay();
	}
	if(ud->timerUd == AFK)
	{
		userDataReset(true ,false ,false ,true ,ud);
		nextState.name = MENU;

		nextState.ev_handlers[INPUT_EV] = &MenuRoutine_Input;
		nextState.ev_handlers[TIMER_EV] = &MenuRoutine_Timer;
		nextState.ev_handlers[KEYCARD_EV] = &MenuRoutine_Card;
		PrintMessage("MENU", false);
	}
	return nextState;
}


state_t AddPinRoutine_Card(UserData_t * ud)
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
	if(IDExists)
	{

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
	else
	{
		PrintMessage("INVALID ID", true);
	}
	return nextState;
}
