/***************************************************************************/ /**
  @file     esp_comunication.c
  @brief    Controlador de mesajer usando UART
  @author   Grupo 2
 ******************************************************************************/

#include <stdio.h>
#include "esp_comunication.h"

#include "uart.h"

#include "audio_manager.h"
#include "States/effects_state.h"
#include "queue.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define BUFFER_LEN	256

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * PROTOTYPES
 *******************************************************************************/

static void parse_msg(void);

/*******************************************************************************
 * CODE
 *******************************************************************************/


void esp_Init(void)
{
	uart_cfg_t config = {9600, UART_PARITY_NONE, UART_DATA_BITS_8, UART_STOP_BITS_1};
	UART_init(3, config);
	UART_attach_callback(parse_msg);
}

void esp_Send(char code, char * msg, char len)
{
	char var[] = {code,0};
	UART_write_msg(3,&var[0], 1 );
	UART_write_msg(3, msg	,len);
	UART_write_msg(3,&var[1], 1 );
}

void esp_Read(void)
{
	char buffer;
	char len = 0;

	if(!UART_is_rx_msg(3))
	{
		return;
	}

	len = UART_read_msg(3, &buffer, 1);

	if(len == 1)
	{
		printf("%d\n", buffer);
		if(buffer <= 40){
			Audio_setVolume(buffer);
		}
		else{
			switch(buffer)
			{
			case 41: case 42: Audio_toggle(); break;//play/pausa
			case 43: Audio_stop(); break; //stop
			case 44: emitEvent(PREV_SONG_EV); break;//prev
			case 45: emitEvent(NEXT_SONG_EV); break;//next
			case 46: Effects_SetEffect(0);break;//default
			case 47: Effects_SetEffect(1);break;//Rock
			case 48: Effects_SetEffect(2);break;//jazz
			case 49: Effects_SetEffect(3);break;//pop
			case 50: Effects_SetEffect(4);break;//classic
			default: break;
			}
		}

	}

}

static void parse_msg(void)
{
	char buffer;
	char len = 0;
	len = UART_read_msg(3, &buffer, 1);

	if(len == 1)
	{
		printf("%d\n", buffer);
		if(buffer <= 40){
			Audio_setVolume(buffer);
		}
		else{
			switch(buffer)
			{
			case 41: case 42: Audio_toggle(); break;//play/pausa
			case 43: Audio_stop(); break; //stop
			case 44: emitEvent(PREV_SONG_EV); break;//prev
			case 45: emitEvent(NEXT_SONG_EV); break;//next
			case 46: Effects_SetEffect(0);break;//default
			case 47: Effects_SetEffect(1);break;//Rock
			case 48: Effects_SetEffect(2);break;//jazz
			case 49: Effects_SetEffect(3);break;//pop
			case 50: Effects_SetEffect(4);break;//classic
			default: break;
			}
		}

	}
}
