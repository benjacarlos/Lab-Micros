/***************************************************************************//**
  @file     timer.h
  @brief    Timer driver.
  @author   Grupo 5.
 ******************************************************************************/

#ifndef _TIMER_H_
#define _TIMER_H_

/*******************************************************************************
 * HEADER FILES
 ******************************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>


/*******************************************************************************
 *  CONSTANTES Y MACROS
 ******************************************************************************/

typedef enum {INACTIVITY, DISPLAY, MESSAGE, LED, ROTATION_TIMER, BUTTON_TIMER, DOOR, UNBLOCKED, TIMERS_CANT}timerData_t; //Aca van los diferentes timers utilizados (inactividad, display, rotacion encoder, etc)

/*******************************************************************************
 * ESTRUCTURAS Y TYPEDEF
 ******************************************************************************/


// Timer alias
typedef uint32_t ttick_t;
typedef uint8_t tim_id_t;
typedef void (*tim_callback_t)(void);



/*******************************************************************************
 * FUNCIONES (SERVICIOS)
 ******************************************************************************/

/**
 * @brief Inicializa timers y perifericos necesarios
 */
void timerInit(void);


/**
 * @brief Comienza un nuevo timer
 * @param id ID del timer a iniciar
 * @param period timepo hasta que expire el timer EN MILISEGUNDOS
 * @param callback callback que se llama cuando expira
 */
void timerStart(tim_id_t id, ttick_t period, tim_callback_t callback);

/**
 * @brief devuelve contador del timer
 * @param id indice del timer que quiero reiniciar
 * @return cuanto transcurrio del contador del timer (en ms)
 */
ttick_t timerGetCnt(tim_id_t id);

/**
 * @brief reinicia un timer
 * @param id indice del timer que quiero reiniciar
 */
void timerRestart(tim_id_t id);


/**
 * @brief Activa un timer
 * @param id indice del timer que quiero activar
 */
void timerEnable(tim_id_t id);


/**
 * @brief Desactiva un timer
 * @param id indice del timer que quiero desactivar
 */
void timerDisable(tim_id_t id);


/*******************************************************************************
 ******************************************************************************/

#endif // _TIMER_H_
