/***************************************************/
/**  MAC 0211  -  Laboratório de Programação I    **/
/**  IME-USP   -  Primeiro  Semestre  de    2013  **/
/**  Turma 45  -  Kelly Rosa Bragheto             **/
/**                                               **/
/**  Quarto    Exercício-Programa                 **/
/**  Arquivo:  options.h                          **/
/**                                               **/
/**  Karina Suemi Awoki                  7572102  **/
/**  Renato Cordeiro Ferreira            7990933  **/
/**  Ruan de Menezes Costa               7990929  **/
/**                                               **/
/**  Em caso de eventuais problemas, acesse:      **/
/**  git@github.com:renatocf/MAC0211-EP4.git      **/
/***************************************************/

#ifndef H_OPTIONS_DEFINED
#define H_OPTIONS_DEFINED

/*
////////////////////////////////////////////////////////////////////////
-----------------------------------------------------------------------
                                 MACROS
-----------------------------------------------------------------------
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
*/

#include "test.h"

/* Padrões para a gerar o rio */
#define FLUX       73.54
#define HEIGHT     130
#define LENGTH     150
#define ITERATIONS 10
#define ZONE       10
#define ISLAND     0.4
#define SEED       13
#define FREQ       5
#define SPEEDY     10.0

/* Intervalo de tempo da animação */
#define INTERVAL   1.4*10e-10

/*
////////////////////////////////////////////////////////////////////////
-----------------------------------------------------------------------
                          ESTRUTURAS/MENSAGENS
-----------------------------------------------------------------------
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
*/

/* Opções da linha de comando */
typedef struct options
{
    /* Opções de configuração */
    float F;  /* Fluxo */
    int H;    /* Altura do rio */
    int L;    /* Largura do rio */
    int N;    /* Número de iterações */
    int Z;    /* Distancia de segurança entre as margens */
    float i;  /* Probabilidade de gerar ilha */
    int s;    /* Semente */
    int f;    /* Frequência com que as ilhas aparecem
                 (em número de linhas) */

    /* Nome da saída do relatório */
    char o[SIZE_NAME];

    /* Opções booleanas */
    int t;   /* Modo teste simples */
    int T;   /* Modo teste completo */
    int h;   /* Ajuda */
} Options;

#endif
