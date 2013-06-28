/***************************************************/
/**  MAC 0211  -  Laboratório de Programação I    **/
/**  IME-USP   -  Primeiro  Semestre  de    2013  **/
/**  Turma 45  -  Kelly Rosa Bragheto             **/
/**                                               **/
/**  Quarto    Exercício-Programa                 **/
/**  Arquivo:  river-internal.h                   **/
/**                                               **/
/**  Karina Suemi Awoki                  7572102  **/
/**  Renato Cordeiro Ferreira            7990933  **/
/**  Ruan de Menezes Costa               7990929  **/
/**                                               **/
/**  Em caso de eventuais problemas, acesse:      **/
/**  git@github.com:renatocf/MAC0211-EP4.git      **/
/***************************************************/

#ifndef H_RIVER_INTERNAL_DEFINED
#define H_RIVER_INTERNAL_DEFINED

/* Biblioteca para o cliente */
#include "river.h"
#include "player.h"

/* Estrutura de configurações */
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

/* Variáveis para gerar o rio */
List river; TStrip base;

/* Jogador e configuração padrão */
Conf Config;
Player P1;

/* Posições para o barco */
int boat_vpos, boat_hpos;

/* Auxiliar para impressão do rio */
int frame_height;

/* Função auxiliar para imprimir
 * a faixa de terra */
void strip_print (TStrip);

TStrip strip1;
int scale;

#endif
