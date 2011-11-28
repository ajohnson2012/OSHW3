#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include "main.h"
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

char* args[10];
int flag = 1;
char text[1000];
char quit[]= "quit\n";
char* myShellName; 
char* homeName;
int main (int argc, char *argv[]){
	myShellName= getenv("MYPS");
	homeName= getenv("HOME");
	pid_t child_pid;
	int child_status;
	while (flag){
		if (myShellName == NULL)
			printf("mysh$ ");
		else 
			printf("%s",myShellName);
		if (fgets(text, 1000, stdin)){
			if(strcmp(text, quit)==0){
				return 0;
			}	
			else{
				args[0]=strtok(text, "\n ");
			//	printf("The command was %s\n", args[0]);
				parseArg(text);
				runcmd(args);	
			}
		}		
	}
	return 0;
}


int runcmd(char *cmd){
	pid_t child_pid;
	int child_status;
    	char* errbuf;
	if ((child_pid=vfork()) == 0) {
		//In child
		//Check to see if it's a chdir command
		if (strcmp(args[0], "cd") == 0){
			//printf("Result of changing dir was: %d \n", chdir(args[1]));
			printf("Args[1] is: %s", args[1]);
			int result = chdir(args[1]);
			if (result == 0){
				printf("Directory changed");
			}else{
				switch(result){
					case EACCES: perror("Permission denied");break;
					case EIO: perror("An input output error occurred"); break;
					case ENAMETOOLONG: perror("Path is to long"); break;
					case ENOTDIR: perror("A component of path not a diretory"); break;
					case ENOENT: perror("No such file or directory"); printf("enoent\n"); 
					//default: perror("Couldn't change directory to %s", (char *) args[1]);
					printf("Couldn't change directory to %s");
				}
			}
		}
			//if (chdir(args[1]) == -1)
		//	printf("There was an error while changing directories\n");
		
		if (execvp(args[0], args) ==-1){
			sprintf(errbuf,"%s: child process id =%d",myShellName,child_pid);
            		perror(errbuf);
    //        printf("GREAT GOLLY MISS MOLLY THERE WAS AN ERROR \n");
		}
		_exit(0);
	}
	else { 
	//In parent
		wait(&child_status);
		//printf("AWWW SHIT SON IM THE BIG DADDY PROCESS \n");
		/*do{
			pid_t tpid=wait(&child_status);
			if(tpid!=child_pid) {
				process_terminated(tpid);
			}
		}while(tpid!=child_pid);	*/
	
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
		//printf("%s\n", args[i]);
		i++;
	}
	args[i+1]=NULL;	
}
