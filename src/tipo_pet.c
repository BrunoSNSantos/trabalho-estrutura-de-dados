#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tipo_pet.h"
#define max 10

tipo_pet_lista* iniciar_tipo_pet_lista(void){
    tipo_pet_lista *l = malloc(sizeof(tipo_pet_lista));
    if (!l) return NULL;

    l->cabeca = NULL;
    l->cauda = NULL;
    l->tam = 0;

    return l;
}

void liberar_tipo_pet_lista(tipo_pet_lista *lista) {
    if (lista == NULL) return;

    tipo_pet_no *auxp = lista->cabeca;
    while (auxp != NULL) {
        tipo_pet_no *no = auxp->prox;
        free(auxp);
        auxp = no;
    }
    free(lista);
    return;
}

static tipo_pet_no* criar_no(tipo_pet_dados *d){
    tipo_pet_no *n = malloc(sizeof(tipo_pet_no));
    if (!n) return NULL;
    n->data = *d;
    n->ant = n->prox = NULL;
    return n;
}

int criar_tipo_pet(tipo_pet_lista *lista, tipo_pet_dados dados) {
    if (!lista) return -1;

    if (buscar_tipo_pet(lista, dados.codigo) != NULL) return -1;
    tipo_pet_no *n = criar_no(&dados);
    
    if (lista->cauda == NULL) {
        lista->cabeca = lista->cauda = n;
    } else {
        lista->cauda->prox = n;
        n->ant = lista->cauda;
        lista->cauda = n;
    }
    lista->tam++;

    return 0;
}

tipo_pet_no* buscar_tipo_pet(tipo_pet_lista *lista, int codigo) {
    if (!lista) return NULL;
    tipo_pet_no *auxp = lista->cabeca;

    while (auxp) {
        if (auxp->data.codigo == codigo) return auxp;
        auxp = auxp->prox;
    }
    return NULL;
}

int atualizar_tipo_pet(tipo_pet_lista *lista, tipo_pet_dados dados) {
    tipo_pet_no *n = buscar_tipo_pet(lista, dados.codigo);
    if (!n) return -1;
    n->data = dados;

    return 0;
}

int remover_tipo_pet(tipo_pet_lista *lista, int codigo) {
    tipo_pet_no *n = buscar_tipo_pet(lista, codigo);
    if (!n) return -1;
    //checando se n está no meio da lista
    if (n->ant) n->ant->prox = n->prox;
    else lista->cabeca = n->prox; //serve para o caso do no ser o primeiro item da lista

    if (n->prox) n->prox->ant = n->ant;
    else lista->cauda = n->prox;

    free(n);
    lista->tam--;

    return 0;
}

int tipo_pet_carregar_arquivo(tipo_pet_lista *lista, const char *nome_arquivo) {
    if (!lista || !nome_arquivo) return -1;

    FILE *f = fopen(nome_arquivo, "r");
    if (!f) return -1;

    char line[512];
    while(fgets(line, sizeof(line), f)) {
        line[strcspn(line, "\n")] = '\0';
        if (line[0] == '\0') continue;

        tipo_pet_dados d = {0};

        char *tok = strtok(line, ";");
        if (!tok) continue;
        d.codigo = atoi(tok);

        tok = strtok(NULL, ";");
        if (tok) strncpy(d.descricao, tok, max-1);

        criar_tipo_pet(lista, d);
    }

    fclose(f);
    return 0;
}

int tipo_pet_salvar_arquivo(tipo_pet_lista *lista, const char *nome_arquivo) {
    if (!lista || !nome_arquivo) return -1;

    FILE *f = fopen(nome_arquivo, "w");
    if (!f) return -1;

    tipo_pet_no *auxp = lista->cabeca;
    while (auxp) {
        fprintf(f, "%d;%s\n", 
        auxp->data.codigo,
        auxp->data.descricao);
    auxp = auxp->prox;
    }

    fclose(f);
    return 0;
}