#ifndef PET_H
#define PET_H

#include <stdio.h>
#define PET_NAME_MAX 100

typedef struct {
    int codigo;
    int codigo_pessoa;
    int codigo_tipo;
    char nome[PET_NAME_MAX];
} pet_dados;

typedef struct {
    pet_dados;
    struct pet_no *ant;
    struct pet_no *prox;
    struct pet_no *pai;
} pet_no;

typedef struct {
    struct pet_lista *cabeca;
    struct pet_lista *cauda;
    int tam;
} pet_lista;

pet_lista* iniciar_pet_lista(void);
void liberar_pet_lista(pet_lista *lista);

/* insert returns 0 ok, -1 duplicate code, -2 invalid owner, -3 invalid tipo */
int criar_pet(pet_lista *lista, pet_dados data,
                int (*dono_existe)(int),
                int (*tipo_existe)(int));

pet_no* buscar_pet(pet_lista *lista, int codigo);
int remover_pet(pet_lista *lista, int codigo);
int atualizar_pet(pet_lista *lista, pet_dados data,
                int (*dono_existe)(int),
                int (*tipo_existe)(int));


int pet_carregar_arquivo(pet_lista *lista, const char *nome_arquivo);
int pet_salvar_arquivo(pet_lista *lista, const char *nome_arquivo);

#endif