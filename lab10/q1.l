%{
    #include "q1.tab.h"
%}

%%
[0-9]+                  { return NUMBER;    }
"int "                   { return TYPE;      }
[a-zA-Z_][a-zA-Z0-9_]*  { return ID;        }
.                       { return yytext[0]; }
%%

int yywrap()
{
    return 1;
}

