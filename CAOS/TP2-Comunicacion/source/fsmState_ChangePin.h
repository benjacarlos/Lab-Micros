/*
 * fsmState_ChangePin.h
 *
 *      Author: Agus
 */

#ifndef FSMSTATE_CHANGEPIN_H_
#define FSMSTATE_CHANGEPIN_H_

#include "User.h"
#include "fsm.h"

state_t pinChangeRoutine_Input(UserData_t * ud);

state_t pinChangeRoutine_Timer(UserData_t * ud);

state_t pinChangeRoutine_Card(UserData_t * ud);

#endif /* FSMSTATE_CHANGEPIN_H_ */
