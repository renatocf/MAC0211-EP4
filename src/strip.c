/***************************************************/
/**  MAC 0211  -  Laboratório de Programação I    **/
/**  IME-USP   -  Primeiro  Semestre  de    2013  **/
/**  Turma 45  -  Kelly Rosa Bragheto             **/
/**                                               **/
/**  Segundo   Exercício-Programa                 **/
/**  Arquivo:  strip.c                            **/
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

/* Bibliotecas-padrão */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* Bibliotecas internas */
#include "strip.h"
#include "utils.h"
#include "terrain.h"
#include "statistical.h"

/*
////////////////////////////////////////////////////////////////////////
-----------------------------------------------------------------------
                           VARIÁVEIS GLOBAIS
-----------------------------------------------------------------------
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
*/

int j = 0;         /* Nº de frames desde última ilha */
int freq_island;   /* Intervalo de surgimento de ilhas */
float prob_island; /* Probabilidade de haver uma ilha na linha */


/*////////////////////////////////////////////////////////////////////////
-----------------------------------------------------------------------
                                 FUNÇÕES
-----------------------------------------------------------------------
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/* Insere semente da biblioteca estatística */
void tstrip_seed  (int seed)
{ stat_set_seed(seed); }

/* Insere informações referentes à ilha */
void tstrip_island(float prob, int freq)
{ freq_island = freq; prob_island = prob; }

/* Libera uma faixa de terra */
void tstrip_free(TStrip strip)
{ free(strip); }

/* Gera uma nova faixa de terra */
TStrip tstrip_generate
(int size, int zone, float normalization, TStrip base, TStrip nova)
{
    /** VARIÁVEIS *****************************************************/
        int i;         /* Contador auxiliar para propósitos gerais   */
        int maxl;      /* Limite máximo à esquerda para o rio        */
        int maxr;      /* Limite máximo à direita para o rio         */
        int lmargin;   /* Posição da margem esquerda do rio          */
        int rmargin;   /* Posição da margem direita do rio           */
        float K = 0;   /* Constante de normalização                  */
        float sum = 0; /* Somatório das velocidades                  */


        /* Se não tivermos memória alocada, criamos */
        if(nova == NULL) nova = (TStrip) mallocSafe(size * sizeof(*nova));

        /* Escolhe em que ponto estará a 'zona de conforto' */
        maxl = stat_gen_uniform_int(1, size-zone-1);
        maxr = maxl + zone;

    /** GERADOR DE FAIXAS DE TERRA ************************************/
        if(base == NO_BASE) /* 1º caso: não há base */
        {

            /* Sorteia limites das marges esquerda/direita */
            lmargin = stat_gen_uniform_int(0, maxl);
            rmargin = stat_gen_uniform_int(maxr, size-1);

            /* Gera terra na margem esquerda com velocidade 0.*/
            for(i = 0; i <= lmargin; i++)
            {
                nova[i].v = 0;
                nova[i].t = LAND;
            }

            /* Gera terra na margem direita com velocidade 0.*/
            for(i = size-1; i >= rmargin ; i--)
            {
                nova[i].v = 0;
                nova[i].t = LAND;
            }
            /* Gera água entre os limites de ambas as margens (exceto
             * nas casas laterais, em que v = 0). Vai somando os valores
             * (Ω) para posterior normalização. */

            for(i = lmargin+1; i < rmargin; i++)
            {
                nova[i].t = WATER;
                if(i == lmargin+1 || i == rmargin-1)nova[i].v = 0;
                else
                {
                    nova[i].v = stat_gen_uniform_float(0, PI);
                    sum += nova[i].v;
                }
            }

            /* Cria constante de normalização K = Φ/Ω para manter o
             * fluxo desejado constante. */
            K = normalization/sum;
            for(i = 0; i < size; i++) nova[i].v *= K;
        }
        else /* 2º caso: Temos uma linha base */
        {
            int tam_island = 0, pos_island = 0;

            /* Limpa a linha */
            for(i = 0; i < size; i++) nova[i].t = ' ';

            /* Busca pela margem esquerda */
            for(i = 0; i < size; i++)
                if(base[i].t != base[i+1].t) break;
            lmargin = i;
            /* Busca pela margem direita */
            for(i = size - 1; i >= 0; i--)
                if(base[i].t != base[i-1].t) break;
            rmargin = i;

            /* Margem esquerda nos limites da tela */
            if(maxl == 0);
            else if(lmargin < maxl && lmargin > 0)
                lmargin += stat_gen_uniform_int(-1, 1);
            else if(lmargin==0)
                lmargin += stat_gen_uniform_int(0, 1);
            else
                lmargin += stat_gen_uniform_int(-1, 0);

            /* Margem direita nos limites da tela */
            if(size-1 == maxr);
            else if(rmargin>maxr && rmargin < size-1)
                rmargin += stat_gen_uniform_int(-1, 1);
            else if(rmargin == size - 1)
                rmargin += stat_gen_uniform_int(-1, 0);
            else
                rmargin += stat_gen_uniform_int(0, 1);

            /* A ilha vem aqui */
            if(j != freq_island) j++;
            else
            {
                j = 0; /* Zeramos 'j' para recomeçar a contar a distância entre as ilhas */
                if(stat_gen_uniform_float(0,1) < prob_island) /* Gerando uma ilha: */
                {
                    j = 0;
                    tam_island = stat_gen_uniform_int(1, (rmargin - lmargin)/2.0);
                    pos_island = stat_gen_uniform_int(lmargin+1, rmargin - tam_island -1);

                    for(i = pos_island-1; i <= pos_island+tam_island+1; i++)
                    {
                        nova[i].v = 0;
                        if(i == pos_island-1 || i == tam_island+pos_island+1)nova[i].t = WATER;
                        else nova[i].t = LAND;
                    }

                }
            } /* else */

            /* Preenchendo a linha atual: */
            for(i = 0; i < size; i++)
            {
                /* 1º caso: margens */
                if(i <= lmargin + 1 || i >= rmargin - 1)
                {
                    nova[i].v = 0;
                    if(i == lmargin+1 || i == rmargin-1)nova[i].t = WATER;
                    else nova[i].t = LAND;
                }
                /* 2º caso: terra que se tornou água */
                else if(base[i].t == LAND && nova[i].t != LAND)
                {
                    nova[i].v = stat_gen_uniform_float(0,10*PI/4.0);
                    sum += nova[i].v;
                    nova[i].t = WATER;
                }
                /* 3º caso: já era água (e continuará a ser) */
                else if(nova[i].t != LAND)
                {
                    nova[i].v = base[i].v + (stat_gen_uniform_float(0,1)/2) * base[i].v;
                    sum += nova[i].v;
                    nova[i].t = WATER;
                }
            }

            /* Normalizando velocidades para manter o fluxo: */
            (sum != 0) ? (K = normalization/sum) : (K = 0);
            for(i = 0; i < size; i++) nova[i].v *= K;

        } /* 2º caso: Temos uma linha base */

    return nova;

} /* tstrip_generate */
