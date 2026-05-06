%{
    #include <stdio.h>
    #include <stdlib.h>

    int yylex();
    int yyerror(char*);
%}

%token TYPE ID NUMBER

%%
stmt : TYPE declist ';' { printf("Valid declaration\n"); exit(0); }
     ;

declist : dec ',' declist
        | dec

dec : vardec
    | arrdec
    ;

vardec : ID '=' val
       | ID

arrdec : ID'['NUMBER']' '=' '{'vallist'}'
       | ID'['NUMBER']'

vallist : val ',' vallist
        | val

val : ID
    | NUMBER
    ;
%%

int yyerror(char* msg)
{
    printf("Invalid declaration\n", msg);
    exit(0);
}

void main()
{
    printf("Enter a declaration\n");
    yyparse();
}

