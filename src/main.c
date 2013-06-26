/***************************************************/
/**  MAC 0211  -  Laboratório de Programação I    **/
/**  IME-USP   -  Primeiro  Semestre  de    2013  **/
/**  Turma 45  -  Kelly Rosa Bragheto             **/
/**                                               **/
/**  Quarto    Exercício-Programa                 **/
/**  Arquivo:  main.c                             **/
/**                                               **/
/**  Karina Suemi Awoki                  7572102  **/
/**  Renato Cordeiro Ferreira            7990933  **/
/**  Ruan de Menezes Costa               7990929  **/
/**                                               **/
/**  Em caso de eventuais problemas, acesse:      **/
/**  git@github.com:renatocf/MAC0211-EP4.git      **/
/***************************************************/

/*
////////////////////////////////////////////////////////////////////////
-----------------------------------------------------------------------
                              BIBLIOTECAS
-----------------------------------------------------------------------
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
*/

/* Bibliotecas-padrão */
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

/* Bibliotecas internas */
#include "test.h"
#include "utils.h"
#include "river.h"
#include "strip.h"
#include "getopt.h"
#include "allegro.h"
#include "options.h"
/* #include "scanner.yy.h" */
#include "parser.tab.h"

extern FILE *yyin;
extern int yyparse();
extern Options yygetopt();

/*
////////////////////////////////////////////////////////////////////////
-----------------------------------------------------------------------
                                 AJUDA
-----------------------------------------------------------------------
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
*/

/* Mensagens */
char help[] = "Jogo das canoas!\n"
              "\n"
              "Opções:\n"
              "-F: Fluxo de cada linha\n"
              "-H: Altura da janela\n"
              "-L: Largura da janela\n"
              "-Z: Distancia mínima entre as margens\n"
              "-i: Probabilidade de haver ilha em determinada linha\n"
              "-s: Seed para a geração de números pseudo-aleatórios\n"
              "-f: Distancia mínima entre as ilhas\n"
              "-t: Modo de testes simples\n"
              "-T: Modo de testes completo\n"
              "-h: Ajuda\n"
              "\n"
              "Para mais informações, veja a documentação.\n";

/*
////////////////////////////////////////////////////////////////////////
-----------------------------------------------------------------------
                               PROTÓTIPOS
-----------------------------------------------------------------------
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
*/

static int  receive_arguments (int argc, char **argv, Options *args);
static int  menu              (Options *args);
static void configurations    (Options *args);

/*
////////////////////////////////////////////////////////////////////////
-----------------------------------------------------------------------
                                  MAIN
-----------------------------------------------------------------------
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
*/
int main(int argc, char **argv)
{
    /** VARIÁVEIS *****************************************************/
        int test_mode = 0; /* Auxiliar para por o programa em teste  */
        int func_err;      /* Faz a captura do resultado das saídas  */
        int exit = 0;      /* Auxiliar para definir quando sair      */
        clock_t init;      /* Início do relógio (para animação)      */
        clock_t end;       /* Fim do relógio (para animação)         */

        /* Struct com argumentos da linha de comando */
        Options args = { FLUX, HEIGHT, LENGTH, ITERATIONS, ZONE,
                         ISLAND, SEED, FREQ, STDOUT, 0, 0, 0};

    /** ARGUMENTOS/MENU/CONFIGURAÇÕES *********************************/
        func_err = receive_arguments(argc, argv, &args);
        if(func_err) return EXIT_FAILURE;

        if(args.h == 1)
        {
            printf("\n%s\n", help);
            return EXIT_SUCCESS;
        }
        
        func_err = system("find options.conf &> /dev/null; return $?");
        if(!func_err)
        {
            fprintf(stderr, "ENTROU!\n");
            /* Arquivo de configuração: options.conf
             * Guarda o stream no padrão yyin do scanner */
            yyin = fopen("options.conf", "r");

            fprintf(stderr, "PARSEANDO!\n");
            /* Chama o parser para ler opções */
            func_err = yyparse(); 
            if(func_err)
            {
                fprintf(stderr, "Problemas no arquivo de configuração!\n");
                return EXIT_FAILURE;
            }
            
            /* Atribui opções nos argumentos */
            args = yygetopt();
            
            fprintf(stderr, "FECHANDO!\n");
            /* Fecha o arquivo configurado para leitura */
            fclose(yyin);
        }
        else printf("options.conf não identificado\n");

        /* Modo de teste: */
        test_mode = args.t + args.T;

        /* Chamada para o nosso menu do usuário:
         * Quando ele devolve 'falha', o usuário quer sair do jogo */
        if(!test_mode) func_err = menu(&args);
        if(func_err == EXIT_FAILURE) return EXIT_SUCCESS;

    /** CONFIGURAÇÕES DO RIO ******************************************/
        river_config_flux    (args.F);
        river_config_size    (args.L, args.H);
        river_config_island  (args.i, args.f);
        river_config_margins (args.Z);

    /** ANIMAÇÃO DO RIO ***********************************************/
        test_mode = args.t + args.T;

        river_animation_init();
        if(test_mode) analyse_program(args.s, args.N, test_mode, args.o);
        else
        {
            river_animation_generate(args.s);
            while(!exit)
            {
                end = init = clock();
                while(end-init < INTERVAL)
                {



                    end = clock();
                }
                exit = river_animation_iterate();




                /*******************************************************************************************/


        /**************************************************************************************************/





            }
        }

    /** LIBERAÇÃO DE MEMÓRIA ******************************************/
        river_animation_finish();

    return EXIT_SUCCESS;
}


