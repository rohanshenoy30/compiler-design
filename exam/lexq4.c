#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#define MAXBUF 100
void remove_blanks(FILE *fp, FILE *out){
	char filename[100], c;
	c=fgetc(fp);
	while(c!=EOF){
		if(c=='\t'){
			while(c=='\t'){
				c=fgetc(fp);
			}
		}
		if(c==' ' || c=='\t'){
			fputc(' ', out);
			while(c==' ' || c=='\t'){
				c=fgetc(fp);
			}
		}
		if(c=='\n'){
			fputc(c, out);
			c=fgetc(fp);
			if(c=='\n'){
				c=fgetc(fp);
			}
		}
		else{
			fputc(c, out);
			c=fgetc(fp);
		}
	}
}
void remove_preproc(FILE *fp, FILE *out){
	char filename[100], c, cb, cc;
	char* directives[2] = {"include", "define"};
	int len = sizeof(directives)/sizeof(directives[0]);
	char buf[20];
	c=fgetc(fp);
	while(c!=EOF){
		if(c=='#'){
			int i = 0;
			do{
				c=fgetc(fp);
				buf[i++]=c;
			}while(isalnum(c));
			buf[--i]='\0';
			int flag=0;
			for(int i=0;i<len;i++){
				if(strcmp(buf, directives[i])==0){
					while(c!='\n'){
						c=fgetc(fp);
					}
					c=fgetc(fp);
					flag=1;
					break;
				}
			}
			if(flag==0){
				fputc('#', out);
				for(int i=0;buf[i]!='\0';i++){
					fputc(buf[i], out);
				}
				while(c!='\n'){
					fputc(c, out);
					c=fgetc(fp);
				}
				fputc(c, out);
				c=fgetc(fp);
			}
		}
		else{
			while(c!='\n'){
				fputc(c, out);
				c=fgetc(fp);
			}
			fputc(c, out);
			c=fgetc(fp);
		}
	}
}
enum TokenType{
	TOKEN_EOF, TOKEN_HASH, TOKEN_SEMICOLON, TOKEN_COMMA, TOKEN_COLON,
	TOKEN_LBRACKET, TOKEN_RBRACKET, TOKEN_LBRACE, TOKEN_RBRACE, TOKEN_LSQUARE,
	TOKEN_RSQUARE,
	TOKEN_ADD, TOKEN_SUB, TOKEN_MUL, TOKEN_DIV, TOKEN_MOD,
	TOKEN_INC, TOKEN_DEC,
	TOKEN_ASSIGN, TOKEN_ADD_ASSIGN, TOKEN_SUB_ASSIGN, TOKEN_MUL_ASSIGN,
	TOKEN_DIV_ASSIGN, TOKEN_MOD_ASSIGN,
	TOKEN_EQ, TOKEN_LT, TOKEN_LE, TOKEN_GT, TOKEN_GE, TOKEN_NE,
	TOKEN_AND, TOKEN_OR, TOKEN_NOT, TOKEN_BIT_AND, TOKEN_BIT_OR,
	TOKEN_IDENTIFIER, TOKEN_KEYWORD, TOKEN_INT, TOKEN_FLOAT, TOKEN_STRING, TOKEN_NULL
//keep float after int always
};
char* TokenNames[] = {
	"EOF", "#", ";", ",", ":",
	"(", ")", "{", "}", "[", "]",
	"+", "-", "*", "/", "%",
	"++", "--",
	"=", "+=", "-=", "*=", "/=", "%=",
	"==", "<", "<=", ">", ">=", "!=",
	"&&", "||", "!", "&", "|",
	"id", "Keyword", "int", "float", "string", "NULL"
};
char* directives[] = {
	"break", "case", "char", "const", "auto", "short", "struct",
	"switch", "double", "int", "else", "enum", "float", "continue", "sizeof",
	"default",
	"extern", "for", "do", "goto", "if", "typedef", "union", "void", "static",
	"signed",
	"long", "register", "return", "unsigned", "volatile", "while"
};
int directive_len = sizeof(directives)/sizeof(directives[0]);
char recentType[MAXBUF];
int recentSize;
char* identifierType[] = {
	"void", "int", "char", "float"
};
int identifierSize[] = {
	0, 4, 1, 4
};
void updateRecentType(char* buf){
	for(int i=0;i<sizeof(identifierType)/sizeof(identifierType[0]);i++){
		if(strcmp(buf, identifierType[i])==0){
			strcpy(recentType, buf);
			recentSize=identifierSize[i];
		}
	}
}
unsigned int row = 1, col = 0;
struct token {
	enum TokenType token_name;
	unsigned int row, col;
	int index;
};
struct symbol{
	char lexeme[50];
	char datatype[50];
	char returntype[50];
	int size;
};
#define TableLength 41
struct ListElement{
	struct symbol sym;
	struct ListElement *next;
};
struct ListElement *TABLE[TableLength];
void InitializeTable(){
	for(int i=0;i<TableLength;i++){
		TABLE[i] = NULL;
	}
}
int HashIndex(char* lexeme){
	int sum = 0;
	for(int i=0;i<strlen(lexeme);i++){
		sum+=lexeme[i];
	}
	return sum % TableLength;
}
int SearchTable(char* string, int index){
	if(TABLE[index]==NULL) return 0;
	struct ListElement* temp = TABLE[index];
	if(strcmp(temp->sym.lexeme, string)==0) return 1;
	while(temp->next!=NULL){
		temp = temp->next;
		if(strcmp(temp->sym.lexeme, string)==0){
			return 1;
		}
	}
	return 0;
}
void InsertIntoTable(struct symbol sym, int index){
	struct ListElement* cur = (struct ListElement*)malloc(sizeof(struct
		ListElement));
	cur->sym = sym;
	cur->next = NULL;
	if(TABLE[index]==NULL){
		TABLE[index]=cur;
	}
	else{
		struct ListElement* temp = TABLE[index];
		while(temp->next != NULL){
			temp = temp->next;
		}
		temp->next = cur;
	}
}
void DisplayTable(){
	printf("\nSymbol Table:\nIndex\tName\tType\tSize\tReturn Type\n");
	int i=0;
	struct ListElement* temp;
	while(i<TableLength){
		if(TABLE[i]==NULL){
			i++;
			continue;
		}
		temp = TABLE[i];
		printf("%d\t%s\t%s\t%d\t%s\n", i, temp->sym.lexeme, temp->sym.datatype,
			temp->sym.size, temp->sym.returntype);
		while(temp->next!=NULL){
			temp = temp->next;
			printf("%d\t%s\t%s\t%d\t%s\n", i, temp->sym.lexeme, temp->sym.datatype,
				temp->sym.size, temp->sym.returntype);
		}
		i++;
	}
}
int isOperator(char c){
 char* op = "+-*/%=!<>|&[]{}();:,";
	for(int i=0;i<strlen(op);i++){
		if(op[i]==c)return 1;
	}
	return 0;
}
struct token manageOperator(FILE *fp, char c){
	struct token temp;
	temp.row=row;
	temp.col=col;
	switch(c){
	case '=':
		c = fgetc(fp);col++;
		if(c=='='){
			temp.token_name = TOKEN_EQ;
			break;
		}
		ungetc(c, fp);col--;
		temp.token_name = TOKEN_ASSIGN;
		break;
	case '<':
		c = fgetc(fp);col++;
		if(c=='='){
			temp.token_name = TOKEN_LE;
			break;
		}
		ungetc(c, fp);col--;
		temp.token_name = TOKEN_LT;
		break;
	case '>':
		c = fgetc(fp);col++;
		if(c=='='){
			temp.token_name = TOKEN_GE;
			break;
		}
		ungetc(c, fp);col--;
		temp.token_name = TOKEN_GT;
		break;
	case '!':
		c = fgetc(fp);col++;
		if(c=='='){
			temp.token_name = TOKEN_NE;
			break;
		}
		ungetc(c, fp);col--;
		temp.token_name = TOKEN_NOT;
		break;
	case '+':
		c=fgetc(fp);col++;
		if(c=='+'){
			temp.token_name = TOKEN_INC;
			break;
		}
		else if(c=='='){
			temp.token_name = TOKEN_ADD_ASSIGN;
			break;
		}
		ungetc(c, fp);col--;
		temp.token_name = TOKEN_ADD;
		break;

	case '-':
		c=fgetc(fp);col++;
		if(c=='-'){
			temp.token_name = TOKEN_DEC;
			break;
		}
		else if(c=='='){
			temp.token_name = TOKEN_SUB_ASSIGN;
			break;
		}
		ungetc(c, fp);col--;
		temp.token_name = TOKEN_SUB;
		break;
	case '*':
		c=fgetc(fp);col++;
		if(c=='='){
			temp.token_name = TOKEN_MUL_ASSIGN;
			break;
		}
		ungetc(c, fp);col--;
		temp.token_name = TOKEN_MUL;
		break;
	case '/':
		c=fgetc(fp);col++;
		if(c=='='){
			temp.token_name = TOKEN_DIV_ASSIGN;
			break;
		}
		ungetc(c, fp);col--;
 temp.token_name = TOKEN_DIV; //DO COMMENTS
 break;
case '|':
	c=fgetc(fp);col++;if(c=='|'){
		temp.token_name = TOKEN_OR;
		break;
	}
	ungetc(c, fp);col--;
	temp.token_name = TOKEN_BIT_OR;
	break;
case '%':
	c=fgetc(fp);col++;
	if(c=='='){
		temp.token_name = TOKEN_MOD_ASSIGN;
		break;
	}
	ungetc(c, fp);col--;
	temp.token_name = TOKEN_MOD;
	break;
case '&':
	c=fgetc(fp);col++;
	if(c=='&'){
		temp.token_name = TOKEN_AND;
		break;
	}
	ungetc(c, fp);col--;
	temp.token_name = TOKEN_BIT_AND;
	break;
case '[':
	temp.token_name = TOKEN_LSQUARE;
	break;
case ']':
	temp.token_name = TOKEN_RSQUARE;
	break;
case '(':
	temp.token_name = TOKEN_LBRACKET;
	break;
case ')':
	temp.token_name = TOKEN_RBRACKET;
	break;
case '{':
	temp.token_name = TOKEN_LBRACE;
	break;
case '}':
	temp.token_name = TOKEN_RBRACE;
	break;
case ',':
	temp.token_name = TOKEN_COMMA;
	break;
case ';':
	temp.token_name = TOKEN_SEMICOLON;
	strcpy(recentType, "-1");
	recentSize = -1;
	break;
case ':':
	temp.token_name = TOKEN_COLON;
	break;
}
return temp;
}
struct token manageIdentifierKeyword(FILE *fp, char c){
	struct token temp;
	temp.row=row;
	temp.col=col;
	char buf[MAXBUF];
	int i = 0;
	buf[i++] = c;
	c = fgetc(fp);col++;
	while(isalnum(c)||c=='_'){
		buf[i++] = c;
		c = fgetc(fp);col++;
	}
	buf[i]='\0';
	ungetc(c, fp);col--;
	for(int j=0;j<directive_len;j++){
		if(strcmp(buf, directives[j])==0){
			temp.token_name = TOKEN_KEYWORD;
			updateRecentType(buf);
			return temp;
		}
	}
	temp.token_name = TOKEN_IDENTIFIER;

