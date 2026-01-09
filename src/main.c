#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pessoa.h"
#include "pet.h"
#include "tipo_pet.h"
#include "fila.h"
#include "parser.h"

int main() {
    printf("=== SISTEMA INICIADO ===\n");

    // 1. Inicializa Listas
    pessoa_lista *lista_pessoas = iniciar_pessoa_lista(); 
    pet_lista *lista_pets = iniciar_pet_lista();
    tipo_pet_lista *lista_tipos = iniciar_tipo_pet_lista();

    // 2. Carrega Arquivos Existentes
    tipo_pet_carregar_arquivo(lista_tipos, "tipos.txt"); 
    pessoa_carregar_arquivo(lista_pessoas, "pessoas.txt");
    pet_carregar_arquivo(lista_pets, lista_pessoas, lista_tipos, "pets.txt");

    // 3. Lê o Script para a Fila Geral
    Fila fila_geral;
    iniciarFila(&fila_geral);
    processar_arquivo("script.txt", &fila_geral); 

    printf("\n--- Processando Script ---\n");

    // 4. Processa a Fila Geral UM POR UM (Sem separar por tabelas)
    while(!filaVazia(&fila_geral)) {
        Comando cmd = remover(&fila_geral);

        // ==========================================================
        // TIPO PET
        // ==========================================================
        if (cmd.tabela == TAB_TIPO_PET) {
            if (cmd.operacao == OP_INSERT) {
                tipo_pet_dados d;
                d.codigo = atoi(cmd.valores[0]);
                strcpy(d.descricao, cmd.valores[1]);
                if (criar_tipo_pet(lista_tipos, d) == 0)
                    printf("[OK] Tipo %d criado.\n", d.codigo);
                else
                    printf("[ERRO] Tipo %d ja existe.\n", d.codigo);
            }
            else if (cmd.operacao == OP_UPDATE) {
                int id = atoi(cmd.valores[1]);
                if (atualizar_tipo_pet(lista_tipos, id, cmd.campos[0], cmd.valores[0]) == 0)
                    printf("[OK] Tipo %d atualizado.\n", id);
                else
                    printf("[ERRO] Falha update Tipo %d.\n", id);
            }
            // Implementar Delete Tipo se necessario
        }

        // ==========================================================
        // PESSOA
        // ==========================================================
        else if (cmd.tabela == TAB_PESSOA) {
            if (cmd.operacao == OP_INSERT) {
                pessoa_dados d;
                d.codigo = atoi(cmd.valores[0]);
                strcpy(d.nome, cmd.valores[1]);
                strcpy(d.fone, cmd.valores[2]);
                if(cmd.qtd_params > 3) strcpy(d.endereco, cmd.valores[3]);
                if(cmd.qtd_params > 4) strcpy(d.data_nascimento, cmd.valores[4]);
                
                if(criar_pessoa(lista_pessoas, d) == 0) 
                    printf("[OK] Pessoa %d criada.\n", d.codigo);
                else
                    printf("[ERRO] Pessoa %d ja existe.\n", d.codigo);
            }
            else if (cmd.operacao == OP_DELETE) {
                int id = atoi(cmd.valores[0]);
                
                // VALIDACAO CRITICA: Verifica vinculo antes de apagar
                if(pet_existe_dono(lista_pets, id)) {
                    printf("[ERRO] Pessoa %d possui Pets e nao pode ser removida.\n", id);
                } else {
                    if(remover_pessoa(lista_pessoas, id) == 0) 
                        printf("[OK] Pessoa %d removida.\n", id);
                    else 
                        printf("[ERRO] Pessoa %d nao encontrada para remocao.\n", id);
                }
            }
            else if (cmd.operacao == OP_UPDATE) {
                int id = atoi(cmd.valores[1]);
                if (atualizar_pessoa(lista_pessoas, id, cmd.campos[0], cmd.valores[0]) == 0)
                    printf("[OK] Pessoa %d atualizada.\n", id);
                else 
                    printf("[ERRO] Falha update Pessoa %d.\n", id);
            }
            else if (cmd.operacao == OP_SELECT && cmd.tem_order_by) {
                pessoa_gerar_relatorio_ordenado(lista_pessoas);
            }
        }

        // ==========================================================
        // PET
        // ==========================================================
        else if (cmd.tabela == TAB_PET) {
            if (cmd.operacao == OP_INSERT) {
                pet_dados d;
                d.codigo = atoi(cmd.valores[0]);
                d.codigo_pessoa = atoi(cmd.valores[1]);
                strcpy(d.nome, cmd.valores[2]);
                d.codigo_tipo = atoi(cmd.valores[3]);

                if(criar_pet(lista_pets, lista_pessoas, lista_tipos, d) == 0) 
                    printf("[OK] Pet %d criado.\n", d.codigo);
                else 
                    printf("[ERRO] Falha criar Pet %d (Dados invalidos ou duplicado).\n", d.codigo);
            }
            else if (cmd.operacao == OP_SELECT && cmd.tem_order_by) {
                pet_gerar_relatorio_ordenado(lista_pets);
            }
            else if (cmd.operacao == OP_DELETE) {
                int id = atoi(cmd.valores[0]);
                if(remover_pet(lista_pets, id) == 0)
                    printf("[OK] Pet %d removido.\n", id);
                else
                    printf("[ERRO] Pet %d nao encontrado.\n", id);
            }
            else if (cmd.operacao == OP_UPDATE) {
                int id = atoi(cmd.valores[1]);
                int res = atualizar_pet(lista_pets, lista_pessoas, lista_tipos, id, cmd.campos[0], cmd.valores[0]);
                if (res == 0) printf("[OK] Pet %d atualizado.\n", id);
                else printf("[ERRO] Falha update Pet %d (Cod erro: %d).\n", id, res);
            }
        }
    }

    printf("\nSalvando arquivos...\n");
    pessoa_salvar_arquivo(lista_pessoas, "pessoas.txt");
    pet_salvar_arquivo(lista_pets, "pets.txt");
    tipo_pet_salvar_arquivo(lista_tipos, "tipos.txt");
    
    // Nao esqueca de liberar memoria das listas aqui (liberar_pessoa_lista, etc)
    
    return 0;
}