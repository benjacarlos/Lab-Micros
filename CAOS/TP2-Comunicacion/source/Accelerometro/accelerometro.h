/*
 * acelerometro.h
 *
 *  Created on: 28 Nov 2020
 *      Author: benja
 */

//Recolecta data de un sensor accelerometro

#ifndef ACCELEROMETRO_ACCELEROMETRO_H_
#define ACCELEROMETRO_ACCELEROMETRO_H_

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

//Opciones
typedef enum {ACCEL_ACCEL_DATA, ACCEL_MAGNET_DATA} accel_options_t;

//Datos del sensor
typedef struct {
	int16_t x;
	int16_t y;
	int16_t z;
} accel_data_t;


void accel_init();

//De vuelve valores actualizados del sensor
accel_data_t accel_get_last_data(accel_options_t data_option);


#endif /* ACCELEROMETRO_ACCELEROMETRO_H_ */
