/***************************************************************************/ /**
  @file     queue.c
  @brief    Event queue functions.
  @author   Grupo 5
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
                        GLOBAL FUNCTION DEFINITIONS
 ******************************************************************************/

void initQueue(void)
{
  int i;

  event_t *tempaux = queue;

  for (i = 0; i < QUEUE_SIZE; i++)
  {
    tempaux++->ack = true; // Debo hacer esto para asegurarme de que todas las celdas de la cola esten habilitadas para ser escritas
  }

  eventQueue.queue = queue; // La cola de eventos
  eventQueue.p2get = queue; // Los punteros para leer y escribir estan en el mismo lugar (cola vacia)
  eventQueue.p2write = queue;
}

bool emitEvent(EventType type)
{
  event_t *temp = eventQueue.p2write;

  if (eventQueue.p2write->ack == false) // Si en la cola de eventos no hay mas lugar -> error
  {
    return false;
  }

  eventQueue.p2write->type = type; 		// Guardo el ID del evento

  if ((eventQueue.p2write) == ((eventQueue.queue) + (QUEUE_SIZE - 1))) // Movimiento del puntero de write
  {
    eventQueue.p2write = eventQueue.queue; 								// Si estoy al final de la cola debo moverlo al principio
  }
  else
  {
    (eventQueue.p2write)++; // Sino avanza una posicion
  }
  temp->p2NextEv = (struct EVENT *)eventQueue.p2write; // Guardo en el evento anterior un puntero al evento actual para no tener que problems con el movimiento en get_event
  temp->ack = false;                                   // Marco el lugar como ocupado
  return true;
}

EventType getEvent()
{
  EventType retval;
  if (queueIsEmpty())
  {
    return NONE_EV; 				// si la cola esta vacia -> error
  }
  retval = eventQueue.p2get->type; // guardo el tipo y muevo el puntero
  eventQueue.p2get->ack = true;
  eventQueue.p2get = (event_t *)eventQueue.p2get->p2NextEv;
  return retval;
}

bool queueIsEmpty(void)
{
  return (eventQueue.p2get == eventQueue.p2write && eventQueue.p2get->ack == true); // Si ambos punteros estan en la misma posicion y el evento al que apunta read ya ha sido leido entonces la cola esta vacia
}
