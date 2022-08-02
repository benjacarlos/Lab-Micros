/***************************************************************************/ /**
  @file     audio_manager.c
  @brief    Audio manager, to wrap file related function and decoding stuff
  @author   Grupo 2 - Lab de Micros
 ******************************************************************************/

/******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <stdint.h>

#include "audio_manager.h"
#include "file_system_manager.h"
#include "memory_manager.h"
#include "AudioPlayer.h"
#include "vumeterRefresh.h"
#include "decoder.h"
#include "equalizer.h"

#include "fsl_common.h"
#include "queue.h"
#include "esp_comunication.h"

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
/******************************************************************************
 *
 ******************************************************************************/
void Audio_init(void)
{
	if(!init)
	{
		Mm_OnConnection(); //Init the SD;
		FileSystem_ScanFiles(); // Build file system tree
		currFile = FileSystem_GetFirstFile();
		maxFile = FileSystem_GetFilesCount();
		init = !init;
	}
}

void Audio_deinit(void)
{
	AudioPlayer_Stop();


	decoder_shutDown();

	Mm_OnDesconection();

	FileSystem_ResetFiles();

	vumeterRefresh_clean_display();

	playing = false;
	init = false;
}

void Audio_nextFile(void)
{
	currFile = FileSystem_GetNextFile(currFile);
}

void Audio_prevFile(void)
{
	currFile = FileSystem_GetPreviousFile(currFile);
}

void Audio_playNextFile(void)
{
	playingFile = FileSystem_GetNextFile(playingFile);

	decoder_MP3LoadFile(playingFile.path);
	/* Primeros dos buffer constante, no hay sonido */
	memset(g_bufferRead, 0x08, sizeof(g_bufferRead));

	/* Podria buscar el sample rate y mandarlo */
	AudioPlayer_LoadSongInfo(g_bufferRead, 44100);

	Audio_updateBuffer();

	char * name = Audio_getCurrentName();
	esp_Send(1, name, strlen(name));
}

void Audio_playPrevFile(void)
{
	playingFile = FileSystem_GetPreviousFile(playingFile);

	decoder_MP3LoadFile(playingFile.path);
	/* Primeros dos buffer constante, no hay sonido */
	memset(g_bufferRead, 0x08, sizeof(g_bufferRead));

	/* Podria buscar el sample rate y mandarlo */
	AudioPlayer_LoadSongInfo(g_bufferRead, 44100);

	Audio_updateBuffer();

	char * name = Audio_getCurrentName();
	esp_Send(1, name, strlen(name));
}

void Audio_selectFile(void)
{
	playingFile = currFile;

	decoder_MP3LoadFile(playingFile.path);
	/* Primeros dos buffer constante, no hay sonido */
	memset(g_bufferRead, 0x08, sizeof(g_bufferRead));

	/* Podria buscar el sample rate y mandarlo */
	AudioPlayer_LoadSongInfo(g_bufferRead, 44100);

	Audio_updateBuffer();

	char * name = Audio_getCurrentName();
	esp_Send(1, name, strlen(name));
}

char * Audio_getCurrentName(void)
{
	char * ret;
	ret = FileSystem_GetFileName(currFile);
	return ret;
}

void Audio_updateBuffer(void)
{
	uint16_t sampleCount = 0;
	uint8_t channelCount = 1;

	AudioPlayer_UpdateBackBuffer(g_bufferRead);

	/* Clean buffers to rewrite */
	memset(g_bufferRead, 0, sizeof(g_bufferRead));
	memset(decoder_buffer, 0, sizeof(decoder_buffer));

	/* Fetch the new frame */
	decoder_return_t check = decoder_MP3DecodedFrame(decoder_buffer, 2*BUFFER_SIZE, &sampleCount);
	/* Get the amount of channels in the frame */
	decoder_MP3GetLastFrameChannelCount(&channelCount);

	/* Scale from int16 to float[-1;1] */
	float coef = 1.0/32768.0;
	for (uint32_t index = 0; index < BUFFER_SIZE; index++)
	{
		effects_in[index] = decoder_buffer[channelCount * index]*coef;
	}

	/* aca van los efectos */
	equalizer_equalize(effects_in, effects_out);

	/* Scale to 12 bits, to fit in the DAC */
	coef = (vol*1.0)/MAX_VOLUME;
	for (uint32_t index = 0; index < BUFFER_SIZE; index++)
	{
		g_bufferRead[index] = (effects_out[index]*coef+1)*2048;
	}

	if (check == DECODER_END_OF_FILE)
	{
		/* Por las dudas completo la salida para tener 0V */
		for (uint32_t index = (sampleCount / channelCount); index < BUFFER_SIZE ; index++)
		{
			g_bufferRead[index] = 2048;
		}

		emitEvent(NEXT_SONG_EV);

	}

	vumeterRefresh_fft(effects_out, 44100.0, 80, 10000);
}

void Audio_showFFT(void)
{
	vumeterRefresh_draw_display();
}

void Audio_updateAll(void)
{
	Audio_updateBuffer();
	Audio_showFFT();
}

void Audio_play(void)
{
	AudioPlayer_Play();
	playing = true;
}

void Audio_toggle(void)
{
	if(playing)
		AudioPlayer_Pause();
	else
		AudioPlayer_Play();

	playing = !playing;
}

void Audio_stop(void)
{
	decoder_MP3LoadFile(currFile.path);
	AudioPlayer_Pause();
	playing = false;
}

char * Audio_getName(void)
{
	char * ret;
	if(!decoder_getFileTitle(&ret))
	{
		ret = FileSystem_GetFileName(playingFile);
	}
	return ret;
}

char * Audio_getArtist(void)
{
	char * ret;
	if(!decoder_getFileArtist(&ret))
	{
		ret = "-";
	}
	return ret;
}

char * Audio_getAlbum(void)
{
	char * ret;
	if(!decoder_getFileAlbum(&ret))
	{
		ret = "-";
	}
	return ret;
}

char * Audio_getYear(void)
{
	char * ret;
	if(!decoder_getFileYear(&ret))
	{
		ret = "-";
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
