/*
 * door.h
 *
 *      Author: Grupo 5
 */

#ifndef DOOR_H_
#define DOOR_H_

#define TIMEOUT_DOOR 6000 //en ms

void doorInit(void);
void openDoor(void);
void closeDoor(void);
void toggleDoor(void);
void openDoorTemporally(void);



#endif /* DOOR_H_ */
