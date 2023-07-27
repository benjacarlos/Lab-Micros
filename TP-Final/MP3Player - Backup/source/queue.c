/***************************************************************************/ /**
  @file     queue.c
  @brief    Event queue functions.
  @author   Grupo 2 - Lab de Micros
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "queue.h"

/*******************************************************************************
 * PRIVATE VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

event_queue_t eventQueue;
event_t queue[QUEUE_SIZE];

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void initQueue(void)
{
  int i;

  event_t *tempaux = queue;

  for (i = 0; i < QUEUE_SIZE; i++)
  {
    tempaux++->ack = true; //debo hacer esto para asegurarme de que todas las celdas de la cola esten habilitadas para ser escritas
  }

  eventQueue.queue = queue; //la cola de eventos
  eventQueue.p2get = queue; //los punteros para leer y escribir estan en el mismo lugar (cola vacia)
  eventQueue.p2write = queue;
}

bool emitEvent(EventType type)
{
  event_t *temp = eventQueue.p2write;

  if (eventQueue.p2write->ack == false) // si en la cola de eventos no hay mas lugar -> error
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
  temp->p2NextEv = (struct EVENT *)eventQueue.p2write; // guardo en el evento anterior un puntero al evento actual para no tener que problems con el movimiento en get_event
  temp->ack = false;                                   // marco el lugar como ocupado
  return true;
}

EventType getEvent()
{
  EventType retval;
  if (queueIsEmpty())
  {
    return NONE_EV; // si la cola esta vacia -> error
  }
  retval = eventQueue.p2get->type; // guardo el tipo y muevo el puntero
  eventQueue.p2get->ack = true;
  eventQueue.p2get = (event_t *)eventQueue.p2get->p2NextEv;
  return retval;
}

bool queueIsEmpty(void)
{
  return (eventQueue.p2get == eventQueue.p2write && eventQueue.p2get->ack == true); // si ambos punteros estan en la misma posicion y el evento al que apunta read ya ha sido leido entonces la cola esta vacia
}
