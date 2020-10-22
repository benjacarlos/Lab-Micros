/*
 * fsmState_IdIn.h
 *
 *  Created on: Oct 21, 2020
 *      Author: Agus
 */

#ifndef FSMSTATE_IDIN_H_
#define FSMSTATE_IDIN_H_

#include "User.h"
#include "FSM.h"

state_t IDInRoutine_Input(UserData_t * ud);

state_t IDInMenuRoutine_Timer(UserData_t * ud);

state_t IDInMenuRoutine_Card(UserData_t * ud);

#endif /* FSMSTATE_IDIN_H_ */
