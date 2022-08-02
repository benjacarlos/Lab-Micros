/***************************************************************************/ /**
  @file     FTM.h
  @brief    FTM Header
  @author   Grupo 2 - Lab de Micros
 ******************************************************************************/
#ifndef FTM_H_
#define FTM_H_

#include <stdint.h>
#include <stdbool.h>

#define FTM_DMA_SOURCE	20

typedef enum
{
	FTM_mInputCapture,
	FTM_mOutputCompare,
	FTM_mPulseWidthModulation
} FTMMode_t;

typedef enum
{
	FTM_eRising = 0x01,
	FTM_eFalling = 0x02,
	FTM_eEither = 0x03
} FTMEdge_t;

typedef enum
{
	FTM_eToggle = 0x01,
	FTM_eClear = 0x02,
	FTM_eSet = 0x03
} FTMEffect_t;

typedef enum
{
	FTM_lAssertedHigh = 0x02,
	FTM_lAssertedLow = 0x03
} FTMLogic_t;

typedef enum
{
	FTM_PSC_x1 = 0x00,
	FTM_PSC_x2 = 0x01,
	FTM_PSC_x4 = 0x02,
	FTM_PSC_x8 = 0x03,
	FTM_PSC_x16 = 0x04,
	FTM_PSC_x32 = 0x05,
	FTM_PSC_x64 = 0x06,
	FTM_PSC_x128 = 0x07,

} FTM_Prescal_t;

typedef enum
{
	FTM_CH_0,
	FTM_CH_1,
	FTM_CH_2,
	FTM_CH_3,
	FTM_CH_4,
	FTM_CH_5,
	FTM_CH_6,
	FTM_CH_7
} FTM_Channel_t;

typedef enum
{
	FTM_DMA_OFF,
	FTM_DMA_ON
} FTM_DmaMode_t;

typedef uint8_t FTM_t; // id
typedef uint16_t FTMData_t;

//void 		FTM_Init(void);
void PWM_Init(FTM_t id, FTM_Channel_t channel, FTM_Prescal_t prescaler,
			  uint8_t port_ftm, uint8_t pin_ftm, uint8_t pin_alt, FTMLogic_t logic,
			  uint16_t pwm_mod, uint16_t pwm_duty, FTM_DmaMode_t is_dma);

void OVF_Init(FTM_t, FTM_Channel_t, FTM_Prescal_t, uint8_t port_out, uint8_t pin_out,
			  uint32_t init, uint32_t mod);

void FTM_SetPrescaler(FTM_t, FTM_Prescal_t);
void FTM_SetModulus(FTM_t, FTMData_t);
FTMData_t FTM_GetModulus(FTM_t);

void FTM_StartClock(FTM_t);
void FTM_StopClock(FTM_t);

void FTM_SetOverflowMode(FTM_t, bool);
bool FTM_IsOverflowPending(FTM_t);
void FTM_ClearOverflowFlag(FTM_t);

void FTM_SetWorkingMode(FTM_t, FTM_Channel_t, FTMMode_t);
FTMMode_t FTM_GetWorkingMode(FTM_t, FTM_Channel_t);
void FTM_SetInputCaptureEdge(FTM_t, FTM_Channel_t, FTMEdge_t);
FTMEdge_t FTM_GetInputCaptureEdge(FTM_t, FTM_Channel_t);
void FTM_SetOutputCompareEffect(FTM_t, FTM_Channel_t, FTMEffect_t);
FTMEffect_t FTM_GetOutputCompareEffect(FTM_t, FTM_Channel_t);
void FTM_SetPulseWidthModulationLogic(FTM_t, FTM_Channel_t, FTMLogic_t);
FTMLogic_t FTM_GetPulseWidthModulationLogic(FTM_t, FTM_Channel_t);

void FTM_SetCounter(FTM_t, FTM_Channel_t, FTMData_t);
FTMData_t FTM_GetCounter(FTM_t, FTM_Channel_t);

void FTM_SetInterruptMode(FTM_t, FTM_Channel_t, bool);
bool FTM_IsInterruptPending(FTM_t, FTM_Channel_t);
void FTM_ClearInterruptFlag(FTM_t, FTM_Channel_t);

void FTM_DmaMode(FTM_t id, FTM_Channel_t channel, bool dma_mode);
void FTM_ToggleOM(FTM_t id, FTM_Channel_t channel);
void FTM_onOM(FTM_t id, FTM_Channel_t channel);
void FTM_offOM(FTM_t id, FTM_Channel_t channel);
uint32_t FTM_GetCnVAddress(FTM_t id, FTM_Channel_t channel);

#endif /* FTM_H_ */
