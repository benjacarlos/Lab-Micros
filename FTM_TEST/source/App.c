/***************************************************************************//**
  @file     App.c
  @brief    Application functions
  @author   Grupo 5
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "SDK/CMSIS/MK64F12.h"
#include "SDK/CMSIS/MK64F12_features.h"
#include "FTM.h"
#include "gpio.h"
#include "board.h"
#include "SDK/startup/hardware.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/



/*******************************************************************************
 *******************************************************************************
                        VARIABLES LOCALES
 *******************************************************************************
 ******************************************************************************/



/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/


/* Función que se llama 1 vez, al comienzo del programa */
void App_Init (void)
{
	hw_DisableInterrupts ();
	ftm_init(FTM_0,FTM_PSC_x2);		// 50MHz/2=25MHz aprox (0.4uSeg)

	// PTC 1 as PWM
	gpioMode (PIN_PWM0, INPUT);

	// PTC 8 as GPIO
	gpioMode (PORTNUM2PIN(PC,8), OUTPUT);


	write_mod_value(FTM_0, 250);					// Periodo de 250/25MHz=10uSeg
	ftm_enable_overflow_irq(FTM_0, false);			// No quiero overflow
	/*Se configura para poder utilizar el PWM del canal 0 del FTM0*/
	ftm_pwm_config_t pwm_conf = {
			.channel = FTM_CH_0,
			.mod=250,
			.CnV=0,
			.mode=FTM_PWM_EDGE_ALIGNED
		};
	ftm_set_pwm_conf(FTM_0, pwm_conf);
	ftm_set_pwm_duty_cycle(FTM_0, FTM_CH_0,50);		//50% de duty cycle
	ftm_start_clk(FTM_0, FTM_SYSTEM_CLK);

	hw_EnableInterrupts();
}


/* Función que se llama constantemente en un ciclo infinito ---->>> aplcia mi maquina de estados*/
void App_Run (void)
{

}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/


