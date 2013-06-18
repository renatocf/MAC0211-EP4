/***************************************************/
/**  MAC 0211  -  Laboratório de Programação I    **/
/**  IME-USP   -  Primeiro  Semestre  de    2013  **/
/**  Turma 45  -  Kelly Rosa Bragheto             **/
/**                                               **/
/**  Segundo   Exercício-Programa                 **/
/**  Arquivo:  test.c                             **/
/**                                               **/
/**  Karina Suemi Awoki                  7572102  **/
/**  Renato Cordeiro Ferreira            7990933  **/
/**  Ruan de Menezes Costa               7990929  **/
/**                                               **/
/**  Em caso de eventuais problemas, acesse:      **/
/**  git@github.com:renatocf/MAC0211-EP2.git      **/
/***************************************************/ 

/*
////////////////////////////////////////////////////////////////////////
-----------------------------------------------------------------------
                              BIBLIOTECAS
-----------------------------------------------------------------------
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
*/

/* Bibliotecas padrão */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Nossa biblioteca para o teste */
#include "test.h"

/* Bibliotecas incluídas para o teste */
#include "list.h"
#include "strip.h"
#include "terrain.h"
#include "river-internal.h"

/*
////////////////////////////////////////////////////////////////////////
-----------------------------------------------------------------------
                          MACROS/VARIÁVEIS GLOBAIS
-----------------------------------------------------------------------
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
*/

/* Macro para tipo de teste simples
 * (teste menos verboso, com análise final apenas) */
#define SIMPLE 1

/* Variáveis globais multiuso */
static int   maxr;          /* Limite máximo direito            */
static int   maxl;          /* Limite máximo esquerdo           */
static int   n_lines = 0;   /* Número total de linhas           */
static int   test_mode = 0; /* Modo de teste (simples/completo) */
static char  last_char;     /* Último caracter checado          */
static float maxr_mean = 0; /* Média dos limites direitos       */
static float maxl_mean = 0; /* Média dos limites esquerdos      */
static FILE *OUT;           /* Stream para imprimir relatório   */

/*
////////////////////////////////////////////////////////////////////////
-----------------------------------------------------------------------
                                PROTÓTIPOS         
-----------------------------------------------------------------------
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
*/
static void print_lines   (TStrip strip);
static void analyse_lines (TStrip strip);
static void analyse_river (int seed, FILE *OUT);

/*
////////////////////////////////////////////////////////////////////////
-----------------------------------------------------------------------
                             FUNÇÔES DE TESTE          
-----------------------------------------------------------------------
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
*/
void analyse_program(int seed, int iterations, int mode, char output[SIZE_NAME])
{
    /** VARIÁVEIS *****************************************************/
        int is_stdout = 0;     /* Flag para indicar impressão stdout */
        int i = 0;             /* Contador para número de iterações  */
        char ans;              /* Resposta para as perguntas         */
        char out[SIZE_NAME+1]; /* Nome corrigido (com \0)            */

        test_mode = mode;      /* Modo de teste simples/completo     */
    
    /** ABRE STREAM ***************************************************/
        if(strncmp((const char *) output, STDOUT, SIZE_NAME) != 0) 
        {
            for(i = 0; i < SIZE_NAME && output[i] != '\0'; i++)
                out[i] = output[i];
            for(; i <= SIZE_NAME; i++)
                out[i] = '\0';
            OUT = fopen(out, "w");
        }
        else { OUT = stdout; is_stdout = 1; }
    
    /** TESTE 1: OPÇÕES DO JOGADOR ************************************/
        /* Zerando todas nossas variáveis para o teste */
        n_lines = maxr_mean = maxl_mean = 0;
        
        fprintf(stdout, "\nTeste nº 1: Opções do jogador:\n");
        fprintf(stdout, "Deseja prosseguir com o teste? ");  
        scanf(" "); scanf("%c", &ans);
        if(ans == 's' || ans == 'y' || ans == 'S' || ans == 'Y')
        {
            river_animation_generate(seed);
            for(i = 0; i < iterations-1; i++)
                { system("clear||cls"); river_animation_iterate(); }
            fprintf(OUT, "\n"); analyse_river(seed, OUT);
            river_animation_finish();
        }
    
    /** TESTE 2: ROBUSTEZ *********************************************/
        /* Zerando todas nossas variáveis para o teste */
        n_lines = maxr_mean = maxl_mean = 0;
        
        fprintf(stdout, "\nTeste nº 2: Teste de Robustez:\n");
        fprintf(stdout, "Deseja prosseguir com o teste? ");
        scanf(" "); scanf("%c", &ans);
        if(ans == 's' || ans == 'y' || ans == 'S' || ans == 'Y')
        {
            river_config_size    (4, 1); /* Rio 4x1 */
            river_config_margins (1);    /* Pequena zona de conforto  */
            printf("%d", Config.height);
            
            river_animation_generate(seed);
            for(i = 0; i < iterations-1; i++)
                { system("clear||cls"); river_animation_iterate(); }
            fprintf(OUT, "\n"); analyse_river(seed, OUT);
            river_animation_finish();
        }
    
    /** FECHA STREAM **************************************************/
        if(!is_stdout) fclose(OUT);
}

