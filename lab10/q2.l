%{
    #include "q2.tab.h"
%}

%%
[0-9]+                                  { return NUMBER;    }
"if"                                    { return IF;        }
"else"                                  { return ELSE;      }
\n                                      { return NL;        }
[a-zA-Z_][a-zA-Z0-9_]*                  { return ID;        }
[<>]=?|!=|==                            { return REL;       }
.                                       { return yytext[0]; }
%%

int yywrap()
{
    return 1;
}

