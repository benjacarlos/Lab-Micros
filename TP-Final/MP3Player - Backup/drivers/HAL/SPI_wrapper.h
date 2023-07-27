/*
 * SPI_wrapper.h
 *
 *  Created on: 16 ene. 2021
 *      Author: Santi
 */

#ifndef SPI_WRAPPER_H_
#define SPI_WRAPPER_H_

#include <stdint.h>

typedef enum
{
  SPI_0_ID,
  SPI_1_ID,
  SPI_2_ID,
} spi_id_t;

typedef enum
{
  SPI_SLAVE_0 = 0,
  SPI_SLAVE_1 = 4,
  SPI_SLAVE_2 = 3,
  SPI_SLAVE_3 = 2,
  SPI_SLAVE_4 = 1,
  SPI_SLAVE_5 = 23
} spi_slave_t;

void SPI_Init(spi_id_t id, spi_slave_t slave, uint32_t baudrate);

void SPI_Send(spi_id_t id, spi_slave_t slave, const char *msg, uint16_t len, void (*end_callback)(void));

void SPI_Config(spi_id_t id, spi_slave_t slave, uint32_t baudrate); // Update the clock

#endif /* SPI_WRAPPER_H_ */