/*
////////////////////////////////////////////////////////////////////////
-----------------------------------------------------------------------
                                 FUNÇÕES
-----------------------------------------------------------------------
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
*/
static int receive_arguments(int argc, char **argv, Options *args)
/* Recebe os argumentos da linha de comando e os
 * armazena na struct correspondente */
{
    char opt; int i;
    while((opt = getopt(argc, argv, "F:H:L:N:Z:i:s:f:o:tTh")) != NONE)
    {
        switch(opt)
        {
        case 'F':
            args->F = atof(optarg);
            break;
        case 'H':
            args->H = atoi(optarg);
            break;
        case 'L':
            args->L = atoi(optarg);
            break;
        case 'N':
            args->N = atoi(optarg);
            break;
        case 'Z':
            args->Z = atoi(optarg);
            break;
        case 'i':
            args->i = atof(optarg);
            break;
        case 's':
            args->s = atoi(optarg);
            break;
        case 'f':
            args->f = atoi(optarg);
            break;
        case 'o':
            for(i = 0; i < SIZE_NAME && optarg[i] != '\0'; i++)
                args->o[i] = optarg[i];
            for(; i < SIZE_NAME; i++)
                args->o[i] = '\0';
            break;
        case 't':
            args->t = 1;
            break;
        case 'T':
            args->T = 2;
            break;
        case 'h':
            args->h = 1;
            break;
        case '?':
            printf("Argumento -%c desconhecido\n", optopt);
            return EXIT_FAILURE;
        }
    } /* while */
    return EXIT_SUCCESS;
}

