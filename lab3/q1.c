#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

bool isArithmetic(char ch) {
    return (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%');
}

bool isRelational(char* str) {
    char* relOps[] = {"==", "!=", ">", "<", ">=", "<="};
    for (int i = 0; i < 6; i++) { 
        if (strcmp(str, relOps[i]) == 0) return true;
    }
    return false; 
}

bool isLogical(char* str) {
    return (strcmp(str, "&&") == 0 || strcmp(str, "||") == 0 || strcmp(str, "!") == 0);
}

bool isKeyword(char* str) {
    char* keywords[] = {"int", "float", "char", "if", "else", "while", "for", "return", "void", "main", "break", "continue"};
    for (int i = 0; i < 12; i++) { 
        if (strcmp(str, keywords[i]) == 0) return true;
    }
    return false; 
}

bool isSpecialSymbol(char ch) {
    return (ch == '(' || ch == ')' || ch == '{' || ch == '}' || ch == '[' || ch == ']' || ch == ';' || ch == ',');
}

bool isNumericConstant(char* str) {
    if (strlen(str) == 0) return false;
    for (int i = 0; i < strlen(str); i++) {
        if (!isdigit(str[i])) return false;
    }
    return true;
}

bool isValidIdentifier(char* str) {
    if (isdigit(str[0]) || isKeyword(str) || strlen(str) == 0) return false;
    for (int i = 0; i < strlen(str); i++) {
        if (!isalnum(str[i]) && str[i] != '_') return false;
    }
    return true;
}

int main() {
    FILE *fp = fopen("digit.c", "r");
    if (!fp) { 
        printf("Cannot open file\n");
        exit(0); 
    }

    char c, buf[32];
    int i;

    c = fgetc(fp); 
    while (c != EOF) {
        if (isspace(c)) {
            c = fgetc(fp);
            continue;
        }

        if (c == '"') {
            i = 0; buf[i++] = c;
            while ((c = fgetc(fp)) != '"' && c != EOF) 
                buf[i++] = c;
            buf[i++] = '"'; buf[i] = '\0';
            printf("String Literal: %s\n", buf);
            c = fgetc(fp); 
        }
        else if (c == '=' || c == '!' || c == '<' || c == '>' || c == '&' || c == '|') {
            char op[3];
            op[0] = c;
            char next = fgetc(fp);
            
            if ((c == '=' && next == '=') || (c == '!' && next == '=') || 
                (c == '<' && next == '=') || (c == '>' && next == '=') || 
                (c == '&' && next == '&') || (c == '|' && next == '|')) {
                op[1] = next; op[2] = '\0';
                if (isRelational(op)) printf("Relational Operator: %s\n", op);
                else printf("Logical Operator: %s\n", op);
                c = fgetc(fp); 
            } else {
                op[1] = '\0';
                if (c == '=') printf("Assignment Operator: %s\n", op);
                else if (c == '!') printf("Logical Operator: %s\n", op);
                else printf("Relational Operator: %s\n", op);
                c = next; 
            }
        }
        else if (isArithmetic(c)) {
            printf("Arithmetic Operator: %c\n", c);
            c = fgetc(fp);
        }
        else if (isSpecialSymbol(c)) {
            printf("Special Symbol: %c\n", c);
            c = fgetc(fp);
        }
        else if (isalnum(c) || c == '_') {
            i = 0; 
            buf[i++] = c;
            while (isalnum(c = fgetc(fp)) || c == '_') {
                buf[i++] = c;
            }
            buf[i] = '\0';

            if (isKeyword(buf)) printf("Keyword: %s\n", buf);
            else if (isNumericConstant(buf)) printf("Numerical Constant: %s\n", buf);
            else if (isValidIdentifier(buf)) printf("Identifier: %s\n", buf);
            
        }
        else {
            c = fgetc(fp); 
        }
    }

    fclose(fp);
    return 0;
}
