/***************************************************/
/**  MAC 0211  -  Laboratório de Programação I    **/
/**  IME-USP   -  Primeiro  Semestre  de    2013  **/
/**  Turma 45  -  Kelly Rosa Bragheto             **/
/**                                               **/
/**  Segundo   Exercício-Programa                 **/
/**  Arquivo:  list.h                             **/
/**                                               **/
/**  Karina Suemi Awoki                  7572102  **/
/**  Renato Cordeiro Ferreira            7990933  **/
/**  Ruan de Menezes Costa               7990929  **/
/**                                               **/
/**  Em caso de eventuais problemas, acesse:      **/
/**  git@github.com:renatocf/MAC0211-EP2.git      **/
/***************************************************/ 

#ifndef H_LIST_DEFINED
#define H_LIST_DEFINED

#include "list-item.h"

#define HEAD 1
#define TAIL 0

/* Tipos para lista e nódulo */
typedef struct list *List;
typedef struct lnode *Link;

/* Funções para uso do cliente */
List  list_init     (int);
Link  list_head     (List);
Link  list_next     (Link);
Link  list_prev     (Link);
LItem list_item     (Link);
void  list_free     (List);
int   list_empty    (List);
Link  list_remove   (List, Link);
void  list_insert   (List, Link);
void  list_select   (List, int direction, void (*visit) (LItem));
Link  list_new_node (LItem);

#endif
