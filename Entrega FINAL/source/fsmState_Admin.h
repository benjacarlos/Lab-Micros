/*
 * fsmState_admin.h
 *
 *  Created on: Oct 22, 2020
 *      Author: Agus
 */

#ifndef FSMSTATE_ADMIN_H_
#define FSMSTATE_ADMIN_H_

#include "User.h"
#include "fsm.h"

state_t AdminRoutine_Input(UserData_t * ud);

state_t AdminRoutine_Timer(UserData_t * ud);

state_t AdminRoutine_Card(UserData_t * ud);


#endif /* FSMSTATE_ADMIN_H_ */
