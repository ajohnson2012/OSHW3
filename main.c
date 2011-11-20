#include <stdio.h>
#include <pthread.h>
#include "main.h"

int main (int argc, char *argv[]){
	int flag = 1;
	char *text= NULL;
	while (flag){
		printf("mysh$ ");
		if (fgets(text, 100, stdin)){
			printf("The command was %s", text);
			parseArg(text);
		}	
	}
	return 0;
}
void parseArg(char* args){
	printf("%s", args);
}
