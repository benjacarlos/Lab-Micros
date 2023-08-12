/*******************************************************************************
  @file     Idle.h
  @brief    Idle state header
  @author   Grupo 5
 ******************************************************************************/

#ifndef IDLE_STATE_H
#define IDLE_STATE_H

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdint.h>

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief Initializes the idle state
 */
void Idle_InitState(void);

/**
 * @brief Stop showing the time when the user interacts with the system.
 */
void Idle_OnUserInteraction(void);

void Idle_Update(void);

#endif /* IDLE_STATE_H */
