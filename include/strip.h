/***************************************************/
/**  MAC 0211  -  Laboratório de Programação I    **/
/**  IME-USP   -  Primeiro  Semestre  de    2013  **/
/**  Turma 45  -  Kelly Rosa Bragheto             **/
/**                                               **/
/**  Segundo   Exercício-Programa                 **/
/**  Arquivo:  strip.h                            **/
/**                                               **/
/**  Karina Suemi Awoki                  7572102  **/
/**  Renato Cordeiro Ferreira            7990933  **/
/**  Ruan de Menezes Costa               7990929  **/
/**                                               **/
/**  Em caso de eventuais problemas, acesse:      **/
/**  git@github.com:renatocf/MAC0211-EP2.git      **/
/***************************************************/ 

#ifndef H_STRIP_DEFINED
#define H_STRIP_DEFINED

#include "terrain.h"

/* Critério utilizado para declarar que o
 * terreno não será baseado em nenhum outro */
#define NO_BASE     NULL

/* Faixa de terreno: vetor de terrenos */
typedef terrain *TStrip;

/* Funções para manipular faixas de terreno */
void   tstrip_seed     (int);
void   tstrip_free     (TStrip);
void   tstrip_print    (TStrip);
void   tstrip_island   (float prob, int freq);
TStrip tstrip_generate (int size, int zone,
                        float normalization, TStrip base, TStrip nova);

#endif
