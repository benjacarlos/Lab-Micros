/***************************************************************************/ /**
  @file     equalizer.c
  @brief    Music graphic equalizer using filters
  @author   Grupo 5
  @commentaries For more information use the following link https://www.keil.com/pack/doc/CMSIS/DSP/html/group__BiquadCascadeDF1.html#ga5563b156af44d1be2a7548626988bf4e
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "equalizer.h"

/*******************************************************************************
 *******************************************************************************
 	 	 	 	 CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 *******************************************************************************
 ******************************************************************************/

// Filters data
#define IIR_STAGES       			3     			// Number of stages per filter
#define IIR_COEFFS       			5     			// Number of coefficients per stages
#define IIR_STATE_VARS   			4     			// Number of state vars
#define BLOCKSIZE 					36				// Block size for the underlying processing is the number of sample that the filter will process
#define IIR_FRAME_SIZE				1152U			// Length of the overall data FRAME
#define NUMBER_OF_BLOCKS	(FRAME_SIZE/BLOCKSIZE)	// Total number of (block to run)/(times the filter is run)

/*******************************************************************************
 *******************************************************************************
                        GLOBAL VARIABLES
 *******************************************************************************
 ******************************************************************************/

/************************ Entire coefficient table ***********************
** Filter band used are calculated with the given frequency range defined
** in spectrometer.h.
** There are 5*3 = 15 coefficients per 6th order Biquad cascade filter.
** The first 15 coefficients correspond to the -10 dB gain setting of band 1;
** the next coefficients correspond to the -8 dB gain setting of band 1;
** and so on.  There are 15*10 = 150 coefficients in total
** for band 1 (gains = -10, -8, -4, 0, 2, 4, 8, 10).
** After this come the 150 coefficients for band 2 and so on.
**
** The coefficients are in Q31 format.
*************************************************************************/
static const q31_t coefTable[NUMBER_OF_FILTERS * IIR_COEFFS * GAINS_LEVELS] = {
	/*80   Hz  band */
	/*160  Hz  band */
	/*350  Hz  band */
	/*650  Hz  band */
	/*1300 Hz  band */
	/*2500 Hz  band */
	/*5    KHz band */
	/*10   KHz band */
};

// Gains table									80 Hz			160 Hz		3650 Hz			650 Hz		1,3 KHz			2,5 KHz		5 KHz			10 KHz
static uint32_t bandsGain[NUMBER_OF_FILTERS]= {DEFAULT_GAIN, DEFAULT_GAIN, DEFAULT_GAIN, DEFAULT_GAIN, DEFAULT_GAIN, DEFAULT_GAIN, DEFAULT_GAIN, DEFAULT_GAIN, };

// Table of filters state variables used by ARM for filtering with DSP module.
static q31_t stateVars [NUMBER_OF_FILTERS][IIR_STATE_VARS * IIR_STAGES];

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void equalizer_init(void)
{
    /* Initialize the state and coefficient buffers for all Biquad cascade IIR sections and gains are set to 0 on setup*/
	// We will use arm_biquad_cascade_df1_init_q31 because in dft we use 32bit floating point
}

void equalize_data(float32_t* input, float32_t * output)
{
}

void equalizer_set_band_gain (uint8_t band, gains_id_t gain)
{
	if (band < NUMBER_OF_FILTERS)
	{
		if (gain < GAINS_LEVELS )
		{
			bandsGain[band] = gain;
			// Initialize filter with new gain
				// To do
		}
	}
}


uint32_t equalizer_get_band_gain (uint8_t band)
{
	uint32_t gain;
	// If band is not valid value then we get gain of first filter
	if (band > NUMBER_OF_FILTERS)
		gain = bandsGain[0];
	else
		gain = bandsGain[band];
	return gain;
}
