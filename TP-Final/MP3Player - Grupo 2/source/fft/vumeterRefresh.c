#include "arm_math.h"
#include "math.h"
#include "matrix_display.h"
#include <stdbool.h>
#include "fsl_device_registers.h"
#include "vumeterRefresh.h"


#define SAMPLE_LENGTH       FFT_SIZE
#define NUMBER_OF_BANDS     8  
#define VUMETER_HEIGHT      8
#define NOISE               5
#define MAX_AMPLITUDE       50
#define AVERAGE				2

static arm_rfft_fast_instance_f32 rfft_fast_instance;
static float32_t output[SAMPLE_LENGTH];
static float32_t outputFft[SAMPLE_LENGTH / 2];
static int vumeterMatrix[NUMBER_OF_BANDS];
static colors_t auxMatrix[VUMETER_HEIGHT * NUMBER_OF_BANDS];

void vumeterRefresh_init()
{
    arm_rfft_fast_init_f32(&rfft_fast_instance, SAMPLE_LENGTH);
}



int vumeterRefresh_fft(float32_t * inputSignal, float32_t sampleRate, int lowerFreqBand, int higherFreqBand)
{ 
    static volatile char average = 0;
    volatile unsigned int usableBins = (SAMPLE_LENGTH / 2 - 1);

    volatile double inv_binWidth = SAMPLE_LENGTH/sampleRate;
    volatile double base = higherFreqBand / lowerFreqBand;
    volatile double exp = 1.0f / (NUMBER_OF_BANDS - 1.0f );
    volatile float32_t freqMultiplierPerBand = pow(base,exp);

    arm_rfft_fast_f32(&rfft_fast_instance, inputSignal, output, 0);

    for(uint16_t j = 0; j < SAMPLE_LENGTH/2; j++)
    {
    	volatile float32_t temp = output[2*j]*output[2*j] + output[2*j+1]*output[2*j+1];
    	arm_sqrt_f32(temp, &outputFft[j]);
    }

    outputFft[0] = 0; //Removing DC


    volatile float32_t currentBinFreq = lowerFreqBand;
    volatile float32_t nextBinFreq = lowerFreqBand * freqMultiplierPerBand;
    volatile float32_t currentCenterBin;
    volatile float32_t nextCenterBin;
    volatile float32_t lowerBin = 0;
    volatile float32_t higherBin;
    volatile float32_t temp;
    for (size_t i = 0; i < NUMBER_OF_BANDS; i++)
    {

        currentCenterBin = currentBinFreq * inv_binWidth;
        nextCenterBin = nextBinFreq * inv_binWidth;
        lowerBin = i ? higherBin : 0;
        higherBin = (nextCenterBin-currentCenterBin)*0.5 + currentCenterBin;//calcular higherBin!!!

        temp = 0;
        for (size_t j = (int)(lowerBin); ((j < (int)(higherBin))&&(j < usableBins)); j++)
        {
        	temp += (outputFft[j] > NOISE)? outputFft[j] : 0;
        }
        int roundedHeight = (int)(temp/MAX_AMPLITUDE);
        //int roundedHeight = floor((vumeterValues[i]/(higherBin - lowerBin))/1000);
        vumeterMatrix[i] += (roundedHeight > VUMETER_HEIGHT ? VUMETER_HEIGHT : roundedHeight)/AVERAGE;

        currentBinFreq = nextBinFreq;
        nextBinFreq *= freqMultiplierPerBand;
    }
    // Frecuency off FFT shown in display
    average = (average+1)%AVERAGE;
    if(average == 0)
    {
    	vumeterRefresh_write_to_matrix(vumeterMatrix);
    	for(int j = 0; j <  NUMBER_OF_BANDS; j++)
    		 vumeterMatrix[j] = 0;
    }
    return 0;
}

void vumeterRefresh_write_to_matrix(int * vumeterMatrix)
{
    for(int i = 0; i < NUMBER_OF_BANDS; i++)
    {
    	for(int j = 0; j<VUMETER_HEIGHT; j++)
    	{
    		if(vumeterMatrix[i] > j)
			{
				if(j >= 7)
					auxMatrix[ (NUMBER_OF_BANDS - i - 1) + j * 8] = RED;
				else if(j >= 4)
					auxMatrix[ (NUMBER_OF_BANDS - i - 1) + j * 8] = YELLOW;
				else
					auxMatrix[ (NUMBER_OF_BANDS - i - 1) + j * 8] = GREEN;
			}
			else
				auxMatrix[(NUMBER_OF_BANDS - i - 1) + j * 8] = CLEAN;
    	}
    }
    
}

void vumeterRefresh_clean_display()
{
    for(int i = 0; i < NUMBER_OF_BANDS; i++)
    {
    	for(int j = 0; j < VUMETER_HEIGHT; j++)
    	{
			auxMatrix[j*8 + i] = CLEAN;
    	}
    	vumeterMatrix[i] = 0;
    }
    vumeterRefresh_draw_display();
}

void vumeterRefresh_draw_display()
{
    md_writeBuffer(auxMatrix);
}
