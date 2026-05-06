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

// ---------- C Keywords ----------
char *keywords[] = {
    "int", "float", "char", "if", "else", "while",
    "for", "return", "void", "main", "double"
};
int kwCount = 11;

// ---------- Operators ----------
char *operators[] = {
    "+", "-", "*", "/", "%", "=", "==", "!=", 
    ">", "<", ">=", "<=", "&&", "||", "!"
};
int opCount = 15;

// ---------- Special Symbols ----------
int isSpecialSymbol(char ch) {
    return (ch=='('||ch==')'||ch=='{'||ch=='}'||
            ch=='['||ch==']'||ch==';'||ch==',');
}

// ---------- Utility Functions ----------
int isKeyword(char *str) {
    for (int i = 0; i < kwCount; i++)
        if (strcmp(str, keywords[i]) == 0)
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

// ---------- Lexical Analyzer ----------
void tokenize(FILE *fp) {
    char ch;
    char buffer[200];

    while ((ch = fgetc(fp)) != EOF) {
        col++;

        // ---------- Newline ----------
        if (ch == '\n') {
            row++;
            col = 0;
            continue;
        }

        // ---------- Skip spaces ----------
        if (isspace(ch))
            continue;

        // ---------- Preprocessor directives ----------
        if (ch == '#') {
            while ((ch = fgetc(fp)) != '\n' && ch != EOF);
            row++;
            col = 0;
            continue;
        }

        // ---------- Comments ----------
        if (ch == '/') {
            int next = fgetc(fp);

            // Single line comment
            if (next == '/') {
                while ((ch = fgetc(fp)) != '\n' && ch != EOF);
                row++;
                col = 0;
                continue;
            }

            // Multi-line comment
            else if (next == '*') {
                int prev = 0;
                while ((ch = fgetc(fp)) != EOF) {
                    if (ch == '\n') {
                        row++;
                        col = 0;
                    }
                    if (prev == '*' && ch == '/')
                        break;
                    prev = ch;
                }
                continue;
            }
            else {
                ungetc(next, fp);
            }
        }

        int startCol = col;

        // ---------- String Literal ----------
        if (ch == '"') {
            int i = 0;
            buffer[i++] = ch;

            while ((ch = fgetc(fp)) != '"' && ch != EOF) {
                buffer[i++] = ch;
                if (ch == '\n') {
                    row++;
                    col = 0;
                } else {
                    col++;
                }
            }

            buffer[i++] = '"';
            buffer[i] = '\0';

            printToken(buffer, row, startCol);
            continue;
        }

        // ---------- Identifier / Keyword / Function ----------
        if (isalpha(ch) || ch == '_') {
            int i = 0;
            buffer[i++] = ch;

            while (isalnum(ch = fgetc(fp)) || ch == '_') {
                buffer[i++] = ch;
                col++;
            }
            buffer[i] = '\0';
            ungetc(ch, fp);

            // Check for function (look ahead for '(')
            int next = fgetc(fp);
            if (next == '(') {
                printToken(buffer, row, startCol);

                char arg[200] = "";
                int j = 0;
                col++;

                while ((ch = fgetc(fp)) != ')' && ch != EOF) {
                    arg[j++] = ch;
                    col++;
                }
                arg[j] = '\0';
                col++;

                addSymbol(buffer, "FUNC", arg); //isFunction(buffer) remove everything above and add a strcpy in the while loop
            }
            else {
                ungetc(next, fp);

                printToken(buffer, row, startCol); //identifier

                if (isKeyword(buffer))
                    continue;
                else
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

        // ---------- Operators (check 2-char first) ----------
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
            else if (isSpecialSymbol(ch)) {
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
    FILE *fp = fopen("digit.c", "r");
    if (!fp) {
        printf("File not found\n");
        return 0;
    }

    tokenize(fp);
    fclose(fp);
    printSymbolTable();
    return 0;
}
