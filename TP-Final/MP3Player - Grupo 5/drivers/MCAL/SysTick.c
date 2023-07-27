/***************************************************************************/ /**
  @file     SysTick.c
  @brief    SysTick Driver
  @author   Grupo 2 - Lab de Micros
 ******************************************************************************/
/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "SysTick.h"
#include "hardware.h"
#include "fsl_clock.h"

#include "board.h"

#define SYSTICK_ISR_PERIOD 100000L //1ms
#define INITIAL_SYSTICK_ELEMENTS_ARRAY_LENGTH 20
#define MS_TO_TICK_CONVERTION 100000 //1ms

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
/**
 * @brief 	Finds the amount of not NULL elements in a SystickElement's array.
 * 			The not NULL elements must be consecutive.
 * @param sysTickElements SystickElement's array.
 * @return The amount of not NULL elements.
 */
static int getArrayEffectiveLength(SysTickElement sysTickElements[]);

/**
 * @brief Manages the calling of the callbacks after their period has elapsed.
 */
__ISR__ SysTick_Handler(void);

/*******************************************************************************
 * PRIVATE VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*A SystickElement's array to store the callbacks, their period, and other variables needed.*/
static SysTickElement sysTickElements[INITIAL_SYSTICK_ELEMENTS_ARRAY_LENGTH];
/*A counter that avoid the repetition of the IDs returned by SysTick_AddCallback*/
static int idCounter;
/*Flag indicating initialization completed*/
static bool alreadyInit = false;

static uint32_t freq;
/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
bool SysTick_Init(void)
{
	freq = CLOCK_GetFreq(kCLOCK_CoreSysClk) / 1000 ;
	if(!alreadyInit)
	{
		SysTick->CTRL = 0x00;
		SysTick->LOAD = freq - 1;
		SysTick->VAL = 0x00;
		SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;

		idCounter = 1;
		alreadyInit = true;
	}
	return true;
}

void SysTick_UpdateClk(void)
{
    /* Disable SysTick. */
    SysTick->CTRL &= ~(SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk);
    SysTick->LOAD = 0UL;
    SysTick->VAL  = 0UL;

    freq = CLOCK_GetFreq(kCLOCK_CoreSysClk) / 1000 ;

    SysTick->CTRL = 0x00;
	SysTick->LOAD = freq - 1;
	SysTick->VAL = 0x00;
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;

}

int SysTick_AddCallback(void (*newCallback)(void), int period)
{
	int quotient = (int)(period * MS_TO_TICK_CONVERTION / SYSTICK_ISR_PERIOD); //Calculates how many SYSTICK_ISR_PERIODs are equivalent to the callback period.

	if (quotient <= 0)
		return SystickPeriodError; //period must be greater than SYSTICK_ISR_PERIOD

	SysTickElement newSystickElement = {idCounter, newCallback, quotient, 0, false}; //Creates the new element.
	sysTickElements[getArrayEffectiveLength(sysTickElements)] = newSystickElement;	 //Stores the new element.
	return idCounter++;																 //Returns the corresponding ID and increases the count afterwards.
}

SystickError Systick_ClrCallback(int id)
{

	bool idFound = false;												 //Flag
	int i = 0;															 //Index
	int arrayEffectiveLength = getArrayEffectiveLength(sysTickElements); //Amount of not NULL elements in sysTickElements.

	/*Searches for the id in the array*/
	while ((idFound == false) && (i < arrayEffectiveLength))
	{
		if (sysTickElements[i].callbackID == id) //ID found.
		{
			idFound = true; //Flag ON
			/*Shifts the next elements one position (overwriting the element to be deleted)*/
			for (int j = i; j < ((getArrayEffectiveLength(sysTickElements)) - 1); j++)
			{
				sysTickElements[j].callbackID = sysTickElements[j + 1].callbackID;
				sysTickElements[j].callback = sysTickElements[j + 1].callback;
				sysTickElements[j].counter = sysTickElements[j + 1].counter;
				sysTickElements[j].paused = sysTickElements[j + 1].paused;
				sysTickElements[j].counterLimit = sysTickElements[j + 1].counterLimit;
			}
			/*Re-establishes the previous last position of the array to default values.*/
			sysTickElements[arrayEffectiveLength - 1].callback = NULL;
			sysTickElements[arrayEffectiveLength - 1].counter = 0;
			sysTickElements[arrayEffectiveLength - 1].paused = false;
			sysTickElements[arrayEffectiveLength - 1].callbackID = 0;
			sysTickElements[arrayEffectiveLength - 1].counterLimit = 0;
		}
		i++;
	}
	if (idFound == false)
		return SystickNoIdFound;

	return SystickNoError;
}

