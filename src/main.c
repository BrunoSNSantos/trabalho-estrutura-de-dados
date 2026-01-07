#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fila.h"

int main() {
    Fila testeFila;
    iniciarFila(&testeFila);

    Comando c1;
    c1.operacao = OP_INSERT;
    c1.tabela = TAB_PESSOA;
    c1.qtd_params = 2;
    strcpy(c1.campos[0], "codigo");
    strcpy(c1.valores[0], "10");
    strcpy(c1.campos[1], "nome");
    strcpy(c1.valores[1], "Carlos");

    printf("Adicionando na fila...\n");
    adicionar(&testeFila, c1);
    if (!filaVazia(&testeFila)) {
        Comando removido = remover(&testeFila);
        printf("Sucesso! Comando retirado:\n");
        printf("Operacao: %d (Esperado: 1)\n", removido.operacao);
        printf("Valor 1: %s (Esperado: 10)\n", removido.valores[0]);
    } else {
        printf("Erro: A fila estava vazia.\n");
    }

    return 0;
}