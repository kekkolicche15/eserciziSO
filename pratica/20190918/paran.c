#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int name2vect(char *src, char *dest[], char sep){
    int destMIdx = 0;
    int destIdx = 0;
    int srcIdx = 0;
    dest[0] = malloc(1024);
    do{
        if(src[srcIdx]==sep){
            dest[destMIdx][destIdx]='\0';
            destMIdx++;
            dest[destMIdx] = malloc(1024);
            destIdx=0;
        }
        else
            dest[destMIdx][destIdx++] = src[srcIdx];
        srcIdx++;
    }while(src[srcIdx]!=0);
    dest[destMIdx++][++destIdx] = 0;
    dest[destMIdx] = NULL;
    return destMIdx;
}
void handler(int idx, int maxproc, int procnum, char* cmds[]){
    pid_t cp;
    for(int i=idx; i<procnum; i+=maxproc)
        if(!(cp=fork())){
            char *args[1024];
            name2vect(cmds[i], args, ' ');
            execvp(args[0], args);
        }
        else
            waitpid(cp, NULL, WUNTRACED);
    _exit(0);
}
int main(int argc, char*argv[]){
    if(argc<2)
        exit(EXIT_FAILURE);
    char *commands[1024];
    int cmd_idx = -1, maxpar = atoi(argv[1]);
    for(int i=1; i<argc; i++)
        if(i>1&&strcmp(argv[i], "//")){
            strcat(commands[cmd_idx], argv[i]);
            if(i+1<argc&&strcmp(argv[i+1], "//"))
                strcat(commands[cmd_idx], " ");
        }else
            commands[++cmd_idx] = malloc(1024);
    cmd_idx++;
    pid_t children[maxpar];
    for(int i=0; i<maxpar; i++)
        if(!(children[i]=fork()))
            handler(i, maxpar, cmd_idx, commands);
    for(int i=0; i<maxpar; i++)
        waitpid(children[i], NULL, WUNTRACED);
    exit(EXIT_SUCCESS);
}