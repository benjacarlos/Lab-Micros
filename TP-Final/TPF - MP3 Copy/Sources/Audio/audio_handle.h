/***************************************************************************/ /**
  @file     audio_handle.c
  @brief    Audio handle
  @author   Grupo 5
 ******************************************************************************/


#ifndef SOURCES_AUDIO_AUDIO_HANDLE_H_
#define SOURCES_AUDIO_AUDIO_HANDLE_H_

void Audio_init(void);

void Audio_deinit(void);

void Audio_nextFile(void);

void Audio_prevFile(void);

void Audio_selectFile(void);

void Audio_playPrevFile(void);

void Audio_playNextFile(void);

char * Audio_getCurrentName(void);

char * Audio_getArtist(void);

char * Audio_getAlbum(void);

char * Audio_getYear(void);

void Audio_updateAll(void);

void Audio_updateBuffer(void);

void Audio_showFFT(void);

void Audio_play(void);

void Audio_toggle(void);

void Audio_stop(void);


void Audio_IncVolume(void);

void Audio_DecVolume(void);

char Audio_getVolume(void);

void Audio_setVolume(char value);

#endif /* SOURCES_AUDIO_AUDIO_HANDLE_H_ */
