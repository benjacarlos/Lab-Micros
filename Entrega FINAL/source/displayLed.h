/*
 * displayLed.h
 *
 *  Created on: 15 Oct 2020
 *      Author: benja
 */

#ifndef DISPLAYLED_H_
#define DISPLAYLED_H_
#include <stdbool.h>

#define LED_DISPLAY_MAX 3
#define LED_MS 3 //1ms

void InitDisplayLeds();
void ResetDisplayLeds();

//Configuraciones del led
void SetLedDisplayBlink(int pos, bool on_off);
void SetDisplayBrightness(int pos, int brightness);

void SetDisplayLed(int pos, bool on_off);
bool GetDisplayLedBlink(int pos);

//Prendo y apago los leds del displey convenientes
void setDisplayLedPos(int pos, bool on_off);
//Verifico caracteristicas del led y updateo el led
void showDisplayLeds(int pos);
#endif /* SOURCES_DISPLAYLED_H_ */
