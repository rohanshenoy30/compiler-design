%{
    #include <stdio.h>
    #include <stdlib.h>

    int yylex();
    int yyerror(char*);
%}

%token NUMBER ID NL

%%
stmt : exp NL { printf("Valid Expression\n"); exit(0); }
     ;

exp : exp '+' term 
    | term
    ;

term : term '*' factor 
     | factor
     ;

factor : ID 
       | NUMBER
       ;
%%

int yyerror(char* msg)
{
    printf("Invalid expression\n");
    exit(0);
}

void main()
{
    printf("Enter the expression\n");
    yyparse();
}

