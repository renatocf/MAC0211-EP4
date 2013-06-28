/***************************************************
 **  MAC 0211  -  Laboratório de Programação I    **
 **  IME-USP   -  Primeiro  Semestre  de    2013  **
 **  Turma 45  -  Kelly Rosa Bragheto             **
 **                                               **
 **  Quarto    Exercício-Programa                 **
 **  Arquivo:  scanner.l                          **
 **                                               **
 **  Karina Suemi Awoki                  7572102  **
 **  Renato Cordeiro Ferreira            7990933  **
 **  Ruan de Menezes Costa               7990929  **
 **                                               **
 **  Em caso de eventuais problemas, acesse:      **
 **  git@github.com:renatocf/MAC0211-EP4.git      **
 ***************************************************/

FLOAT      [0-9]*\.[0-9]+
INT        [0-9]+
FLUXO      ("FLUXO"|"Fluxo"|"fluxo")
ALTURA     ("ALTURA"|"Altura"|"altura")
LARGURA    ("LARGURA"|"Largura"|"largura")
ZONA       ("ZONA"|"Zona"|"zona")
ILHAS      ("ILHAS"|"Ilhas"|"ilhas")
SEMENTE    ("SEMENTE"|"Semente"|"semente")
FREQUENCIA ("FREQUENCIA"|"Frequencia"|"frequencia")

SEP        :

%{
    /* Bibliotecas-padrão */
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    
    /* Header criado pelo parser */
    #include "parser.tab.h"
    
    /* Comentário para exportar o valor 
     * sintático conforme interpretado 
     * pelo Flex */
    extern YYSTYPE yylval;
    
    /* Função do scanner disponível 
     * para ser usada pelo usuário */
    extern int yylex();
    
    /* Exporta o yyin para poder ler de
     * um arquivo. Se o yyin não estiver
     * definido, o padrão é o stdin */
    extern FILE *yyin;
    
    /* Auxiliar para percorrer yytext e 
     * encontrar posição do 1º número */
    static char *aux;
%}

/* Para ficar livre dos 'warnings': input, yyunput e fileno. */
/* A opção correspondente a 'fileno' evitada com 'never-interactive'
 * é útil, mas ela desabilita que o flex leia entradas interativas */
%option noinput
%option nounput
%option never-interactive

%%

[\n<<EOF>>]               { return EOL; }

[ \t]*                    ;

{FLUXO}\s*{SEP}\s*{FLOAT} {
    
    for(aux = yytext; *aux <= '0' || *aux >= '9'; aux++);
    yylval.f_value = strtod(aux, NULL);
    return B_FLUX;
}

{ALTURA}\s*{SEP}\s*{INT} {
    
    for(aux = yytext; *aux <= '0' || *aux >= '9'; aux++);
    yylval.i_value = strtol(aux, NULL, 10);
    return B_HEIGHT;
}

{LARGURA}\s*{SEP}\s*{INT} {

    for(aux = yytext; *aux <= '0' || *aux >= '9'; aux++);
    yylval.i_value = strtol(aux, NULL, 10);
    return B_LENGTH;
}

{ZONA}\s*{SEP}\s*{INT} {

    for(aux = yytext; *aux <= '0' || *aux >= '9'; aux++);
    yylval.i_value = strtol(aux, NULL, 10);
    return B_ZONE;
}

{ILHAS}\s*{SEP}\s*FLOAT {

    for(aux = yytext; *aux <= '0' || *aux >= '9'; aux++);
    yylval.f_value = strtod(aux, NULL);
    return B_ISLAND;
}

{SEMENTE}\s*{SEP}\s*{INT} {

    for(aux = yytext; *aux <= '0' || *aux >= '9'; aux++);
    yylval.i_value = strtol(aux, NULL, 10);
    return B_SEED;
}

{FREQUENCIA}\s*{SEP}\s*{INT} {

    for(aux = yytext; *aux <= '0' || *aux >= '9'; aux++);
    yylval.i_value = strtol(aux, NULL, 10);
    return B_FREQ;
}

%%
