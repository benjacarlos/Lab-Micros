#ifndef FSMTABLE_H
	#define FSMTABLE_H
	//Funciones de interfaz de la maquina de estados
	STATE* get_state(void);
	void set_init_state(void);
	void store_state(STATE* p2_new_state);
	//Eventos
	enum {OPEN_DOOR,ERROR,USER,ADMIN,DISPLAY,LIGHT_UP,LIGHT_DOWN,USER_ID,USER_PIN,ADMIN_ID,ADMIN_PIN,ADD_ID,ADD_PIN,CHANGE_PIN};
#endif