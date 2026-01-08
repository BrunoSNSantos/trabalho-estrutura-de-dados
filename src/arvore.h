#ifndef ARVORE_H
#define ARVORE_H

// Definição do Tipo de Função para Impressão (Callback)
// Isso permite que a árvore saiba imprimir Pessoa ou Pet sem saber o que é.
typedef void (*FuncaoImpressao)(void*);

typedef struct NoArvore {
    int id;                 // Chave de busca (Código)
    void *ptr_dado;         // Ponteiro para o nó da Lista Encadeada (Pessoa ou Pet)
    struct NoArvore *pai;
    struct NoArvore *esq;
    struct NoArvore *dir;
} NoArvore;

// --- Funções ---

// Cria uma árvore vazia
NoArvore* arv_criar();

// Insere um novo ID e o ponteiro do dado real
NoArvore* arv_inserir(NoArvore *raiz, int id, void *ptr_dado);

// Busca um dado pelo ID (Retorna o ponteiro original ou NULL)
void* arv_buscar(NoArvore *raiz, int id);

// Percorre em Ordem (Para o ORDER BY)
// Recebe uma função auxiliar que sabe imprimir o dado
void arv_imprimir_em_ordem(NoArvore *raiz, FuncaoImpressao func);

// Limpa a memória da árvore (apenas os nós da árvore, não os dados da lista)
void arv_liberar(NoArvore *raiz);

#endif