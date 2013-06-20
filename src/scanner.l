/***************************************************
 **  MAC 0211  -  Laboratório de Programação I    **
 **  IME-USP   -  Primeiro  Semestre  de    2013  **
 **  Turma 45  -  Kelly Rosa Bragheto             **
 **                                               **
 **  Segundo   Exercício-Programa                 **
 **  Arquivo:  scanner.l                          **
 **                                               **
 **  Karina Suemi Awoki                  7572102  **
 **  Renato Cordeiro Ferreira            7990933  **
 **  Ruan de Menezes Costa               7990929  **
 **                                               **
 **  Em caso de eventuais problemas, acesse:      **
 **  git@github.com:renatocf/MAC0211-EP2.git      **
 ***************************************************/

FLOAT      [0-9]*\.[0-9]+
INT        [0-9]+
FLUXO      ["FLUXO"|"Fluxo"|"fluxo"]
ALTURA     ["ALTURA"|"Altura"|"altura"]
LARGURA    ["LARGURA"|"Largura"|"largura"]
ZONA       ["ZONA"|"Zona"|"zona"]
ILHAS      ["ILHAS"|"Ilhas"|"ilhas"]
SEMENTE    ["SEMENTE"|"Semente"|"semente"]
FREQUENCIA ["FREQUENCIA"|"Frequencia"|"frequencia"]

SEP        :

%{
    /* Header criado pelo parser */
    #include "parser.tab.h"
    
    /* Comentário para exportar o valor 
     * sintático conforme interpretado 
     * pelo Flex */
    extern YYSTYPE yylval;
%}

%%

[ \t]*                      ;

{FLUXO}\s*{SEP}\s*{FLOAT} {
    
    sscanf(yytext, "%f", (float *) &yylval);
    return FLUX;
}

{ALTURA}\s*{SEP}\s*{INT} {
    
    sscanf(yytext, "%d", (int *) &yylval);
    return HEIGHT;
}

{LARGURA}\s*{SEP}\s*{INT} {

    sscanf(yytext, "%d", (int *) &yylval);
    return HEIGHT;
}

{ZONA}\s*{SEP}\s*{INT} {

    sscanf(yytext, "%d", (int *) &yylval);
    return HEIGHT;
}

{ILHAS}\s*{SEP}\s*FLOAT {

    sscanf(yytext, "%f", (float *) &yylval);
    return FLUX;
}

{SEMENTE}\s*{SEP}\s*{INT} {

    sscanf(yytext, "%d", (int *) &yylval);
    return HEIGHT;
}

{FREQUENCIA}\s*{SEP}\s*{INT} {

    sscanf(yytext, "%d", (int *) &yylval);
    return HEIGHT;
}
