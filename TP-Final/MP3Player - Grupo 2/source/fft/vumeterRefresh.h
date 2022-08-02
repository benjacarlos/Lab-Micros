#ifndef _VUMETER_REFRESH_H
#define _VUMETER_REFRESH_H
#include<stdio.h>
#include<stdint.h>
#include "arm_math.h"

#define FFT_SIZE	1024

void vumeterRefresh_init();
int vumeterRefresh_fft(float32_t * inputSignal, float32_t sampleRate, int lowerFreqBand, int higherFreqBand);
void vumeterRefresh_write_to_matrix(int * vumeterMatrix);
void vumeterRefresh_draw_display();
void vumeterRefresh_clean_display();

#endif
/**
 *
 * End of file.
 */
