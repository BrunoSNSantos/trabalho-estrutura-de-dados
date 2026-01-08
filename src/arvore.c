#include <stdio.h>
#include <stdlib.h>
#include "arvore.h"

NoArvore *arv_criar() {
    return NULL;
}

NoArvore *arv_inserir(NoArvore *raiz, int id, void *ptr_dado) {
    if(!raiz) {
        NoArvore *novo = (NoArvore*)malloc(sizeof(NoArvore));
        novo->id = id;
        novo->ptr_dado = ptr_dado;
        novo->dir = NULL;
        novo->esq = NULL;
        return novo;
    }
    
    if (id < raiz->id) {
        raiz->esq = arv_inserir(raiz->esq, id, ptr_dado);
    }
    else if(id > raiz->id) {
        raiz->dir = arv_inserir(raiz->dir, id, ptr_dado);
    }
    // Se for igual, não insere duplicado ou atualiza (neste caso, mantemos o primeiro)
    return raiz;
}

void *arv_buscar(NoArvore *raiz, int id) {
    if(!raiz) {
        return NULL;
    }
    if(id == raiz->id) {
        return raiz->ptr_dado; // <--- CORREÇÃO: Faltava o return aqui
    }
    if (id < raiz->id) {
        return arv_buscar(raiz->esq, id);
    }
    else {
        return arv_buscar(raiz->dir, id);
    }
}

// Função auxiliar recursiva
void arv_imprimir_recursivo(NoArvore *raiz, FuncaoImpressao func) {
    if (raiz != NULL) {      
        arv_imprimir_recursivo(raiz->esq, func);
        if (func != NULL) func(raiz->ptr_dado); // Chama a função de print do main
        arv_imprimir_recursivo(raiz->dir, func);
    }
}

// Função chamada pela main
void arv_imprimir_em_ordem(NoArvore *raiz, FuncaoImpressao func) {
    arv_imprimir_recursivo(raiz, func);
}

void arv_liberar(NoArvore *raiz) {
    if (raiz != NULL) {
        arv_liberar(raiz->esq);
        arv_liberar(raiz->dir);
        free(raiz); // Libera apenas o nó da árvore, não o dado da lista!
    }
}