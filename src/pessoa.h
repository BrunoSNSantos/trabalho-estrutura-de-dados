#ifndef PESSOA_H
#define PESSOA_H

#include <stdio.h>

#define PESSOA_NOME_MAX 100
#define PESSOA_FONE_MAX 30
#define PESSOA_END_MAX 150
#define PESSOA_DATA_MAX 20

typedef struct pessoa_dados {
    int codigo;
    char nome[PESSOA_NOME_MAX];
    char fone[PESSOA_FONE_MAX];
    char endereco[PESSOA_END_MAX];
    char data_nascimento[PESSOA_DATA_MAX];
} pessoa_dados;

typedef struct pessoa_no {
    pessoa_dados data;
    struct pessoa_no *prox;
    struct pessoa_no *ant;
    struct pessoa_no *pai;
} pessoa_no;

typedef struct pessoa_lista{
    pessoa_no *cabeca;
    pessoa_no *cauda;
    int tam;
} pessoa_lista;

pessoa_lista* iniciar_pessoa_lista(void);
void liberar_pessoa_lista(pessoa_lista *lista);

int criar_pessoa(pessoa_lista *lista, pessoa_dados dados);
pessoa_no* buscar_pessoa(pessoa_lista *lista, int codigo);
int atualizar_pessoa(pessoa_lista *lista, pessoa_dados dados);
int remover_pessoa(pessoa_lista *lista, int codigo);

int pessoa_carregar_arquivo(pessoa_lista *lista, const char *nome_arquivo);
int pessoa_salvar_arquivo(pessoa_lista *lista, const char *nome_arquivo);


#endif 