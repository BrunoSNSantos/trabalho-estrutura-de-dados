#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parser.h" 

#ifdef _WIN32
    #define strcasecmp stricmp
#endif


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

void limpar_coluna(char *str) {
    char *src = str, *dst = str;
    while(*src) {
        if (*src != '(' && *src != ')' && *src != ',') {
            *dst++ = *src;
        }
        src++;
    }
    *dst = '\0';
}

// --- Função Principal: Ler Linha ---

void ler_linha(char *linha, Fila *f) {
    char buffer[500];
    strcpy(buffer, linha);

    Comando cmd = {0};

    // 1. Identificar OPERAÇÃO
    char *token = strtok(buffer, " ");
    if (!token) return;

    if (strcasecmp(token, "insert") == 0) cmd.operacao = OP_INSERT;
    else if (strcasecmp(token, "delete") == 0) cmd.operacao = OP_DELETE;
    else if (strcasecmp(token, "select") == 0) cmd.operacao = OP_SELECT;
    else if (strcasecmp(token, "update") == 0) cmd.operacao = OP_UPDATE;
    else return;

    // 2. Identificar TABELA
    token = strtok(NULL, " ");
    // Pula palavras de ligação ("into", "from")
    while (token && (strcasecmp(token, "into") == 0 || strcasecmp(token, "from") == 0)) {
        token = strtok(NULL, " ");
    }

    if (!token) return;

    if (strcasecmp(token, "pessoa") == 0) cmd.tabela = TAB_PESSOA;
    else if (strcasecmp(token, "pet") == 0) cmd.tabela = TAB_PET;
    else if (strcasecmp(token, "tipo_pet") == 0) cmd.tabela = TAB_TIPO_PET;
    else return;
    // =========================================================
    // BLOCO DO INSERT
    // =========================================================
    if (cmd.operacao == OP_INSERT) {
        char temp_cols[6][50];
        char temp_vals[6][50];
        int qtd = 0;

        // A. LER NOMES DAS COLUNAS
        while ((token = strtok(NULL, " ,()")) != NULL && qtd < 6) {
            
            if (strcasecmp(token, "values") == 0) break; 
            
            strcpy(temp_cols[qtd], token);
            qtd++;
        }

        // B. LER VALORES
        int i = 0;
        while ((token = strtok(NULL, "(),;")) != NULL && i < 6) {
            limpar_string(token);
            strcpy(temp_vals[i], token);
            i++;
        }

        // C. MAPEAMENTO (De -> Para)
        for (int k = 0; k < qtd; k++) {
            char *col = temp_cols[k];
            char *val = temp_vals[k];
            int index = -1;

            // --- Mapa PESSOA ---
            if (cmd.tabela == TAB_PESSOA) {
                if (strcasecmp(col, "codigo") == 0) index = 0;
                else if (strcasecmp(col, "nome") == 0) index = 1;
                else if (strcasecmp(col, "fone") == 0) index = 2;
                else if (strcasecmp(col, "endereco") == 0) index = 3;
            }
            // --- Mapa PET ---
            else if (cmd.tabela == TAB_PET) {
                if (strcasecmp(col, "codigo") == 0) index = 0;
                else if (strcasecmp(col, "id_pessoa") == 0) index = 1;
                else if (strcasecmp(col, "nome") == 0) index = 2;
                else if (strcasecmp(col, "id_tipo") == 0) index = 3;
            }
            // --- Mapa TIPO_PET ---
            else if (cmd.tabela == TAB_TIPO_PET) {
                if (strcasecmp(col, "codigo") == 0) index = 0;
                else if (strcasecmp(col, "descricao") == 0) index = 1;
            }

            if (index != -1) strcpy(cmd.valores[index], val);
        }
        cmd.qtd_params = 4;
    }

    // =========================================================
    // BLOCO DO DELETE
    // =========================================================
    else if (cmd.operacao == OP_DELETE) {
        while (token && strcasecmp(token, "where") != 0) token = strtok(NULL, " ");
        
        token = strtok(NULL, " =;"); // Campo
        if (token) strcpy(cmd.campos[0], token);
        
        token = strtok(NULL, " =;"); // Valor
        if (token) {
            limpar_string(token);
            strcpy(cmd.valores[0], token);
            cmd.qtd_params = 1;
        }
    }
    
    // =========================================================
    // BLOCO DO SELECT
    // =========================================================
    else if (cmd.operacao == OP_SELECT) {
        int tem_where = 0;
        cmd.tem_order_by = 0; // Inicializa com 0
        
        // Varre procurando 'where' ou 'order'
        char *token_ptr = token; // Guarda ponteiro atual para continuar busca se precisar
        
        // Lógica para WHERE
        while (token != NULL) {
            if (strcasecmp(token, "where") == 0) { tem_where = 1; break; }
            token = strtok(NULL, " ");
        }

        if (tem_where) {
            token = strtok(NULL, " =;");
            if (token) strcpy(cmd.campos[0], token);
            
            token = strtok(NULL, " =;");
            if (token) {
                limpar_string(token);
                strcpy(cmd.valores[0], token);
                cmd.qtd_params = 1;
            }
        } else {
            cmd.qtd_params = 0; // Select *
        }
        
        // --- NOVO BLOCO: Lógica para ORDER BY ---
        // Reinicia a busca no buffer ou continua (dependendo de como o strtok parou)
        // Como strtok é destrutivo, vamos assumir que o resto da string ainda está lá.
        // O jeito mais seguro no seu parser atual é continuar pedindo tokens:
        
        while (token != NULL) {
            if (strcasecmp(token, "order") == 0) {
                token = strtok(NULL, " "); // Pega o "by"
                if (token && strcasecmp(token, "by") == 0) {
                    // PDF diz que é order by nome.
                    // Se quiser ler o campo: token = strtok(NULL, " ;");
                    cmd.tem_order_by = 1; 
                }
            }
            token = strtok(NULL, " ");
        }
    }

    // =========================================================
    // BLOCO DO UPDATE
    // =========================================================
    else if (cmd.operacao == OP_UPDATE) {
        // Procura SET
        while (token && strcasecmp(token, "set") != 0) token = strtok(NULL, " ");
        
        // Dados do SET
        if ((token = strtok(NULL, " ="))) strcpy(cmd.campos[0], token);
        if ((token = strtok(NULL, " ="))) { limpar_string(token); strcpy(cmd.valores[0], token); }

        // Procura WHERE
        while (token && strcasecmp(token, "where") != 0) token = strtok(NULL, " ");
        
        // Dados do WHERE
        if ((token = strtok(NULL, " =;"))) strcpy(cmd.campos[1], token);
        if ((token = strtok(NULL, " =;"))) { limpar_string(token); strcpy(cmd.valores[1], token); }
        
        cmd.qtd_params = 2;
    }

    // FINAL: Enfileirar
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
        // Ignora linhas vazias ou muito curtas
        if(strlen(linha) > 2) {
            ler_linha(linha, f);
        }
    }
    fclose(arq);
}