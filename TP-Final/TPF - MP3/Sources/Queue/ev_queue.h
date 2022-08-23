/***************************************************************************/ /**
  @file     queue.h
  @brief	queue header
  @author   Grupo 5
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
  LKP_EV,	//long key press
  PRESS_EV,	//a botton press

  /* BUTTONS NAMES */
  PP_EV, //play & pause
  STOP_EV,
  NEXT_EV,
  PREV_EV,

  ENCODER_RIGHT_EV,
  ENCODER_LEFT_EV,
  ENCODER_PRESS_EV,
  ENCODER_LKP_EV,

  SD_IN_EV,
  SD_OUT_EV,

  /* SELF GENERATED EV */
  TIMEOUT_EV,
  START_EV,
  NEXT_SONG_EV,
  PREV_SONG_EV,

  CHANGE_MODE_EV,
  EFF_SELECTED_EV,	//effect selected
  FILE_SELECTED_EV,

  FILL_BUFFER_EV	//playing song
} EventType;

typedef struct EVENT
{
  EventType eType;
  bool ack;
  struct EVENT *NextEv;
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

#endif
