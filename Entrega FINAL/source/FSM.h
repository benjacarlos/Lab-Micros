/*
	Maquina de estados 14/10/2020 by Juan Martin
*/

#ifndef FSM_H

	#define FSM_H
	//includes
	#include <stdint.h>
	// Variables
	#define EV_FIN_TABLA 0xFF
	typedef uint8_t BYTE;
	typedef struct tabla_estados	{
		BYTE evento;
		BYTE *nextstate;
		void(*accion) (void);
	}STATE;

#endif
	
