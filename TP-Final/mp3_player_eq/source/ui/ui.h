/*******************************************************************************
  @file     ui.h
  @brief    User Interface Module
  @author   G. Davidov, F. Farall, J. Gaytán, L. Kammann, N. Trozzo
 ******************************************************************************/

#ifndef _UI_H
#define _UI_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "events/events.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief Initializes the UI module.
 */
void uiInit(void);

/**
 * @brief Cycles the UI module with the next user event.
 * @param event     Next event
 */
void uiRun(event_t event);

/*******************************************************************************
 ******************************************************************************/

#endif