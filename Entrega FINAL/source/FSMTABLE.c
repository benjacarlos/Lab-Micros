#include <stdio.h>
#include "fsm.h"
#include "fsmtable.h"
#include "FSM_actions.h"

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

/*
    Estado 0: Menu
*/
STATE estado_0[] =
{
    {EV_USER, estado_1, do_nothing},
    {EV_ADMIN, estado_2, do_nothing},
    {EV_DISPLAY, estado_3, do_nothing},
    {EV_ERROR, estado_0, error_message},
    {EV_FIN_TABLA, estado_0, do_nothing}
};
/*
    Estado 1: Modo usuario, chekeo de id
*/
STATE estado_1[] =
{
    {EV_USER_ID, estado_4, check_user_id},
    {EV_ERROR,estado_1,error_message},
    {EV_FIN_TABLA, estado_0, do_nothing}
};
/*
    Estado 4: Modo usuario, checkeo de pin y apertura de puerta
*/
STATE estado_4[] = {
    {EV_USER_PIN, estado_4, check_user_pin},
    {EV_OPEN_DOOR, estado_0, open_door},
    {EV_ERROR, estado_4, error_message},
    {EV_FIN_TABLA, estado_0, do_nothing}
};
/*
    Estado 2: Modo admin, chekeo id
*/
STATE estado_2[] =
{
    {EV_ADMIN_ID, estado_5, check_admin_id},
    {EV_ERROR, estado_2, error_message},
    {EV_FIN_TABLA, estado_0, do_nothing}
};
/*
    Estado 5: Modo admin, chekeo pin
*/
STATE estado_5[] =
{
    {EV_ADMIN_PIN, estado_6, check_admin_pin},
    {EV_ERROR, estado_5, error_message},
    {EV_FIN_TABLA, estado_0, do_nothing}
};
/*
    Estado 6: Modo admin, acciones a usuarios
*/
STATE estado_6[] =
{
    {EV_USER_ID, estado_6, check_user_id},
    {EV_ADD_ID, estado_7, add_id},
    {EV_USER_PIN, estado_8, check_user_pin},
    {EV_ERROR, estado_6, error_message},
    {EV_FIN_TABLA, estado_0, do_nothing}
};
/*
    Estado 7: Modo admin, agregado de usuarios pin
*/
STATE estado_7[] =
{
    {EV_ADD_PIN, estado_6, add_pin},
    {EV_ERROR, estado_7, error_message},
    {EV_FIN_TABLA, estado_0, do_nothing}
};
/*
    Estado 8: Modo admin, cambio de usuarios pin
*/
STATE estado_8[] =
{
    {EV_CHANGE_PIN, estado_6, change_pin},
    {EV_ERROR, estado_8, error_message},
    {EV_FIN_TABLA, estado_0, do_nothing}
};
/*
    Estado 3: Modo Display
*/
STATE estado_3[] =
{
    {EV_LIGHT_UP, estado_3, light_plus},
    {EV_LIGHT_DOWN, estado_3, light_minus},
    {EV_ERROR, estado_3, error_message},
    {EV_FIN_TABLA, estado_0, do_nothing}
};

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
