/*
 * CardReaderDecoder.c
 *
 *  Created on: Oct 18, 2020
 *      Author: Agus
 */


#include "CardReaderStandard.h"
#include "CardReaderDecoder.h"
#include <stdbool.h>
#include <stddef.h>


typedef struct{

UINT_T nBits;
UINT_T nBitsPerChar;
UINT_T nChars;
UINT_T parity;
CardEncodedData_t ssSymbol;
CardEncodedData_t fsSymbol;
CardEncodedData_t esSymbol;
CardEncodedData_t minValue;
CardEncodedData_t maxValue;
CardEncodedData_t asciiOffset;
uint8_t nFieldSeparators;
uint8_t nFormatCode;

}trackdata_t;

//track1
const trackdata_t track1 = {BITS_TRACK1, CHAR_SIZE_TRACK1, CHARS_NUM_TRACK1, PARITY_TRACK1,
						START_SENTINEL_TRACK1,
						FIELD_SEPARATOR_TRACK1,
						END_SENTINEL_TRACK1,
						MIN_VALUE_TRACK1,
						MAX_VALUE_TRACK1,
						DATA_OFFSET_ASCII_TRACK1,
						N_OF_FIELDS_SEPARATORS_TRACK1,
						N_OF_FORMAT_CODE_TRACK1};

//track2

//track3


const trackdata_t * trackarray[] = {&track1};

#define MAX_DATA_LEN DATA_LEN

#define NO_TRACK 255
#define TRACK1 0
#define TRACK2 1
#define TRACK3 2

#define SEPARATOR_CHAR '/'


uint8_t getShapedTrack(CardEncodedData_t * dataIn);

bool identifySymbol(CardEncodedData_t * dataIn, UINT_T indexStart, UINT_T indexLimit, CardEncodedData_t symbol, uint8_t symbolSize, UINT_T * indexFounded, uint8_t step);

void shapeTrack(CardEncodedData_t * dataIn, UINT_T indexStart, UINT_T indexEnd);



/**
 * @brief Decodifica la data que estaba en la cola
 * @dataIn puntero a la cola In
 * @dataOut puntero a la cola OUT
 * @trackdata puntero a estructura donde se almacena la data decodificada
 */
bool decodeTrack(CardEncodedData_t * dataIn, CardDecodedData_t * dataOut, const trackdata_t * trackData);






bool dataParser(CardEncodedData_t * dataIn, CardDecodedData_t * dataOut, int * foundTrack)
{
	bool out = false;
	int trackNum = NO_TRACK;

	//Si efectivamente tengo data
	if ((dataIn != NULL) && (dataOut != NULL) && (foundTrack != NULL))
	{
		trackNum = getShapedTrack(dataIn);
	}

	//Si es valido el numero de track decodifico
	if((trackNum != NO_TRACK) && (trackNum < TRACK_NUM))
	{
		out = decodeTrack(dataIn, dataOut, trackarray[trackNum]);
	}

	*foundTrack = trackNum;

	return out;

}



