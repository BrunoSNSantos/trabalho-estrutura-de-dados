#ifndef ARVORE_H
#define ARVORE_H

typedef void (*FuncaoImpressao)(void*);

typedef struct NoArvore {
    char chave[100];      
    void *ptr_dado;
    struct NoArvore *esq;
    struct NoArvore *dir;
} NoArvore;

NoArvore* arv_criar();

NoArvore* arv_inserir(NoArvore *raiz, char* chave, void *ptr_dado);

void* arv_buscar(NoArvore *raiz, char* chave);

void arv_imprimir_em_ordem(NoArvore *raiz, FuncaoImpressao func);

void arv_liberar(NoArvore *raiz);

#endif