#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "la.c"

#define MAX_ERRORS 100
#define MAX_ERR_LEN 256

FILE *f;
struct token t;
char error_buffer[MAX_ERRORS][MAX_ERR_LEN];
int error_count = 0;

void printCurrentToken() {
    if (strcmp(t.type, "identifier") == 0)
        printf("<id, %d, %d>\n", t.row, t.col);
    else
        printf("<%s, %d, %d>\n", t.token_name, t.row, t.col);
}

void advance() {
    t = getNextToken(f);

    /* Ignore comments */
    while (strcmp(t.type, "comment") == 0) {
        t = getNextToken(f);
    }
}

void record_error(char *expected) {
    if (error_count < MAX_ERRORS) {
        snprintf(error_buffer[error_count], MAX_ERR_LEN,
                 "Row: %d, Col: %d | Expected '%s', but found '%s'",
                 t.row, t.col, expected, t.token_name);
        error_count++;
    }
}

void synchronize() {
    while (t.row != -1 &&
           strcmp(t.token_name, ";") != 0 &&
           strcmp(t.token_name, "}") != 0 &&
           strcmp(t.token_name, "int") != 0 &&
           strcmp(t.token_name, "char") != 0 &&
           strcmp(t.token_name, "if") != 0 &&
           strcmp(t.token_name, "while") != 0) {
        advance();
    }
    if (t.row != -1 && strcmp(t.token_name, ";") == 0) advance();
}

void match(char *expected_name) {
    if (strcmp(t.token_name, expected_name) == 0) {
        printCurrentToken();
        advance();
    } else {
        record_error(expected_name);
        synchronize();
    }
}

void Program();
void declarations();
void datatype();
void id_list();
void id_list_prime();
void id_list_prime_prime();
void stmt_list();
void statement();
void assign_stat();
void expn();
void eprime();
void simple_expn();
void seprime();
void term();
void tprime();
void factor();
void decision_stat();
void dprime();
void looping_stat();
void relop();
void addop();
void mulop();

void Program() {
    advance();
    if (strcmp(t.token_name, "main") == 0) {
        match("main");
        match("(");
        match(")");
        match("{");
        declarations();
        stmt_list();
        match("}");
    } else {
        record_error("main");
    }

    if (error_count == 0) {
        printf("--------PARSING SUCCESSFUL----------\n");
    } else {
        printf("------PARSING FAILED-------\n");
        printf("------Total Errors Found: %d--------\n", error_count);
       
        for (int i = 0; i < error_count; i++) {
            printf("%d. %s\n", i + 1, error_buffer[i]);
        }
    }
}

void declarations() {
    if (strcmp(t.token_name, "int") == 0 || strcmp(t.token_name, "char") == 0) {
        datatype();
        id_list();
        match(";");
        declarations();
    }
}

void datatype() {
    if (strcmp(t.token_name, "int") == 0 || strcmp(t.token_name, "char") == 0) {
        printCurrentToken();
        advance();
    } else {
        record_error("int or char");
    }
}

void id_list() {
    if (strcmp(t.type, "identifier") == 0) {
        printCurrentToken();
        advance();
        id_list_prime();
    } else {
        record_error("identifier");
    }
}

void id_list_prime() {
    if (strcmp(t.token_name, ",") == 0) {
        printCurrentToken();
        advance();
        id_list();
    } else if (strcmp(t.token_name, "[") == 0) {
        printCurrentToken();
        advance();
        if (strcmp(t.type, "number") == 0) {
            printCurrentToken();
            advance();
            match("]");
            id_list_prime_prime();
        } else {
            record_error("number");
        }
    }
}

void id_list_prime_prime() {
    if (strcmp(t.token_name, ",") == 0) {
        printCurrentToken();
        advance();
        id_list();
    }
}

void stmt_list() {
    if (strcmp(t.type, "identifier") == 0 ||
        strcmp(t.token_name, "if") == 0 ||
        strcmp(t.token_name, "while") == 0 ||
        strcmp(t.token_name, "for") == 0) {
        statement();
        stmt_list();
    }
}

void statement() {
    if (strcmp(t.token_name, "if") == 0) {
        decision_stat();
    } else if (strcmp(t.token_name, "while") == 0 ||
               strcmp(t.token_name, "for") == 0) {
        looping_stat();
    } else if (strcmp(t.type, "identifier") == 0) {
        assign_stat();
        match(";");
    } else if (strcmp(t.token_name, "}") != 0 && t.row != -1) {
        record_error("Statement start (id, if, while, for)");
        advance();
    }
}

void assign_stat() {
    if (strcmp(t.type, "identifier") == 0) {
        printCurrentToken();
        advance();
        match("=");
        expn();
    } else {
        record_error("identifier");
    }
}

void expn() {
    simple_expn();
    eprime();
}

void eprime() {
    char *ops[] = {"==", "!=", "<=", ">=", "<", ">"};
    for (int i = 0; i < 6; i++) {
        if (strcmp(t.token_name, ops[i]) == 0) {
            relop();
            simple_expn();
            return;
        }
    }
}

void simple_expn() {
    term();
    seprime();
}

void seprime() {
    if (strcmp(t.token_name, "+") == 0 || strcmp(t.token_name, "-") == 0) {
        addop();
        term();
        seprime();
    }
}

void term() {
    factor();
    tprime();
}

void tprime() {
    if (strcmp(t.token_name, "*") == 0 ||
        strcmp(t.token_name, "/") == 0 ||
        strcmp(t.token_name, "%") == 0) {
        mulop();
        factor();
        tprime();
    }
}

void factor() {
    if (strcmp(t.type, "identifier") == 0 ||
        strcmp(t.type, "number") == 0) {
        printCurrentToken();
        advance();
    } else {
        record_error("identifier or number");
    }
}

void decision_stat() {
    match("if");
    match("(");
    expn();
    match(")");
    match("{");
    stmt_list();
    match("}");
    dprime();
}

void dprime() {
    if (strcmp(t.token_name, "else") == 0) {
        printCurrentToken();
        advance();
        match("{");
        stmt_list();
        match("}");
    }
}

void looping_stat() {
    if (strcmp(t.token_name, "while") == 0) {
        printCurrentToken();
        advance();
        match("(");
        expn();
        match(")");
        match("{");
        stmt_list();
        match("}");
    } else if (strcmp(t.token_name, "for") == 0) {
        printCurrentToken();
        advance();
        match("(");
        assign_stat();
        match(";");
        expn();
        match(";");
        assign_stat();
        match(")");
        match("{");
        stmt_list();
        match("}");
    }
}

void relop() { printCurrentToken(); advance(); }
void addop() { printCurrentToken(); advance(); }
void mulop() { printCurrentToken(); advance(); }

int main() {
    f = fopen("input.c", "r");

    if (!f) {
        printf("Error: Could not open temp.c\n");
        return 1;
    }

    printf("TOKENS UPTO ERROR DETECTED:\n");
    Program();

    fclose(f);
    return 0;
}