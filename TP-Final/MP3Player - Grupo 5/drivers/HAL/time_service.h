/***************************************************************************/ /**
  @file     time_service.h
  @brief    Time Service Header
  @author   Grupo 2 - Lab de Micros
 ******************************************************************************/

#ifndef HAL_TIME_SERVICE_H_
#define HAL_TIME_SERVICE_H_

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
/*! @brief Structure is used to hold the date and time */
typedef struct TimeServiceDate_t
{
    uint16_t year;  /*!< Range from 1970 to 2099.*/
    uint8_t month;  /*!< Range from 1 to 12.*/
    uint8_t day;    /*!< Range from 1 to 31 (depending on month).*/
    uint8_t hour;   /*!< Range from 0 to 23.*/
    uint8_t minute; /*!< Range from 0 to 59.*/
    uint8_t second; /*!< Range from 0 to 59.*/
} TimeServiceDate_t;
/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
void TimeService_Init(void (*callback)(void));
TimeServiceDate_t TimeService_GetCurrentDateTime(void);

void TimeService_Enable(void);
void TimeService_Disable(void);

#endif /* HAL_TIME_SERVICE_H_ */
