/*
 * displayManager.h
 *
 *  Created on: 13 Oct 2020
 *      Author: benja
 */

#ifndef DISPLAYMANAGER_H_
#define DISPLAYMANAGER_H_
#include <stdbool.h>

//Limites de brillo
#define MIN_BRIGHTNESS 0
#define MAX_BRIGHTNESS 3

//Inicializa los recursos necesarios para utilizar el display
void InitDisplay(void);

//Borra el contenido del display
void ClearDisplay(void);

//Imprime el string que recibe en el display con opcion de que el mensaje sea en movimiento o no
void PrintMessage(const char* string, bool moving_string);

//Cambia la luminosidad del display
void SetBrightness(unsigned char brightness_factor);

//Actualiza el contenido del display.
void UpdateDisplay(void);

//Obtengo la luminusidad actual
unsigned char GetBrightnees(void);


#endif /* DISPLAYMANAGER_H_ */
