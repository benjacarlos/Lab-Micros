/***************************************************************************/ /**
  @file     queue.c
  @brief    Event queue functions.
  @author   Grupo 5
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "ev_queue.h"

/*******************************************************************************
 * PRIVATE VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

event_queue_t eventQueue;
event_t ev_queue[QUEUE_SIZE];

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

/**
 * @brief Checks if the event queue is empty.
 * @return bool indicating whether the event queue is empty or not. false: empty. true: empty.
 */
static bool queueIsEmpty(void);


/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void initQueue(void)
{
  int i;

  event_t *temp = ev_queue;

  //Inicializar la cola de eventos para poder ser escrita y leida
  for (i = 0; i < QUEUE_SIZE; i++)
  {
    temp++->ack = true;
  }

  eventQueue.queue = ev_queue; // cola de eventos

  //los punteros para leer y escribir estan en el mismo lugar donde la cola esta vacia
  eventQueue.p2get = ev_queue;
  eventQueue.p2write = ev_queue;
}

bool emitEvent(EventType type)
{
  event_t *temp = eventQueue.p2write;

  if (eventQueue.p2write->ack == false) // verifico si hay mas lugar en la cola
  {
    return false;
  }

  eventQueue.p2write->type = type; // guardo el ID del evento

  if ((eventQueue.p2write) == ((eventQueue.queue) + (QUEUE_SIZE - 1))) // movimiento del puntero de write
  {
    eventQueue.p2write = eventQueue.queue; // si estoy al final de la cola debo moverlo al principio
  }
  else
  {
    (eventQueue.p2write)++; // sino avanza una posicion
  }

  temp->NextEv = (struct EVENT *)eventQueue.p2write; // guardo en el evento anterior un puntero al evento actual para no tener que problems con el movimiento en get_event
  temp->ack = false;                                   // marco el lugar como ocupado

  return true;
}

EventType getEvent()
{
  EventType eventT;
  if (queueIsEmpty()) // verifico si la cola esta vacia
  {
    return NONE_EV;
  }
  eventT = eventQueue.p2get->type; // guardo el tipo de evento y muevo el puntero
  eventQueue.p2get->ack = true; //pop the event from queue
  eventQueue.p2get = (event_t *)eventQueue.p2get->NextEv;

  return eventT;
}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

static bool queueIsEmpty(void)
{
  return (eventQueue.p2get == eventQueue.p2write && eventQueue.p2get->ack == true); // si ambos punteros estan en la misma posicion y el evento al que apunta read ya ha sido leido entonces la cola esta vacia
}
