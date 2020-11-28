/*
 * FTM.c
 *
 *  Created on: 24 nov. 2020
 *      Author: Juan Martin
 */

/*Includes*/
#include "FTM.h"
#include "MK64F12.h"
#include <stdlib.h>
#include "gpio.h"
#include "SDK/startup/hardware.h"

/***************************************/
/*************Variables*****************/
/***************************************/
static bool initiliazed[FTM_N_MODULES]={false,false,false,false};
static int const ftm_irqs[FTM_N_MODULES] = {FTM0_IRQn, FTM1_IRQn, FTM2_IRQn, FTM3_IRQn};
/*For pwm configurations*/
static uint32_t const pwm_combine_masks[4]= {FTM_COMBINE_COMBINE0_MASK, FTM_COMBINE_COMBINE1_MASK,
		FTM_COMBINE_COMBINE2_MASK, FTM_COMBINE_COMBINE3_MASK};
static uint32_t const pwm_decapen_masks[4] = {FTM_COMBINE_DECAPEN0_MASK, FTM_COMBINE_DECAPEN1_MASK,
		FTM_COMBINE_DECAPEN2_MASK, FTM_COMBINE_DECAPEN3_MASK};

/***************************************/
/**Declaraciones de funciones locales***/
/***************************************/
void ftm_set_prescaler(ftm_modules_t module , ftm_prescaler_t prescaler_config);

/***************************************/
/*************Funciones*****************/
/***************************************/
void ftm_init(ftm_modules_t module, ftm_prescaler_t prescaler_config){
	if(initiliazed[module] == false){

		//Clock Gating
		if (module == FTM_0){
			SIM->SCGC6 |= SIM_SCGC6_FTM0_MASK;
		}
		else if(module == FTM_1){
			SIM->SCGC6 |= SIM_SCGC6_FTM1_MASK;
		}
		else if (module == FTM_2){
			SIM->SCGC6 |= SIM_SCGC6_FTM2_MASK;
			SIM->SCGC3 |= SIM_SCGC3_FTM2_MASK;
		}
		else if( module == FTM_3){
			SIM->SCGC3 |= SIM_SCGC3_FTM3_MASK;
		}
		//Clear IRQ and enable it
		NVIC_ClearPendingIRQ(ftm_irqs[module]);
		NVIC_EnableIRQ(ftm_irqs[module]);

		//Define frecuency= Source/prescale_config
		ftm_set_prescaler(module, prescaler_config);

		initiliazed[module] = true;
	}
}

void ftm_set_prescaler(ftm_modules_t module , ftm_prescaler_t prescaler_config){
	ftms[module]->SC = (ftms[module]->SC & ~FTM_SC_PS_MASK) | FTM_SC_PS(prescaler_config);
}

void ftm_start_clk(ftm_modules_t module, ftm_clk_src_t source){
	ftms[module]->SC |= FTM_SC_CLKS(source);
}

void ftm_stop_clock(ftm_modules_t module){
	ftms[module]->SC = (ftms[module]->SC & ~FTM_SC_CLKS_MASK) | FTM_SC_CLKS(0);
}

void ftm_enable_overflow_irq(ftm_modules_t module, bool enable_disable){
	ftms[module]->SC = (ftms[module]->SC & ~FTM_SC_TOIE_MASK) | FTM_SC_TOIE(enable_disable);
}

bool ftm_has_overflowed(ftm_modules_t module){
	return ftms[module]->SC & FTM_SC_TOF_MASK;
}

void ftm_clear_overflow_flag (ftm_modules_t module){
	ftms[module]->SC &= ~FTM_SC_TOF_MASK;
}

/*COUNTER REGISTER*/
void ftm_reset_counter_value(ftm_modules_t module){
	//Reset clears the CNT register
	ftms[module]->CNT |= FTM_CNT_COUNT(0x00);
}
uint16_t ftm_read_counter_value(ftm_modules_t module){
	return ftms[module]->CNT & FTM_CNT_COUNT_MASK;
}

/*MOD REGISTER*/
/*
 The Modulo register contains the modulo value for the FTM counter. After the FTM
counter reaches the modulo value, the overflow flag (TOF) becomes set at the next clock,
and the next value of FTM counter depends on the selected counting method.

Initialize the FTM counter, by writing to CNT, before writing to the MOD register to
avoid confusion about when the first counter overflow will occur.
 */

void write_mod_value(ftm_modules_t module , uint16_t mod_value){
	ftms[module]->MOD = FTM_MOD_MOD(mod_value);
}
uint16_t ftm_get_mod_value(ftm_modules_t module){
	return ftms[module]->MOD;
}
/*Set output channel by default it will be low*/
void ftm_constant_ch_output(ftm_modules_t module,ftm_channel_t channel, bool exit){
	uint32_t CHANNELS_OUT_MASK[]= {FTM_OUTMASK_CH0OM_MASK,FTM_OUTMASK_CH1OM_MASK,FTM_OUTMASK_CH2OM_MASK,
			FTM_OUTMASK_CH3OM_MASK,FTM_OUTMASK_CH4OM_MASK, FTM_OUTMASK_CH5OM_MASK,
			FTM_OUTMASK_CH6OM_MASK,FTM_OUTMASK_CH7OM_MASK};
	uint32_t CHANNELS_OUT_HIGH[]= {FTM_OUTMASK_CH0OM_MASK<<8,FTM_OUTMASK_CH1OM_MASK<<8,FTM_OUTMASK_CH2OM_MASK<<8,
				FTM_OUTMASK_CH3OM_MASK<<8,FTM_OUTMASK_CH4OM_MASK<<8, FTM_OUTMASK_CH5OM_MASK<<8,
				FTM_OUTMASK_CH6OM_MASK<<8,FTM_OUTMASK_CH7OM_MASK<<8};
	ftms[module]->SWOCTRL |= CHANNELS_OUT_MASK[channel];
	if (exit== true)
		ftms[module]->SWOCTRL |= CHANNELS_OUT_HIGH[channel];
	else
		ftms[module]->SWOCTRL = (ftms[module]->SWOCTRL & (~CHANNELS_OUT_HIGH[channel]) );
}

