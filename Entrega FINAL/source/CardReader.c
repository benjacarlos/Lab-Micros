/*
 * 	file: CardReader.c
 *  Trabajo Práctico 1 - Interrupciones
 *  GRUPO 5 - Laboratorio de Microprocesadores
 */

/*******************************************************************************
 * 								HEADERS
 ******************************************************************************/

#include <CardReader.h>
#include "CardReaderHAL.h"
#include "CardReaderDecoder.h"
#include <stdbool.h>

/*******************************************************************************
 * 								FUNCIONES
 ******************************************************************************/

void Cardreader_Init(void)
{
	CardReaderHW_Init();
}


bool lecture_ready(void)
{
	return queue_not_empty();
}

readerData_t get_decoded(void)
{
	readerData_t data;

	if(queue_not_empty())
	{
		if(dataParser(get_buffer(), data.track_string, &(data.track_id)))
		{
			data.isValid = true;
		}
		else
		{
			data.isValid = false;
		}
	}

	return data;
}
