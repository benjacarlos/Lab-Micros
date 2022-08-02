/***************************************************************************//**
  @file     App.c
  @brief    Application functions
  @author   Nicolás Magliola
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "hardware.h"
#include "gpio.h"
#include "board.h"
#include "SDK/CMSIS/MK64F12.h"
#include "SDK/CMSIS/MK64F12_features.h"
#include "DMA.h"
#include <stdio.h>


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
/* Auxiliary variable used to modify the source buffer on each iteration. */
static uint8_t MinorTransferDone = 0;

//uint16_t sourceBuffer[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
uint16_t sourceBuffer[10] = {0x1234,0x6789,0x1122,0x2233,0x5588,0x2345,0x3145,0x8172,0x6183,0x3756};
uint8_t destinationBuffer[10];

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
void DMA_Test(void);


/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/* Función que se llama 1 vez, al comienzo del programa */
void App_Init (void)
{
	hw_DisableInterrupts ();
	/**************************************************************************/
	/***** Configure the FRDM-K64F LEDs (RED - PTB22, GREEN - PTE26,
	 * BLUE - PTB21) to report events. *****/

	/* Select LEDs as outputs. */
	gpioMode(PIN_LED_RED, OUTPUT);
	gpioMode(PIN_LED_BLUE, OUTPUT);
	gpioMode(PIN_LED_GREEN, OUTPUT);

	/* Turn all the LEDs off. (Active low) */
	gpioWrite (PIN_LED_RED, true);
	gpioWrite (PIN_LED_BLUE, true);
	gpioWrite (PIN_LED_GREEN, true);


	/**************************************************************************/
	/***** Configure the FRDM-K64F SW2 (PTC6) as the DMA request source. *****/
	gpioMode(PIN_SW2, INPUT_PULLUP);
	gpioIRQ (PIN_SW2, PORT_eDMAFalling, NULL);			//Usamos el puerto del pin como flanco negativo

	dma_init();

	/* Enable the eDMA channel 0 and set the PORTC as the DMA request source. */
	DMAMUX->CHCFG[0] |= DMAMUX_CHCFG_ENBL_MASK | DMAMUX_CHCFG_SOURCE(51);	//Llama al puerto C que es donde está el sw 2

	/* Enable the interrupts for the channel 0. */
	/* Clear all the pending events. */
	NVIC_ClearPendingIRQ(DMA0_IRQn);
	/* Enable the DMA interrupts. */
	NVIC_EnableIRQ(DMA0_IRQn);


	/// ============= INIT TCD0 ===================//
	/* Set memory address for source and destination. */

	DMA0->TCD[0].SADDR= (uint32_t)(sourceBuffer);
	DMA0->TCD[0].DADDR = (uint32_t)(destinationBuffer);	//Se castean a 32 porque asi son el tamaño de los registros

		/* Set an offset for source and destination address. */
	DMA0->TCD[0].SOFF =0x02; // Source address offset of 2 bytes per transaction.
	DMA0->TCD[0].DOFF =0x01; // Destination address offset of 1 byte per transaction.

	/* Set source and destination data transfer size is 1 byte. */
	DMA0->TCD[0].ATTR = DMA_ATTR_SSIZE(0) | DMA_ATTR_DSIZE(0);

	/*Number of bytes to be transfered in each service request of the channel.*/
	DMA0->TCD[0].NBYTES_MLNO= 0x01;	//1X8-Bits=1x1-Byte

	/* Current major iteration count (5 iteration of 1 byte each one). */
	DMA0->TCD[0].CITER_ELINKNO = DMA_CITER_ELINKNO_CITER(0x05);
	DMA0->TCD[0].BITER_ELINKNO = DMA_BITER_ELINKNO_BITER(0x05);
	//Lo de arriba seria el minor loop que implica 5 transferencias chicas de 1 Byte


	/* Address for the next TCD to be loaded in the scatter/gather mode. */
	//	tcd[TCD0].SLAST = 0;			// Source address adjustment not used.
	//	tcd[TCD0].DLASTSGA = (uint32_t)&tcd[TCD1];	// The tcd[TCD1] is the next TCD to be loaded.

	//Son offset que aparecen dependiendo de si se cumplen determinadas condiciones
	DMA0->TCD[0].SLAST = 0x00;	//Esté es un offset que aparece al terminar de contar
	DMA0->TCD[0].DLAST_SGA = 0x00;	//Esté es el equivalente para el destination

	/* Setup control and status register. */

	DMA0->TCD[0].CSR = DMA_CSR_INTMAJOR_MASK;	//Enable Major Interrupt.
	//Sin lo de arriba no tenemos interrupciones cada vez que se termina de realizar un minor loop

	/* Enable request signal for channel 0. */
	DMA0->ERQ = DMA_ERQ_ERQ0_MASK;

	hw_EnableInterrupts();

}


/* Función que se llama constantemente en un ciclo infinito */
void App_Run (void)
{
	DMA_Test();
}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void DMA_Test(void)
{
	if(MinorTransferDone){
		MinorTransferDone = 0;
	}
}

/* The blue LED is toggled when a TCD is completed. */
__ISR__ DMA0_IRQHandler(void)
{
	/* Clear the interrupt flag. */
	DMA0->CINT |= 0;

	/* Toggle the blue LED. */
	gpioToggle (PIN_LED_BLUE);

	/* Change the source buffer contents. */
	MinorTransferDone = 1;
}

/* The red LED is toggled when an error occurs. Esto sucede cuando se encuentra mal configurado */
__ISR__ DMA_Error_IRQHandler(void)
{
	/* Clear the error interrupt flag.*/
	DMA0->CERR |= 0;

	/* Turn the red LED on. */
	gpioToggle (PIN_LED_RED);
}
