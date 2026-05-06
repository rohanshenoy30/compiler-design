#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define TableLength 30

typedef struct {
    char token_name[100];
    char type[50];       
    int size;            
    char returnType[50]; 
} SymbolEntry;

struct ListElement {
    SymbolEntry entry;
    struct ListElement *next;
};

struct ListElement *TABLE[TableLength];

void Initialize() {
    for (int i = 0; i < TableLength; i++) TABLE[i] = NULL;
}

int HASH(char *str) {
    unsigned int hash = 0;
    while (*str) hash = (hash << 5) + *str++;
    return hash % TableLength;
}

int SEARCH(char *str) {
    int val = HASH(str);
    struct ListElement *cur = TABLE[val];
    while (cur) {
        if (strcmp(cur->entry.token_name, str) == 0) return 1;
        cur = cur->next;
    }
    return 0;
}

void INSERT(SymbolEntry se) {
    if (SEARCH(se.token_name)) return; 
    int val = HASH(se.token_name);
    struct ListElement *newNode = (struct ListElement*)malloc(sizeof(struct ListElement));
    newNode->entry = se;
    newNode->next = TABLE[val]; 
    TABLE[val] = newNode;
}

void Display() {
    printf("\n%-35s %-15s %-10s %-15s\n", "Token name", "Type", "Size", "Ret Type");
    printf("--------------------------------------------------------------------------------\n");
    for (int i = 0; i < TableLength; i++) {
        struct ListElement *cur = TABLE[i];
        while (cur) {
            printf("%-35s %-15s %-10d %-15s\n", 
                cur->entry.token_name, cur->entry.type, cur->entry.size, 
                cur->entry.returnType);
            cur = cur->next;
        }
    }
}

int getNextChar(FILE *fp) {
    return fgetc(fp);
}

int peekNextChar(FILE *fp) {
    int c = fgetc(fp);
    ungetc(c, fp);
    return c;
}

int getDataTypeSize(char *type) {
    if (strcmp(type, "int") == 0) return 4;
    if (strcmp(type, "float") == 0) return 4;
    if (strcmp(type, "char") == 0) return 1;
    if (strcmp(type, "double") == 0) return 8;
    return 0;
}

void getNextToken(FILE *fp) {
    char lastType[50] = "-1"; 
    int c;
    
    while ((c = getNextChar(fp)) != EOF) {

        if (c == '#') {
            while ((c = getNextChar(fp)) != '\n' && c != EOF);
            continue;
        }

        if (c == '/' && peekNextChar(fp) == '/') {
            while ((c = getNextChar(fp)) != '\n' && c != EOF);
            continue;
        }

        if (isspace(c)) continue;

        if (c == '"') {
            while ((c = getNextChar(fp)) != '"' && c != EOF);
            continue;
        }

        if (isalpha(c) || c == '_') {
            char buffer[100]; 
            int i = 0;
            buffer[i++] = c;

            while (isalnum(peekNextChar(fp)) || peekNextChar(fp) == '_')
                buffer[i++] = getNextChar(fp);

            buffer[i] = '\0';

            char *keywords[] = {"int", "float", "char", "void", "double", "return"};
            bool isKw = false;

            for(int k = 0; k < 6; k++) {
                if(strcmp(buffer, keywords[k]) == 0) {
                    strcpy(lastType, buffer);
                    isKw = true;
                    break;
                }
            }

            if (!isKw) {
                SymbolEntry se;
                strcpy(se.token_name, buffer);

                if (peekNextChar(fp) == '(') {
                    strcpy(se.type, "Function");

                    if (strcmp(buffer, "printf") == 0)
                        strcpy(se.returnType, "None");
                    else
                        strcpy(se.returnType, lastType);

                    se.size = 0;
                } 
                else {
                    strcpy(se.type, "Variable");
                    strcpy(se.returnType, "-1");
                    se.size = getDataTypeSize(lastType);
                }

                INSERT(se);
            }
        }
    }
}

int main() {
    FILE *fp = fopen("digit.c", "r");
    if (!fp) { 
        printf("Error: Could not open 'digit.c'\n"); 
        return 1; 
    }

    Initialize();
    getNextToken(fp);
    Display();

    fclose(fp);
    return 0;
}