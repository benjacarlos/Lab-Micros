/***************************************************************************/ /**
  @file     esp_comunication.h
  @brief    Controlador de mesajer usando UART
  @author   Grupo 2
 ******************************************************************************/
#ifndef ESP_COMMUNICATION_H_
#define ESP_COMMUNICATION_H_

#include <stdbool.h>
#include <stdint.h>

void esp_Init(void);

void esp_Send(char code, char * msg, char len);

void esp_Read(void);

#endif /* ESP_COMMUNICATION_H_ */
