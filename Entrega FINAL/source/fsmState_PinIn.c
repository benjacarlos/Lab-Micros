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


#define PIN_OPTIONS	13
#define LAST_OPTION_PIN	(PIN_OPTIONS - 1)
#define INCREMENT	1
#define INITIAL	0
#define MAX_TRIES 3
#define HIDDEN '-'
#define STRING_CANT	(PIN_MAXIMO + 1)
#define INT2CHAR(x)	((char)(x+48))
#define BLOCKED_TIME	60000UL // 1 min in ms


typedef enum {ZERO,ONE,TWO,THREE,FOUR,FIVE,SIX,SEVEN,EIGHT,NINE,BLANK,ERASE_LAST,ERASE_ALL}idOption_name;
static const char pinStrings[PIN_OPTIONS] = {'0','1','2','3','4','5','6','7','8','9',' ','L','A'};
static char PINstring[STRING_CANT];


static long unsigned int getBlockedTime(void);

void blockedCallback(void);

static void createPINString(UserData_t * ud);

static void createPINString(UserData_t * ud)
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
			PrintMessage(PINstring, false);
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
			PrintMessage(PINstring, false);
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
					PrintMessage(PINstring, false);
					break;
				case ERASE_ALL:
					userDataReset(false ,true ,false ,true ,ud);
					createPINString(ud);
					PrintMessage(PINstring, false);
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
						PrintMessage(PINstring, false);
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
							int a = 0;
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
		nextstate.ev_handlers[INPUT_EV] = &PinInRoutine_Input;
		nextstate.ev_handlers[TIMER_EV] = &PinInRoutine_Timer;
		nextstate.ev_handlers[KEYCARD_EV] = &PinInRoutine_Card;
	}
	else{
		// show message in display
		PrintMessage("INVALID ID", true);
	}
	return nextState;
}
