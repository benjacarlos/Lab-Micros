/*
 * sysTick.c
 *
 *  Created on: Sep 12, 2020
 *      Author: Agus
 */


#include "SysTick.h"


#include "MK64F12.h"
#include "gpio.h"
#include "board.h"
#include "hardware.h"


irq_fun systick_irqfun;

bool SysTick_Init (void (*funcallback)(void))
{

	systick_irqfun = funcallback;

	//Inicializacion de SysTick
	SysTick->CTRL = 0x00;
	SysTick->LOAD = 12500000L - 1; // 125ms @ 100MHz
	SysTick->VAL = 0x00;
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}


__ISR__ SysTick_Handler(void)
{
	(*systick_irqfun)();
}
