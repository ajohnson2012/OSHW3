#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include "main.h"

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
				printf("The command was %s", text);
				parseArg(text);
			}
		}		
	}
	return 0;
}
void parseArg(char* args){
	char* arg=strtok(args,"\0");
	printf("%s", arg);
}
