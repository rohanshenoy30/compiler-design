%{
    #include "q4.tab.h"
%}

%%
[0-9]+  { yylval = atoi(yytext);   return NUMBER;   }
[ \t]+  { return SPACE;                             }
\n      { return NL;                                }
.       { return yytext[0];                         }
%%

int yywrap()
{
    return 1;
}

