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

%{
    /* Bibliotecas */
    #include <stdio.h>
    #include "options.h"
    
    /* Variável para opções */
    Options scanner 
        = {0, 0, 0, 0, 0, 0, 0, 0, {0}, 0, 0, 0};
    
    /* Protótipos */
    extern int yylex();
    extern int yyparse();
    int yyerror(const char *str);
    Options yygetopt(Options *base);
%}

%error-verbose

%union {
    float f_value;
    char c_value;
    int i_value;
};

%token <c_value> EOL     
%token <f_value> B_FLUX  
%token <i_value> B_HEIGHT
%token <i_value> B_LENGTH
%token <i_value> B_ZONE  
%token <f_value> B_ISLAND
%token <i_value> B_SEED  
%token <i_value> B_FREQ  

%%

input: /* empty */
       | input line
;

line: EOL | exp EOL | error EOL
;

exp: B_FLUX     { scanner.F = $1; printf("NO BISON: %f\n",$1); }
     | B_HEIGHT { scanner.H = $1; printf("NO BISON: %i\n",$1); }   
     | B_LENGTH { scanner.L = $1; printf("NO BISON: %i\n",$1); }   
     | B_ZONE   { scanner.Z = $1; printf("NO BISON: %i\n",$1); }   
     | B_ISLAND { scanner.i = $1; printf("NO BISON: %f\n",$1); }   
     | B_SEED   { scanner.s = $1; printf("NO BISON: %i\n",$1); }   
     | B_FREQ   { scanner.f = $1; printf("NO BISON: %i\n",$1); }   
;

%%

/* Função correspondente à tratamento de erro */
int yyerror(const char *s) 
{
    fprintf(stderr, "options.conf: %s\n", s);
    return -1;
}

Options yygetopt(Options *base)
{
    if(scanner.F != 0) base->F = scanner.F; /* Fluxo */
    if(scanner.H != 0) base->H = scanner.H; /* Altura */
    if(scanner.L != 0) base->L = scanner.L; /* Largura */
    if(scanner.Z != 0) base->Z = scanner.Z; /* Zona de segurança */
    if(scanner.i != 0) base->i = scanner.i; /* Ilhas */
    if(scanner.s != 0) base->s = scanner.s; /* Semente */
    if(scanner.f != 0) base->f = scanner.f; /* Frequência */
    return *base;
}
