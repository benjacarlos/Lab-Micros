/***************************************************************************/ /**
  @file     FTM.c
  @brief    FTM driver
  @author   Daniel Jacoby y Grupo 2 - Lab de Micros
 ******************************************************************************/

#include "ftm.h"
#include "MK64F12.h"

volatile FTM_Type *p_ftm[] = FTM_BASE_PTRS;
volatile PORT_Type *p_ports[] = PORT_BASE_PTRS;

void PWM_Init(FTM_t id, FTM_Channel_t channel, FTM_Prescal_t prescaler,
			  uint8_t port_ftm, uint8_t pin_ftm, uint8_t pin_alt, FTMLogic_t logic,
			  uint16_t pwm_mod, uint16_t pwm_duty, FTM_DmaMode_t is_dma)
{
	FTM_Type *ftm = p_ftm[id];
	PORT_Type *port = p_ports[port_ftm];

	// CLock gatting ans enable irq

	SIM->SCGC5 |= (SIM_SCGC5_PORTA_MASK << port_ftm);
	NVIC_EnableIRQ(PORTA_IRQn + port_ftm);

	if (id <= 2)
	{
		SIM->SCGC6 |= (SIM_SCGC6_FTM0_MASK << id);
		NVIC_EnableIRQ(FTM0_IRQn + id);
	}
	else if (id == 3)
	{
		SIM->SCGC3 |= SIM_SCGC3_FTM3_MASK;
		NVIC_EnableIRQ(FTM3_IRQn);
	}
	ftm->PWMLOAD = FTM_PWMLOAD_LDOK_MASK | 0x0F;

	port->PCR[pin_ftm] = false;
	port->PCR[pin_ftm] = PORT_PCR_DSE(true);
	port->PCR[pin_ftm] = PORT_PCR_MUX(pin_alt);

	FTM_SetPrescaler(id, prescaler);
	FTM_SetInterruptMode(id, channel, true);

	FTM_SetWorkingMode(id, channel, FTM_mPulseWidthModulation);
	FTM_SetPulseWidthModulationLogic(id, channel, logic);

	FTM_SetModulus(id, pwm_mod);
	FTM_SetCounter(id, channel, pwm_duty);

	FTM_DmaMode(id, channel, is_dma ? FTM_DMA_ON : FTM_DMA_OFF); // DMA ON

}

void FTM_SetPrescaler(FTM_t id, FTM_Prescal_t data)
{
	FTM_Type *ftm = p_ftm[id];
	ftm->SC = (ftm->SC & ~FTM_SC_PS_MASK) | FTM_SC_PS(data);
}

void FTM_SetModulus(FTM_t id, FTMData_t data)
{
	FTM_Type *ftm = p_ftm[id];
	ftm->CNTIN = 0X00;
	ftm->CNT = 0X00;
	ftm->MOD = FTM_MOD_MOD(data);
}

FTMData_t FTM_GetModulus(FTM_t id)
{
	FTM_Type *ftm = p_ftm[id];
	return ftm->MOD & FTM_MOD_MOD_MASK;
}

void FTM_StartClock(FTM_t id)
{
	FTM_Type *ftm = p_ftm[id];
	ftm->SC |= FTM_SC_CLKS(0x01);
}

void FTM_StopClock(FTM_t id)
{
	FTM_Type *ftm = p_ftm[id];
	ftm->SC &= ~FTM_SC_CLKS(0x01);
}

void FTM_SetOverflowMode(FTM_t id, bool mode)
{
	FTM_Type *ftm = p_ftm[id];
	ftm->SC = (ftm->SC & ~FTM_SC_TOIE_MASK) | FTM_SC_TOIE(mode);
}

bool FTM_IsOverflowPending(FTM_t id)
{
	FTM_Type *ftm = p_ftm[id];
	return ftm->SC & FTM_SC_TOF_MASK;
}

void FTM_ClearOverflowFlag(FTM_t id)
{
	FTM_Type *ftm = p_ftm[id];
	ftm->SC &= ~FTM_SC_TOF_MASK;
}

void FTM_SetWorkingMode(FTM_t id, FTM_Channel_t channel, FTMMode_t mode)
{
	FTM_Type *ftm = p_ftm[id];
	ftm->CONTROLS[channel].CnSC = (ftm->CONTROLS[channel].CnSC & ~(FTM_CnSC_MSB_MASK | FTM_CnSC_MSA_MASK)) |
								  (FTM_CnSC_MSB((mode >> 1) & 0X01) | FTM_CnSC_MSA((mode >> 0) & 0X01));
}

FTMMode_t FTM_GetWorkingMode(FTM_t id, FTM_Channel_t channel)
{
	FTM_Type *ftm = p_ftm[id];
	return (ftm->CONTROLS[channel].CnSC & (FTM_CnSC_MSB_MASK | FTM_CnSC_MSA_MASK)) >> FTM_CnSC_MSA_SHIFT;
}

void FTM_SetInputCaptureEdge(FTM_t id, FTM_Channel_t channel, FTMEdge_t edge)
{
	FTM_Type *ftm = p_ftm[id];
	ftm->CONTROLS[channel].CnSC = (ftm->CONTROLS[channel].CnSC & ~(FTM_CnSC_ELSB_MASK | FTM_CnSC_ELSA_MASK)) |
								  (FTM_CnSC_ELSB((edge >> 1) & 0X01) | FTM_CnSC_ELSA((edge >> 0) & 0X01));
}

