#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include "main.h"

char* args[10];

int main (int argc, char *argv[]){
	int flag = 1;
	char text[1000];
	char quit[]= "quit\n";
	
	while (flag){
		printf("mysh$ ");
		if (fgets(text, 1000, stdin)){
			if(strcmp(text, quit)==0){
				return 0;
			}	
			else{
				args[0]=strtok(text, " ");
				printf("The command was %s\n", args[0]);
				parseArg(text);
			}
		}		
	}
	return 0;
}
void parseArg(char* line){
	int i=1;
	char* temp;
//	printf("line was:%s",line);
	while((temp=strtok(NULL,"\n "))!=NULL){
		//printf("[DEBUG] char at begining is:%d",line[0]);
		args[i] = temp;
		//if(args[i]=='0'){
		//	printf("null char");
		//}
		printf("%s\n", args[i]);
		i++;
	}	
}
