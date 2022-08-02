#include "FSM.h"
#include "fsmState_Menu.h"
#include "displayManager.h"

void fsmInit(fsm_t *fsm)
{
	fsm->presentstate.name = MENU;

	//se asignan los handlers
	fsm->presentstate.ev_handlers[INPUT_EV] = &MenuRoutine_Input;
	fsm->presentstate.ev_handlers[TIMER_EV] = &MenuRoutine_Timer;
	fsm->presentstate.ev_handlers[KEYCARD_EV] = &MenuRoutine_Card;

	//Se imprime en pantalla MENU (menu principal inicial)
	PrintMessage("MENU", false);
}

