/***************************************************************************//**
  @file     SysTick.h
  @brief    SysTick driver
  @author   Grupo 5
 ******************************************************************************/


#include "SysTick.h"


#include "MK64F12.h"
#include "gpio.h"
#include "board.h"
#include "hardware.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define SYSTICK_ISR_FREQUENCY   	1000U
#define CPU_FREQUENCY	          	100000000U //100MHz
#define SYSTICK_TICK_MS			   	(1000U / SYSTICK_ISR_FREQUENCY_HZ)
#define SYSTICK_MS2TICKS(x)			((x) / SYSTICK_TICK_MS)

#define MAX_AMOUT_OF_ISR	5

//#define CLK 100000000U //100MHz


/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

static void 	(*systick_irqfun[MAX_AMOUT_OF_ISR])(void);	// Each driver function added to receive systick ticks
static uint8_t 	ISR_elements = 0;					// Amount of functions in ISR

irq_fun systick_irqfun;

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

bool systickInit (void (*funcallback)(void))
{
	// Computing the tick amount with the frequency
	uint32_t ticks = CPU_FREQUENCY / SYSTICK_ISR_FREQUENCY;
	bool succeed = true;

	static bool alreadyInit = false;

	// Verifying if there are subscribers available
	if (ISR_elements < MAX_AMOUT_OF_ISR)
	{
		//Inicializacion de SysTick
		if (!alreadyInit)
		{
			SysTick->CTRL = 0x00;
			SysTick->LOAD = (CPU_FREQUENCY/SYSTICK_ISR_FREQUENCY) - 1; // load en pulsos por periodo - 1
			SysTick->VAL = 0x00;
			SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk
					| SysTick_CTRL_TICKINT_Msk
					| SysTick_CTRL_ENABLE_Msk;

			alreadyInit = true;
		}

		// Adding the driver callback
		systick_irqfun[ISR_elements] = funcallback;
		ISR_elements++;
	}
	else
	{
		succeed = false;
	}

	// Return status of the initialization
	return succeed;
}

/*******************************************************************************
 *******************************************************************************
						INTERRUPT SERVICE ROUTINES
 *******************************************************************************
 ******************************************************************************/

__ISR__ SysTick_Handler(void)
{
	uint8_t i;
	for (i = 0 ; i < ISR_elements ; i++)
	{
		if (systick_irqfun[i])
		{
			systick_irqfun[i]();
		}
	}
}

/////////////////////////
