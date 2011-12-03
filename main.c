#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include "main.h"
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include "filereader.h"

char* args[10];
int flag = 1;
char text[1000];
char quit[]= "quit\n";
char* myShellName; 
char* homeName;
int background=0;
int redir_stdout=0;
int redir_stdin=0;
int redir_stderr=0;
int new_stdout=0;
int new_stdin=0;
int new_stderr=0;

int main (int argc, char *argv[]){
	myShellName= getenv("MYPS");
	if (myShellName == NULL){
		printf("Shell env was null, making it mysh \n");
		myShellName="mysh$ ";
	}
	homeName= getenv("HOME");
	printf("%s \n", homeName);

	int returnStatus;

	if(argc>1){
		printf("There are %d arguments\n", argc);
		printf("The filename to use is: %s\n", argv[1]);
		returnStatus=readFile(argv[1]);
		if(returnStatus==-1){
			printf("ERROR no such file exists");
			return -1;
		}
		return 0;
	}
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
				parseArgs(text);
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
int checkForRedirection(int arglen){
	char * stderr = "2>";
	char stdin= '<';
	char stdout = '>';
	char* stderrPtr;
	char* stdinPtr;
	char* stdoutPtr;
	printf("Checking for redirection janx\n");
	int i=0;
	for(i =0; i < arglen; i++){
		stderrPtr=strstr(args[i], stderr);
		stdinPtr=strchr (args[i], stdin);
		stdoutPtr=strchr (args[i], stdout);
		if (stderrPtr !=NULL){
			printf("stderr redirection turning on.\n");
			redir_stderr=1;
			//new_stderr = open(&(args[i][1]), O_WRONLY|O_CREAT|O_TRUNC,(mode_t)0644);
			if (new_stderr == -1) {
				// Open failed: error-handling here
				printf("idk what happened, shit.");
			}
		}
		
		if(stdinPtr !=NULL){
			printf("stdin redirection turning on.\n");
			redir_stdin=1;
			//new_stdin = open(&(args[i][1]), O_WRONLY|O_CREAT|O_TRUNC,(mode_t)0644);
			if(new_stdin==-1){
				printf("idk what happened, shit.");
			}

		}
		
		if(stdoutPtr !=NULL){
			printf("stdout redirection\n");
			redir_stdout=1;
			//printf("file to be used:%s", args[i][1]);
			new_stdin = open("test.txt", O_WRONLY|O_CREAT|O_TRUNC,(mode_t)0644);
			if(new_stdout==-1){
				printf("idk what happened, shit.");
			}
		}
		
	}
	return 1;
}

int runcmd(char **cmd){
	pid_t child_pid;
	int child_status;
    	char* errbuf;
	if (strcmp(args[0], "cd") == 0){
		//printf("Result of changing dir was: %d \n", chdir(args[1]));
		//printf("Args[1] is: %s \n", args[1]);
		//printf("Home is: %s \n", homeName);
		int result;
		if (args[1]==NULL){
			result=chdir(homeName);
		} else {
			result = chdir(args[1]);
		}
		if (result == 0){
			char *cwd;
			printf("%s \n", getcwd(cwd, 1000));
			//printf("Directory changed\n");
		}else{
			switch(result){
				case EACCES: printf("Permission denied");break;
				case EIO: printf("An input output error occurred"); break;
				case ENAMETOOLONG: printf("Path is to long"); break;
				case ENOTDIR: printf("A component of path not a diretory"); break;
				case ENOENT: printf("No such file or directory"); printf("enoent\n"); 
				//default: perror("Couldn't change directory to %s", (char *) args[1]);
				printf("Couldn't change directory to %s",args[1]);
				_exit(1);
			}
		}

	}else{
		child_pid = vfork();

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

void parseArgs(char* line){
	int i=1;
	char* temp = NULL;
	while((temp=strtok(NULL,"\n "))!=NULL&&(i<10)){
		//If temp has $, get env var of temp and set to args
		//temp = parseOutPath(temp);

		if(!findAmpLamp(temp)){
			args[i] = temp;
			i++;
		}
	}
	args[i+1]=NULL;	
	checkForRedirection(i);
}

int findAmpLamp(char* arg){
	if(strcmp(arg,"&")==0){
		printf("Found that & in arg:%s\n",arg);
		background=1;
		return 1;
	}else return 0;


}
