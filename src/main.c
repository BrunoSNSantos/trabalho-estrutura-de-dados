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

    pessoa_lista *lista_pessoas = iniciar_pessoa_lista(); 
    pet_lista *lista_pets = iniciar_pet_lista();
    tipo_pet_lista *lista_tipos = iniciar_tipo_pet_lista();

    tipo_pet_carregar_arquivo(lista_tipos, "tipos.txt"); 
    pessoa_carregar_arquivo(lista_pessoas, "pessoas.txt");
    pet_carregar_arquivo(lista_pets, lista_pessoas, lista_tipos, "pets.txt");

    Fila fila_geral;
    iniciarFila(&fila_geral);
    processar_arquivo("script.txt", &fila_geral); 

    Fila fila_pessoa, fila_pet, fila_tipo;
    iniciarFila(&fila_pessoa);
    iniciarFila(&fila_pet);
    iniciarFila(&fila_tipo);

    printf("\n--- Processando Script ---\n");

    while(!filaVazia(&fila_geral)) {
        Comando cmd = remover(&fila_geral);

        if (cmd.tabela == TAB_TIPO_PET) {
            if (cmd.operacao == OP_INSERT) {
                if(strlen(cmd.valores[0]) == 0 || strlen(cmd.valores[1]) == 0) {
                     printf("[ERRO] Tipo Pet NAO criado: Codigo e Nome sao obrigatorios.\n");
                     continue;
                }
                
                tipo_pet_dados d;
                d.codigo = atoi(cmd.valores[0]);
                strcpy(d.nome, cmd.valores[1]);
                
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
            else if (cmd.operacao == OP_DELETE) {
                int id = atoi(cmd.valores[0]);
                printf("[OK] Tipo %d removido (simulacao).\n", id);
            }
        }

        else if (cmd.tabela == TAB_PESSOA) {
            if (cmd.operacao == OP_INSERT) {
                if(strlen(cmd.valores[0]) == 0 || strlen(cmd.valores[1]) == 0) {
                    printf("[ERRO] Pessoa NAO criada: Codigo e Nome sao obrigatorios.\n");
                    continue;
                }

                pessoa_dados d;
                d.codigo = atoi(cmd.valores[0]);
                strcpy(d.nome, cmd.valores[1]);
                
                if(strlen(cmd.valores[2]) > 0) strcpy(d.fone, cmd.valores[2]);
                else strcpy(d.fone, "");

                if(cmd.qtd_params > 3 && strlen(cmd.valores[3]) > 0) 
                    strcpy(d.endereco, cmd.valores[3]);
                else strcpy(d.endereco, "");
                
                if(cmd.qtd_params <= 4 || strlen(cmd.valores[4]) == 0) {
                    printf("[ERRO] Pessoa %d NAO criada. Data de Nascimento eh obrigatoria.\n", d.codigo);
                } 
                else {
                    strcpy(d.data_nascimento, cmd.valores[4]);
                    
                    if(criar_pessoa(lista_pessoas, d) == 0) 
                        printf("[OK] Pessoa %d criada.\n", d.codigo);
                    else
                        printf("[ERRO] Pessoa %d ja existe.\n", d.codigo);
                }
            }
            else if (cmd.operacao == OP_DELETE) {
                int id = atoi(cmd.valores[0]);
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

        else if (cmd.tabela == TAB_PET) {
            if (cmd.operacao == OP_INSERT) {
                if(strlen(cmd.valores[0]) == 0 || strlen(cmd.valores[1]) == 0 || 
                   strlen(cmd.valores[2]) == 0 || strlen(cmd.valores[3]) == 0) {
                    printf("[ERRO] Pet NAO criado: Todos os campos sao obrigatorios.\n");
                    continue;
                }

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
    
    printf("Liberando memoria...\n");
    liberar_pessoa_lista(lista_pessoas);
    liberar_pet_lista(lista_pets);
    liberar_tipo_pet_lista(lista_tipos);
    
    printf("\n=== SISTEMA FINALIZADO ===\n");
    return 0;
}