/***************************************************************************/ /**
  @file     equalizer.h
  @brief    Music graphic equalizer using filters
  @author   Grupo 5
 ******************************************************************************/
#ifndef EQUALIZER_H_
#define EQUALIZER_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "arm_math.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

// Numbers of frequency equalizer filters and levels of gains
#define NUMBER_OF_FILTERS     	8		// Equal to display width
#define GAINS_LEVELS			8		// Equal to display heights

// Default values
#define DEFAULT_GAIN			NO_GAIN		// 0 dB

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

typedef enum {
	MINUS_10DB,		// -10 dB
	MINUS_6DB,		// -8  dB
	MINUS_4DB,		// -4  dB
	NO_GAIN,		//  0  dB
	PLUS_2DB,		//  2  dB
	PLUS_4DB,		//  4  dB
	PLUS_8DB,		//  8  dB
	PLUS_10DB		//  10 dB
} gains_id_t;

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

void equalizer_init(void);
void equalize_data(float32_t* input, float32_t * output);
void equalizer_set_band_gain (uint8_t band, gains_id_t gain);
uint32_t equalizer_get_band_gain (uint8_t band);

#endif /* EQUALIZER_H_ */
