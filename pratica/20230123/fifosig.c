#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>

#define CHUNK 1024

int main(int argc, char *argv[]){
    if(argc!=2){
        printf("usage: %s <path>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    if(mkfifo(argv[1], 0662)!=0){
        printf("pipe error");
        exit(EXIT_FAILURE);
    }
    int len,
        pid,
        signo;
    char first = 1;
    FILE *fin;
    while(1){
        FILE *fin = fopen(argv[1], "r");
        fscanf(fin, "%d %d\n", &pid, &signo);
        kill(pid, signo);
        fclose(fin);
    }
    unlink(argv[1]);
    exit(EXIT_SUCCESS);
}