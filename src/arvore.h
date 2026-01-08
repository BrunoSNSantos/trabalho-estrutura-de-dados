#ifndef ARVORE_H
#define ARVORE_H

typedef void (*FuncaoImpressao)(void*);

typedef struct NoArvore {
    char chave[100];      // MUDOU: De int id para char chave[]
    void *ptr_dado;
    struct NoArvore *esq;
    struct NoArvore *dir;
} NoArvore;

NoArvore* arv_criar();

// MUDOU: Recebe char* chave em vez de int id
NoArvore* arv_inserir(NoArvore *raiz, char* chave, void *ptr_dado);

// Busca também muda para char* (se for usar no futuro)
void* arv_buscar(NoArvore *raiz, char* chave);

void arv_imprimir_em_ordem(NoArvore *raiz, FuncaoImpressao func);

void arv_liberar(NoArvore *raiz);

#endif