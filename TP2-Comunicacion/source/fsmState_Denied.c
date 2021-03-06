/*
 * fsmState_Denied.c
 *
 *      Author: Agus
 */

#include "fsmState_PinIn.h"
#include "fsmState_Denied.h"


#include "displayManager.h"
#include "encoder.h"
#include "timer.h"

state_t DeniedRoutine_Input(UserData_t * ud)
{
	state_t nextState;
	nextState.name = STAY;
	return nextState;
	//sigo en denied

}


state_t DeniedRoutine_Timer(UserData_t * ud)
{
	state_t nextState;
	nextState.name = STAY;
	if(ud->timerUd == DISPLAY)
	{
		UpdateDisplay();
	}
	if(ud->timerUd == UNBLOCKED) //BLOCKEEEEED BY JAMEEEEES (EASTER EGG )
	{
		userDataReset(false ,true ,false ,true ,ud);

		//proximo estado pin-in
		nextState.name = PIN_IN;
		nextState.ev_handlers[INPUT_EV] = & PinInRoutine_Input;
		nextState.ev_handlers[TIMER_EV] = & PinInRoutine_Timer;
		nextState.ev_handlers[KEYCARD_EV] = & PinInRoutine_Card;
		PrintMessage("BLOCKED TIME OVER", true);
	}
	return nextState;


}

state_t DeniedRoutine_Card(UserData_t * ud)
{
	state_t nextState;
	nextState.name = STAY;
	return nextState;
}
