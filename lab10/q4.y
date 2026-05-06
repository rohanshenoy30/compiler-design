%{
    #include <stdio.h>
    #include <stdlib.h>

    int yylex();
    int yyerror(char*);

    int Pow(int a, int b)
    {
        int e = 1;
        for(int i = 0; i < b; e *= a, i++);

        return e;
    }
%}

%token NUMBER NL SPACE

%%
input : input line 
      | /*empty*/
      ;

line : NL
     | exp NL   { printf("Result: %d\n", $1); }
     ;

exp : NUMBER                    { $$ = $1;          }
    | exp SPACE exp SPACE  '+'  { $$ = $1 + $3;     }
    | exp SPACE exp SPACE  '-'  { $$ = $1 - $3;     }
    | exp SPACE exp SPACE  '*'  { $$ = $1 * $3;     }
    | exp SPACE exp SPACE  '/'  { $$ = $1 / $3;     }
    | exp SPACE exp SPACE  '^'  { $$ = Pow($1, $3); }
    | exp 'n'
    ;
%%

int yyerror(char* msg)
{
    printf("Invalid expression\n");
    exit(0);
}

void main()
{
    printf("Enter postfix expressions:\n");
    yyparse();
}

