#ifndef FILA_H
#define FILA_H

#define OP_INSERT 1
#define OP_DELETE 2
#define OP_SELECT 3
#define OP_UPDATE 4

#define TAB_PESSOA 1
#define TAB_PET 2
#define TAB_TIPO_PET 3

typedef struct {
    int operacao;
    int tabela;

    char campos[5][50];
    char valores[5][50];
    int qtd_params;
} Comando;

typedef struct No {
    Comando info;
    struct No *prox;
} No;

typedef struct {
    No *cabeca;
    No *cauda;
} Fila;

int filaVazia (Fila *f);
void iniciarFila(Fila *f);
int adicionar(Fila *f, Comando c);
Comando remover(Fila *f);

#endif