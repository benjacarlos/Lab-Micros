/*
 * timerqueue.c
 *
 *      Author: Grupo 5
 */

#include "timerqueue.h"

/**********************************************************************
 *						VARIABLES LOCALES
 **********************************************************************/

static timerQueue_t queue;


/**********************************************************************
 * 						FUNCIONES DEL HEADER
 **********************************************************************/

void timerqueueInit(void)
{
	static _Bool yaInit = false;
	if(!yaInit)
	{
		queue.size = 0;
		yaInit = true;
	}
}


_Bool pushTimerEvent(timerData_t event)
{
	_Bool out = false;
	if(queue.size < MAX_TIMER_EVENTS - 1)
	{
		(queue.events)[queue.size] = event;
		(queue.size)++;
		out = true;
	}
	else
	{
		(queue.events)[MAX_TIMER_EVENTS - 1] = event;
	}
	return out;
}

timerData_t popTimerEvent(void)
{
	return (queue.events)[--(queue.size)];
}

_Bool timerqueueEmpty(void)
{
	_Bool out;
	if(queue.size == 0)
	{
		out = true;
	}
	else
	{
		out = false;
	}
	return out;
}