void ftm_disable_constant_ch_output (ftm_modules_t module,ftm_channel_t channel){
	uint32_t CHANNELS_OUT_MASK[]= {FTM_OUTMASK_CH0OM_MASK,FTM_OUTMASK_CH1OM_MASK,FTM_OUTMASK_CH2OM_MASK,
				FTM_OUTMASK_CH3OM_MASK,FTM_OUTMASK_CH4OM_MASK, FTM_OUTMASK_CH5OM_MASK,
				FTM_OUTMASK_CH6OM_MASK,FTM_OUTMASK_CH7OM_MASK};
	ftms[module]->SWOCTRL &= ~CHANNELS_OUT_MASK[channel];
}

/*PWM config*/
void ftm_set_pwm_conf(ftm_modules_t module, ftm_pwm_config_t config){
	//Disable DMA transfer data
	(ftms[module]->CONTROLS[config.channel].CnSC) &= ~FTM_CnSC_DMA_MASK;

	/*Enables the loading of the MOD, CNTIN, C(n)V, and C(n+1)V registers
	 *  with the values of their write buffers when the FTM counter changes
	 *   from the MOD register value to its next value or when a channel (j) match occurs
	 */
	ftms[module]->PWMLOAD = FTM_PWMLOAD_LDOK_MASK | 0x0F;
	/*	SWSOC
	* Software output control synchronization is activated by the software trigger.
	*	0 The software trigger does not activate the SWOCTRL register synchronization.
	*	1 The software trigger activates the SWOCTRL register synchronization.
	*/
	ftms[module]->SYNCONF |= FTM_SYNCONF_SWWRBUF_MASK | FTM_SYNCONF_SWSOC_MASK;
	/*
	 * PWM Synchronization Software Trigger
	 * Selects the software trigger as the PWM synchronization trigger. The software trigger happens when a 1 is
	 * written to SWSYNC bit.
	 * 0 Software trigger is not selected.
	 * 1 Software trigger is selected.
	 */
	ftms[module]->SYNC |= FTM_SYNC_SWSYNC_MASK;
	//Se realizo una primera sincronización para que quede con los valores iniciales

	// QUADEN = 0
	ftms[module]->QDCTRL &= ~FTM_QDCTRL_QUADEN_MASK;


	//DECAPEN = 0 and COMBINE=0, necesary to be in PWM mode
	ftms[module]->COMBINE &= ~(pwm_decapen_masks[module]|pwm_combine_masks[module]);

	/*Mascaras necesarias para activar el mecanismo que permite la sincronizacion de
	 * los registros MOD, CNTIN, CnV, OUTMASK, INVCTRL,SWOCTRL*/
	uint32_t SYNCEN_MASK[]= {FTM_COMBINE_SYNCEN0_MASK, FTM_COMBINE_SYNCEN1_MASK,
			FTM_COMBINE_SYNCEN2_MASK, FTM_COMBINE_SYNCEN3_MASK};
	ftms[module]->COMBINE|=SYNCEN_MASK[module];

	/*Bit needen for this mode*/
	if(config.mode == FTM_PWM_CENTER_ALIGNED){
		ftms[module]->CONTROLS[config.channel].CnSC|=FTM_CnSC_ELSB_MASK;
		ftms[module]->SC |= FTM_SC_CPWMS_MASK;		//centered PWM enable
	}
	/*Bit needed for active the PWM mode*/
	else if(config.mode == FTM_PWM_EDGE_ALIGNED){
		ftms[module]->SC &= ~FTM_SC_CPWMS_MASK;			//centered PWM disabled.
		// edge alligned, high true pulses
		ftms[module]->CONTROLS[config.channel].CnSC |= FTM_CnSC_MSB_MASK | FTM_CnSC_ELSB_MASK;
	}

	write_mod_value(module , config.mod);
	ftms[module]->CONTROLS[config.channel].CnV=config.CnV;


	ftms[module]->CNTIN = 0;		//resets counter value.

	//Enable DMA transfer data
	(ftms[module]->CONTROLS[config.channel].CnSC) |= FTM_CnSC_DMA_MASK;
}

/*El duty cycle se lo pone como si estuviera como porcentaje siendo 100 el maximo*/
void ftm_set_pwm_duty_cycle(ftm_modules_t module, ftm_channel_t channel,uint8_t duty_cycle){
	float new_cnv = ((float)duty_cycle / 100.0) * (float)(ftms[module]->MOD & FTM_MOD_MOD_MASK);
	ftms[module]->CONTROLS[channel].CnV = FTM_CnV_VAL((uint16_t)new_cnv);
}

/*IRQS*/
__ISR__ FTM0_IRQHandler(void){
	ftms[FTM_0]->CONTROLS[0].CnSC &=  ~FTM_CnSC_CHF_MASK;
	gpioToggle(PORTNUM2PIN(PC,8));			  //GPIO pin PTC8;
}

__ISR__ FTM1_IRQHandler(void){
	ftms[FTM_1]->CONTROLS[1].CnSC &=  ~FTM_CnSC_CHF_MASK;
}
__ISR__ FTM2_IRQHandler(void){
	ftms[FTM_2]->CONTROLS[1].CnSC &=  ~FTM_CnSC_CHF_MASK;
}
