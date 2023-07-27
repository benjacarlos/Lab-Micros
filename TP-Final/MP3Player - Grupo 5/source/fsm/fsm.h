/***************************************************************************/ /**
  @file     fsm.h
  @brief    FSM header.
  @author   Grupo 2 - Lab de Micros
 ******************************************************************************/

#ifndef FSM_H_
#define FSM_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdint.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define FIN_TABLA 0xFF

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

typedef uint8_t event;
typedef struct state_diagram_edge STATE;

struct state_diagram_edge
{
  event evento;
  STATE *proximo_estado;
  void (*p_rut_accion)(void);
};

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
// Interfaz
STATE *fsm(STATE *p_tabla_estado, event evento_actual);

void changeCurrentId(uint8_t *currentID);

#endif /* FSM_H_ */
