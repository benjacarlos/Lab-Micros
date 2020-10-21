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
//queue


//Input_Output

#include "displayManager.h"
#include "encoder.h"
#include "CardReader.h"
#include "timer.h"
#include "door.h"

//timerqueue


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

//changingstate
//nameevent

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
	//pusshtimerevent
}
/*******************************************************************************
 ******************************************************************************/
