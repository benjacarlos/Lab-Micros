/*
 * EncoderFsm.h
 *
 *  Created on: 20 oct. 2020
 *      Author: Juan Martin
 */

#ifndef ENCODERHANDLER_H_
	#define ENCODERHANDLER_H_
	#include <stdbool.h>
	//Limites
	#define MAX_NUM '9'
	#define MIN_NUM '0'
	//Flags
	bool IsPin=false;
	bool IsID=false;
	//Funciones
	void UpdateNumber(void);
	char GetNumber(void);
	void ResetNumber(void);

#endif /* ENCODERHANDLER_H_ */
