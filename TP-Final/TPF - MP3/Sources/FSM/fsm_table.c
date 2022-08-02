/***************************************************************************/ /**
  @file     fsm_Table.c
  @brief    FSM Table
  @author   Grupo
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <stdio.h>
#include "fsm.h"


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
static void do_nothing(void);

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
/*Foward Declarations*/
extern STATE init[];
extern STATE idle[];	//saving power (sleep mode)
extern STATE effects[];	//equalizer
extern STATE play[];	//play song
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
	{PP_EV,					idle, 					Idle_StartUp},
	{NEXT_EV,				idle, 					do_nothing},
	{PREV_EV,				idle, 					do_nothing},
	{STOP_EV,				idle, 					do_nothing},

	{ENCODER_PRESS_EV,		idle, 					do_nothing},
	{ENCODER_RIGHT_EV,		idle, 					do_nothing},
	{ENCODER_LEFT_EV,		idle, 					do_nothing},

	{START_EV,				file_selection, 		FileSelection_InitState},
	{SD_IN_EV, 				idle, 					Idle_StartUp},

	{FIN_TABLA, 			idle, 					do_nothing}
};

 /*** Play State ***/
 STATE play[] =
 {
 	{PP_EV, 				play, 					Play_ToggleMusic}, //play pausa
 	{STOP_EV, 				play, 					Play_Stop},
 	{NEXT_EV, 				play, 					Play_PlayNextSong},
 	{PREV_EV, 				play, 					Play_PlayPreviousSong},

 	{ENCODER_PRESS_EV,		file_selection, 		FileSelection_InitState},
 	{ENCODER_RIGHT_EV,		play, 					Play_VolumeInc},
 	{ENCODER_LEFT_EV,		play,					Play_VolumeDec},

 	{ENCODER_LKP_EV,		idle, 					Idle_InitState}, //sleep

 	{SD_OUT_EV, 			idle, 					Idle_InitState},
 	{TIMEOUT_EV,			idle,	 				Idle_InitState},

 	{FILL_BUFFER_EV, 		play,					Audio_updateAll},

	{NEXT_SONG_EV, 			play,					Play_PlayNextSong},
	//maybe para node (?
	//{PREV_SONG_EV, 			play,					Play_PlayPreviousSong},


 	{FIN_TABLA, 			play, 				do_nothing}
 };

 /*** File Selection State ***/
 STATE file_selection[] =
 {
 	{PP_EV, 				file_selection, 		FileSelection_SelectFile},
 	{NEXT_EV, 				file_selection, 		FileSelection_NextFile},
 	{PREV_EV, 				file_selection,			FileSelection_PreviousFile},

 	{ENCODER_PRESS_EV,		effects, 				Effects_InitState}, //Change mode
 	{ENCODER_RIGHT_EV,		file_selection, 		FileSelection_NextFile},
 	{ENCODER_LEFT_EV,		file_selection,			FileSelection_PreviousFile},

 	{ENCODER_LKP_EV,		idle, 					Idle_InitState}, //turn off
 	/*LKP = Long Key Press*/
 	{SD_OUT_EV, 			idle, 					Idle_InitState},
 	{TIMEOUT_EV,			idle,					Idle_InitState},

 	{FILE_SELECTED_EV, 		play, 					Play_InitState},

 	{FILL_BUFFER_EV, 		file_selection,			Audio_updateAll},
 	{NEXT_SONG_EV, 			file_selection,			FileSelection_PlayNextSong},

 	{FIN_TABLA, 			file_selection, 		do_nothing}
 };

/*** Equalizador State ***/

STATE effects[] =
{
	{PP_EV,					effects, 				Effects_SelectOption},
	{STOP_EV, 				effects,				Effects_Back},
	{NEXT_EV, 				effects, 				Effects_NextOption},
	{PREV_EV, 				effects,				Effects_PreviousOption},

	{ENCODER_PRESS_EV,		play,					Play_InitState}, // Change mode
	{ENCODER_RIGHT_EV,		effects, 				Effects_NextOption},
	{ENCODER_LEFT_EV,		effects,				Effects_PreviousOption},

	{ENCODER_LKP_EV,		idle, 					Idle_InitState}, // turn off

	{TIMEOUT_EV, 			idle, 					Idle_InitState},
	{SD_OUT_EV, 			idle, 					Idle_InitState},
	//que no cambie de estado al seleccionar, sino que pueda ir probando en el menu de efectos
	//{CHANGE_MODE_EV,		play,					Play_InitState},

	{FILL_BUFFER_EV, 		effects,			    Audio_updateAll},
	{NEXT_SONG_EV, 			effects,			    Effects_PlayNextSong},

	{FIN_TABLA, 			effects, 				do_nothing}
};







/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
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
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
/*Dummy function*/
static void do_nothing(void)
{
	return;
}

