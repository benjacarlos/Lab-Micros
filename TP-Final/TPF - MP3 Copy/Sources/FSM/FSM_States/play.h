/***************************************************************************/ /**
  @file     play.h
  @brief    Play State Header.
  @author   Grupo 5
 ******************************************************************************/

#ifndef PLAY_H
#define PLAY_H

/*******************************************************************************
 * GLOBAL FUNCTIONS PROTOTYPES
 ******************************************************************************/

void Play_InitState(void);

void Play_ToggleMusic(void);
void Play_Stop(void);

void Play_NextSong(void);
void Play_PreviousSong(void);

void Play_IncVolume(void);
void Play_DecVolume(void);

void Play_Off(void);

#endif /* PLAY_H*/
