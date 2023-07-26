/***************************************************************************/ /**
  @file     matrix_leds.h
  @brief    8x8 RGB display hearder
  @author   Grupo 5 - Lab de Micros
 ******************************************************************************/


#ifndef SOURCES_WS2812_MATRIX_LEDS_H_
#define SOURCES_WS2812_MATRIX_LEDS_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdbool.h>
#include <stdint.h>

/***************************************************************************
*	CONSTANT AND MACRO DEFINITIONS
****************************************************************************/
#define MATRIX_WIDTH 8
#define MATRIX_HEIGHT 8
#define MATRIX_SIZE MATRIX_WIDTH*MATRIX_HEIGHT

/***************************************************************************
*	STRUCTS DEFINITIONS
****************************************************************************/
typedef enum { RED, YELLOW, GREEN, CLEAN}colors_t;
typedef struct
{
  bool R;
  bool G;
  bool B;
} pixel_t;

/***************************************************************************
*	GLOBAL FUNCTION DECLARATIONS
****************************************************************************/
/*
 * @brief: Initialize the matrix, preparing FTM0 channel 0 and DMA0 channel 1
 */
void md_Init(void);


/*
 * @brief: Update the matrix buffer, it will be updated in the next dma callback, upto 4ms leter
 * @param: Buffer to write MUST be DISPLAY_SIZE long
 */
void md_writeBuffer(colors_t *buffer);

/*
 * @brief: Create a pixel_t with the given conditions
 * @param: Each pixel (RGB) presence or not
 * @return: The structed pixel_t data
 */
pixel_t md_makeColor(bool R, bool G, bool B);

#endif /* SOURCES_WS2812_MATRIX_LEDS_H_ */