	int index = HashIndex(buf);
	if(SearchTable(buf, index)==0){
		struct symbol sym;
		strcpy(sym.lexeme, buf);
		c=fgetc(fp);
		if(c=='('){
			strcpy(sym.datatype, "func");
			strcpy(sym.returntype, recentType);
			sym.size = -1;
			InsertIntoTable(sym, index);
		}
		else{
			strcpy(sym.datatype, recentType);
			strcpy(sym.returntype, "-1");
			sym.size = recentSize;
			InsertIntoTable(sym, index);
		}
		ungetc(c, fp);
	}
	temp.index = index;
	return temp;
}
struct token manageNumber(FILE *fp, char c){
	struct token temp;
	temp.row=row;
	temp.col=col;
	char buf[MAXBUF];
	int i=0;
	int isfloat = 0;
	while(isdigit(c)||c=='.'){
		if(c=='.')isfloat=1;
		buf[i++]=c;
		c = fgetc(fp);col++;
	}
	buf[i]='\0';
	ungetc(c, fp);col--;
	temp.token_name = TOKEN_INT + isfloat;
	return temp;
}
struct token manageString(FILE *fp, char c){
	struct token temp;
	temp.row=row;
	temp.col=col;
	char buf[MAXBUF];
	int i=0;
	buf[i++]=c;
	c = fgetc(fp);col++;
	while(c!='\"'){
		buf[i++]=c;
		c = fgetc(fp);col++;
	}
	buf[i++]='\"';
	buf[i]='\0';
	temp.token_name = TOKEN_STRING;
	return temp;
}
struct token getNextToken(FILE *in){
	char c;
	struct token temp;
	struct symbol sym;
	sym.lexeme[0]='\0';
	c = fgetc(in);col++;
	while(c != EOF){
		if(c==' '){
			c = fgetc(in);col++;
		}
		else if(c=='\n'){
			c = fgetc(in);row++;
			col=1;
		}
		else if(c=='#'){
			c = fgetc(in);col++;
			while(c!='\n'){
				c = fgetc(in);col++;
			}
			c = fgetc(in);row++;
			col=1;
		}
		else{
			break;
		}
	}
	if(c==EOF){
		temp.col = col;
		temp.row = row;
		temp.token_name = TOKEN_EOF;
		return temp;
	}
	if(c=='\"'){
		return manageString(in, c);
	}
	if(isdigit(c)){
		return manageNumber(in, c);
	}
	if(isalpha(c)||c=='_'){
		return manageIdentifierKeyword(in, c);
	}
	if(isOperator(c)){
		return manageOperator(in, c);
	}
}
int main(){
 //Remove all blank spaces
	FILE *inFP=fopen("sample.txt","r");
	if(inFP==NULL){
		printf("Cannot open file\n");
		exit(0);
	}
	FILE *outFP = fopen("1.txt","w+");
	remove_blanks(inFP, outFP);
	fclose(inFP);
	fclose(outFP);
 //Remove all preprocessor directives
	inFP=fopen("1.txt","r");
	if(inFP==NULL){
		printf("Cannot open file\n");
		exit(0);
	}
	outFP = fopen("2.txt","w+");
	remove_preproc(inFP, outFP);
	fclose(inFP);
	fclose(outFP);
 //Lexical Analysis
	inFP=fopen("2.txt","r");
	struct token t;
	if(inFP==NULL){
		printf("Cannot open file\n");
		exit(0);
	}
	while(1){
		t=getNextToken(inFP);
		if(t.token_name==TOKEN_EOF)
			break;
		printf("<%s,%u,%u>\n",TokenNames[t.token_name],t.row,t.col);
	}
	DisplayTable();
	fclose(inFP);
	return 0;
}