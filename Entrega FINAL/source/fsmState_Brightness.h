/*
 * fsmState_Brightness.h
 *
 *      Author: Agus
 */

#ifndef FSMSTATE_BRIGHTNESS_H_
#define FSMSTATE_BRIGHTNESS_H_

#include "User.h"
#include "fsm.h"

state_t brightnessRoutine_Input(UserData_t * ud);

state_t brightnessRoutine_Timer(UserData_t * ud);

state_t brightnessRoutine_Card(UserData_t * ud);


#endif /* FSMSTATE_BRIGHTNESS_H_ */
