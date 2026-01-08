#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pessoa.h"
#include "tipo_pet.h"
#include "pet.h"

void testar_pessoa() {
    printf("\n===== TESTE PESSOA =====\n");

    pessoa_lista *lp = iniciar_pessoa_lista();

    pessoa_dados p1 = {1, "Ana", "9999-1111", "Rua A", "01/01/2000"};
    pessoa_dados p2 = {2, "Bruno", "8888-2222", "Rua B", "02/02/1999"};

    criar_pessoa(lp, p1);
    criar_pessoa(lp, p2);

    pessoa_no *p = buscar_pessoa(lp, 2);
    if (p)
        printf("Pessoa encontrada: %s\n", p->data.nome);

    strcpy(p2.fone, "7777-3333");
    atualizar_pessoa(lp, p2);

    pessoa_salvar_arquivo(lp, "pessoas.txt");

    liberar_pessoa_lista(lp);
}

void testar_tipo_pet() {
    printf("\n===== TESTE TIPO PET =====\n");

    tipo_pet_lista *lt = inicar_tipo_pet_lista();

    tipo_pet_dados t1 = {1, "Cachorro"};
    tipo_pet_dados t2 = {2, "Gato"};

    criar_tipo_pet(lt, t1);
    criar_tipo_pet(lt, t2);

    tipo_pet_no *t = buscar_tipo_pet(lt, 1);
    if (t)
        printf("Tipo encontrado: %s\n", t->data.descricao);

    tipo_pet_salvar_arquivo(lt, "tipos_pet.txt");

    liberar_tipo_pet_lista(lt);
}

void testar_pet() {
    printf("\n===== TESTE PET =====\n");

    /* Criar listas auxiliares */
    pessoa_lista *lp = iniciar_pessoa_lista();
    tipo_pet_lista *lt = inicar_tipo_pet_lista();
    pet_lista *lpet = iniciar_pet_lista();

    /* Criar pessoa e tipo válidos */
    pessoa_dados p = {1, "Carlos", "9999", "Rua X", "10/10/1990"};
    tipo_pet_dados t = {1, "Cachorro"};

    criar_pessoa(lp, p);
    criar_tipo_pet(lt, t);

    /* Criar pet */
    pet_dados pet1 = {1, 1, 1, "Rex"};

    criar_pet(lpet, lp, lt, pet1);

    pet_no *pet = buscar_pet(lpet, 1);
    if (pet)
        printf("Pet encontrado: %s\n", pet->data.nome);

    pet_salvar_arquivo(lpet, "pets.txt");

    liberar_pet_lista(lpet);
    liberar_pessoa_lista(lp);
    liberar_tipo_pet_lista(lt);
}

int main() {
    testar_pessoa();
    testar_tipo_pet();
    testar_pet();

    printf("\nTODOS OS TESTES FINALIZADOS COM SUCESSO.\n");
    return 0;
}
