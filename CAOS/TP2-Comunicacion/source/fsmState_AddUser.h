/*
 * fsmState_AddUser.h
 *
 *      Author: Agus
 */

#ifndef FSMSTATE_ADDUSER_H_
#define FSMSTATE_ADDUSER_H_

#include "fsm.h"
#include "User.h"

state_t AddUSerRoutine_Input(UserData_t * ud);

state_t AddUSerRoutine_Timer(UserData_t * ud);

state_t AddUSerRoutine_Card(UserData_t * ud);


#endif /* FSMSTATE_ADDUSER_H_ */
