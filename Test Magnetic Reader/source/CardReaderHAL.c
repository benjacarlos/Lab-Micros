/*
 * CardReaderHAL.c
 *
 *  Created on: Oct 12, 2020
 *      Author: Grupo 5
 */

/*******************************************************************************
 * HEADERS
 ******************************************************************************/


#include "CardReaderHAL.h"
#include "gpio.h"
#include <stddef.h>

/*******************************************************************************
 * CONSTANTES Y MACROS USANDO DEFINE
 ******************************************************************************/

#define DATA_PIN PORTNUM2PIN(PC, 5)  //cable azul
#define CLK_PIN PORTNUM2PIN(PC, 7) //cable verde
#define ENABLE_PIN PORTNUM2PIN(PC, 0) //cable amarillo



/*******************************************************************************
 * STRUCTURAS Y TYPEDEFS
 ******************************************************************************/

typedef struct{

	int topofthequeue;
	CardEncodedData_t encodedData[MAX_NUM_ITEMS][DATA_LEN];
	bool empty;
}cardReaderQueue_t;


/*******************************************************************************
 * PROTOTIPOS FUNCIONES LOCALES
 ******************************************************************************/

void clockfun(void);

void enablefun(void);

void Queue_Init(void);

/*******************************************************************************
 * VARIABLES ESTÁTICAS
 ******************************************************************************/

static bool read_enable = false;

static uint8_t save_items = 0;
static uint32_t data_counter = 0;

static cardReaderQueue_t reader_queue;

/*******************************************************************************
 *******************************************************************************
                       DEFINIICON FUNCIONES DEL HEADER
 *******************************************************************************
 ******************************************************************************/


void CardReaderHW_Init(void)
{
	static bool hw_Init = false;
	if (!hw_Init)
	{
		//Se configuran los puertos de las señales del lector
		gpioMode(DATA_PIN , INPUT_PULLUP);
		gpioMode(CLK_PIN, INPUT_PULLUP);
		gpioMode(ENABLE_PIN, INPUT_PULLUP);

		gpioIRQ(ENABLE_PIN, GPIO_IRQ_MODE_BOTH_EDGES, (pinIrqFun_t) enablefun);

		read_enable = false;
	}
	Queue_Init();

}

/*******************************************************************************
 *******************************************************************************
                        DEFINICION FUNCIONES LOCALES
 *******************************************************************************
 ******************************************************************************/

void clockfun(void)
{
	//Si todavia hay lugar para que entren mas datos siguen entrando a la cola
	if (data_counter < DATA_LEN)
	{
		reader_queue.encodedData[save_items][data_counter] = !gpioRead(DATA_PIN); //es activo bajo (PC5)
		data_counter++;
	}

}

void enablefun(void)
{
	read_enable = !read_enable; //comienza falso, si es verdadero empieza a leer.

	if(read_enable)
	{
		data_counter = 0;
		gpioIRQ(CLK_PIN, GPIO_IRQ_MODE_FALLING_EDGE, (pinIrqFun_t) clockfun); //levanto interrupciones del puerto de clk
	}
	else
	{

		gpioIRQ(CLK_PIN, GPIO_IRQ_MODE_DISABLE, (pinIrqFun_t) clockfun); //desactivo las interrupciones si no se esta leyendo

		//Ahora no esta vacia la cola porque guarde los datos que levante del lector
		reader_queue.topofthequeue = save_items;
		reader_queue.empty = false;

		//Se asegura de iterar en la variable save_items mientras la misma no esta en el tope de la cola
		if(save_items < (MAX_NUM_ITEMS - 1))
		{
			save_items++;
		}


	}

}

void Queue_Init(void)
{

	reader_queue.empty = true;
	reader_queue.topofthequeue = 0;

}


CardEncodedData_t * get_buffer(void)
{
	CardEncodedData_t *buffer;
	if (reader_queue.empty)
	{
		buffer= NULL;
	}
	else
	{
		buffer = &(reader_queue.encodedData[reader_queue.topofthequeue][0]); //hago que el puntero que voy a retornar apunte a la data que lei

		//veo si esta vacia o no la cola
		if (reader_queue.topofthequeue == 0)
		{
			reader_queue.empty = true;
			save_items = 0;
		}
		else
		{
			//cuando no esta vacia, como mi puntero apunta a la ultima data que tengo disminuyo mis variables para enviar dsps los datos mas recientes.
			reader_queue.topofthequeue--;
			save_items--;
		}
	}

	return buffer;
}


bool queue_not_empty(void)
{
	return !reader_queue.empty;
}
