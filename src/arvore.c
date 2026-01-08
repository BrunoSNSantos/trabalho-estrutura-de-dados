#include <stdio.h>
#include <stdlib.h>
#include "arvore.h"

NoArvore *arv_criar() {
    return NULL;
}

NoArvore *arv_inserir(NoArvore *raiz, int id, void *ptr_dado) {
    NoArvore *novo = malloc(sizeof(NoArvore));
    novo->id = id;
    novo->ptr_dado = ptr_dado;
    novo->dir = NULL;
    novo->esq = NULL;
    if(!raiz) {
        return novo;
    }
    if (id < raiz->id) {
        arv_inserir(raiz->esq, id, ptr_dado);
    }
    else if(id >raiz->id) {
        arv_inserir(raiz->dir, id, ptr_dado);
    }
    return raiz;
}

void *arv_buscar(NoArvore *raiz, int id) {
    if(!raiz) {
        return NULL;
    }
    if(id == raiz->id) {
        raiz->ptr_dado;
    }
    if (id < raiz->id) {
        return arv_buscar(raiz->esq, id);
    }
    else {
        return arv_buscar(raiz->dir, id);
    }
}

void arv_imprimir_ordenado(NoArvore *raiz, FuncaoImpressao func) {
    if (raiz != NULL) {       
        arv_imprimir_em_ordem(raiz->esq, func);
        if (func != NULL) func(raiz->ptr_dado);
        arv_imprimir_em_ordem(raiz->dir, func);
    }
}

void arv_liberar(NoArvore *raiz) {
    if (raiz != NULL) {
        arv_liberar(raiz->esq);
        arv_liberar(raiz->dir);
        free(raiz);
    }
}