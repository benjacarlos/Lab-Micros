/*
 * fsmState_AddPin.h
 *
 *      Author: Agus
 */

#ifndef FSMSTATE_ADDPIN_H_
#define FSMSTATE_ADDPIN_H_

#include "fsm.h"
#include "User.h"

state_t AddPinRoutine_Input(UserData_t * ud);

state_t AddPinRoutine_Timer(UserData_t * ud);

state_t AddPinRoutine_Card(UserData_t * ud);

#endif /* FSMSTATE_ADDPIN_H_ */
