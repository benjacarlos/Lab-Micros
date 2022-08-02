/***************************************************************************/ /**
  @file     App.c
  @brief    Application functions
  @author   Nicolás Magliola
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#include "fsl_common.h"

#include "queue.h"
#include "fsm_table.h"

#include "LCD_GDM1602A.h"
#include "memory_manager.h"


#include "gpio.h"
#include "board.h"

#include "SysTick.h"
#include "queue.h"
#include "Timer.h"

#include "matrix_display.h"
#include "AudioPlayer.h"
#include "vumeterRefresh.h"
#include "decoder.h"

#include "button.h"
#include "encoder.h"
#include "power_mode_switch.h"
#include "time_service.h"
#include "equalizer.h"
#include "esp_comunication.h"

#include "States/idle_state.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************/
void getEvents(void)
{
	static bool Lcd_Done = false;
	if(!Lcd_Done && LCD_isInit())
	{
		Lcd_Done = true;
		emitEvent(START_EV);
	}

	if(Mm_SDConnection())
	{
		emitEvent(SD_IN_EV);
	}
	if(Mm_SDDesconnection())
	{
		emitEvent(SD_OUT_EV);
	}

	if (AudioPlayer_IsBackBufferFree())
	{
		emitEvent(FILL_BUFFER_EV);
	}

	/* BUTTONS EVENTS */
	if(wasTap(PIN_SW_A))
	{
		emitEvent(PREV_EV);
	}
	if(wasTap(PIN_SW_B))
	{
		emitEvent(PP_EV);
	}
	if(wasTap(PIN_SW_C))
	{
		emitEvent(STOP_EV);
	}
	if(wasTap(PIN_SW_D))
	{
		emitEvent(NEXT_EV);
	}

	/* ENCODER EVENTS */
	if(wasTap(ENCODER_SW))
	{
		emitEvent(ENCODER_PRESS_EV);
	}
	if(wasLkp(ENCODER_SW))
	{
		emitEvent(ENCODER_LKP_EV);
	}

	int8_t move = Encoder_GetMove();
	if(move != 0)
	{
		emitEvent(move > 0? ENCODER_RIGHT_EV:ENCODER_LEFT_EV);
	}

	esp_Read();

}

STATE * currentState;

/* Función que se llama 1 vez, al comienzo del programa */
void App_Init(void)
{
	PowerMode_Init();

	SysTick_Init();
	Timer_Init();

	Mm_Init();	  //Memory manager
	LCD_Init();   //LCD Driver
	md_Init();	  //NeoPixel matrix

	decoder_MP3DecoderInit(); // Init decoder

	AudioPlayer_Init();	//Audio Player
	vumeterRefresh_init(); // FFT
	equalizer_init();

	TimeService_Init(Idle_UpdateTime);

	Encoder_Init();
	buttonsInit();
	buttonConfiguration(PIN_SW_A, LKP, 20); //20*50=1seg
	buttonConfiguration(PIN_SW_B, LKP, 20);
	buttonConfiguration(PIN_SW_C, LKP, 20);
	buttonConfiguration(PIN_SW_D, LKP, 20);
	buttonConfiguration(ENCODER_SW, LKP, 20);

	esp_Init();
	initQueue();
	currentState = FSM_GetInitState();

	gpioMode(TP, OUTPUT);
	gpioMode(TP2, OUTPUT);

	gpioWrite(TP, false);
	gpioWrite(TP2, false);
}

void App_Run(void)
{

	getEvents();
	if(!queueIsEmpty())
	{
		currentState = fsm(currentState, getEvent());
	}
}


