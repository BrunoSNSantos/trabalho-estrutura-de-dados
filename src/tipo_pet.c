#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tipo_pet.h"
#define max 10

tipo_pet_lista* inicar_tipo_pet_lista(void){
    tipo_pet_lista *l = malloc(sizeof(tipo_pet_lista));
    if (!l) return NULL;

    l->cabeca = NULL;
    l->cauda = NULL;
    l->tam = 0;
    
    return 1;
}