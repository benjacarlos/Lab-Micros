/***************************************************************************/ /**
  @file     init.c
  @brief    Init state functions
  @author   Grupo 5
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "queue.h"
#include "States/init.h"
#include "States/idle.h"
#include "memory_manager.h"
#include "power_mode_switch.h"

/*******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 ******************************************************************************/

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
	// Start the idle state
	Idle_InitState();
}
