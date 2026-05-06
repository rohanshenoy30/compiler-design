%{
    #include <stdio.h>
    #include <stdlib.h>

    int yylex();
    int yyerror(char*);
    extern char* yytext;
%}

%token IF ELSE ID NUMBER REL NL
%right '='

%%
start: ifstmt NL { printf("Valid statement.\n"); exit(0); };

stmt : ID '=' val ';'
     | ifstmt
     ;

ifstmt : IF '(' exp ')' '{' stmtlist '}' elsestmt
       ;

elsestmt : ELSE '{' stmtlist '}'
         | /*empty*/
         ;

stmtlist: stmt stmtlist
        | /*empty*/
        ;

exp : val REL val
    ;

val : ID 
    | NUMBER
    ;
%%

int yyerror(char* msg)
{
    printf("Invalid statement (%s, %s).\n", msg, yytext);
}

void main()
{
    printf("Enter a decision statment:\n");
    yyparse();
}

