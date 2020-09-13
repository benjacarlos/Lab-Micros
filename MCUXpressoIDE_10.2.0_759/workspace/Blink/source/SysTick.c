/*
 * sysTick.c
 *
 *  Created on: Sep 12, 2020
 *      Author: Agus
 */


#include "SysTick.h"
#include "core_cm4.h"



bool SysTick_Init (void (*funcallback)(void))
{
    //Inicializacion de SysTick
	SysTick->CTRL = 0x00;
	SysTick->LOAD = 50000000L - 1; // 500ms @ 100MHz
	SysTick->VAL = 0x00;
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
	SysTick_CTRL_TICKINT_Msk |
	SysTick_CTRL_ENABLE_Msk;
}

