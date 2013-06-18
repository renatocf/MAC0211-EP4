/***************************************************/
/**  MAC 0211  -  Laboratório de Programação I    **/
/**  IME-USP   -  Primeiro  Semestre  de    2013  **/
/**  Turma 45  -  Kelly Rosa Bragheto             **/
/**                                               **/
/**  Segundo   Exercício-Programa                 **/
/**  Arquivo:  terrain.h                          **/
/**                                               **/
/**  Karina Suemi Awoki                  7572102  **/
/**  Renato Cordeiro Ferreira            7990933  **/
/**  Ruan de Menezes Costa               7990929  **/
/**                                               **/
/**  Em caso de eventuais problemas, acesse:      **/
/**  git@github.com:renatocf/MAC0211-EP2.git      **/
/***************************************************/ 

#ifndef H_TERRAIN_DEFINED
#define H_TERRAIN_DEFINED

#define LAND  '#'
#define WATER '.'

/* Tipo para terreno: struct com
 * campos de VELOCIDADE (v) e TIPO
 * DE TERRENO (t) */
struct _terrain
{
    float v;
    char  t;
};
typedef struct _terrain terrain;

#endif
