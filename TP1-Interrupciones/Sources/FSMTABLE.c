#include <stdio.h>
#include "fsm.h"
#include "fsmtable.h"

//Almacena el estado actual
static STATE* p2state = NULL;

int coincidencias = 0;
int monto_acumulado = 0;

extern STATE estado_0[];
extern STATE estado_1[];
extern STATE estado_2[];
extern STATE estado_3[];
extern STATE estado_4[];
extern STATE estado_5[];
extern STATE estado_6[];
extern STATE estado_7[];
extern STATE estado_8[];
extern STATE estado_9[];
extern STATE estado_10[];
extern STATE estado_11[];
extern STATE estado_12[];

/*Funciones de las acciones*/
void do_nothing(void);
void error_message(void);
void open_door(void);
void light_plus(void);
void light_minus(void);
void check_user_id(void);
void check_user_pin(void);
void check_admin_id(void);
void check_admin_pin(void);
void add_id(void);
void change_pin(void);
void add_pin(void);


/*
    Estado 0: Menu
*/
STATE estado_0[] =
{
    {USER, estado_1, do_nothing},
    {ADMIN, estado_2, do_nothing},
    {DISPLAY, estado_3, do_nothing},
    {ERROR, estado_0, error_message},
    {FIN_TABLA, estado_0, do_nothing}
};
/*
    Estado 1: Modo usuario, chekeo de id
*/
STATE estado_1[] =
{
    {USER_ID, estado_4, check_user_id},
    {ERROR,estado_0,error_message},
    {FIN_TABLA, estado_0, do_nothing}
};
/*
    Estado 4: Modo usuario, checkeo de pin
*/
STATE estado_4[] = {
    {USER_PIN, estado_9, check_user_pin},
    {ERROR, estado_1, error_message},
    {FIN_TABLA, estado_0, do_nothing}
};
/*
    Estado 9: Modo usuario, apertura de puerta
*/
STATE estado_9[] ={
    {OPEN_DOOR, estado_0, open_door},
    {ERROR, estado_4, error_message},
    {FIN_TABLA, estado_0, do_nothing}
};
/*
    Estado 2: Modo admin, chekeo id
*/
STATE estado_2[] =
{
    {ADMIN_ID, estado_5, check_admin_id},
    {ERROR, estado_2, error_message},
    {FIN_TABLA, estado_0, do_nothing}
};
/*
    Estado 5: Modo admin, chekeo pin
*/
STATE estado_5[] =
{
    {ADMIN_PIN, estado_6, check_admin_pin},
    {ERROR, estado_2, error_message},
    {FIN_TABLA, estado_0, do_nothing}
};
/*
    Estado 6: Modo admin, chekeo usuario
*/
STATE estado_6[] =
{
    {USER_ID, estado_10, check_user_id},
    {ERROR, estado_5, error_message},
    {FIN_TABLA, estado_0, do_nothing}
};
/*
    Estado 10: Modo admin, acciones a usuarios
*/
STATE estado_10[] =
{
    {ADD_ID, estado_7, add_id},
    {USER_PIN, estado_8, check_user_pin},
    {ERROR, estado_6, error_message},
    {FIN_TABLA, estado_0, do_nothing}
};
/*
    Estado 7: Modo admin, agregado de usuarios
*/
STATE estado_7[] =
{
    {ADD_PIN, estado_11, add_pin},
    {ERROR, estado_10, error_message},
    {FIN_TABLA, estado_0, do_nothing}
};
/*
    Estado 11: Modo admin, manejo de error en el agregado de PIN
*/
STATE estado_11[] =
{
    {SUCCES, estado_6, do_nothing},
    {ERROR, estado_7, error_message},
    {FIN_TABLA, estado_0, do_nothing}
};
/*
    Estado 8: Modo admin, cambio de usuarios pin
*/
STATE estado_8[] =
{
    {CHANGE_PIN, estado_12, change_pin},
    {ERROR, estado_10, error_message},
    {FIN_TABLA, estado_0, do_nothing}
};
/*
    Estado 12: Modo admin, manejo de error en el cambio de PIN
*/
STATE estado_12[] =
{
    {SUCCES, estado_6, do_nothing},
    {ERROR, estado_8, error_message},
    {FIN_TABLA, estado_0, do_nothing}
};
/*
    Estado 3: Modo Display
*/
STATE estado_3[] =
{
    {LIGHT_UP, estado_3, light_plus},
    {LIGHT_DOWN, estado_3, light_minus},
    {ERROR, estado_3, error_message},
    {FIN_TABLA, estado_0, do_nothing}
};

/*Rutinas de accion*/
void do_nothing(void){
}

void error_message(void) {
    printf("Ocurrio el error: ect\n");
}

void open_door(void) {
    printf("Apertura de la puerta\n");
}

void light_plus(void) {
    printf("Aumento el nivel de brillo\n");
}

void light_minus(void) {
    printf("Disminuyo el brillo\n");
}

void check_user_id(void) {
    printf("Checking user ID\n");
}

void check_user_pin(void) {
    printf("Checking user PIN\n");
}

void check_admin_id(void) {
    printf("Checking admin ID\n");
}

void check_admin_pin(void) {
    printf("Checking admin PIN\n");
}

void add_id(void) {
    printf("Agrego el nuevo ID\n");
}

void change_pin(void) {
    printf("Cambio el PIN de un usuario existente\n");
}

void add_pin(void) {
    printf("Agrego el nuevo PIN\n");
}

/*Interfaz*/
STATE* get_state(void)
{
    return (p2state);
}

void set_init_state(void)
{
    p2state = estado_0;
}

void store_state(STATE* p2_new_state)
{
    p2state = p2_new_state;
}