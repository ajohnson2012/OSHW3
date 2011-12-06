#include <stdio.h>
#include <string.h>
#include "main.h"
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include "filereader.h"
#include <sys/wait.h>
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
	if (myShellName == NULL || checkShellName(myShellName, strlen(myShellName))){
		myShellName="mysh$ ";
	}
	homeName= getenv("HOME");
	int returnStatus;
	if(argc>1){
		returnStatus=readFile(argv[1]);
		if(returnStatus==-1){
			printf("ERROR no such file exists");
			return -1;
		}
		flag=0;
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
				if (args[0]!=NULL){
					parseArgs(text);
					runcmd(args);
				}
			}
		}
	}
	return 0;
}
int checkShellName(char * name, int len){
	//Returns 0 if string is only spaces or tabs, 1 if it has valid chars
	int flag=0;
	char * temp=NULL;
	temp =strtok(name, " \t");
	if (temp==NULL)
		flag=1;
	return flag;
}
void clearArray(char ** array, int size){
	int i;
	for (i = 0; i < size; i++){
		array[i]='\0';
	}
	return;
}
int checkForRedirection(char* arg){
	char * stderrchar = "2>";
	char stdinchar= '<';
	char stdoutchar = '>';
	char* stderrPtr;
	char* stdinPtr;
	char* stdoutPtr;
	char* fileLoc;

	stderrPtr=strstr(arg, stderrchar);
	stdinPtr=strchr (arg, stdinchar);
	stdoutPtr=strchr (arg, stdoutchar);
	if (stderrPtr !=NULL){
		redir_stderr=1;
		fileLoc=stderrPtr+2;
		new_stderr = open(fileLoc, O_WRONLY|O_CREAT|O_TRUNC,(mode_t)0644);

		if (new_stderr == -1) {
			// Open failed: error-handling here
			printf("idk what happened, shit.");
		}
		return 1;
	}
		
	if(stdinPtr !=NULL){
		redir_stdin=1;
		fileLoc=stdinPtr+1;
		new_stdin = open(fileLoc, O_RDONLY);
		if(new_stdin==-1){
			printf("idk what happened, shit.");
		}
		return 1;
	}
	
	if(stdoutPtr !=NULL){
		redir_stdout=1;
		fileLoc=stdoutPtr+1;
		new_stdout = open(fileLoc, O_WRONLY|O_CREAT|O_TRUNC,(mode_t)0644);
		if(new_stdout==-1){
			printf("idk what happened, shit.");
		}
		return 1;
	}
	return 0;
}

int runcmd(char **cmd){
	pid_t child_pid;
	if (strcmp(args[0], "cd") == 0){
		int result;
		if (args[1]==NULL){
			result=chdir(homeName);
		} else {
			result = chdir(args[1]);
		}
		if (result != 0){
			perror(NULL);
		}
	}else{
		child_pid = vfork();
		if (child_pid == -1){
			perror("There was an error in the process creation");
			_exit(1);
		}
		if (child_pid == 0) {
		//In child
			if (redir_stdout) {
			// Put new_stdout on file desc #1
				if (dup2(new_stdout, 1) == -1) {
					perror(NULL);
					_exit(127);
				}
				close(new_stdout); // Not needed anymore
			}

			if (redir_stderr) {
			// Put new_stdout on file desc #2
				if (dup2(new_stderr, 2) == -1) {
					perror(NULL);
					_exit(127);
				}
				close(new_stderr); // Not needed anymore
			}
			if (redir_stdin) {
			// Put new_stdout on file desc #0
				if (dup2(new_stdin, 0) == -1) {
					perror(NULL);
					_exit(127);
				}
				close(new_stdin); // Not needed anymore
			}
			if (execvp(args[0], args) ==-1){
       		     		perror(NULL);
				_exit(1);
			}
		_exit(0);
		}
		else { 
		//In parent
			if (redir_stdout) {
				close(new_stdout); // Not needed anymore
				redir_stdout = 0;
			}
			if (redir_stderr) {
				close(new_stderr); // Not needed anymore
				redir_stderr = 0;
			}
			if(!background){
				waitpid(child_pid,NULL,0);
				//Need to add stuff to redirect output
			}
		}
	}
	return 0;
	
}
void parseArgs(char* line){
	int i=1;
	char* temp = NULL;
	while((temp=strtok(NULL,"\n "))!=NULL&&(i<10)){
		if(!findAmpLamp(temp) &&!checkForRedirection(temp)){
			args[i] = temp;
			i++;
		}
	}
	args[i+1]=NULL;	

}

int findAmpLamp(char* arg){
	if(strcmp(arg,"&")==0){
		background=1;
		return 1;
	}else return 0;


}
