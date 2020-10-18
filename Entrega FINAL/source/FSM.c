#include "FSM.h"

STATE* fsm (STATE *punteroatestado, BYTE evento_actual){
    while ((punteroatestado -> evento) != evento_actual && punteroatestado -> evento != EV_FIN_TABLA)
            punteroatestado++;

    (*punteroatestado->accion) ();

    punteroatestado = punteroatestado -> nextstate;

    return (punteroatestado);
}
