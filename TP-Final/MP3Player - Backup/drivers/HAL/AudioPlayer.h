/***************************************************************************//**
 @file     AudioPlayer.h
 @brief    Audio Reproductor using the DAC with PDB.
 @author   Grupo 2
******************************************************************************/

#ifndef _AUDIO_PLAYER_H_
#define _AUDIO_PLAYER_H_

/*******************************************************************************
* INCLUDE HEADER FILES
******************************************************************************/
#include <stdint.h>
#include <stdbool.h>

/*******************************************************************************
* ENUMERATIONS AND STRUCTURES AND TYPEDEFS
******************************************************************************/
typedef enum
{
    AP_NO_ERROR,
    AP_ERROR_BB_NOT_FREE
} audioPlayerError;

/*******************************************************************************
* CONSTANT AND MACRO DEFINITIONS USING #DEFINE
******************************************************************************/
#define AUDIO_PLAYER_BUFF_SIZE 1152U//2560U


/*******************************************************************************
* FUNCTION PROTOTYPES WITH GLOBAL SCOPE
******************************************************************************/
/*!
 * @brief Initialize the Audio Player.
 */
void AudioPlayer_Init(void);

/*!
 *@brief Loads a song's info.
 *@param firstSongFrame first song's frame's samples.
 *@param song's sample rate.
 */
void AudioPlayer_LoadSongInfo(uint16_t *firstSongFrame, uint16_t sampleRate);

/*!
 *@brief Updates the sample rate that it's being used.
 *@param sampleRate, the new sample rate.
*/
void AudioPlayer_UpdateSampleRate(uint16_t sampleRate);

/*!
 *@brief Checks if the back buffer is free or not, in order to know if it can be filled again or not yet.
 *@return Indication about tje back buffer's condition: free or not.
*/
bool AudioPlayer_IsBackBufferFree(void);

/*!
 * @brief Updates the back buffer.
 * @param newBackBuffer, new buffer that will fill the back buffer.
 * @return if there was an error.
*/
audioPlayerError AudioPlayer_UpdateBackBuffer(uint16_t *newBackBuffer);

/*!
 * @brief Reproduces audio using the DAC, from the samples previously saved from the current song.
*/
void AudioPlayer_Play(void);

/*!
 * @brief Pauses the audio reproduction.
*/
void AudioPlayer_Pause(void);

/*!
 * @brief Pauses the audio reproduction and goes back to the beginning of the current song,
 *  so that if you chose play afterwards, the song starts from the beginning again.
*/
void AudioPlayer_Stop(void);

/*!
 * @brief A demo mode to test audio reproduction.
*/
void AudioPlayer_DEMOMode(void);

#endif /* _AUDIO_PLAYER_H_ */