FTMEdge_t FTM_GetInputCaptureEdge(FTM_t id, FTM_Channel_t channel)
{
	FTM_Type *ftm = p_ftm[id];
	return (ftm->CONTROLS[channel].CnSC & (FTM_CnSC_ELSB_MASK | FTM_CnSC_ELSA_MASK)) >> FTM_CnSC_ELSA_SHIFT;
}

void FTM_SetOutputCompareEffect(FTM_t id, FTM_Channel_t channel, FTMEffect_t effect)
{
	FTM_Type *ftm = p_ftm[id];
	ftm->CONTROLS[channel].CnSC = (ftm->CONTROLS[channel].CnSC & ~(FTM_CnSC_ELSB_MASK | FTM_CnSC_ELSA_MASK)) |
								  (FTM_CnSC_ELSB((effect >> 1) & 0X01) | FTM_CnSC_ELSA((effect >> 0) & 0X01));
}

FTMEffect_t FTM_GetOutputCompareEffect(FTM_t id, FTM_Channel_t channel)
{
	FTM_Type *ftm = p_ftm[id];
	return (ftm->CONTROLS[channel].CnSC & (FTM_CnSC_ELSB_MASK | FTM_CnSC_ELSA_MASK)) >> FTM_CnSC_ELSA_SHIFT;
}

void FTM_SetPulseWidthModulationLogic(FTM_t id, FTM_Channel_t channel, FTMLogic_t logic)
{
	FTM_Type *ftm = p_ftm[id];
	ftm->CONTROLS[channel].CnSC = (ftm->CONTROLS[channel].CnSC & ~(FTM_CnSC_ELSB_MASK | FTM_CnSC_ELSA_MASK)) |
								  (FTM_CnSC_ELSB((logic >> 1) & 0X01) | FTM_CnSC_ELSA((logic >> 0) & 0X01));
}

FTMLogic_t FTM_GetPulseWidthModulationLogic(FTM_t id, FTM_Channel_t channel)
{
	FTM_Type *ftm = p_ftm[id];
	return (ftm->CONTROLS[channel].CnSC & (FTM_CnSC_ELSB_MASK | FTM_CnSC_ELSA_MASK)) >> FTM_CnSC_ELSA_SHIFT;
}

void FTM_SetCounter(FTM_t id, FTM_Channel_t channel, FTMData_t data)
{
	FTM_Type *ftm = p_ftm[id];
	ftm->CONTROLS[channel].CnV = FTM_CnV_VAL(data);
}

FTMData_t FTM_GetCounter(FTM_t id, FTM_Channel_t channel)
{
	FTM_Type *ftm = p_ftm[id];
	return ftm->CONTROLS[channel].CnV & FTM_CnV_VAL_MASK;
}

void FTM_SetInterruptMode(FTM_t id, FTM_Channel_t channel, bool mode)
{
	FTM_Type *ftm = p_ftm[id];
	ftm->CONTROLS[channel].CnSC = (ftm->CONTROLS[channel].CnSC & ~FTM_CnSC_CHIE_MASK) | FTM_CnSC_CHIE(mode);
}

bool FTM_IsInterruptPending(FTM_t id, FTM_Channel_t channel)
{
	FTM_Type *ftm = p_ftm[id];
	return ftm->CONTROLS[channel].CnSC & FTM_CnSC_CHF_MASK;
}

void FTM_ClearInterruptFlag(FTM_t id, FTM_Channel_t channel)
{
	FTM_Type *ftm = p_ftm[id];
	ftm->CONTROLS[channel].CnSC &= ~FTM_CnSC_CHF_MASK;
}

void FTM_DmaMode(FTM_t id, FTM_Channel_t channel, bool dma_mode)
{
	FTM_Type *ftm = p_ftm[id];
	ftm->CONTROLS[channel].CnSC = (ftm->CONTROLS[channel].CnSC & ~(FTM_CnSC_DMA_MASK)) |
								  (FTM_CnSC_DMA(dma_mode));
}

void FTM_ToggleOM(FTM_t id, FTM_Channel_t channel)
{
	FTM_Type *ftm = p_ftm[id];
	uint32_t mask = (FTM_OUTMASK_CH0OM_MASK << (uint8_t)channel);
	bool state = ftm->OUTMASK & mask;
	state = !state;
	ftm->OUTMASK = (ftm->OUTMASK & ~mask) | (((uint32_t)(state) << (uint8_t)channel) & mask);
}

void FTM_onOM(FTM_t id, FTM_Channel_t channel)
{
	FTM_Type *ftm = p_ftm[id];
	uint32_t mask = (FTM_OUTMASK_CH0OM_MASK << (uint8_t)channel);
	ftm->OUTMASK = (ftm->OUTMASK & ~mask) | ((1 << (uint8_t)channel) & mask);
}


void FTM_offOM(FTM_t id, FTM_Channel_t channel)
{
	FTM_Type *ftm = p_ftm[id];
		uint32_t mask = (FTM_OUTMASK_CH0OM_MASK << (uint8_t)channel);
		ftm->OUTMASK = (ftm->OUTMASK & ~mask) ;
}

uint32_t FTM_GetCnVAddress(FTM_t id, FTM_Channel_t channel)
{
	FTM_Type *ftm = p_ftm[id];
	return (uint32_t)(&(ftm->CONTROLS[channel].CnV));
}
