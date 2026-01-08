#ifndef PARCER_H
#define PARCER_H
#include "fila.h"

void processar_arquvio(char *nome_arquivo, Fila *f);
void ler_linha(char *linha, Fila *f);

#endif