uint8_t getShapedTrack(CardEncodedData_t * dataIn)
{
	//se inicializa como no track en caso de no encontrar ninguna
	uint8_t found_track = NO_TRACK;

	UINT_T indexStartSentinel = 0;
	UINT_T indexEndSentinel = 0;
    UINT_T indexFinalBit = 0;  //en este entero se guarda el indice del último elemento (bit) que forma parte del track


    //se ve si se identifica SS para track 1/2/3
    if(identifySymbol(dataIn, 0, MAX_DATA_LEN/2, START_SENTINEL_TRACK2, CHAR_SIZE_TRACK2, &indexStartSentinel, 1) &&
    		identifySymbol(dataIn, indexStartSentinel, indexStartSentinel + BITS_TRACK2 - 1, END_SENTINEL_TRACK2, CHAR_SIZE_TRACK2, &indexEndSentinel, CHAR_SIZE_TRACK2))
    {
    	found_track = TRACK2;
        indexFinalBit = indexEndSentinel + (2 * CHAR_SIZE_TRACK2);
    }
    else if(identifySymbol(dataIn, 0, MAX_DATA_LEN/2, START_SENTINEL_TRACK1, CHAR_SIZE_TRACK1, &indexStartSentinel, 1) &&
    		identifySymbol(dataIn, indexStartSentinel, indexStartSentinel + BITS_TRACK1 - 1, END_SENTINEL_TRACK1, CHAR_SIZE_TRACK1, &indexEndSentinel, CHAR_SIZE_TRACK1))
    {
    	found_track = TRACK1;
        indexFinalBit = indexEndSentinel + (2 * CHAR_SIZE_TRACK1);
    }
    else if(identifySymbol(dataIn, 0, MAX_DATA_LEN/2, START_SENTINEL_TRACK3, CHAR_SIZE_TRACK3, &indexStartSentinel, 1) &&
    		identifySymbol(dataIn, indexStartSentinel, indexStartSentinel + BITS_TRACK3 - 1, END_SENTINEL_TRACK3, CHAR_SIZE_TRACK3, &indexEndSentinel, CHAR_SIZE_TRACK3))
    {
    	found_track = TRACK3;
        indexFinalBit = indexEndSentinel + (2 * CHAR_SIZE_TRACK3);
    }

    if(found_track != NO_TRACK)
    {
    	shapeTrack(dataIn, indexStartSentinel, indexFinalBit);
    }

    return found_track;
}


bool identifySymbol(CardEncodedData_t * dataIn, UINT_T indexStart, UINT_T indexLimit, CardEncodedData_t symbol, uint8_t symbolSize, UINT_T * found_index, uint8_t step)
{
    UINT_T i, j;
    CardEncodedData_t value;

    bool found_symbol = false;

    if(indexLimit > (indexStart + symbolSize - 1))
    {
        //Este loop recorre dataIn (codificada)
    	for(i = indexStart; ( i <= (indexLimit - symbolSize + 1) ) && ( !found_symbol ); i += step)
        {
            value = 0; //value se inicia en 0 cada ve que se busca un nuevo simbolo

            //se compara data con el simbolo en cuestión
            for(j = 0; j < (symbolSize - 1); j++)
            {
                value += (dataIn[i+j] << j);  //Se suman las j-esimas potencias de 2 (primero se envían los bits más significativos).
            }

            if(value == (symbol)) //al salir del bucle, i+j hace referencia al índice del bit de paridad. Si se da la paridad, y además se da con el valor buscado, se ha encontrado el símbolo.
            {
            	found_symbol = true;
                *found_index = i;
            }
        }
    }

    return found_symbol;
}

void shapeTrack(CardEncodedData_t * dataIn, UINT_T indexStart, UINT_T indexEnd)
{
    UINT_T i;
    if(indexStart > 0)
    {
        for(i = 0; i <= (indexEnd - indexStart); i++)
        {
            dataIn[i] = dataIn[i + indexStart];
        }
    }
}



