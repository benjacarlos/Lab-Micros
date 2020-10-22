/*
 * fsmState_PinIn.h
 *
 *  Created on: Oct 22, 2020
 *      Author: Agus
 */

#ifndef FSMSTATE_PININ_H_
#define FSMSTATE_PININ_H_

#include "FSM.h"
#include "user.h"

state_t PinInRoutine_Input(UserData_t * ud);

state_t PinInRoutine_Timer(UserData_t * ud);

state_t PinInRoutine_Card(UserData_t * ud);


#endif /* FSMSTATE_PININ_H_ */
