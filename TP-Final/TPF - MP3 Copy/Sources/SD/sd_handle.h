/***************************************************************************/ /**
  @file    	sd_handle.h
  @brief    SD Memory System
  @author   Grupo 5
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <stdio.h>
#include "fsl_sd.h"


#ifndef SOURCES_SD_SD_HANDLE_H_
#define SOURCES_SD_SD_HANDLE_H_

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
void SDHandle_Init(void);

/*
 * SD present or not
 */
bool SDHandle_IsSDPresent(void);

/*
 *	SD initialization routine
 */
void SDHandle_OnConnection(void);

/*
 * SD deinit routine
 */
void SDHandle_OnDesconection(void);

bool SDHandle_SDConnection(void);

bool SDHandle_SDDesconnection(void);



#endif /* SOURCES_SD_SD_HANDLE_H_ */
