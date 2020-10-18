/*
 * boardLed.h
 *
 *  Created on: 17 Oct 2020
 *      Author: benja
 */

#ifndef BOARDLED_H_
#define BOARDLED_H_
#include <stdbool.h>

enum {BR_LED_RED, BR_LED_GREEN, BR_LED_BLUE, BR_TOTAL_LED};

void ledBoardInit();
void setLedBoard(int led_color, bool on_off);

#endif /* BOARDLED_H_ */
