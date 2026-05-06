%{
    #include "q0.tab.h"
%}

%%
[0-9]+                  { return NUMBER;    }
\n                      { return NL;        }
[a-zA-Z_][a-zA-Z0-9_]*  { return ID;        }
.                       { return yytext[0]; }
%%

int yywrap()
{
    return 1;
}
