/***************************************************************************//**
  @file     decoder.h
  @brief    decoder header
  @author   Team 2 - Lab de Micros
 ******************************************************************************/

/*******************************************************************************
*							INCLUDE HEADER FILES
******************************************************************************/

#ifndef _DECODER_H_
#define _DECODER_H_

#include <stdbool.h> 
#include <stdint.h>
#include <string.h>


/*******************************************************************************
*				  CONSTANT AND MACRO DEFINITIONS USING #DEFINE
******************************************************************************/
#define ID3_MAX_NUM_CHARS 50


#define DECODED_BUFFER_SIZE 5000  

/*******************************************************************************
 *					ENUMERATIONS, STRUCTURES AND TYPEDEFS
 ******************************************************************************/

typedef enum
{
	DECODER_WORKED,
	DECODER_ERROR,
	DECODER_NO_FILE,
	DECODER_END_OF_FILE,
	DECODER_OVERFLOW
} decoder_return_t;


/*******************************************************************************
 *					FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*
* @brief Initialize the values inside the decoder_t decoder struct
*/
void decoder_MP3DecoderInit(void);

/**
 * @brief This function open the encoded mp3 file, if it was open 
		  close it reset the values and open again
 * @return true if it can open the mp3 file and false if it can not.
 */
bool decoder_MP3LoadFile(const char* filename);

/**
 * @brief If the depth is not bigger than the MAX_DEPTH value 
          and if the file is open and there are bytes to decoder
		  in that case read the mp3 file and find the sync word and
		  want store the next frame info and decode it.
		  if there were no errors decoding, then update the pointers
		  to use in the next decoder instance.
 * @param outBuffer: the buffer were we will store the de decoded data. this has to be short because 
					  with that we insure that it uses 16 bits (if it had been int we can not assure 
					  that because int could be 16 or 32 bits). Anyway it could be uint_16 but with that 
					  we would be ensuring that the data is an unsigned data and we dont know that
 * @param bufferSize: the size of the buffer were we will store the decoded data.
 * @param decodedsamples: The pointer to an uint_16 data to store there the number of samples decoded
 * @return true if it can open the mp3 file and false if it can not.
 */
decoder_return_t decoder_MP3DecodedFrame(short* outBuffer, uint16_t bufferSize, uint16_t* Decodedsamples);


/**
 * @brief get the info of the last frame data and store it inside 
		  the pointer channelCount passed as an argument
 * @param channelCount: pointer to a variable where we will store the number of channels.
 * @return true if the function can get the info of the last frame 
		   and false if it can not (usually because there was no last frame)
 */
bool decoder_MP3GetLastFrameChannelCount(uint8_t* channelCount);

/**
 * @brief get the Data of the mp3's tag.
 * @param title: pointer to the char array where we will store the songs title.
 * @param album: pointer to the char array where we will store the songs album.
 * @param artist: pointer to the char array where we will store the songs artist.
 * @param trackNum: pointer to the uint8_t array where we will store the songs track number in the album.
 * @param year: pointer to the uint8_t array where we will store the album year.
 */
void decoder_MP3GetTagData(char* _title_, char* _album_, char* _artist_, char* _trackNum_, char* _year_);

/**
 * @brief getter of hasID3
 * @return: the value of hasID3, true if the son has ID3 false if it hasnt.
 */
bool decoder_hasID3(void);

/**
 * @brief: Close file if open
 */
bool decoder_shutDown(void);

/**
 * @brief: Get the current file's title it changes the char* passed as a pointer to pointer to char
 * @param title_: is a pointer to the char pointer (array of chars) that forms the title word.
 */
bool decoder_getFileTitle(char ** title_);
/**
 * @brief: Get the current file's album, it changes the char* passed as a pointer to pointer to char
 * @param album_: is a pointer to the char pointer (array of chars) that forms the album word.
 */
bool decoder_getFileAlbum(char** album_);
/**
 * @brief: Get the current file's artist, it changes the char* passed as a pointer to pointer to char
 * @param artist_: is a pointer to the char pointer (array of chars) that forms the artist word.
 */
bool decoder_getFileArtist(char** artist_);
/**
 * @brief: Get the current file's year, it changes the char* passed as a pointer to pointer to char
 * @param year_: is a pointer to the char pointer (array of chars) that forms the year word, because is a number forms with the ascii values (chars)
 */
bool decoder_getFileYear(char** year_);
/**
 * @brief: Get the current file's track number, it changes the char* passed as a pointer to pointer to char
 * @param trackNum_: is a pointer to the char pointer (array of chars) that forms the Track number word, because is a number forms with the ascii values (chars)
 */
bool decoder_getFileTrackNum(char** trackNum_);
#endif /* _DECODER_H_ */
