#include <stdio.h>
#include <string.h>
#include "filereader.h"
#include "main.h"

char* args[10];
int readFile(char* filename){
	FILE*fp;
	char str[1000];
	fp = fopen (filename, "r" );
	if(fp==NULL){
		return -1;
	}
	
	while(fgets(str, 1000,fp) !=NULL){
		if(strcmp(str, "quit\n")==0){
			printf("quitting\n");
			return 0;
		}	
		else{
			args[0]=strtok(str, " \n");
			printf("The command was %s\n", args[0]);
			parseArgs(str);
			runcmd(args[0]);	
		}
	}	
	fclose(fp);
	return 0;


}


