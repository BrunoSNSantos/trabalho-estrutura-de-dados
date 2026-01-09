#ifndef PET_H
#define PET_H

#include <stdio.h>
#include "pessoa.h"
#include "tipo_pet.h"

#define PET_NAME_MAX 100

typedef struct pessoa_lista pessoa_lista;
typedef struct tipo_pet_lista tipo_pet_lista;

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

/* insert returns 0 ok, -1 duplicate code, -2 invalid owner, -3 invalid tipo */
int criar_pet(pet_lista *lista,pessoa_lista *pessoa_lista,tipo_pet_lista *tipo_pet_lista, pet_dados data);
pet_no* buscar_pet(pet_lista *lista, int codigo);
int remover_pet(pet_lista *lista, int codigo);
int atualizar_pet(pet_lista *lista, pessoa_lista *pl, tipo_pet_lista *tl, int id, char *campo, char *valor);
int pet_existe_dono(pet_lista *lista, int id_pessoa);
int pet_carregar_arquivo(pet_lista *lista, pessoa_lista *pessoa_lista, tipo_pet_lista *tipo_pet_lista, const char *nome_arquivo);
int pet_salvar_arquivo(pet_lista *lista, const char *nome_arquivo);
void pet_print_all(pet_lista *list);

#endif