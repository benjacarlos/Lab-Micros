/*******************************************************************************
  @file     file_selection.c
  @brief    File Selection state functions
  @author   Grupo 5
 ******************************************************************************/

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define TITLE_TIME 5000

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include "audio_manager.h"

#include "file_selection.h"
#include "queue.h"
#include "Timer.h"
#include "LCD_GDM1602A.h"
#include "power_mode_switch.h"

#include "config_main.h"

/*******************************************************************************
 * GLOBAL VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

static bool showingTitle;
static int titleTimerID = -1;

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
/**
 * @brief Show the title of the state on the display. If the user interacts with the system, the title will stop showing.
 */
static void showTitle(void);
/**
 * @brief Stops showing the title of the state on the display.
 */
static void stopShowingTitle(void);

/**
 * @brief Stops showing the title of the state in the display due to a user's interaction.
 * The main reason of creating another function for this is to avoid cancelling a callback (of the timer) inside of it (callback).
 */
static void userInteractionStopsTitle(void);

/**
 * @brief Prints the file's name and artist
 */
static void printFileInfo(void);

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void FileSelection_InitState(void)
{
	showTitle(); 	// Shows the state's title.
	Audio_init();
}

void FileSelection_NextFile(void)
{
	Audio_nextFile();
	printFileInfo();
}

void FileSelection_PreviousFile(void)
{
	Audio_prevFile();
	printFileInfo();
}

void FileSelection_SelectFile(void)
{
	/* Start decoding the file and play the audio player */
	Audio_selectFile();
	Audio_play();
	emitEvent(FILE_SELECTED_EV);
}

void FileSelection_PlayNextSong(void)
{
	Audio_playNextFile();
	Audio_play();
}

void FileSelection_PlayPrevSong(void)
{
	Audio_playPrevFile();
	Audio_play();
}
/*******************************************************************************
 *******************************************************************************
 *                         LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
static void showTitle(void)
{
	LCD_stopMove(0);
	LCD_writeStrInPos("Elegir Archivo  ", 16, 0, 0);
	LCD_clearRow(1);
	showingTitle = true;
	titleTimerID = Timer_AddCallback(&stopShowingTitle, TITLE_TIME, true);
#ifdef DEBUG_PRINT
	printf("\n\nElegir Archivo\n\n");
#endif
}

static void stopShowingTitle(void)
{
	showingTitle = false;
	printFileInfo();
}

static void userInteractionStopsTitle(void)
{
	Timer_Delete(titleTimerID);
	titleTimerID = -1;
	stopShowingTitle();
}

static void printFileInfo(void)
{
	char * name = Audio_getCurrentName();
	char path[50];

	uint8_t len = strlen(name);
	uint8_t mod = len%DISPLAY_COLUMNS;
	len += (DISPLAY_COLUMNS-mod);

	memset(path, 0x20, 50);
	memcpy(path, name, strlen(name));
	LCD_writeShiftingStr(path,  len, 0, MIDIUM);
#ifdef DEBUG_PRINT
	printf("Archivo: %s\n",path);
#endif

}