/*
 * FTM.h
 *
 *  Created on: 24 nov. 2020
 *      Author: Juan Martin
 */

#ifndef FTM_H_
#define FTM_H_

/*Includes*/
#include "MK64F12.h"
#include <stdbool.h>

/*Variables*/
typedef enum{FTM_0, FTM_1, FTM_2, FTM_3, FTM_N_MODULES} ftm_modules_t;
typedef enum{ FTM_PSC_x1= 0x00, FTM_PSC_x2, FTM_PSC_x4, FTM_PSC_x8, FTM_PSC_x16,
			  FTM_PSC_x32, FTM_PSC_x64, FTM_PSC_x128} ftm_prescaler_t;
typedef enum{ FTM_SYSTEM_CLK = 0x01, 	//Bus clock 50MHz
				FTM_FIXED_FREQ_CLK , FTM_EXTERN_CLK = 0x03
			  }ftm_clk_src_t;
typedef enum{FTM_CH_0,FTM_CH_1,FTM_CH_2,FTM_CH_3,FTM_CH_4,FTM_CH_5,FTM_CH_6,FTM_CH_7}ftm_channel_t;
typedef enum{FTM_PWM_CENTER_ALIGNED, FTM_PWM_EDGE_ALIGNED, FTM_PWM_COMBINED }ftm_pwm_mode_t;
static FTM_Type * const ftms[FTM_N_MODULES] = FTM_BASE_PTRS;
#define X FTM_OUTMASK_CH0OM_MASK<<8;
typedef struct{
	ftm_channel_t channel;
	ftm_pwm_mode_t mode;
	uint16_t mod;
	uint8_t CnV;
}ftm_pwm_config_t;

/*Funciones*/
void ftm_init(ftm_modules_t module, ftm_prescaler_t prescaler_config);
void ftm_start_clk(ftm_modules_t module, ftm_clk_src_t source);
void ftm_stop_clock(ftm_modules_t module);
void ftm_enable_overflow_irq(ftm_modules_t module, bool enable_disable);
bool ftm_has_overflowed(ftm_modules_t module);
void ftm_clear_overflow_flag (ftm_modules_t module);
void ftm_reset_counter_value(ftm_modules_t module);
uint16_t ftm_read_counter_value(ftm_modules_t module);
void write_mod_value(ftm_modules_t module , uint16_t mod_value);
uint16_t ftm_get_mod_value(ftm_modules_t module);
void ftm_constant_ch_output(ftm_modules_t module,ftm_channel_t channel, bool exit);
void ftm_disable_constant_ch_output (ftm_modules_t module,ftm_channel_t channel);
void ftm_set_pwm_conf(ftm_modules_t module, ftm_pwm_config_t config);
void ftm_set_pwm_duty_cycle(ftm_modules_t module, ftm_channel_t channel,uint8_t duty_cycle);

#endif /* FTM_H_ */
