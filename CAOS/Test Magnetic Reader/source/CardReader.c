/*
 * CardReader.c
 *
 *  Created on: Oct 12, 2020
 *      Author: Grupo 5
 */


#include "CardReader.h"
#include "CardReaderHAL.h"
#include "CardReaderDecoder.h"

#include <stdbool.h>




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
