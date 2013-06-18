/***************************************************/
/**  MAC 0211  -  Laboratório de Programação I    **/
/**  IME-USP   -  Primeiro  Semestre  de    2013  **/
/**  Turma 45  -  Kelly Rosa Bragheto             **/
/**                                               **/
/**  Segundo   Exercício-Programa                 **/
/**  Arquivo:  list.c                             **/
/**                                               **/
/**  Karina Suemi Awoki                  7572102  **/
/**  Renato Cordeiro Ferreira            7990933  **/
/**  Ruan de Menezes Costa               7990929  **/
/**                                               **/
/**  Em caso de eventuais problemas, acesse:      **/
/**  git@github.com:renatocf/MAC0211-EP2.git      **/
/***************************************************/ 

/*
////////////////////////////////////////////////////////////////////////
-----------------------------------------------------------------------
                              BIBLIOTECAS
-----------------------------------------------------------------------
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
*/

/* Bibliotecas padrão */
#include <stdlib.h>
#include <stdio.h>

/* Bibliotecas internas */
#include "utils.h"
#include "list-internal.h"

/*
////////////////////////////////////////////////////////////////////////
-----------------------------------------------------------------------
                                 FUNÇÕES    
-----------------------------------------------------------------------
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
*/

/* 'Getters' para estruturas da lista */
Link  list_head (List list) { return list->head; }
Link  list_next (Link node) { return node->next; }
Link  list_prev (Link node) { return node->prev; }
LItem list_item (Link node) { return node->item; }

List list_init(int N)
{
    List new = (List) mallocSafe(sizeof(*new));
    new->head = (Link) mallocSafe(sizeof(*new->head));
    new->head->next = new->head;
    new->head->prev = new->head;
    new->head->item = NULL;
    return new;
}

Link list_new_node(LItem item)
{
    Link new = (Link) mallocSafe(sizeof(*new));
    new->item = item;
    new->next = NULL;
    new->prev = NULL;
    return new;
}

void list_free(List list)
{
    Link dead;
    while(list != NULL && !list_empty(list))
    {
        dead = list_remove(list, list->head->next);
        free(dead->item);
        free(dead);
    }

    if(list != NULL) free(list_head(list));
    if(list != NULL) free(list);
}

int list_empty(List list)
{
    if(list == NULL) return 0;
    if(list->head->next == list->head) return 1;
    return 0;
}

Link list_remove(List list, Link node)
{
    Link aux;
    aux = node->prev;
    aux->next = node->next;
    if(node->next != NULL) node->next->prev = aux;
    node->next = NULL;
    node->prev = NULL;
    return node;
}

void list_insert(List list, Link new)
{
    new->next = list->head->next;
    new->prev = list->head;
    new->next->prev = new;
    new->prev->next = new;
}

void list_select(List list, int direction, void (*visit) (LItem))
{
    Link aux;
    if(direction == HEAD)
        for(aux = list->head->next; aux != list->head; aux = aux->next)
            visit(aux->item);
    else
        for(aux = list->head->prev; aux != list->head; aux = aux->prev)
            visit(aux->item);
}
