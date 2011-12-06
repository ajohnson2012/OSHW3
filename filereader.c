#include <stdio.h>
#include <string.h>
#include "filereader.h"
#include "main.h"

char* args[10];
int readFile(char* filename){
	FILE*fp;
	int flag=1;
	char str[1000];
	fp = fopen (filename, "r" );
	if(fp==NULL){
		return -1;
	}
	while (flag){
		clearArray(args, 10);
		if(fgets(str, 1000,fp) !=NULL){
			if(strcmp(str, "quit\n")==0){;
				return 0;
			}	
			else{
				args[0]=strtok(str, " \n");
				parseArgs(str);
				runcmd(args);	
			}
		}
		else{
			fclose(fp);
			flag=0;
		}
	}
	return 0;


}
