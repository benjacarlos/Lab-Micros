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
#define DEFAULT_GAIN			GAIN_LV4

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
// Revisar acorde a los filtros correspondientes
typedef enum {
	GAIN_LV1,
	GAIN_LV2,
	GAIN_LV3,
	GAIN_LV4,
	GAIN_LV5,
	GAIN_LV6,
	GAIN_LV7,
	GAIN_LV8
} gains_id_t;

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

void equalizer_init(void);
void equalize_data(float32_t* input, float32_t * output);
void equalizer_set_band_gain (uint8_t band, gains_id_t gain);
uint32_t equalizer_get_band_gain (uint8_t band);

#endif /* EQUALIZER_H_ */
