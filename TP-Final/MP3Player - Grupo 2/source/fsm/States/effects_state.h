/***************************************************************************/ /**
  @file     effects_state.h
  @brief    Effects state
  @author   Grupo 2 - Lab de Micros
 ******************************************************************************/
#ifndef EFFECTS_STATE_H
#define EFFECTS_STATE_H

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <stdint.h>
#include <stdbool.h>

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
/**
 * @brief Initializes the main effects state
 */
void Effects_InitState(void);

/**
 * @brief 	Shows the next option.
 * 			If the user is setting the bands' gain (custom mode), decreases 1dB the current band's gain.
 */
void Effects_NextOption(void);
/**
 * @brief 	Shows the previous option.
 * 			If the user is setting the bands' gain (custom mode), increases 1dB the current band's gain.
 */
void Effects_PreviousOption(void);

/**
 * @brief	Selects the current option. Adds an event to the event queue of the FSM.
 * 			If the user is setting the bands' gain (custom mode), sets the current band with the value selected and continues to the next band.
 */
void Effects_SelectOption(void);

/**
 * @brief 	If the user is setting the bands' gain (custom mode), returns to the main effects' menu.
 * 			If not, emits a CHANGE_MODE_EV.
 */
void Effects_Back(void);

void Effects_SetEffect(char option);

#endif /* EFFECTS_STATE_H */
