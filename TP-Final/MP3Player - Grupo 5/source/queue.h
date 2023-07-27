/***************************************************************************/ /**
  @file     queue.h
  @brief
  @author   Grupo 2 - Lab de Micros
 ******************************************************************************/
#ifndef EVENTQUEUE_H
#define EVENTQUEUE_H

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
//EVENTS
typedef enum
{
  NONE_EV,
  LKP_EV,
  PRESS_EV,

  /* BUTTONS NAMES */
  PP_EV, //play pausa
  STOP_EV,
  NEXT_EV,
  PREV_EV,

  ENCODER_RIGHT_EV,
  ENCODER_LEFT_EV,
  ENCODER_PRESS_EV,
  ENCODER_LKP_EV,

  TIMEOUT_EV,

  START_EV,
  CHANGE_MODE_EV,
  EFF_SELECTED_EV,
  FILE_SELECTED_EV,
  SD_IN_EV,
  SD_OUT_EV,
  NEXT_SONG_EV,
  PREV_SONG_EV,
  FILL_BUFFER_EV
} EventType; //VER SI ESTO ES LO MEJOR, O SI ES MEJOR UNA ESTRUCTURA EVENTO MAS COMPLEJA, ADEMAS FALTA DIFERENCIAR LOS DISTINTOS EVENTOS DE TIMER

typedef struct EVENT
{
  EventType type;
  bool ack;
  struct EVENT *p2NextEv;
} event_t;

typedef struct
{
  event_t *queue;
  event_t *p2get;
  event_t *p2write;
} event_queue_t;

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define QUEUE_SIZE 50

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
/**
 * @brief Initialization of the Event Queue. Must call this function before emitting or getting events.
 */
void initQueue(void);

/**
 * @brief Addition of an event to the event queue.
 * @param EventType identifying the event.
 * @return bool indicating if an error has occured. 1: error. 0: no error.
 */
bool emitEvent(EventType type);

/**
 * @brief Returns the next event present in the event queue.
 * @return EventType identifying the next event in the queue.
 */
EventType getEvent(void);

/**
 * @brief Checks if the event queue is empty.
 * @return bool indicating whether the event queue is empty or not. false: empty. true: empty.
 */
bool queueIsEmpty(void);

#endif
