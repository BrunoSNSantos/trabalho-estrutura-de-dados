#include <stdio.h>
#include <stdlib.h>
#include "fila.h"

int filaVazia(Fila *f) {
    if(f->cabeca) {
        return 0;
    } else {
        return 1;
    }
}

void iniciarFila(Fila *f) {
    f->cabeca = NULL;
    f->cauda = NULL;
}

int adicionar(Fila *f, Comando c) {
    No *aux = malloc(sizeof(No));
    if(!aux) {
        printf("Erro: Memoria cheia. \n");
        return 0;
    }
    aux->info = c;
    aux->prox = NULL;
    if(filaVazia(f)) {
        f->cabeca = aux;
        f->cauda = f->cabeca;
    } else {
        f->cauda->prox = aux;
        f->cauda = f->cauda->prox;
    }
    return 1;
}

Comando remover(Fila *f) {
    Comando backup = {0};
    if(filaVazia(f)) {
        printf("Erro, fila vazia.\n");
        return backup;
    }
    No *aux = f->cabeca;
    backup = f->cabeca->info;
    f->cabeca = f->cabeca->prox;
    if (!(f->cabeca)) {
        f->cauda = NULL;
    }
    free(aux);
    return backup;
}