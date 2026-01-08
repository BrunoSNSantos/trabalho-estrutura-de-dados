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

    // 1. Inicializar Listas (Com os nomes corrigidos)
    pessoa_lista *lista_pessoas = iniciar_pessoa_lista(); 
    pet_lista *lista_pets = iniciar_pet_lista();
    tipo_pet_lista *lista_tipos = iniciar_tipo_pet_lista(); // Confirme se o nome desta função está correto no .h

    // Carregar dados (Com os nomes corrigidos)
    pessoa_carregar_arquivo(lista_pessoas, "pessoas.txt");
    pet_carregar_arquivo(lista_pets, "pets.txt");
    tipo_pet_carregar_arquivo(lista_tipos, "tipos.txt"); 

    // 2. Processar Script
    Fila fila_geral;
    iniciarFila(&fila_geral);
    
    // Tenta processar o arquivo. Certifique-se que script.txt está na pasta correta.
    processar_arquivo("script.txt", &fila_geral); 

    // 3. Separação em Filas Específicas
    Fila fila_pessoa, fila_pet, fila_tipo;
    iniciarFila(&fila_pessoa);
    iniciarFila(&fila_pet);
    iniciarFila(&fila_tipo);

    while(!filaVazia(&fila_geral)) {
        Comando cmd = remover(&fila_geral);
        if(cmd.tabela == TAB_PESSOA) adicionar(&fila_pessoa, cmd);
        else if(cmd.tabela == TAB_PET) adicionar(&fila_pet, cmd);
        else if(cmd.tabela == TAB_TIPO_PET) adicionar(&fila_tipo, cmd);
    }

    // 4. Processar TIPOS (Primeiro, para garantir integridade dos Pets)
    printf("\n--- Processando TIPOS ---\n");
    while(!filaVazia(&fila_tipo)) {
        Comando cmd = remover(&fila_tipo);
        if(cmd.operacao == OP_INSERT) {
            tipo_pet_dados d;
            d.codigo = atoi(cmd.valores[0]);
            strcpy(d.descricao, cmd.valores[1]);
            
            // Assumindo que criar_tipo_pet retorna 0 para sucesso
            // Se a função no seu .h for void, remova o "int res ="
            criar_tipo_pet(lista_tipos, d);
            printf("[OK] Tipo %d processado.\n", d.codigo);
        }
    }

    // 5. Processar PESSOAS
    printf("\n--- Processando PESSOAS ---\n");
    while(!filaVazia(&fila_pessoa)) {
        Comando cmd = remover(&fila_pessoa);

        if(cmd.operacao == OP_INSERT) {
            pessoa_dados d;
            d.codigo = atoi(cmd.valores[0]);
            strcpy(d.nome, cmd.valores[1]);
            strcpy(d.fone, cmd.valores[2]);
            
            // Tratamento de opcionais
            if(cmd.qtd_params > 3) strcpy(d.endereco, cmd.valores[3]);
            else strcpy(d.endereco, "");
            
            if(cmd.qtd_params > 4) strcpy(d.data_nascimento, cmd.valores[4]);
            else strcpy(d.data_nascimento, "");
            
            int res = criar_pessoa(lista_pessoas, d);
            if(res == 0) printf("[OK] Pessoa %d criada.\n", d.codigo);
            else printf("[ERRO] Falha na Pessoa %d (Erro: %d)\n", d.codigo, res);
        }
        else if(cmd.operacao == OP_DELETE) {
            int id = atoi(cmd.valores[0]);
            
            int res = remover_pessoa(lista_pessoas, id);
            
            if(res == 0) printf("[OK] Pessoa %d removida.\n", id);
            else printf("[ERRO] Falha ao remover Pessoa %d (Erro: %d)\n", id, res);
        }
        else if(cmd.operacao == OP_SELECT) {
            if(cmd.tem_order_by) {
                // Chama a função que movemos para pessoa.c
                // Certifique-se que ela está declarada em pessoa.h
                pessoa_gerar_relatorio_ordenado(lista_pessoas);
            }
        }
    }

    // 6. Processar PETS
    printf("\n--- Processando PETS ---\n");
    while(!filaVazia(&fila_pet)) {
        Comando cmd = remover(&fila_pet);

        if(cmd.operacao == OP_INSERT) {
            pet_dados d;
            d.codigo = atoi(cmd.valores[0]);
            d.codigo_pessoa = atoi(cmd.valores[1]);
            strcpy(d.nome, cmd.valores[2]);
            d.codigo_tipo = atoi(cmd.valores[3]);

            // AQUI ESTÁ A ATUALIZAÇÃO IMPORTANTE:
            // Passando as 3 listas para a validação funcionar
            int res = criar_pet(lista_pets, lista_pessoas, lista_tipos, d);
            
            if(res == 0) printf("[OK] Pet %d criado.\n", d.codigo);
            else if (res == -2) printf("[ERRO] Pet %d cancelado: Dono %d nao existe.\n", d.codigo, d.codigo_pessoa);
            else if (res == -3) printf("[ERRO] Pet %d cancelado: Tipo %d nao existe.\n", d.codigo, d.codigo_tipo);
            else printf("[ERRO] Falha ao criar Pet %d (Erro: %d)\n", d.codigo, res);
        }
    }

    // 7. Salvar e Sair
    printf("\nSalvando arquivos...\n");
    pessoa_salvar_arquivo(lista_pessoas, "pessoas.txt");
    pet_salvar_arquivo(lista_pets, "pets.txt");
    tipo_pet_salvar_arquivo(lista_tipos, "tipos.txt");
    
    // Liberar memória das filas auxiliares (Opcional, mas boa prática)
    // liberarFila(&fila_geral); ...

    return 0;
}