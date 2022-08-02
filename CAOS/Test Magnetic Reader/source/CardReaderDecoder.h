/*
 * CardReaderDecoder.h
 *
 *  Created on: Oct 12, 2020
 *      Author: Grupo 5
 */

#ifndef CARDREADERDECODER_H_
#define CARDREADERDECODER_H_

#include <stdint.h>

#define BITS_CAPACITY 32
#if BITS_CAPACITY >= 32
#define UINT_T uint32_t
#else
#define UINT_T unit8_t
#endif

#define MAX_TRACK_LEN 200
#define DATA_LEN 256


#define TERMINATOR_DATA_DECODED '\0'



typedef uint8_t CardEncodedData_t;
typedef char CardDecodedData_t;


_Bool dataParser(CardEncodedData_t * dataIn, CardDecodedData_t * dataOut, int * foundTrack);



#endif /* CARDREADERDECODER_H_ */
