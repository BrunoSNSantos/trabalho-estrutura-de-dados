#include <stdlib.h>
#include <string.h>
#include "pet.h"
#include "pessoa.h"
#include "tipo_pet.h"
#include "arvore.h"

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
    if (!n) return NULL;
    n->data = *d;
    n->ant = n->prox = NULL;
    n->pai = NULL;
    return n;
}

int criar_pet(pet_lista *lista,pessoa_lista *pessoa_lista,tipo_pet_lista *tipo_pet_lista, pet_dados data) {
    if (!lista) return -1;

    if (buscar_pet(lista, data.codigo) != NULL) return -1;

    if (!buscar_pessoa(pessoa_lista, data.codigo_pessoa)) return -2;

    if (!buscar_tipo_pet(tipo_pet_lista, data.codigo_tipo)) return -3;

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

int atualizar_pet(pet_lista *lista, pessoa_lista *pl, tipo_pet_lista *tl, int id, char *campo, char *valor) {
    pet_no *p = buscar_pet(lista, id);
    if (!p) return -1;

    if (strcasecmp(campo, "nome") == 0) {
        strncpy(p->data.nome, valor, PET_NAME_MAX - 1);
    }
    else if (strcasecmp(campo, "codigo_pes") == 0 || strcasecmp(campo, "codigo_cli") == 0) {
        int novo_dono = atoi(valor);
        if (buscar_pessoa(pl, novo_dono) == NULL) return -2;
        p->data.codigo_pessoa = novo_dono;
    }
    else if (strcasecmp(campo, "codigo_tipo") == 0) {
        int novo_tipo = atoi(valor);
        if (buscar_tipo_pet(tl, novo_tipo) == NULL) return -3;
        p->data.codigo_tipo = novo_tipo;
    }
    return 0;
}

int pet_carregar_arquivo(pet_lista *lista, pessoa_lista *pessoa_lista, tipo_pet_lista *tipo_pet_lista, const char *nome_arquivo) {
    if(!lista || !pessoa_lista || !tipo_pet_lista || !nome_arquivo) return -1;

    FILE *f = fopen(nome_arquivo, "r");
    if(!f) return -1;

    char line[256];
    while(fgets(line, sizeof(line), f)){
        line[strcspn(line, "\n")] = '\0';
        if(line[0]=='\0') continue;

        pet_dados d = {0};

        char *tok = strtok(line, ";");
        if(!tok) continue;
        d.codigo = atoi(tok);

        tok = strtok(NULL, ";");
        if(!tok) continue;
        d.codigo_pessoa = atoi(tok);

        tok = strtok(NULL, ";");
        if(tok){
            strncpy(d.nome, tok, PET_NAME_MAX-1);
            d.nome[PET_NAME_MAX-1] = '\0';
        }

        tok = strtok(NULL, ";");
        if(!tok) continue;
        d.codigo_tipo = atoi(tok);

        if (!buscar_pessoa(pessoa_lista, d.codigo_pessoa))
            continue;

        if (!buscar_tipo_pet(tipo_pet_lista, d.codigo_tipo))
            continue;

        if (buscar_pet(lista, d.codigo))
            continue;

        pet_no *n = criar_no(&d);
        if(!n) continue;

        if(lista->cauda == NULL)
            lista->cabeca = lista->cauda = n;
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

int pet_existe_dono(pet_lista *lista, int id_pessoa) {
    if(!lista) return 0;
    pet_no *aux = lista->cabeca;
    while(aux) {
        if(aux->data.codigo_pessoa == id_pessoa) return 1;
        aux = aux->prox;
    }
    return 0;
}

void pet_imprimir_callback(void *dado) {
    pet_no *p = (pet_no*)dado;
    printf(" -> ID: %d | Nome: %s | Dono: %d | Tipo: %d\n", 
           p->data.codigo, p->data.nome, p->data.codigo_pessoa, p->data.codigo_tipo);
}

void pet_gerar_relatorio_ordenado(pet_lista *lista) {
    if (!lista) return;

    printf("\n[RELATORIO ARVORE] Pets Ordenados por Nome:\n");
    NoArvore *raiz = arv_criar();
    pet_no *curr = lista->cabeca;
    while (curr != NULL) {
        raiz = arv_inserir(raiz, curr->data.nome, (void*)curr);
        curr = curr->prox;
    }
    arv_imprimir_em_ordem(raiz, pet_imprimir_callback);
    arv_liberar(raiz);
    
    printf("--------------------------------------\n");
}