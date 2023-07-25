/***************************************************************************//**
  @file     mp3decoder.c
  @brief    ...
  @author   G5
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <string.h>
#include <stdbool.h>
#include "mp3decoder.h"
#include "ff.h"
#include "pub/mp3dec.h"
#include "read_id3.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define MP3DECODER_MODE_NORMAL  0
#define MP3_FRAME_BUFFER_BYTES  6913            // MP3 buffer size (in bytes)
#define DEFAULT_ID3_FIELD       0
#define MP3_REC_MAX_DEPTH       5


/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
typedef struct
{
  // Helix structures
  HMP3Decoder   helixDecoder;                                   // Helix MP3 decoder instance
  MP3FrameInfo  lastFrameInfo;                                  // current MP3 frame info

  // MP3 file
  FILE*         mp3File;                                        // MP3 file object
  uint32_t      fileSize;                                       // file size
  uint32_t      bytesRem;                                 // Encoded MP3 bytes remaining to be processed by either offset or decodeMP3
  bool          fileOpened;                                     // true if there is a loaded file
  uint16_t      lastFrameLength;                                // Last frame length

  // MP3-encoded buffer
  uint8_t       mp3FrameBuffer[MP3_FRAME_BUFFER_BYTES];         // buffer for MP3-encoded frames
  uint32_t      top;                                            // current position in frame buffer (points to top)
  uint32_t      bottom;                                         // current position at info end in frame buffer

  // ID3 tag
  bool                  hasID3Tag;                              // True if the file has valid ID3 tag
  mp3decoder_tag_data_t ID3Data;                                // Parsed data from ID3 tag

} mp3decoder_context_t;

/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

/**
 * @brief Closes current file
 */
static void closeFile(void);

/**
 * @brief Opens the given file
 * @param filename  File to be opened
 * @retval True if successfull
 */
static bool openFile(const char * filename);

/**
 * @brief Returns current file size
 */
static size_t FileSize();

/**
 * @brief Reads the requested amount of bytes from the file
 * @param buf Buffer
 * @param count Amount of elements
 * @retval Amount of bytes read
 */
static size_t readFile(void * buf, size_t count);

/*
 * @brief Reads ID3 tag from MP3 file and updates file pointer after tag info
 */
static void readID3Tag(void);

/*
* @brief  This function fills buffer with info encoded in mp3 file and update the pointers
*/
static void ToBuffer(void);

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

static mp3decoder_context_t dec;

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/


void MP3DecoderInit(void)
{
  dec.dec.helixDecoder = MP3InitDecoder();
  dec.mp3File = NULL;
  dec.fileOpened = false;
  dec.bottom = 0;
  dec.top = 0;
  dec.fileSize = 0;
  dec.bytesRem = 0;
  dec.hasID3Tag = false;
}

bool MP3LoadFile(const char* filename)
{
  bool ret = false;

  if (dec.fileOpened)
  {
    // Close previous file and context if necessary
    closeFile();

    // Reset context pointers and vars
    dec.fileOpened = false;
    dec.bottom = 0;
    dec.top = 0;
    dec.fileSize = 0;
    dec.bytesRemaining = 0;
    dec.hasID3Tag = false;
  }

  // Open new file, if successfully opened
  if (openFile(filename))
  {
    dec.fileOpened = true;
    dec.fileSize = currentFileSize();
    dec.bytesRem = dec.fileSize;

    // read ID3 tag and update pointers
    readID3Tag();

    // flush file to buffer
    ToBuffer();

    ret = true;
  }

  return ret;
}

bool MP3GetTagData(mp3decoder_tag_data_t* data)
{
    bool ret = false;
    if (dec.hasID3Tag)
    {
        strcpy(data->album, dec.ID3Data.album);
        strcpy(data->artist, dec.ID3Data.artist);
        strcpy(data->title, dec.ID3Data.title);
        strcpy(data->trackNum, dec.ID3Data.trackNum);
        strcpy(data->year, dec.ID3Data.year);
        ret = true;
    }

    return ret;
}

