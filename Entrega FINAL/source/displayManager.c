/*
 * 	file: displayManager.c
 *  Trabajo Práctico 1 - Interrupciones
 *  GRUPO 5 - Laboratorio de Microprocesadores
 */

/*******************************************************************************
 * 								HEADERS
 ******************************************************************************/

#include "displayManager.h"
#include "timer.h"
#include "timerqueue.h"
#include <stdint.h>
#include "displaySegment.h"
#include "displayLed.h"

/******************************************************************************
 *							  DEFINICIONES
 ******************************************************************************/
#define DISPLAY_SIZE 4
#define STRING_TIME 325 //Delay en ms entre cada shifteo
#define FPS 60 //Frames per second
#define MS_BETWEEN_SYMBOLS ( (1000/FPS)/(DISPLAY_SIZE) )

/*******************************************************************************
 * 							VARIABLES ESTATICAS
 ******************************************************************************/
static const char* current_string;
static int string_pos;
static int display_pos;
static int string_size;
static unsigned char brigthness;
static unsigned char display_counter;
static bool initialized = false;

/******************************************************************************
 *						DECLARO FUNCIONES LOCALES
 ******************************************************************************/
void ledDisplayCallback (void);
unsigned int GetStringSize(const char* str);
void GenerateDisplayEv(void);
void ShiftLeft(void);

/*******************************************************************************
 * 								FUNCIONES
 ******************************************************************************/

void InitDisplay(void)
{
	if(!initialized)
	{
		InitSegmentDisplay();
		InitDisplayLeds();

		ClearDisplay();

		InitializeTimerQueue();
		timerInit();
		timerStart(DISPLAY, MS_BETWEEN_SYMBOLS/MAX_BRIGHTNESS, &GenerateDisplayEv);
		timerStart(MESSAGE,STRING_TIME, &ShiftLeft);//Setteo el timer con la velocidad de movimiento del string.
		timerStart(LED, LED_MS, &ledDisplayCallback);
		timerDisable(MESSAGE); //Por default asumo que se desea un mensaje que nose mueva a traves del display.

		brigthness=MAX_BRIGHTNESS;
		SetBrightness(brigthness); //Por default comienza con la intensidad del display al maximo.
		initialized = true;
		display_counter = 0;
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
			timerDisable(MESSAGE);//Deshabilito mensaje corrido
			string_size = GetStringSize(string);
			if(string_size > DISPLAY_SIZE)
			{
				current_string = string +(string_size - DISPLAY_SIZE);
			}
			else
			{
				current_string = string;
			}
			string_pos = 0;
			display_pos = 0;

		}
		else
		{
			current_string = string;
			string_pos = 0;
			display_pos = DISPLAY_SIZE-1; //El mensaje se mueve de derecha a izquierda.
			timerEnable(MESSAGE);
		}
}

void SetBrightness(unsigned char brightness_factor)
{
	for (int i = 0; i < LED_DISPLAY_MAX; i++)
	{
		SetDisplayBrightness(i, brightness_factor);
	}
	brigthness=brightness_factor;
}

void UpdateDisplay(void)
{
	++display_counter;
	if(display_counter == MAX_BRIGHTNESS)
	{
		display_counter = 0; //Paso al proximo simbolo y reinicio el contador
		string_pos++;
		display_pos++;
		if(display_pos == DISPLAY_SIZE)
		{
			string_pos -= DISPLAY_SIZE;
			display_pos -= DISPLAY_SIZE;
		}
		if(string_pos < 0)
		{
			PrintChar(' ',display_pos); //Imprimo espacio en blanco
		}
		else
		{
			if(string_pos > string_size)
			{
				PrintChar(' ',display_pos);
				if(string_pos == (string_size + DISPLAY_SIZE) ) //Si se mostro todo el mensaje, vuelve a pasarlo.
				{
					string_pos = 0;
					display_pos = DISPLAY_SIZE-1;
				}
			}
			else if(current_string[string_pos] == '\0')
			{
				string_size = string_pos;
				PrintChar(' ',display_pos);
			}
			else
			{
				PrintChar(current_string[string_pos],display_pos);
			}
		}

	}
	else if( display_counter>= brigthness )
	{
		PrintChar(' ',display_pos); //Imprimo espacio en blanco
		return;
	}

}


/*{
	string_pos++;
	display_pos++;
	if(display_pos == DISPLAY_SIZE)
	{
		string_pos -= DISPLAY_SIZE;
		display_pos -= DISPLAY_SIZE;
	}
	if(string_pos < 0)
	{
		PrintChar(' ',display_pos); //Imprimo espacio en blanco
	}
	else
	{
		if(string_pos > string_size)
		{
			PrintChar(' ',display_pos);
			if(string_pos == (string_size + DISPLAY_SIZE) ) //Si se mostro  el mensaje, vuelve a pasarlo.
			{
				string_pos = 0;
				display_pos = DISPLAY_SIZE-1;
			}
		}
		else if(current_string[string_pos] == '\0')
		{
			string_size = string_pos;
			PrintChar(' ',display_pos);
		}
		else
		{
			PrintChar(current_string[string_pos],display_pos);
		}
	}
}*/

void ShiftLeft(void)
{
	string_pos++;
}

void ledDisplayCallback (void)
{
	static int pos = 0;
	//Borro el estado en que estaba antes
	setDisplayLedPos(pos, false);
	//inc pos para el prox led
	pos = (pos == LED_DISPLAY_MAX-1) ? 0 : pos + 1;
	showDisplayLeds(pos);

}


unsigned int GetStringSize(const char* str)
{
	unsigned int size = 0;
	while (str[size++] != '\0');
	return --size;
}

unsigned char GetBrightnees(void){
	return brigthness;
}

void GenerateDisplayEv(void)
{
	pushTimerEvent(DISPLAY);
}

