/*
 * encoderHal.h
 *
 *  Created on: 14 oct. 2020
 *  Author: Paulo
 */

#ifndef SOURCES_ENCODERHAL_H_
#define SOURCES_ENCODERHAL_H_

#include <stdbool.h>
#include <stdint.h>

/******************************************************************************************
 *  				TIPOS DE DATOS, CONSTANTES Y VARIABLES GLOBALES
 ******************************************************************************************/
typedef enum {A, B, C, ENC_SIGNAL_COUNT}encoderSignal;
typedef void (*callback_ptr)(void);


/********************************************************************************************
 * 										FUNCIONES
 ********************************************************************************************/

void initEncoderHal(void (*funcallback)(void));
void setButtonCallback(void (*funcallback)(void));

// funciones para el encoder genéricos
bool readEncoder(encoderSignal);
void resetEncoderTimerCount(void);
uint8_t getEncoderTimerCount(void);
void encoderTimerRoutine(void);

#endif /* SOURCES_ENCODERHAL_H_ */
