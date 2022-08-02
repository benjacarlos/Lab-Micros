/*
 * EventQueue.c
 *
 *  Created on: Oct 21, 2020
 *      Author: Agus
 */


#include "EventQueue.h"

#include "timer.h"
#include "timerqueue.h"
#include "encoder.h"
#include "CardReader.h"


event_t getEvent(UserData_t * userData)
{
	event_t event = EVENT_N;

	if(!timerqueueEmpty())
	{
		userData->timerUd = popTimerEvent();
		event = TIMER_EV;
	}
	else if(lecture_ready())
	{
		userData->magnetLectorUd = get_decoded(); // get specific data for that event
		event = KEYCARD_EV;
	}
	else if(isEncoderEventValid())
	{
		userData->encoderUd = pullEncoderEvent();
		event = INPUT_EV;
	}
	return event;
}

void userDataReset(_Bool resetID, _Bool resetPIN, _Bool resetCategory, _Bool resetOption, UserData_t * ud)
{
	int i;
	if(resetID){
	    for(i=0;i < TAMANO_ID;++i){
	    	ud->received_ID[i] = '\0';
	    } // clean user ID
	}
	if(resetPIN){
	    for(i=0;i < PIN_MAXIMO;++i){
	    	ud->received_PIN[i] = '\0';
	    } // clean user PIN
	}
	if(resetCategory){
		ud->category = NONE;
	}
	if(resetOption){
		ud->choice = -1;
	}
}
