#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

typedef struct {
    char token_name[100];
    char type[50];
    int row;
    int col;
} Token;

int curr_row = 1, curr_col = 1;

bool isArithmetic(char ch) { return (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%'); }
bool isSpecialSymbol(char ch) { return (ch == '(' || ch == ')' || ch == '{' || ch == '}' || ch == '[' || ch == ']' || ch == ';' || ch == ','); }

bool isKeyword(char* str) {
    char* keywords[] = {"int", "float", "char", "if", "else", "while", "for", "return", "void", "main", "double", "printf"};
    for (int i = 0; i < 12; i++) { if (strcmp(str, keywords[i]) == 0) return true; }
    return false;
}

int getNextChar(FILE *fp) {
    int c = fgetc(fp);
    if (c == '\n') { curr_row++; curr_col = 1; } 
    else { curr_col++; }
    return c;
}

int peekNextChar(FILE *fp) {
    int c = fgetc(fp);
    ungetc(c, fp);
    return c;
}

Token getNextToken(FILE *fp) {
    Token t;
    int c;
    t.type[0] = '\0';

    while ((c = getNextChar(fp)) != EOF) {
        if (isspace(c)) continue;

        if (c == '"') {
            int i = 0;
            t.token_name[i++] = '"';
            while ((c = getNextChar(fp)) != '"' && c != EOF) {
                t.token_name[i++] = c;
            }
            t.token_name[i++] = '"';
            t.token_name[i] = '\0';
            
            t.row = curr_row; t.col = curr_col - i;
            strcpy(t.type, "String Literal");
            
            return t;
        }


        if (isalnum(c) || c == '_') {
            char buffer[100]; int i = 0; buffer[i++] = c;
            while (isalnum(peekNextChar(fp)) || peekNextChar(fp) == '_') buffer[i++] = getNextChar(fp);
            buffer[i] = '\0';
            strcpy(t.token_name, isKeyword(buffer) ? buffer : (isdigit(buffer[0]) ? "num" : "id"));
            t.row = curr_row; t.col = curr_col - i;
            strcpy(t.type, "Valid");
            return t;
        }
        
        if (isArithmetic(c) || isSpecialSymbol(c)) {
            sprintf(t.token_name, "%c", c);
            t.row = curr_row; t.col = curr_col - 1;
            strcpy(t.type, "Valid");
            return t;
        }
    }
    strcpy(t.type, "EOF");
    return t;
}

int main() {
    FILE *fp = fopen("digit.c", "r");
    if (!fp) return 0;

    printf("%-25s %-10s %-10s\n", "Token Name", "Row", "Col");
    while (true) {
        Token t = getNextToken(fp);
        if (strcmp(t.type, "EOF") == 0) break;
        printf("%-25s %-10d %-10d\n", t.token_name, t.row, t.col);
    }


    fclose(fp);
    return 0;
}
