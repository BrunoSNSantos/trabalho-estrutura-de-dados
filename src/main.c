#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pessoa.h"
#include "pet.h"
#include "tipo_pet.h"
#include "fila.h"
#include "parser.h"
#include "arvore.h"

// --- FUNÇÕES AUXILIARES DE VALIDAÇÃO (Proteção contra código incompleto) ---

// Verifica se existe pessoa com este ID
int aux_pessoa_existe(pessoa_lista *l, int id) {
    if(!l) return 0;
    if(buscar_pessoa(l, id) != NULL) return 1;
    return 0;
}

// Verifica se existe tipo de pet com este ID
int aux_tipo_existe(tipo_pet_lista *l, int id) {
    if(!l) return 0;
    if(buscar_tipo_pet(l, id) != NULL) return 1;
    return 0;
}

// Verifica se a pessoa tem pets (Impede delete indevido)
int aux_pessoa_tem_pet(pet_lista *lpet, int id_pessoa) {
    if(!lpet) return 0;
    // Acessando manualmente pois a função buscar do amigo não serve pra isso
    // OBS: Se der erro aqui, verifique se struct pet_no está no .h
    pet_no *aux = lpet->cabeca; 
    while(aux) {
        if(aux->data.codigo_pessoa == id_pessoa) return 1;
        aux = aux->prox;
    }
    return 0;
}

// Função Callback para imprimir Pessoa na Árvore
void imprimir_pessoa_arvore(void *dado) {
    pessoa_no *p = (pessoa_no*)dado;
    printf(" -> ID: %d | Nome: %s | Fone: %s\n", p->data.codigo, p->data.nome, p->data.fone);
}

// --- MAIN PRINCIPAL ---
int main() {
    printf("=== SISTEMA INICIADO ===\n");

    // 1. Inicializar Listas 
    // OBS: Corrija os nomes aqui se você já tiver arrumado os typos nos arquivos .c
    pessoa_lista *lista_pessoas = iniciar_pessoa_listaa(); 
    pet_lista *lista_pets = iniciar_pet_lista();
    tipo_pet_lista *lista_tipos = inicar_tipo_pet_lista();

    // Carrega dados persistidos
    pessoa_carregar_arquivo(lista_pessoas, "pessoas.txt");
    pet_carregar_arquivo(lista_pets, "pets.txt");
    tipo_pet_carregar__arquivo(lista_tipos, "tipos.txt"); 

    // 2. Processar Script de Comandos
    Fila fila_geral;
    iniciarFila(&fila_geral);
    
    // Tente ler o script. Crie um arquivo 'script.txt' para testar!
    processar_arquivo("script.txt", &fila_geral); 

    // 3. Separação em Filas Específicas
    Fila fila_pessoa, fila_pet;
    iniciarFila(&fila_pessoa);
    iniciarFila(&fila_pet);

    while(!filaVazia(&fila_geral)) {
        Comando cmd = remover(&fila_geral);
        if(cmd.tabela == TAB_PESSOA) adicionar(&fila_pessoa, cmd);
        else if(cmd.tabela == TAB_PET) adicionar(&fila_pet, cmd);
        // Pode adicionar fila_tipo se necessário
    }

    // 4. Executar Fila de PESSOAS
    printf("\n--- Processando PESSOAS ---\n");
    while(!filaVazia(&fila_pessoa)) {
        Comando cmd = remover(&fila_pessoa);

        if(cmd.operacao == OP_INSERT) {
            pessoa_dados d;
            d.codigo = atoi(cmd.valores[0]);
            strcpy(d.nome, cmd.valores[1]);
            strcpy(d.fone, cmd.valores[2]);
            // Tratamento simples para campos opcionais no parser
            if(cmd.qtd_params > 3) strcpy(d.endereco, cmd.valores[3]);
            else strcpy(d.endereco, "");
            
            if(cmd.qtd_params > 4) strcpy(d.data_nascimento, cmd.valores[4]);
            else strcpy(d.data_nascimento, "");
            
            if(criar_pessoa(lista_pessoas, d) == 0) printf("[OK] Inserido Pessoa ID %d\n", d.codigo);
            else printf("[ERRO] Falha ao inserir Pessoa ID %d (Duplicado?)\n", d.codigo);
        }
        else if(cmd.operacao == OP_DELETE) {
            int id = atoi(cmd.valores[0]);
            
            // VALIDAÇÃO CRÍTICA (Que o amigo não fez)
            if(aux_pessoa_tem_pet(lista_pets, id)) {
                printf("[ERRO] Pessoa %d tem pets e nao pode ser removida.\n", id);
            } else {
                if(remover_pessoa(lista_pessoas, id) == 0) printf("[OK] Removido Pessoa ID %d\n", id);
                else printf("[ERRO] Pessoa ID %d nao encontrada.\n", id);
            }
        }
        else if(cmd.operacao == OP_SELECT) {
            if(cmd.tem_order_by) {
                // --- ÁRVORE ENTRA AQUI ---
                printf("\n[RELATORIO ARVORE] Pessoas Ordenadas:\n");
                NoArvore *raiz = arv_criar();
                pessoa_no *curr = lista_pessoas->cabeca;
                
                // Preenche a árvore com os dados da lista
                while(curr != NULL) {
                    raiz = arv_inserir(raiz, curr->data.codigo, (void*)curr);
                    curr = curr->prox;
                }
                
                arv_imprimir_em_ordem(raiz, imprimir_pessoa_arvore);
                arv_liberar(raiz); // Libera memória da árvore
                printf("--------------------------------------\n");
            } else {
                printf("[SELECT] Listagem simples sem ordem (Implementar se quiser)\n");
            }
        }
    }

    // 5. Executar Fila de PETS
    printf("\n--- Processando PETS ---\n");
    while(!filaVazia(&fila_pet)) {
        Comando cmd = remover(&fila_pet);

        if(cmd.operacao == OP_INSERT) {
            pet_dados d;
            d.codigo = atoi(cmd.valores[0]);
            d.codigo_pessoa = atoi(cmd.valores[1]);
            strcpy(d.nome, cmd.valores[2]);
            d.codigo_tipo = atoi(cmd.valores[3]);

            // VALIDAÇÕES CRÍTICAS
            if(!aux_pessoa_existe(lista_pessoas, d.codigo_pessoa)) {
                printf("[ERRO] Pet %d cancelado: Dono %d nao existe.\n", d.codigo, d.codigo_pessoa);
            }
            else if(!aux_tipo_existe(lista_tipos, d.codigo_tipo)) {
                printf("[ERRO] Pet %d cancelado: Tipo %d nao existe.\n", d.codigo, d.codigo_tipo);
            }
            else {
                if(criar_pet(lista_pets, d) == 0) printf("[OK] Inserido Pet %d\n", d.codigo);
                else printf("[ERRO] Falha ao criar Pet %d\n", d.codigo);
            }
        }
        // Adicionar lógica de DELETE e UPDATE se necessário
    }

    // 6. Salvar Tudo
    pessoa_salvar_arquivo(lista_pessoas, "pessoas.txt");
    pet_salvar_arquivo(lista_pets, "pets.txt");
    tipo_pet_salvar_arquivo(lista_tipos, "tipos.txt");
    
    // Opcional: Liberar listas aqui
    printf("\n=== FIM DE EXECUCAO ===\n");
    return 0;
}