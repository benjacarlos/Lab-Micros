/*
 * fsmState_Aprovado.h
 *
 *  Created on: Oct 22, 2020
 *      Author: Agus
 */

#ifndef FSMSTATE_APROVADO_H_
#define FSMSTATE_APROVADO_H_

#include "User.h"
#include "fsm.h"

state_t AprovadoRoutine_Input(UserData_t * ud);

state_t AprovadoRoutine_Timer(UserData_t * ud);

state_t AprovadoRoutine_Card(UserData_t * ud);

#endif /* FSMSTATE_APROVADO_H_ */
