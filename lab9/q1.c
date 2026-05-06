#include<stdio.h>
#include<stdlib.h>
#include<string.h>


char* symbol[6]= {"id","(",")","*","+","$"};
typedef struct{
char* plus;
char* mult;
char* lbracket;
char* rbracket;
char* plus;
char* dollar;
}stateaction;


stateaction ACTION[12]={

{"","","s4","","s5",""},
{"s6","","","",""."accept"},
{"r2","s7","","r2","","r2"},
{"r4","r4","","r4","","r2"},
{"","","s4","","s5",""},
{"r6","r6","","r6","","r6"},
{"","","s4","","s5",""},
{"","","s4","","s5",""},
{"s6","","","s11","",""},
{"s1","s7","","r1","","r1"},
{"r3","r5","","r3","","r3"},
{"r5","r5","","r5","","r5"},

};


char GOTO[][]={

{1,2,3},
{-1,-1,-1},
{-1,-1,-1},
{-1,-1,-1},
{8,2,3},
{-1,-1,-1},
{-1,9,3},
{-1,-1,10},

};

char lhs={'E','T','F',};

char productions[]={};

char production_head[]={};

int production_len[]={};

char getsymbol(){


}


int main()
{
	printf("Constructing the SLR Parser for the following grammar");
	//check for S
	//then check for R
	//else if check=accept
	//else
	//error
}


