/*
 * displayLed.c
 *
 *  Created on: 15 Oct 2020
 *      Author: benja
 */


#include "displayLed.h"
#include "gpio.h"
#include "board.h"

/******************************************************************************
 *									DEFINICIONES
 ******************************************************************************/
//Leds del display 1, 2, 3
#define STATUS0 PORTNUM2PIN(PA,0)//PTA0
#define STATUS1 PORTNUM2PIN(PC,4)//PTC4
   //Leds del board (incluidos en board.h)
//#define PIN_LED_RED     PORTNUM2PIN(PB, 22)// PTB22
//#define PIN_LED_GREEN   PORTNUM2PIN(PE, 26) // PTE26
//#define PIN_LED_BLUE    PORTNUM2PIN(PB,21) // PTB21
/******************************************************************************
 *									VARIABLES
 ******************************************************************************/
static bool initialized = false;

void InitLeds()
{
	if(!initialized)
	{
		//Display leds
		gpioMode(STATUS0, OUTPUT);
		gpioMode(STATUS1, OUTPUT);

		//Board leds
		gpioMode(PIN_LED_RED, OUTPUT);
		gpioMode(PIN_LED_GREEN, OUTPUT);
		gpioMode(PIN_LED_BLUE, OUTPUT);

		initialized = true;
	}
}
void ResetLeds();
void LedBlink(int pos, bool on_off);
void SetLed(int pos, bool on_off);
bool GetLedBlink(int pos);
bool GetLedStatus();
void SetBrightness(int pos, int brightness);
