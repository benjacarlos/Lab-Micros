/*
 * EncoderFsm.h
 *
 *  Created on: 20 oct. 2020
 *      Author: Juan Martin
 */

#ifndef ENCODERHANDLER_H_
	#define ENCODERHANDLER_H_
	#include <stdbool.h>
	//Flags
	bool IsPin=false;
	bool IsID=false;
	//Interfaz
	void InitNumber(void);
	bool GettingTotalNumber(void);
	char* GetTotalNum(void);

#endif /* ENCODERHANDLER_H_ */
