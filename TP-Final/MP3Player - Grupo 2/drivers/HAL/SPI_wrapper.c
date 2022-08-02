/*
 * SPI_wrapper.c
 *
 *  Created on: 16 ene. 2021
 *      Author: Santi
 */

#include <stdbool.h>
#include <stdint.h>

#include "fsl_common.h"
#include "fsl_port.h"
#include "fsl_dspi.h"

#include "SPI_wrapper.h"
#include "hardware.h"

#define MAX_SIZE 750
#define MSG_LEN(rear, front, max) (((rear) + (max) - (front)) % ((max)-1)) // MSG_LEN(rear, front, max_len)
#define BUFFER_FULL(rear, front, max) ((((rear) + 2) % ((max)-1)) == (front))

typedef struct spi_block
{
	uint32_t data;
	bool end;
	void (*callback)(void);
} spi_block_t;

static SPI_Type *p_spi[] = SPI_BASE_PTRS;

static spi_block_t buffer_out[MAX_SIZE] = {0U};
static uint16_t p_out_rear = 0, p_out_front = 1;
static uint16_t msg_size = 0;

//static uint32_t masterFifoSize; // necesario??
static uint32_t masterCommand;

void SPI_Init(spi_id_t id, spi_slave_t slave, uint32_t baudrate)
{
	/* Clock Gate Control: Clock enabled */
	if (id == 0)
	{
		CLOCK_EnableClock(kCLOCK_PortD);
		if (slave > 0 && slave < 5)
			CLOCK_EnableClock(kCLOCK_PortC);
		else if (slave == 0)
			CLOCK_EnableClock(kCLOCK_PortB);
	}
	else
		CLOCK_EnableClock(kCLOCK_PortB);

	/* Pin routing */
	PORT_Type *port_spi_n = id == 0 ? PORTD : PORTB;
	uint8_t sck = id == 0 ? 1 : 2, sout = id == 0 ? 2 : 16, sin = id == 0 ? 3 : 17;

	PORT_SetPinMux(port_spi_n, sck, kPORT_MuxAlt2);
	PORT_SetPinMux(port_spi_n, sout, kPORT_MuxAlt2);
	PORT_SetPinMux(port_spi_n, sin, kPORT_MuxAlt2);

	if (id != 0)
		PORT_SetPinMux(PORTB, 3, kPORT_MuxAlt2); /* PORTD0 (pin 93) is configured as SPI0_PCS0 */
	else if (slave > 0 && slave < 5)
	{
		PORT_SetPinMux(PORTC, slave - 1, kPORT_MuxAlt2);
		slave = 5 - slave;
	}
	else if (slave != 0)
	{
		PORT_SetPinMux(PORTB, slave, kPORT_MuxAlt2);
		slave = 5;
	}
	else
		PORT_SetPinMux(PORTD, slave, kPORT_MuxAlt2);

	SPI_Config(id, slave, baudrate);

	/* Enable the NVIC for DSPI peripheral. */
	EnableIRQ(SPI0_IRQn + (id != 2 ? id : 39));

	/* Start master transfer*/
	dspi_command_data_config_t commandData;

	commandData.isPcsContinuous = false;
	commandData.whichCtar = kDSPI_Ctar0;
	commandData.whichPcs = 1 << slave;
	;
	commandData.isEndOfQueue = false;
	commandData.clearTransferCount = false;

	masterCommand = DSPI_MasterGetFormattedCommand(&commandData);

	//	masterFifoSize = FSL_FEATURE_DSPI_FIFO_SIZEn(port_spi_n);

	DSPI_StopTransfer(p_spi[id]);
	DSPI_FlushFifo(p_spi[id], true, true);
	DSPI_ClearStatusFlags(p_spi[id], (uint32_t)kDSPI_AllStatusFlag);
}

