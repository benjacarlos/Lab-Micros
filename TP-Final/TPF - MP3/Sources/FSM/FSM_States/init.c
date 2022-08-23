/***************************************************************************/ /**
  @file     init_state.c
  @brief    Init state functions.
  @author   Grupo
 ******************************************************************************/


#include "init.h"

#include "queue.h"
#include "States/idle.h"
//#include "memory_manager.h"
//#include "power_mode_switch.h"

void Init_StartAll(void)
{
	if(Mm_SDConnection())
	{
		// Fetch the sd on conection event
		emitEvent(SD_IN_EV);
	}
	else if(Mm_IsSDPresent())
	{
		// The sd is present
		emitEvent(SD_IN_EV);
	}
	// Start the idle state
	Idle_InitState();
}
