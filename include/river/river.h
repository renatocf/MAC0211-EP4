/***************************************************/
/**  MAC 0211  -  Laboratório de Programação I    **/
/**  IME-USP   -  Primeiro  Semestre  de    2013  **/
/**  Turma 45  -  Kelly Rosa Bragheto             **/
/**                                               **/
/**  Quarto    Exercício-Programa                 **/
/**  Arquivo:  river.h                            **/
/**                                               **/
/**  Karina Suemi Awoki                  7572102  **/
/**  Renato Cordeiro Ferreira            7990933  **/
/**  Ruan de Menezes Costa               7990929  **/
/**                                               **/
/**  Em caso de eventuais problemas, acesse:      **/
/**  git@github.com:renatocf/MAC0211-EP4.git      **/
/***************************************************/

#ifndef H_RIVER_DEFINED
#define H_RIVER_DEFINED

/* Funções para configuração de parâmetros */
void river_config_flux        (float);
void river_config_size        (int, int);   /* largura/altura */
void river_config_island      (float, int); /* probabilidade/frequência da ilha */
void river_config_margins     (int);        /* zona de conforto */

/* Funções para controle de animação */
void river_animation_init     (void);
void river_animation_generate (int);        /* semente */
int  river_animation_iterate  (void);
void river_animation_finish   (void);
void movement(int key);

#define GAME_OVER 42

#endif
