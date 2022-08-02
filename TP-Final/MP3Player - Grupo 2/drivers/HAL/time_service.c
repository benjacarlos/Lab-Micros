/***************************************************************************/ /**
  @file     time_service.c
  @brief    Time Service Functions
  @author   Grupo 2 - Lab de Micros
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "clock_config.h"
#include "fsl_rtc.h"
#include "time_service.h"
#include "Timer.h"

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

/*******************************************************************************
 * DEFINITIONS
 ******************************************************************************/
#define EXAMPLE_OSC_WAIT_TIME_MS 1000UL

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
#if !(defined(FSL_FEATURE_RTC_HAS_NO_CR_OSCE) && FSL_FEATURE_RTC_HAS_NO_CR_OSCE)
/* Wait for 32kHz OSC clock start up. */
static void EXAMPLE_WaitOSCReady(uint32_t delay_ms);
#endif

static void TimeService_FinishInit(void);

/*******************************************************************************
 * PRIVATE VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/
static rtc_datetime_t date;
void (*rtc_callback)(void);
/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
/*!
 * @brief Main function
 */
void TimeService_Init(void (*callback)(void))
{

	/* Set a start date time */
	date = (rtc_datetime_t) {.year = 2021U,
			.month = 2U,
			.day = 6U,
			.hour = 19U,
			.minute = 41U,
			.second = 0};

	/*char *ptr = strtok(__TIME__, ':');
	int i = 0;
	while(ptr != NULL)
	{
		if (i==0)
			date.hour = atoi(ptr); //Guardar hour = ptr  st -> to int
		else if (i == 1)
			date.minute = atoi(ptr);//Guardar minute = ptr  st -> to int
		else if (i == 2)
			date.second = atoi(ptr);//Guardar second = ptr  st -> to int
		ptr = strtok(NULL, ':');
		i++;
	}*/

	date.hour = (__TIME__[0] - '0')*10 + (__TIME__[1]-'0');
	date.minute = (__TIME__[3] - '0')*10 + (__TIME__[4]-'0');
	date.second = (__TIME__[6] - '0')*10 + (__TIME__[7]-'0');
	date.day = (__DATE__[4] - '0')*10 + (__DATE__[5]-'0');

    rtc_config_t rtcConfig;

    /* Init RTC */
    /*
     * rtcConfig.wakeupSelect = false;
     * rtcConfig.updateMode = false;
     * rtcConfig.supervisorAccess = false;
     * rtcConfig.compensationInterval = 0;
     * rtcConfig.compensationTime = 0;
     */
    RTC_GetDefaultConfig(&rtcConfig);
    RTC_Init(RTC, &rtcConfig);

    /* If the oscillator has not been enabled. */
    if (0U == (RTC->CR & RTC_CR_OSCE_MASK))
    {
        /* Select RTC clock source */
        RTC_SetClockSource(RTC);

        /* Wait for OSC clock steady. */
        Timer_AddCallback(TimeService_FinishInit, 1000, true);
    }
    else
    {
    	TimeService_FinishInit();
    }
    rtc_callback = callback;
}


TimeServiceDate_t TimeService_GetCurrentDateTime(void)
{
	/* Get date time */
	RTC_GetDatetime(RTC, &date);
	TimeServiceDate_t newDate =  {.year = date.year,
		.month = date.month,
		.day = date.day,
		.hour = date.hour,
		.minute = date.minute,
		.second = date.second};
	return newDate;
	/* print default time */
	/*
	char dateTime[19];
	snprintf(dateTime, sizeof(dateTime), "%02hd-%02hd-%04hd %02hd:%02hd:%02hd", date.day, date.month, date.year, date.hour,
			   date.minute, date.second);
	return &dateTime[0];*/
}

void TimeService_Enable(void)
{

    /* Enable RTC second interrupt */
    RTC_EnableInterrupts(RTC, kRTC_SecondsInterruptEnable);

}

void TimeService_Disable(void)
{
	/* Disable RTC second interrupt */
	RTC_DisableInterrupts(RTC, kRTC_SecondsInterruptEnable);
}

/*!
 * @brief ISR for Alarm interrupt
 *
 * This function changes the state of busyWait.
 */
#include "gpio.h"
#include "board.h"

void RTC_Seconds_IRQHandler(void)
{
	gpioToggle(TP);

    if (rtc_callback != NULL)
    	rtc_callback();

    gpioToggle(TP);

    SDK_ISR_EXIT_BARRIER;
}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
static void TimeService_FinishInit(void)
{

    /* RTC time counter has to be stopped before setting the date & time in the TSR register */
    RTC_StopTimer(RTC);

    /* Set RTC time to default */
    RTC_SetDatetime(RTC, &date);

    /* Disable Interrupts */
    TimeService_Disable();

    /* Enable at the NVIC */
    EnableIRQ(RTC_Seconds_IRQn);

    /* Start the RTC time counter */
    RTC_StartTimer(RTC);
}


#if !(defined(FSL_FEATURE_RTC_HAS_NO_CR_OSCE) && FSL_FEATURE_RTC_HAS_NO_CR_OSCE)
/*!
 * @brief Waitting for the OSC clock steady.
 *
 * Due to the oscillator startup time is depending on the hardware design and usually
 * take hundreds of milliseconds to make the oscillator stable. Here, we just delay a certain
 * time to ensure the oscillator stable, please use the suitable delay time to adapt
 * to your real usage if needed.
 */
static void EXAMPLE_WaitOSCReady(uint32_t delay_ms)
{
    uint32_t ticks = 0UL;
    uint32_t count = delay_ms;

    /* Make a 1 milliseconds counter. */
    ticks = SystemCoreClock / 1000UL;
    assert((ticks - 1UL) <= SysTick_LOAD_RELOAD_Msk);

    /* Enable the SysTick for counting. */
    SysTick->LOAD = (uint32_t)(ticks - 1UL);
    SysTick->VAL  = 0UL;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;

    for (; count > 0U; count--)
    {
        /* Wait for the SysTick counter reach 0. */
        while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk))
        {
        }
    }

    /* Disable SysTick. */
    SysTick->CTRL &= ~(SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk);
    SysTick->LOAD = 0UL;
    SysTick->VAL  = 0UL;
}
#endif
