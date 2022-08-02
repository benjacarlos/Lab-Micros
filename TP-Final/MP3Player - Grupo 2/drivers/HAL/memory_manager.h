/***************************************************************************/ /**
  @file     memory_manager.h
  @brief    Memory System header
  @author   Grupo 2 - Lab de Micros
 ******************************************************************************/


#ifndef MEMORY_MANAGER_H_
#define MEMORY_MANAGER_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdbool.h>

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
#define SD_BUFFER_ALIGN_SIZE 	(4U)

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*
 * Init routine
 * cd: a callback to receive and store the scanned files
 */
void Mm_Init(void);

/*
 * SD present or not
 */
bool Mm_IsSDPresent(void);

/*
 *	SD initialization routine
 */
void Mm_OnConnection(void);

/*
 * SD deinit routine
 */
void Mm_OnDesconection(void);

bool Mm_SDConnection(void);

bool Mm_SDDesconnection(void);

#endif /* MEMORY_MANAGER_H_ */
