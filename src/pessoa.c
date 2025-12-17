#include <stdio.h>
#include <stdlib.h>
#include "pessoa.h"
#define max 10

pessoa_lista* iniciar_pessoa_lista(void) {
    pessoa_lista *l = malloc(sizeof(pessoa_lista));
    l->cabeca = NULL;
    l-> cauda = NULL;
    l->tam = 0;

    return l;
}

void liberar_pessoa_lista(pessoa_lista *lista) {
    if (lista=NULL) return;

    pessoa_no *auxp = lista->cabeca;
    while(auxp!=NULL) {
        pessoa_no *no = auxp->prox;
        free(auxp);
        auxp = no;
    }
}

int criar_pessoa(pessoa_lista *lista, pessoa_dados dados) {

}

pessoa_no* buscar_pessoa(pessoa_lista *lista, int codigo) {

}

int atualizar_pessoa(pessoa_lista *lista, pessoa_dados dados) {

}

int remover_pessoa(pessoa_lista *lista, int codigo) {
    
}