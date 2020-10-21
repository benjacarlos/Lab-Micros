#include <stdio.h>
#include "FSM_actions.h"

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