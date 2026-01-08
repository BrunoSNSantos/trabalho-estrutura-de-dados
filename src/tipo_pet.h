#ifndef TIPO_PET_H
#define TIPO_PET_H

#include <stdio.h>
#define max 10

typedef struct tipo_pet_dados{
    int codigo;
    char descricao[max];
} tipo_pet_dados;

typedef struct tipo_pet_no {
    tipo_pet_dados data;
    struct tipo_pet_no *prox;
    struct tipo_pet_no *ant;
    struct tipo_pet_no *pai;
} tipo_pet_no;

typedef struct tipo_pet_lista {
    tipo_pet_no *cabeca;
    tipo_pet_no *cauda;
    int tam;
} tipo_pet_lista;

tipo_pet_lista* iniciar_tipo_pet_lista(void);
void liberar_tipo_pet_lista(tipo_pet_lista *lista);

int criar_tipo_pet(tipo_pet_lista *lista, tipo_pet_dados dados);
tipo_pet_no* buscar_tipo_pet(tipo_pet_lista *lista, int codigo);
int atualizar_tipo_pet(tipo_pet_lista *lista, tipo_pet_dados dados);
int remover_tipo_pet(tipo_pet_lista *lista, int codigo);

int tipo_pet_carregar_arquivo(tipo_pet_lista *lista, const char *nome_arquivo);
int tipo_pet_salvar_arquivo(tipo_pet_lista *lista, const char *nome_arquivo);


#endif