#ifndef PET_H
#define PET_H

#include <stdio.h>
#include "pessoa.h"
#include "tipo_pet.h"

#define PET_NAME_MAX 100

typedef struct pet_dados{
    int codigo;
    int codigo_pessoa;
    int codigo_tipo;
    char nome[PET_NAME_MAX];
} pet_dados;

typedef struct pet_no{
    pet_dados data;
    struct pet_no *ant;
    struct pet_no *prox;
    struct pet_no *pai;
} pet_no;

typedef struct pet_lista{
    struct pet_no *cabeca;
    struct pet_no *cauda;
    int tam;
} pet_lista;

pet_lista* iniciar_pet_lista(void);
void liberar_pet_lista(pet_lista *lista);

int criar_pet(pet_lista *lista, pessoa_lista *l_pessoas, tipo_pet_lista *l_tipos, pet_dados data);
pet_no* buscar_pet(pet_lista *lista, int codigo);
int remover_pet(pet_lista *lista, int codigo);
int atualizar_pet(pet_lista *lista, pessoa_lista *l_pessoas, tipo_pet_lista *l_tipos, pet_dados data);

int pet_carregar_arquivo(pet_lista *lista, const char *nome_arquivo);
int pet_salvar_arquivo(pet_lista *lista, const char *nome_arquivo);
void pet_print_all(pet_lista *list);

#endif