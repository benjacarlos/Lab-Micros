/*
 * fsmState_RemoveUser.h
 *
 *  Created on: Oct 22, 2020
 *      Author: Agus
 */

#ifndef FSMSTATE_REMOVEUSER_H_
#define FSMSTATE_REMOVEUSER_H_

#include "fsm.h"
#include "User.h"


state_t removeUserRoutine_Input(UserData_t * ud);

state_t removeUserRoutine_Timer(UserData_t * ud);

state_t removeUserRoutine_Card(UserData_t * ud);

#endif /* FSMSTATE_REMOVEUSER_H_ */
