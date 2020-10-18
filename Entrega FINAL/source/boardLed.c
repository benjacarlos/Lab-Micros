/*
 * boardLed.c
 *
 *  Created on: 17 Oct 2020
 *      Author: benja
 */


#include "board.h"
#include "boardLed.h"


static bool initialized = false;

void ledBoardInit()
{
	if(!initialized)
	{
		gpioMode(PIN_LED_RED, OUTPUT);
		gpioMode(PIN_LED_GREEN, OUTPUT);
		gpioMode(PIN_LED_BLUE, OUTPUT);

		initialized = true;
	}
}

void setLedBoard(int led_color, bool on_off)
{
	switch (color)
	{
		//led on when low (check)
		case BR_LED_RED:
			gpioWrite(PIN_LED_RED, !on_off);
			break;
		case BR_LED_GREEN:
			gpioWrite(PIN_LED_GREEN, !on_off);
			break;
		case BR_LED_BLUE:
			gpioWrite(PIN_LED_BLUE, !on_off);
			break;
		default:
			break;
	}
}
