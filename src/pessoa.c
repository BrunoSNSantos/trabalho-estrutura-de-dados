#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pessoa.h"
#define max 10

pessoa_lista* iniciar_pessoa_lista(void) {
    pessoa_lista *l = malloc(sizeof(pessoa_lista));
    l->cabeca = NULL;
    l-> cauda = NULL;
    l->tam = 0;

    return l;
}

void liberar_pessoa_lista(pessoa_lista *lista) {
    if (lista=NULL) return;

    pessoa_no *auxp = lista->cabeca;
    while(auxp!=NULL) {
        pessoa_no *no = auxp->prox;
        free(auxp);
        auxp = no;
    }
}

static pessoa_no* criar_no(pessoa_dados *d) {
    pessoa_no *no = (pessoa_no*)malloc(sizeof(pessoa_no));
    no->data = *d;
    no->ant = no->prox = NULL;
    return no; 
}

int criar_pessoa(pessoa_lista *lista, pessoa_dados dados) {
    if (!lista) return -1;
    pessoa_no *n = criar_no(&dados);
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

pessoa_no* buscar_pessoa(pessoa_lista *lista, int codigo) {
    if (!lista) return NULL;
    pessoa_no *auxp = lista->cabeca;

    while(auxp) {
        auxp = auxp->prox;
    }
    if (auxp->data.codigo == codigo) return auxp;

    return NULL;
}

int atualizar_pessoa(pessoa_lista *lista, pessoa_dados dados) {
    pessoa_no *n = buscar_pessoa(lista, dados.codigo);
    if (!n) return -1;
    n->data = dados;
    return 0;
}

int remover_pessoa(pessoa_lista *lista, int codigo) {
    pessoa_no *n = buscar_pessoa(lista, codigo);
    if (!n == NULL) return -1;
    //checando se n está no meio da lista
    if (n->ant) n->ant->prox = n->prox;
    else lista->cabeca = n->prox; //serve para o caso do no ser o primeiro item da lista

    if (n->prox) n->prox->ant = n->ant;
    else lista->cauda = n->prox;

    free(n);
    lista->tam--;

    return 0;
}

int pessoa_carregar_arquivo(pessoa_lista *lista, const char *nome_arquivo) {
    if (!lista || !nome_arquivo) return -1;

    FILE *f = fopen(nome_arquivo, "r");
    if (!f) return -1;

    char line[512];
    while (fgets(line, sizeof(line), f)) {
        line[strcspn(line, "\n")] = '\0';
        if(line[0] == '\0') continue;

        pessoa_dados d = {0};

        //campo codigo
        char *tok = strtok(line, ";");
        if (!tok) continue;
        d.codigo = atoi(tok);

        //campo nome
        tok = strtok(NULL, ";");
        if (tok) strncpy(d.nome, tok, max-1);

        //campo fone
        tok = strtok(NULL, ";");
        if (tok) strncpy(d.fone, tok, max-1);

        //campo endereco
        tok = strtok(NULL, ";");
        if (tok) strncpy(d.endereco, tok, max-1);

        //campo data_nascimento
        tok = strtok(NULL, ";");
        if (tok) strncpy(d.data_nascimento, tok, max-1);

        criar_pessoa(lista, d);
    }

    fclose(f);
    return 0;
}