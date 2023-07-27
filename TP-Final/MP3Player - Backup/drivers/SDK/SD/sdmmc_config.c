/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "sdmmc_config.h"
#include "hardware.h"
#include "gpio.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define SD_PIN_T						 PORTNUM2PIN(PE, BOARD_SDMMC_SD_CD_GPIO_PIN)

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*!brief sdmmc dma buffer */
SDK_ALIGN(static uint32_t s_sdmmcHostDmaBuffer[BOARD_SDMMC_HOST_DMA_DESCRIPTOR_BUFFER_SIZE],
          SDMMCHOST_DMA_DESCRIPTOR_BUFFER_ALIGN_SIZE);

static sd_detect_card_t s_cd;

static sdmmchost_t s_host;
static uint32_t s_event[4];
/*******************************************************************************
 * Code
 ******************************************************************************/

bool BOARD_SDCardGetDetectStatus(void)
{
    return gpioRead(SD_PIN_T) == BOARD_SDMMC_SD_CD_INSERT_LEVEL;
}

void BOARD_SDDetectCallback(void)
{
	if (s_cd.callback != NULL)
	{
		s_cd.callback(BOARD_SDCardGetDetectStatus(), s_cd.userData);
	}
}


void BOARD_SDCardDetectInit(sd_cd_t cd, void *userData)
{
    /* install card detect callback */
    s_cd.cdDebounce_ms = BOARD_SDMMC_SD_CARD_DETECT_DEBOUNCE_DELAY_MS;
    s_cd.type          = BOARD_SDMMC_SD_CD_TYPE;
    s_cd.cardDetected  = BOARD_SDCardGetDetectStatus;
    s_cd.callback      = cd;
    s_cd.userData      = userData;

    /* Card detection pin will generate interrupt on either eage */
	gpioIRQ(SD_PIN_T, GPIO_IRQ_MODE_BOTH_EDGES, BOARD_SDDetectCallback);


    if (BOARD_SDCardGetDetectStatus())
    {
        if (cd != NULL)
        {
            cd(true, userData);
        }
    }
}

static void BOARD_SDInitPins(void)
{
	SYSMPU->CESR &= ~SYSMPU_CESR_VLD_MASK; //Disable sysmpu

	CLOCK_EnableClock(kCLOCK_PortE);                           /* Port E Clock Gate Control: Clock enabled */

	const port_pin_config_t port_config = {
			kPORT_PullUp,                                            /* Internal pull-up resistor is enabled */
			kPORT_FastSlewRate,                                      /* Fast slew rate is configured */
			kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */
			kPORT_OpenDrainDisable,                                  /* Open drain is disabled */
			kPORT_HighDriveStrength,                                 /* High drive strength is configured */
			kPORT_MuxAlt4,                                           /* Pin is configured as SDHC0_D1 */
			kPORT_UnlockRegister                                     /* Pin Control Register fields [15:0] are not locked */
	};

	PORT_SetPinConfig(PORTE, 0, &port_config);   /* PORTE0 (pin 1) is configured as SDHC0_D1 */
	PORT_SetPinConfig(PORTE, 1, &port_config);   /* PORTE1 (pin 2) is configured as SDHC0_D0 */
	PORT_SetPinConfig(PORTE, 2, &port_config);   /* PORTE2 (pin 3) is configured as SDHC0_DCLK */
	PORT_SetPinConfig(PORTE, 3, &port_config);   /* PORTE3 (pin 4) is configured as SDHC0_CMD */
	PORT_SetPinConfig(PORTE, 4, &port_config);   /* PORTE4 (pin 5) is configured as SDHC0_D3 */
	PORT_SetPinConfig(PORTE, 5, &port_config);   /* PORTE5 (pin 6) is configured as SDHC0_D2 */

	gpioMode(SD_PIN_T, INPUT_PULLDOWN);

}

void BOARD_SD_Config(void *card, sd_cd_t cd, uint32_t hostIRQPriority, void *userData)
{
    assert(card);

    s_host.dmaDesBuffer                                      = s_sdmmcHostDmaBuffer;
    s_host.dmaDesBufferWordsNum                              = BOARD_SDMMC_HOST_DMA_DESCRIPTOR_BUFFER_SIZE;
    ((sd_card_t *)card)->host                                = &s_host;
    ((sd_card_t *)card)->host->hostController.base           = BOARD_SDMMC_SD_HOST_BASEADDR;
    ((sd_card_t *)card)->host->hostController.sourceClock_Hz = CLOCK_GetFreq(kCLOCK_CoreSysClk);

    ((sd_card_t *)card)->host->hostEvent = &s_event;
    ((sd_card_t *)card)->usrParam.cd     = &s_cd;

    BOARD_SDInitPins();
    BOARD_SDCardDetectInit(cd, userData);
}
