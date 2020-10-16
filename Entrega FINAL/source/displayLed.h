/*
 * displayLed.h
 *
 *  Created on: 15 Oct 2020
 *      Author: benja
 */

#ifndef DISPLAYLED_H_
#define DISPLAYLED_H_
#include <stdbool.h>

void InitLeds();
void ResetLeds();
void LedBlink(int pos, bool on_off);
void SetLed(int pos, bool on_off);
bool GetLedBlink(int pos);
bool GetLedStatus();
void SetBrightness(int pos, int brightness);

#endif /* SOURCES_DISPLAYLED_H_ */
