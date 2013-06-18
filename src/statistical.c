/***************************************************/
/**  MAC 0211  -  Laboratório de Programação I    **/
/**  IME-USP   -  Primeiro  Semestre  de    2013  **/
/**  Turma 45  -  Kelly Rosa Bragheto             **/
/**                                               **/
/**  Segundo   Exercício-Programa                 **/
/**  Arquivo:  statistical.c                      **/
/**                                               **/
/**  Karina Suemi Awoki                  7572102  **/
/**  Renato Cordeiro Ferreira            7990933  **/
/**  Ruan de Menezes Costa               7990929  **/
/**                                               **/
/**  Em caso de eventuais problemas, acesse:      **/
/**  git@github.com:renatocf/MAC0211-EP2.git      **/
/***************************************************/ 

/* Bibliotecas-padrão */
#include <stdlib.h>
#include <math.h>

/* Bibliotecas internas */
#include "statistical.h"

/* Semente do gerador de números aleatórios */
void stat_set_seed(int seed)
{
    srand(seed);
}

/* Gera float entre dois inteiros */
float stat_gen_uniform_float(float min, float max)
{
    float d;
    d = (float) rand() / ((float) RAND_MAX + 1);
    return (min + d * (max - min));
}

/* Gera inteiro entre dois inteiros */
int stat_gen_uniform_int(int min, int max)
{
    int k;
    double d;
    d = (double) rand() / ((double) RAND_MAX + 1);
    k = (int) (d * (max - min + 1));
    return (min + k);
}
