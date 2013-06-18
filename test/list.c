#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "utils.h"
#include "terrain.h"

void imprime(LItem item)
{
    printf("tipo: %c velocidade: %0.2f\n", item->t, item->v);
}

void lista_vazia(List lista)
{
    printf("Lista vazia? ");
    if(list_empty(lista)) printf("Sim\n");
    else printf("Não\n");
}

int main()
{
    List lista;
    terrain *terra = mallocSafe(sizeof(*terra));
    terrain *agua  = mallocSafe(sizeof(*agua));
    terra->t = '#'; terra->v = 0;   /* terra */
    agua->t  = '.'; agua->v  = 0.2; /* água  */
    
    printf("\n------------------\n");
    printf("Checando inicialização:\n");
    lista = list_init(2);
    lista_vazia(lista);
    
    printf("\n------------------\n");
    printf("Inserindo terra:\n");
    lista_vazia(lista);
    printf("Processando...\n");
    list_insert(lista, terra);
    list_select(lista, HEAD, imprime);
    lista_vazia(lista);

    printf("\n------------------\n");
    printf("Removendo terra:\n");
    lista_vazia(lista);
    printf("Processando...\n");
    list_remove(lista, list_next(list_head(lista)));
    list_select(lista, HEAD, imprime);
    lista_vazia(lista);

    printf("\n------------------\n");
    printf("Adicionando 2 elementos:\n");
    lista_vazia(lista);
    printf("Processando...\n");
    list_insert(lista, terra);
    list_insert(lista, agua);
    list_select(lista, HEAD, imprime);
    lista_vazia(lista);
    
    printf("\n------------------\n");
    printf("Removendo 2 elementos:\n");
    lista_vazia(lista);
    printf("Processando...\n");
    list_remove(lista, list_next(list_head(lista)));
    list_remove(lista, list_next(list_head(lista)));
    list_select(lista, HEAD, imprime);
    lista_vazia(lista);
    
    printf("\n------------------\n");
    printf("Liberando lista:\n");
    lista_vazia(lista);
    printf("Reinserindo...\n");
    list_insert(lista, terra);
    list_insert(lista, agua);
    lista_vazia(lista);
    list_select(lista, HEAD, imprime);
    printf("Processando...\n");
    list_free(lista);
    
    /* Liberando da função: */
    free(agua); free(terra);

    return 0;
}
