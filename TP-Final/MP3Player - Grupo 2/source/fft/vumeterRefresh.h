/*****************************************************************************
  @file     vumeterRefresh.h
  @author   Grupo 5
 ******************************************************************************/

#ifndef _VUMETER_REFRESH_H
#define _VUMETER_REFRESH_H

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include<stdio.h>
#include<stdint.h>
#include "arm_math.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define FFT_SIZE	1024	// Number of sample use to get good spectral resolution

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

void vumeterRefresh_init();
int vumeterRefresh_fft(float32_t * inputSignal, float32_t sampleRate, int lowerFreqBand, int higherFreqBand);
void vumeterRefresh_write_to_matrix(int * vumeterMatrix);
void vumeterRefresh_draw_display();
void vumeterRefresh_clean_display();

#endif
