/***************************************************************************//**
  @file     mp3decoder.h
  @brief    ...
  @author   G5
 ******************************************************************************/

#ifndef SOURCES_AUDIO_DECODE_MP3DECODER_H_
#define SOURCES_AUDIO_DECODE_MP3DECODER_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include  <stdbool.h>
#include  <stdint.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define MP3_DECODED_BUFFER_SIZE (4*1152)                                     // maximum frame size if max bitrate is used (in samples)
#define ID3_MAX_FIELD_SIZE      50

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

typedef enum
{
  MP3DECODER_NO_ERROR,
  MP3DECODER_ERROR,
  MP3DECODER_FILE_END,
  MP3DECODER_NO_FILE,
  MP3DECODER_BUFFER_OVERFLOW
} mp3decoder_result_t;

typedef struct
{
    uint16_t    bitRate;
    uint8_t     channelCount;
    uint16_t	sampleRate;
    uint16_t    binitsPerSample;
    uint16_t    sampleCount;
} mp3decoder_frame_data_t;

typedef struct
{
    uint8_t title[ID3_MAX_FIELD_SIZE];
    uint8_t artist[ID3_MAX_FIELD_SIZE];
    uint8_t album[ID3_MAX_FIELD_SIZE];
    uint8_t trackNum[10];
    uint8_t year[10];

} mp3decoder_tag_data_t;

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/


/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*
* @brief Initialices the mp3 decoder driver
*/
void  MP3DecoderInit(void);

/*
* @brief Initialices the mp3 decoder driver
* @param fileptr pointer to the mp3 file
*/
bool  MP3LoadFile(const char* filename);

/*
* @brief Gives the song's tag data like name, artist, etc
* @param data Pointer to object to be filled with info
* @returns True if there is a valid tag
*/
bool MP3GetTagData(mp3decoder_tag_data_t* data);

/**
 * @brief If the depth is not bigger than the MAX_DEPTH value
          and if the file is open and there are bytes to decoder
		  in that case read the mp3 file and find the sync word and
		  want store the next frame info and decode it.
		  if there were no errors decoding, then update the pointers
		  to use in the next decoder instance.
 * @param outBuffer: the buffer were we will store the decoded data. this has to be short because
					  with that we insure that it uses 16 bits (if it had been int we can not assure
					  that because int could be 16 or 32 bits). Anyway it could be uint_16 but with that
					  we would be ensuring that the data is an unsigned data and we dont know that
 * @param bufferSize: the size of the buffer were we will store the decoded data.
 * @param decodedsamples: The pointer to an uint_16 data to store there the number of samples decoded
 * @return true if it can open the mp3 file and false if it can not.
 */
mp3decoder_result_t MP3DecodedFrame(short* outBuffer, uint16_t bufferSize, uint16_t* samplesDecoded);

/**
 * @brief get the info of the last frame data and store it inside
		  the pointer channelCount passed as an argument
 * @param channelCount: pointer to a variable where we will store the number of channels.
 * @return true if the function can get the info of the last frame
		   and false if it can not (usually because there was no last frame)
 */
bool decoder_MP3GetLastFrameChannelCount(uint8_t* channelCount);

/**
 * @brief: Close file if open
 */
bool MP3DecoderShutDown(void);

#endif /* SOURCES_AUDIO_DECODE_MP3DECODER_H_ */
