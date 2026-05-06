%{
    #include <stdio.h>
    #include <stdlib.h>

    int yylex();
    int yyerror(char*);
%}

%token ID NUMBER OP NL

%%
start: expr NL { printf("Valid expression\n"); exit(0); }
     ;

expr : expr OP val
     | val
     ;

val : ID 
    | NUMBER
    ;
%%

int yyerror(char* msg)
{
    printf("Invalid expression: %s\n", msg);
    exit(0);
}

void main()
{
    printf("Enter an expression: ");
    yyparse();
}