void analyse_river(int seed, FILE *OUT)
{
    fprintf(OUT, "\n\n\n\n\n");
    fprintf(OUT, "Análise da estrutura do Jogo:\n");
    fprintf(OUT, "-------------------------------------------------------\n");
    fprintf(OUT, "\n");
    fprintf(OUT, "Último frame:\n");
    fprintf(OUT, "\n");
    list_select(river, HEAD, print_lines);
    fprintf(OUT, "\n");
    fprintf(OUT, "Configurações do rio\n");
    fprintf(OUT, "\n");
    fprintf(OUT, "* Fluxo: %.3f\n", Config.flux);
    fprintf(OUT, "* Altura da tela: %d\n", Config.height);
    fprintf(OUT, "* Largura da tela: %d\n", Config.length);
    fprintf(OUT, "* 'Zona de conforto': %d\n", Config.zone);
    fprintf(OUT, "* Probabilidade de gerar ilha: %.3f\n", Config.prob_island);
    fprintf(OUT, "* Distância mínima para geração de ilhas: %d\n", Config.freq_island);
    fprintf(OUT, "\n");
    
    n_lines = 0;
    list_select(river, HEAD, analyse_lines);
    
    if(test_mode != SIMPLE) fprintf(OUT, "\n\n");
    fprintf(OUT, "Média do limite da margem esquerda: %.2f\n",
           1.0 * maxl_mean/Config.height);
    fprintf(OUT, "Média do limite da margem direita:  %.2f\n",
            1.0 * maxr_mean/Config.height);
}

static void print_lines(TStrip strip)
{
    int i = 0;      /* Contador */
    for(i = 0; i < Config.length; i++)
        fprintf(OUT, "%c", strip[i].t);
    fprintf(OUT, "\n");
}

static void analyse_lines(TStrip strip)
{
    int i = 0;      /* Contador */
    int print;      /* Imprimir ou não */
    float flux = 0; /* Velocidade média */

    int n_water = 0; /* Pixels com água */
    int n_lands = 0; /* Pixels com terra */

    int lmargin = 0; /* Por causa das ilhas,
                        precisamos garantir que
                        a primeira margem esquerda
                        econtrada seja contabilizada */
    
    n_lines++; /* Total de linhas analisadas */
    last_char = strip[0].t; /* Primeiro terreno */
    
    /* Checa se devemos ou não imprimir mensagens de erro 
     * (conforme modo de testes simples ou completo) */
    (test_mode == SIMPLE) ? (print = 0) : (print = 1);
    
    /* Analisa/imprime a n_lines-ésima linha */
    if(print) fprintf(OUT, "\nLinha %d:\n", n_lines);
    for(i = 0; i < Config.length; i++)
    {
        if(print) fprintf(OUT, "%c", strip[i].t);
        if(strip[i].t == WATER)
        {
            if(last_char == LAND && !lmargin)
                { maxl = i; maxl_mean += i; lmargin = 1; }
            n_water++;
        }
        else
        {
            if(last_char == WATER)
                { maxr = i; maxr_mean += i; }
            n_lands++;
        }
        last_char = strip[i].t;
        flux += strip[i].v;
    }
    if(print) fprintf(OUT, "\n");

    /* Relatório sobre a linha: */
    if(print) fprintf(OUT, "Quantidade de água  (%c): %d\n", WATER, n_water);
    if(print) fprintf(OUT, "Quantidade de terra (%c): %d\n", LAND, n_lands);
    if(print) fprintf(OUT, "Posição da margem esquerda: %d\n", maxl);
    if(print) fprintf(OUT, "Posição da margem direita:  %d\n", maxr);
    if(print) fprintf(OUT, "Fluxo: %.3f\n", flux);
}
