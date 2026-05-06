#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

/* =====================================================
TOKEN STRUCTURE
===================================================== */

typedef struct {
    char lexeme[100];
    char type[20];
    int row;
    int col;
} Token;

FILE *fp;
Token lookahead;

int row=1,col=0;
int errorCount=0;

Token tokenStream[1000];
int tokenCount=0;
int stopRecording=0;

/* =====================================================
KEYWORDS
===================================================== */

char *keywords[]={"main","int","char","if","else"};
int kwCount=5;

int isKeyword(char *str)
{
    for(int i=0;i<kwCount;i++)
        if(strcmp(str,keywords[i])==0)
            return 1;
    return 0;
}

/* =====================================================
LEXER
===================================================== */

Token getNextToken()
{
    Token token;
    char ch;
    char buffer[100];

    while((ch=fgetc(fp))!=EOF)
    {
        col++;

        if(ch=='\n')
        {
            row++;
            col=0;
            continue;
        }

        if(isspace(ch))
            continue;

        /* COMMENTS */

        if(ch=='/')
        {
            char next=fgetc(fp);

            if(next=='/')
            {
                while((ch=fgetc(fp))!='\n' && ch!=EOF);
                row++;
                col=0;
                continue;
            }

            else if(next=='*')
            {
                char prev=0;

                while((ch=fgetc(fp))!=EOF)
                {
                    if(ch=='\n')
                    {
                        row++;
                        col=0;
                    }

                    if(prev=='*' && ch=='/')
                        break;

                    prev=ch;
                }
                continue;
            }

            else
                ungetc(next,fp);
        }

        token.row=row;
        token.col=col;

        /* IDENTIFIER */

        if(isalpha(ch))
        {
            int i=0;
            buffer[i++]=ch;

            while(isalnum(ch=fgetc(fp)))
            {
                buffer[i++]=ch;
                col++;
            }

            buffer[i]='\0';
            ungetc(ch,fp);

            strcpy(token.lexeme,buffer);

            if(isKeyword(buffer))
                strcpy(token.type,buffer);
            else
                strcpy(token.type,"id");

            if(!stopRecording)
                tokenStream[tokenCount++]=token;

            return token;
        }

        /* NUMBER */

        if(isdigit(ch))
        {
            int i=0;
            buffer[i++]=ch;

            while(isdigit(ch=fgetc(fp)))
            {
                buffer[i++]=ch;
                col++;
            }

            buffer[i]='\0';
            ungetc(ch,fp);

            strcpy(token.lexeme,buffer);
            strcpy(token.type,"num");

            if(!stopRecording)
                tokenStream[tokenCount++]=token;

            return token;
        }

        /* STRING */

        if(ch=='"')
        {
            int i=0;

            while((ch=fgetc(fp))!='"' && ch!=EOF)
            {
                buffer[i++]=ch;
                col++;
            }

            buffer[i]='\0';

            strcpy(token.lexeme,buffer);
            strcpy(token.type,"string");

            if(!stopRecording)
                tokenStream[tokenCount++]=token;

            return token;
        }

        /* SYMBOL */

        token.lexeme[0]=ch;
        token.lexeme[1]='\0';
        strcpy(token.type,token.lexeme);

        if(!stopRecording)
            tokenStream[tokenCount++]=token;

        return token;
    }

    strcpy(token.lexeme,"EOF");
    strcpy(token.type,"EOF");
    token.row=row;
    token.col=col;

    return token;
}

/* =====================================================
ERROR
===================================================== */

void error(char *expected)
{
    printf("\nSyntax Error at line %d column %d\n",
           lookahead.row,lookahead.col);

    printf("Expected : %s\n",expected);
    printf("Found    : %s\n",lookahead.lexeme);

    errorCount++;
    stopRecording=1;
}

/* =====================================================
MATCH
===================================================== */

void match(char *expected)
{
    if(strcmp(lookahead.lexeme,expected)==0)
        lookahead=getNextToken();
    else
    {
        error(expected);
        lookahead=getNextToken();
    }
}

/* =====================================================
HELPERS
===================================================== */

int isIdentifier()
{
    return strcmp(lookahead.type,"id")==0;
}

int isNumber()
{
    return strcmp(lookahead.type,"num")==0;
}

