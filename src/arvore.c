#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvore.h"

NoArvore *arv_criar() {
    return NULL;
}

NoArvore *arv_inserir(NoArvore *raiz, char* chave, void *ptr_dado) {
    if(!raiz) {
        NoArvore *novo = (NoArvore*)malloc(sizeof(NoArvore));
        strcpy(novo->chave, chave);
        novo->ptr_dado = ptr_dado;
        novo->dir = NULL;
        novo->esq = NULL;
        return novo;
    }

    int cmp = strcasecmp(chave, raiz->chave);

    if (cmp < 0) {
        raiz->esq = arv_inserir(raiz->esq, chave, ptr_dado);
    }
    else {
        raiz->dir = arv_inserir(raiz->dir, chave, ptr_dado);
    }
    return raiz;
}

void *arv_buscar(NoArvore *raiz, char* chave) {
    if(!raiz) {
        return NULL;
    }
    
    int cmp = strcasecmp(chave, raiz->chave);

    if(cmp == 0) {
        return raiz->ptr_dado;
    }
    if (cmp < 0) {
        return arv_buscar(raiz->esq, chave);
    } else {
        return arv_buscar(raiz->dir, chave);
    }
}

void arv_imprimir_recursivo(NoArvore *raiz, FuncaoImpressao func) {
    if (raiz != NULL) {      
        arv_imprimir_recursivo(raiz->esq, func);
        if (func != NULL) func(raiz->ptr_dado);
        arv_imprimir_recursivo(raiz->dir, func);
    }
}

void arv_imprimir_em_ordem(NoArvore *raiz, FuncaoImpressao func) {
    arv_imprimir_recursivo(raiz, func);
}

void arv_liberar(NoArvore *raiz) {
    if (raiz != NULL) {
        arv_liberar(raiz->esq);
        arv_liberar(raiz->dir);
        free(raiz);
    }
}