SystickError Systick_PauseCallback(int id)
{
	bool idFound = false;												 //Flag
	int i = 0;															 //Index
	int arrayEffectiveLength = getArrayEffectiveLength(sysTickElements); //Amount of not NULL elements in sysTickElements.
	/*Searches for the id in the array*/
	while ((idFound == false) && (i < arrayEffectiveLength))
	{
		if (sysTickElements[i].callbackID == id) //ID found.
		{
			idFound = true;
			sysTickElements[i].paused = true; //Pauses the calling of the callback.
		}
		i++;
	}
	if (idFound == false)
		return SystickNoIdFound;

	return SystickNoError;
}

SystickError Systick_ResumeCallback(int id)
{
	bool idFound = false;												 //Flag
	int i = 0;															 //Index
	int arrayEffectiveLength = getArrayEffectiveLength(sysTickElements); //Amount of not NULL elements in sysTickElements.
	/*Searches for the id in the array*/
	while ((idFound == false) && (i < arrayEffectiveLength))
	{
		if (sysTickElements[i].callbackID == id) //ID found.
		{
			idFound = true;
			sysTickElements[i].paused = false; //Resumes the calling of the callback.
		}
		i++;
	}
	if (idFound == false)
		return SystickNoIdFound;

	return SystickNoError;
}

SystickError Systick_ChangeCallbackPeriod(int id, int newPeriod)
{
	bool idFound = false; //Flag
	int i = 0;			  //Index

	/*Searches for the id in the array*/
	while ((idFound == false) && (i < getArrayEffectiveLength(sysTickElements)))
	{
		if (sysTickElements[i].callbackID == id) //ID found
		{
			idFound = true;
			int quotient = (int)(newPeriod * MS_TO_TICK_CONVERTION / SYSTICK_ISR_PERIOD);
			if (quotient <= 0)
				return SystickPeriodError; //newPeriod must be greater than SYSTICK_ISR_PERIOD

			sysTickElements[i].counterLimit = quotient; //New counter limit.
			sysTickElements[i].counter = 0;				//Restarts counter.
		}
		i++;
	}

	if (idFound == false)
		return SystickNoIdFound;

	return SystickNoError;
}

bool Systick_GetStatus(int id)
{
	bool idFound = false;												 //Flag
	int i = 0;															 //Index
	int arrayEffectiveLength = getArrayEffectiveLength(sysTickElements); //Amount of not NULL elements in sysTickElements.
	/*Searches for the id in the array*/
	while ((idFound == false) && (i < arrayEffectiveLength))
	{
		if (sysTickElements[i].callbackID == id) //ID found.
		{
			return (sysTickElements[i].counter == 0); //Resumes the calling of the callback.
		}
		i++;
	}
	return false;
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

static int getArrayEffectiveLength(SysTickElement sysTickElements[])
{
	int i = 0;				//Index
	bool foundLast = false; //Flag
	while (foundLast == false && i < INITIAL_SYSTICK_ELEMENTS_ARRAY_LENGTH)
	{
		if (sysTickElements[i].callback == NULL) //Current element is null => The previous element was the last one.
			foundLast = true;
		else
			i++;
	}
	return i;
}

__ISR__ SysTick_Handler(void)
{
	gpioToggle(TP);

	for (int i = 0; i < (getArrayEffectiveLength(sysTickElements)); i++) //Iterates through all the elements.
	{
		if (!sysTickElements[i].paused)
		{
			if (sysTickElements[i].counter == sysTickElements[i].counterLimit) //If the counter reaches the counterLimit the element's callback must be called.
			{
				sysTickElements[i].counter = 0;	  //Counter re-establishment.
				(*sysTickElements[i].callback)(); //Callback's calling.
			}
			sysTickElements[i].counter++;
		}
	}

	gpioToggle(TP);
}