void SPI_Config(spi_id_t id, spi_slave_t slave, uint32_t baudrate)
{
	/* SPI Config */

	dspi_master_config_t masterConfig;

	DSPI_MasterGetDefaultConfig(&masterConfig);
	masterConfig.whichPcs = 1 << slave;
	masterConfig.ctarConfig.baudRate = baudrate;

	uint32_t srcClock_Hz = CLOCK_GetFreq(kCLOCK_BusClk);//__CORE_CLOCK__ / 2; // Bus Clock
	DSPI_MasterInit(p_spi[id], &masterConfig, srcClock_Hz);
}

void SPI_Send(spi_id_t id, spi_slave_t slave, const char *msg, uint16_t len, void (*end_callback)(void))
{
	/**
	 * static uint32_t buffer_out[MAX_SIZE] = {0U};
	 * static uint16_t p_out_rear = 0, p_out_front = 1;
	 */
	uint16_t i = 0;

	if (slave < 5 && slave > 0)
		slave = 5 - slave;
	else if (slave != 0)
		slave = 5;

	masterCommand = (masterCommand & ~SPI_PUSHR_PCS_MASK) | SPI_PUSHR_PCS(1 << slave);

	while ((len > i) && !BUFFER_FULL(p_out_rear, p_out_front, MAX_SIZE))
	{
		p_out_rear = (p_out_rear + 1) % (MAX_SIZE - 1);
		spi_block_t block = {
			.data = masterCommand | msg[i],
			.end = false,
			.callback = NULL};
		buffer_out[p_out_rear] = block;
		i++;
	}
	buffer_out[p_out_rear].end = true;
	buffer_out[p_out_rear].callback = end_callback;
	msg_size += len;

	/*Fill up the master Tx data*/
	while (DSPI_GetStatusFlags(p_spi[id]) & kDSPI_TxFifoFillRequestFlag)
	{
		if (msg_size != 0)
		{
			p_spi[id]->PUSHR = buffer_out[p_out_front].data;

			if (buffer_out[p_out_front].end &&
				buffer_out[p_out_front].callback != NULL)
			{
				buffer_out[p_out_front].callback();
			}

			p_out_front = (p_out_front + 1) % (MAX_SIZE - 1);
			--msg_size;
		}
		else
		{
			break;
		}

		/* Try to clear the TFFF; if the TX FIFO is full this will clear */
		DSPI_ClearStatusFlags(p_spi[id], kDSPI_TxFifoFillRequestFlag);
	}

	/*Enable master RX interrupt*/
	DSPI_EnableInterrupts(p_spi[id], kDSPI_TxFifoFillRequestInterruptEnable);
	/* Start DSPI transafer.*/
	DSPI_StartTransfer(p_spi[id]);
}

#include "gpio.h"
#include "board.h"

#ifndef TEST
void SPI0_IRQHandler(void)
{
	gpioToggle(TP);

	while (DSPI_GetStatusFlags(p_spi[0]) & kDSPI_TxFifoFillRequestFlag)
	{
		if (msg_size != 0)
		{
			p_spi[0]->PUSHR = buffer_out[p_out_front].data;

			if (buffer_out[p_out_front].end &&
				buffer_out[p_out_front].callback != NULL)
			{
				buffer_out[p_out_front].callback();
			}

			p_out_front = (p_out_front + 1) % (MAX_SIZE - 1);
			--msg_size;
		}
		else
		{
			break;
		}

		/* Try to clear the TFFF; if the TX FIFO is full this will clear */
		DSPI_ClearStatusFlags(p_spi[0], kDSPI_TxFifoFillRequestFlag);
	}

	/* Check if we're done with this transfer.*/
	if (msg_size == 0)
	{
		//isTransferCompleted = true;
		/* Complete the transfer and disable the interrupts */
		DSPI_DisableInterrupts(p_spi[0], kDSPI_TxFifoFillRequestInterruptEnable);
	}

	gpioToggle(TP);

	SDK_ISR_EXIT_BARRIER;
}
#endif
