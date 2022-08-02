/*
 * fsmState_Brightness.c
 *
 *      Author: Agus
 */

#include "fsmState_Brightness.h"
#include "fsmState_Menu.h"
#include "fsmState_PinIn.h"

#include "displayManager.h"
#include "encoder.h"
#include "AdminID.h"

#define INTENSITY_OPTIONS	4
#define INCREMENT	1
#define INITIAL	 0
#define LAST_OPTION (INTENSITY_OPTIONS -1)

const char * intStrings[INTENSITY_OPTIONS] = {"25","50","75","100"};



state_t brightnessRoutine_Input(UserData_t * ud)
{
	state_t nextState;
	nextState.name = STAY;
	switch(ud->encoderUd.input)
	{
		case UP:
			if(ud->choice < LAST_OPTION)
			{
				ud->choice += INCREMENT;
			}
			else{
				ud->choice = INITIAL;
			}
			PrintMessage(intStrings[ud->choice], false);
			break;
		case DOWN:
			if(ud->choice > INITIAL)
			{
				ud->choice -= INCREMENT;
			}
			else
			{
				ud->choice = LAST_OPTION;
			}
			PrintMessage(intStrings[ud->choice], false);
			break;
		case ENTER:
			if(ud->choice < INITIAL)
			{
				PrintMessage("PLEASE SELECT INTENSITY FIRST", true);
			}
			else
			{
				SetBrightness((unsigned char) (ud->choice+1));
				userDataReset(false, false, false, true, ud);
				nextState.name = MENU;

				nextState.ev_handlers[INPUT_EV] = &MenuRoutine_Input;
				nextState.ev_handlers[TIMER_EV] = &MenuRoutine_Timer;
				nextState.ev_handlers[KEYCARD_EV] = &MenuRoutine_Card;
				PrintMessage("MENU", false);
			}
			break;
		case CANCEL:
			userDataReset(true ,true ,true ,true ,ud);
			nextState.name = MENU;

			//CONFIGURACION SIGUIENTE ESTADO: MENU
			nextState.ev_handlers[INPUT_EV] = &MenuRoutine_Input;
			nextState.ev_handlers[TIMER_EV] = &MenuRoutine_Timer;
			nextState.ev_handlers[KEYCARD_EV] = &MenuRoutine_Card;
			PrintMessage("MENU", false);
			break;
	}
	return nextState;
}

state_t brightnessRoutine_Timer(UserData_t * ud)
{
	state_t nextState;
	nextState.name = STAY;
	if(ud->timerUd == DISPLAY)
	{
		UpdateDisplay();
	}
	return nextState;
}

state_t brightnessRoutine_Card(UserData_t * ud)
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
		for(i=0;i<TAMANO_ID;++i){
			ud->received_ID[i] = cardID[i];
		}
		userDataReset(false, false, false, true, ud);
		nextState.name = PIN_IN;

		//siguiente estado pin-in
		nextState.ev_handlers[INPUT_EV] = &PinInRoutine_Input;
		nextState.ev_handlers[TIMER_EV] = &PinInRoutine_Timer;
		nextState.ev_handlers[KEYCARD_EV] = &PinInRoutine_Card;
		PrintMessage("ENTER PIN", true);
	}
	else
	{
		PrintMessage("INVALID ID", true);
	}
	return nextState;
}
