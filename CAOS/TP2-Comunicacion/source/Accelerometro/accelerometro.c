/*
 * acelerometro.c
 *
 *  Created on: 28 Nov 2020
 *      Author: benja
 */

#include "accelerometro.h"
#include "timer.h"
#include "timerqueue.h"

/******************************************************************************
 *									DEFINICIONES
 ******************************************************************************/

#define ACCEL_ADDRESS	0x1D
#define ACCEL_DATA_PACK_LEN	13


// ACCEL I2C address
#define ACCEL_SLAVE_ADDR 0x1D	//default


//Registros internos
#define ACCEL_STATUS 0x00
#define ACCEL_WHOAMI 0x0D
#define ACCEL_XYZ_DATA_CFG 0x0E
#define ACCEL_CTRL_REG1 0x2A
#define ACCEL_M_CTRL_REG1 0x5B
#define ACCEL_M_CTRL_REG2 0x5C
#define ACCEL_WHOAMI_VAL 0xC7

//Numero de bytes que puede leer
#define ACCEL_READ_LEN 13

typedef enum { I2C_ERROR, I2C_OK} accel_errors_t;

/******************************************************************************
 *									VARIABLES
 ******************************************************************************/

static unsigned char reading_buffer[ACCEL_DATA_PACK_LEN];
static accel_data_t last_read_data_mag;
static accel_data_t last_read_data_acc;

/********************************************************************************************
 * 										FUNCIONES
 ********************************************************************************************/

static void handling_reading_calls();
static void write_reg(unsigned char reg, unsigned char data);

static void handling_read();
static accel_errors_t start();

void accel_init()
{
	static bool initialized = false;
	if(initialized) return;

	i2c_master_int_init(I2C0_INT_MOD);

	timerInit();
	timerqueueInit();

	while(start() == I2C_ERROR);
	timerStart(ACCEL_R, TIMER_HZ_RELOAD(5), &handling_read);
	timerStart(ACCEL_RC, TIMER_HZ_RELOAD(2), &handling_reading_calls);

	initialized = true;

}

//FXOS8700CQ REFERENCE MANUAL, SECTION 13.4
//Configura FXOS8700CQ
static accel_errors_t start()
{

	unsigned char question = ACCEL_WHOAMI;

	i2c_master_int_set_slave_addr(I2C0_INT_MOD, ACCEL_SLAVE_ADDR);
	i2c_master_int_read_data(I2C0_INT_MOD, &question, 1 , 1);

	unsigned char data[2] = {0, 0};

	while(!i2c_master_int_has_new_data(I2C0_INT_MOD));

	if (i2c_master_int_get_new_data_length(I2C0_INT_MOD) != 1)
		return (I2C_ERROR); // read and check the FXOS8700CQ WHOAMI register
	else
	{
		i2c_master_int_get_new_data(I2C0_INT_MOD, data, 1);
		if(data[0] != ACCEL_WHOAMI_VAL)
			return (I2C_ERROR);
	}

	while(i2c_master_int_bus_busy(I2C0_INT_MOD));
	write_reg(ACCEL_M_CTRL_REG1, 0x00);

	while(i2c_master_int_bus_busy(I2C0_INT_MOD));
	write_reg(ACCEL_M_CTRL_REG1, 0x1F);

	while(i2c_master_int_bus_busy(I2C0_INT_MOD));
	write_reg(ACCEL_M_CTRL_REG2, 0x20);

	while(i2c_master_int_bus_busy(I2C0_INT_MOD));
	write_reg(ACCEL_XYZ_DATA_CFG, 0x01);

	while(i2c_master_int_bus_busy(I2C0_INT_MOD));
	write_reg(ACCEL_CTRL_REG1, 0x0D);

	while(i2c_master_int_bus_busy(I2C0_INT_MOD));

	return I2C_OK;
}



static void handling_read()
{
	//Lee lo que hay en el buffer para el accl dado por el i2c
	while(i2c_master_int_has_new_data(I2C0_INT_MOD) && (i2c_master_int_get_new_data_length(I2C0_INT_MOD) >= ACCEL_DATA_PACK_LEN))
	{
		i2c_master_int_get_new_data(I2C0_INT_MOD, reading_buffer, ACCEL_DATA_PACK_LEN);

		//Primer byte ignorado
		//accelerometer data : serial... 14 bits
		last_read_data_acc.x = (int16_t)((reading_buffer[1] << 8) | reading_buffer[2])>> 2;
		last_read_data_acc.y = (int16_t)((reading_buffer[3] << 8) | reading_buffer[4])>> 2;
		last_read_data_acc.z = (int16_t)((reading_buffer[5] << 8) | reading_buffer[6])>> 2;

		//magnetometer data : serial... 16 bits
		last_read_data_mag.x = (reading_buffer[7] << 8) | reading_buffer[8];
		last_read_data_mag.y = (reading_buffer[9] << 8) | reading_buffer[10];
		last_read_data_mag.z = (reading_buffer[11] << 8) | reading_buffer[12];
	}
}

static void handling_reading_calls()
{

	unsigned char read_addr = ACCEL_STATUS;
	if(!i2c_master_int_bus_busy(I2C0_INT_MOD))
	{
		timerDisable(ACCEL_R);		//Mientras leo no updateo
		i2c_master_int_read_data(I2C0_INT_MOD, &read_addr, 1, ACCEL_DATA_PACK_LEN);
		timerEnable(ACCEL_R);
	}
}

static void write_reg(unsigned char reg, unsigned char data)
{
	unsigned char reg_data[2] = {reg, data};
	i2c_master_int_write_data(I2C0_INT_MOD, reg_data, 2);
}

accel_data_t accel_get_last_data(accel_options_t data_option)
{
	accel_data_t returnable;

	if(data_option == ACCEL_ACCEL_DATA)
		returnable = last_read_data_acc;
	else if(data_option == ACCEL_MAGNET_DATA)
		returnable = last_read_data_mag;

	return returnable;
}

