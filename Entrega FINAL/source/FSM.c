#include "FSM.h"
//#include "stateMenu.h"
#include "displayManager.h"

STATE* fsm (STATE *punteroatestado, BYTE evento_actual)
{
    while ((punteroatestado -> evento) != evento_actual && punteroatestado -> evento != EV_FIN_TABLA)
            punteroatestado++;

    (*punteroatestado->accion) ();

    punteroatestado = punteroatestado -> nextstate;

    return (punteroatestado);
}

void fsmInit(fsm_t *fsm)
{
	fsm->presentstate.name = MENU;

	//se asignan los handlers
	fsm->presentstate.routines[INPUT_EV] = &MinputEvHandler;
	fsm->presentstate.routines[TIMER_EV] = &MtimerEvHandler;
	fsm->presentstate.routines[KEYCARD_EV] = &MkeycardEvHandler;

	//Se imprime en pantalla MENU (menu principal inicial)
	PrintMessage("MENU", false);
}

