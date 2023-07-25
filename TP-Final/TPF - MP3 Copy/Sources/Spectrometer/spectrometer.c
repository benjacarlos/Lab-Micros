/***************************************************************************/ /**
  @file     spectrometer.c
  @brief    Spectrometer by using an vumeter
  @author   Grupo 5
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "spectrometer.h"
#include "dft.h"

/*******************************************************************************
 *******************************************************************************
 	 	 	 	 CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 *******************************************************************************
 ******************************************************************************/

// Width and height of frequency bands that hardware can display
#define NUMBER_OF_BANDS     	8
#define SPECTROMETER_HEIGHT     8
// Value of spectrum amplitude consider noise and max amplitude possible between SPECTROMETER_HEIGHT
#define NOISE               5
#define MAX_AMPLITUDE       50
// Average done between values
#define AVERAGE				2

/*******************************************************************************
 *******************************************************************************
                        GLOBAL VARIABLES
 *******************************************************************************
 ******************************************************************************/
// Where we store the needed half of the frequency spectrum
static float32_t outputFft[DFT_OUT_SIZE];
// Spectrometer bins matrix
static int spectrometerMatrix[NUMBER_OF_BANDS];
// Spectrometer display matrix
static colors_t auxMatrix[VUMETER_HEIGHT * NUMBER_OF_BANDS];

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void spectrometer_init()
{
	dft_init();
}

void spectrometer_dft(float32_t * inputSignal, float32_t sampleRate, int lowerFreqBand, int higherFreqBand)
{
	// Get spectrum
	dft_value(inputSignal,outputFft);
	//Removing DC
	outputFft[0] = 0;
	// Variables for scaling spectrum to spectometer display bins resolution
	volatile double base = higherFreqBand / lowerFreqBand;
	volatile double exp = 1.0f / (NUMBER_OF_BANDS - 1.0f );
	volatile float32_t freqMultiplierPerBand = pow(base,exp);	// We use the multiplier (Max/Min)^[1/(Bands-1)] to divide the spectrum range in equal parts
	// Variables of bins spectrometer
	volatile double inv_binWidth = DFT_SIZE/sampleRate;		// inverse width between samples
	volatile unsigned int usableBins = (DFT_OUT_SIZE - 1);
	static volatile char average = 0;
	volatile float32_t currentBinFreq = lowerFreqBand;
	volatile float32_t nextBinFreq = lowerFreqBand * freqMultiplierPerBand;
	volatile float32_t currentCenterBin;	// DFT output bin for currentBinFreq value
	volatile float32_t nextCenterBin;		// DFT output bin for nextBinFreq value
	volatile float32_t lowerBin = 0;		// Starting DFT bin for the currentBinFreq range
	volatile float32_t higherBin;			// Ending DFT bin for the currentBinFreq range
	volatile float32_t temp;
	// Calculate bins values
	for (size_t i = 0; i < NUMBER_OF_BANDS; i++)
	{
		// Get bin where the currentBinFreq and nextBinFreq are
		currentCenterBin = currentBinFreq * inv_binWidth;
	    nextCenterBin = nextBinFreq * inv_binWidth;
	    // Get starting and last bin of currentBinFreq range
	    if (i != 0)
	    	lowerBin = higherBin;
	    else
	    	lowerBin = 0;
	    higherBin = (nextCenterBin-currentCenterBin)*0.5 + currentCenterBin;
	    // Average value of output bins in currentBinFreq range
	    temp = 0;
	    for (size_t j = (int)(lowerBin); ((j < (int)(higherBin))&&(j < usableBins)); j++)
	    {
	    	if (outputFft[j] > NOISE)
	    		temp += outputFft[j];
	    	else
	    		temp += 0;
	    }
	    // Get current display bin amplitude
	    int roundedHeight = (int)(temp/MAX_AMPLITUDE);
	    roundedHeight = roundedHeight/AVERAGE;	// We need to divided by 2 to get real value from the dft given values because the input signal has only real values instead of complex
	    if (roundedHeight > SPECTROMETER_HEIGHT)
	    	spectrometerMatrix[i] += SPECTROMETER_HEIGHT;
	    else
	    	spectrometerMatrix[i] += roundedHeight;
	    // Update bin frequency values
	    currentBinFreq = nextBinFreq;
	    nextBinFreq *= freqMultiplierPerBand;
	}
	// To implement for refresh display function
}

void spectrometer_write_to_matrix(int * vumeterMatrix)
{
    // To implement

}

void spectrometer_draw_display()
{
	// To implement
}

void spectrometer_clean_display()
{
    // To implement
}
