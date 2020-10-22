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


#define TEST_PIN PORTNUM2PIN(PB, 18)
#define CLK 100000000U //100MHz

irq_fun systick_irqfun;

bool SysTick_Init (void (*funcallback)(void))
{


	//Inicializacion de SysTick
	SysTick->CTRL = 0x00;
	SysTick->LOAD = (CLK/SYSTICK_ISR_FREQUENCY_HZ) - 1; // load en pulsos por periodo - 1
	SysTick->VAL = 0x00;
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
	systick_irqfun = funcallback;

	gpioMode(TEST_PIN, OUTPUT);
}


__ISR__ SysTick_Handler(void)
{
	gpioWrite(TEST_PIN, HIGH);
	(*systick_irqfun)();
	gpioWrite(TEST_PIN, LOW);
}