bool decodeTrack(CardEncodedData_t * dataIn, CardDecodedData_t * dataOut, const trackdata_t * trackData)
{
	//orden de los campos: SS-PAN-FS-DATA-ES-LRC


    UINT_T i, j, charCounter = 0;
    CardEncodedData_t controlParity, value;
    CardEncodedData_t controlLRC;
    uint8_t k, indicatorsCounter = 0;  //vale 0 si no llego ningun counter, cambia a 1 cuando llega ss,
                                //cambia 2 si llega fs, a 3 si llega  es (da lugar a corroborar LRC).

    uint8_t nCodeFormat = (trackData->nFormatCode);
    uint8_t nFS = (trackData->nFieldSeparators);
    uint8_t endSentinelCount = nCodeFormat + nFS + 1;
    uint8_t LRCcount = endSentinelCount + 1;

    bool dataFailed = false;
    bool data2saveFlag = false;
    bool fieldSeparatorArrived = false;
    bool messageHasFinished = false;

    //inicializo controLRC
    controlLRC = 0;
    for (k = 0; k < (trackData->nBitsPerChar - 1); k++)
    {
        controlLRC += (trackData->parity << k); //armo el controlLRC para realizar XOR más adelante
    }
    controlLRC ^= controlLRC; //LRC tiene paridad contraria

    for(i = 0; (i < trackData->nBits) && !dataFailed && !messageHasFinished; i += trackData->nBitsPerChar)
    {
        value = 0;  //inicializo el valor en cero antes de leer cada character
        controlParity = trackData->parity;  //inicializo el control de paridad antes de leer cada caracter
        fieldSeparatorArrived = false;

        for(j = 0; j < (trackData->nBitsPerChar)-1; j++) //recorro el i-esimo caracter, sin contar el bit de paridad
        {
            value += (dataIn[i+j] << j);  //sumo las j-esimas potencias de 2 (primero se envían los bits más significativos).
            controlParity ^= dataIn[i+j];  //realizo XOR entre los bits para controlar paridad
        }

        if(controlParity != dataIn[i+j]) //al salir del bucle for, i+j hace referencia al índice del bit de paridad
        {
            dataFailed = true;
        }
        else //si paso el control del bit de paridad, se corroboran los indicadores especiales en caso de ser necesario
        {
            if((i == 0) && (value == trackData->ssSymbol) && (indicatorsCounter == 0))
            {																		//el primer caracter debe ser el start sentinel
                                                                                   //(ademas de ser el primer indicador)
                indicatorsCounter++;  //se da lugar a que venga el field separator o codigo de formato
                data2saveFlag = false; //vino un sentinel entonces no se guarda en el arreglo de salida
            }
            else if((indicatorsCounter > 0) && (indicatorsCounter <= nCodeFormat))
            {
            	indicatorsCounter++;  //se da lugar a que venga el field separator
            	data2saveFlag = true; //vino un codigo de formato que debe guardarse en el arreglo de salida
            }
            else if((value == trackData->fsSymbol) && (indicatorsCounter > nCodeFormat)
            				&& (indicatorsCounter <= (nCodeFormat + nFS)))
            {																	//si llega field separator luego de que llego
            															//format codes y/o start sentinel
                indicatorsCounter++;  //se da lugar al end sentinel
                data2saveFlag = true;  //se guarda el separador del sentinela
                fieldSeparatorArrived = true;
            }
            else if((value == trackData->esSymbol) && (indicatorsCounter == endSentinelCount))
            {
                indicatorsCounter++; //se da lugar a que se pueda corroborar el LRC
                data2saveFlag = false; //no se guarda el end sentinel en el arreglo de salida
            }
            else if((indicatorsCounter == LRCcount) && (controlLRC == value)) //si se cumple el control longitudinal, justo despues del end sentinel
            {
                data2saveFlag = false;
                messageHasFinished = true;
            }
            else if((indicatorsCounter >  nCodeFormat) && (indicatorsCounter <= endSentinelCount) && (value >= trackData->minValue) && (value <= trackData->maxValue))
            {

            	data2saveFlag = true;
            }
            else
            {
                dataFailed = true;
            }

        }

        if(!dataFailed)
        {
            controlLRC ^= value;   //sabiendo que la data es valida actualizo LRC para control longitudinal
            if(charCounter < (trackData->nChars-1))
            { //entro a escribir el buffer de data si no me pase de caracteres (resto 1 para dejar lugar al terminador),
                // si value no se va de rango, y si el caracter que vino no corresponde a un separador.
                if(data2saveFlag)
                {
                	if(fieldSeparatorArrived)
                	{
                		dataOut[charCounter] = SEPARATOR_CHAR;
                	}
                	else
                	{
                		dataOut[charCounter] = (CardDecodedData_t)(value + trackData->asciiOffset);  //escribo, teniendo en cuenta el derivado
                	}	                                                                        //del ascii utilizado en la codificación

                    charCounter++;
                }

            }
            else
            {
                dataFailed = true;
            }

        }
    }

    dataOut[charCounter] = TERMINATOR_DATA_DECODED;

    if(!messageHasFinished)
    {					//si el mensaje aun no ha terminado (por que no se validaron todos los indicadores), la salida es erróonea.
    	dataFailed = true;
    }

    return !dataFailed;
}

