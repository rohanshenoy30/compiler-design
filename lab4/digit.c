#include<stdio.h>
#idefne<xyz.h>
int main(){
	char username[5]="admin"; //string initialization
	char password[4]="1234";
	char u,p; /* declaration 
	complete*/
	printf("Enter username and password:");
	scanf("%s%s,u,p");
	//logical and (&&) operator
	if (strcmp(u,username)==0 &&(u!=p){
	printf("Login succesful\n");}
	elseif(strcmp(u,username)==0||(u>=p){
	printf("Partial match\n");
	
	}else{
	printf("Login failed\n");
	}
return 0;
}
