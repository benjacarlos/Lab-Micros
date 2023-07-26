/***************************************************************************/ /**
  @file     play.c
  @brief    Play State Functions
  @author   Grupo 5
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "play.h"

#include <stdbool.h>
#include <string.h>

#include "../../Queue/ev_queue.h"
#include "Timer.h"
#include "audio_handle.h"
#include "sd_handle.h"

//#include LCD
/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define VOLUME_TIME		(5000U)
#define SPACE_CHARACTER	(0x20)
#define ZERO_CHARACTER	(0x30)

/*******************************************************************************
 * VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

static bool showingVol = false;
static int volumeTimerID = -1;

/*******************************************************************************
 * 	LOCAL FUNCTION DEFINITIONS
 *******************************************************************************/

/**
 * Prints song information
**/
static void printFileInfo(void);
/**
 * Only volume is shown in whole numbers from 00 to 99
**/
static void showVolume(void);
static void stopShowingVolume(void);

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/**
 * Shows song information
**/
void Play_InitState(void)
{
	printFileInfo();
}

/**
 * Pause and play music
**/
void Play_ToggleMusic(void)
{
	Audio_toggle();
}


void Play_Stop(void)
{
	Audio_stop();
}

void Play_NextSong(void)
{
	Audio_nextFile();
	Audio_selectFile();
	Audio_play();
	printFileInfo();
}

void Play_PreviousSong(void)
{
	Audio_prevFile();
	Audio_selectFile();
	Audio_play();
	printFileInfo();
}

/**
 * Increase and show new value of volume if not ask to stop showing it
**/
void Play_IncVolume(void)
{
	Audio_IncVolume();
	showVolume();
}

/**
 * Decrease and show new value of volume if not ask to stop showing it
**/
void Play_DecVolume(void)
{
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
	char path[50], info[400];
	// Fill data to print with blank character in case information is not found
	memset(info, SPACE_CHARACTER, 400);
	memset(path, SPACE_CHARACTER, 50);

	// Get info
	char * name = Audio_getName();
	char * artist = Audio_getArtist();
	char * album = Audio_getAlbum();
	char * year = Audio_getYear();
	char * gather[] = {"Artista: ", artist, " Album: ", album, " Year: ", year};

	// Print name
	uint16_t len = strlen(name);
//	len += (DISPLAY_COLUMNS-(len%DISPLAY_COLUMNS));
	memcpy(path, name, strlen(name));
//	LCD_writeShiftingStr(path,  len, 0, MIDIUM);

	// Print extra data
	len = 0;
	for(int k = 0; k < sizeof(gather)/sizeof(gather[0]); k++)
	{
		memcpy(info + len, gather[k], strlen(gather[k]));
		len += strlen(gather[k]);
	}
//	len += (DISPLAY_COLUMNS-(len%DISPLAY_COLUMNS));
//	LCD_writeShiftingStr(info,  len, 1, MIDIUM);

}

static void showVolume(void)
{
	if(!showingVol)
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
	// Convert number to ASCI value
	str2wrt[9] = vol/10 != 0? ZERO_CHARACTER + vol/10 : ' ';
	str2wrt[10] = ZERO_CHARACTER + (char)vol%10;

	LCD_writeStrInPos(str2wrt, sizeof(str2wrt)/sizeof(str2wrt[0]), 0, 0);
	showingVol = true;
}

static void stopShowingVolume(void)
{
	showingVol = false;
	printFileInfo();
}