mp3decoder_result_t MP3DecodedFrame(short* outBuffer, uint16_t bufferSize, uint16_t* Decodedsamples)
{
    // someone is innocent until proven guilty.
	mp3decoder_result_t check = MP3DECODER_NO_ERROR;

    if (!dec.fileOpened)
    {
        check = MP3DECODER_NO_FILE;
    }
    // checks if there is still a part of the file to be decoded
    else if (dec.bytesRem)
    {
        // checks if the conditions are fine
        if ((dec.top > 0) && ((dec.bottom - dec.top) > 0) && (dec.bottom - dec.top < MP3_FRAME_BUFFER_BYTES))
        {
        	memmove(dec.mp3FrameBuffer, dec.mp3FrameBuffer + dec.top, dec.bottom - dec.top);
        	dec.bottom = dec.bottom - dec.top;
        	dec.top = 0;
        }
        // Read data from file
        ToBuffer();
        // search the mp3 header
        int offset = MP3FindSyncWord(dec.mp3FrameBuffer + dec.top, dec.bottom);
        if (offset >= 0)
        {
            // check errors in searching for sync words
            dec.top += offset;
            dec.bytesRem -= offset;  //! subtract garbage
        }
        // check samples in next frame
        MP3FrameInfo nextFrameInfo;
        // with this function we store the nextFrameInfo data in our struct
        int err = MP3GetNextFrameInfo(dec.dec.helixDecoder, &nextFrameInfo, dec.mp3FrameBuffer + dec.top);
        if (err == 0)
        {
            if (nextFrameInfo.outputSamps > bufferSize)
            {
                return MP3DECODER_BUFFER_OVERFLOW;
            }
        }
        // we can now decode a frame
        uint8_t* decPointer = dec.mp3FrameBuffer + dec.top;
        int bytesLeft = dec.bottom - dec.top;
        // the next funtion autodecrements fileSize with bytes decoded and updated bytesLeft
        int res = MP3Decode(dec.helixDecoder, &decPointer, &(bytesLeft), outBuffer, DECODER_NORMAL_MODE);

        // if everithing worked okey
        if (res == ERR_MP3_NONE)
        {
            // we calculate the bytes decoded
            uint16_t decodedBytes = dec.bottom - dec.top - bytesLeft;
            dec.lastFrameLength = decodedBytes;

            // update pointers and the numb of bytes that left to decode.
            dec.top += decodedBytes;
            dec.bytesRem -= decodedBytes;

            // update last frame decoded data
            MP3GetLastFrameInfo(dec.helixDecoder, &(dec.lastFrameInfo));

            // update num of samples decoded
            *Decodedsamples = dec.lastFrameInfo.outputSamps;
            check = MP3DECODER_NO_ERROR;
        }
        else if (res == ERR_MP3_INDATA_UNDERFLOW || res == ERR_MP3_MAINDATA_UNDERFLOW)
        {
            if (dec.bytesRem == 0)
            {
                return MP3DECODER_FILE_END;
            }
        }
        else
        {
            if (dec.bytesRem <= dec.lastFrameLength)
            {
                //if you are here it means that you ended the file
                return MP3DECODER_FILE_END;
            }
            else
            {
                printf("Error");
            }
        }
    }
    else
    {
        // if you are here it means that you ended the file
        check = MP3DECODER_FILE_END;
    }
    return check;
}

bool MP3DecoderGetLastFrameChannelCount(uint8_t* channelCount)
{
    // we assume that there are no last frame.
    bool ret = false;
    if (dec.bytesRem < dec.fileSize)
    {
        *channelCount = dec.lastFrameInfo.nChans;
        ret = true;
    }
    return ret;
}

bool MP3DecoderShutDown(void)
{
	if (dec.fileOpened)
	{
		// Close the file
		f_close(dec.mp3File);

		// Reset pointers and variables
		dec.fileOpened = false;
		dec.bottom = 0;
		dec.top = 0;
		dec.fileSize = 0;
		dec.bytesRem = 0;
		dec.hasID3Tag = false;

	}
	return true;
}
/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
bool openFile(const char * filename)
{
    bool ret = false;
    dec.mp3File = fopen(filename, "rb");
    ret = (dec.mp3File != NULL);

    return ret;
}

void closeFile(void)
{
    if(dec.fileOpened)
    {
        fclose(dec.mp3File);

    }

    dec.fileOpened = false;
}

size_t FileSize()
{
    size_t result = 0;
    if (dec.fileOpened)
    {
        // There may be an error on this line
        result = f_size(dec.mp3File);
    }
    return result;
}




void decoder_readID3Tag(void)
{
    // checks if the file has ID3 Tag inside with the ID3 library
    if (has_ID3_tag(dec.mp3File))
    {
        // if you are here it means that the file has ID3
    	dec.hasID3Tag = true;

    	if (!read_ID3_info(TITLE_ID3, dec.ID3Data.title, ID3_MAX_FIELD_SIZE, dec.mp3File))
    		strcpy(dec.ID3Data.title, DEFAULT_ID3_FIELD);

    	if (!read_ID3_info(ALBUM_ID3, dec.ID3Data.album, ID3_MAX_FIELD_SIZE, dec.mp3File))
    		strcpy(dec.ID3Data.album, DEFAULT_ID3_FIELD);

    	if (!read_ID3_info(ARTIST_ID3, dec.ID3Data.artist, ID3_MAX_FIELD_SIZE, dec.mp3File))
    		strcpy(dec.ID3Data.artist, DEFAULT_ID3_FIELD);

    	if (!read_ID3_info(YEAR_ID3, dec.ID3Data.year, 10, dec.mp3File))
    		strcpy(dec.ID3Data.year, DEFAULT_ID3_FIELD);

    	if (!read_ID3_info(TRACK_NUM_ID3, dec.ID3Data.trackNum, 10, dec.mp3File))
    		strcpy(dec.ID3Data.trackNum, DEFAULT_ID3_FIELD);



        // here we get the size of the tag
        unsigned int tagSize = get_ID3_size(dec.mp3File);

        // we moves the position according to the tagSize
        fileSeek(tagSize);
        dec.bytesRem -= tagSize;

    }
    else
    {
        // if you are here the mp3 file has not ID3 Tag
        f_rewind(dec.mp3File);
    }
}


void ToBuffer()
{
    uint16_t bytesRead;

    // Fill buffer with info in mp3 file
    uint8_t* dest = dec.mp3FrameBuffer + dec.bottom;
    if (MP3_FRAME_BUFFER_BYTES - dec.bottom > 0)
    {
        bytesRead = readFile(dest, (MP3_FRAME_BUFFER_BYTES - dec.bottom));
        // Update bottom pointer
        dec.bottom += bytesRead;
    }

}
