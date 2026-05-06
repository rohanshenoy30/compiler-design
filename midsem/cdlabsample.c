#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX 200

// ---------- Symbol Table ----------
typedef struct {
    char lexeme[100];
    char type[20];
    char argument[200];
} Symbol;

Symbol symtab[MAX];
int symCount = 0;

// ---------- Row & Column ----------
int row = 1, col = 0;

// ---------- Keywords ----------
char *keywords[] = {"let", "function", "document"};
int kwCount = 3;

// ---------- Predefined Functions ----------
char *functions[] = {
    "ready", "on", "val", "split", "map",
    "filter", "text", "join", "isNaN"
};
int funcCount = 9;

// ---------- Operators ----------
char *operators[] = {
    "$", "=", "+", "-", "*", "/", "%", "&&", "!", "?", ":", "=="
};
int opCount = 12;

// ---------- Utility Functions ----------
int isKeyword(char *str) {
    for (int i = 0; i < kwCount; i++)
        if (strcmp(str, keywords[i]) == 0)
            return 1;
    return 0;
}

int isFunction(char *str) {
    for (int i = 0; i < funcCount; i++)
        if (strcmp(str, functions[i]) == 0)
            return 1;
    return 0;
}

int isOperator(char *str) {
    for (int i = 0; i < opCount; i++)
        if (strcmp(str, operators[i]) == 0)
            return 1;
    return 0;
}

void addSymbol(char *lex, char *type, char *arg) {
    for (int i = 0; i < symCount; i++)
        //check if its already there, then skip
        if (strcmp(symtab[i].lexeme, lex) == 0)
            return;

    strcpy(symtab[symCount].lexeme, lex);
    strcpy(symtab[symCount].type, type);
    strcpy(symtab[symCount].argument, arg);
    symCount++;
}

void printToken(char *name, int r, int c) {
    printf("<%s, %d, %d>\n", name, r, c);
}

// ---------- Main Lexical Analyzer ----------
void tokenize(FILE *fp) {
    char ch;
    char buffer[200];

    while ((ch = fgetc(fp)) != EOF) {
        col++;

        if (ch == '\n') {
            row++;
            col = 0;
            continue;
        }

        // Skip spaces
        if (isspace(ch))
            continue;

        int startCol = col;

        // ---------- String / #Identifier ----------
        if (ch == '"' || ch == '\'') {
            char quote = ch;
            int i = 0;
            buffer[i++] = ch;

            while ((ch = fgetc(fp)) != quote && ch != EOF) {
                buffer[i++] = ch;
                col++;
            }
            buffer[i++] = quote;
            buffer[i] = '\0';

            // Check case: "#identifier"
            if (buffer[1] == '#') {
                char id[100];
                int j = 0;
                for (int k = 2; k < i - 1; k++)
                    id[j++] = buffer[k];
                id[j] = '\0';

                printToken(id, row, startCol);
                addSymbol(id, "Identifier", "-");
            }
            else {
                printToken(buffer, row, startCol);
            }
            continue;
        }

        // ---------- Identifier / Keyword / Function ----------
        if (isalpha(ch) || ch == '$' || ch == '#') {
            int i = 0;
            buffer[i++] = ch;

            while (isalnum(ch = fgetc(fp)) || ch == '_' || ch == '#') {
                buffer[i++] = ch;
                col++;
            }
            buffer[i] = '\0';
            ungetc(ch, fp);

            if (isKeyword(buffer)) {
                printToken(buffer, row, startCol);
                addSymbol(buffer, "Keyword", "-");
            }
            else if (isFunction(buffer)) {
                printToken(buffer, row, startCol);

                // Capture argument inside ()
                char arg[200] = "";
                int j = 0;

                ch = fgetc(fp);
                if (ch == '(') {
                    col++;
                    while ((ch = fgetc(fp)) != ')' && ch != EOF) {
                        arg[j++] = ch;
                        col++;
                    }
                    arg[j] = '\0';
                    col++;
                }
                addSymbol(buffer, "FUNC", arg);
            }
            else {
                printToken(buffer, row, startCol);
                addSymbol(buffer, "Identifier", "-");
            }
            continue;
        }

        // ---------- Numbers ----------
        if (isdigit(ch)) {
            int i = 0;
            buffer[i++] = ch;

            while (isdigit(ch = fgetc(fp))) {
                buffer[i++] = ch;
                col++;
            }
            buffer[i] = '\0';
            ungetc(ch, fp);

            printToken(buffer, row, startCol);
            continue;
        }

        // ---------- Operators (2-char first) ----------
        char op[3];
        op[0] = ch;
        op[1] = fgetc(fp);
        op[2] = '\0';

        if (isOperator(op)) {
            printToken(op, row, startCol);
            col++;
        }
        else {
            ungetc(op[1], fp);
            op[1] = '\0';

            if (isOperator(op))
                printToken(op, row, startCol);
            else {
                // ---------- Special Symbol ----------
                char s[2] = {ch, '\0'};
                printToken(s, row, startCol);
            }
        }
    }
}

// ---------- Print Symbol Table ----------
void printSymbolTable() {
    printf("\nSymbol Table:\n");
    printf("%-15s %-12s %-20s\n", "Lexeme", "Type", "Argument");

    for (int i = 0; i < symCount; i++) {
        printf("%-15s %-12s %-20s\n",
               symtab[i].lexeme,
               symtab[i].type,
               symtab[i].argument);
    }
}

// ---------- Main ----------
int main() {
    FILE *fp = fopen("input.js", "r");
    if (!fp) {
        printf("File not found\n");
        return 0;
    }

    tokenize(fp);
    fclose(fp);

    printSymbolTable();
    return 0;
}
