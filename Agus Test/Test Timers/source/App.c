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

#include"SysTick.h"

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
static void test_fun(void);
static void systick_test_fun(void);


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

static void delayLoop(uint32_t veces)
{
    while (veces--);
}

//funcion de prueba
void test_fun(void)
{
	gpioToggle(PIN_LED_RED);
}

void systick_test_fun(void)
{

	static uint32_t counter = 4; //cuento 4 veces y asi con 125 ms logro un blink cada medio segundo, es practico porque solo tengo 24 bits en el registro systick

		if (counter == 0)
		{
			gpioToggle(PIN_LED_RED);
			counter = 4;
		}

		counter--;
}
/*******************************************************************************
 ******************************************************************************/
