/***************************************************/
/**  MAC 0211  -  Laboratório de Programação I    **/
/**  IME-USP   -  Primeiro  Semestre  de    2013  **/
/**  Turma 45  -  Kelly Rosa Bragheto             **/
/**                                               **/
/**  Segundo   Exercício-Programa                 **/
/**  Arquivo:  getopt.c                           **/
/**                                               **/
/**  Karina Suemi Awoki                  7572102  **/
/**  Renato Cordeiro Ferreira            7990933  **/
/**  Ruan de Menezes Costa               7990929  **/
/**                                               **/
/**  Em caso de eventuais problemas, acesse:      **/
/**  git@github.com:renatocf/MAC0211-EP2.git      **/
/***************************************************/ 

#include <string.h>
#include "getopt.h"

int optind = 1;
/* Variável global, inicializada inicialmente com 0, que mos-
 * tra a primeira opção de 'argv' a partir da qual começam 
 * os elementos que são argumentos simples. */

static void move_to_init(char **argv, int initial_pos);

char getopt(int argc, char **argv, char *optstring)
{
    /** VARIÁVEIS *****************************************************/
        int i, j;          /* Contadores auxiliares                  */
        char option = '?'; /* Caracter da opção da linha de comando. */
        
        int arg_position = NONE; 
        /* Se existir, troca-se o padrão (NONE) pela posição do argu-
         * mento na linha de comando */
        
        optarg = NULL;
        /* Variável global, inicializada inicialmente com NULL, que
         * aponta para a string com o argumento da opção pesquisada
         * na chamada da função. */
        
        optopt = ' ';
        /* Variável global, inicializada inicialmente com ' ' (espa-
         * ço em branco) para devolver alguma opção desconhecida. */
    
    /** PROCESSAMENTO *************************************************/
        /* Retorna ':' caso a optstring esteja sem o argumento */
        if(optstring[0] == ':') return ':';
        
        /* Analisa o argv buscando por opções iniciadas por '-' */
        for(i = optind; i < argc; i++)
        {
            if(argv[i][0] == '-')
            {
                if(argv[i][1] == '-' && argv[i+1][0] == '-') 
                {
                    i++; continue;
                    /* Caso o elemento verificado seja '--', 
                     * ignora a opção apresentada em seguida */
                }
                
                for(j = 0; optstring[j] != '\0'; j++) 
                    /* Percorre 'optstring' - string com opções
                     * conhecidas pelo cliente. */
                {
                    if(argv[i][1] != optstring[j]) continue;
                    /* Opção desconhecida: avança para o próximo 
                     * item de optstring. */
                    
                    option = optstring[j];
                    
                    /* Caso de opção com argumentos (letra seguida 
                     * de : em optstring) */
                    if(optstring[j+1] == ':')
                    {
                        j++; /* Não precisamos analisar ':' */
                        if(argv[i][2] != '\0') optarg = &argv[i][2];
                            /* 1º caso: opção tem algum
                             * complemento colado em seu
                             * nome: esta é a opção de-
                             * sejada. */
                        else if(argv[i+1] == NULL) 
                        {
                            if(optstring[j+1] == ':') j++; 
                            else { optopt = option; option = '?'; }
                            /* 2º caso: opção está no fi-
                             * nal de argv: seu argumento
                             * requerido não existe. Se o 
                             * argumento não for obrigató-
                             * rio, seguimos. Caso contrá-
                             * rio, retorna '?' e salva a
                             * opção em 'optopt'. */
                        }
                        else if(argv[i+1][0] == '-') 
                        {
                            if(optstring[j+1] == ':') j++; 
                            else { optopt = option; option = '?'; }
                            /* 3º caso: o elemento seguinte
                             * no argv é outra opção. Se o
                             * argumento não for obrigató-
                             * rio, seguimos. Caso contrá-
                             * rio, retorna '?' e salva a 
                             * opção em 'optopt'. */
                        }
                        else
                        {
                            /* Último caso: o elemento de argv
                             * seguinte é o argumento da op-
                             * ção que analisamos. */
                            optarg = argv[i+1];
                            arg_position = i+1;
                        }
                    }
                    else if(argv[i][2] != '\0') option = '?';
                    /* Se a opção não requiser argumentos, mas
                     * tiver um acoplado nela, devolve '?'. */
                }
                
                /* Troca a posição da opção encontrada nesta 
                 * chamada de função: Todas as opções são en-
                 * viadas até 'nextopt' para manter a ordem
                 * das opções encontradas. */
                move_to_init(argv, i);
                
                /* Caso o argumento esteja em outro dos ele-
                 * mentos do vetor 'argv', faz a troca de 
                 * posições para ele ficar em frente à sua
                 * opção correspondente. */
                if(arg_position != NONE)
                    move_to_init(argv, arg_position);
                
                if(option == '?') { return '?'; }
                /* Opção desconhecida */
                
                return option;
                
            } /* if(argv[i][0] == '-') */
            
        } /* for(i = optind; i < argc; i++) */
    
    return NONE;
}

static void move_to_init(char **argv, int initial_pos)
{
    int k; /* Contador */
    char *aux = argv[initial_pos];
    /* Ponteiro auxiliar para movimentar os 
     * elementos do vetor. */
    
    for(k = initial_pos; k > optind; k--)
    {
        argv[k] = argv[k-1];
    }
    argv[optind] = aux; 
    optind++;
}
