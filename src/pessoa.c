#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pessoa.h"
#include "arvore.h"

pessoa_lista* iniciar_pessoa_lista(void) {
    pessoa_lista *l = malloc(sizeof(pessoa_lista));
    if (!l) return NULL;
    l->cabeca = NULL;
    l-> cauda = NULL;
    l->tam = 0;

    return l;
}

void liberar_pessoa_lista(pessoa_lista *lista) {
    if (lista==NULL) return;

    pessoa_no *auxp = lista->cabeca;
    while(auxp!=NULL) {
        pessoa_no *no = auxp->prox;
        free(auxp);
        auxp = no;
    }
    free(lista);
    return;
}

static pessoa_no* criar_no(pessoa_dados *d) {
    pessoa_no *no = (pessoa_no*)malloc(sizeof(pessoa_no));
    if (!no) return NULL;
    no->data = *d;
    no->ant = no->prox = NULL;
    return no; 
}

int criar_pessoa(pessoa_lista *lista, pessoa_dados dados) {
    if (!lista) return -1;

    if (buscar_pessoa(lista, dados.codigo) != NULL) return -1;
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

    while (auxp) {
        if (auxp->data.codigo == codigo) return auxp;
        auxp = auxp->prox;
    }
    return NULL;
}


int atualizar_pessoa(pessoa_lista *lista, int id, char *campo, char *valor) {
    pessoa_no *p = buscar_pessoa(lista, id);
    if (!p) return -1;

    if (strcasecmp(campo, "nome") == 0) {
        strncpy(p->data.nome, valor, PESSOA_NOME_MAX - 1);
    } 
    else if (strcasecmp(campo, "fone") == 0) {
        strncpy(p->data.fone, valor, PESSOA_FONE_MAX - 1);
    }
    else if (strcasecmp(campo, "endereco") == 0) {
        strncpy(p->data.endereco, valor, PESSOA_END_MAX - 1);
    }
    else if (strcasecmp(campo, "data_nascimento") == 0) {
        strncpy(p->data.data_nascimento, valor, PESSOA_DATA_MAX - 1);
    }
    
    return 0;
}

int remover_pessoa(pessoa_lista *lista, int codigo) {
    pessoa_no *n = buscar_pessoa(lista, codigo);
    if (!n) return -1;
    if (n->ant) n->ant->prox = n->prox;
    else lista->cabeca = n->prox;
    if (n->prox) n->prox->ant = n->ant;
    else lista->cauda = n->ant;

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

        char *tok = strtok(line, ";");
        if (!tok) continue;
        d.codigo = atoi(tok);

        tok = strtok(NULL, ";");
        if (tok) strncpy(d.nome, tok, PESSOA_NOME_MAX-1);

        tok = strtok(NULL, ";");
        if (tok) strncpy(d.fone, tok, PESSOA_FONE_MAX-1);

        tok = strtok(NULL, ";");
        if (tok) strncpy(d.endereco, tok, PESSOA_END_MAX-1);

        tok = strtok(NULL, ";");
        if (tok) strncpy(d.data_nascimento, tok, PESSOA_DATA_MAX-1);

        criar_pessoa(lista, d);
    }

    fclose(f);
    return 0;
}


int pessoa_salvar_arquivo(pessoa_lista *lista, const char *nome_arquivo) {
    if (!lista || !nome_arquivo) return -1;

    FILE *f = fopen(nome_arquivo, "w");
    if (!f) return -1;

    pessoa_no *auxp = lista->cabeca;
    while (auxp) {
        fprintf(f, "%d;%s;%s;%s;%s\n", 
        auxp->data.codigo,
        auxp->data.nome,
        auxp->data.fone,
        auxp->data.endereco,
        auxp->data.data_nascimento);
    auxp = auxp->prox;
    }

    fclose(f);
    return 0;
}

void pessoa_imprimir_callback(void *dado) {
    pessoa_no *p = (pessoa_no*)dado;
    printf(" -> ID: %d | Nome: %s | Fone: %s | Nasc: %s | End: %s\n", 
           p->data.codigo, 
           p->data.nome, 
           p->data.fone,
           p->data.data_nascimento,
           p->data.endereco);
}

void pessoa_gerar_relatorio_ordenado(pessoa_lista *lista) {
    if (!lista) return;

    printf("\n[RELATORIO ARVORE] Pessoas Ordenadas por Nome:\n");
    NoArvore *raiz = arv_criar();
    pessoa_no *curr = lista->cabeca;
    while (curr != NULL) {
        raiz = arv_inserir(raiz, curr->data.nome, (void*)curr);
        curr = curr->prox;
    }
    arv_imprimir_em_ordem(raiz, pessoa_imprimir_callback);
    arv_liberar(raiz);
    
    printf("--------------------------------------\n");
}