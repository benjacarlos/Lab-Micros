/*
 * displayLed.h
 *
 *  Created on: 15 Oct 2020
 *      Author: benja
 */

#ifndef DISPLAYLED_H_
#define DISPLAYLED_H_
#include <stdbool.h>

void InitDisplayLeds();
void ResetDisplayLeds();
void LedBlink(int pos, bool on_off);
void SetDisplayLed(int pos, bool on_off);
bool GetDisplayLedBlink(int pos);
void SetDisplayBrightness(int pos, int brightness);

#endif /* SOURCES_DISPLAYLED_H_ */
