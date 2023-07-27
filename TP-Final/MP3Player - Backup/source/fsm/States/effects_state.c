/***************************************************************************/ /**
  @file     effects_state.c
  @brief    Effects state functions.
  @author   Grupo 2 - Lab de Micros
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "States/effects_state.h"
#include "LCD_GDM1602A.h"
#include "queue.h"
#include "Timer.h"
#include "equalizer.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define TITLE_TIME 2000
#define OPTIONS_COUNT 6
#define OPTION_VALUES_ARRAY_SIZE	NUMBER_OF_BANDS
#define MAX_BAND_GAIN	MAX_GAIN
#define	MIN_BAND_GAIN	-MAX_GAIN


/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
typedef enum
{
	DEFAULT,
	ROCK,
	JAZZ,
	POP,
	CLASSIC,
	CUSTOM
} options_t;
/*******************************************************************************
 * GLOBAL VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/
static bool showingTitle;
static int titleTimerID = -1;
static uint8_t currentOptionIndex = 0;
static char * frequencyBandsTitles [] = {"80Hz Band       ",
										 "160Hz Band      ",
										 "320Hz Band      ",
										 "640Hz Band      ",
										 "1.28kHz Band    ",
										 "2.5kHz Band     ",
										 "5kHz Band       ",
										 "10kHz Band      "};
static bool settingCustom = false;
static uint8_t currentBand = 0;
static int32_t currentBandValue = 0;

int optionValues[5][OPTION_VALUES_ARRAY_SIZE] =
	{{0, 0, 0, 0,  0, 0, 0, 0}, 	//default
	 {0, 0, 1, 3,-10,-2,-1, 3}, 	//rock
	 {0, 0, 2, 5, -6,-2,-1, 2},		//jazz
	 {0, 0, 0, 0,  2, 2, 3,-3},		//pop
	 {0, 0,-1,-6,  0, 1, 1, 3}		//classic
};
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
 * @brief Stops showing the title of the state on the display due to a user's interaction.
 */
static void userInteractionStopsTitle(void);

/**
 * @brief Shows the current option in the display.
 */
static void setCurrentOption(void);
/**
 * @brief Shows the current band and its gain in the display.
 */
static void showCustomBandSetting(void);
/*******************************************************************************
 * FUNCTIONS WITH GLOBAL SCOPE
 ******************************************************************************/

void Effects_InitState(void)
{
	LCD_clearDisplay();
	LCD_stopMove(0);
	showTitle();
	currentOptionIndex = 0;
}

void Effects_NextOption(void)
{
    if (showingTitle)
        userInteractionStopsTitle();
    else if (settingCustom)
    {
    	currentBandValue +=1;
    	if (currentBandValue == MAX_BAND_GAIN +1)
    	{
    		currentBandValue = MIN_BAND_GAIN;
    	}
    	equalizer_set_band_gain(currentBand+1, currentBandValue);
    	showCustomBandSetting();
    }
    else
    {
        uint8_t max = OPTIONS_COUNT - 1;
        if (currentOptionIndex == max - 1)
            currentOptionIndex = 0;
        else
            currentOptionIndex++;
        setCurrentOption();
    }

}

void Effects_PreviousOption(void)
{
    if (showingTitle)
        userInteractionStopsTitle();
    else if(settingCustom)
    {
    	currentBandValue -=1;
		if (currentBandValue == MIN_BAND_GAIN -1)
		{
			currentBandValue = MAX_BAND_GAIN;
		}
		equalizer_set_band_gain(currentBand+1, currentBandValue);
		showCustomBandSetting();
    }
    else
    {
        uint8_t max = OPTIONS_COUNT - 1;
        if (currentOptionIndex == 0)
            currentOptionIndex = max - 1;
        else
            currentOptionIndex--;
        setCurrentOption();
    }
}

void Effects_SelectOption(void)
{
    if (showingTitle)
        userInteractionStopsTitle();
    else if(settingCustom)
    {
    	if (currentBand == OPTION_VALUES_ARRAY_SIZE - 1)
    	{
    		currentBand = 0;
    		Effects_Back();
    	}
    	else
    	{
    		currentBand += 1;
    		currentBandValue = equalizer_get_band_gain(currentBand+1);
    		showCustomBandSetting();
    	}
    }
    else
    {
    	//LCD_clearDisplay();
        if (currentOptionIndex == OPTIONS_COUNT-1)
        {
        	settingCustom = true;
        	currentBand = 0;
        	currentBandValue = equalizer_get_band_gain(currentBand+1);
        	showCustomBandSetting();
        }
        else
        {
        	for (int i = 0; i < OPTION_VALUES_ARRAY_SIZE; i++)
        	{
        		equalizer_set_band_gain(i+1, optionValues[currentOptionIndex][i]);
        	}
        	emitEvent(CHANGE_MODE_EV);
        }
    }

}


void Effects_Back(void)
{
	if (settingCustom)
	{
		settingCustom = false;
		LCD_clearRow(1);
		setCurrentOption();
	}else
	{
		emitEvent(CHANGE_MODE_EV);
	}
}

void Effects_SetEffect(char option)
{
	currentOptionIndex = option;
	for (int i = 0; i < OPTION_VALUES_ARRAY_SIZE; i++)
	{
		equalizer_set_band_gain(i+1, optionValues[currentOptionIndex][i]);
	}
}
/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
static void showTitle(void)
{
	LCD_stopMove(0);
	LCD_stopMove(1);
	//LCD_clearDisplay();
	//LCD_clearRow(0);
	LCD_writeStrInPos("Efectos         ", 16, 0, 0);
	showingTitle = true;
	titleTimerID = Timer_AddCallback(&stopShowingTitle, TITLE_TIME, true);
}

static void stopShowingTitle(void)
{
	showingTitle = false;
	LCD_clearDisplay();
	setCurrentOption();
}

static void userInteractionStopsTitle(void)
{
	Timer_Delete(titleTimerID);
	titleTimerID = -1;
	stopShowingTitle();
}

static void showCustomBandSetting(void)
{
	//LCD_clearDisplay();
	LCD_writeStrInPos(frequencyBandsTitles[currentBand], 16, 0, 0);

	char bandGainText[16] = "                ";
	int writtenChars = sprintf(bandGainText, "%ddB", currentBandValue);
	bandGainText[writtenChars] = ' ';
	LCD_writeStrInPos(bandGainText, 16, 1, 0);
}

static void setCurrentOption(void)
{
    //LCD_clearDisplay();
	LCD_stopMove(0);
    switch (currentOptionIndex)
    {
    case DEFAULT:
		LCD_writeStrInPos("DEFAULT             ", 16, 0, 0);
		break;
    case ROCK:
    	LCD_writeStrInPos("ROCK                ", 16, 0, 0);
        break;
    case JAZZ:
    	LCD_writeStrInPos("JAZZ                ", 16, 0, 0);
        break;
    case POP:
    	LCD_writeStrInPos("POP                 ", 16, 0, 0);
        break;
    case CLASSIC:
		LCD_writeStrInPos("CLASSIC             ", 16, 0, 0);
		break;
    case CUSTOM:
    	LCD_writeStrInPos("CUSTOM              ", 16, 0, 0);
		break;
    }
}

