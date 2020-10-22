/*
 * fsmState_AddPin.c
 *
 *  Created on: Oct 22, 2020
 *      Author: Agus
 */


#include "fsmState_AddPin.h"
#include "fsmState_Menu.h"
#include "fsmState_PinIn.h"

#include "AdminID.h"
#include "displayManager.h"
#include "encoder.h"

#define PIN_OPTIONS	13
#define LAST_OPTION_PIN	(PIN_OPTIONS-1)
#define INCREMENT	1
#define INITIAL	0
#define MAX_TRIES	3
#define HIDDEN '-'
#define STRING_CANT	(PIN_MAXIMO+1)
#define INT2CHAR(x)	((char)(x+48))

typedef enum {ZERO,ONE,TWO,THREE,FOUR,FIVE,SIX,SEVEN,EIGHT,NINE,BLANK,ERASE_LAST,ERASE_ALL}idOption_name;
static const char pinStrings[PIN_OPTIONS] = {'0','1','2','3','4','5','6','7','8','9',' ','L','A'};
static char PINstring[STRING_CANT];


static void createPINString(UserData_t * ud);

static void createPINString(UserData_t * ud){
	int i=0;
	while(ud->received_PIN[i] != '\0')
	{
		PINstring[i] = HIDDEN;
		i++;
	}
	if(ud->choice != -1){
		PINstring[i] = pinStrings[ud->choice];
		i++;
	}
	PINstring[i] = '\0';
}

state_t AddPinRoutine_Input(UserData_t * ud)
{
	state_t nextState;
	nextState.name = STAY;
	int j = 0;
	int k = 0;
	switch(ud->encoderUd.input)
	{
		case UP: // change current option
			if(ud->choice < LAST_OPTION_PIN){
				ud->choice += INCREMENT;
			}
			else
			{
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
			else
			{
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
						ud->received_ID[j-1] = '\0';
					}
					userDataReset(false ,false ,false ,true ,ud);
					createPINString(ud);
					PrintMessage(PINstring, false);
					break;
				case ERASE_ALL:
					userDataReset(true ,false ,false ,true ,ud);
					createPINString(ud);
					PrintMessage(PINstring, false);
					break;
				case BLANK:
					if(j == PIN_MINIMO)
					{
						ud->received_PIN[j] = ' ';
						user_t newUser;
						for(k = 0;k<TAMANO_ID;k++){
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
				default: // number
					if((ud->choice >= INITIAL) && (j < PIN_MAXIMO)){
						ud->received_PIN[j] = INT2CHAR(ud->choice);
						j++;
						userDataReset(false ,false ,false ,true ,ud);
						createPINString(ud);
						PrintMessage(PINstring, false);
					}
					if(j == PIN_MAXIMO)
					{ // save user
						user_t newUser;
						for(k = 0;k<TAMANO_ID;k++){
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
			break; // Cancels selection and back to menu
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


state_t AddPinRoutine_Card(UserData_t * ud)
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
		userDataReset(false, false, false, true, ud);
		nextState.name = PIN_IN;
		nextState.ev_handlers[INPUT_EV] = &PinInRoutine_Input;
		nextState.ev_handlers[TIMER_EV] = &PinInRoutine_Timer;
		nextState.ev_handlers[KEYCARD_EV] = &PinInRoutine_Card;
		PrintMessage("ENTER PIN", false);
	}
	else
	{
		// show message in display
		PrintMessage("INVALID ID", true);
	}
	return nextState;
}
