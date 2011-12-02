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
int background=0;
int main (int argc, char *argv[]){
	myShellName= getenv("MYPS");
	if (myShellName == NULL){
		printf("Shell env was null, making it mysh \n");
		myShellName="mysh$ ";
	}
	homeName= getenv("HOME");
	printf("%s \n", homeName);
	pid_t child_pid;
	int child_status;
	while (flag){
		clearArray(args, 10);
		background=0;
		printf("%s", myShellName);	
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
void clearArray(char ** array, int size){
	int i;
	for (i = 0; i < size; i++){
		array[i]='\0';
	}
	return;
}

int runcmd(char **cmd){
	pid_t child_pid;
	int child_status;
    	char* errbuf;
	if (strcmp(args[0], "cd") == 0){
		//printf("Result of changing dir was: %d \n", chdir(args[1]));
		printf("Args[1] is: %s \n", args[1]);
		printf("Home is: %s \n", homeName);
		int result;
		if (args[1]==NULL){
			result=chdir(homeName);
		} else {
			//args[1]=parseOutPath(args[1]);	
			result = chdir(args[1]);
		}
		if (result == 0){
			char *cwd;
			printf("%s \n", getcwd(cwd, 1000));
			printf("Directory changed\n");
		}else{
			switch(result){
				case EACCES: printf("Permission denied");break;
				case EIO: printf("An input output error occurred"); break;
				case ENAMETOOLONG: printf("Path is to long"); break;
				case ENOTDIR: printf("A component of path not a diretory"); break;
				case ENOENT: printf("No such file or directory"); printf("enoent\n"); 
				//default: perror("Couldn't change directory to %s", (char *) args[1]);
				printf("Couldn't change directory to %s");
				_exit(1);
			}
		}
		//_exit(0);
	}else{
		child_pid = vfork();
		//printf("Child Process ID: %d \n", child_pid);
		if (child_pid == -1){
			printf("There was an error in the process creation");
			_exit(1);
		}
		if (child_pid == 0) {
		//In child
			if (execvp(args[0], args) ==-1){
				sprintf(errbuf,"%s: child process id =%d",myShellName,child_pid);
       		     		perror(errbuf);
				_exit(1);
			}
		_exit(0);
		}
		else { 
		//In parent

		if(!background){
			wait(&child_status);
		}
		//printf("AWWW SHIT SON IM THE BIG DADDY PROCESS \n");
		/*do{
			pid_t tpid=wait(&child_status);
			if(tpid!=child_pid) {
				process_terminated(tpid);
			}
		}while(tpid!=child_pid);	*/
	
		}
	}
	return 0;
	
}
/*char* parseOutPath(char* argToCheck){
	if (argToCheck[0] == '$'){
		char* temp;
		temp=strtok(argToCheck,'/');
		char* getRidOFMoney = NULL;
		int i;
		for (i =0; i < strlen(temp); i++){
			getRidOFMoney[i]=temp[i+1];
		}
		temp=getenv(getRidOfMoney);
		return strcat(temp, argToCheck);
	}else {
		return argToCheck;
	}
}*/
void parseArg(char* line){
	int i=1;
	char* temp = NULL;
//	printf("line was:%s",line);
	while((temp=strtok(NULL,"\n "))!=NULL&&(i<10)){
		//If temp has $, get env var of temp and set to args
		//temp = parseOutPath(temp);
		//printf("[DEBUG] char at begining is:%d",line[0]);
		if(findAmpLamp(temp)==0){
			args[i] = temp;
			i++;
		}
	}
	args[i+1]=NULL;	
}

int findAmpLamp(char* arg){
	if(strcmp(arg,"&")==0){
		printf("Found that & in arg:%s\n",arg);
		background=1;
		return 1;
	}else return 0;


}
