/*******************************************************************************
  @file     file_selection_state.h
  @brief   	File Selection state header
  @author   Grupo 2 - Lab de Micros
 **********/
#ifndef FILE_SELECTION_STATE_H
#define FILE_SELECTION_STATE_H

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdint.h>

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief Initialization of the state.
 */
void FileSelection_InitState(void);
/**
 * @brief Gets the next file.
 */
void FileSelection_NextFile(void);
/**
 * @brief Gets the previous file.
 */
void FileSelection_PreviousFile(void);

/*
 * @brief	Selects the current file.
 */
void FileSelection_SelectFile(void);

/*
 * @brief Start playing the next song in the background
 */
void FileSelection_PlayNextSong(void);

void FileSelection_PlayPrevSong(void);

#endif //FILE_SELECTION_STATE_H
