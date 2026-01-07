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

void liberar_tipo_pet_lista(tipo_pet_lista *lista) {
    if (lista == NULL) return;

    tipo_pet_no *auxp = lista->cabeca;
    while (auxp != NULL) {
        tipo_pet_no *no = auxp->prox;
        free(auxp);
        auxp = no;
    }
    free(lista);
    return;
}