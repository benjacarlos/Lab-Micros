/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "fsl_smc.h"
#include "fsl_rcm.h"
#include "power_mode_switch.h"

#include "clock_config.h"
#include "fsl_pmc.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define APP_DEBUG_UART_BAUDRATE    9600              /* Debug console baud rate.           */
#define APP_DEBUG_UART_CLKSRC_NAME kCLOCK_CoreSysClk /* System clock.       */

#define CORE_CLK_FREQ           CLOCK_GetFreq(kCLOCK_CoreSysClk)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void APP_PowerPreSwitchHook(smc_power_state_t originPowerState, app_power_mode_t targetMode);
void APP_PowerPostSwitchHook(smc_power_state_t originPowerState, app_power_mode_t targetMode);
static void SetMode(app_power_mode_t mode);
/*
 * Set the clock configuration for RUN mode from VLPR mode.
 */
extern void APP_SetClockRunFromVlpr(void);

/*
 * Set the clock configuration for VLPR mode.
 */
extern void APP_SetClockVlpr(void);

/*
 * Hook function called before power mode switch.
 */
extern void APP_PowerPreSwitchHook(smc_power_state_t originPowerState, app_power_mode_t targetMode);

/*
 * Hook function called after power mode switch.
 */
extern void APP_PowerPostSwitchHook(smc_power_state_t originPowerState, app_power_mode_t targetMode);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static uint8_t s_wakeupTimeout;            /* Wakeup timeout. (Unit: Second) */
static app_wakeup_source_t s_wakeupSource; /* Wakeup source.                 */

/*******************************************************************************
 * Code
 ******************************************************************************/

void APP_SetClockVlpr(void)
{
    const sim_clock_config_t simConfig = {
        .pllFllSel = 3U,          /* PLLFLLSEL select IRC48MCLK. */
        .er32kSrc  = 2U,          /* ERCLK32K selection, use RTC. */
        .clkdiv1   = 0x00040000U, /* SIM_CLKDIV1. */
    };

    CLOCK_SetSimSafeDivs();
    CLOCK_SetInternalRefClkConfig(kMCG_IrclkEnable, kMCG_IrcFast, 0U);

    /* MCG works in PEE mode now, will switch to BLPI mode. */

    CLOCK_ExternalModeToFbeModeQuick();                     /* Enter FBE. */
    CLOCK_SetFbiMode(kMCG_Dmx32Default, kMCG_DrsLow, NULL); /* Enter FBI. */
    CLOCK_SetLowPowerEnable(true);                          /* Enter BLPI. */

    CLOCK_SetSimConfig(&simConfig);
}

void APP_SetClockRunFromVlpr(void)
{
    const sim_clock_config_t simConfig = {
        .pllFllSel = 1U,          /* PLLFLLSEL select PLL. */
        .er32kSrc  = 2U,          /* ERCLK32K selection, use RTC. */
        .clkdiv1   = 0x01140000U, /* SIM_CLKDIV1. */
    };

    const mcg_pll_config_t pll0Config = {
        .enableMode = 0U,
        .prdiv      = 0x13U,
        .vdiv       = 0x18U,
    };

    CLOCK_SetSimSafeDivs();

    /* Currently in BLPI mode, will switch to PEE mode. */
    /* Enter FBI. */
    CLOCK_SetLowPowerEnable(false);
    /* Enter FBE. */
    CLOCK_SetFbeMode(7U, kMCG_Dmx32Default, kMCG_DrsLow, NULL);
    /* Enter PBE. */
    CLOCK_SetPbeMode(kMCG_PllClkSelPll0, &pll0Config);
    /* Enter PEE. */
    CLOCK_SetPeeMode();

    CLOCK_SetSimConfig(&simConfig);
}

static void APP_InitDebugConsole(void)
{
    uint32_t uartClkSrcFreq;
    uartClkSrcFreq = CLOCK_GetFreq(APP_DEBUG_UART_CLKSRC_NAME);

}


