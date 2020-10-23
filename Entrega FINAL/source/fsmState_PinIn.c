/*
 * fsmState_PinIn.c
 *
 *  Created on: Oct 22, 2020
 *      Author: Agus
 */

#include "fsmState_PinIn.h"
#include "fsmState_Menu.h"
#include "fsmState_Denied.h"
#include "fsmState_Aprovado.h"

#include "displayManager.h"
#include "encoder.h"
#include "AdminId.h"
#include "timerqueue.h"
#include "fsmUtils_pin.h"


#define BLOCKED_TIME	60000UL // 1 min in ms


static long unsigned int getBlockedTime(void);

void blockedCallback(void);


void blockedCallback()
{
	pushTimerEvent(UNBLOCKED);
	timerDisable(UNBLOCKED);
	timerRestart(INACTIVITY);
}


static int tryNro = 0;
static long unsigned int f1=0;
static long unsigned int f2=1;

static long unsigned int getBlockedTime(void)
{
	static long unsigned int f3;
	f3 = f2+f1;
	f1 = f2;
	f2 = f3;
	return f3 * BLOCKED_TIME;
}





state_t PinInRoutine_Input(UserData_t * ud)
{
	state_t nextState;
	nextState.name = STAY;
	char* string;
	int j = 0;
	bool validPIN = false;
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
						ud->received_PIN[j-1] = '\0';
					}
					userDataReset(false ,false ,false ,true ,ud);
					createPINString(ud);
					string = getpin();
					PrintMessage(string, false);
					break;
				case ERASE_ALL:
					userDataReset(false ,true ,false ,true ,ud);
					createPINString(ud);
					string = getpin();
					PrintMessage(string, false);
					break;
				case BLANK:
					if(j == PIN_MINIMO)
					{
						ud->received_PIN[j] = ' ';
						validPIN = verificoPIN(ud->received_ID, ud->received_PIN);
						if(validPIN)
						{

							//Paso a usuaria aprobado
							nextState.name = APROVADO;
							tryNro = 0;
							nextState.ev_handlers[INPUT_EV] = &AprovadoRoutine_Input;
							nextState.ev_handlers[TIMER_EV] = &AprovadoRoutine_Timer;
							nextState.ev_handlers[KEYCARD_EV] = &AprovadoRoutine_Card;
							PrintMessage("USER APPROVED", true);
						}
						else
						{
							PrintMessage("INCORRECT PIN", true);
							userDataReset(false ,true ,false ,true ,ud);
							tryNro ++;
							if(tryNro < MAX_TRIES)
							{
								//stays
							}
							else
							{
								//Paso a usuaria bloqueado
								nextState.name = DENIED;
								nextState.ev_handlers[INPUT_EV] = &DeniedRoutine_Input;
								nextState.ev_handlers[TIMER_EV] = &DeniedRoutine_Timer;
								nextState.ev_handlers[KEYCARD_EV] = &DeniedRoutine_Card;
								PrintMessage("USER BLOCKED", true);
								timerStart(UNBLOCKED, getBlockedTime(), blockedCallback);
							}
						}
					}
					break;
				default: // number
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
						validPIN = verificoPIN(ud->received_ID, ud->received_PIN);
						if(validPIN)
						{
							//Paso a usuaria aprobado
							nextState.name = APROVADO;
							tryNro = 0;
							nextState.ev_handlers[INPUT_EV] = &AprovadoRoutine_Input;
							nextState.ev_handlers[TIMER_EV] = &AprovadoRoutine_Timer;
							nextState.ev_handlers[KEYCARD_EV] = &AprovadoRoutine_Card;
							PrintMessage("APROVADO ENTRANDO", true);
						}
						else
						{
							PrintMessage("INCORRECT PIN", true);
							userDataReset(false ,true ,false ,true ,ud);
						    tryNro ++;
						    if(tryNro < MAX_TRIES)
						    {
						    	//	stays
						    }
						    else
						    {
						    	//Paso a usuaria bloqueado
						    	nextState.name = DENIED;
								nextState.ev_handlers[INPUT_EV] = &DeniedRoutine_Input;
								nextState.ev_handlers[TIMER_EV] = &DeniedRoutine_Timer;
								nextState.ev_handlers[KEYCARD_EV] = &DeniedRoutine_Card;
								PrintMessage("USER BLOCKED", true);
								timerStart(UNBLOCKED, getBlockedTime(), blockedCallback);
						    }
						}
					}
					break;
			}
			userDataReset(false ,false ,false ,true ,ud);
			break;
		case CANCEL:
			userDataReset(true ,true ,true ,true ,ud);
		    tryNro = 0;

		    //Paso a menu
		    nextState.name = MENU;
		    nextState.ev_handlers[INPUT_EV] = &MenuRoutine_Input;
		    nextState.ev_handlers[TIMER_EV] = &MenuRoutine_Timer;
		    nextState.ev_handlers[KEYCARD_EV] = &MenuRoutine_Card;
			PrintMessage("MENU", false);
			break;
	}
	return nextState;
}


state_t PinInRoutine_Timer(UserData_t * ud)
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


state_t PinInRoutine_Card(UserData_t * ud)
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

		//configuracion eventos funciones siguientes
		nextState.ev_handlers[INPUT_EV] = &PinInRoutine_Input;
		nextState.ev_handlers[TIMER_EV] = &PinInRoutine_Timer;
		nextState.ev_handlers[KEYCARD_EV] = &PinInRoutine_Card;
	}
	else{
		// show message in display
		PrintMessage("INVALID ID", true);
	}
	return nextState;
}
