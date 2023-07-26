/***************************************************************************/ /**
  @file     init_state.c
  @brief    Init state functions.
  @author   Grupo
 ******************************************************************************/


#include "init.h"

#include "../../Queue/ev_queue.h"
#include "idle.h"
#include "sd_handle.h"
#include "power_mode_switch.h"

void Init_StartAll(void)
{
	if(SDHandle_SDConnection())
	{
		// Fetch the sd on conection event
		emitEvent(SD_IN_EV);
	}
	else if(SDHandle_IsSDPresent())
	{
		// The sd is present
		emitEvent(SD_IN_EV);
	}
	// Start the idle state
	Idle_InitState();
}
