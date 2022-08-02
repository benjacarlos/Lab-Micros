/***************************************************************************/ /**
  @file     Player_state.h
  @brief    Player state header.
  @author   Grupo 2 - Lab de Micros
 ******************************************************************************/

#ifndef PLAYER_STATE_H
#define PLAYER_STATE_H

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

void Player_InitState(void);

void Player_ToggleMusic(void);
void Player_Stop(void);

void Player_PlayNextSong(void);
void Player_PlayPreviousSong(void);

void Player_IncVolume(void);
void Player_DecVolume(void);

void Player_Off(void);

#endif /* PLAYER_STATE_H*/
