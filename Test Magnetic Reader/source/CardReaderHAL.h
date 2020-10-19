/*
 * CardReaderHAL.h
 *
 *  Created on: Oct 17, 2020
 *      Author: Grupo 7
 */

#ifndef CARDREADERHAL_H_
#define CARDREADERHAL_H_


/*******************************************************************************
 *  CONSTANTES Y MACROS
 ******************************************************************************/

#define DATA_LEN 256
#define MAX_NUM_ITEMS 5


/*******************************************************************************
 * ESTRUCTURAS Y TYPEDEF
 ******************************************************************************/

typedef uint8_t CardEncodedData_t;


/*******************************************************************************
 * FUNCIONES (SERVICIOS)
 ******************************************************************************/

/**
 * @brief Inicializa driver de hardware de lector de tarjeta magnetizada
 */
void CardReaderHW_Init(void);


/**
 * @brief devuelve puntero a los datos que tengo guardados de la cola del lector
 */
CardEncodedData_t * get_buffer(void);


#endif /* CARDREADERHAL_H_ */