void APP_PowerPreSwitchHook(smc_power_state_t originPowerState, app_power_mode_t targetMode)
{



    /* Disable the PLL in WAIT mode to same more power. FEI mode. */
    if (kAPP_PowerModeWait == targetMode)
    {
        const mcg_config_t mcgConfigStruct = {
            .mcgMode         = kMCG_ModeFEI,      /* FEI - FLL with internal RTC. */
            .irclkEnableMode = kMCG_IrclkEnable,  /* MCGIRCLK enabled, MCGIRCLK disabled in STOP mode */
            .ircs            = kMCG_IrcSlow,      /* Slow internal reference clock selected */
            .fcrdiv          = 0x0U,              /* Fast IRC divider: divided by 1 */
            .frdiv           = 0x0U,              /* FLL reference clock divider: divided by 32 */
            .drs             = kMCG_DrsLow,       /* Low frequency range */
            .dmx32           = kMCG_Dmx32Default, /* DCO has a default range of 25% */
            .oscsel          = kMCG_OscselOsc,    /* Selects System Oscillator (OSCCLK) */
            .pll0Config =
                {
                    .enableMode = 0,     /* MCGPLLCLK disabled */
                    .prdiv      = 0x13U, /* PLL Reference divider: divided by 20 */
                    .vdiv       = 0x18U, /* VCO divider: multiplied by 48 */
                },
        };

        CLOCK_SetMcgConfig(&mcgConfigStruct);
    }
}

void APP_PowerPostSwitchHook(smc_power_state_t originPowerState, app_power_mode_t targetMode)
{


    /*
     * For some other platforms, if enter LLS mode from VLPR mode, when wakeup, the
     * power mode is VLPR. But for some platforms, if enter LLS mode from VLPR mode,
     * when wakeup, the power mode is RUN. In this case, the clock setting is still
     * VLPR mode setting, so change to RUN mode setting here.
     */
    if ((kSMC_PowerStateVlpr == originPowerState) && (kSMC_PowerStateRun == SMC_GetPowerModeState(SMC)))
    {
        APP_SetClockRunFromVlpr();
    }

    /*
     * If enter stop modes when MCG in PEE mode, then after wakeup, the MCG is in PBE mode,
     * need to enter PEE mode manually.
     */
    if ((kAPP_PowerModeRun != targetMode) && (kAPP_PowerModeWait != targetMode) && (kAPP_PowerModeVlpw != targetMode) &&
        (kAPP_PowerModeVlpr != targetMode))
    {
        if (kSMC_PowerStateRun == originPowerState)
        {
            /* Wait for PLL lock. */
            while (!(kMCG_Pll0LockFlag & CLOCK_GetStatusFlags()))
            {
            }
            CLOCK_SetPeeMode();
        }
    }

    /* Return the PEE mode when come back to RUN mode from WAIT mode. */
    if ((kAPP_PowerModeWait == targetMode) && (kSMC_PowerStateRun == originPowerState))
    {
        const mcg_config_t mcgConfigStruct = {
            .mcgMode         = kMCG_ModePEE,      /* PEE - PLL Engaged External */
            .irclkEnableMode = kMCG_IrclkEnable,  /* MCGIRCLK enabled, MCGIRCLK disabled in STOP mode */
            .ircs            = kMCG_IrcSlow,      /* Slow internal reference clock selected */
            .fcrdiv          = 0x0U,              /* Fast IRC divider: divided by 1 */
            .frdiv           = 0x0U,              /* FLL reference clock divider: divided by 32 */
            .drs             = kMCG_DrsLow,       /* Low frequency range */
            .dmx32           = kMCG_Dmx32Default, /* DCO has a default range of 25% */
            .oscsel          = kMCG_OscselOsc,    /* Selects System Oscillator (OSCCLK) */
            .pll0Config =
                {
                    .enableMode = 0,     /* MCGPLLCLK disabled */
                    .prdiv      = 0x13U, /* PLL Reference divider: divided by 20 */
                    .vdiv       = 0x18U, /* VCO divider: multiplied by 48 */
                },
        };

        CLOCK_SetMcgConfig(&mcgConfigStruct);
    }

    APP_InitDebugConsole();
}





/*!
 * @brief Get input from user about wakeup timeout
 */
static uint8_t APP_GetWakeupTimeout(void)
{
    uint8_t timeout;

    while (1)
    {
    	printf("Select the wake up timeout in seconds.\r\n");
    	printf("The allowed range is 1s ~ 9s.\r\n");
    	printf("Eg. enter 5 to wake up in 5 seconds.\r\n");
    	printf("\r\nWaiting for input timeout value...\r\n\r\n");

        timeout = getchar();
        printf("%c\r\n", timeout);
        if ((timeout > '0') && (timeout <= '9'))
        {
            return timeout - '0';
        }
        printf("Wrong value!\r\n");
    }
}

/* Get wakeup source by user input. */
static app_wakeup_source_t APP_GetWakeupSource(void)
{
    uint8_t ch;

    while (1)
    {
    	printf("Select the wake up source:\r\n");
    	printf("Press T for LPTMR - Low Power Timer\r\n");

    	printf("\r\nWaiting for key press..\r\n\r\n");

        ch = getchar();

        if ((ch >= 'a') && (ch <= 'z'))
        {
            ch -= 'a' - 'A';
        }

        if (ch == 'T')
        {
            return kAPP_WakeupSourceLptmr;
        }
        else if (ch == 'S')
        {
            return kAPP_WakeupSourcePin;
        }
        else
        {
        	printf("Wrong value!\r\n");
        }
    }
}

