#ifndef MAIN_C
#define MAIN_C
void parseArgs(char* line);
int checkShellName(char * name, int len);
int runcmd(char **cmd);
void clearArray(char** array, int size);
int checkForRedirection(char* arg);
int findAmpLamp(char* arg);
void getArgsFromFile();
#endif
