#include <stdlib.h>
#include <string.h>
#include "pet.h"

pet_lista* iniciar_pet_lista(void){
    pet_lista *l = malloc(sizeof(pet_lista));
    l->cabeca = NULL;
    l->cauda = NULL;
    l->tam = 0;
    return l;
}

void liberar_pet_lista(pet_lista *lista){
    if(!lista) return;
    pet_no *auxp = lista->cabeca;
    while(auxp){
        pet_no *n = auxp->prox;
        free(auxp);
        auxp = n;
    }
    free(lista);
}

static pet_no* criar_no(pet_dados *d){
    pet_no *n = malloc(sizeof(pet_no));
    n->data = *d;
    n->ant = n->prox = NULL;
    n->pai = NULL;
    return n;
}

int criar_pet(pet_lista *lista, pessoa_lista *l_pessoas, tipo_pet_lista *l_tipos, pet_dados data) {
    if (!lista) return -1;

    if (buscar_pet(lista, data.codigo) != NULL) return -1;

    if (l_pessoas != NULL) {
        if (buscar_pessoa(l_pessoas, data.codigo_pessoa) == NULL) return -2;
    }

    if (l_tipos != NULL) {
        if (buscar_tipo_pet(l_tipos, data.codigo_tipo) == NULL) return -3;
    }

    pet_no *n = criar_no(&data);

    if(lista->cauda == NULL) lista->cabeca = lista->cauda = n;
    else {
        lista->cauda->prox = n;
        n->ant = lista->cauda;
        lista->cauda = n;
    }
    lista->tam++;
    return 0;
}

pet_no* buscar_pet(pet_lista *lista, int codigo){
    if (!lista) return NULL;
    pet_no *auxp = lista->cabeca;
    while(auxp){
        if(auxp->data.codigo == codigo) return auxp;
        auxp = auxp->prox;
    }
    return NULL;
}

int remover_pet(pet_lista *lista, int codigo){
    pet_no *n = buscar_pet(lista, codigo);
    if (!n) return -1;

    if (n->ant) n->ant->prox = n->prox;
    else lista->cabeca = n->prox;

    if(n->prox) n->prox->ant = n->ant;
    else lista->cauda = n->ant;

    free(n);
    lista->tam--;
    return 0;
}

int atualizar_pet(pet_lista *lista, pessoa_lista *l_pessoas, tipo_pet_lista *l_tipos, pet_dados data) {
    pet_no *n = buscar_pet(lista, data.codigo);
    if(!n) return -1;

    if (l_pessoas != NULL) {
        if (buscar_pessoa(l_pessoas, data.codigo_pessoa) == NULL) return -2;
    }

    if (l_tipos != NULL) {
        if (buscar_tipo_pet(l_tipos, data.codigo_tipo) == NULL) return -3;
    }

    n->data = data;
    return 0;
}

int pet_carregar_arquivo(pet_lista *lista, const char *nome_arquivo){
    if(!lista || !nome_arquivo) return -1;

    FILE *f = fopen(nome_arquivo, "r");
    if(!f) return -1;

    char line[256];
    while(fgets(line, sizeof(line), f)){
        line[strcspn(line, "\n")] = 0;
        if(line[0]=='\0') continue;

        pet_dados d = {0};
        char *tok = strtok(line, ";");
        if(!tok) continue;
        d.codigo = atoi(tok);

        tok = strtok(NULL, ";");
        if(tok) d.codigo_pessoa = atoi(tok);
        
        tok = strtok(NULL, ";");
        if(tok) strncpy(d.nome, tok, PET_NAME_MAX-1);

        tok = strtok(NULL, ";");
        if(tok) d.codigo_tipo = atoi(tok);

        pet_no *n = criar_no(&d);
        if(lista->cauda==NULL) lista->cabeca = lista->cauda = n;
        else {
            lista->cauda->prox = n;
            n->ant = lista->cauda;
            lista->cauda = n;
        }
        lista->tam++;
    }
    fclose(f);
    return 0;
}

int pet_salvar_arquivo(pet_lista *lista, const char *nome_arquivo){
    if(!lista || !nome_arquivo) return -1;

    FILE *f = fopen(nome_arquivo, "w");
    if(!f) return -1;

    pet_no *auxp = lista->cabeca;
    while(auxp){
        fprintf(f, "%d;%d;%s;%d\n",
            auxp->data.codigo,
            auxp->data.codigo_pessoa,
            auxp->data.nome,
            auxp->data.codigo_tipo);
        auxp = auxp->prox;
    }
    fclose(f);
    return 0;
}

void pet_print_all(pet_lista *list){
    if(!list) return;
    pet_no *cur = list->cabeca;
    while(cur){
        printf("Pet[%d] nome='%s' dono=%d tipo=%d\n",
               cur->data.codigo,
               cur->data.nome,
               cur->data.codigo_pessoa,
               cur->data.codigo_tipo);
        cur = cur->prox;
    }
}