static int menu(Options *args)
{
    int option;
    system("clear||cls"); /* Limpa a tela */

    while(1)
    {
        /* Nossa ASCII Art: */
        printf("........................   .........................................\n");
        printf(":::::::::::::::::::::::::.   '::::::::::::::::::::::::::::::::::::::\n");
        printf(";;;;;;;;;;;;;;;;;;;;;;;;;;;.      ::;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;\n");
        printf("+++++++++++++++++++++++++++++        +++++++++++++++++++++++++++++++\n");
        printf("+++++++++++++++++++++++++++++++         ++++++++++++++++++++++++++++\n");
        printf("++++++++++++++++++++++++++++++++           +++++++++++++++++++++++++\n");
        printf("================================             =======================\n");
        printf("===============================                =====================\n");
        printf("oooooooooooooooooooooooooooooo                   ooooooooooooooooooo\n");
        printf("oooooooooooooooooooooooooooo                      oooooooooooooooooo\n");
        printf("$$$$$$$$$$$$$$$$$$$$$$$$$    ,   .                 $$$$$$$$$$$$$$$$$\n");
        printf("$$$$$$$$$$$$$$$$$$$$$$$    ,´,'¨¨¨`.¨¨'            $$$$$$$$$$$$$$$$$\n");
        printf("$$$$$$$$$$$$$$$$$$$$     ,´Y|______Y`.|            $$$$$$$$$$$$$$$$$\n");
        printf("##################    _,´,|______,´   `.          ##################\n");
        printf("###############      | ,´      ,´   ,`  |¨|      ###################\n");
        printf("#############        ,´______,´   ,´    |_|    #####################\n");
        printf("@@@@@@@@@@           \\       /  ,´           @@@@@@@@@@@@@@@@@@@@@@@\n");
        printf("@@@@@@@               \\     / ,´           @@@@@@@@@@@@@@@@@@@@@@@@@\n");
        printf("@@@@                   `---'´            @@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
        printf("                                                                    \n");
        printf("--------------------------------------------------------------------\n");
        printf("                        JOGO DAS CANOAS!                            \n");
        printf("--------------------------------------------------------------------\n");
        printf("                                                                    \n");
        printf(" 1) Jogar                                                           \n");
        printf(" 2) Configurar jogo                                                 \n");
        printf(" 3) Modo teste (simples)                                            \n");
        printf(" 4) Modo teste (completo)                                           \n");
        printf(" 5) Sair                                                            \n");
        printf("                                                                    \n");

        /* Seleciona opção */
        while(1)
        {
            printf(" Selecione sua opção: ");
            scanf(" %d", &option); while(getchar() != '\n');

            /* JOGAR */
            if(option == 1)
            { return EXIT_SUCCESS; }

            /* CONFIGURAÇÕES */
            else if(option == 2)
            { configurations(args); break; }

            /* MODO TESTE (SIMPLES) */
            else if(option == 3)
            { args->t = 1;  return EXIT_SUCCESS; }

            /* MODO TESTE (COMPLETO) */
            else if(option == 4)
            { args->T = 2; return EXIT_SUCCESS; }

            /* SAIR */
            else if(option == 5)
            { return EXIT_FAILURE; }

            /* ERRO */
            else
            { printf(" Opção não reconhecida!\n"); }

        } /* while das opções */
    } /* while do menu */
}

static void configurations(Options *args)
{
    char ans;
    printf(" Insira as configurações:\n");

    printf(" Deseja configurar o fluxo do rio? "); scanf(" %c", &ans);
    if(ans == 's' || ans == 'y' || ans == 'S' || ans == 'Y')
    { printf(" Fluxo do rio: "); scanf("%f", &args->F); }

    printf(" Deseja configurar a altura do rio? "); scanf(" %c", &ans);
    if(ans == 's' || ans == 'y' || ans == 'S' || ans == 'Y')
    { printf(" Altura do rio: "); scanf("%d", &args->H); }

    printf(" Deseja configurar a largura do rio? "); scanf(" %c", &ans);
    if(ans == 's' || ans == 'y' || ans == 'S' || ans == 'Y')
    { printf(" Largura do rio: "); scanf("%d", &args->L); }

    printf(" Deseja configurar a distância mínima entre as margens? "); scanf(" %c", &ans);
    if(ans == 's' || ans == 'y' || ans == 'S' || ans == 'Y')
    { printf(" Distância mínima das margens: "); scanf("%d", &args->Z); }

    printf(" Deseja configurar a distância mínima entre ilhas? "); scanf(" %c", &ans);
    if(ans == 's' || ans == 'y' || ans == 'S' || ans == 'Y')
    { printf(" Distancia mínima entre ilhas: "); scanf("%d", &args->f); }

    printf(" Deseja configurar a probabilidade de gerar ilhas? "); scanf(" %c", &ans);
    if(ans == 's' || ans == 'y' || ans == 'S' || ans == 'Y')
    { printf(" Probabilidade de gerar ilhas: "); scanf("%f", &args->i); }
}
