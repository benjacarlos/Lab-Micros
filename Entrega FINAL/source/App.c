/***************************************************************************//**
  @file     App.c
  @brief    Application functions
  @author   Nicolás Magliola
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdbool.h>
#include "AdminID.h"
//fsm
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
#define INACTIVITY_T 80000

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

void inactivity_callback(void);

/*******************************************************************************
 *******************************************************************************
                        VARIABLES LOCALES
 *******************************************************************************
 ******************************************************************************/

//fsm
//nextstate

static UserData_t userData;

static _Bool changeState = false;
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
   timerStart(INACTIVITY, INACTIVITY_T, inactivity_callback);
   initEncoder();
   InitDisplay();
   Cardreader_Init();
   doorInit();


   //FSm inits + database
   initDataBase();

   //user data init
   userDataReset(true, true, true, true, &userData);


}


/* Función que se llama constantemente en un ciclo infinito */
void App_Run (void)
{

}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void inactivity_callback(void)
{
	pushTimerEvent(INACTIVITY);
}
/*******************************************************************************
 ******************************************************************************/
