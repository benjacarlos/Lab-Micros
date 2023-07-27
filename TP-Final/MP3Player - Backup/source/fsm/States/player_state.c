/***************************************************************************/ /**
  @file     add_user.c
  @brief    add user state functions
  @author   Grupo 2 - Lab de Micros
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <fsm/States/player_state.h>
#include <stdbool.h>
#include <string.h>

#include "queue.h"
#include "audio_manager.h"
#include "LCD_GDM1602A.h"

#include "vumeterRefresh.h"

#include "AudioPlayer.h"
#include "ff.h"
#include "file_system_manager.h"
#include "Timer.h"

#define VOLUME_TIME		(5000U)
/*******************************************************************************
 * GLOBAL VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/
static bool showingVolume = false;
static int volumeTimerID = -1;

/*******************************************************************************
 * 	LOCAL FUNCTION DEFINITIONS
 *******************************************************************************/

static void printFileInfo(void);
static void showVolume(void);
static void stopShowingVolume(void);
/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void Player_InitState(void)
{
	printFileInfo();
}

void Player_ToggleMusic(void)
{
	Audio_toggle();
}


void Player_Stop(void)
{
	Audio_stop();
}

void Player_PlayNextSong(void)
{
	Audio_nextFile();
	Audio_selectFile();
	Audio_play();
	printFileInfo();
}

void Player_PlayPreviousSong(void)
{
	Audio_prevFile();
	Audio_selectFile();
	Audio_play();
	printFileInfo();
}

void Player_IncVolume(void)
{
	// algo de mostrar en el display por un tiempo
	Audio_IncVolume();
	showVolume();
}

void Player_DecVolume(void)
{
	// algo de mostrar en el display por un tiempo
	Audio_DecVolume();
	showVolume();
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
static void printFileInfo(void)
{
	char path[50], data[400];
	memset(data, 0x20, 400);
	memset(path, 0x20, 50);
	char * name = Audio_getName();
	char * artist = Audio_getArtist();
	char * album = Audio_getAlbum();
	char * year = Audio_getYear();
	char * gather[] = {"Artista: ", artist, " Album: ", album, " Year: ", year};

	uint16_t len = strlen(name);
	len += (DISPLAY_COLUMNS-(len%DISPLAY_COLUMNS));
	memcpy(path, name, strlen(name));
	LCD_writeShiftingStr(path,  len, 0, MIDIUM);



	len = 0;
	for(int k = 0; k < sizeof(gather)/sizeof(gather[0]); k++)
	{
		memcpy(data + len, gather[k], strlen(gather[k]));
		len += strlen(gather[k]);
	}
	len += (DISPLAY_COLUMNS-(len%DISPLAY_COLUMNS));
	LCD_writeShiftingStr(data,  len, 1, MIDIUM);

}

static void showVolume(void)
{
	if(!showingVolume)
	{
		LCD_clearDisplay();
		volumeTimerID = Timer_AddCallback(stopShowingVolume, VOLUME_TIME, true);
	}
	else
	{
		Timer_Reset(volumeTimerID);
	}

	char str2wrt[11] = "Volumen: --";

	char vol = Audio_getVolume();
	str2wrt[9] = vol/10 != 0? 0x30 + vol/10 : ' ';
	str2wrt[10] = 0x30 + (char)vol%10;

	LCD_writeStrInPos(str2wrt, sizeof(str2wrt)/sizeof(str2wrt[0]), 0, 0);
	showingVolume = true;
}

static void stopShowingVolume(void)
{
	showingVolume = false;
	printFileInfo();
}
