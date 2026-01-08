#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parcer.h"

void limpar_string(char *str) {
    char *src, *dst;
    while(*src) {
        if (*src != '\'' && *src != '\"' && *src != '\n' && *src != '\r') {
            *dst++ = *src;
        }
        *src++;
    }
}

void ler_linha(char *linha, Fila *f) {
    Comando cmd = {0};
    char buffer[500];
    strcpy(buffer, linha);
    char *token = strtok(buffer, " ");
    if (strcasecmp(token, "insert")) cmd.operacao = OP_INSERT;

    while(token && strcasecmp(token, "into") == 0)token = strtok(NULL, " *");

    if(strcasecmp(token, "pessoa")) cmd.tabela = TAB_PESSOA;

    
}