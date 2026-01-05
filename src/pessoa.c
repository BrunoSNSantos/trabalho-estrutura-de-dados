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

static pessoa_no* criar_no(pessoa_dados *d) {
    pessoa_no *no = (pessoa_no*)malloc(sizeof(pessoa_no));
    no->data = *d;
    no->ant = no->prox = NULL;
    return no; 
}

int criar_pessoa(pessoa_lista *lista, pessoa_dados dados) {
    if (!lista) return -1;
    pessoa_no *n = criar_no(&dados);
    if (lista->cauda == NULL) {
        lista->cabeca = lista->cauda = n;
    } else {
        lista->cauda->prox = n;
        n->ant = lista->cauda;
        lista->cauda = n;
    }
    lista->tam++;
    return 0;

}

pessoa_no* buscar_pessoa(pessoa_lista *lista, int codigo) {

}

int atualizar_pessoa(pessoa_lista *lista, pessoa_dados dados) {

}

int remover_pessoa(pessoa_lista *lista, int codigo) {
    
}