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
#include <stdint.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

// Numbers of frequency equalizer filters and levels of gains
#define NUMBER_OF_FILTERS     	8								// Equal to display width
#define GAINS_LEVELS			(MAX_GAIN + MIN_GAIN + 1)		// Equal to display heights
#define MAX_GAIN 				10								// Máx value of positive gains (10 dB)
#define MIN_GAIN 				10								// Min value of negative gains (- 10 dB)

// Default values
#define DEFAULT_GAIN			0 		// 0 dB

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

void equalizer_init(void);
void equalizer_data(float32_t* input, float32_t * output);
void equalizer_set_band_gain (uint8_t band, gains_id_t gain);
uint32_t equalizer_get_band_gain (uint8_t band);

#endif /* EQUALIZER_H_ */
