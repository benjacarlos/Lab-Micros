/***************************************************************************/ /**
  @file     init_state.c
  @brief    Init state functions.
  @author   Grupo 2 - Lab de Micros
 ******************************************************************************/


#include "queue.h"
#include "States/init_state.h"
#include "States/idle_state.h"
#include "memory_manager.h"
#include "power_mode_switch.h"

void Init_StartAll(void)
{
	if(Mm_SDConnection())
	{
		// Fetch the sd on conection event
		emitEvent(SD_IN_EV);
	}
	else if(Mm_IsSDPresent())
	{
		//The sd is present but the event has been fetch while not able to handle
		emitEvent(SD_IN_EV);
	}
	// Start the idle state, not best practise to do here ( ͡° ͜ʖ ͡°)
	Idle_InitState();
}
