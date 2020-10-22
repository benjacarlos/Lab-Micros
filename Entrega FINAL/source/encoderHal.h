/*
 * 	file: encoderHal.h
 *  Trabajo Práctico 1 - Interrupciones
 *  GRUPO 5 - Laboratorio de Microprocesadores
 */

#ifndef SOURCES_ENCODERHAL_H_
#define SOURCES_ENCODERHAL_H_

/*******************************************************************************
 * 								HEADERS
 ******************************************************************************/

#include <stdbool.h>
#include <stdint.h>

/******************************************************************************
 *							  DEFINICIONES
 ******************************************************************************/

typedef enum {A, B, C, ENC_SIGNAL_COUNT}encoderSignal;
typedef void (*callback_ptr)(void);

/******************************************************************************
 *						DECLARO FUNCIONES DEL HEADER
 ******************************************************************************/

void initEncoderHal(void (*funcallback)(void));
void setButtonCallback(void (*funcallback)(void));

// funciones para el encoder genéricos
bool readEncoder(encoderSignal);
void resetEncoderTimerCount(void);
uint8_t getEncoderTimerCount(void);
void encoderTimerRoutine(void);

#endif /* SOURCES_ENCODERHAL_H_ */
