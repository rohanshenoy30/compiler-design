#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define TABLE_SIZE 100

/* ---------------- TOKEN STRUCT ---------------- */
struct Token {
    int row;
    int col;
    char name[100];
};

/* ---------------- SYMBOL TABLE STRUCT ---------------- */
struct SymbolEntry {
    char name[100];
    char category[20];     // function / identifier
    char argument[200];    // only for functions
    struct SymbolEntry *next;
};

struct SymbolEntry *symbolTable[TABLE_SIZE] = {NULL};

FILE *fp;
int row = 1, col = 0;

/* ---------------- C KEYWORDS ---------------- */
char *keywords[] = {
    "if","else","for","while",
    "return","int","float","char","double","void"
};
int keywordCount = sizeof(keywords)/sizeof(keywords[0]);

/* ---------------- CHECK KEYWORD ---------------- */
int isKeyword(char *str) {
    for (int i = 0; i < keywordCount; i++)
        if (!strcmp(str, keywords[i]))
            return 1;
    return 0;
}

/* ---------------- HASH FUNCTION ---------------- */
unsigned int hash(char *str) {
    unsigned int h = 5381;
    int c;
    while ((c = *str++))
        h = ((h << 5) + h) + c;
    return h % TABLE_SIZE;
}

/* ---------------- SEARCH ---------------- */
struct SymbolEntry* search(char *name) {
    unsigned int index = hash(name);
    struct SymbolEntry *curr = symbolTable[index];
    while (curr) {
        if (!strcmp(curr->name, name))
            return curr;
        curr = curr->next;
    }
    return NULL;
}

/* ---------------- INSERT ---------------- */
void insert(char *name, char *category, char *arg) {

    if (search(name)) return;

    unsigned int index = hash(name);
    struct SymbolEntry *e = malloc(sizeof(struct SymbolEntry));

    strcpy(e->name, name);
    strcpy(e->category, category);

    if (arg)
        strcpy(e->argument, arg);
    else
        strcpy(e->argument, "");

    e->next = symbolTable[index];
    symbolTable[index] = e;
}

/* ---------------- GET CHAR ---------------- */
int getChar() {
    int c = fgetc(fp);

    if (c == '\n') {
        row++;
        col = 0;
    } else if (c != EOF) {
        col++;
    }

    return c;
}

void skipSingleLine(){
int c;
while((c=getChar())!=EOF && c!='\n');
}

void skipMultiLine(){
int c, prev=0;
while((c=getChar())!=EOF){
if(prev=='*' && c=='/'){
break;
}
prev=c;
}
}

/* ---------------- TOKENIZER ---------------- */
int getNextToken(struct Token *t) {

    int c = getChar();

    while (c != EOF) {

        if (isspace(c)) {
            c = getChar();
            continue;
        }
   
    if(c=='#'){
    skipSingleLine();
    c=getChar();
    continue;
    }
   
    if(c=='/'){
    int next=getChar();
    if(next=='/'){
    skipSingleLine();
    c=getChar();
    continue;
    }
    else if(next=='*'){
    skipMultiLine();
    c=getChar();
    continue;
    }
    ungetc(next, fp);
        if (c == '\n')
    row--;
  else
    col--;
    }
    break;
   }

    if (c == EOF) return 0;

    t->row = row;
    t->col = col;

    int i = 0;

    /* STRING LITERAL */
    if (c == '"' || c=='\'') {
      int quote=c;
        t->name[i++] = c;
        while ((c = getChar()) != quote && c != EOF)
            t->name[i++] = c;
        t->name[i++] = quote;
        t->name[i] = '\0';
        return 1;
    }

    /* IDENTIFIER */
    if (isalpha(c) || c == '_') {

        while (isalnum(c) || c == '_') {
            t->name[i++] = c;
            c = getChar();
        }

        t->name[i] = '\0';
        ungetc(c, fp);
        if (c == '\n')
    row--;
  else
    col--;

        return 1;
    }

    /* NUMBER */
    if (isdigit(c)) {

        while (isdigit(c)) {
            t->name[i++] = c;
            c = getChar();
        }

        t->name[i] = '\0';
        ungetc(c, fp);
        if (c == '\n')
    row--;
  else
    col--;


        return 1;
    }
   
    /*MULTI CHAR TOKEN */
    int next = getChar();
    char two[3] = {c, next, '\0'};

    if (!strcmp(two,"&&") || !strcmp(two,"==") ||
        !strcmp(two,"<=") || !strcmp(two,">=") ||
        !strcmp(two,"!=") || !strcmp(two,"=>")) {

        strcpy(t->name, two);
        return 1;
    }

    ungetc(next, fp);
    if (next == '\n')
    row--;
    else
    col--;


    /* SINGLE CHAR TOKEN */
    t->name[0] = c;
    t->name[1] = '\0';
    return 1;
}

/* ---------------- PROCESS IDENTIFIER ---------------- */
void processIdentifier(struct Token *t) {

    if (isKeyword(t->name))
        return;

    long pos = ftell(fp);
    struct Token next;

    if (getNextToken(&next)) {

        /* FUNCTION CHECK */
        if (!strcmp(next.name, "(")) {

            char args[200] = "";
            int c = getChar();
            int i = 0;

            /* Capture until first ')' */
            while (c != ')' && c != EOF) {
                if (i < 195)
                    args[i++] = c;
                c = getChar();
            }

            args[i] = '\0';

            insert(t->name, "function", args);
        }
        else {
            insert(t->name, "identifier", "");
            fseek(fp, pos, SEEK_SET);
        }
    }
}

/* ---------------- PRINT SYMBOL TABLE ---------------- */
void printSymbolTable() {

    printf("\nSYMBOL TABLE\n");
    printf("%-15s | %-12s | %-20s\n",
           "Name", "Category", "Argument");
    printf("--------------------------------------------------\n");

    for (int i = 0; i < TABLE_SIZE; i++) {

        struct SymbolEntry *curr = symbolTable[i];

        while (curr) {
            printf("%-15s | %-12s | %-20s\n",
                   curr->name,
                   curr->category,
                   curr->argument);

            curr = curr->next;
        }
    }
}

/* ---------------- MAIN ---------------- */
int main() {

    fp = fopen("digit.c", "r");

    if (!fp) {
        printf("File not found\n");
        return 1;
    }

    struct Token t;

    printf("TOKEN LIST:\n\n");

    while (getNextToken(&t)) {

        printf("<%s, %d, %d>\n", t.name, t.row, t.col);

        if (isalpha(t.name[0]) || t.name[0]=='_')
            processIdentifier(&t);
    }

    fclose(fp);

    printSymbolTable();

    return 0;
}