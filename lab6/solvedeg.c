
#include <stdio.h>
#include <stdlib.h>
int curr = 0;
char str[100];
void invalid(){
printf("Error\n");
exit(0);
}
void valid(){
printf("Success\n");
exit(0);
}
void F();
void E();
void TPrime(){
if(str[curr] == '*'){
curr++;
F();
TPrime();
}
}
void F(){
if(str[curr] == '('){curr++;
E();
if(str[curr] == ')'){
curr++;
return;
}
else
invalid();
}
else if(str[curr] == 'i'){
curr++;
return;
}
else
invalid();
}
void T(){
F();
TPrime();
}
void EPrime(){
if(str[curr] == '+'){
curr++;
T();
EPrime();
}
}
void E(){
T();
EPrime();
}
void main(){
printf("Enter String: ");
scanf("%s", str);
E();
if(str[curr] == '$')
valid();
else
// printf("%c\n", str[curr]);
invalid();
}