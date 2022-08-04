/***************************************************************************/ /**
  @file     idle.c
  @brief    Idle state functions.
  @author   Grupo
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "idle.h"
#include "queue.h"
//#include "power_mode_switch.h"
//#include "LCD_GDM1602A.h"
//#include "time_service.h"
#include "timer.h"
//#include "memory_manager.h"
//#include "SysTick.h"
//#include "audio_manager.h"
//#include "LCD_GDM1602A.h"

/*********************************************************
 * 		LOCAL STRUCTS AND ENUMS
 ********************************************************/
typedef enum
{
  SLEEP_MODE,
  OPERATION_MODE,
} OpMode_t;

/*******************************************************************************
 * PRIVATE VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

/**
 * @brief Changes the operation mode of the device.
 * @param OpMode_t operation mode.
 */
static void setOperationMode(OpMode_t OpMode);


/*
 *@brief Callback after init idle to change to sleep mode
 */
static void setSleepMode(void);

/*
 *@brief Callback after changing to high power mode
 */
static void emitStartEv(void);

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void Idle_InitState(void)
{
	Audio_deinit();
	LCD_clearDisplay();

	timerStart(IDLE_T, 1000, TIM_MODE_SINGLESHOT, setSleepMode);
}

void Idle_StartUp(void)
{
	if (!Mm_IsSDPresent())
		return;
	setOperationMode(OPERATION_MODE);

	if(timeCallbackId != -1)
	{
		Timer_Delete(timeCallbackId);
		timeCallbackId = -1;
	}
	TimeService_Disable();


	timeCallbackId = Timer_AddCallback(emitStartEv, 3000, true); //Delay until clock stabilizes

}

void Idle_Update()
{
	TimeServiceDate_t date = TimeService_GetCurrentDateTime();

	char dateString[16];
	char timeString[16];
	snprintf(dateString, sizeof(dateString), "   %02hd-%02hd-%04hd     ", date.day, date.month, date.year);
	snprintf(timeString, sizeof(timeString), "    %02hd:%02hd:%02hd      ", date.hour,date.minute, date.second);

	//LCD_writeStrInPos ("Poner SD para que utilizar MP3", x, y, z)
	LCD_writeStrInPos(timeString, 15, 0, 0);
	LCD_writeStrInPos(dateString, 15, 1, 0);
}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

static void changePowerMode(void)
{
	setEnergyConsumptionMode(LOW_CONSUMPTION);
	timeCallbackId = -1;

	LCD_UpdateClock();
	TimeService_Enable();
	SysTick_UpdateClk();
}

static void emitStartEv(void)
{
	timeCallbackId = -1;
	LCD_UpdateClock();
	SysTick_UpdateClk();
	emitEvent(START_EV);
}

static void setEnergyConsumptionMode(EnergyConsumptionMode_t energyConsumptionMode)
{
	  switch (energyConsumptionMode)
	  {
	  case LOW_CONSUMPTION:
		  PowerMode_SetVLPRMode();
		break;

	  case HIGH_CONSUMPTION:
		  PowerMode_SetRunMode();
		break;

	  default:
		break;
	  }
}
