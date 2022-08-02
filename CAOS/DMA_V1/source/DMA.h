/*
 * DMA.h
 *
 *  Created on: 22 nov. 2020
 *      Author: Juan Martin
 */

#ifndef DMA_H_
#define DMA_H_

/*Includes*/
#include <stdbool.h>
#include <stdint.h>

/*Variables*/
#define DMA_CHANNELS 16
typedef enum{DMA_8BIT,DMA_16BIT,DMA_32BIT,DMA_16BYTE_BURST=4,DMA_32BYTE_BURST} dma_transfer_size_t;
typedef enum {DMA_FTM=20,DMA_SW2=51,DMA_ALWAYS=58} dma_sources_t;
typedef void (* dma_irqfunc_t)(void);
/*Configuracion del mux para un canal*/
typedef struct{
	uint8_t channel;
	bool dma_enable;
	bool trigger_enable;
	dma_sources_t source;
}dma_mux_conf_t;

/*Funciones*/
void dma_init(void);

#endif /* DMA_H_ */
