/***************************************************************************/ /**
  @file     dft.h
  @brief    Discrete fourier transformation
  @author   Grupo 5
  @commentaries For more information use the following link https://www.keil.com/pack/doc/CMSIS/DSP/html/group__RealFFT.html
 ******************************************************************************/
#ifndef DFT_H_
#define DFT_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "arm_math.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define DFT_SIZE 		1024 			//Number of sample use to get good spectral resolution
#define DFT_OUT_SIZE 	DFT_SIZE/2 		//Number of sample the useful half of the frequency spectrum

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
// Init FFT table for DFT
void dft_init();
// Get FFT value
void dft_value(float32_t *in,float32_t *out);

#endif /* DFT_H_ */
