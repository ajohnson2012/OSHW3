#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include "main.h"

char* args[10];
int flag = 1;
char text[1000];
char quit[]= "quit\n";
char* myShellName = getenv("MYPS");
char* homeName= getenv("HOME");	
int main (int argc, char *argv[]){
	pid_t child_pid;
	int child_status;
	while (flag){
		if (myShellName == NULL)
			printf("mysh$ ");
		else 
			printf(myShellName);
		if (fgets(text, 1000, stdin)){
			if(strcmp(text, quit)==0){
				return 0;
			}	
			else{
				args[0]=strtok(text, " ");
				printf("The command was %s\n", args[0]);
				parseArg(text);
				if ((pid=vfork()) == 0) {
					//In child
					if (execvp("/bin/ls", NULL) ==-1){
						printf("GREAT GOLLY MISS MOLLY THERE WAS AN ERROR");
					}
				}
				else { 
					//In parent
					
				}
			}
		}		
	}
	return 0;
}
void parseArg(char* line){
	int i=1;
	char* temp;
//	printf("line was:%s",line);
	while((temp=strtok(NULL,"\n "))!=NULL&&(i<10)){
		//printf("[DEBUG] char at begining is:%d",line[0]);
		args[i] = temp;
		//if(args[i]=='0'){
		//	printf("null char");
		//}
		printf("%s\n", args[i]);
		i++;
	}
	
}
