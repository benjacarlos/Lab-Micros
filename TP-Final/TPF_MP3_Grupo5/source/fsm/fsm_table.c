/*******************************************************************************
  @file     fsm_Table.c
  @brief    FSM Table
  @author   Grupo 5
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <fsm/States/effects.h>
#include <stdio.h>
#include "fsm.h"
#include "fsm_table.h"
#include "States/init.h"
#include "States/idle.h"
#include "States/file_selection.h"
#include "States/player.h"
#include "States/effects.h"

#include "audio_manager.h"

#include "queue.h"

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

static void do_nothing(void);

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

extern STATE init[];
extern STATE idle[];
extern STATE effects[];
extern STATE player[];
extern STATE file_selection[];


/*** tablas de estado ***/

/*** Init State ***/

STATE init[]=
{
	{START_EV,				idle, 					Init_StartAll},
	{FIN_TABLA,				init,					do_nothing}
};

/*** Idle State ***/

 STATE idle[]=
{
	{PP_EV,					idle, 					Idle_OnUserInteraction},
	{NEXT_EV,				idle, 					Idle_OnUserInteraction},
	{PREV_EV,				idle, 					Idle_OnUserInteraction},
	{STOP_EV,				idle, 					Idle_OnUserInteraction},

	{ENCODER_PRESS_EV,		idle, 					Idle_OnUserInteraction},
	{ENCODER_RIGHT_EV,		idle, 					Idle_OnUserInteraction},
	{ENCODER_LEFT_EV,		idle, 					Idle_OnUserInteraction},

	{START_EV,				file_selection, 		FileSelection_InitState},
	{SD_IN_EV, 				idle, 					Idle_OnUserInteraction},
  	{FIN_TABLA, 			idle, 					do_nothing}
};

/*** Equalizador State ***/

STATE effects[] =
{
	{PP_EV,					effects, 				Effects_SelectOption},
	{STOP_EV, 				effects,				Effects_Back},
	{NEXT_EV, 				effects, 				Effects_NextOption},
	{PREV_EV, 				effects,				Effects_PreviousOption},

	{ENCODER_PRESS_EV,		player,					Player_InitState}, // Change mode
	{ENCODER_RIGHT_EV,		effects, 				Effects_NextOption},
	{ENCODER_LEFT_EV,		effects,				Effects_PreviousOption},

	{ENCODER_LKP_EV,		idle, 					Idle_InitState}, // turn off

	{TIMEOUT_EV, 			idle, 					Idle_InitState},	// return to default menu because of inactivity
	{SD_OUT_EV, 			idle, 					Idle_InitState},
	{CHANGE_MODE_EV,		player,					Player_InitState},

	{FILL_BUFFER_EV, 		effects,			    Audio_updateAll},
	{NEXT_SONG_EV, 			effects,			    FileSelection_PlayNextSong},
	{PREV_SONG_EV, 			effects,			    FileSelection_PlayPrevSong},

	{FIN_TABLA, 			effects, 				do_nothing}
};


/*** File Selection State ***/
STATE file_selection[] =
{
	{PP_EV, 				file_selection, 		FileSelection_SelectFile},
	{NEXT_EV, 				file_selection, 		FileSelection_NextFile},
	{PREV_EV, 				file_selection,			FileSelection_PreviousFile},

	{ENCODER_PRESS_EV,		effects, 				Effects_InitState}, // Change mode
	{ENCODER_RIGHT_EV,		file_selection, 		FileSelection_NextFile},
	{ENCODER_LEFT_EV,		file_selection,			FileSelection_PreviousFile},

	{ENCODER_LKP_EV,		idle, 					Idle_InitState}, // turn off
	/*LKP = Long Key Press*/
	{SD_OUT_EV, 			idle, 					Idle_InitState},
	{TIMEOUT_EV,			idle,					Idle_InitState},	// return to default menu because of inactivity

	{FILE_SELECTED_EV, 		player, 				Player_InitState},

	{FILL_BUFFER_EV, 		file_selection,			Audio_updateAll},
	{NEXT_SONG_EV, 			file_selection,			FileSelection_PlayNextSong},
	{PREV_SONG_EV, 			file_selection,			FileSelection_PlayPrevSong},

	{FIN_TABLA, 			file_selection, 		do_nothing}
};

/*** Player State ***/
STATE player[] =
{
	{PP_EV, 				player, 				Player_ToggleMusic}, // play pausa
	{STOP_EV, 				player, 				Player_Stop},
	{NEXT_EV, 				player, 				Player_PlayNextSong},
	{PREV_EV, 				player, 				Player_PlayPreviousSong},

	{ENCODER_PRESS_EV,		file_selection, 		FileSelection_InitState},
	{ENCODER_RIGHT_EV,		player, 				Player_IncVolume},
	{ENCODER_LEFT_EV,		player,					Player_DecVolume},

	{ENCODER_LKP_EV,		idle, 					Idle_InitState}, // turn off

	{SD_OUT_EV, 			idle, 					Idle_InitState},
	{TIMEOUT_EV,			idle,	 				Idle_InitState},	// return to default menu because of inactivity

	{FILL_BUFFER_EV, 		player,					Audio_updateAll},
	{NEXT_SONG_EV, 			player,					Player_PlayNextSong},
	{PREV_SONG_EV, 			player,					Player_PlayPreviousSong},

	{FIN_TABLA, 			player, 				do_nothing}
};


/*******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 ******************************************************************************/

//========interfaz=================
STATE *FSM_GetInitState(void)
{
	 return(init);
}


///=========Rutinas de accion===============
void FSM_StartInitState()
{
}

/*******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 ******************************************************************************/

/*Dummy function*/
static void do_nothing(void)
{
	volatile char foo = 0;
	foo++;
}
