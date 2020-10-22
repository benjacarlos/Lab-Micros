/*
	Maquina de estados 14/10/2020 by Juan Martin
*/

#ifndef FSM_H
#define FSM_H

//includes
#include <stdbool.h>
#include "EventQueue.h"

typedef enum {MENU, ID_IN, PIN_IN, CHANGE_BRIGTHNESS, APROVADO, DENIED, MODO_ADMIN, CHANGE_PIN_MODE, ADD_PIN, ADD_USER_MODE, REMOVE_USER_MODE, STATES_N, STAY}state_name;


//debe definirse asi para no tener problemas en la estructura.
struct estados;
typedef struct estados (*evHandler_pointer)(UserData_t *);


typedef struct estados{
	state_name name;
	evHandler_pointer ev_handlers[EVENT_N];
}state_t;


typedef struct{
	state_t presentstate;
	_Bool salir;
}fsm_t;


void fsmInit(fsm_t *fsm);

#endif
	
