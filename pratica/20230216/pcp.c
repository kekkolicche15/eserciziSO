#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

void copychunk(char *filename1, char *filename2, int start, int end){
    FILE *f = fopen(filename1, "r");
    fseek(f, start, SEEK_SET);
    FILE *g = fopen(filename2, "r");
    fseek(g, start, SEEK_SET);
    for(int i=start; i<end; i++)
        putc(getc(f), g);
}

int main(int argc, char *argv[]){
    if(argc<3){
        printf("usage: ./pcmp <file1> <file2>");
        exit(EXIT_FAILURE);
    }
    FILE *f1 = fopen(argv[1], "r");
    FILE *f2 = fopen(argv[1], "w+");
    
    
    fseek(f1, 0, SEEK_END);
    int f1size = ftell(f1);


    fclose(f1);
    fclose(f2);

    pid_t pid[2];
    if((pid[0]=fork())){
        if((pid[1]=fork())){
            int status;
            wait(&status);
            wait(&status);            
        }
        else _exit(compare(argv[1], argv[2], f1size/2, f1size));
    }
    else _exit(compare(argv[1], argv[2], 0, f1size/2));
    exit(EXIT_SUCCESS);
}
