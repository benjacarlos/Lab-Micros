/*
 * EventQueue.h
 *
 *  Created on: Oct 21, 2020
 *      Author: Agus
 */

#ifndef EVENTQUEUE_H_
#define EVENTQUEUE_H_

#include "User.h"

#define MAX_EVENTS 25

typedef enum {INPUT_EV, TIMER_EV, KEYCARD_EV, EVENT_N}event_t;

event_t getEvent(UserData_t * ud);

void userDataReset(_Bool resetID, _Bool resetPIN, _Bool resetCategory, _Bool resetOption, UserData_t * ud);

#endif /* EVENTQUEUE_H_ */
