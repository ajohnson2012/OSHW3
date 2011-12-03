#ifndef MAIN_C
#define MAIN_C
void parseArgs(char* line);
int runcmd(char **cmd);
void clearArray(char** array, int size);
int checkForRedirection(int arglen);
int findAmpLamp(char* arg);
#endif
