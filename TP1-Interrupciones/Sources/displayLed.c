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
//Leds del display 1, 2, 3, el 0 no tiene led
#define STATUS0 PORTNUM2PIN(PA,0)//PTA0
#define STATUS1 PORTNUM2PIN(PC,4)//PTC4
   //Leds del board (incluidos en board.h)
//#define PIN_LED_RED     PORTNUM2PIN(PB, 22)// PTB22
//#define PIN_LED_GREEN   PORTNUM2PIN(PE, 26) // PTE26
//#define PIN_LED_BLUE    PORTNUM2PIN(PB,21) // PTB21

enum {DISPLAY_LED, BOARD_LED, WHERE_LED};

#define LED_DISPLAY_MAX 3
#define LED_BOAD_MAX 3
#define LED_MAX LED_DISPLAY_MAX+LED_BOAD_MAX

#define MIN_BRIGHTNESS 0
#define MAX_BRIGHTNESS 3

#define STATUS0_MASK 0x01
#define STATUS1_MASK 0x02
/******************************************************************************
 *									VARIABLES
 ******************************************************************************/
static bool initialized = false;

static bool blinking[LED_DISPLAY_MAX];				//blinking (true) or not (false)
static bool blink_cleared[LED_DISPLAY_MAX];		//status of the blinking (showing:true, not showing: false)
static int brightness[LED_DISPLAY_MAX];			//brightness level, from MIN_BRIGHT to MAX_BRIGHT
static bool curr_displaying[LED_DISPLAY_MAX];			//leds buffer. Initialized at leds_init()
static int bright_counter[LED_DISPLAY_MAX];

//Se ejecuta cada vez que se ejecuta una interupcion
void ledDisplayCallback ();

void setDisplayLedPos(int pos, bool on_off);

void showDisplayLeds(int pos);

//void blinkHandle(int pos);

void brightnessHandle(int pos);

void InitDisplayLeds()
{
	if(!initialized)
	{
		//Display leds
		gpioMode(STATUS0, OUTPUT);
		gpioMode(STATUS1, OUTPUT);

//		//Board leds
//		gpioMode(PIN_LED_RED, OUTPUT);
//		gpioMode(PIN_LED_GREEN, OUTPUT);
//		gpioMode(PIN_LED_BLUE, OUTPUT);

		ResetDisplayLeds(DISPLAY_LED);
//		ResetLeds(BOARD_LED);

		systick_init();
		systick_add_callback(&ledInteruptCallback, COUNTER_FREQ, PERIODIC);

		initialized = true;
	}
}

void ResetDisplayLeds(unsigned int led_here)
{
	if(led_here == DISPLAY_LED)
	{
		for(int i = 0; i < LED_DISPLAY_MAX; i++)
		{
			curr_displaying[i] = false;
			blinking[i] = false;
			blink_cleared[i] = false;
			bright_counter[i] = 0;
			brightness[i] = MAX_BRIGHTNESS;
		}
	}
//	else if(led_here == BOARD_LED)
//	{
//		for(int i = LED_DISPLAY_MAX-1; i < LED_MAX; i++)
//		{
//			curr_displaying[i] = false;
//			blinking[i] = false;
//			blink_cleared[i] = false;
//			bright_counter[i] = 0;
//			brightness[i] = MAX_BRIGHTNESS;
//		}
//	}
}

//void LedBlink(int pos, bool on_off)
//{
//	blinking[pos] = on_off;
//	blink_cleared[pos] = false;
//}

void SetDisplayLed(int pos, bool on_off)
{
	if( pos < LED_DISPLAY_MAX)
		curr_displaying[pos] = on_off;
}

bool GetDisplayLedBlink(int pos)
{
	return blinking[pos];
}

void SetDisplayBrightness(int pos, int brightness)
{
	if( (bright < MAX_BRIGHTNESS) && (bright >= MIN_BRIGHTNESS) )
		brightness[pos] = bright;
}

void ledDisplayCallback (void)
{
	static int pos = 0;
	//Borro el estado en que estaba antes
	setLedpos(pos, false);
	//inc pos para el prox led
	pos = (pos == LED_DISPLAY_MAX-1) ? 0 : pos + 1;
	displayLeds(pos);

}

void setDisplayLedPos(int pos, bool on_off)
{
	pos++; // 1 2 3
	gpioWrite (PIN_DIODE_0, on_off && (pos & STATUS0_MASK));
	gpioWrite (PIN_DIODE_1, on_off && (pos & STATUS1_MASK));
}

void showDisplayLeds(int pos)
{
	if( !blinkHandle(pos) || !brightnessHandle(pos))
		draw_diode(pos, false);
	else
		draw_diode(pos, (int)curr_displaying[pos]);
}


void brightnessHandle(int pos)
{
	bool should_show = true;
	if(brightness[pos] < MAX_BRIGHTNESS)
			if((bright_counter[pos]++) == brightness[pos])
			{
				should_show = false;
				bright_counter[pos] = 0;
			}
		return should_show;
}
