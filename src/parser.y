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

%{
    /* Bibliotecas */
    #include <stdio.h>
    #include "options.h"
    
    /* Variável para opções */
    Options scanner;
    
    /* Protótipos */
    extern int yylex();
    int yyerror(const char *str);
    Options yygetopt();
%}

%token B_FLUX      
%token B_HEIGHT    
%token B_LENGTH    
%token B_ZONE      
%token B_ISLAND    
%token B_SEED      
%token B_FREQ      
%token B_SPEEDY    

%%

input: /* empty */
       | input line
;

line: '\n'
      | exp '\n'
;

exp: B_FLUX     { scanner.F = $1; }
     | B_HEIGHT { scanner.H = $1; }   
     | B_LENGTH { scanner.L = $1; }   
     | B_ZONE   { scanner.Z = $1; }   
     | B_ISLAND { scanner.i = $1; }   
     | B_SEED   { scanner.s = $1; }   
     | B_FREQ   { scanner.f = $1; }   
;

%%

/* Função correspondente à tratamento de erro */
int yyerror(const char *s) 
{
    fprintf(stderr, "%s\n", s);
    return -1;
}

Options yygetopt()
{
    return scanner;
}