#include <stdio.h>
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
char* lineHLDR;
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
		//lineHLDR=stdin;
		if (fgets(text, 1000, stdin)){
			if(strcmp(text, quit)==0){
				return 0;
			}	
			else{
				args[0]=strtok(text, "\n ");
				parseArgs(text);
				runcmd(args);	
			}
		}
	}
	return 0;
}
int checkShellName(char * name, int len){
	//Returns 0 if string is only spaces or tabs, 1 if it has valid chars
	int i;
	int flag=0;
	for (i=0; i < len; i++){
		if (name[i]!=' ' || name[i] != "\t"){
			flag=1;
		}
	}
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
	///	printf("stderr redirection turning on.\n");
		redir_stderr=1;
		fileLoc=stderrPtr+2;
//		printf("%s\n",fileLoc);
		new_stderr = open(fileLoc, O_WRONLY|O_CREAT|O_TRUNC,(mode_t)0644);

		if (new_stderr == -1) {
			// Open failed: error-handling here
			printf("idk what happened, shit.");
		}
		return 1;
	}
		
	if(stdinPtr !=NULL){
	//	printf("stdin redirection turning on.\n");
		redir_stdin=1;
		fileLoc=stdinPtr+1;
		//printf("file location is:%s\n",fileLoc);
		new_stdin = open(fileLoc, O_RDONLY);
		if(new_stdin==-1){
			printf("idk what happened, shit.");
		}
		return 1;
	}
	
	if(stdoutPtr !=NULL){
	//	printf("stdout redirection\n");
		redir_stdout=1;
		//printf("file to be used:%s", args[i][1]);
		fileLoc=stdoutPtr+1;
	//	printf("%s\n",fileLoc);
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
	int child_status;
    	char* errbuf;
	if (strcmp(args[0], "cd") == 0){
		int result;
		if (args[1]==NULL){
			result=chdir(homeName);
		} else {
			result = chdir(args[1]);
		}
		if (result != 0){
			//char *cwd;
			//getcwd(cwd, 1000));
			//printf("Directory changed\n");
	
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
		//printf("new_stdin is:%d\n",new_stdin);
		child_pid = vfork();

		if (child_pid == -1){
			printf("There was an error in the process creation");
			_exit(1);
		}
		if (child_pid == 0) {
		//In child
			if (redir_stdout) {
			// Put new_stdout on file desc #1
				if (dup2(new_stdout, 1) == -1) {

					_exit(127);
				}
				close(new_stdout); // Not needed anymore
			}

			if (redir_stderr) {
			// Put new_stdout on file desc #2
				if (dup2(new_stderr, 2) == -1) {
					_exit(127);
				}
				close(new_stderr); // Not needed anymore
			}
			if (redir_stdin) {
			// Put new_stdout on file desc #0
				//printf("In child process with stdin redir error, new_stdin=%d\n",new_stdin);
				//char* strng;	
				if (dup2(new_stdin, 0) == -1) {
					printf("SHIIITT");
					_exit(127);
				}
				close(new_stdin); // Not needed anymore
				//strng[0] = '\0';
				//read(strng,new_stdin,8);
				//printf("file has %s\n",strng);
				
			//	getArgsFromFile();
			}
			if (execvp(args[0], args) ==-1){
				sprintf(errbuf,"%s: child process id =%d",myShellName,child_pid);
       		     		perror(errbuf);
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
/*void getArgsFromFile(){
	/*	FILE*fp;
	char str[1000];
	fp = fopen (filename, "r" );
	if(fp==NULL){
		return -1;
	}
//	printf("getting args from file\n");
	char str[1];
	char ch;
	char command[1000];
	int count=0;


	while(fgets(command, 1000, stdin)){

	}	
}*/

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
	//	printf("Found that & in arg:%s\n",arg);
		background=1;
		return 1;
	}else return 0;


}
