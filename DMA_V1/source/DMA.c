/*
 * DMA.c
 *
 *  Created on: 22 nov. 2020
 *      Author: Juan Martin
 */

/*Includes*/
#include "DMA.h"
#include "SDK/CMSIS/MK64F12.h"
#include <stdlib.h>

/*Variables*/
static bool Inicialized=false;
static dma_irqfunc_t irqfunc[DMA_CHANNELS];

/*Funciones*/
void dma_init(void){

	if (Inicialized == false){
		SIM_Type* sim= SIM;
		// Enable the clock for the eDMA and the DMAMUX.
		sim->SCGC7 |= SIM_SCGC7_DMA_MASK;
		sim->SCGC6 |= SIM_SCGC6_DMAMUX_MASK;
		//
		for (unsigned int i=0; i< DMA_CHANNELS; i++){
			irqfunc[i]=NULL;
		}
		Inicialized=true;
	}
}

/*
 * @brief Configure the selected mu channel of the DMA
 * @param config: enable channel, channel, enable trigger and source of DMA request
 * @especifications: the periodic trigger is define by PIT (Default is 50MHz) and need to active PIT in order to work
 */
static void dma_mux_init(dma_mux_conf_t config){
	if(config.channel < DMA_CHANNELS){
		DMAMUX_Type* dma_mux = DMAMUX;
		//Reset channel
		dma_mux->CHCFG[config.channel] = 0x00;
		//Active channel
		dma_mux->CHCFG[config.channel] = DMAMUX_CHCFG_ENBL(config.dma_enable) | DMAMUX_CHCFG_TRIG(config.trigger_enable) | DMAMUX_CHCFG_SOURCE(config.source);
	}
}

