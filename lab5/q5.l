%{
#include <stdio.h>

int num;
%}

%%


[0-9]+               { pos_int++; }


%%

int yywrap() {
    return 1;
}

int main() {
    printf("Enter numbers (Press Ctrl+D to stop):\n");
    yylex();
    
    printf("\n--- Results ---\n");
    printf("Positive Integers: %d\n", pos_int);
    printf("Negative Integers: %d\n", neg_int);
    printf("Positive Floating Numbers: %d\n", pos_float);
    printf("Negative Floating Numbers: %d\n", neg_float);
    
    return 0;
}
