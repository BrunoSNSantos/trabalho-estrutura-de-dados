#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parser.h" 

void limpar_string(char *str) {
    char *src = str, *dst = str;
    while (*src) {
        if (*src != '\'' && *src != '\"' && *src != '\n' && *src != '\r') {
            *dst++ = *src;
        }
        src++;
    }
    *dst = '\0';
}

void reparar_token(char *token_inicial, char *destino) {
    strcpy(destino, token_inicial);
    if (token_inicial[0] == '\'' && token_inicial[strlen(token_inicial)-1] != '\'') {
        char *proximo;
        while ((proximo = strtok(NULL, " "))) {
            strcat(destino, " ");
            strcat(destino, proximo);
            if (proximo[strlen(proximo)-1] == '\'') break;
        }
    }
}

void ler_linha(char *linha, Fila *f) {
    char buffer[500];
    strcpy(buffer, linha);
    Comando cmd = {0};
    
    // 1. Pega a operacao (INSERT, DELETE...)
    char *token = strtok(buffer, " ");

    if (!token) return;

    if (strcasecmp(token, "insert") == 0) cmd.operacao = OP_INSERT;
    else if (strcasecmp(token, "delete") == 0) cmd.operacao = OP_DELETE;
    else if (strcasecmp(token, "select") == 0) cmd.operacao = OP_SELECT;
    else if (strcasecmp(token, "update") == 0) cmd.operacao = OP_UPDATE;
    else return;

    // 2. Pula palavras chaves (INTO, FROM) e busca a tabela
    // CORRECAO AQUI: Adicionado '(' nos delimitadores para separar "pessoa(codigo..."
    token = strtok(NULL, " ("); 
    while (token && (strcasecmp(token, "into") == 0 || strcasecmp(token, "from") == 0 || strcmp(token, "*") == 0)) {
        token = strtok(NULL, " ("); // CORRECAO AQUI TAMBEM
    }

    if (!token) return;

    // Agora o token deve ser apenas "pessoa", "pet" ou "tipo_pet" limpo
    if (strcasecmp(token, "pessoa") == 0) cmd.tabela = TAB_PESSOA;
    else if (strcasecmp(token, "pet") == 0) cmd.tabela = TAB_PET;
    else if (strcasecmp(token, "tipo_pet") == 0) cmd.tabela = TAB_TIPO_PET;
    else return;

    if (cmd.operacao == OP_INSERT) {
        char temp_cols[6][50];
        char temp_vals[6][50];
        int qtd = 0;

        // Note: Aqui ja usamos " ,()" entao o resto funciona bem
        while ((token = strtok(NULL, " ,()")) != NULL && qtd < 6) {
            if (strcasecmp(token, "values") == 0) break; 
            strcpy(temp_cols[qtd], token);
            qtd++;
        }

        int i = 0;
        // Parsing dos valores
        while ((token = strtok(NULL, "(),;")) != NULL && i < 6) {
            limpar_string(token);
            // Pequeno trim manual para remover espacos no inicio de valores (ex: " 'Joao'")
            char *start = token;
            while(*start == ' ') start++;
            strcpy(temp_vals[i], start);
            i++;
        }

        for (int k = 0; k < qtd; k++) {
            char *col = temp_cols[k];
            char *val = temp_vals[k];
            int index = -1;

            if (cmd.tabela == TAB_PESSOA) {
                if (strcasecmp(col, "codigo") == 0) index = 0;
                else if (strcasecmp(col, "nome") == 0) index = 1;
                else if (strcasecmp(col, "fone") == 0) index = 2;
                else if (strcasecmp(col, "endereco") == 0) index = 3;
                else if (strcasecmp(col, "data_nascimento") == 0) index = 4;
            } else if (cmd.tabela == TAB_PET) {
                if (strcasecmp(col, "codigo") == 0) index = 0;
                else if (strcasecmp(col, "codigo_pes") == 0 || strcasecmp(col, "codigo_cli") == 0) index = 1;
                else if (strcasecmp(col, "nome") == 0) index = 2;
                else if (strcasecmp(col, "codigo_tipo") == 0) index = 3;
            } else if (cmd.tabela == TAB_TIPO_PET) {
                if (strcasecmp(col, "codigo") == 0) index = 0;
                else if (strcasecmp(col, "descricao") == 0) index = 1;
            }
            if (index != -1) strcpy(cmd.valores[index], val);
        }
        cmd.qtd_params = 4; // Ajuste conforme necessidade ou logica dinamica

    } else if (cmd.operacao == OP_DELETE) {
        while (token && strcasecmp(token, "where") != 0) token = strtok(NULL, " ");
        
        token = strtok(NULL, " =;");
        if (token) strcpy(cmd.campos[0], token);
        
        token = strtok(NULL, " =;");
        if (token) {
            limpar_string(token);
            strcpy(cmd.valores[0], token);
            cmd.qtd_params = 1;
        }

    } else if (cmd.operacao == OP_SELECT) {
        cmd.tem_order_by = 0;
        cmd.qtd_params = 0;
        while (token != NULL) {
            if (strcasecmp(token, "where") == 0) {
                token = strtok(NULL, " =;");
                if (token) strcpy(cmd.campos[0], token);
                
                token = strtok(NULL, " =;");
                if (token) {
                    limpar_string(token);
                    strcpy(cmd.valores[0], token);
                    cmd.qtd_params = 1;
                }
            } else if (strcasecmp(token, "order") == 0) {
                char *prox = strtok(NULL, " ;"); 
                if (prox && strcasecmp(prox, "by") == 0) {
                    cmd.tem_order_by = 1;
                }
            }
            token = strtok(NULL, " ");
        }

    } else if (cmd.operacao == OP_UPDATE) {
        while (token && strcasecmp(token, "set") != 0) token = strtok(NULL, " ");
        if ((token = strtok(NULL, " ="))) strcpy(cmd.campos[0], token);
        
        if ((token = strtok(NULL, " ="))) { 
            char buffer_reparado[100];
            reparar_token(token, buffer_reparado);
            limpar_string(buffer_reparado); 
            strcpy(cmd.valores[0], buffer_reparado); 
        }

        while (token && strcasecmp(token, "where") != 0) token = strtok(NULL, " ");
        if ((token = strtok(NULL, " =;"))) strcpy(cmd.campos[1], token);
        
        if ((token = strtok(NULL, " =;"))) { 
            char buffer_reparado[100];
            reparar_token(token, buffer_reparado);
            limpar_string(buffer_reparado); 
            strcpy(cmd.valores[1], buffer_reparado); 
        }
        
        cmd.qtd_params = 2;
    }
    adicionar(f, cmd);
}

void processar_arquivo(char *nome_arquivo, Fila *f) {
    FILE *arq = fopen(nome_arquivo, "r");

    if(!arq) {
        printf("Erro: Nao foi possivel abrir o arquivo: %s\n", nome_arquivo);
        return;
    }

    char linha[200];
    while(fgets(linha, sizeof(linha), arq)) {
        // Remove \n e \r do final da linha antes de processar
        linha[strcspn(linha, "\r\n")] = 0;
        
        if(strlen(linha) > 2) {
            ler_linha(linha, f);
        }
    }
    fclose(arq);
}