/*******************************************************************************
  @file     SPI_wrapper.h
  @author   Grupo 5
 ******************************************************************************/

#ifndef SPI_WRAPPER_H_
#define SPI_WRAPPER_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdint.h>

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

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

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

void SPI_Init(spi_id_t id, spi_slave_t slave, uint32_t baudrate);

void SPI_Send(spi_id_t id, spi_slave_t slave, const char *msg, uint16_t len, void (*end_callback)(void));

void SPI_Config(spi_id_t id, spi_slave_t slave, uint32_t baudrate); // Update the clock

#endif /* SPI_WRAPPER_H_ */