/* Get wakeup timeout and wakeup source. */
void APP_GetWakeupConfig(app_power_mode_t targetMode)
{
    /* Get wakeup source by user input. */
    if (targetMode == kAPP_PowerModeVlls0)
    {
        /* In VLLS0 mode, the LPO is disabled, LPTMR could not work. */
    	printf("Not support LPTMR wakeup because LPO is disabled in VLLS0 mode.\r\n");
        s_wakeupSource = kAPP_WakeupSourcePin;
    }
    else
    {
        /* Get wakeup source by user input. */
        s_wakeupSource = APP_GetWakeupSource();
    }

    if (kAPP_WakeupSourceLptmr == s_wakeupSource)
    {
        /* Wakeup source is LPTMR, user should input wakeup timeout value. */
        s_wakeupTimeout = APP_GetWakeupTimeout();
        printf("Will wakeup in %d seconds.\r\n", s_wakeupTimeout);
    }

}

void APP_SetWakeupConfig(app_power_mode_t targetMode)
{
    /* Set LPTMR timeout value. */



    /* If targetMode is VLLS/LLS, setup LLWU. */
    if ((kAPP_PowerModeWait != targetMode) && (kAPP_PowerModeVlpw != targetMode) &&
        (kAPP_PowerModeVlps != targetMode) && (kAPP_PowerModeStop != targetMode))
    {

        NVIC_EnableIRQ(LLWU_IRQn);
    }
}

void APP_ShowPowerMode(smc_power_state_t powerMode)
{
    switch (powerMode)
    {
        case kSMC_PowerStateRun:
        	printf("    Power mode: RUN\r\n");
            break;
        case kSMC_PowerStateVlpr:
        	printf("    Power mode: VLPR\r\n");
            break;
        default:
        	printf("    Power mode wrong\r\n");
            break;
    }
}

/*
 * Check whether could switch to target power mode from current mode.
 * Return true if could switch, return false if could not switch.
 */
bool APP_CheckPowerMode(smc_power_state_t curPowerState, app_power_mode_t targetPowerMode)
{
    bool modeValid = true;

    /*
     * Check wether the mode change is allowed.
     *
     * 1. If current mode is HSRUN mode, the target mode must be RUN mode.
     * 2. If current mode is RUN mode, the target mode must not be VLPW mode.
     * 3. If current mode is VLPR mode, the target mode must not be HSRUN/WAIT/STOP mode.
     * 4. If already in the target mode.
     */
    switch (curPowerState)
    {
        case kSMC_PowerStateRun:
            if (kAPP_PowerModeVlpw == targetPowerMode)
            {
            	printf("Could not enter VLPW mode from RUN mode.\r\n");
                modeValid = false;
            }
            break;

        case kSMC_PowerStateVlpr:
            if ((kAPP_PowerModeWait == targetPowerMode) || (kAPP_PowerModeStop == targetPowerMode))
            {
            	printf("Could not enter HSRUN/STOP/WAIT modes from VLPR mode.\r\n");
                modeValid = false;
            }
            break;
        default:
        	printf("Wrong power state.\r\n");
            modeValid = false;
            break;
    }

    if (!modeValid)
    {
        return false;
    }

    /* Don't need to change power mode if current mode is already the target mode. */
    if (((kAPP_PowerModeRun == targetPowerMode) && (kSMC_PowerStateRun == curPowerState)) ||
        ((kAPP_PowerModeVlpr == targetPowerMode) && (kSMC_PowerStateVlpr == curPowerState)))
    {
    	printf("Already in the target power mode.\r\n");
        return false;
    }

    return true;
}

/*
 * Power mode switch.
 */
