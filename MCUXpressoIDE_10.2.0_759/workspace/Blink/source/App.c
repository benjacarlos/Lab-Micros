/***************************************************************************//**
  @file     App.c
  @brief    Application functions
  @author   Nicolás Magliola
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "board.h"
#include "gpio.h"
#include "SysTick.h"
#include "hardware.h"

#include "SDK/CMSIS/MK64F12.h"
#include "SDK/CMSIS/MK64F12_features.h"

#include <stdio.h>


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

static void delayLoop(uint32_t veces);


/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/* Función que se llama 1 vez, al comienzo del programa */
void App_Init (void)
{
    gpioMode(PIN_LED_RED, OUTPUT);
    gpioMode(PIN_SW3, INPUT);
//    SysTick_init(handler(PUERTOA))
    NVIC_EnableIRQ(PORTA_IRQn);

}


/* Función que se llama constantemente en un ciclo infinito */
void App_Run (void)
{
	if (!gpioRead(PIN_SW3))
	{

	}
}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

static void delayLoop(uint32_t veces)
{
    while (veces--);
}


__ISR__ PORTA_IRQHandler (void)
{
// Clear port IRQ flag
PORT_ClearInterruptFlag (PORTA, PIN_SW3);
gpioToggle(PIN_LED_RED);
}


void PORT_ClearInterruptFlag (PORport, pin_t pin)
{
port->PCR[PIN2NUM(pin)] |= PORT_PCR_ISF_MASK;
}


/*******************************************************************************
 ******************************************************************************/
