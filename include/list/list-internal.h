/***************************************************/
/**  MAC 0211  -  Laboratório de Programação I    **/
/**  IME-USP   -  Primeiro  Semestre  de    2013  **/
/**  Turma 45  -  Kelly Rosa Bragheto             **/
/**                                               **/
/**  Segundo   Exercício-Programa                 **/
/**  Arquivo:  list-internal.h                    **/
/**                                               **/
/**  Karina Suemi Awoki                  7572102  **/
/**  Renato Cordeiro Ferreira            7990933  **/
/**  Ruan de Menezes Costa               7990929  **/
/**                                               **/
/**  Em caso de eventuais problemas, acesse:      **/
/**  git@github.com:renatocf/MAC0211-EP2.git      **/
/***************************************************/ 

#ifndef H_LIST_INTERNAL_DEFINED
#define H_LIST_INTERNAL_DEFINED

#include "list.h"

/* Estrutura para lista:
 * Ponteiro para a cabeça da lista */
struct list {
    Link head;
};

/* Estrutura para nódulo da lista:
 * Campo de estrutura + ponteiro para o próximo */
struct lnode {
    LItem item;
    Link  next;
    Link  prev;
};

#endif
