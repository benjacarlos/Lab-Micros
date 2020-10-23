/*
 * timerqueue.h
 *
 *      Author: Agus
 */

#ifndef TIMERQUEUE_H_
#define TIMERQUEUE_H_

#include "timer.h"
#include <stdbool.h>


#define MAX_TIMER_EVENTS 50

typedef struct{
	timerData_t events[MAX_TIMER_EVENTS];
	unsigned int size;
}timerQueue_t;



void timerqueueInit(void);

//Guarda un evento de timer en la cola, y retorna true en caso que haya podido hacerse. Si la lista esta llena se sobreescribe el ultimo evento y retorno falso.
bool pushTimerEvent(timerData_t);

//Saca y retorna primer evento de la lista
timerData_t popTimerEvent(void);


_Bool timerqueueEmpty(void);



#endif /* TIMERQUEUE_H_ */
