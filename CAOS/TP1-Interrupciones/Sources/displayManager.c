/*
 * displayManager.c
 *
 *  Created on: 14 Oct 2020
 *      Author: benja
 */

#include "displayManager.h"
#include "timer.h"
#include <stdbool.h>
#include <stdint.h>
#include "displaySegment.h"

/******************************************************************************
 *									DEFINICIONES
 ******************************************************************************/
#define DISPLAY_SIZE 4
#define STRING_TIME 325 //Delay en ms entre cada shifteo
#define FPS 60 //Frames per second
#define MIN_BRIGHTNESS 0
#define MAX_BRIGHTNESS 3
#define MS_BETWEEN_SYMBOLS ( (1000/FPS)/(DISPLAY_SIZE) )

/******************************************************************************
 *									VARIABLES
 ******************************************************************************/
static const char* current_string;
static int string_pos;
static int display_pos;
static int string_size;
static unsigned int brigthness;
static unsigned char display_counter; //cuantos llamados faltan para pasar al proximo simbolo
static bool initialized = false;

/******************************************************************************
 *								FUNCIONES LOCALES
 ******************************************************************************/



void InitDisplay(void)
{
	if(!initialized)
	{
		InitializeSegmentDisplay();
		ClearDisplay();
		InitializeTimers();
//		SetTimer(DISPLAY, MS_BETWEEN_SYMBOLS/MAX_BRIGHTNESS, &GenerateDisplayEv);
//		SetTimer(MESSAGE,STRING_TIME, &ShiftLeft);//Setteo el timer con la velocidad de movimiento del string.
		DisableTimer(MESSAGE); //Por default asumo que se desea un mensaje que nose mueva a traves del display.
		brigthness = MAX_BRIGHTNESS; //Por default comienza con la intensidad del display al maximo.
		display_counter= 0;
		initialized = true;
	}
}


void ClearDisplay(void)
{
	current_string = "";
	string_pos = 0;
	display_pos = 0;
	string_size = -1;
}

void PrintMessage(const char* string, bool moving_string)
{

	ClearDisplay();
	string_size = INT_FAST8_MAX; //Asumo que el string de largo maximo.

	if(!moving_string)
		{
			DisableTimer(MESSAGE);//Deshabilito mensaje corrido
			string_size = GetStringSize(string);
			if(string_size > DISPLAY_SIZE)
			{
				current_string = string +(string_size - DISPLAY_SIZE);
			}
			else
			{
				current_string = string;
			}
			string_position = 0;
			display_position = 0;

		}
		else
		{
			current_string = string;
			string_position = 0;
			display_position = DISPLAY_SIZE-1; //El mensaje se mueve de derecha a izquierda.
			EnableTimer(MESSAGE);
		}
}

void SetBrightness(unsigned char brightness_factor);

void UpdateDisplay(void);