int isRelop()
{
    return (!strcmp(lookahead.lexeme,">") ||
            !strcmp(lookahead.lexeme,"<") ||
            !strcmp(lookahead.lexeme,"==")||
            !strcmp(lookahead.lexeme,"!="));
}

/* =====================================================
FORWARD DECLARATIONS
===================================================== */

void statement_list();
void statement();
void expn();

/* =====================================================
ARRAY INDEX
===================================================== */

void index_opt()
{
    if(!strcmp(lookahead.lexeme,"["))
    {
        match("[");

        if(isNumber())
            lookahead=getNextToken();
        else
            error("num");

        match("]");
    }
}

/* =====================================================
DECLARATIONS
===================================================== */

void identifier_list_prime();

void identifier_list()
{
    if(isIdentifier())
    {
        lookahead=getNextToken();
        identifier_list_prime();
    }
    else
        error("identifier");
}

void identifier_list_prime()
{
    if(!strcmp(lookahead.lexeme,","))
    {
        match(",");
        identifier_list();
    }

    else if(!strcmp(lookahead.lexeme,"["))
    {
        match("[");
        if(isNumber())
            lookahead=getNextToken();
        else
            error("num");

        match("]");
        identifier_list_prime();
    }

    else if(!strcmp(lookahead.lexeme,";"))
        return;

    else
        error(", or [ or ;");
}

void data_type()
{
    if(!strcmp(lookahead.lexeme,"int") ||
       !strcmp(lookahead.lexeme,"char"))
        lookahead=getNextToken();
    else
        error("int or char");
}

void declarations()
{
    while(!strcmp(lookahead.lexeme,"int") ||
          !strcmp(lookahead.lexeme,"char"))
    {
        data_type();
        identifier_list();
        match(";");
    }
}

/* =====================================================
EXPRESSIONS
===================================================== */

void factor()
{
    if(isIdentifier())
    {
        lookahead=getNextToken();
        index_opt();
    }

    else if(isNumber())
        lookahead=getNextToken();

    else
        error("id or num");
}

void term()
{
    factor();
}

void simple_expn()
{
    term();
}

void expn()
{
    simple_expn();

    if(isRelop())
    {
        lookahead=getNextToken();
        simple_expn();
    }
}

/* =====================================================
ASSIGNMENT
===================================================== */

void assign_stat()
{
    if(isIdentifier())
    {
        lookahead=getNextToken();
        index_opt();

        match("=");

        expn();

        match(";");
    }
    else
        error("identifier");
}

/* =====================================================
IF ELSE
===================================================== */

void decision_stat();

void dprime()
{
    if(!strcmp(lookahead.lexeme,"else"))
    {
        match("else");
        match("{");
        statement_list();
        match("}");
    }
}

void decision_stat()
{
    match("if");
    match("(");
    expn();
    match(")");

    match("{");
    statement_list();
    match("}");

    dprime();
}

/* =====================================================
STATEMENTS
===================================================== */

void statement()
{
    if(isIdentifier())
        assign_stat();

    else if(!strcmp(lookahead.lexeme,"if"))
        decision_stat();

    else
    {
        error("statement");
        lookahead=getNextToken();
    }
}

void statement_list()
{
    while(isIdentifier() || !strcmp(lookahead.lexeme,"if"))
        statement();
}

/* =====================================================
PROGRAM
===================================================== */

void Program()
{
    match("main");
    match("(");
    match(")");
    match("{");

    declarations();
    statement_list();

    match("}");
}

/* =====================================================
MAIN
===================================================== */

int main()
{
    fp=fopen("input.c","r");

    if(!fp)
    {
        printf("File not found\n");
        return 1;
    }

    lookahead=getNextToken();

    Program();

    if(errorCount==0)
         printf("\n---------TOKENS--------------- \n");
    else
         printf("\n-------------TOKENS detected upto error---------\n");

    for(int i=0;i<tokenCount;i++)
    {
        printf("<%s,%d,%d>\n",
               tokenStream[i].type,
               tokenStream[i].row,
               tokenStream[i].col);
    }

    if(errorCount==0)
        printf("\n---------Parsing Successful--------\n");
    else
        printf("\n-----------Parsing Unsuccessful (%d errors)--------------\n",errorCount);

    fclose(fp);
}