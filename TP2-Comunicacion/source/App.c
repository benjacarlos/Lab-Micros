/***************************************************************************//**
  @file     App.c
  @brief    Application functions
  @author   Grupo 5
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdbool.h>
#include "AdminID.h"
#include "fsm.h"
#include "EventQueue.h"


//Input_Output

#include "displayManager.h"
#include "encoder.h"
#include "CardReader.h"
#include "timer.h"
#include "door.h"
#include "timerqueue.h"


#include "SDK/CMSIS/MK64F12.h"
#include "SDK/CMSIS/MK64F12_features.h"

#include <stdio.h>


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define AFK_T 80000

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

void AFK_callback(void);

/*******************************************************************************
 *******************************************************************************
                        VARIABLES LOCALES
 *******************************************************************************
 ******************************************************************************/

static fsm_t fsm;
static state_t nextstate;

static UserData_t userData;

static _Bool changestate = false;
static event_t event;

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/


/* Función que se llama 1 vez, al comienzo del programa */
void App_Init (void)
{
   //Input_Output inits
   timerInit();
   timerStart(AFK, AFK_T, AFK_callback);
   initEncoder();
   InitDisplay();
   Cardreader_Init();
   doorInit();


   //FSm inits + database
   initDataBase();
   fsmInit(&fsm);

   //user data init
   userDataReset(true, true, true, true, &userData);


}


/* Función que se llama constantemente en un ciclo infinito ---->>> aplcia mi maquina de estados*/
void App_Run (void)
{
	event = getEvent(&userData); // tomo el nuevo evento y luego cambio de estado dependiendo del switch
	switch(event)
	{
		case INPUT_EV:
			timerRestart(AFK);
			nextstate = (fsm.presentstate.ev_handlers[INPUT_EV])(&userData); // action routine
			if(nextstate.name != STAY)
			{
				changestate = true;
			}
			break;
		case TIMER_EV:
			nextstate = (fsm.presentstate.ev_handlers[TIMER_EV])(&userData); // action routine
			if(nextstate.name != STAY)
			{
				changestate = true;
			}
			break;
		case KEYCARD_EV:
			timerRestart(AFK);
			nextstate = (fsm.presentstate.ev_handlers[KEYCARD_EV])(&userData); // action routine
			if(nextstate.name != STAY)
			{
				changestate = true;
			}
			break;
		default:
			break;
	}
	if(changestate)
	{
		fsm.presentstate = nextstate;
		changestate = false;
	}
}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void AFK_callback(void)
{
	pushTimerEvent(AFK);
}
/*******************************************************************************
 ******************************************************************************/