void APP_PowerModeSwitch(smc_power_state_t curPowerState, app_power_mode_t targetPowerMode)
{
    smc_power_mode_vlls_config_t vlls_config;
    vlls_config.enablePorDetectInVlls0 = true;

    switch (targetPowerMode)
    {
        case kAPP_PowerModeVlpr:
            APP_SetClockVlpr();
            SMC_SetPowerModeVlpr(SMC, false);
            while (kSMC_PowerStateVlpr != SMC_GetPowerModeState(SMC))
            {
            }
            break;

        case kAPP_PowerModeRun:

            /* Power mode change. */
            SMC_SetPowerModeRun(SMC);
            while (kSMC_PowerStateRun != SMC_GetPowerModeState(SMC))
            {
            }

            /* If enter RUN from VLPR, change clock after the power mode change. */
            if (kSMC_PowerStateVlpr == curPowerState)
            {
                APP_SetClockRunFromVlpr();
            }
            break;

        case kAPP_PowerModeWait:
            SMC_PreEnterWaitModes();
            SMC_SetPowerModeWait(SMC);
            SMC_PostExitWaitModes();
            break;

        case kAPP_PowerModeStop:
            SMC_PreEnterStopModes();
            SMC_SetPowerModeStop(SMC, kSMC_PartialStop);
            SMC_PostExitStopModes();
            break;

        case kAPP_PowerModeVlpw:
            SMC_PreEnterWaitModes();
            SMC_SetPowerModeVlpw(SMC);
            SMC_PostExitWaitModes();
            break;

        case kAPP_PowerModeVlps:
            SMC_PreEnterStopModes();
            SMC_SetPowerModeVlps(SMC);
            SMC_PostExitStopModes();
            break;

        case kAPP_PowerModeLls:
            SMC_PreEnterStopModes();
            SMC_SetPowerModeLls(SMC);
            SMC_PostExitStopModes();
            break;

        case kAPP_PowerModeVlls0:
            vlls_config.subMode = kSMC_StopSub0;
            SMC_PreEnterStopModes();
            SMC_SetPowerModeVlls(SMC, &vlls_config);
            SMC_PostExitStopModes();
            break;

        case kAPP_PowerModeVlls1:
            vlls_config.subMode = kSMC_StopSub1;
            SMC_PreEnterStopModes();
            SMC_SetPowerModeVlls(SMC, &vlls_config);
            SMC_PostExitStopModes();
            break;

        case kAPP_PowerModeVlls2:
            vlls_config.subMode = kSMC_StopSub2;
            SMC_PreEnterStopModes();
            SMC_SetPowerModeVlls(SMC, &vlls_config);
            SMC_PostExitStopModes();
            break;

        case kAPP_PowerModeVlls3:
            vlls_config.subMode = kSMC_StopSub3;
            SMC_PreEnterStopModes();
            SMC_SetPowerModeVlls(SMC, &vlls_config);
            SMC_PostExitStopModes();
            break;

        default:
        	printf("Wrong value");
            break;
    }
}

void PowerMode_Init(void)
{
	/* Power related. */
	SMC_SetPowerModeProtection(SMC, kSMC_AllowPowerModeAll);
	if (kRCM_SourceWakeup & RCM_GetPreviousResetSources(RCM)) /* Wakeup from VLLS. */
	{
		PMC_ClearPeriphIOIsolationFlag(PMC);
		NVIC_ClearPendingIRQ(LLWU_IRQn);
	}
	BOARD_InitBootClocks();

	if (kRCM_SourceWakeup & RCM_GetPreviousResetSources(RCM)) /* Wakeup from VLLS. */
	{
		printf("\r\nMCU wakeup from VLLS modes...\r\n");
	}
}

void PowerMode_SetVLPRMode(void)
{
	SetMode(kAPP_PowerModeVlpr);
}

void PowerMode_SetRunMode(void)
{
	SetMode(kAPP_PowerModeRun);
}


static void SetMode(app_power_mode_t mode)
{
	smc_power_state_t curPowerState;
	app_power_mode_t targetPowerMode;
	curPowerState = SMC_GetPowerModeState(SMC);

	int freq = CLOCK_GetFreq(kCLOCK_CoreSysClk);

	printf("\r\n####################  Power Mode Switch. Old Mode: ####################\n\r\n");
	printf("Old Core Clock = %dHz \r\n", freq);

	APP_ShowPowerMode(curPowerState);
	targetPowerMode = mode;
	if ((targetPowerMode > kAPP_PowerModeMin) && (targetPowerMode < kAPP_PowerModeMax))
	{
		/* If could not set the target power mode, returns. */
		if (!APP_CheckPowerMode(curPowerState, targetPowerMode))
		{
			return;
		}


		APP_PowerPreSwitchHook(curPowerState, targetPowerMode);


		APP_PowerModeSwitch(curPowerState, targetPowerMode);
		APP_PowerPostSwitchHook(curPowerState, targetPowerMode);

	}
	printf("\r\n####################  Power Mode Switch. New Mode: ####################\n\r\n");
	freq = CLOCK_GetFreq(kCLOCK_CoreSysClk);
	printf("New Core Clock = %dHz \r\n", freq);
	curPowerState = SMC_GetPowerModeState(SMC);
	APP_ShowPowerMode(curPowerState);
}
