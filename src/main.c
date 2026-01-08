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

    // 1. Inicializar Listas
    pessoa_lista *lista_pessoas = iniciar_pessoa_lista(); 
    pet_lista *lista_pets = iniciar_pet_lista();
    tipo_pet_lista *lista_tipos = iniciar_tipo_pet_lista();

    // 2. Carregar dados (ATENÇÃO À ORDEM PARA VALIDAR FK)
    
    // 1º Carrega Tipos (Pet precisa deles)
    tipo_pet_carregar_arquivo(lista_tipos, "tipos.txt"); 
    
    // 2º Carrega Pessoas (Pet precisa delas)
    pessoa_carregar_arquivo(lista_pessoas, "pessoas.txt");
    
    // 3º Carrega Pets (Agora valida Pessoa e Tipo na carga)
    // ATUALIZAÇÃO: Passando as 3 listas conforme o código do seu amigo
    pet_carregar_arquivo(lista_pets, lista_pessoas, lista_tipos, "pets.txt");


    // 3. Processar Script
    Fila fila_geral;
    iniciarFila(&fila_geral);
    
    processar_arquivo("script.txt", &fila_geral); 

    // 4. Separação em Filas Específicas
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

    // 5. Processar TIPOS
    printf("\n--- Processando TIPOS ---\n");
    while(!filaVazia(&fila_tipo)) {
        Comando cmd = remover(&fila_tipo);
        if(cmd.operacao == OP_INSERT) {
            tipo_pet_dados d;
            d.codigo = atoi(cmd.valores[0]);
            strcpy(d.descricao, cmd.valores[1]);
            
            criar_tipo_pet(lista_tipos, d);
            printf("[OK] Tipo %d processado.\n", d.codigo);
        }
    }

    // 1º TURNO: Criar TIPOS e PESSOAS (Base para os Pets)
    printf("\n--- Turno 1: Carga de Base ---\n");
    
    // Processa apenas INSERTS de Tipos
    while(!filaVazia(&fila_tipo)) {
        Comando cmd = remover(&fila_tipo);
        if(cmd.operacao == OP_INSERT) {
            tipo_pet_dados d;
            d.codigo = atoi(cmd.valores[0]);
            strcpy(d.descricao, cmd.valores[1]);
            criar_tipo_pet(lista_tipos, d);
            printf("[OK] Tipo %d processado.\n", d.codigo);
        }
    }

    // Processa apenas INSERTS de Pessoas
    // Criamos uma fila temporária para guardar os DELETEs e SELECTs para o 3º turno
    Fila fila_pessoa_restante;
    iniciarFila(&fila_pessoa_restante);

    while(!filaVazia(&fila_pessoa)) {
        Comando cmd = remover(&fila_pessoa);
        if(cmd.operacao == OP_INSERT) {
            pessoa_dados d;
            d.codigo = atoi(cmd.valores[0]);
            strcpy(d.nome, cmd.valores[1]);
            strcpy(d.fone, cmd.valores[2]);
            if(cmd.qtd_params > 3) strcpy(d.endereco, cmd.valores[3]);
            if(cmd.qtd_params > 4) strcpy(d.data_nascimento, cmd.valores[4]);
            
            if(criar_pessoa(lista_pessoas, d) == 0) printf("[OK] Pessoa %d criada.\n", d.codigo);
        } else {
            adicionar(&fila_pessoa_restante, cmd);
        }
    }

    // 2º TURNO: Criar PETS (Agora os donos já existem na lista!)
    printf("\n--- Turno 2: Processando PETS ---\n");
    while(!filaVazia(&fila_pet)) {
        Comando cmd = remover(&fila_pet);
        if(cmd.operacao == OP_INSERT) {
            pet_dados d;
            d.codigo = atoi(cmd.valores[0]);
            d.codigo_pessoa = atoi(cmd.valores[1]);
            strcpy(d.nome, cmd.valores[2]);
            d.codigo_tipo = atoi(cmd.valores[3]);

            if(criar_pet(lista_pets, lista_pessoas, lista_tipos, d) == 0) 
                printf("[OK] Pet %d criado.\n", d.codigo);
            else 
                printf("[ERRO] Falha no Pet %d (Dono ou Tipo invalido).\n", d.codigo);
        }
    }

    // 3º TURNO: Ações Críticas (SELECT e DELETE)
    printf("\n--- Turno 3: Relatorios e Remocoes ---\n");
    while(!filaVazia(&fila_pessoa_restante)) {
        Comando cmd = remover(&fila_pessoa_restante);
        
        if(cmd.operacao == OP_SELECT && cmd.tem_order_by) {
            pessoa_gerar_relatorio_ordenado(lista_pessoas);
        } 
        else if(cmd.operacao == OP_DELETE) {
            int id = atoi(cmd.valores[0]);
            if(pet_existe_dono(lista_pets, id)) {
                printf("[ERRO] Pessoa %d possui Pets e nao pode ser removida.\n", id);
            } else {
                if(remover_pessoa(lista_pessoas, id) == 0) printf("[OK] Pessoa %d removida.\n", id);
            }
        }
    }

    // 8. Salvar e Sair
    printf("\nSalvando arquivos...\n");
    pessoa_salvar_arquivo(lista_pessoas, "pessoas.txt");
    pet_salvar_arquivo(lista_pets, "pets.txt");
    tipo_pet_salvar_arquivo(lista_tipos, "tipos.txt");
    
    return 0;
}