#ifndef PESSOA_H
#define PESSOA_H

#include <stdio.h>
#define max 10

typedef struct {
    int codigo;
    char nome[max];
    char fone[max];
    char endereco[max];
    char data_nascimento[max];
} pessoa_dados;

typedef struct pessoa_no {
    pessoa_dados data;
    struct pessoa_no *prox;
    struct pessoa_no *ant;
    struct pessoa_no *pai;
} pessoa_no;

typedef struct {
    pessoa_no *cabeca;
    pessoa_no *cauda;
    int tam;
} pessoa_lista;

pessoa_lista* iniciar_pessoa_lista(void);
void liberar_pessoa_lista(pessoa_lista *lista);

int criar_pessoa(pessoa_lista *lista, pessoa_dados dados);
pessoa_no* buscar_pessoa(pessoa_lista *lista, int codigo);
int atualizar_pessoa(pessoa_lista *lista, pessoa_dados dados);
int remover_pessoa(pessoa_lista *lista, int dados);

int pessoa_carregar_arquivo(pessoa_lista *lista, const char *nome_arquivo);
int pessoa_salvar_arquivo(pessoa_lista *lista, const char *nome_arquivo);


#endif 