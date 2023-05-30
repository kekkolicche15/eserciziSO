#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

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
    int pd,
        len,
        size;
    char first = 1;
    char *str;
    while(1){
        pd = open(argv[1], O_RDONLY);
        str = malloc(CHUNK);
        first = 1;
        while((len = read(pd, str, CHUNK))>0){
            if(first && strstr(str, "\nFINE\n"))
                goto end;
            printf("%s", str);
            first = 0;
        }
        free(str);
        close(pd);
    }
    end: free(str);
    close(pd);
    unlink(argv[1]);
    exit(EXIT_SUCCESS);
}