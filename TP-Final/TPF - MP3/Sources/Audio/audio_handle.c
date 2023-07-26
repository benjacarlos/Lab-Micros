/***************************************************************************/ /**
  @file     audio_handle.c
  @brief    Audio handle
  @author   Grupo 5
 ******************************************************************************/


/******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdint.h>

#include "audio_handle.h"
#include "sd_file.h"
#include "sd_handle.h"
#include "AudioPlay.h"
//#include "vumeterRefresh.h"
#include "mp3decoder.h"
//#include "equalizer.h"

#include "fsl_common.h"
#include "ev_queue.h"


/******************************************************************************
 * DEFINES
 ******************************************************************************/

#define BUFFER_SIZE (AUDIO_PLAYER_BUFF_SIZE)
#define MAX_VOLUME	(40U)

/*******************************************************************************
 * LOCAL VARIABLES
 ******************************************************************************/
static bool playing = false;
static bool init = false;

static Mp3File_t currFile;
static Mp3File_t playingFile;

static int maxFile = 0;

SDK_ALIGN(static uint16_t g_bufferRead[BUFFER_SIZE] , SD_BUFFER_ALIGN_SIZE);
SDK_ALIGN(static short decoder_buffer[2*BUFFER_SIZE], SD_BUFFER_ALIGN_SIZE);
static float effects_in[BUFFER_SIZE], effects_out[BUFFER_SIZE];

static uint8_t vol = 15;
static char vol2send = 15 + 40;

static mp3decoder_tag_data_t* AudioTagData;
/******************************************************************************
 *
 ******************************************************************************/
void Audio_init(void)
{
	if(!init)
	{
		SDHandle_OnConnection(); //Init the SD;
		SD_File_Scan(); // Build file system tree
		currFile = SD_File_GetFirst();
		maxFile = SD_File_GetFilesCount();
		init = !init;
	}
}

void Audio_deinit(void)
{
	AudioPlayer_Stop();

	MP3DecoderShutDown();

	SDHandle_OnDesconection();

	SD_File_ResetFiles();

//	xx vumeterRefresh_clean_display();

	playing = false;
	init = false;
}

void Audio_nextFile(void)
{
	currFile = SD_File_GetNext(currFile);
}

void Audio_prevFile(void)
{
	currFile = SD_File_GetPrevious(currFile);
}

void Audio_playNextFile(void)
{
	playingFile = SD_File_GetNext(playingFile);

	MP3LoadFile(playingFile.path);
	/* Primeros dos buffer constante, no hay sonido */
	memset(g_bufferRead, 0x08, sizeof(g_bufferRead));

	/* Podria buscar el sample rate y mandarlo */
	AudioPlay_LoadSongInfo(g_bufferRead, 44100);

	Audio_updateBuffer();

}

void Audio_playPrevFile(void)
{
	playingFile = SD_File_GetPrevious(playingFile);

	MP3LoadFile(playingFile.path);
	/* Primeros dos buffer constante, no hay sonido */
	memset(g_bufferRead, 0x08, sizeof(g_bufferRead));

	/* Podria buscar el sample rate y mandarlo */
	AudioPlay_LoadSongInfo(g_bufferRead, 44100);

	Audio_updateBuffer();

}

void Audio_selectFile(void)
{
	playingFile = currFile;

	MP3LoadFile(playingFile.path);
	/* Primeros dos buffer constante, no hay sonido */
	memset(g_bufferRead, 0x08, sizeof(g_bufferRead));

	/* Podria buscar el sample rate y mandarlo */
	AudioPlay_LoadSongInfo(g_bufferRead, 44100);

	Audio_updateBuffer();

}

char * Audio_getCurrentName(void)
{
	char * ret;
	ret = SD_File_GetName(currFile);
	return ret;
}

void Audio_updateBuffer(void)
{
	uint16_t sampleCount = 0;
	uint8_t channelCount = 1;

	AudioPlay_UpdateBackBuffer(g_bufferRead);

	/* Clean buffers to rewrite */
	memset(g_bufferRead, 0, sizeof(g_bufferRead));
	memset(decoder_buffer, 0, sizeof(decoder_buffer));

	/* Fetch the new frame */
	mp3decoder_result_t check = MP3DecodedFrame(decoder_buffer, 2*BUFFER_SIZE, &sampleCount);
	/* Get the amount of channels in the frame */
	MP3DecoderGetLastFrameChannelCount(&channelCount);

	/* Scale from int16 to float[-1;1] */
	float coef = 1.0/32768.0;
	for (uint32_t index = 0; index < BUFFER_SIZE; index++)
	{
		effects_in[index] = decoder_buffer[channelCount * index]*coef;
	}

	/* aca van los efectos */
//	xx equalizer_equalize(effects_in, effects_out);

	/* Scale to 12 bits, to fit in the DAC */
	coef = (vol*1.0)/MAX_VOLUME;
	for (uint32_t index = 0; index < BUFFER_SIZE; index++)
	{
		g_bufferRead[index] = (effects_out[index]*coef+1)*2048;
	}

	if (check == MP3DECODER_FILE_END)
	{
		/* Por las dudas completo la salida para tener 0V */
		for (uint32_t index = (sampleCount / channelCount); index < BUFFER_SIZE ; index++)
		{
			g_bufferRead[index] = 2048;
		}

		emitEvent(NEXT_SONG_EV);

	}

//	xx vumeterRefresh_fft(effects_out, 44100.0, 80, 10000);
}

void Audio_showFFT(void)
{
//	xx vumeterRefresh_draw_display();
}

void Audio_updateAll(void)
{
	Audio_updateBuffer();
	Audio_showFFT();
}

void Audio_play(void)
{
	AudioPlay_Play();
	playing = true;
}

void Audio_toggle(void)
{
	if(playing)
		AudioPlay_Pause();
	else
		AudioPlay_Play();

	playing = !playing;
}

void Audio_stop(void)
{
	MP3LoadFile(currFile.path);
	AudioPlay_Pause();
	playing = false;
}

char * Audio_getName(void)
{
	char * ret;
	if(!MP3GetTagData(AudioTagData)) //If no id3 read file name
	{
		ret = SD_File_GetFileName(playingFile);
	}
	else
	{
		ret = AudioTagData->title;
	}
	return ret;
}

char * Audio_getArtist(void)
{
	char * ret;
	if(!MP3GetTagData(AudioTagData))
	{
		ret = '-';
	}
	else
	{
		ret = AudioTagData->artist;
	}
	return ret;
}

char * Audio_getAlbum(void)
{
	char * ret;
	if(!MP3GetTagData(AudioTagData))
		{
			ret = '-';
		}
		else
		{
			ret = AudioTagData->album;
		}
		return ret;
	return ret;
}

char * Audio_getYear(void)
{
	char * ret;
	if(!MP3GetTagData(AudioTagData))
		{
			ret = '-';
		}
		else
		{
			ret = AudioTagData->year;
		}
		return ret;
}

void Audio_IncVolume(void)
{
	vol += (vol >= MAX_VOLUME)? 0 : 1;
	vol2send = vol+40;
	esp_Send(2, &vol2send, 1);
}

void Audio_DecVolume(void)
{
	vol -= (vol > 0) ? 1 : 0;
	vol2send = vol+40;
	esp_Send(2, &vol2send, 1);
}

char Audio_getVolume(void)
{
	return (char)vol;
}

void Audio_setVolume(char value)
{
	if(vol <= 40 && vol >=0)
	{
		vol = value;
	}
}
