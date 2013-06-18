/***************************************************/
/**  MAC 0211  -  Laboratório de Programação I    **/
/**  IME-USP   -  Primeiro  Semestre  de    2013  **/
/**  Turma 45  -  Kelly Rosa Bragheto             **/
/**                                               **/
/**  Segundo   Exercício-Programa                 **/
/**  Arquivo:  utils.c                            **/
/**                                               **/
/**  Karina Suemi Awoki                  7572102  **/
/**  Renato Cordeiro Ferreira            7990933  **/
/**  Ruan de Menezes Costa               7990929  **/
/**                                               **/
/**  Em caso de eventuais problemas, acesse:      **/
/**  git@github.com:renatocf/MAC0211-EP2.git      **/
/***************************************************/ 

/* Bibliotecas padrão/internas */
#include <stdlib.h>
#include <stdio.h>
#include "utils.h"

/* Macros */
#define THE_ANSWER 42

void *mallocSafe(size_t n)
{
    void *pt;
    pt = malloc(n);
    if (pt == NULL)
    {
        printf("ERRO na alocacao de memoria.\n\n");
        exit(-1);
    }
    return pt;
}
