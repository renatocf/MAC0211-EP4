/***************************************************/
/**  MAC 0211  -  Laboratório de Programação I    **/
/**  IME-USP   -  Primeiro  Semestre  de    2013  **/
/**  Turma 45  -  Kelly Rosa Bragheto             **/
/**                                               **/
/**  Segundo   Exercício-Programa                 **/
/**  Arquivo:  river-internal.h                   **/
/**                                               **/
/**  Karina Suemi Awoki                  7572102  **/
/**  Renato Cordeiro Ferreira            7990933  **/
/**  Ruan de Menezes Costa               7990929  **/
/**                                               **/
/**  Em caso de eventuais problemas, acesse:      **/
/**  git@github.com:renatocf/MAC0211-EP2.git      **/
/***************************************************/

#ifndef H_RIVER_INTERNAL_DEFINED
#define H_RIVER_INTERNAL_DEFINED

#include "river.h"

typedef struct conf Conf;
struct conf
{
    float flux;
    int   height;
    int   length;
    int   zone;
    float prob_island;
    int   freq_island;
};

List river;
TStrip base;
Conf Config;

/* Função auxiliar para imprimir
 * a faixa de terra */
void save_base   (TStrip);
void strip_print (TStrip);

#endif
