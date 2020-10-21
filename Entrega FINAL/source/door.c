/*
 * door.c
 *
 *  Created on: Oct 21, 2020
 *      Author: Agus
 */


#include "door.h"
#include "gpio.h"
#include "timer.h"
#include <stdint.h>

#define PIN_LED_GREEN PORTNUM2PIN(PE,26)

#define LED_ON      LOW
#define LEND_OFF	HIGH

void closeAfterTimeOut(void);

void doorInit(void)
{
	gpioMode(PIN_LED_GREEN, OUTPUT);
	closeDoor();
	timerInit();
}


void openDoorTemporally()
{
	openDoor();
	timerStart(DOOR, TIMEOUT_DOOR, closeAfterTimeOut);
}


void closeAfterTimeOut(void)
{
	closeDoor();
	timerDisable(DOOR);
}

void openDoor(void)
{
	gpioWrite(PIN_LED_GREEN, LED_ON);
}

void closeDoor(void)
{
	gpioWrite(PIN_LED_GREEN, LEND_OFF);
}

void toggleDoor(void)
{
	gpioToggle(PIN_LED_GREEN);
}

