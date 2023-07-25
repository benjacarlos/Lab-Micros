/***************************************************************************/ /**
  @file     spectrometer.h
  @brief    Spectrometer by using an vumeter
  @author   Grupo 5
 ******************************************************************************/
#ifndef SPECTROMETER_H_
#define SPECTROMETER_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "arm_math.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define LOWER_FREQ 80		// Music lower frequency is 80 Hz
#define HIGHER_FREQ 10000	// Music higher frequency is 10 KHz

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

void spectrometer_init();
// Get spectrum from dft
void spectrometer_dft(float32_t * inputSignal, float32_t sampleRate, int lowerFreqBand, int higherFreqBand);
// Save spectrum for print
void spectrometer_write_to_matrix(int * vumeterMatrix);
void spectrometer_draw_display();
void spectrometer_clean_display();

#endif /* SPECTROMETER_H_ */
