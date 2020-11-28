/*
 * fsmState_Menu.h
 *
 *      Author: Agus
 */

#ifndef FSMSTATE_MENU_H_
#define FSMSTATE_MENU_H_

#include "User.h"
#include "FSM.h"

//Aca quizas puede hacerse con una sola funcion
state_t MenuRoutine_Input(UserData_t * ud);

state_t MenuRoutine_Timer(UserData_t * ud);

state_t MenuRoutine_Card(UserData_t * ud);


#endif /* FSMSTATE_MENU_H_ */
