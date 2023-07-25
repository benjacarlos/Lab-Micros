/***************************************************************************/ /**
  @file     dft.c
  @brief    FSM functions.
  @author   Grupo 5
 ******************************************************************************/

/*******************************************************************************
 *******************************************************************************
 	 	 	 	 	 	 	 INCLUDE HEADER FILES
 *******************************************************************************
 ******************************************************************************/
#include "dft.h"

/*******************************************************************************
 *******************************************************************************
 	 	 	 	 CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 *******************************************************************************
 ******************************************************************************/

#define SAMPLE_SIZE DFT_SIZE

/*******************************************************************************
 *******************************************************************************
                        GLOBAL VARIABLES
 *******************************************************************************
 ******************************************************************************/

// Structure for floating-point RFFT/RIFFT function (DFT floating point function)
static arm_rfft_fast_instance_f32 dft_fast_instance;
// To store DFT table output
static float32_t output_dft[SAMPLE_SIZE];

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

// Initialization Real DFT table
void dft_init()
{
	arm_rfft_fast_init_f32(dft_fast_instance,SAMPLE_SIZE);
}

// Gives spectrum
void dft_value(float32_t *in,float32_t *out)
{
	arm_rfft_fast_f32(dft_fast_instance,in,output_dft,0);
	// We need to reshape the array to obtain half of the frequency spectrum in no complex format
	for(uint16_t j = 0; j < DFT_OUT_SIZE; j++)
	{
		// Absolute value of complex number given
		volatile float32_t temp = output_dft[2*j]*output_dft[2*j] + output_dft[2*j+1]*output_dft[2*j+1];
	    arm_sqrt_f32(temp, &out[j]);
	}
}
